
CC=gcc
CFLAGS =  -Wall -O3

.PHONY: all
all:  Trie4Hist.exe # Makedata.py

.PHONY: debug
debug:  Trie4Hist-g.exe


.PHONY: clean
clean:
	rm -rf *.exe

Makedata.py: *.dat
	python Makedata.py


Trie4Hist.exe: tree.c main.c main_functions.c array_list.c
	$(CC) tree.c main.c main_functions.c array_list.c -o  Trie4Hist.exe  $(CFLAGS)

Trie4Hist-g.exe: tree.c main.c main_functions.c array_list.c
	$(CC) tree.c main.c main_functions.c array_list.c -o  Trie4Hist-g.exe  $(CFLAGS) -g

