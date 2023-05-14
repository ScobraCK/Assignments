knapsack_solve_bnb: solve_bnb.o knapsack_bnb.o priority_queue.o heap.o
	gcc -o $@ $^ -g

%.o: %.c knapsack_bnb.h priority_queue.h heap.h
	gcc -c -o $@ $< -g

clean:
	del knapsack_solve_bnb.exe *.o