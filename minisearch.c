#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "trie.h"
#include "postingList.h"

#define INIT_TEXT_SIZE 100
#define INIT_STRING_LENGTH 50

int removeStringIndex(char *str, int expIndex){
  //Remove the first word of the string and convert it to an int
  //If it equals the expected index return 1, otherwise return 0

  char *buffer;
  int i, j;
  int index;

  i = 0;
  while(str[i] != ' '){
    i++;
  }

  buffer = malloc((i+1) * sizeof(char));
  //TODO: Error checking

  //Copy index to buffer
  str[i] = '\0';
  strcpy(buffer, str);
  //Remove index from str
  j = -1;
  do{
    j++;
    i++;
    str[j] = str[i];
  }while(str[j] != '\0');

  index = atoi(buffer);
  free(buffer);

  return index == expIndex;
}

char **getInput(const char *fileName, int *size){
  char c;
  char **text;
  int textSize = INIT_TEXT_SIZE; //Number of available pointers
  int textCount = 0; //Number of used pointers

  text = malloc(textSize * sizeof(char*));
  //TODO: Error checking

  FILE *fp = fopen(fileName, "r");
  if(fp == NULL){
    //TODO: Error handling
    fprintf(stderr, "Error opening file\n");
    return NULL;
  }

  int textIndex = 0;
  int charIndex = 0;
  int curLength = 0;

  while((c = fgetc(fp)) != EOF){
    if(c == '\n'){
      if(charIndex == 0){
        continue; //Ignore empty lines
      }

      text[textIndex][charIndex] = '\0';
      if(!removeStringIndex(text[textIndex], textIndex)){
        printf("Index error\n");
        //TODO: Error handling
      }

      //Go to next line
      textIndex++;

      if(textIndex == textSize){
        //If necessary increase array's size
        textSize *= 2;
        text = realloc(text, textSize * sizeof(char*));
        //TODO: Error checking
      }

      curLength = 0;
      charIndex = 0;
    }
    else{
      if(curLength == 0){
        curLength = INIT_STRING_LENGTH;
        text[textIndex] = malloc(curLength * sizeof(char));
        //TODO: Error checking
      }
      else if(charIndex + 1 == curLength){
        //If necessary increase string's length
        //+1 is to make sure we always have an extra position for '\0'
        curLength *= 2;
        text[textIndex] = realloc(text[textIndex], curLength * sizeof(char));
        //TODO: Error checking
      }

      text[textIndex][charIndex] = c;
      charIndex++;
    }
  }

  fclose(fp);

  *size = textIndex;
  return text;
}

int main(int argc, char * const *argv) {
  //Get command line arguments
  int c;
  char *inputFile = NULL;
  int k = 0;

  while((c = getopt(argc, argv, "i:k:")) != -1){
    switch(c){
      case 'i':
        inputFile = optarg;
        break;
      case 'k':
        k = atoi(optarg);
        break;
    }
  }
  //Check arguments
  if(inputFile == NULL || k == 0){
    fprintf(stderr, "Usage: ./minisearch -i docfile -k K\n");
    return 1;
  }
  else if(k <= 0 || k > 10){
    fprintf(stderr, "Error: k must be between 1-10\n");
    return 1;
  }

  //Read input file
  int textCount;
  char **text = getInput(inputFile, &textCount);
  //Create trie from input
  trie *t = createTrie(text, textCount);


  char *buffer;
  int bufferSize;
  //Command Line Interface

  printTrie(t);


  //Free memory
  for(int i = 0; i < textCount; i++){
    free(text[i]);
  }
  free(text);

  deleteTrie(t);

  return 0;
}
