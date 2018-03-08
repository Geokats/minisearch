#ifndef TRIE_H
#define TRIE_H

#include "postingList.h"

typedef struct trie trie;

trie *createTrie(char **text, int textCount);
void deleteTrie(trie *t);

int insertStringTrie(trie *t, char *str, int textIndex);
int insertWordTrie(trie *t, char *word, int textIndex);

postingList *searchWordTrie(trie *t, char *word);

void printTrie(trie *t);

#endif
