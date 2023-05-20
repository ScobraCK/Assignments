import socket
import json, os, pathlib
from file_protocol import *

host = ''
port = 12000

def read_file(path: pathlib.Path):
    if path.exists():
        with open(path, 'r',encoding='utf-8') as f:
            contents = f.read()
    else:
        contents = None
    return contents


def write_file(path: pathlib.Path, contents: str):
    with open(path, 'w',encoding='utf-8') as f:
        data = f.write(contents)

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
                    client_data = conn.recv(4096)
                    print(addr, end=' ')
                    if not client_data:  # get empty bytestring b''
                        print(f"Closing connection: {addr}")
                        break

                    try:  # check for keyerrors from incorrect format(dict)
                        status, data = read_data(client_data)  # status field resued for client request
                        path = pathlib.Path(data['file'])  # path for file
                        contents = data['contents']
                    except KeyError:
                        print('Error: Incorrect format')
                        conn.sendall(parse_data(20, contents='Incorrect format'))
                    
                    # only .txt file allowed
                    if path.suffix != '.txt':
                        conn.sendall(parse_data(11, contents='Only .txt filenames allowed'))

                    if status == 1: # read
                        print('Request: Read file')
                        if path.exists():  
                            read_contents = read_file(path)
                            conn.sendall(parse_data(1, contents=read_contents))
                        else:  # File not found: 12
                            conn.sendall(parse_data(12, contents='File not found'))
                    
                    elif status == 2:  # write
                        print('Request: Write file')
                        if path.exists():  # File exists: 13
                            conn.sendall(parse_data(13, contents='File exists'))
                        else:
                            write_file(path, contents)
                            conn.sendall(parse_data(1))
                    
                    elif status == 3: # force write
                        print('Request: Force write file')
                        if path.exists():  # send back contents of original file if exists
                            read_contents = read_file(path)
                            conn.sendall(parse_data(1, contents=read_contents))
                        else:
                            write_file(path, contents)
                            conn.sendall(parse_data(1))
                    else:
                        print('Error: Unknown status')  
                        conn.sendall(parse_data(20, contents='Unknown status'))