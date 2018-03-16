#include "trie.h"

#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

float k1 = 1.2;
float b = 0.75;

/****************************** Helping Functions *****************************/

void printSpaces(int count){
  for(int i = 0; i < count; i++){
    printf(" ");
  }
}

void printUnderline(int *underline, int length){
  //Underlines marked spots and resets the flag array
  //If there are no marked spots it does nothing
  int flag = 0;
  for(int i = 0; i < length; i++){
    if(underline[i]){
      flag = 1;
      break;
    }
  }

  if(flag){
    for(int i = 0; i < length; i++){
      if(underline[i]){
        putchar('^');
      }
      else{
        putchar(' ');
      }
      underline[i] = 0;
    }
    putchar('\n');
  }
}

void changeLine(int frontSpace, int *underline, int length){
  putchar('\n');
  printUnderline(underline, length);
  printSpaces(frontSpace);
}

int isKeyword(char *str, char **keyWords){
  for(int i = 0; keyWords[i] != NULL; i++){
    if(strcmp(str, keyWords[i]) == 0){
      return 1;
    }
  }
  return 0;
}

void printResult(int index, int textIndex, float score, char *str, char **keyWords){
  //Prints the result of the search, keeping the appropriate format
  //while also underlining the keywords int the string
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  int length = w.ws_col;
  int frontSpace = printf("%d.(%d)[%.4f] ", index, textIndex, score);

  int *underline = malloc(length * sizeof(int));
  //TODO: Error checking
  for(int i = 0; i < length; i++){
    underline[i] = 0;
  }

  int bufferSize = 100;
  int bufferIndex = 0;
  char *buffer = malloc(bufferSize * sizeof(char));
  //TODO: Error checking

  int afterSpace = 1;
  int spaceLeft = length - frontSpace;
  for(int i = 0; str[i] != '\0'; i++){

    if(isspace(str[i])){
      if(bufferIndex > 0){
        //Print the word in the buffer
        buffer[bufferIndex] = '\0';

        if(strlen(buffer) > spaceLeft){
          //If there is no room change line first
          changeLine(frontSpace, underline, length);
          spaceLeft = length - frontSpace;
        }

        if(isKeyword(buffer, keyWords)){
          //If it's a keyword mark the underline spots
          for(int j = length - spaceLeft; j < length - spaceLeft + strlen(buffer); j++){
            underline[j] = 1;
          }
        }

        spaceLeft -= printf("%s", buffer);

        bufferIndex = 0;
      }

      if(spaceLeft <= 0){
        //Change line
        changeLine(frontSpace, underline, length);
        spaceLeft = length - frontSpace;
      }

      putchar(str[i]);
      spaceLeft--;
      afterSpace = 1;
    }
    else{
      afterSpace = 0;

      if(bufferIndex >= bufferSize - 2){
        //If needed increase buffer size
        bufferSize *= 2;
        buffer = realloc(buffer, bufferSize * sizeof(char));
        //TODO: Error checking
      }

      buffer[bufferIndex] = str[i];
      bufferIndex++;
    }

  }




  if(bufferIndex > 0){
    //Print the word in the buffer
    buffer[bufferIndex] = '\0';

    if(strlen(buffer) > spaceLeft){
      //If there is no room change line first
      changeLine(frontSpace, underline, length);
      spaceLeft = length - frontSpace;
    }

    if(isKeyword(buffer, keyWords)){
      for(int j = length - spaceLeft; j < length - spaceLeft + strlen(buffer); j++){
        underline[j] = 1;
      }
    }

    spaceLeft -= printf("%s", buffer);

    bufferIndex = 0;
  }
  changeLine(0, underline, length);



  printf("\n");

  free(buffer);
  free(underline);
}


/********************************* Trie Node *********************************/

typedef struct trieNode trieNode;

struct trieNode{
  trieNode *prev, *next;
  trieNode *parent, *child;

  char value;
  postingList *pl;
};

// Constructor

trieNode *createTN(trieNode *prev, trieNode *next, trieNode *parent, trieNode *child, char value, postingList *pl){
  trieNode *tn = malloc(sizeof(trieNode));
  if(tn == NULL){
    return NULL;
  }

  tn->prev = prev;
  tn->next = next;
  tn->parent = parent;
  tn->child = child;
  tn->value = value;
  tn->pl = pl;

  return tn;
}

//Destructor

void deleteTN(trieNode *t){
  if(t->pl != NULL){
    deletePL(t->pl);
  }
  free(t);
}

// Accessors

char getValueTN(trieNode *tn){
  return tn->value;
}

postingList *getPostingListTN(trieNode *tn){
  return tn->pl;
}

