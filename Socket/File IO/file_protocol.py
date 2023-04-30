import json
from typing import Tuple

def parse_data(status: int, file: str=None, contents: str=None, overwrite: bool=None) -> str:
    '''
    Custom 'protocol' made to send and recieve data for current objective

    Server:
        file is same as recieved data

        Read: return file data in contents
            contents = None if file does not exist
        Write: if file does not exist file is made and contents is None.
            if file exists and overwrite = True, contents = overwritten file contents
            else if overwrite = False, file = None(special case) to show
            that contents was not written.


    Client:
        Read: status = 1
            contents = None
        Write: status = 2
            contents is written
            overwrite flag in case file exists

    '''
    
    data = {
        'status': status, #status or read/write mode(client)
        'data': {
            'file': file,  # file path
            'contents': contents,  #file contents
            'overwrite': overwrite  # flag for file write
            }
        }
    return json.dumps(data, ensure_ascii=False).encode('utf-8')

def read_data(data: bytes) -> Tuple[int, dict[str, str, bool]]:
    '''returns status, data'''
    json_data = json.loads(data)
    return json_data['status'], json_data['data']
