#include "postingList.h"
#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef struct trieNode trieNode;

struct trieNode{
  trieNode *prev, *next;
  trieNode *parent, *child;

  char value;
  postingList *pl;
};

//Trie Node functions

trieNode *createTN(trieNode *prev, trieNode *next, trieNode *parent, trieNode * child, char value, postingList *pl){
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

void printWordRecTN(trieNode *tn){
  if(tn->parent != NULL){
    printWordRecTN(tn->parent);
  }
  printf("%c", tn->value);
}

void printRecTN(trieNode *tn){
  if(tn->pl != NULL){
    printWordRecTN(tn);
    printf(" %d\n", getTotalAppearancesPL(tn->pl));
  }

  if(tn->child != NULL){
    printRecTN(tn->child);
  }

  if(tn->next != NULL){
    printRecTN(tn->next);
  }
}

//Trie functions

struct trie{
  trieNode *head;
};

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

trie *createTrie(char **text, int textCount){
  trie *t = malloc(sizeof(trie));
  if(t == NULL){
    return NULL;
  }

  t->head = NULL;


  for(int i = 0; i < textCount; i++){
    printf("\rInserting text in trie (%d/%d lines) [%d%%]", i+1, textCount, (i+1)*100/textCount);
    fflush(stdout);

    insertStringTrie(t, text[i], i);
    //TODO: Error checking
  }
  printf("\n");

  return t;
}

void deleteTrieRec(trieNode *tn){
  postingList *pl;

  //Delete next nodes
  if(tn->next != NULL){
    deleteTrieRec(tn->next);
  }
  if(tn->child != NULL){
    deleteTrieRec(tn->child);
  }

  //Delete current node
  if((pl = getPostingListTN(tn)) != NULL){
    deletePL(pl);
  }
  free(tn);
}

void deleteTrie(trie *t){
  if(t->head != NULL){
    deleteTrieRec(t->head);
  }
  free(t);
}

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
