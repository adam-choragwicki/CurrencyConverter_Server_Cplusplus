from request_tests.get_config_request_test import test_get_config_request
from request_tests.calculate_exchange_request_test import test_calculate_exchange_request
from request_tests.update_cache_request_test import test_update_cache_request
from request_tests.custom_request_test import test_custom_request

if __name__ == "__main__":
    test_get_config_request()
    test_calculate_exchange_request()
    test_update_cache_request()
    test_custom_request()
