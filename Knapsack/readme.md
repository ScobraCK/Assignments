# Knapsack Problem
Solution for the knapsack problme using dynamic programming.

## Problem description
- Given 𝑛 items and a "knapsack.”
- Item 𝑖 has weight 𝑤𝑖 > 0 and has value 𝑣𝑖 > 0.
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

