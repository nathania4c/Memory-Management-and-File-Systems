#include <stdlib.h>
#include <vector>
using namespace std;

class inode {
public:
  
  char name[8]; //file name
  int32_t size; //file size (in number of blocks)
  int32_t blockPointer[8]; //direct block pointers
  int32_t used; //0 means free, 1 means used
  
  inode(char n[8], int32_t s){
    for (int i=0; i<8; i++){
      name[i] = n[i];
    }
    size = s;
    used = 0;
  }
  
  void setPointer(int32_t bp[8]){
    for (int i=0; i<8; i++){
      blockPointer[i] = bp[i];
    }
  }
};

const int MAX_FILES = 16;

struct SuperBlock{
public:
  int32_t blockPointer[8];
  vector<inode *> inodes;
};


