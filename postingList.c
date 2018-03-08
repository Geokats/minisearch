#include "postingList.h"

#include <stdlib.h>
#include <stdio.h>

struct postingList{
  int textIndex;
  int appearanceCount;

  postingList *next;
};

postingList *createPL(int textIndex, int count){
  postingList *pl = malloc(sizeof(postingList));
  if(pl == NULL){
    return NULL;
  }

  pl->textIndex = textIndex;
  pl->appearanceCount = count;
  pl->next = NULL;

  return pl;
}

int addAppearancePL(postingList *pl, int textIndex){
  while(pl->textIndex != textIndex && pl->next != NULL){
    //Go to the node with the same index or the end of the list
    pl = pl->next;
  }
  if(pl->textIndex == textIndex){
    pl->appearanceCount += 1;
    return 1;
  }
  else{
    pl->next = createPL(textIndex, 1);
    return pl->next != NULL;
  }
}

void deletePL(postingList *pl){
  if(pl->next != NULL){
    deletePL(pl->next);
  }
  free(pl);
}

postingList *getNextPL(postingList *pl){
  return pl->next;
}

int getIndexPL(postingList *pl){
  return pl->textIndex;
}

int getCountPL(postingList *pl){
  return pl->appearanceCount;
}

int getSizePL(postingList *pl){
  int size = 1;
  while(pl->next != NULL){
    pl = pl->next;
    size++;
  }
  return size;
}

int getTotalAppearancesPL(postingList *pl){
  int total = 0;

  while(pl != NULL){
    total += getCountPL(pl);
    pl = getNextPL(pl);
  }

  return total;
}

void printPL(postingList *pl){
  printf("[ ");
  while(pl != NULL){
    printf("(%d,%d) ", pl->textIndex, pl->appearanceCount);
    pl = pl->next;
  }
  printf("]");
}
