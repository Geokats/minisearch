#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_TEXT_SIZE 100
#define INIT_STRING_LENGTH 50

int removeStringIndex(char *str, int expIndex){
  //Remove the first word of the string and check if it is the expected index
  //If it is return 1, otherwise return 0

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
      else if(charIndex == curLength){
        //If necessary increase string's length
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

int main(int argc, char const *argv[]) {
  //TODO: Get command line parameters
  char *inputFile = "input/example.in";
  int K = 10;

  int textCount;
  char **text = getInput(inputFile, &textCount);

  //Print text
  for(int i = 0; i < textCount; i++){
    printf("%d %s\n", i, text[i]);
  }

  //Free memory
  for(int i = 0; i < textCount; i++){
    free(text[i]);
  }
  free(text);

  return 0;
}
