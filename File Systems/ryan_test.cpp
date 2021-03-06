#include "inode.cpp"

class myFileSystem {
public:
  
  inode inodes[16]; //array of inodes
  char freeBlocks[127]; //keep track of which blocks are free
  char superBlock[1024];
  int availableBlocks; //keep track of how many blocks are free
  std::string diskName; //disk name
  
  //constructor
  myFileSystem(string diskName2) {
    
    cout << "creating file system object for " << diskName2 << "\n";
    
    //set attributes
    diskName = "disk0";	
    //TODO: If diskName = diskName2 it becomes "disk0?" 
    //diskName = diskName2;
    
    //open file
    std::ifstream infile;
    infile.open(diskName, std::ios::in | std::ios::binary);
    //read super block
    infile.read(superBlock,sizeof(superBlock));
    
    //initialize available blocks count
    availableBlocks = 127;
    //initialize free blocks list with 0
    for(int i = 0; i < sizeof(freeBlocks); i++){
      freeBlocks[i] = '0';
    }
   
   //initialize 16 inodes
   for (int x = 0; x<16;x++) {
   	char name[8]; 
   	for (int y = 0; y<8;y++) {
   		name[y] = 'x'; //8 spaces as placeholder for name
   	  inodes[x].setBlock(y,0); //0s as placeholder for block pointer
   	}
   	inodes[x].setName(name);
   	inodes[x].setUsed(0); //0 means free
   	inodes[x].setSize(0); //initially size 0
   }
   
   //add to chunk
   int pos = 1; //chunk pointer
    
   //first add free block list
   for (int i = 0; i < sizeof(freeBlocks);i++){
     superBlock[pos] = freeBlocks[i];
     //update pos
     pos++;
   }
   
   //next add inode attributes
   for (int x = 0; x<16;x++) {
     
     //add name
     for (int i = 0; i < sizeof(inodes[x].getName()); i++){
       superBlock[pos] = inodes[x].getName()[i];
       //update pos
       pos++;
     }
     
     //add block pointer
     for (int i = 0; i<sizeof(inodes[x].getBlock()); i++){
       //convert int to char
       char c = '0' + static_cast<char>(inodes[x].getBlock()[i]);
       superBlock[pos] = c;
       //update pos
       pos++;
     }
     
     //add used
     char c = '0' + static_cast<char>(inodes[x].getUsed());
     superBlock[pos]= c;
     //update pos
     pos++;

     //add size
     char * sizeBinary = inodes[x].getSizeBinary();
     for (int i = 0; i < 8;i++){
       superBlock[pos] = sizeBinary[i];
       //update pos
       pos++;
     }
   }
   
   //write to disk
   std::ofstream outfile;
   outfile.open(diskName, ios::out | std::ios::binary);
   outfile.seekp(1);
   outfile.write(superBlock, sizeof(superBlock));
   
   //close file
	 outfile.close();
}
  
