import socket, json
from file_protocol import *

host = 'localhost'
port = 12000

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print('Connected to server')

        end = False
        while not end:
            mode = int(input('Enter Mode(Read: 1, Write: 2, Force write: 3): '))
            file = input('Enter file name: ')
            contents = None
            if mode == 2 or mode == 3:
                contents = input('Input file contents: ')

            s.sendall(parse_data(mode, file, contents))
            server_data = s.recv(4096)  
            status, data = read_data(server_data)

            if status > 10:
                print(f"Error code {status}: {data['contents']}\n")
                retry = input('Redo request? (y, n): ')
                if not (retry.lower() == 'y' or retry.lower() == 'yes'):
                    end = True  # end connection without retrying
                        
            else:
                print('Success')
                if mode == 1:  # read      
                    print(f"File contents: {data['contents']}")

                # if file was overwritten in force write
                if mode == 3 and (original_contents := data.get(contents)): 
                    print(f"Overwritten file contents: {data['contents']}")
                end = True
        s.close()
    input()