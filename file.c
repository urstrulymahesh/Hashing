#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char *argv[])
{
  char operation[50];
  int key;
  FILE *fp;
  fp = fopen(argv[1],"r");
  if(!fp)
  {
    printf("File not found\n");
  }
  while(!feof(fp))
  {
    fscanf(fp,"%s %d",operation,&key);
    if(feof(fp))
      break;
    if(strcmp(operation,"insert")==0)
      printf("%d is inserted\n",key);
    else if(strcmp(operation,"delete")==0)
      printf("10 is deleted\n");
    else if(strcmp(operation,"status")==0)
      printf("status\n");
  }
  fclose(fp);
}
