# Automata and Compiler - DFA
Create a DFA code for the following R.E.

Σ = {a, b}<br>
(a+b)\*bbb(a+b)\*

## class DFA 
Class defining M = (Q, Σ, q0, F, δ)

```py
    def __init__(self, states: set, sigma: set, start, final: set, table: dict) -> None:
        self.states = states
        self.sigma = sigma
        self.start = start
        self.final = final
        self.table = table
        
        self.current_state = self.start
```

### next_state(state, symbol)
returns the next state according to the defined transition function

- raises ValueError if undefined symbol(input) detected (User error)
- raises KeyError when transition function is incomplete (Incomplete DFA)

```py
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
```

### evaluate()
Reads string char by char while updating current state.<br>
If resulting state is a final state returns true eles false. ValueError raised by undefined symbal also returns false.
```py
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
```
Example
```
: False
a: False
bbb: True
aba: False
bbbbbbbb: True
abababababbb: True
bbbaaa: True
baababababbabababaaababbabab: False
Error: symbol X not in sigma
ababbabbbaX: False
```


## DFA Minimize
### reachable_states()
returns a set of states reachable starting from q0. Uses a BFS algorithm.
```py
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
```

### optimize()
**Step 1**<br>
Removes unreachable states
```py
    # remove unreachable states
    reachable = self.reachable_states()
    print(
        f'Step 1\n'
        f'Initial States: {self.states}\n'
        f'Reachable States: {reachable}\n'
        f'Removed States: {self.states - reachable}\n'
        )
    self.states = reachable
```

**Step 2**<br>
First Equivalent States(동치류)
```py
# Equivalent Name: Equivalent State
    equals = {
        1: self.states - self.final,  # non final states
        2: self.final
    }
    finals = set([2])  # final Equivalent State

    reverse_equals = get_reverse_equals(equals)  # to find equal from state
```

Step 2 processes are repeated in a while loop until we get the final Equivalent States

**Step 2.1**<br>
Calculate next Equivalent States

```py
# calculate equivalents
        equalstates = dict()  # Equivalent Name: (result Equivalent State for each symbol)
        
        new_equals: Dict[int, set] = dict()  # new Equivalent States
        new_finals = set()
        new_equal = 0  # Equivalent Name (counting from 0)

        for equal, states in equals.items():
            print(f'Equivalent {equal}')
            symbol_text = "\t".join(list(self.sigma))
            print(f'State\t{symbol_text}')

            for state in sorted(states):
                next_equals = []
                for symbol in self.sigma:
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
```
**Example**
```
Loop 1
Equivalent 1
State   a       b
0       1       1
1       1       1
2       1       2
Equivalent 2
State   a       b
3       2       2


Loop 2
Equivalent 1
State   a       b
0       1       1
1       1       2
Equivalent 2
State   a       b
2       1       3
Equivalent 3
State   a       b
3       3       3
```
In loop 1 States 0,1 are the same(1, 1), however state 2 results in (1, 2) meaning it is split into a new Equivalent in the next loop. This repeats until no chage is found.


**Step 2.2**<br>
Update final Equivalents. The Equivalents become the new states and the transition table is updated accordingly.
```py
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
```


### Example
```
Original DFA:
Q = ['A', 'B', 'C', 'D', 'E', 'F']
Σ = ['0', '1']
q0 = A
F = ['E', 'F']
δ:
('A', '0') = B
('A', '1') = E
('B', '0') = A
('B', '1') = E
('C', '0') = B
('C', '1') = D
('D', '0') = E
('D', '1') = E
('E', '0') = F
('E', '1') = F
('F', '0') = F
('F', '1') = F


Step 1
Initial States: {'D', 'E', 'A', 'C', 'F', 'B'}
Reachable States: {'E', 'F', 'A', 'B'}
Removed States: {'D', 'C'}

Step 2
Loop 1
Equivalent 1
State   0       1
A       1       2
B       1       2
Equivalent 2
State   0       1
E       2       2
F       2       2


Minimized DFA:
Q = [1, 2]
Σ = ['0', '1']
q0 = 1
F = [2]
δ:
(1, '1') = 1
(1, '0') = 2
(2, '1') = 2
(2, '0') = 2
```