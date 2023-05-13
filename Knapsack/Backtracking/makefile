knapsack_solve_bt: solve_bt.o knapsack_bt.o
	gcc -o $@ $^ -g

%.o: %.c knapsack_bt.h
	gcc -c -o $@ $< -g

clean:
	del knapsack_solve_bt.exe *.o