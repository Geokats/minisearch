#ifndef TRIE_H
#define TRIE_H

#include "postingList.h"

typedef struct trie trie;

trie *createTrie(char **text, int textCount);
void deleteTrie(trie *t);

postingList *searchWordTrie(trie *t, char *word);

void printTrie(trie *t);

#endif
