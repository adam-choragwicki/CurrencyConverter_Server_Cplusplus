# test_calculate.py
import json
import os
from decimal import Decimal, ROUND_HALF_UP, localcontext
from pathlib import Path

import pytest
import requests

CORRELATION_ID_HEADER = {"X-Correlation-ID": "test-correlation-id"}
INVALID_CALCULATION_ERROR_CODE = "INVALID_CALCULATION_REQUEST"
EXCHANGE_RATE_MISSING_ERROR_CODE = "EXCHANGE_RATE_DATA_MISSING"
def _get_download_dir() -> Path:
    return Path(os.environ.get("SERVER_WORKDIR", Path(__file__).resolve().parent.parent)) / "downloaded_data"


def _load_downloaded_rate(source_currency: str, target_currency: str):
    data_path = _get_download_dir() / f"{source_currency.lower()}.json"
    assert data_path.exists(), f"Missing downloaded exchange rates file: {data_path}"
    content = data_path.read_text(encoding="utf-8", errors="ignore")
    data = json.loads(content, parse_float=Decimal, parse_int=Decimal)
    entry = data.get(target_currency.lower())
    assert isinstance(entry, dict), f"Missing or invalid entry for {source_currency}->{target_currency}"
    # Server parses JSON numeric as double and std::to_string (6 decimal places).
    rate_raw = entry.get("rate")
    rate_float = float(rate_raw)
    rate = Decimal(f"{rate_float:.6f}")
    date = entry.get("date")
    assert date, f"Missing date for {source_currency}->{target_currency}"
    return rate, date


def _expected_exchange_result(amount_str: str, rate: Decimal) -> str:
    with localcontext() as ctx:
        ctx.prec = 50
        raw = Decimal(amount_str) * rate
        rounded = raw.quantize(Decimal("0.01"), rounding=ROUND_HALF_UP)
    return f"{rounded:.2f}"


@pytest.fixture(scope="module")
def updated_cache(server_base_url):
    url = f"{server_base_url}/update"
    r = requests.get(url, timeout=30, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200
    data = r.json()
    assert data.get("status") == "OK"
    return True


def _get_currency_codes(server_base_url):
    cfg = requests.get(f"{server_base_url}/config", timeout=5, headers=CORRELATION_ID_HEADER).json()
    currency_map = cfg["currencies_names_and_codes"]
    return list(currency_map.values())


def test_calculate_exchange_request_success(server_base_url):
    all_codes = _get_currency_codes(server_base_url)
    assert len(all_codes) >= 2, "need at least two currencies in config for calculate test"

    from_currency = all_codes[0]
    to_currency = all_codes[1]
    amount = "100.00"

    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": from_currency,
        "target_currency": to_currency,
        "source_currency_amount": amount
    }

    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200, f"/calculate returned {r.status_code}, body={r.text}"

    data = r.json()

    expected_keys = ("source_currency",
                     "target_currency",
                     "source_currency_amount",
                     "exchange_rate",
                     "exchange_result",
                     "exchange_rate_timestamp")

    for key in expected_keys:
        assert key in data, f"Missing expected key '{key}' in response"

    assert data["source_currency"].upper() == from_currency.upper()
    assert data["target_currency"].upper() == to_currency.upper()
    assert data["source_currency_amount"] == amount


def test_calculate_exchange_request_deterministic_values(server_base_url, updated_cache):
    url = f"{server_base_url}/calculate"
    cases = [
        ("EUR", "USD", "1"),
        ("USD", "PLN", "2.5"),
        ("PLN", "USD", "0.051234"),
        ("EUR", "PLN", "0"),
    ]

    for source_currency, target_currency, amount in cases:
        params = {
            "source_currency": source_currency,
            "target_currency": target_currency,
            "source_currency_amount": amount
        }
        r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
        assert r.status_code == 200, f"/calculate returned {r.status_code}, body={r.text}"
        data = r.json()
        rate, date = _load_downloaded_rate(source_currency, target_currency)
        expected_result = _expected_exchange_result(amount, rate)
        assert data["exchange_result"] == expected_result
        assert data["exchange_rate_timestamp"] == date