  //this method is for updating the file after we change something
void writeToDisk(int x, char buf[1024], uint32_t blockNum, uint32_t howManyBlocks){
    
    //the inode
    inode dudu = inodes[x];
    
    //open file
    std::ifstream infile;
    infile.open(diskName, std::ios::in | std::ios::binary);
    //read super block
    infile.read(superBlock,sizeof(superBlock));
    
    //pointer
    int pos = 1; 
    
    //first update free block list
    for (int i = 0; i < sizeof(freeBlocks); i++){
      superBlock[pos+i] = freeBlocks[pos+i];
    }
    
    //similar to when creating disk but here we only change the ones changed
    //next update inode attributes
    pos = pos+ 127;
    pos = pos + x*25;
    //TODO: should be adding by x*48?
    
    //update name
    for (int i = 0; i < 8;i++){
      //if name less than 8, write x for the remaining space
      if(dudu.getName()[i]==0){
        superBlock[pos]='x';
      } else{
        superBlock[pos] = dudu.getName()[i];
      }
      pos++;
    }
    
    //update block pointer
    for (int i = 0; i<sizeof(dudu.getBlock()); i++){
      char c = '0' + static_cast<char>(dudu.getBlock()[i]);
      superBlock[pos] = c;
      pos++;
    }
      
    //update used
    char c = '0' + static_cast<char>(dudu.getUsed());
    superBlock[pos]= c;
    pos++;
    
    //update size
    char * sizeBinary = inodes[x].getSizeBinary();
    for (int i = 0; i < 8;i++){
      superBlock[pos] = sizeBinary[i];
      pos++;
    }
    
    //read the rest of the file
    char buffer[127][1024];
    for(int i = 0; i < 127; i++){
      infile.read(buffer[i],sizeof(buffer[i]));
    } 
    
    //close the input stream
    infile.close();
    
    //write to disk
    std::ofstream outfile;
    outfile.open(diskName, std::ios::binary);
    outfile.seekp(1);
    outfile.write(superBlock, sizeof(superBlock));
    
    //update the data in block()s)
    for (int j = 0; j < howManyBlocks; j++){
      for(int i = 0; i<1024;i++){
        buffer[dudu.getBlockPointer()+blockNum+j][i] = buf[i];
      }
    }
    
    //write back all the blocks
    pos = sizeof(superBlock)+1;
    for(int i = 0; i<127;i++){
      outfile.seekp(pos);
      outfile.write(buffer[i], sizeof(buffer[i]));
      pos = pos + 1024;
    }
    
    //close file
    outfile.close();
  }

int create(char name[8], uint32_t size) { 
  
  //create a file with this name and this size
  printf("creating file %s of size %i \n",name,size);

  // Step 1: Look for a free inode by searching the collection of objects
  // representing inodes within the super block object.
  // If none exist, then return an error.
  // Also make sure that no other file in use with the same name exists.
  // +
  // Step 2: Look for a number of free blocks equal to the size variable
  // passed to this method. If not enough free blocks exist, then return an error.
  
  char chunk[1024];
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
  if (availableBlocks < size)
  {
  	std::cout << "NOT ENOUGH SPACE\n";
    return 0;
  }

  // Step 3: Now we know we have an inode and free blocks necessary to
  // create the file. So mark the inode and blocks as used and update the rest of
  // the information in the inode.
  
  //update available blocks count
  availableBlocks = availableBlocks-size;
  //set inode attributes
  inodes[free].setName(name);
  inodes[free].setUsed(1);
  inodes[free].setSize(size);
  //update free blocks list
  int index = 0;
  int freeCount[size];
  int s = size;
  for (int x = 1;x<128;x++)
  {
    //if block is free
    if (freeBlocks[x] == '0')
    {
      //store in count
      freeCount[index] = x;
      s--;
      index++;
    }
    //if the next one is 1 but s is not 0 yet
    if(freeBlocks[x+1]=='1' && s!=0){
      s = size;
      index = 0;
      continue;
    }
    //if done with all blocks
    if (s == 0)
    {
      break;
    }
  }
  
  //set start of block pointer
  inodes[free].setBlockPointer(freeCount[0]);
  
  for (int i = 0; i < size; i++){
    freeBlocks[freeCount[i]] = '1';
  }
  
  
  // Step 4: Write the entire super block back to disk.
  //	An easy way to do this is to seek to the beginning of the disk
  //	and write the 1KB memory chunk.
  
  //marking the blocks for this file with*
  char buf[1024];
  for (int i = 0; i < sizeof(buf);i++){
    buf[i] = '*';
  }
  writeToDisk(free,buf,0,size);
  
  return 0;

} // End Create

int del(char name[8]) {
  
  // Delete the file with this name
  printf("deleting file %s \n", name);
  inode dudu;
  int index = -1;
  
  //search through files
  std::string str2 (name);
  for(int x = 0;x<16;x++) {
    //if name matches
    std::string str1 (inodes[x].getName());
    if (str1.compare(str2)==0) {
      dudu=inodes[x];
      index = x;
      break;
    }
  }
  if (index == -1){
    //if file not found
    std::cout<<"FILE NOT FOUND\n";
    return 0;
  }
  
  //pointer
  int pos = 1 + dudu.getBlockPointer();
  
  //update free blocks list
  for(int i = 0; i < dudu.getSize();i++){
    freeBlocks[pos+i] = '0';
  }
  //update available blocks count
  availableBlocks = availableBlocks+dudu.getSize();
  
  //save info
  uint32_t size = dudu.getSize();
  uint32_t bp = dudu.getBlockPointer();
  
  //reset inode info
  dudu.resetName();
  dudu.resetBlockPointer();
  dudu.setUsed(0);
  dudu.setSize(0);
  
  //creating empty block data
  char buf[1024];
  
  //open file
  std::ifstream infile;
  infile.open(diskName, std::ios::in | std::ios::binary);
  //read super block
  infile.read(superBlock,sizeof(superBlock));
  
  //basically the writeToDisk with minor changes
  
  pos = 1; //pointer
  
  //first update free block list
  for (int i = 0; i < sizeof(freeBlocks); i++){
    superBlock[pos+i] = freeBlocks[pos+i];
  }
  
  //only change the ones changed
  //next update inode attributes
  pos = pos+ 127;
  pos = pos + index*25;
  //TODO: should be adding by x*48?
  
  //update name
  for (int i = 0; i < 8;i++){
    //if name less than 8, write x for the remaining space
    if(dudu.getName()[i]==0){
      superBlock[pos]='x';
    } else{
      superBlock[pos] = dudu.getName()[i];
    }
    pos++;
  }
  
  //update block pointer
  for (int i = 0; i<sizeof(dudu.getBlock()); i++){
    char c = '0' + static_cast<char>(dudu.getBlock()[i]);
    superBlock[pos] = c;
    pos++;
  }
  
  //update used
  char c = '0' + static_cast<char>(dudu.getUsed());
  superBlock[pos]= c;
  pos++;
  
  //update size
  char * sizeBinary = dudu.getSizeBinary();
  for (int i = 0; i < 8;i++){
    superBlock[pos] = sizeBinary[i];
    pos++;
  }
  
  //read the rest of the file
  char buffer[127][1024];
  for(int i = 0; i < 127; i++){
    infile.read(buffer[i],sizeof(buffer[i]));
  } 
  
  //close the input stream
  infile.close();
  
  //write to disk
  std::ofstream outfile;
  outfile.open(diskName, std::ios::binary);
  outfile.seekp(1);
  outfile.write(superBlock, sizeof(superBlock));
  
  //update block data
  for (int j = 0; j < size; j++){
    for(int i = 0; i<1024;i++){
      buffer[bp+0+j][i] = buf[i];
    }
  }
  
  //write back all the blocks
  pos = sizeof(superBlock)+1;
  for(int i = 0; i<127;i++){
    outfile.seekp(pos);
    outfile.write(buffer[i], sizeof(buffer[i]));
    pos = pos + 1024;
  }
  
  //close file
  outfile.close();
  
  //update inodes
  for (int i = index;i<16;i++){
    inodes[i] = inodes[i+1];
  }
  
  return 0;

} // End Delete


int ls(void) { 
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
  		str = str + " "+std::to_string(inodes[x].getSize()) + "\n";
  		std::cout << str;
  	}
  }
  return 0;
} // End ls

