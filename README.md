# Memory-Management-and-File-Systems
#### Members: 

## Paging
### How to run
Compile the Paging.cpp file by typing 'g++ Paging.cpp -o run' then './run' .

The program will ask you for the name of the input file. To use the one provided, type 'inputfile.txt' .

The program will process each memory address and print the page number and offset before reading the next memory address .

## File Systems
### How to run
To run the create_fs file, type 'g++ lab3.create_fs.cpp -o create_fs' then ' ./create_fs disk0'

Then, to run the program, type 'g++ index.cpp -o run' then './run'

### General Notes and Concerns

The program will read file "lab3input.txt" and execute the commands line by line (i.e. will not read the next line until the previous command is finished). 
- To help the user follow the process, it is also set to sleep for 1 time unit (if not it just goes by too fast). You can comment out this line if you want to.
- To help debug, comment out the "sleep" and uncomment the "printf" and "readChar" lines so you can go through each line at your own pace.

When creating the file system, the name of the disk must be "disk0"
- When we tried to set it equal to the input, it became "disk0?" and we couldn't figure out how to fix this

When writing the inodes to disk, the program writes the name first, then the block pointer, then the used int, then the size.
- It is worth noting that the inode does not take up 48 bytes, but 25 instead (we couldn't figure this out)

When reading the data from disk, the program doesn't print the data by default, but you can uncomment the "printf" in the method to print the data.

It is worth nothing that when calling the del method, the sample input "D file3.c" works but the input "D lab1.jav" does not

Sorry if there are actually more problems that we did not mentioned

### Sample output
The file "disk0" should contain the output that was generated from the above input file
