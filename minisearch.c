#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "textIndex.h"
#include "trie.h"
#include "postingList.h"

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

  //Create index from input
  textIndex *ti = createTI(inputFile);
  //Create trie from text index
  trie *t = createTrie(ti);


  //Command Line Interface
  char *buffer = NULL;
  size_t bufferSize = 0;
  char *cmd[12]; //A command can have 11 words at most

  printf(">/");
  while(getline(&buffer, &bufferSize, stdin) > 0){
    cmd[0] = strtok(buffer, " \t\n"); //Get command
    for(int i = 1; i <= 10; i++){
      cmd[i] = strtok(NULL, " \t\n"); //Get command parameters
    }
    cmd[11] = NULL;

    if(strcmp(cmd[0], "exit") == 0){
      break;
    }
    else if(strcmp(cmd[0], "df") == 0){
      if(cmd[1] == NULL){
        printTrie(t);
      }
      else{
        printFrequencyTrie(t, cmd[1]);
      }
    }
    else if(strcmp(cmd[0], "tf") == 0){
      if(cmd[1] != NULL && cmd[2] != NULL){
        printTextFrequencyTrie(t, atoi(cmd[1]), cmd[2]);
      }
      else{
        fprintf(stderr, "Error: No parameters given!\n");
      }
    }
    else if(strcmp(cmd[0], "search") == 0){
      printQueryTrie(t, &cmd[1], k);
    }
    else if(strcmp(cmd[0], "help") == 0){
      printf("Minisearch commands:\n");
      printf("\t-search\n");
      printf("\t-df\n");
      printf("\t-tf\n");
      printf("\t-help\n");
      printf("\t-exit\n");
    }
    else{
      fprintf(stderr, "Error: Not a valid command! Type 'help' to see available commands\n");
    }

    printf(">/");
  }

  //Free memory
  free(buffer);
  deleteTI(ti);
  deleteTrie(t);

  return 0;
}