trieNode *getNextTN(trieNode *tn){
  return tn->next;
}

trieNode *getPrevTN(trieNode *tn){
  return tn->prev;
}

trieNode *getParentTN(trieNode *tn){
  return tn->parent;
}

trieNode *getChildTN(trieNode *tn){
  return tn->child;
}

// Mutators

void setPostingListTN(trieNode *tn, postingList *pl){
  tn->pl = pl;
}

void setNextTN(trieNode *tn, trieNode *next){
  tn->next = next;
}

void setPrevTN(trieNode *tn, trieNode *prev){
  tn->prev = prev;
}

void setChildTN(trieNode *tn, trieNode *child){
  tn->child = child;
}

// Functions for printing the trie

void printWordRecTN(trieNode *tn){
  if(tn->parent != NULL){
    printWordRecTN(tn->parent);
  }
  printf("%c", tn->value);
}

void printRecTN(trieNode *tn){
  if(tn->pl != NULL){
    printWordRecTN(tn);
    printf(" %d\n", getSizePL(tn->pl));
  }

  if(tn->child != NULL){
    printRecTN(tn->child);
  }

  if(tn->next != NULL){
    printRecTN(tn->next);
  }
}


/************************************ Trie ************************************/

struct trie{
  trieNode *head;
  textIndex *ti;
};

// Helping functions for insertions

int insertWordTrie(trie *t, char *word, int textIndex){
  int wordIndex = 0;
  postingList *pl;
  trieNode *cur = t->head;
  trieNode *parent = NULL;
  trieNode *prev, *new;

  while(word[wordIndex] != '\0'){
    //Every letter of the word takes us a level deeper in the trie
    prev = NULL;
    new = NULL;

    while(cur != NULL && getValueTN(cur) < word[wordIndex]){
      prev = cur; //Find the correct node, or position to create
      cur = getNextTN(cur); //a new node, in the current level
    }

    if(cur == NULL){
      //If we reach the end without finding the node
      new = createTN(prev, NULL, parent, NULL, word[wordIndex], NULL);
      //TODO: Error checking
    }
    else if(getValueTN(cur) != word[wordIndex]){
      //If we don't find the node but haven't reached the end
      new = createTN(prev, cur, parent, NULL, word[wordIndex], NULL);
      //TODO: Error checking
    }

    if(new != NULL){
      //If a new node was created, we must update its parent and brothers
      if(prev == NULL){
        //If prev is NULL then the new node is the first in this level
        if(parent != NULL){
          setChildTN(parent, new);
        }
        else{
          t->head = new;
        }
      }
      else{
        //Update previous node
        setNextTN(prev, new);
      }

      if(getNextTN(new) != NULL){
        setPrevTN(getNextTN(new), new);
      }

      cur = new;
    }

    //Move down a level
    parent = cur;
    cur = getChildTN(cur);
    //Go to next letter
    wordIndex++;
  }

  //Go back to the target node (we skipped it because of how the loop works)
  cur = parent;
  if((pl = getPostingListTN(cur)) != NULL){
    addAppearancePL(pl, textIndex);
    //TODO: Error checking
  }
  else{
    pl = createPL(textIndex, 1);
    //TODO: Error checking
    setPostingListTN(cur, pl);
  }
  return 1;
}

int insertStringTrie(trie *t, char *str, int textIndex){
  char temp;
  char *word = NULL;

  for(int i = 0; str[i] != '\0'; i++){
    if(isspace(str[i])){
      if(word == NULL){
        continue;
      }
      else{
        temp = str[i]; //Temporarily set the whitespace to '\0' in order
        str[i] = '\0'; //to pass a string containing only one word
        insertWordTrie(t, word, textIndex);
        //TODO: Error checking
        str[i] = temp;
        word = NULL; //Start looking for a new word
      }
    }
    else if(word == NULL){
      word = &(str[i]); //Mark the beggining of a new word
    }
  }

  if(word != NULL){
    //If the string doesn't have any whitespace at the end we might miss a word
    insertWordTrie(t, word, textIndex);
  }

  return 1;
}

// Constructor

trie *createTrie(textIndex *ti){
  trie *t = malloc(sizeof(trie));
  if(t == NULL){
    return NULL;
  }

  t->head = NULL;
  t->ti = ti;

  int textCount = getTextCountTI(ti);

  for(int i = 0; i < textCount; i++){
    //Print progress
    printf("\rInserting text in trie (%d/%d lines) [%d%%]", i+1, textCount, (i+1)*100/textCount);
    fflush(stdout);

    insertStringTrie(t, getTextTI(ti, i), i);
    //TODO: Error checking
  }
  printf(" Done!\n");

  return t;
}

// Destructor

