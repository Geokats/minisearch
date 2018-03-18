#ifndef TRIE_H
#define TRIE_H

#include "postingList.h"
#include "textIndex.h"

typedef struct trie trie;

trie *createTrie(textIndex *ti);
/* Creates a trie from an already existing text index, if any error
 * is encountered, NULL is returned
*/

void deleteTrie(trie *t);
/* Deletes the trie and frees all memory allocated by it
*/

postingList *searchWordTrie(trie *t, char *word);
/* Searches for the given word in the trie, if it's found a pointer to
 * its posting list is returned, otherwise NULL
*/

void printTrie(trie *t);
/* Prints all words in the trie allong with the number of
 * documents they appear in
*/

void printFrequencyTrie(trie *t, char *word);
/* Searches for the given word in the trie and prints it allong with the
 * number of documents it appers in
*/

void printTextFrequencyTrie(trie *t, int textIndex, char* word);
/* Searches for the given word and print it allong with the number
 * of time it appears in the document with given the given index
*/

void printQueryTrie(trie *t, char **q, int k);
/* Searches each word q[i] and print the k documents with the highest
 * score for the given words
*/

#endif
