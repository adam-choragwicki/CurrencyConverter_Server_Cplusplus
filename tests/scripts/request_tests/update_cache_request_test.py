from utilities.test_procedures import send_request_json_and_wait_for_response
from utilities.test_utilities import assert_response, test_runner


@test_runner
def test1():
    update_cache_request = {"type": "update_cache_request", "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"}

    expected_update_cache_response = {
        "type": "update_cache_response",
        "status": "OK",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64",
        # "new_exchange_rates_timestamp": "variable"
    }

    # triggers currencies exchange rate databank update, takes some time
    response_json_dict = send_request_json_and_wait_for_response(update_cache_request)

    return assert_response(response_json_dict, expected_update_cache_response)


def test_update_cache_request():
    return test1()
