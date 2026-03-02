import socket
from command_processor import proccess_command

HOST = "127.0.0.1"
PORT = 65432

def start_brain():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(1)

    print("Brain Iniciando..")
    conn, addr = server.accept()

    while True:
        data = conn.recv(1024).decode()

        if not data:
            break

        response = proccess_command(data)

        conn.send(response.encode())

        if response == "EXIT":
            break

    conn.close()
    server.close()
if __name__ == "__main__":
    start_brain()
        