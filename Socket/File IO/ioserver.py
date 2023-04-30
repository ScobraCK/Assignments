import socketserver as ss
import json, os, pathlib
from file_protocol import *

host = 'localhost'
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


class MyServer(ss.ThreadingMixIn, ss.TCPServer):
    daemon_threads = True
    allow_reuse_address = True

    def __init__(self, server_address, RequestHandlerClass):
        ss.TCPServer.__init__(self, server_address, RequestHandlerClass)

class MyHandler(ss.BaseRequestHandler):
    def handle(self) -> None:
        client_data = self.request.recv(4096)
        if not client_data:
            self.request.sendall(parse_data(400, None, None))

        status, data = read_data(client_data)  # status field resued for client request
        path = pathlib.Path(data['file'])
        print(self.client_address, end=' ')

        if status == 1:
            print('Request: Read file')    
            contents = read_file(path)
            self.request.sendall(parse_data(200, data['file'], contents))

        elif status == 2:
            print('Request: Write file')
            if data['contents']:
                if path.exists(): 
                    if data['overwrite']:  # overwrite file
                        contents = read_file(path)  # existing file contents
                        write_file(path, data['contents'])
                        self.request.sendall(parse_data(200, data['file'], contents)) 
                    else: # don't overwrite
                        self.request.sendall(parse_data(200)) # file = None
                else:  # new file made
                    write_file(path, data['contents'])
                    self.request.sendall(parse_data(200, data['file'])) # contents = None
            else:  # empty contents
                self.request.sendall(parse_data(400, None, 'Empty contents for write'))
        else:
            self.request.sendall(parse_data(501, None, f'Unknown request mode {status}'))  # Not implemented
        

if __name__ == "__main__":   
    with MyServer((host, port), MyHandler) as s:
        print(f'Server listening on port {port}')
        s.serve_forever()