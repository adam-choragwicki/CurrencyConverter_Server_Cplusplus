# test_status.py
import requests

CORRELATION_ID_HEADER = {"X-Correlation-ID": "test-correlation-id"}


def test_status_request(server_base_url):
    url = f"{server_base_url}/status"
    r = requests.get(url, timeout=5, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200
    data = r.json()
    assert isinstance(data, dict)
    assert data.get("message") == "status response from server"
    assert data.get("status") == "OK"


def test_status_request_missing_correlation_id(server_base_url):
    url = f"{server_base_url}/status"
    r = requests.get(url, timeout=5)
    assert r.status_code == 400
    data = r.json()
    assert "error" in data
