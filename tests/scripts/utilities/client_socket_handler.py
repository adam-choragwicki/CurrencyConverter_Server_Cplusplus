import socket


class ClientSocketHandler:
    def __init__(self, server_address, server_port):
        self.server_address = server_address
        self.server_port = server_port

        server_address = (self.server_address, self.server_port)
        self.socket_object = socket.create_connection(server_address)

    def send_message(self, message):
        # Append new line as message separator
        message += "\n"
        self.socket_object.sendall(message.encode())

    def receive_message(self):
        message = ""

        while True:
            message_chunk = self.socket_object.recv(1024).decode()
            message += message_chunk

            if '\n' in message:
                return message
