#ifndef POSTING_LIST_H
#define POSTING_LIST_H

typedef struct postingList postingList;

postingList *createPL(int textIndex, int count);
int addAppearancePL(postingList *pl, int textIndex);
void deletePL(postingList *pl);

postingList *getNextPL(postingList *pl);

int getIndexPL(postingList *pl);
int getCountPL(postingList *pl);
int getSizePL(postingList *pl);
int getTotalAppearancesPL(postingList *pl);

void printPL(postingList *pl);

#endif
