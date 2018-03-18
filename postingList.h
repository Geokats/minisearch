#ifndef POSTING_LIST_H
#define POSTING_LIST_H

typedef struct postingList postingList;

postingList *createPL(int textIndex, int count);
/*  Creates new posting list with one node containing textIndex and count
 *  and returns a pointer to it. If memory allocation fails NULL is returned
*/

int addAppearancePL(postingList *pl, int textIndex);
/* Adds one apperance to the index given in an already existing posting list
 * if the posting list doesn't contain the index then a new node is created
 * if memory allocation fails for the new node 0 is returned otherwise 1
*/

void deletePL(postingList *pl);
/* Given the first node of the posting list, it deletes all the list's nodes
 * freeing all memory allocated for the list
*/


postingList *getNextPL(postingList *pl);
/* Given a node of the posting list, it returns the next node in the list
*/

int getIndexPL(postingList *pl);
/* Returns the textIndex of the given node
*/

int getCountPL(postingList *pl);
/* Returns the appearance count of the given node
*/

int getSizePL(postingList *pl);
/* Given the staring node of the list, returns the number of nodes in the list
*/

int getTotalAppearancesPL(postingList *pl);
/* Given the starting node of the list, return the sum of appearance count
 * int all the list's nodes
*/

void printPL(postingList *pl);
/* Prints all the indices and appearance counts in the list
*/

#endif
