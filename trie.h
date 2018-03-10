#ifndef TRIE_H
#define TRIE_H

#include "postingList.h"
#include "textIndex.h"

typedef struct trie trie;

trie *createTrie(textIndex *ti);
void deleteTrie(trie *t);

postingList *searchWordTrie(trie *t, char *word);

void printTrie(trie *t);
void printFrequencyTrie(trie *t, char *word);
void printTextFrequencyTrie(trie *t, int textIndex, char* word);

#endif
