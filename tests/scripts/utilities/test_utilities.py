import sys


def assert_response(actual_response_json_dict: dict, expected_response_json_dict: dict):
    for key in actual_response_json_dict.keys():
        if key in expected_response_json_dict:
            if expected_response_json_dict[key] != actual_response_json_dict[key]:
                print_error(f'Error, expected field value for key "{key}" is "{expected_response_json_dict[key]}", actual is "{actual_response_json_dict[key]}"')
                return False
        else:
            # Omit non-interesting response JSON fields
            pass

    return True


def print_error(message: str):
    print(message, file=sys.stderr)


def test_runner(func):
    def wrapper():
        if func():
            print("TEST PASSED")
        else:
            print_error("TEST FAILED")

    return wrapper
