# test_update.py
import json
import os
import threading
from pathlib import Path

import requests

CORRELATION_ID_HEADER = {"X-Correlation-ID": "test-correlation-id"}
UPDATE_CACHE_ALREADY_IN_PROGRESS = "UPDATE_CACHE_ALREADY_IN_PROGRESS"


def test_update_cache_request_success(server_base_url):
    url = f"{server_base_url}/update"
    r = requests.get(url, timeout=30, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200
    data = r.json()
    assert data.get("status") == "OK"

    base_dir = Path(os.environ.get("SERVER_WORKDIR", Path(__file__).resolve().parent.parent))
    download_dir = base_dir / "downloaded_data"
    for code in ("eur", "usd", "pln"):
        data_path = download_dir / f"{code}.json"
        assert data_path.exists(), f"Missing downloaded exchange rates file: {data_path}"
        content = data_path.read_text(encoding="utf-8", errors="ignore")
        data = json.loads(content)
        assert isinstance(data, dict)
        assert len(data) > 0
        for target_code, entry in data.items():
            assert isinstance(entry, dict)
            assert entry.get("date"), f"Missing date for {code}->{target_code}"
            assert entry.get("rate") is not None, f"Missing rate for {code}->{target_code}"


def test_update_cache_request_extra_parameters(server_base_url):
    url = f"{server_base_url}/update"
    r = requests.get(url, params={"extra": "nope"}, timeout=30, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 400


def test_update_cache_request_simultaneous_one_denied(server_base_url):
    url = f"{server_base_url}/update"
    barrier = threading.Barrier(2)
    results = []
    lock = threading.Lock()

    def send_request():
        barrier.wait()
        r = requests.get(url, timeout=30, headers=CORRELATION_ID_HEADER)
        data = r.json()
        with lock:
            results.append((r.status_code, data))

    t1 = threading.Thread(target=send_request)
    t2 = threading.Thread(target=send_request)
    t1.start()
    t2.start()
    t1.join()
    t2.join()

    status_codes = [status for status, _ in results]
    assert 200 in status_codes
    assert 409 in status_codes

    error_payload = next((payload for status, payload in results if status == 409), None)
    assert error_payload is not None
    assert error_payload.get("error_code") == UPDATE_CACHE_ALREADY_IN_PROGRESS
