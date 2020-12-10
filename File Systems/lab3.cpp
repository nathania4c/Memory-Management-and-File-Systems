#include <iostream>
#include <fstream>
#include <bitset>
#include <stdlib.h>
#include <stdint.h>
#include "inode.cpp"
using namespace std;

SuperBlock super;

class myFileSystem
{
public:
  myFileSystem(string diskName)
  {
    // Open the file with name diskName
    ifstream myFile;
    myFile.open(diskName);
    // Read the first 1KB and parse it to structs/objecs representing the super block
    // 	An easy way to work with the 1KB memory chunk is to move a pointer to a
    //	position where a struct/object begins. You can use the sizeof operator to help
    //	cleanly determine the position. Next, cast the pointer to a pointer of the
    //	struct/object type.
    
    // Be sure to close the file in a destructor or otherwise before
    // the process exits.
    myFile.close();
  }
  
  int create(char name[8], int32_t size)
  { printf("creating");
    
    //create a file with this name and this size
    // high level pseudo code for creating a new file
    
    // Step 1: Look for a free inode by searching the collection of objects
    // representing inodes within the super block object.
    // If none exist, then return an error.
    // Also make sure that no other file in use with the same name exists.

    int sameChar = 0;
    int free = -1;
    for (int i = 0; i < super.inodes.size(); i++){
      inode *node = super.inodes[i];
      for (int j = 0; j<8;j++){
        if ((*node).name[j] == name[j])
          sameChar++;
      }
      if (sameChar == 8){
        printf("name exists");
        return 0;
      }
      sameChar = 0;
      if ((*node).used == 0){
        free = i;
        break;
      }
    }
    
    if (free == -1){
      printf("no free nodes");
      return 0;
    }
    
    // Step 2: Look for a number of free blocks equal to the size variable
    // passed to this method. If not enough free blocks exist, then return an error.
    
    // Step 3: Now we know we have an inode and free blocks necessary to
    // create the file. So mark the inode and blocks as used and update the rest of
    // the information in the inode.
    
    inode *node = super.inodes[free];
    node -> used = 1;
    for (int i = 0; i < 8; i++){
      node ->name[i] = name[i];
    }
    node -> size = size;
    
    // Step 4: Write the entire super block back to disk.
    //	An easy way to do this is to seek to the beginning of the disk
    //	and write the 1KB memory chunk.
    
    
    
    return 0;
  } // End Create
  
  int deletee(char name[8]){
    
    printf("deleting");
    
    // Delete the file with this name
    
    // Step 1: Look for an inode that is in use with given name
    // by searching the collection of objects
    // representing inodes within the super block object.
    // If it does not exist, then return an error.
    
    // Step 2: Free blocks of the file being deleted by updating
    // the object representing the free block list in the super block object.
    
    // Step 3: Mark inode as free.
    
    // Step 4: Write the entire super block back to disk.
    return 0;
  } // End Delete
  
  
  int ls(void)
  { 
    
    printf("ls-ing");
    
    // List names of all files on disk
    
    // Step 1: Print the name and size fields of all used inodes.
    return 0;
  } // End ls
  
  int read(char name[8], int32_t blockNum, char buf[1024])
  {
    printf("reading");
    
    // read this block from this file
    // Return an error if and when appropriate. For instance, make sure
    // blockNum does not exceed size-1.
    
    // Step 1: Locate the inode for this file as in Step 1 of delete.
    
    // Step 2: Seek to blockPointers[blockNum] and read the block
    // from disk to buf.
    return 0;
  } // End read
  
  
  int write(char name[8], int32_t blockNum, char buf[1024])
  {
    printf("writing");
    
    // write this block to this file
    // Return an error if and when appropriate.
    
    // Step 1: Locate the inode for this file as in Step 1 of delete.
    
    // Step 2: Seek to blockPointers[blockNum] and write buf to disk.
    return 0;
  } // end write
  
};