void deleteTrieRec(trieNode *tn){
  //Delete next nodes
  if(tn->next != NULL){
    deleteTrieRec(tn->next);
  }
  if(tn->child != NULL){
    deleteTrieRec(tn->child);
  }

  //Delete current node
  deleteTN(tn);
}

void deleteTrie(trie *t){
  if(t->head != NULL){
    deleteTrieRec(t->head);
  }
  free(t);
}

// Trie Operations

postingList *searchWordTrie(trie *t, char *word){
  int wordIndex = 0;
  trieNode *parent = NULL;
  trieNode *tn = t->head;

  if(word[0] == '\0'){
    return NULL;
  }

  while(word[wordIndex] != '\0'){
    while(tn != NULL && getValueTN(tn) < word[wordIndex]){
      tn = getNextTN(tn);
    }

    if(tn != NULL && getValueTN(tn) == word[wordIndex]){
      parent = tn;
      tn = getChildTN(tn);
    }
    else{
      return NULL;
    }

    wordIndex++;
  }

  return getPostingListTN(parent);
}

void printTrie(trie *t){
  printRecTN(t->head);
}

void printFrequencyTrie(trie *t, char *word){
  postingList *pl = searchWordTrie(t, word);
  printf("%s %d\n", word, pl != NULL ? getSizePL(pl) : 0);
}

void printTextFrequencyTrie(trie *t, int textIndex, char* word){
  postingList *pl = searchWordTrie(t, word);
  while(pl != NULL){
    if(getIndexPL(pl) == textIndex){
      break;
    }
    pl = getNextPL(pl);
  }
  printf("%d %s %d\n", textIndex, word, pl != NULL ? getCountPL(pl) : 0);
}


int getMinIndex(postingList **pl, int size){
  //Returns the index of the list that has the smallest index
  int index = -1;
  for(int i = 0; i < size; i++){
    if(pl[i] != NULL){
      if(index == -1 || getIndexPL(pl[i]) < index){
        index = getIndexPL(pl[i]);
      }
    }
  }
  return index;
}


void printQueryTrie(trie *t, char **q, int k){
  postingList *pl[10];

  float idf[10];
  float N = (float) getTextCountTI(t->ti);
  float avgdl = (float) getWordCountTI(t->ti) / (float) getTextCountTI(t->ti);

  for(int i = 0; i < 10; i++){
    if(q[i] != NULL){
      pl[i] = searchWordTrie(t, q[i]);

      if(pl[i] != NULL){
        float n_qi = (float) getSizePL(pl[i]);
        idf[i] = log10((N - n_qi + 0.5) / (n_qi + 0.5)) / log10(2);
      }
    }
    else{
      pl[i] = NULL;
    }
  }

  int resultSize = 100;
  int resultIndex = -1;
  int *indices = malloc(resultSize * sizeof(int));
  float *scores = malloc(resultSize * sizeof(float));
  //TODO: Error checking

  int curIndex;
  while((curIndex = getMinIndex(pl, 10)) != -1){
    float D = textWordCountTI(t->ti, curIndex);

    resultIndex++;
    if(resultIndex >= resultSize){
      resultSize *= 2;
      indices = realloc(indices, resultSize * sizeof(int));
      scores = realloc(scores, resultSize * sizeof(float));
      //TODO: Error checking
    }

    indices[resultIndex] = curIndex;
    scores[resultIndex] = 0;

    for(int i = 0; i < 10; i++){
      if(q[i] != NULL){
        float f;

        if(pl[i] != NULL && getIndexPL(pl[i]) == curIndex){
          f = getCountPL(pl[i]);
          pl[i] = getNextPL(pl[i]);
        }
        else{
          f = 0;
        }

        scores[resultIndex] += idf[i] * (f * (k1+1) / (f + k1*(1-b + b*(D/avgdl))));
      }
    }
  }

  resultSize = resultIndex + 1;

  //Sort k best scores
  for(int i = 0; i < k && i < resultSize; i++){
    int maxIndex = i;
    for(int j = i; j < resultSize; j++){
      if(scores[j] > scores[maxIndex]){
        maxIndex = j;
      }
    }
    int tempIndex = indices[i];
    float tempScore = scores[i];

    indices[i] = indices[maxIndex];
    scores[i] = scores[maxIndex];

    indices[maxIndex] = tempIndex;
    scores[maxIndex] = tempScore;
  }

  if(resultSize == 0){
    printf("Nothing found.\n");
  }
  else{
    for(int i = 0; i < k && i < resultSize; i++){
      printResult(i+1, indices[i], scores[i], getTextTI(t->ti, indices[i]), q);
    }
  }

  free(indices);
  free(scores);
}
