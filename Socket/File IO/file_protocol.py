import json
from typing import Tuple

def parse_data(status: int, file: str=None, contents: str=None) -> str:
    '''
    Custom 'protocol' made to send and recieve data for current objective
    
    Status Flags
    Client
    1: Read
    2: Write new file
    3: Force Write file (overwrite if exists)

    Server
    1: Success
    
    11: Failed: unsupported file type (only .txt)
    12: Failed Read: File not found 
    13: Failed Write: File exists (use force write to overwrite)

    20: Unknown Request
    '''
    
    data = {
        'status': status, #status or read/write mode(client)
        'data': {
            'file': file,  # file path
            'contents': contents,  #file contents
            }
        }
    return json.dumps(data, ensure_ascii=False).encode('utf-8')

def read_data(data) -> Tuple[int, dict[str, str, bool]]:
    '''returns status, data'''
    json_data = json.loads(data)
    return json_data['status'], json_data['data']
