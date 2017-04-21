#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <cmath>
#include<cstdio>
#include<bitset>
using namespace std;
int MAX_SIZE, globaldepth, mode;
typedef struct bucket
{
  int localdepth;
  std::vector<int> data;
}bucket;
typedef struct directory
{
  bucket *pointer;
}directory;

directory *table=NULL;

void insert(int key);
void splitinsert(int key);
void create()
{
  int i;
  table = (directory *)malloc(pow(2,globaldepth)*sizeof(directory));
  for(i=0;i<pow(2,globaldepth);i++)
  {
    table[i].pointer = new bucket;
    table[i].pointer->localdepth = globaldepth;
  }
}

char *binaryconv(int a,int b)
{
  char *c = (char *)malloc(a*sizeof(char));
  int i;
  for(i=0;i<a;i++)
  {
    c[i] = (b%2) +48;
    b = b/2;
  }
  char *d = (char *)malloc(a*sizeof(char));
  for(i=0;i<a;i++)
  {
    d[i] = c[a-i-1];
  }
  //cout << d << "\n";
  return d;
}

int hashval(int key)
{
  int temp = pow(2,globaldepth);
  return key%temp;
}

directory *doubledirectory()
{
  int i;
  globaldepth++;
  directory *dup = (directory *)malloc(pow(2,globaldepth)*sizeof(directory));
  for(i=0;i<pow(2,globaldepth-1);i++)
  {
    dup[i].pointer = table[i].pointer;
  }
  for(i=pow(2,globaldepth-1);i<pow(2,globaldepth);i++)
  {
    int temp = hashval(i+pow(2,globaldepth-1));
    dup[i].pointer = table[temp].pointer;
    dup[i].pointer->localdepth = table[temp].pointer->localdepth;
  }
  return dup;
}

directory *halfdirectory()
{
  int i;
  directory *dup = (directory *)malloc(pow(2,globaldepth)*sizeof(directory));
  for(i=0;i<pow(2,globaldepth);i++)
  {
    dup[i].pointer = table[i].pointer;
  }
  return dup;
}

void updateglobaldepth()
{
  int max = 0,i;
  for(i=0;i<pow(2,globaldepth);i++)
  {
    if(max<table[i].pointer->localdepth)
      max= table[i].pointer->localdepth;
  }
  if(max < globaldepth)
  {
    globaldepth = max;
    table = halfdirectory();
  }
}

void deletekey(int key)
{
  int loc = hashval(key);
  bucket *temp = table[loc].pointer;
  int i;
  for(i=0;i<temp->data.size();i++)
  {
    if(key == temp->data.at(i))
    {
      temp->data.erase(temp->data.begin()+i);
      cout << "Key " << key << " deleted from bucket " << binaryconv(temp->localdepth,loc) << "\n";
      if(temp->data.empty())
      {
        int img = hashval(loc + pow(2,globaldepth-1));
        table[img].pointer->localdepth -= 1;
        table[loc].pointer = table[img].pointer;
        temp->localdepth = table[img].pointer->localdepth;
      }
      if(mode == 2)
          updateglobaldepth();

      return;
    }
  }
  cout << "Key not found.\n";
}

void search(int key)
{
  int loc = hashval(key);
  bucket *temp = table[loc].pointer;
  int i;
  for(i=0;i<temp->data.size();i++)
  {
    if(key == temp->data.at(i))
    {
      cout << "Key " << key << " found in bucket "<< binaryconv(temp->localdepth,loc) << "\n";
      return;
    }
  }
  cout << "Key not found.\n";
}

void status()
{
  cout << "Global Depth = " << globaldepth << "\n";
  int i,j;
  for(i=0;i<pow(2,globaldepth);i++)
  {
     if( i < pow(2,table[i].pointer->localdepth))
     {
      cout << binaryconv(table[i].pointer->localdepth,i)  << " --> " ;
      bucket *temp = table[i].pointer;
      for(j=0;j<(temp->data).size();j++)
      {
        cout << (temp->data).at(j) << " ";
      }
      cout <<  "\n";
     }
  }
}

int main(int argc,char *argv[])
{
  globaldepth = atoi(argv[1]);
  MAX_SIZE = atoi(argv[2]);
  mode = atoi(argv[3]);
  create();
  char operation[50];
  int key;
  FILE *fp;
  fp = fopen(argv[4],"r");
  if(!fp)
  {
    cout << "File not found\n";
  }
  while(!feof(fp))
  {
    fscanf(fp,"%s %d",operation,&key);
    // if(feof(fp))
    //   break;
    if(strcmp(operation,"insert")==0)
      insert(key);
    else if(strcmp(operation,"delete")==0)
      deletekey(key);
    else if(strcmp(operation,"search")==0)
      search(key);
    else if(strcmp(operation,"status")==0)
      status();
  }
  fclose(fp);
}

void splitinsert(int key)
{
  int loc = hashval(key);
  int dup = hashval(loc+pow(2,globaldepth-1));
  //cout << loc << dup << "\n";
  table[dup].pointer->localdepth += 1;
  table[loc].pointer = new bucket;
  table[loc].pointer->localdepth = table[dup].pointer->localdepth;
  bucket *temp = table[dup].pointer;
  std::vector<int> copy;
  while(!temp->data.empty())
  {
    copy.push_back(temp->data.back());
    //cout << temp->data.back() << "\n";
    temp->data.pop_back();
  }
  while(!copy.empty())
  {
    int j = copy.back();
    table[hashval(j)].pointer->data.push_back(j);
    copy.pop_back();
  }
  (table[loc].pointer->data).push_back(key);
  //cout << "split and insertion completed\n";
  cout << "Inserted " << key << " in bucket " << binaryconv(table[loc].pointer->localdepth,loc) << "\n";
}

void insert(int key)
{
  int loc = hashval(key);
  bucket *temp = table[loc].pointer;
  if((temp->data).size()==MAX_SIZE)
  {
    //cout << "Bucket Full!\n";
    if(temp->localdepth == globaldepth)
    {
      //cout << "Localdepth = Globaldepth\n";
      table = doubledirectory();
      splitinsert(key);
    }
    else{
      splitinsert(key);
    }
  }
  else
  {
    (temp->data).push_back(key);
    const int size = temp->localdepth;
    cout << "Inserted " << key << " in bucket " << binaryconv(temp->localdepth,loc) << "\n";
  }
}
