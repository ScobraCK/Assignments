'''
DFA assignment

Σ = {a, b}
(a+b)*bbb(a+b)*
'''

from typing import Dict

def get_reverse_equals(my_dict: dict):
    reverse_dict = {}
    for index, my_set in my_dict.items():
        for element in my_set:
            reverse_dict[element] = index
    return reverse_dict

def find_equal_state(equals: dict, equalstates: dict, state: int):
    '''
    state와 입력 결과가 같은 동치류가 있는지 찾는다.
    '''
    for k, v in equals.items():
        if equalstates[list(v)[0]] == equalstates[state]:  # compare with first item
            return k
    
    return None

class DFA():
    '''
    DFA implementation by automata
    states: set of states(Q)
    sigma: set of input symbols
    start: start state
    final: set of final states
    table: dictionary(table) for (state, symbol): next_state
    '''
    def __init__(self, states: set, sigma: set, start, final: set, table: dict) -> None:
        self.states = states
        self.sigma = sigma
        self.start = start
        self.final = final
        self.table = table
        
        self.current_state = self.start

    def print_DFA(self):
        print(f'Q = {sorted(self.states)}')
        print(f'Σ = {sorted(self.sigma)}')
        print(f'q0 = {self.start}')
        print(f'F = {sorted(self.final)}')
        print('δ:')
        for k, v in self.table.items():
            print(f'{k} = {v}')
        print('\n')

    def next_state(self, state, symbol):
        if symbol not in self.sigma:
            raise ValueError(f'Error: symbol {symbol} not in sigma')
        
        next_state = self.table.get((state, symbol))

        if next_state in self.states:
            return next_state
        elif next_state is None:
            raise KeyError(f"({state}, '{symbol}') not found in transition table")
        else:
            raise KeyError(f"{next_state}' not found in states")
    
    def evaluate(self, input: str):
        self.current_state = self.start  # reset state
        try:
            for symbol in input:
                self.current_state = self.next_state(self.current_state, symbol)
        except ValueError as e:
            print(e)
            return False

        if self.current_state in self.final:
            return True
        else:
            return False
        
    def reachable_states(self):
        reachable = set()  # set of reachable states
        state_queue = [self.start]  # queue of states to search

        while state_queue:
            current = state_queue.pop(0)
            # already checked state
            if current in reachable: 
                continue
            
            # add next states
            for symbol in self.sigma:
                next_state = self.next_state(current, symbol)
                state_queue.append(next_state)
            
            reachable.add(current)  
        return reachable

    def optimize(self):
        # remove unreachable states
        reachable = self.reachable_states()
        print(
            f'Step 1\n'
            f'Initial States: {self.states}\n'
            f'Reachable States: {reachable}\n'
            f'Removed States: {self.states - reachable}\n'
            )
        self.states = reachable

        # Equivalent Name: Equivalent State
        equals = {
            1: self.states - self.final,  # non final states
            2: self.final
        }
        finals = set([2])  # final Equivalent State

        reverse_equals = get_reverse_equals(equals)  # to find equal from state
        print('Step 2')
        loop = 1
        while(True):
            # calculate equivalents
            equalstates = dict()  # Equivalent Name: (result Equivalent State for each symbol)
            
            new_equals: Dict[int, set] = dict()  # new Equivalent States
            new_finals = set()
            new_equal = 0  # Equivalent Name (counting from 0)

            print(f'Loop {loop}')
            loop += 1

            for equal, states in equals.items():
                print(f'Equivalent {equal}')
                symbol_text = "\t".join(sorted(list(self.sigma)))
                print(f'State\t{symbol_text}')

                for state in sorted(states):
                    next_equals = []
                    for symbol in sorted(self.sigma):
                        next_state = self.next_state(state, symbol)
                        next_equals.append(reverse_equals.get(next_state))
                    equalstates[state] = tuple(next_equals)
                    
                    # add to new equalstate
                    if (has_equal := find_equal_state(new_equals, equalstates, state)):
                        new_equals[has_equal].add(state)  # add to existing
                    else:
                        new_equal += 1
                        new_equals[new_equal] = set([state])  # add new equivalent
                        if equal in finals:  # Equivalent State is also final state
                            new_finals.add(new_equal)  
                    
                    # print process
                    symbols = "\t".join(map(str, next_equals))
                    print(f'{state}\t{symbols}')
            print('\n')

            # no change, found final
            if equals == new_equals and finals == new_finals:
                # Update table
                self.states = set(equals.keys())
                self.start = 1  # fixed
                self.final = finals
                self.table = dict()
                for equal, states in equals.items():
                    state = list(states)[0]  # Any Equivalent State (all are same result)
                    next_states = equalstates[state]  # Tuple of (result Equivalent State for each symbol):from line 122
                    for symbol, next_state in zip(self.sigma, next_states):
                        self.table[(equal, symbol)] = next_state        
                break
            else:
                equals = new_equals
                finals = new_finals
                reverse_equals = get_reverse_equals(equals)

        print(f'Minimized DFA:')
        self.print_DFA()
    
if __name__ == "__main__":
    states = {'A', 'B', 'C', 'D'}
    sigma = {'a', 'b'}
    start = 'A'
    final = {'D'}
    table = {
        ('A', 'a'): 'A',
        ('A', 'b'): 'B',
        ('B', 'a'): 'A',
        ('B', 'b'): 'C',
        ('C', 'a'): 'A',
        ('C', 'b'): 'D',
        ('D', 'a'): 'D',
        ('D', 'b'): 'D',
    }

    test = [
        '',
        'a',
        'bbb',
        'aba',
        'bbbbbbbb',
        'abababababbb',
        'bbbaaa',
        'baababababbabababaaababbabab',
        'ababbabbbaX'
    ]

    minimize_states = {'A', 'B', 'C', 'D', 'E', 'F'}
    minimize_sigma = {'0', '1'}
    minimize_start = 'A'
    minimize_final = {'E', 'F'}
    minimize_table = {
        ('A', '0'): 'B',
        ('A', '1'): 'E',
        ('B', '0'): 'A',
        ('B', '1'): 'E',
        ('C', '0'): 'B',
        ('C', '1'): 'D',
        ('D', '0'): 'E',
        ('D', '1'): 'E',
        ('E', '0'): 'F',
        ('E', '1'): 'F',
        ('F', '0'): 'F',
        ('F', '1'): 'F',
    }


    dfa = DFA(states, sigma, start, final, table)
    dfa.print_DFA()
    dfa.optimize()  # test
    
    for testcase in test:
        result = dfa.evaluate(testcase)
        print(f'{testcase}: {result}')
    print('\n')

    dfa2 = DFA(minimize_states, minimize_sigma, minimize_start, minimize_final, minimize_table)
    print("Original DFA:")
    dfa2.print_DFA()
    dfa2.optimize()