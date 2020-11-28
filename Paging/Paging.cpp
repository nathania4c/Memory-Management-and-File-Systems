#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

/*
 * Note:  To help explain the code, we will use the example v=49155, n=9,m=7
 *        This example is found in the inputfile.txt
 */

uint n_plus_m = 16; //how many bits
uint bitSize = (1<<16); //2^16
uint maxNum = bitSize-1; //maximum address number ie. 1111 1111 1111 1111 in binary

//use this method to print the uint as binary for visual checking
//there are a lot of calls of check scattered, if you would like to see, just uncomment them
void check(uint num){
  printf("The binary is ");
  std::cout << std::bitset<32>(num);
  printf("\n");
}

uint getPageNumber(short v, short m) {
  
  //x is what we will use to mask with the address
  uint x;
  
  //shift maxNum to the left by m. this will move the one's to the left and give us n zero's at the right
  x = maxNum<<m;  //check(x); 
                  //example: 1111 1111 1111 1111 --> 111 1111 1111 1111 1000 0000
  
  //"and" x with maxNum. this will leave us with m one's
  x = x&maxNum;   //check(x);
                  //example: 111 1111 1111 1111 1000 0000 --> 000 0000 1111 1111 1000 0000
  
  //mask the address v with x
  uint result = v&x;  //check(v); check(result);
                      //example: v is 1100 0000 0000 0011 --> 1100 0000 0000 0000
  
  
  //shift the result to the right by n so we only get the left m bits
  result = result>> (n_plus_m-m); //check(result);
                                  //example: 1100 000
  
  return (result);
}

uint getOffset(short v, uint n) {
  //x is what we will use to mask with the address
  uint x;
  
  //shift maxNum to the right by n. this is the opposite of what we did above so this will give us n one's 
  x = maxNum>>n;  //check(x);
                  //example: 1111 1111 1111 1111 ---> 0000 0000 0111 1111
  
  //mask the address v with x
                  //check(v&x);
  return (v&x);   
                  //example: v is 1100 0000 0000 0011 --> 0000 0000 0000 0011
}

int main (int argc, char**argv){
  
  //users can try with their own input file
  //the provided file is "inputfile.txt"
  string filename; 
  cout << "Reading from file: "; 
  getline(cin, filename); 
  
  uint n; //the n lowest significant bits that represent the offset
  uint m; //the next m bits that represent the page number; assume that n+m is always 16
  uint v; //virtual address that needs to be mapped to a page number and offset
  
  //reading file variables
  string line; //line read
  ifstream myfile; //file to read
  
  //open file
  myfile.open (filename); 
  
  if (myfile.is_open()) //if the file exists
  {
    //get n
    getline(myfile,line);
    //assume input is a number and cast it to int
    n = stoi(line); 
    
    //get m
    getline(myfile,line);
    //to prevent any user input mistakes, we will calculate m
    m = n_plus_m-n;
    
    //while there's a next line, get the next line
    while ( getline (myfile,line) )
    {
      //assume input is a number and cast it to int
      v = stoi(line);
      printf("Virtual address %u ",v);
      //get page number
      uint p = getPageNumber(v,m);
      printf("is in page %u ",p);
      //get offset
      uint d = getOffset(v,n);
      printf("and offset %u. \n",d);
    }
    //close io stream
    myfile.close();
  }
  //if file can't be opened
  else cout << "Unable to open file"; 

  return 0;
}