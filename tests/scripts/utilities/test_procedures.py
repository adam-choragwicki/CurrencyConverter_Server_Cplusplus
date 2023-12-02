import sys
import json
import config
from utilities.client_socket_handler import ClientSocketHandler
from utilities.test_utilities import print_error


def send_request_json_and_wait_for_response(request_data: dict):
    message = json.dumps(request_data)

    try:
        client_socket_handler = ClientSocketHandler(config.SERVER_IP, config.SERVER_PORT)
        client_socket_handler.send_message(message)

        server_response = client_socket_handler.receive_message()

        # print(f"Server responded with {len(server_response)} bytes:")
        # print(server_response)

        try:
            response_json_dict = json.loads(server_response)
        except json.JSONDecodeError:
            print_error("JSON decoding error occurred. Server's response is not a valid JSON")
            return

        return response_json_dict

    except ConnectionRefusedError as e:
        print_error(f"Error, cannot connect to server: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")


def send_request_json(request_data: dict):
    message = json.dumps(request_data)

    try:
        client_socket_handler = ClientSocketHandler(config.SERVER_IP, config.SERVER_PORT)
        client_socket_handler.send_message(message)

    except ConnectionRefusedError as e:
        print_error(f"Error, cannot connect to server: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")


def send_request_custom_string(message: str):
    try:
        client_socket_handler = ClientSocketHandler(config.SERVER_IP, config.SERVER_PORT)
        client_socket_handler.send_message(message)

    except ConnectionRefusedError as e:
        print_error(f"Error, cannot connect to server: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
