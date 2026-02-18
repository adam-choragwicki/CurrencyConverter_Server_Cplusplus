import json
import os
from pathlib import Path

import pytest
import requests

CORRELATION_ID_HEADER = {"X-Correlation-ID": "live-download-test"}


def _live_download_enabled() -> bool:
    value = os.environ.get("RUN_LIVE_DOWNLOAD_TEST", "")
    return value.strip().lower() in ("1", "true", "yes", "on")


pytestmark = pytest.mark.skipif(
    not _live_download_enabled(),
    reason="Live download test disabled. Set RUN_LIVE_DOWNLOAD_TEST=1 to enable.",
)


def test_live_download_from_floatrates(server_base_url):
    url = f"{server_base_url}/update"
    r = requests.get(url, timeout=180, headers=CORRELATION_ID_HEADER)
    assert r.status_code == 200
    data = r.json()
    assert data.get("status") == "OK"

    base_dir = Path(os.environ.get("SERVER_WORKDIR", Path(__file__).resolve().parent.parent))
    download_dir = base_dir / "downloaded_data"
    for code in ("eur", "usd", "pln"):
        data_path = download_dir / f"{code}.json"
        assert data_path.exists(), f"Missing downloaded exchange rates file: {data_path}"
        content = data_path.read_text(encoding="utf-8", errors="ignore")
        payload = json.loads(content)
        assert isinstance(payload, dict)
        assert len(payload) > 0
        sample = next(iter(payload.values()))
        assert isinstance(sample, dict)
        assert "rate" in sample, f"Missing rate for {code}"
        assert "date" in sample, f"Missing date for {code}"
