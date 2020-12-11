#include "inode.cpp"

class myFileSystem
{
inode inodes[16];
char freeBlocks[127];
char chunk[1024];
int availableBlocks;
std::string diskName;
public:
  myFileSystem(std::string diskName2)
{
    cout << "creating file system " << diskName << "\n";
   diskName = diskName2;	
   // Open the file with name diskName
   std::ifstream infile;
   infile.open(diskName, std::ios::in | std::ios::binary);
   infile.read(chunk,1024);
   int currentPointer = 128;
   for (int x = 0; x<16;x++) //for 16 inodes
   {
   	char name[8]; 
   	for (int y = 0; y<8;y++)
   	{
   		name[y] = chunk[currentPointer];
   		currentPointer++;
   	}
   	inodes[x].setName(name);
   	uint32_t input = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
   	currentPointer = currentPointer + 4;
   	inodes[x].setSize(input);
   	uint32_t* blocks = inodes[x].getBlock();
   	for (int y = 0;y<8;y++)
   	{
   		uint32_t blockVal = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
   		currentPointer = currentPointer + 4;
   		blocks[y] = blockVal;
   	}
   	uint32_t used = (chunk[currentPointer] << 24) | (chunk[currentPointer+1] << 16) | (chunk[currentPointer+2] << 8) | (chunk[currentPointer+3]);
   	currentPointer = currentPointer+4;
   	inodes[x].setUsed(used);
   }
	infile.close();
}


void create(char name[8], uint32_t size)
{ //create a file with this name and this size

  printf("creating file %s of size %i \n",name,size);
  
  // high level pseudo code for creating a new file

  // Step 1: Look for a free inode by searching the collection of objects
  // representing inodes within the super block object.
  // If none exist, then return an error.
  // Also make sure that no other file in use with the same name exists.
  int free = -1;
  int isDup;
  for (int x = 0; x<16;x++)
  {
  	if (inodes[x].getUsed() == 0)
  	{
  		free = x;
  	}
  	else if (inodes[x].getName() == name)
  	{
  		isDup = 1;
  		break;
  	}
  }
  if (isDup == 1)
  {
  	std::cout << "DUPLICATE NAME\n";
  }
  else if (free == -1)
  {
  	std::cout << "MAX NUM OF FILES\n";
  }
  else if (availableBlocks < size)
  {
  	std::cout << "NOT ENOUGH SPACE\n";
  }
  else
  {
  	int size2 = size;
  	availableBlocks = availableBlocks-size;
  	inodes[free].setName(name);
  	inodes[free].setUsed(1);
  	inodes[free].setSize(size);
  	int pointer = 0;
  	for (int x = 1;x<128;x++)
  	{
  		if (freeBlocks[x] == '0')
  		{
  			inodes[free].setBlock(pointer,x);
  			size2--;
  			pointer++;
  			freeBlocks[x] = '1';
  		}
  		if (size2 == 0)
  		{
  			break;
  		}
  	}
  }
  
  std::ofstream fileOut(diskName,std::ios::out | std::ios::binary);
  fileOut.seekp(1);
  fileOut.write(freeBlocks,127);
  int pos = (48*free)+128;
  fileOut.seekp(pos);
  fileOut.write(name,8);
  pos = pos+8;
  fileOut.write(reinterpret_cast<const char *>(&size), sizeof(size));
  pos = pos+4;
  uint32_t* blocks = inodes[free].getBlock();
  fileOut.write(reinterpret_cast<const char *>(&blocks), sizeof(blocks));
  pos = pos+sizeof(blocks);
  uint32_t used = inodes[free].getUsed();
  fileOut.write(reinterpret_cast<const char *>(&used),(4));

} // End Create



void del(char name[8])
{
  printf("deleting file %s \n", name);
  // Delete the file with this name
  for(int x = 0;x<16;x++)
  {
  	if (inodes[x].getName() == name)
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
  		break;
  	}
  	std::cout<<"FILE NOT FOUND\n";
  }
  

} // End Delete


void ls(void)
{ 
  printf("Listing all files on disk \n");
  // List names of all files on disk
  for (int x=0;x<16;x++)
  {
  	if(inodes[x].getUsed() == 1)
  	{
  		std::string str (inodes[x].getName());
  		str = str + "   " + std::to_string(inodes[x].getSize()) + "\n";
  		std::cout << str;
  	}
  }

} // End ls

void read(char name[8], uint32_t blockNum, char buf[1024])
{
  printf("reading file %s at block %i \n",name,blockNum);
   for (int x=0;x<16;x++)
   {
   	if (inodes[x].getName() == name)
   	{
   		if (blockNum > inodes[x].getSize()-1)
   		{
   			uint32_t* blocks = inodes[x].getBlock();
   			std::ifstream infile;
   			infile.open(diskName, std::ios::in | std::ios::binary);
   			infile.seekg(128+((blocks[(int)blockNum]*1024)));
   			infile.read(buf,1024);
   			break;
   		}
   		else
   		{
   			std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
   			break;
   		}
   	}
   	std::cout << "FILE NOT FOUND";
   }

} // End read


void write(char name[8], uint32_t blockNum, char buf[1024])
{
  printf("writing to file %s at block %i \n",name,blockNum);
   for (int x=0;x<16;x++)
   {
   	if (inodes[x].getName() == name)
   	{
   		if (blockNum > inodes[x].getSize()-1)
   		{
   			uint32_t* blocks = inodes[x].getBlock();
   			std::ofstream outfile;
   			outfile.open(diskName, std::ios::out | std::ios::binary);
   			outfile.seekp(128+(blocks[(int)blockNum]*1024));
   			outfile.write(buf,1024);
   			break;
   		}
   		else
   		{
   			std::cout << "BLOCK NUMBER GREATER THAN SIZE\n";
   			break;
   		}
   	}
   	std::cout << "FILE NOT FOUND\n";
   }
   
} // end write	
};