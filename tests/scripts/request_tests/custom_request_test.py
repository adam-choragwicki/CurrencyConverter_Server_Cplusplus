from time import sleep
from utilities.test_procedures import send_request_json
from utilities.test_procedures import send_request_custom_string


def test_custom_request():
    incomplete_request1 = {"type": "calculate_exchange_request"}
    incomplete_request2 = {"type": "calculate_exchange_request", "source_currency": "eur"}
    incomplete_request3 = {"type": "calculate_exchange_request", "source_currency": "eur", "target_currency": "usd"}

    send_request_json(incomplete_request1)
    send_request_json(incomplete_request2)
    send_request_json(incomplete_request3)

    bad_request1 = ""
    bad_request2 = "{}"
    bad_request3 = '{"type": "NULL"}'
    bad_request4 = '{"type": "get_config_request"}'

    send_request_custom_string(bad_request1)
    send_request_custom_string(bad_request2)
    send_request_custom_string(bad_request3)
    send_request_custom_string(bad_request4)

    very_long_request = "{" + 500000 * "1" + "}"  # extreme case
    send_request_custom_string(very_long_request)
    sleep(2)

    send_request_json(incomplete_request1)
