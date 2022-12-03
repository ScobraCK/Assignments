csim: csim.o cache.o
	gcc -o $@ $^ -g

%.o: %.c cache.h
	gcc -c -o $@ $< -g

clean:
	del csim.exe *.o