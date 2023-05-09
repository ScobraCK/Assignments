knapsack_solve: solve.o knapsack.o array.o
	gcc -o $@ $^ -g

%.o: %.c knapsack.h array.h
	gcc -c -o $@ $< -g

clean:
	del knapsack_solve.exe *.o