#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define WORDS_FILE "words.txt"
#define STRING_MAX 256


char *removenl(char *line) {
  char *p=strchr(line,'\n');
  if(p) *p='\0';
  return line;
}


char *strupr(char *s) {
  char *p=s;
  while(*p) {
    *p=toupper(*p);
    p++;
  }
  return s;
}


char *getrandword(char *filename) {
  static char selected[STRING_MAX];
  char line[STRING_MAX];
  FILE *file=fopen(filename,"r");

  if(!file) return NULL;

  size_t n=0;
  while(fgets(line,sizeof(line),file)) {
    if(rand()%(++n)==0) {
      removenl(line);
      strncpy(selected,line,STRING_MAX);
    }
  }

  fclose(file);

  return n>0?selected:NULL;
}


bool find(char *word) {
  char line[STRING_MAX];
  FILE *fp=fopen("words.txt","r");
  while(fgets(line,STRING_MAX,fp)) {
    removenl(line);
    strupr(line);
    if(!strcmp(line,word)) return true;
  }
  return false;
}


void wordle(const char* randomWord, const char* guess) {
  char result[6] = {0};
  int used[5] = {0};

  for (int i = 0; i < 5; i++) {
    if (guess[i] == randomWord[i]) {
      result[i] = toupper(guess[i]);
      used[i] = 1;
    }
  }

  for (int i = 0; i < 5; i++) {
    if (result[i] != 0) continue;
    for (int j = 0; j < 5; j++) {
      if (!used[j] && guess[i] == randomWord[j]) {
        result[i] = tolower(guess[i]);
        used[j] = 1;
        break;
      }
    }

    if (result[i] == 0) {
      result[i] = '?';
    }
  }

  printf("  %s\n",result);
}


int main() {

  srand(time(NULL));

  bool quit=false;
  char word[STRING_MAX];
  char *rword=getrandword(WORDS_FILE);
  char *clue;
  char n=0;

  if(!rword) return -1;

  strupr(rword);

  while(!quit) {

    printf("> ");

    if(!fgets(word,STRING_MAX,stdin)) {
      break;
    }

    removenl(word);
    strupr(word);

    if(strlen(word)!=5) {
      puts("five lettered words only");
      continue;
    }

    if(!find(word)) {
      printf("unknown word %s\n",word);
      continue;
    }

    wordle(rword,word);

    if(!strcmp(rword,word)) {
      puts("you got it");
      quit=true;
    }

    n++;
    if(n==6) {
      printf("it is %s\n",rword);
      quit=true;
    }
  }

  puts("game over");

  return 0;
}

