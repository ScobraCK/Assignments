import socket
from dns import *

host = 'localhost'
port = 12000

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print('Connected to server')

        s.sendall(parse_data(1, 'A', ip='1.1.1.1', dname='server.test'))
        raw_data = s.recv(1024)
        print(f"Received from server: {read_data(raw_data)}")
        s.close()
    input()