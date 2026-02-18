# conftest.py
import os
import signal
import subprocess
import sys
import time
from pathlib import Path

import pytest
import requests
from typing import Optional

os.environ.setdefault("SERVER_STARTED_BY_PYTEST", "1")

DEFAULT_SERVER_BIN = Path(__file__).resolve().parent.parent / "bin" / "CurrencyConverterServer"
if sys.platform.startswith("win"):
    DEFAULT_SERVER_BIN = DEFAULT_SERVER_BIN.with_suffix(".exe")

# Environment override: useful in CI
SERVER_BIN = Path(os.environ.get("SERVER_BIN", str(DEFAULT_SERVER_BIN))).resolve()

# Server will be started with its working directory set to the binary parent,
# so that 'data/rates.json' and 'data/currencies.json' are discoverable.
SERVER_PORT = int(os.environ.get("SERVER_PORT", "8080"))
BASE_URL = f"http://127.0.0.1:{SERVER_PORT}"

# Timeout for server startup
STARTUP_TIMEOUT = 10.0
DEFAULT_CORRELATION_ID_HEADER = {"X-Correlation-ID": "test-correlation-id"}


def _env_truthy(value: Optional[str]) -> bool:
    if value is None:
        return False
    return value.strip().lower() not in ("0", "false", "no", "off", "")


def _is_executable(path: Path) -> bool:
    return path.exists() and os.access(path, os.X_OK)


def _wait_for_status(url: str, timeout: float = STARTUP_TIMEOUT):
    deadline = time.time() + timeout
    last_ex = None
    while time.time() < deadline:
        try:
            r = requests.get(url, timeout=1.0, headers=DEFAULT_CORRELATION_ID_HEADER)
            if r.status_code == 200:
                return True
        except Exception as ex:
            last_ex = ex
        time.sleep(0.2)
    raise RuntimeError(f"Server did not respond on {url} within {timeout}s. Last error: {last_ex}")


@pytest.fixture(scope="session")
def server_base_url():
    """
    Starts the server binary (if available) and yields the base url to be used by tests.
    The server is automatically terminated after the tests.
    """
    if not _is_executable(SERVER_BIN):
        pytest.skip(f"Server binary not found or not executable: {SERVER_BIN}")

    repo_root = Path(__file__).resolve().parent.parent
    bin_dir = repo_root / "bin"
    required_files = [bin_dir / "connection.json", bin_dir / "data" / "config.json"]
    missing = [str(p) for p in required_files if not p.exists()]
    if missing:
        pytest.fail(f"Server must run from bin/. Missing runtime files: {missing}")
    cwd = bin_dir
    env = os.environ.copy()
    env["PYTEST_LOGGING"] = "1"
    env["SERVER_WORKDIR"] = str(cwd)
    os.environ["SERVER_WORKDIR"] = str(cwd)
    env["CURRENCIES_NAMES_AND_CODES_FILE_PATH"] = str(repo_root / "data" / "currencies_names_and_codes.json")
    env["CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH"] = str(repo_root / "data" / "currencies_exchange_rate_cache")
    env.setdefault("UPDATE_CACHE_TEST_DELAY_MS", "200")
    env["SERVER_STARTED_BY_PYTEST"] = "1"

    # Default to local cache data to keep tests deterministic. Override with PYTEST_USE_LOCAL_DATA=0
    use_local_setting = os.environ.get("PYTEST_USE_LOCAL_DATA")
    if use_local_setting is None:
        use_local_setting = os.environ.get("UPDATE_CACHE_USE_LOCAL_DATA", "1")

    if _env_truthy(use_local_setting):
        env["UPDATE_CACHE_USE_LOCAL_DATA"] = "1"
        env.setdefault(
            "UPDATE_CACHE_LOCAL_DIR",
            str(repo_root / "data" / "currencies_exchange_rate_cache"),
        )
    else:
        env.pop("UPDATE_CACHE_USE_LOCAL_DATA", None)
        env.pop("UPDATE_CACHE_LOCAL_DIR", None)

    # Start the server
    # Use creationflags on Windows to allow killing child process; subprocess.terminate() works.
    creationflags = 0
    if sys.platform.startswith("win"):
        # CREATE_NEW_PROCESS_GROUP might be used; we'll still call terminate()
        creationflags = subprocess.CREATE_NEW_PROCESS_GROUP

    proc = subprocess.Popen([str(SERVER_BIN)], cwd=str(cwd), env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            creationflags=creationflags)

    try:
        # Wait until /status responds
        status_url = f"{BASE_URL}/status"
        _wait_for_status(status_url)

        # yield the base url for tests
        yield BASE_URL

    finally:
        # terminate process
        try:
            if proc.poll() is None:
                # On Windows, use proc.terminate(); on POSIX send SIGINT then terminate if needed
                if sys.platform.startswith("win"):
                    proc.terminate()
                else:
                    proc.send_signal(signal.SIGINT)
                    try:
                        proc.wait(timeout=3)
                    except subprocess.TimeoutExpired:
                        proc.terminate()
                # give it a moment
                try:
                    proc.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    proc.kill()
        except Exception:
            pass

        # collect and print server stdout/stderr for debugging (helpful in CI)
        try:
            out, err = proc.communicate(timeout=1)
            out = out.decode(errors="ignore") if isinstance(out, (bytes, bytearray)) else str(out)
            err = err.decode(errors="ignore") if isinstance(err, (bytes, bytearray)) else str(err)
            if out.strip():
                print("=== server stdout ===")
                print(out)
            if err.strip():
                print("=== server stderr ===")
                print(err)
        except Exception:
            pass
