#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> //USES DIR, DIRENT, OPENDIR, ETC.
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <algorithm> 
#include <vector>
using namespace std;

int blocks(char* directoryname, int flag) //Acts as a helper for dir syscalls
{
	int blk = 0;
	DIR *dirStream;
	dirent* currentdirectory;

	if (!(dirStream = opendir(directoryname)))
	{
		perror("opendir"); // Error with opendir syscall
	}
	
	while((currentdirectory = readdir(dirStream))) //Read directory used here
	{
		if(errno!=0) //Will return an error if anything but 0
		{
			perror("readdir");
		}
		struct stat x; //Create a struct stat 

		char path[1000];

		strcpy(path, directoryname);
		strcat(path, "/");
		strcat(path, currentdirectory->d_name);
		if (stat(path, &x) == -1) //Stat returns -1 if error
		{
			perror("stat");
		}

		if (flag % 2 == 0) //Mod by 2
		{
			blk += x.st_blocks;
		}

		else
		{
			if (currentdirectory->d_name[0] != '.') //Checks for . 
			{
				blk += x.st_blocks;
			}
		}
	}
	if (closedir(dirStream) == -1)
	{
		perror("closedir");
	}
	return blk;
}


void recursioncall(char *directoryname, int flag) //Function for -R
{
	DIR* dirStream;
	dirent* currentdirectory;

	if(flag%5 == 0)//Sets up for -R
	{
		cout << directoryname << ": " << endl;
	}

	if (!(dirStream = opendir(directoryname)))
	{
		perror("opendir"); //Error with opendir syscall
		exit(1); //Exit 1 if error
	}

	int block;

	if(flag%3 == 0)
	{
		block = blocks(directoryname, flag);
		cout << "Total: " << (block/2) << endl;
	}

	vector <char*> cstrings;

	while ((currentdirectory = readdir(dirStream)))
	{
		cstrings.push_back(currentdirectory->d_name);//Copy contents of file into cstrings 
	}
	sort(cstrings.begin(), cstrings.end()); //Sort the cstrings "vector" to be in order

	for (unsigned i = 0; i < cstrings.size(); i++)
	{
		if (errno!= 0)
		{
			perror("readdir"); //Error if not equal to 0 with errno
		}
	}
}


int main()
{
	return 0;
}







