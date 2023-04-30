import socket

host = 'localhost'
port = 12000

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print('Connected to server')

        message = input('Enter message: ')
        s.sendall(message.encode('utf-8'))
        data = s.recv(1024)
        print(f"Received from server: {data.decode('utf-8')}")