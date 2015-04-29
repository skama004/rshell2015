#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> //USES DIR, DIRENT, OPENDIR, ETC.
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int blocks(char* directoryname, int flag)
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
		if(errno!=0)
		{
			perror("readdir");
		}
		struct stat x;

		char path[1000];

		strcpy(path, directoryname);
		strcat(path, "/");
		strcat(path, currentdirectory->d_name);
		if (stat(path, &x) == -1)
		{
			perror("stat");
		}

		if (flag % 2 == 0)
		{
			blk += x.st_blocks;
		}

		else
		{
			if (currentdirectory->d_name[0] != '.')
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


void recursioncall(char *directoryname, int flag)
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
	}
}


int main()
{
	return 0;
}







