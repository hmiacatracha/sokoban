#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void readline(FILE *f,char *line, int *goals) {
  int c,i=0;
 
  c=fgetc(f);
  while (c!=EOF && c!='\n') {
    line[i++]=c;
    if (c=='$' || c=='*') (*goals)++;
    c=fgetc(f);
  }
  line[i]=0;
}

char **loadFile(char *fileName, int *rows, int *cols, int *goals) {
  FILE *f;
  char line[512], *s;
  int i,j,l;
  QUEUE lines=qEMPTY;
  char **array;
 
  *goals=0;
  *cols=-1;
  if (fileName==NULL || (f=fopen(fileName,"r"))==NULL) {
    printf("Could not open input file %s\n",fileName); exit(1);
  }
  readline(f,line,goals);
  while ((l=strlen(line))!=0) {
    if (l>*cols) *cols=l;
    s=(char *)malloc((l+1)*sizeof(char));
    strcpy(s,line);
    qInsertLast(&lines,&s,sizeof(char *));
    readline(f,line,goals);
  }
  fclose(f);
 
  array=(char **)Queue2Array(lines,sizeof(char *),rows);
  qFree(&lines);
 
  for (i=0;i<*rows;i++) {
    l=strlen(array[i]);
    s=(char *)malloc(sizeof(char)* (*cols));
    memcpy(s,array[i],sizeof(char)* (*cols));
    free(array[i]);
    array[i]=s;
  }

  return array;
}
