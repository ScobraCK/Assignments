# 0/1 Knapsack Problem
Last update: 2023/04/10
<br>Algorithm HW 2
<br>Solution for the 0/1 knapsack problem using dynamic programming.

## Problem description
- Given 𝑛 items and a "knapsack.”
- Item 𝑖 has weight 𝑤𝑖 > 0 and has value 𝑣<sub>𝑖</sub> > 0.
- Knapsack has capacity of 𝑊.
- Goal: Fill knapsack so as to maximize total value.


## Algorithm
We define 𝑉(𝑖, 𝑗) as the optimal solution of items 
subset {1,…, 𝑖} with capacity 𝑗. 
Then, the optimal solution would be 𝑉(𝑛, 𝑊).

There are two cases to consider for 𝑉(𝑖, 𝑗) depending on if we add the item 𝑖:

𝑉(𝑖, 𝑗) does not include item 𝑖, because of out of capacity or not worthy.
- 𝑉(𝑖, 𝑗) = 𝑉(𝑖 − 1, 𝑗).

 𝑉(𝑖, 𝑗) includes item 𝑖.
- 𝑉(𝑖, 𝑗) = 𝑉(𝑖 − 1, 𝑗 − 𝑤<sub>𝑖</sub>) + 𝑣<sub>𝑖</sub>.

Note: 𝑉(𝑖, 𝑗) = *max*(𝑉(𝑖 − 1, 𝑗), 𝑉(𝑖 − 1, 𝑗 − 𝑤<sub>𝑖</sub>) + 𝑣<sub>𝑖</sub>) if (𝑗 − 𝑤<sub>𝑖</sub> ≥ 0)<br>automatically determines if an item is not worthy or if it should be included.

We will also have the base case of 𝑉(0, 𝑗) = 0 and 𝑉(𝑖, 0) = 0. 
<br>Bottum-up construction will be done by simply looping over 𝑖 and 𝑗.


## Finding the optimal items
The optimal value is stored in 𝑉(𝑛, 𝑊). To find the optimal path it took to get to there, we go backwards.

- If 𝑉(𝑖, 𝑗) = 𝑉(𝑖 − 1, 𝑗)
<br>That item was not worthy so we go to 𝑉(𝑖 − 1, 𝑗)
- Else
<br>The item was taken and we go to 𝑉(𝑖 − 1, 𝑗 − 𝑤<sub>𝑖</sub>)


## Code explanation
The core part of the code has 2 functions that make the array V *solve_knapsack()* and then finds the optimal list of items *find_optimal_items()* using the solved V. If you wish to only know the maximum value, only the first function is nessecary.

These both take 3 input paramaters:
- **max_weight:** The capacity(𝑊). 
- **items[𝑛 + 1]:** An array of Item(defined below) that has the data of the items.
The size of this array should be `𝑛 + 1` with `item[0].value = 𝑛` and `item[0].weight` is unused. This is done for convnience as the index matches with the item number while providing additional information.
- **V[𝑛+1][𝑊+1]:** Array of optimal solutions.
```c
typedef struct Item {
    int value;
    int weight;
} Item;

void solve_knapsack(int max_weight, Item* items, int** V);
int* find_optimal_items(int max_weight, Item* items, int** V);
```

The return value for *find_optimal_items()* is an int array with boolean values for each item showing if it was taken or not. This array also has size `𝑛+1` with the value of index 0 the *total weight* of the items.


## Usage
As long as the input and output of the 2 functions described above are correct, the ways to read the data can done in any way. In this case, *solve.c* was implemented to get 2 arguments for the data file and capacity. Example usage shown below.
```bash
# windows
knapsack_solve.exe filepath capacity
# linux
./knapsack_solve filepath capacity
```

The data is read in csv format with (value, weight) pairings for each entry.

### Example
```bash
>knapsack_solve.exe samples\simple_test.csv 5
V:
         0       0       0       0       0       0
         0       10      10      10      10      10
         0       12      22      22      22      22
         0       12      22      27      37      37
         0       12      22      27      37      42

----------Optimal Items----------
Selected items:
1. Value: 10, Weight: 1
2. Value: 12, Weight: 1
3. Value: 20, Weight: 3

Total Value: 42, Total Weight: 5
```

*The array V is only shown for example purposes and it is coded such that it will not show if 𝑛 or 𝑊 is larger than 10.