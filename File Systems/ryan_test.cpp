#include <stdio.h>
#include "inode.cpp"

class myFileSystem {
public:
  
  inode inodes[16]; //array of inodes
  char freeBlocks[127]; //keep track of which blocks are free
  int availableBlocks; //keep track of how many blocks are free
  std::string diskName; //disk name
  
  //constructor
  myFileSystem(string diskName2) {
    
    // Open the file with name diskName
    
    // Read the first 1KB and parse it to structs/objecs representing the super block
    // 	An easy way to work with the 1KB memory chunk is to move a pointer to a
    //	position where a struct/object begins. You can use the sizeof operator to help
    //	cleanly determine the position. Next, cast the pointer to a pointer of the
    //	struct/object type.
    
    // Be sure to close the file in a destructor or otherwise before
    // the process exits.
    
    cout << "creating file system object for " << diskName2 << "\n";
    
    //set attributes
    diskName = "disk0";	
    //TODO: If diskName = diskName2 it becomes "disk0?" 
    //diskName = diskName2;
    
    //open file
    std::ifstream infile;
    infile.open(diskName, std::ios::in | std::ios::binary);
    char chunk[1024]; //super block
    infile.read(chunk,sizeof(chunk));
    
    availableBlocks = 127;
    //initialize free blocks list with 0
    for(int i = 0; i < sizeof(freeBlocks); i++){
      freeBlocks[i] = '0';
    }
   
   //initialize 16 inodes
   for (int x = 0; x<16;x++) 
   {
   	char name[8]; 
   	for (int y = 0; y<8;y++)
   	{
   		name[y] = 'x';
   	  inodes[x].setBlock(y,1);
   	}
   	inodes[x].setName(name);
   	inodes[x].setUsed(0);
   	inodes[x].setSize(0);
   }
   
   //add to chunk
   
    int pos = 1;
    
   //first add free block list
   for (int i = 0; i < sizeof(freeBlocks);i++){
     chunk[pos] = freeBlocks[i];
     pos++;
   }
   
   //next add inode attributes
   for (int x = 0; x<16;x++) {
     //name
     for (int i = 0; i < sizeof(inodes[x].getName()); i++){
       chunk[pos] = inodes[x].getName()[i];
       pos++;
     }
     //block pointer
     for (int i = 0; i<sizeof(inodes[x].getBlock()); i++){
       char c = '0' + static_cast<char>(inodes[x].getBlock()[i]);
        chunk[pos] = c;
       pos++;
     }
     //used
     char c = '0' + static_cast<char>(inodes[x].getUsed());
     chunk[pos]= c;
     pos++;
     //size
     c = '0' + static_cast<char>(inodes[x].getSize());
     chunk[pos] = c;
     pos++;
   }
   
   //write to disk
   std::ofstream outfile;
   outfile.open(diskName, fstream::app | std::ios::binary);
   outfile.seekp(1);
    outfile.write(chunk, sizeof(chunk));
   //close file
	outfile.close();
}

int create(char name[8], uint32_t size)
{ //create a file with this name and this size

  printf("creating file %s of size %i \n",name,size);
  char chunk[1024];
  
  // high level pseudo code for creating a new file

  // Step 1: Look for a free inode by searching the collection of objects
  // representing inodes within the super block object.
  // If none exist, then return an error.
  // Also make sure that no other file in use with the same name exists.
  
  // Step 2: Look for a number of free blocks equal to the size variable
  // passed to this method. If not enough free blocks exist, then return an error.
  
  int free = -1;
  for (int x = 0; x<16;x++)
  {
    //if file name already exists
    std::string str1 (inodes[x].getName());
    std::string str2 (name);
    if (str1.compare(str2)==0)
    {
      std::cout << "DUPLICATE NAME\n";
      return 0;
    } 
    //if inode is free
    else if (inodes[x].getUsed() == 0)
  	{
  	  //get the index
  		free = x;
  	  break;
  	}
  }
  //if no free inode
  if (free == -1)
  {
  	std::cout << "MAX NUM OF FILES\n";
    return 0;
  }
  //if not enough available blocks
  else if (availableBlocks < size)
  {
  	std::cout << "NOT ENOUGH SPACE\n";
    return 0;
  }

  // Step 3: Now we know we have an inode and free blocks necessary to
  // create the file. So mark the inode and blocks as used and update the rest of
  // the information in the inode.
  
  else
  {
  	int s = size;
    //update available blocks count
  	availableBlocks = availableBlocks-size;
  	//set inode attributes
  	inodes[free].setName(name);
  	inodes[free].setUsed(1);
  	inodes[free].setSize(size);
  	//update freeblocks
  	int pointer = 0;
  	for (int x = 1;x<128;x++)
  	{
  		if (freeBlocks[x] == '0')
  		{
  			inodes[free].setBlock(pointer,x);
  			s--;
  			pointer++;
  			freeBlocks[x] = '1';
  		}
  		if (s == 0)
  		{
  			break;
  		}
  	}
  }

  // Step 4: Write the entire super block back to disk.
  //	An easy way to do this is to seek to the beginning of the disk
  //	and write the 1KB memory chunk.
  
  std::ofstream fileOut(diskName,fstream::app | ofstream::binary);
  fileOut.seekp(1);
  for (int i = 0; i < size; i++)
    fileOut.write(chunk,sizeof(chunk));
  int pos = (48*free)+128;
  fileOut.seekp(pos);
  fileOut.write(name,8);
  pos = pos+8;
  fileOut.write(reinterpret_cast<const char *>(&size), sizeof(size));
  pos = pos+4;
  fileOut.write(reinterpret_cast<const char *>(inodes[free].getBlock()), sizeof(inodes[free].getBlock()));
  pos = pos+sizeof(inodes[free].getBlock());
  uint32_t used = inodes[free].getUsed();
  fileOut.write(reinterpret_cast<const char *>(&used),(4));
  
  return 0;

} // End Create

void del(char name[8])
{
  // Delete the file with this name
  printf("deleting file %s \n", name);
  //search through files
  for(int x = 0;x<16;x++)
  {
    //if name matches
    std::string str1 (inodes[x].getName());
    std::string str2 (name);
    if (str1.compare(str2)==0)
  	{
  		char blankName[8];
  		inodes[x].setName(blankName);
  		uint32_t* blocks = inodes[x].getBlock();
  		for (int y = 0;y<(int)inodes[x].getSize();y++)
  		{
  			freeBlocks[blocks[y]] = '0';
  			inodes[x].setBlock(y,0);
  		}
  		availableBlocks = availableBlocks+inodes[x].getSize();
  		inodes[x].setSize(0);
  		inodes[x].setUsed(0);
  		int pos = (48*x)+128;
  		std::ofstream fileOut;
  		fileOut.open(diskName, std::ios::out | std::ios::binary);
  		fileOut.seekp(pos);
  		fileOut.write(blankName,8);
  	 	pos = pos+8;
  	 	fileOut.write(reinterpret_cast<const char *>(&inodes[x]), 4);
  		pos = pos+4;
  		uint32_t *block = inodes[x].getBlock();
  		fileOut.write(reinterpret_cast<const char *>(&blocks), sizeof(blocks));
  		pos = pos+sizeof(blocks);
  		fileOut.write(reinterpret_cast<const char *>(&inodes[x]),4);
  		return;
  	}
  }
  //if file not found
  std::cout<<"FILE NOT FOUND\n";

} // End Delete


void ls(void)
{ 
  // List names of all files on disk
  printf("Listing all files on disk \n");
  //iterate through the inodes
  for (int x=0;x<16;x++)
  {
    //if the inode is used
  	if(inodes[x].getUsed() == 1)
  	{
  	  //print file name
  		std::string str (inodes[x].getName());
  		str = str + std::to_string(inodes[x].getSize()) + "\n";
  		std::cout << str;
  	}
  }

} // End ls

void read(char name[8], uint32_t blockNum, char buf[1024])
{
  printf("reading file %s at block %i \n",name,blockNum);
   for (int x=0;x<16;x++)
   {
      std::string str1 (inodes[x].getName());
      std::string str2 (name);
   	if (str1.compare(str2)==0)
   	{
   		if (blockNum < inodes[x].getSize()-1)
   		{
   			uint32_t* blocks = inodes[x].getBlock();
   			std::ifstream infile;
   			infile.open(diskName, std::ios::in | std::ios::binary);
   			infile.seekg(128+((blocks[(int)blockNum]*1024)));
   			infile.read(buf,1024);
   			return;
   		}
   		//if block number bigger than size
   		else
   		{
   			std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
   			return;
   		}
   	}
   }
   //if file not found
   std::cout << "FILE NOT FOUND\n";

} // End read


void write(char name[8], uint32_t blockNum, char buf[1024])
{
  printf("writing to file %s at block %i \n",name,blockNum);
  //search through files
   for (int x=0;x<16;x++) {
     //if file name matches
     std::string str1 (inodes[x].getName());
     std::string str2 (name);
     if (str1.compare(str2)==0){
   	  //if block number smaller than size
   		if (blockNum < inodes[x].getSize()-1) {
   		  //write to file
   			uint32_t* blocks = inodes[x].getBlock();
   			std::ofstream outfile;
   			outfile.open(diskName, fstream::app);
   			outfile.seekp(128+(blocks[(int)blockNum]*1024));
   			outfile.write(buf,1024);
   			return;
   		} 
   		//if block number bigger than size
   		else {
   			std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
   			return;
   		}
   	}
   }
   //if file not found
   std::cout << "FILE NOT FOUND\n";
   
} // end write	
  
};