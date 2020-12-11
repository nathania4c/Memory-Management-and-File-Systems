#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class inode
{
public:
  char name[8]; //file name
  uint32_t size;     // file size (in number of blocks)
  uint32_t blockPointers[8]; // direct block pointers
  uint32_t used;             // 0 => inode is free; 1 => in use
  
  void setName(int index,char input)
  {
    name[index] = input;
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
  
  char * getName()
  {
    return name;
  }
  
  uint32_t getSize()
  {
    return size;
  }
  
  uint32_t * getBlock()
  {
    return blockPointers;
  }
  
  uint32_t getUsed()
  {
    return used;
  }
  
};