class DFA():
    '''
    DFA implementation by automata (modified from SimpleDFA)
    states: set of states(Q)
    symbol_group: group symbols together to make transition table simpler
        Ex: [a-z] -> alpha
        symbol_group = {
            'a': 'alpha'
            'b': 'alpha'
            ...
        }
        sigma only contains 'alpha' instead of [a-z]
    start: start state
    final: set of final states
    table: dictionary(table) for (state, symbol): next_state

    Note symbol_group is used to make coding a DFA easier and is no way a real implamentation of a DFA.
    It only replicates how a DFA would work if one had made state transitions for every symbol inside the group manually.
    '''
    def __init__(self, symbol_group: dict, start, final: set, table: dict) -> None:
        self.symbol_group = symbol_group
        self.start = start
        self.final = final
        self.table = table
        
        self.current_state = self.start

    def next_state(self, state, symbol):        
        while symbol:
            next_state = self.table.get((state, symbol))  # None if not defined in table
            if next_state:
                return next_state

            symbol = self.symbol_group.get(symbol)  # try symbol group
            
        if next_state := self.table.get((state, 'any')):  # rule for all symbols
            return next_state
        return -1  # junk
    
    def evaluate(self, input: str):
        self.current_state = self.start  # reset state

        for symbol in input:
            self.current_state = self.next_state(self.current_state, symbol)

        if self.current_state in self.final:
            return self.current_state
        else:
            return -1


class Tokenizer(DFA):
    '''
    Uses DFA to read tokens
    '''
    def __init__(self, 
                 symbol_group: dict, 
                 start, 
                 final: set, 
                 table: dict, 
                 block_comment_char = '?') -> None:
        super().__init__(symbol_group, start, final, table)
        self.text = ''
        self.index = 0
        self.block_comment_char = block_comment_char
    
    def read_file(self, file):
        '''
        read from file
        '''
        with open(file, 'r', encoding='utf-8') as f:
            self.text = f.read()
            self.index = 0

    def read_text(self, text):
        '''
        read from string
        '''
        self.text = text
        self.index = 0

    def isEOF(self):
        return self.index >= len(self.text)
    
    def next_token(self):
        token = ''
        while not self.isEOF():
            if self.except_comment():
                raise IndexError("Invalid Block Comment")

            symbol = self.text[self.index]
            next_state = self.next_state(self.current_state, symbol)
            if next_state == -1:  # end of token
                if self.current_state == self.start:  # if first token is invalid
                    self.index += 1
                    token = symbol
                yield token, self.evaluate(token)
                self.current_state = self.start
                token = ''
            elif next_state == 'Literal':
                yield token, self.evaluate(token)  # ', 38  start of literal
                token = ''
                self.current_state = next_state  # 'Literal

                while not self.isEOF():
                    symbol = self.text[self.index]
                    next_state = self.next_state(self.current_state, symbol)
                    if next_state == 38:
                        yield token, 4  # literal
                        yield symbol, 38  # ', 38  end of literal
                        break
                    elif next_state == -2:  # special error for unfinished literal
                        yield token, -2
                        break
                    else:  # keep looping literal
                        self.current_state = next_state
                        token += symbol
                        self.index += 1
                # move cursor to after end of literal and reset
                self.index += 1  
                self.current_state = self.start
                token = ''
                
            else:
                self.index += 1
                self.current_state = next_state

            if self.current_state != self.start:  # ignore whitespace
                token += symbol
            
        if token:
            yield token, self.evaluate(token)
                
    def except_comment(self):
        while not self.isEOF():
            current_char: str = self.text[self.index]
            if current_char == '/':
                if (next_char := self.text[self.index+1]) == '/':  # line comment
                    self.index += 2
                    while not self.isEOF() and self.text[self.index] != '\n':  # move cursor until newline or EOF
                        self.index += 1
                    self.index += 1  # move cursor to next char
                elif next_char == self.block_comment_char:
                    self.index += 2
                    while self.text[self.index] != self.block_comment_char or self.text[self.index+1] != '/':  # move cursor until end of block
                        self.index += 1
                        if self.index + 1 >= len(self.text):
                            return True
                    self.index += 2  # move cursor to next char after block comment
            return False
        
