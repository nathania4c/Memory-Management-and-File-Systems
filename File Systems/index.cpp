#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>  
#include "ryan_test.cpp"

using namespace std;

//got this from project 1
char readChar() {
  char c = getchar();
  while (getchar() != '\n');
  return c;
}

int main(int argc, char *argv[]){
  
  string line; //to read file
  string word; //to parse line
  char command; //ex. C,W,L
  char name[8]; //name of file
  int32_t size; //size of file
  
  //open file
  ifstream myFile;
  myFile.open("lab3input.txt");
  if (myFile.is_open()) //if the file exists
  {
    //get diskname
    getline(myFile,line);
    //create file system
    myFileSystem fs = myFileSystem(line);
    
    while(getline(myFile,line)){
      
      //to slow down process so we can see line by line
      sleep(1);
      
      //to debug
      //printf("Press any key to continue");
      //readChar();
      
      //parse each word
      istringstream ss(line);
      ss >> word;
      //get command char
      command = word[0];
      //if L call ls
      if (command == 'L'){
        fs.ls();
        continue;
      }
      //if not L then get file name
      ss >> word;
      strcpy(name,word.c_str());
      //if D call delete
      if (command == 'D'){
        fs.del(name);
        continue;
      }
      //if not D then get size
      ss >>word;
      //convert string to int
      size = stoi(word);
      //create dummy 1KB to pass as argument for read and write
      char dummy[1024];
      //if C call create
      if (command == 'C'){
        fs.create(name,size);
      }
      //if W call write
      if (command == 'W'){
        fs.write(name,size,dummy);
      }
      //if R call read
      if (command == 'R'){
        fs.read(name,size,dummy);
      }
    }
  }
  //close file
  myFile.close();
}