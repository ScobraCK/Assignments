import socket

host = 'localhost'
port = 12000

def to_ascii(msg):
    return str([ord(c) for c in msg]).encode('utf-8')

if __name__ == "__main__":   
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()

        print(f'Server listening on port {port}')
        while True:
            conn, addr = s.accept()
            print(f'Connected by {addr}')
            with conn:  # context manager to ensure exit
                while True:                
                    data = conn.recv(1024)
                    print(f"Received from {addr}: {data}")
                    conn.sendall(to_ascii(data))
                    break
