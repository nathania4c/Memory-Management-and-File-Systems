#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

class inode
{
  char name[8]; //file name
  uint32_t size;     // file size (in number of blocks)
  uint32_t blockPointers[8]; // direct block pointers
  uint32_t used;             // 0 => inode is free; 1 => in use
public:
  void setName(char input[8])
  {
    string inputS = input;
    strcpy(name,inputS.c_str());
    //name = input;
  }
  
  void setSize(uint32_t input)
  {
    size = input;
  }
  
  void setBlock(int index, uint32_t input)
  {
    blockPointers[index] = input;
  }
  
  void setUsed(uint32_t input)
  {
    used = input;
  }
  
  char* getName()
  {
    return name;
  }
  
  uint32_t getSize()
  {
    return size;
  }
  
  uint32_t* getBlock()
  {
    return blockPointers;
  }
  
  uint32_t getUsed()
  {
    return used;
  }
  
};