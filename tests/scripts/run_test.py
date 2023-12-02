from request_tests.get_config_request_test import test_get_config_request
from request_tests.calculate_exchange_request_test import test_calculate_exchange_request
from request_tests.update_cache_request_test import test_update_cache_request
from request_tests.custom_request_test import test_custom_request

if __name__ == "__main__":
    test_suites = [
        ("test_get_config_request", test_get_config_request),
        ("test_calculate_exchange_request", test_calculate_exchange_request),
        ("test_update_cache_request", test_update_cache_request),
        ("test_custom_request", test_custom_request),
    ]

    all_tests_passed = True

    for test_name, test in test_suites:
        try:
            print(f"Running '{test_name}'...")

            if test():
                print(f"'{test_name}' PASSED")
            else:
                print(f"'{test_name}' FAILED")
                all_tests_passed = False

        except AssertionError as e:
            print(f"'{test_name}' FAILED with AssertionError: {e}")
            all_tests_passed = False
        except Exception as e:
            print(f"'{test_name}' FAILED with Exception: {e}")
            all_tests_passed = False

        print()

    if all_tests_passed:
        print("All tests PASSED")
        exit(0)
    else:
        print("Some tests FAILED")
        exit(1)
