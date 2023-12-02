from utilities.test_procedures import send_request_json_and_wait_for_response
from utilities.test_utilities import assert_response, test_runner


@test_runner
def test1():
    calculate_exchange_request = {
        "type": "calculate_exchange_request",
        "source_currency": "eur",
        "target_currency": "usd",
        "money_amount": "1",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    expected_calculate_exchange_response = {
        "type": "calculate_exchange_response",
        "calculation_result": "1.07",
        "status": "OK",
        "failure_reason": "NULL",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    response_json_dict = send_request_json_and_wait_for_response(calculate_exchange_request)

    return assert_response(response_json_dict, expected_calculate_exchange_response)


@test_runner
def test2():
    calculate_exchange_request = {
        "type": "calculate_exchange_request",
        "source_currency": "eur",
        "target_currency": "usd",
        "money_amount": "2.53",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    expected_calculate_exchange_response = {
        "type": "calculate_exchange_response",
        "calculation_result": "2.72",
        "status": "OK",
        "failure_reason": "NULL",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    response_json_dict = send_request_json_and_wait_for_response(calculate_exchange_request)

    return assert_response(response_json_dict, expected_calculate_exchange_response)


@test_runner
def test3():
    calculate_exchange_request = {
        "type": "calculate_exchange_request",
        "source_currency": "eur",
        "target_currency": "usd",
        "money_amount": "0",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    expected_calculate_exchange_response = {
        "type": "calculate_exchange_response",
        "calculation_result": "0.00",
        "status": "OK",
        "failure_reason": "NULL",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    response_json_dict = send_request_json_and_wait_for_response(calculate_exchange_request)

    return assert_response(response_json_dict, expected_calculate_exchange_response)


@test_runner
def test4():
    calculate_exchange_request = {
        "type": "calculate_exchange_request",
        "source_currency": "eur",
        "target_currency": "usd",
        "money_amount": "0.051234",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    expected_calculate_exchange_response = {
        "type": "calculate_exchange_response",
        "calculation_result": "0.06",
        "status": "OK",
        "failure_reason": "NULL",
        "correlation_id": "a8eb01b5-b7f3-44bb-b3bb-f54486c8fd64"
    }

    response_json_dict = send_request_json_and_wait_for_response(calculate_exchange_request)

    return assert_response(response_json_dict, expected_calculate_exchange_response)


def test_calculate_exchange_request():
    test1()
    test2()
    test3()
    test4()
