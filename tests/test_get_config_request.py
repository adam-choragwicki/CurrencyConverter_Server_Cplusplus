# test_get_config_request.py
import requests
import re

CORRELATION_ID_HEADER = {"X-Correlation-ID": "test-correlation-id"}


def test_get_config_request_structure_and_currencies(server_base_url):
    url = f"{server_base_url}/config"
    r = requests.get(url, timeout=5, headers=CORRELATION_ID_HEADER)

    assert r.status_code == 200
    data = r.json()

    expected_keys = {
        "initial_source_currency",
        "initial_target_currency",
        "currencies_names_and_codes"
    }

    assert expected_keys.issubset(data.keys())

    currency_map = data["currencies_names_and_codes"]
    assert isinstance(currency_map, dict)
    assert len(currency_map) > 0

    for name, code in currency_map.items():
        assert isinstance(name, str)
        assert len(name) > 0

        assert isinstance(code, str)
        assert re.match(r"^[A-Z]{3}$", code), f"Invalid currency code format: {code}"

    all_codes = list(currency_map.values())
    assert data["initial_source_currency"].upper() in all_codes
    assert data["initial_target_currency"].upper() in all_codes
