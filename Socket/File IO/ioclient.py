import socket, json
from file_protocol import *

host = 'localhost'
port = 12000

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print('Connected to server')

        mode = int(input('Enter Mode(Read: 1, Write: 2): '))
        file = input('Enter file name: ')
        contents = None
        overwrite = None
        if mode == 2:
            contents = input('Input file contents: ')
            overwrite_input = input('Overwrite if file exists? (y for True)')
            overwrite = (True if overwrite_input == 'y' else False)

        s.sendall(parse_data(mode, file, contents, overwrite))
        server_data = s.recv(4096)
        status, data = read_data(server_data)

        if status != 200:
            print(f"An error occured: {data['contents']}")
        else:
            if mode == 1:  # read      
                if data['contents']:
                    print(f'Filename: {file}')
                    print(f"Contents: data['contents']")
                else:
                    print('File does not exist')
            else:
                if data['file']:  
                    if data['contents']:  # data overwritten
                        print('Existing file was overwritten')
                        print(f"Original contents: {data['contents']}")
                    else: # new file made
                        print(f'New file {file} made')
                else: # file exists but was not overwritten
                    print('File exists and was not overwritten')
                    
        s.close()

    input()