int read(char name[8], uint32_t blockNum, char buf[1024]) {
  
  printf("reading file %s at block %i \n",name,blockNum);
  
  std::string str2 (name);
  //search through inodes
   for (int x=0;x<16;x++)
   {
     //if name matches
      std::string str1 (inodes[x].getName());
   	if (str1.compare(str2)==0)
   	{
   	  //if block number bigger than size
   		if (blockNum > inodes[x].getSize()-1){
   		  std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
   		  return 0;
   		} else {
   		  //get inode
   		  inode dudu = inodes[x];
   		  //open file
   			std::ifstream infile;
   			infile.open(diskName, std::ios::in | std::ios::binary);
   			//set pointer
   			int pos = 1024*(dudu.getBlockPointer()+blockNum);
   			infile.seekg(pos);
   			//read
   			infile.read(buf,1024);
   			//printf("%s \n",buf);
   			return 0;
   		}
   	}
   }
   //if file not found
   std::cout << "FILE NOT FOUND\n";
   return 0;
} // End read


int write(char name[8], uint32_t blockNum, char buf[1024]) {
  
  printf("writing to file %s at block %i \n",name,blockNum);
  
  //search through files
  std::string str2 (name);
   for (int x=0;x<16;x++) {
     //if file name matches
     std::string str1 (inodes[x].getName());
     if (str1.compare(str2)==0){
       //if block number bigger than size of file
       if(blockNum > inodes[x].getSize()-1){
         std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
         return 0;
       } else {
         writeToDisk(x, buf, blockNum,1);
         break;
       }
     }
     if(x+1==16){
       //if file not found
       std::cout << "FILE NOT FOUND\n";
     }
   }
   return 0;
   
} // end write	
  
};