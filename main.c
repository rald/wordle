#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define STRING_MAX 256


double drand() {
  return rand()/(RAND_MAX+1.0);
}


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


char *getrandword() {
  char line[STRING_MAX];
  char rstr[STRING_MAX];
  FILE *fp=fopen("words.txt","r");

  long n=0;
  while(fgets(line,STRING_MAX,fp)) {
    removenl(line);
    n++;
    if(1.0/n>drand()) {
      strcpy(rstr,line);
    }
  }

  fclose(fp);

  return strdup(rstr);
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


char *getclue(char *r,char *w) {
  char *clue=calloc(6,sizeof(*clue));
  char w1[STRING_MAX]={0};
  char w2[STRING_MAX]={0};

  strcpy(w1,strupr(r));
  strcpy(w2,strupr(w));

  for(int i=0;i<5;i++) {
    if(w1[i]==w2[i]) {
      clue[i]=toupper(w1[i]);
      w1[i]='\0';
      w2[i]='\0';
    }
  }

  for(int i=0;i<5;i++) {
    for(int j=0;j<5;j++) {
      if(w1[j]!='\0' && w2[i]!='\0' && w1[j]==w2[i]) {
        clue[i]=tolower(w1[j]);
        w1[j]='\0';
        w2[i]='\0';
        break;
      }
    }
  }

  for(int i=0;i<5;i++) {
    if(clue[i]=='\0') clue[i]='?';
  }

  return clue;
}


int main() {

  srand(time(NULL));

  bool quit=false;
  char word[STRING_MAX];
  char *rword=getrandword();
  char *clue;
  char n=0;

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

    clue=getclue(rword,word);
    printf("  %s\n",clue);
    free(clue);

    if(!strcmp(rword,word)) {
      puts("you got it");
      quit=true;
    }
    n++;
    if(n==5) {
      printf("it is %s\n",rword);
      quit=true;
    }
  }

  puts("game over");

  free(rword);
  rword=NULL;

  return 0;
}