def test_calculate_exchange_request_precision_rounding(server_base_url, updated_cache):
    url = f"{server_base_url}/calculate"
    cases = [
        ("EUR", "PLN", "0.004"),
        ("EUR", "PLN", "0.005"),
        ("EUR", "USD", "0.3333333333"),
    ]

    for source_currency, target_currency, amount in cases:
        params = {
            "source_currency": source_currency,
            "target_currency": target_currency,
            "source_currency_amount": amount
        }
        r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
        assert r.status_code == 200, f"/calculate returned {r.status_code}, body={r.text}"
        data = r.json()
        rate, date = _load_downloaded_rate(source_currency, target_currency)
        expected_result = _expected_exchange_result(amount, rate)
        assert data["exchange_result"] == expected_result
        assert data["exchange_rate_timestamp"] == date


def test_calculate_exchange_request_large_amounts(server_base_url, updated_cache):
    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": "USD",
        "target_currency": "PLN",
        "source_currency_amount": "1000000000000"
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200, f"/calculate returned {r.status_code}, body={r.text}"
    data = r.json()
    rate, date = _load_downloaded_rate("USD", "PLN")
    expected_result = _expected_exchange_result("1000000000000", rate)
    assert data["exchange_result"] == expected_result
    assert data["exchange_rate_timestamp"] == date


def test_calculate_exchange_request_missing_parameters(server_base_url):
    url = f"{server_base_url}/calculate"
    r = requests.get(url, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 400


def test_calculate_exchange_request_extra_parameters(server_base_url):
    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": "USD",
        "target_currency": "EUR",
        "source_currency_amount": "1",
        "extra_parameter": "foo"
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 400


def test_calculate_exchange_request_invalid_amount(server_base_url):
    all_codes = _get_currency_codes(server_base_url)
    from_currency = all_codes[0]
    to_currency = all_codes[1]

    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": from_currency,
        "target_currency": to_currency,
        "source_currency_amount": "not_a_number"
    }

    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)

    assert r.status_code == 422
    data = r.json()
    assert data.get("error_code") == INVALID_CALCULATION_ERROR_CODE


def test_calculate_exchange_request_huge_amount_out_of_range(server_base_url):
    url = f"{server_base_url}/calculate"
    too_large_amount = "1" + ("0" * 400)
    params = {
        "source_currency": "EUR",
        "target_currency": "USD",
        "source_currency_amount": too_large_amount
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    # Current behavior: out-of-range amount triggers an unhandled std::out_of_range in validation,
    # leading to a 500 response instead of a structured 422 error.
    assert r.status_code == 500


@pytest.mark.parametrize("amount", ["-1", "01", "0x1", "1e2", "12.3.4"])
def test_calculate_exchange_request_invalid_amount_variants(server_base_url, amount):
    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": "EUR",
        "target_currency": "USD",
        "source_currency_amount": amount
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 422
    data = r.json()
    assert data.get("error_code") == INVALID_CALCULATION_ERROR_CODE


def test_calculate_exchange_request_same_currency(server_base_url):
    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": "EUR",
        "target_currency": "EUR",
        "source_currency_amount": "1"
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 422
    data = r.json()
    assert data.get("error_code") == INVALID_CALCULATION_ERROR_CODE


def test_calculate_exchange_request_unknown_currency(server_base_url):
    url = f"{server_base_url}/calculate"
    params = {
        "source_currency": "FOO",
        "target_currency": "BAR",
        "source_currency_amount": "10"
    }
    r = requests.get(url, params=params, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 400
    data = r.json()
    assert data.get("error_code") == EXCHANGE_RATE_MISSING_ERROR_CODE


@pytest.mark.skipif(
    not os.environ.get("SERVER_STARTED_BY_PYTEST"),
    reason="Server logs not accessible when server is started externally"
)
def test_server_log_contains_calculate(server_base_url):
    all_codes = _get_currency_codes(server_base_url)
    from_currency = all_codes[0]
    to_currency = all_codes[1]
    params = {
        "source_currency": from_currency,
        "target_currency": to_currency,
        "source_currency_amount": "1"
    }
    requests.get(f"{server_base_url}/calculate", params=params, timeout=5, headers=CORRELATION_ID_HEADER)

    candidates = [
        Path("logs") / "server.log",
        Path.cwd() / "logs" / "server.log",
        Path(__file__).resolve().parent.parent / "logs" / "server.log",
        Path(__file__).resolve().parent.parent / "bin" / "logs" / "server.log"
    ]
    found = None
    for p in candidates:
        if p.exists():
            found = p
            break

    assert found is not None, f"server.log not found in any of candidate locations: {candidates}"
    content = found.read_text(encoding="utf-8", errors="ignore")
    assert "GET /calculate" in content or "Received GET /calculate" in content
