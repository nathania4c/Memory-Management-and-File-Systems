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

class myFileSystem
{
public:
  inode inodes[16];
  int freeBlocks[128];
  char chunk[1024];
  
  myFileSystem(string diskName)
  {
    cout << "creating file system " << diskName << "\n";
    // Open the file with name diskName
    std::ifstream infile;
    infile.open(diskName, ios::in | ios::binary);
    infile.read(chunk,1024);
    int currentPointer = 128;
    for (int x = 0; x<16;x++) //for 16 inodes
    {
      for (int y = 0; y<8;y++)
      {
        inodes[x].setName(y,chunk[currentPointer]);
        currentPointer++;
      }
      uint32_t size = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
      currentPointer = currentPointer + 4;
      inodes[x].setSize(size);
      for (int y = 0;y<8;y++)
      {
        uint32_t blockVal = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
        currentPointer = currentPointer + 4;
        inodes[x].blockPointers[y] = blockVal;
      }
      uint32_t used = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
      currentPointer = currentPointer+4;
      inodes[x].setUsed(used);
    }
    infile.close();
  }
  
  int create(char name[8], int32_t size)
  { //create a file with this name and this size
    
    // high level pseudo code for creating a new file
    
    // Step 1: Look for a free inode by searching the collection of objects
    // representing inodes within the super block object.
    // If none exist, then return an error.
    // Also make sure that no other file in use with the same name exists.
      
      // Step 2: Look for a number of free blocks equal to the size variable
      // passed to this method. If not enough free blocks exist, then return an error.
      
      // Step 3: Now we know we have an inode and free blocks necessary to
      // create the file. So mark the inode and blocks as used and update the rest of
      // the information in the inode.
      
      // Step 4: Write the entire super block back to disk.
      //	An easy way to do this is to seek to the beginning of the disk
      //	and write the 1KB memory chunk.
      return 0;
  } // End Create
  
  
  int deletee(char name[8])
  {
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
    // List names of all files on disk
    
    // Step 1: Print the name and size fields of all used inodes.
    return 0;
  } // End ls
  
  int read(char name[8], int32_t blockNum, char buf[1024])
  {
    
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
    
    // write this block to this file
    // Return an error if and when appropriate.
    
    // Step 1: Locate the inode for this file as in Step 1 of delete.
    
    // Step 2: Seek to blockPointers[blockNum] and write buf to disk.
    return 0;
  } // end write	
};