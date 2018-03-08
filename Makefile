CC = gcc
OUT = minisearch
OBJ = minisearch.o postingList.o trie.o

all: minisearch postingList trie
	$(CC) $(OBJ) -o $(OUT)

minisearch: minisearch.c
	$(CC) -c minisearch.c

postingList: postingList.c postingList.h
	$(CC) -c postingList.c

trie: trie.c trie.h
	$(CC) -c trie.c

clean:
	rm $(OUT) $(OBJ)
