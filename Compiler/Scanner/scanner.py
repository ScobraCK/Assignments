from enum import Enum
from typing import Any, Dict, List, Tuple
from dfa import Tokenizer

class Identifier():
    def __init__(self, name, id) -> None:
        self.name = name
        self.id = id
        self.count = 0

    def inc_count(self):
        self.count += 1
    
    def __repr__(self) -> str:
        return f'{self.name:10} {self.id:<5} {self.count}'

class Scanner():
    class NumberSymbol(Enum):
        Zero = 'Zero'
        Dec = 'Dec'
        Oct = 'Oct'
        Hex = 'Hex'
        PreHex = 'PreHex' 
        PreFloat1 = 'PreFloat1' 
        PreFloat2 = 'PreFloat2'
        PreFloat3 = 'PreFloat3'
        Float = 'Float'
        Floate = 'Floate'  # e notation
        InvalidInt = 'InvalidInt'
        InvalidOct = 'InvalidOct'
        InvalidFloat = 'InvalidFloat'

    def __init__(self, optable, default_value='_') -> None:
        self.tokenizer = self.init_tokenizer(optable)
        self.symboltable = dict()
        self.optable = optable
        self.default_value = default_value

    def evaluate_numeric_token(self, token, tokenstate):
        if tokenstate == self.NumberSymbol.Zero:
            value = int(token)
            tokenstate = 5
        elif tokenstate == self.NumberSymbol.Dec:
            value = int(token)
            tokenstate = 5
        elif tokenstate == self.NumberSymbol.Oct:
            value = int(token, base=8)
            tokenstate = 5
        elif tokenstate == self.NumberSymbol.Hex:
            value = int(token, base=16)
            tokenstate = 5
        elif tokenstate == self.NumberSymbol.PreFloat1 or tokenstate == self.NumberSymbol.Float or tokenstate == self.NumberSymbol.Floate:
            value = float(token)
            tokenstate = 6 
        elif tokenstate == self.NumberSymbol.InvalidInt or tokenstate == self.NumberSymbol.PreHex:
            raise ValueError("Invalid integer token")
        elif tokenstate == self.NumberSymbol.InvalidOct:
            raise ValueError("Invalid octal token (8, 9)")
        else:
            raise ValueError("Invalid float token")
        
        return tokenstate, value


    def get_symboltable(self, token):
        '''
        Gets symboltable entry from symboltable if exist, else adds and returns new entry id
        '''
        identifier = self.symboltable.get(token)
        if not identifier:
            identifier = Identifier(token, len(self.symboltable)+1)
            self.symboltable[token] = identifier
        identifier.inc_count()
        return identifier.id, identifier.count

    def read_file(self, file):
        self.symboltable = dict()  # reset
        self.tokenizer.read_file(file)

    def read_text(self, text):
        self.symboltable = dict()  # reset
        self.tokenizer.read_text(text)

    def read_tokens(self)->Tuple[int, Any]:
        for token, tokenstate in self.tokenizer.next_token():
            msg = ''
            value = self.default_value
            # if Integer or Float
            if isinstance(tokenstate, self.NumberSymbol): 
                try:
                    tokenstate, value = self.evaluate_numeric_token(token, tokenstate)
                except ValueError as e:
                    print(f"Error: '{token}' {e}")
                    continue
            elif tokenstate == 'Keyword':
                tokenstate = self.optable.get(token)
                if not tokenstate:
                    print(f"Error: '{token}' Invalid Keyword")
                    continue
            elif tokenstate == 3:  # Id
                value, count = self.get_symboltable(token)
                msg += f' Count = {count}'
                # extra check
                idtoken = token[0].upper() + token[1:]
                if self.optable.get(idtoken):
                    msg += f'\nScanner: Did you mean {idtoken}?'
            elif tokenstate == -1:
                print(f"Error: '{token}' Unidentified Token")
                continue
            
            print(f'{token:10}: {f"({tokenstate}, {value})":10}{msg}')
        print('')

    def print_symboltable(self):
        print("Symbol Table")
        print(f"{'Symbol':10} {'Id':5} Count")
        for id in self.symboltable.values():
            print(id)
            
    def init_tokenizer(self, optable):
        # define symbol_groups
        symbol_group = {chr(i): 'l_alpha' for i in range(ord('a'), ord('z') + 1)}  # [a-z]: l_alpha
        symbol_group.update({chr(i): 'u_alpha' for i in range(ord('A'), ord('Z') + 1)})  # [A-Z]: u_alpha
        symbol_group.update({str(i): 'digit' for i in range(10)})  # [0-9]: digit
        symbol_group.update({i: 'whitespace' for i in [' ', '\n', '\t']})
        symbol_group.update({i: 'alnum' for i in ['l_alpha', 'u_alpha', 'digit', '_']})

        # optable -> transition table
        transition_table = dict()
        for k, v in sorted(optable.items(), key=lambda x: x[0]):  # sort by key
            current_state = "Initial"
            if k.isalpha():  # Keywords will be added separately
                continue
            for i, letter in enumerate(k):
                if transition_table.get((current_state, letter)):  # already in entry (Ex: '=' and '==')
                    current_state = transition_table.get((current_state, letter))
                else:
                    next_state = f"S{i + 1}_{k[:i + 1]}"
                    if i == len(k) - 1:
                        next_state = v
                    transition_table[(current_state, letter)] = next_state
                    current_state = next_state

        # Manual rules added with use of symbol_groups(see DFA)
        # add rules for Id
        transition_table[('Initial', 'l_alpha')] = 3
        transition_table[('Initial', '_')] = 3
        transition_table[(3, 'alnum')] = 3

        # add rules for Keyword
        transition_table[('Initial', 'u_alpha')] = 'Keyword'
        transition_table[('Keyword', 'alnum')] = 'Keyword'

        # add rules for Integers
        # Dec
        transition_table[('Initial', 'digit')] = self.NumberSymbol.Dec
        transition_table[(self.NumberSymbol.Dec, 'digit')] = self.NumberSymbol.Dec
        transition_table[(self.NumberSymbol.Dec, 'alnum')] = self.NumberSymbol.InvalidInt

        transition_table[(self.NumberSymbol.InvalidInt, 'alnum')] = self.NumberSymbol.InvalidInt
        
        # Oct
        transition_table[('Initial', '0')] = self.NumberSymbol.Zero  # '0' has priority over digit

        transition_table[self.NumberSymbol.Zero, 'digit'] = self.NumberSymbol.Oct
        transition_table[self.NumberSymbol.Zero, '8'] = self.NumberSymbol.InvalidOct  # [8-9] is invalid for Oct
        transition_table[self.NumberSymbol.Zero, '9'] = self.NumberSymbol.InvalidOct
        transition_table[(self.NumberSymbol.Zero, 'alnum')] = self.NumberSymbol.InvalidInt

        transition_table[self.NumberSymbol.Oct, 'digit'] = self.NumberSymbol.Oct
        transition_table[self.NumberSymbol.Oct, '8'] = self.NumberSymbol.InvalidOct
        transition_table[self.NumberSymbol.Oct, '9'] = self.NumberSymbol.InvalidOct
        transition_table[(self.NumberSymbol.Oct, 'alnum')] = self.NumberSymbol.InvalidInt

        transition_table[(self.NumberSymbol.InvalidOct, 'alnum')] = self.NumberSymbol.InvalidOct
        
        # Hex
        hex_chars = [chr(ord('a') + i) for i in range(6)] + [chr(ord('A') + i) for i in range(6)]  # [a-fA-F]
        transition_table[(self.NumberSymbol.Zero, 'x')] = self.NumberSymbol.PreHex 
        transition_table[(self.NumberSymbol.PreHex , 'digit')] = self.NumberSymbol.Hex
        transition_table.update({(self.NumberSymbol.PreHex , c): self.NumberSymbol.Hex for c in hex_chars})   # add all hex chars 
        transition_table[(self.NumberSymbol.PreHex , 'alnum')] = self.NumberSymbol.InvalidInt  # remaining chars are invalid

        transition_table[(self.NumberSymbol.Hex, 'digit')] = self.NumberSymbol.Hex
        transition_table.update({(self.NumberSymbol.Hex, c): self.NumberSymbol.Hex for c in hex_chars})
        transition_table[(self.NumberSymbol.Hex, 'alnum')] = self.NumberSymbol.InvalidInt

        # add rules for Floats
        # Float1: 0.12, 1.0...
        transition_table[(self.NumberSymbol.Zero, '.')] = self.NumberSymbol.PreFloat1
        transition_table[(self.NumberSymbol.Dec, '.')] = self.NumberSymbol.PreFloat1

        transition_table[(self.NumberSymbol.PreFloat1, 'digit')] = self.NumberSymbol.Float
        transition_table[(self.NumberSymbol.PreFloat1, 'alnum')] = self.NumberSymbol.InvalidFloat

        transition_table[(self.NumberSymbol.Float, 'digit')] = self.NumberSymbol.Float
        transition_table[(self.NumberSymbol.Float, 'alnum')] = self.NumberSymbol.InvalidFloat
        
        # Float2: 1e+2, 0.2e-1...
        transition_table[(self.NumberSymbol.Dec, 'e')] = self.NumberSymbol.PreFloat2
        transition_table[(self.NumberSymbol.Float, 'e')] = self.NumberSymbol.PreFloat2

        transition_table[(self.NumberSymbol.PreFloat2, '+')] = self.NumberSymbol.PreFloat3  # 1e+7
        transition_table[(self.NumberSymbol.PreFloat2, '-')] = self.NumberSymbol.PreFloat3  # 1e-7 
        transition_table[(self.NumberSymbol.PreFloat2, 'digit')] = self.NumberSymbol.Floate  # 1e7
        transition_table[(self.NumberSymbol.PreFloat2, 'alnum')] = self.NumberSymbol.InvalidFloat

        transition_table[(self.NumberSymbol.PreFloat3, 'digit')] = self.NumberSymbol.Floate
        transition_table[(self.NumberSymbol.PreFloat3, 'alnum')] = self.NumberSymbol.InvalidFloat

        transition_table[(self.NumberSymbol.Floate, 'digit')] = self.NumberSymbol.Floate
        transition_table[(self.NumberSymbol.Floate, 'alnum')] = self.NumberSymbol.InvalidFloat
        transition_table[(self.NumberSymbol.Floate, 'e')] = self.NumberSymbol.PreFloat2  # invalid e expression

        transition_table[(self.NumberSymbol.InvalidFloat, 'alnum')] = self.NumberSymbol.InvalidFloat
        transition_table[(self.NumberSymbol.InvalidFloat, 'e')] = self.NumberSymbol.PreFloat2  # invalid e expression

        # add rule for whitespace
        transition_table[('Initial', 'whitespace')] = 'Initial'

        # add rule for literal
        transition_table[optable["'"], 'any'] = 'Literal'
        transition_table['Literal', 'any'] = 'Literal'
        transition_table['Literal', '\n'] = -2  # error
        transition_table['Literal', "'"] = optable["'"]

        # get final set
        final=set(optable.values())
        final.add(3)  # id
        final.add('Keyword')
        final.update(set(self.NumberSymbol))  # Dec, Oct, Hex, Float1, Float2

        return Tokenizer(symbol_group=symbol_group, start='Initial', final=final, table=transition_table)
