#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>  
#include "lab3.cpp"
using namespace std;

int main(int argc, char *argv[]){

  uint block = 1000; //1 block is 1 KB
  uint fileSystemSize = 128 * block; //size of the file system
  int files = 0; //will keep track of file sin file systems
  
  char command;
  char name[8]; //file name
  int32_t size; // file size (in number of blocks)
  int32_t blockPointers[8]; // direct block pointers
  int32_t used; // 0 => inode is free; 1 => in use
  string diskname[1];
  
  
  
  
return 0;
}