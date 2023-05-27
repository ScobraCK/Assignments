avl: avl_tree.o main.o
	gcc -o $@ $^ -g

%.o: %.c avl_tree.h
	gcc -c -o $@ $< -g

clean:
	del avl.exe *.o