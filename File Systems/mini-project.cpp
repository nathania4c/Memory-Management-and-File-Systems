#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>  
#include "lab3.cpp"
using namespace std;

//Disk layout in bytes
const uint BLOCK_SIZE = 1024;
const uint DISK_SIZE = 128 * BLOCK_SIZE; 
uint free_block_list = 127;

myFileSystem *fs;

int main(int argc, char *argv[]){
  
  char command;
  string diskname;
  string line;
  string word;
  string name;
  int32_t size;
  
  ifstream myFile;
  myFile.open("lab3input.txt");
  if (myFile.is_open()) //if the file exists
  {
    printf("here");
    getline(myFile,diskname);
    myFileSystem myFS = myFileSystem(diskname);
    fs = &myFS;
    
    while (getline(myFile,line)){
      istringstream ss(line);
      ss >>word;
      command = word[0];
      if (command == 'L'){
        fs ->ls();
      } else {
        ss >> name;
        ss >> word;
        size = stoi(word);
        
        char n[8];
        strcpy(n,name.c_str());
        char dummy[1024];
        
        if (command == 'C'){
          fs ->create(n,size);
        }
        if (command == 'W'){
          fs ->write(n,size,dummy);
        }
        if (command == 'D'){
          fs ->deletee(n);
        }
        if (command == 'R'){
          fs ->read(n,size,dummy);
        }
      }
    }
  }
  
return 0;
}