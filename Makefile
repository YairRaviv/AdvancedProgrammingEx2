all: stree

stree:
	gcc -o stree stree.c
clean:
	rm stree