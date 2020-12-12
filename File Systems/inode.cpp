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
  
  void setBlockPointer(uint32_t input)
  {
    for(int i = 7; i >= 0;i--){
      if(input-(pow(2,i)) > 0){
        setBlock((i),1);
        input = input - (pow(2,i));
      }
      else if(input-(pow(2,i)) == 0){
        setBlock((i),1);
        break;
      }
    }
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
  
  char* getSizeBinary(){
    static char result[8];
    int s = size;
    for(int i = 7; i >= 0;i--){
      if(s-(pow(2,i)) >= 0){
        result[i]='1';
        s = s - (pow(2,i));
      }
      else {
        result[i]='0';
      }
    }
    return result;
  }
  
  uint32_t* getBlock()
  {
    return blockPointers;
  }
  uint32_t getBlockPointer()
  {
    uint32_t result = 0;
    for (int i = 0; i < 8; i++){
      result = result + pow(2*blockPointers[i],i);
    }
    return result;
  }
  
  uint32_t getUsed()
  {
    return used;
  }
  
};