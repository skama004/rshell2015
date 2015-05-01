#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> //USES DIR, DIRENT, OPENDIR, ETC.
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <algorithm> 
#include <ctime>
#include <time.h>
#include <iomanip>
#include <vector>
using namespace std;

int blocks(char* directoryname, int flag) //get total number of blocks and divide by block size working with bytes
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
		//directoryname/d_name

		if (stat(path, &x) == -1) //Stat returns -1 if error
		{
			perror("stat");
		}

		if (flag % 2 == 0) //If even
		{
			blk += x.st_blocks; //st_blocks = 512B allocated, added to variable blk
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

void printl(struct stat x, char* str, char* name)
{
	string directory = str;
	string cmd = "stat --printf= '%A %h %U %G %s' " + directory;
	system(cmd.c_str()); //Essentially executes cmd

	char temp[18];
	struct tm *Time;
	Time = localtime(&(x.st_mtime)); 
	strftime(temp, 18, "%b %d %H:%M", Time);//passes in time, month, day, hour, minute

	cout <<left << setw(15) << ' ' << temp << ' '; //Sets width to 15 and outputs left operand

	if ((x.st_mode & S_IFDIR) && str[0] == '.')
	{
		cout << "\x1b[94m\x1b[100m" << name << "\x1b[0m"; //Print blue, gray, and off
	}

	else if (x.st_mode &  S_IFDIR) 
	{
		cout << "\x1b[94m" << name << "\x1b[0m"; //Print blue
		//cout << '/';
	}

	else if ((x.st_mode & S_IXUSR) && str[0] == '.')
	{
		cout << "\x1b[92m\x1b[100m" << name << "\x1b[0m"; //Print green, gray, and off
	}

	else if (x.st_mode & S_IXUSR)
	{
		cout << "\x1b[92m" << name << "\x1b[0m"; 
	//	cout << '*';
	}

	else 
	{
		cout << name;
	}

	if (x.st_mode & S_IFDIR)
	{
		cout << '/';
	}

	else if (x.st_mode & S_IXUSR)
	{
		cout << '*';
	}
	cout << "  " << endl;
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
		//cout << "Total: " << (block/2) << endl;
	}

	if (flag%3 == 0)
	{
		cout << "Total: " << (block/2) << endl;
	}

	vector <char*> cstrings;

	while ((currentdirectory = readdir(dirStream)))
	{
		cstrings.push_back(currentdirectory->d_name);//Copy contents of file into cstrings 
	}
	sort(cstrings.begin(), cstrings.end()); //Sort the cstrings vector to be in order

	for (unsigned i = 0; i < cstrings.size(); i++)
	{
		if (errno!= 0)
		{
			perror("readdir"); //Error if not equal to 0 with errno
		}

		char path[1000];
		strcpy(path, directoryname);
		strcat(path, "/");
		strcat(path, cstrings.at(i));
		//directoryname/cstrings.at(i)
		//src/ls.cpp as an example
		struct stat s;

		if (stat(path, &s) == -1)
		{
			perror("stat"); //If stat returns -1 then it's an error
		}

		if (flag == 1)
		{
			if (cstrings.at(i)[0] != '.') //Case for .'s
			{
				if((s.st_mode & S_IFDIR) && cstrings.at(i)[0] == '.')
				//Checks S_IFDIR permission first
				{
					cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
					//Outputs blue, gray, << , off
				}
				else if(s.st_mode & S_IFDIR)
				{
					cout << "\x1b[94m" << cstrings.at(i) << "\x1b[0m"; //directories in blue
					cout << '/';
				}


				else if (s.st_mode & S_IXUSR && cstrings.at(i)[0] == '.') 
				{
					cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";//green << << off
				}

				else if (s.st_mode & S_IXUSR)
				{
					cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m"; //executables
					cout << '*';
				}

				else 
				{
					cout << cstrings.at(i);
				}

				cout << "  ";
			}
		}

		else if (flag%2 == 0 && flag%3 == 0 && flag%5 == 0) //-a -l -R  cases
		{
			printl(s, path, cstrings.at(i));
			if ((s.st_mode & S_IFDIR) && cstrings.at(i)[0] != '.')
			{
				recursioncall(path, flag);
				cout << endl << endl;
			}
		}

		else if (flag%2 == 0 && flag%3 == 0 && flag%5 != 0) //-a -l
		{
			printl(s, path, cstrings.at(i));
		}

		else if (flag%2 == 0 && flag%3 != 0 && flag%5 == 0) //-a, -R cases
		{
			if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0] == '.')
			{
				cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
			}
			
			else if (s.st_mode & S_IFDIR)
			{
				cout << "\x1b[94m" << cstrings.at(i) << "\x1b[0m";
				cout << '/';
			}

			else if ((s.st_mode & S_IXUSR)&& cstrings.at(i)[0] == '.')
			{
				cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
			}

			else if (s.st_mode & S_IXUSR)
			{
				cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
				cout << '*';
			}

			else 
			{
				cout << cstrings.at(i);
			}
			cout << "  ";
		}

		else if (flag%2 != 0 && flag%3 == 0 && flag%5 == 0) //-l -R cases
		{
			if (cstrings.at(i)[0] != '.')
			{
				printl(s, path, cstrings.at(i));
			}
			if ((s.st_mode & S_IFDIR)&& cstrings.at(i)[0] != '.')
			{
				cout << endl << endl;
				recursioncall(path, flag);
			}
		}

		else if (flag%2 == 0 && flag%3 != 0 && flag%5 != 0) //-a case
		{
			if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0] == '.')
			{
				cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
			}
			
			else if (s.st_mode & S_IFDIR)
			{
				cout << "\x1b[94m" << cstrings.at(i) << "\x1b[0m";
				cout << '/';
			}

			else if ((s.st_mode & S_IXUSR)&& cstrings.at(i)[0] == '.')
			{
				cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
			}

			else if (s.st_mode & S_IXUSR)
			{
				cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
				cout << '*';
			}

			else 
			{
				cout << cstrings.at(i);
			}
			cout << "  ";
		}

		else if (flag%2 != 0 && flag%3 == 0 && flag%5 != 0) //-l case
		{
			if (cstrings.at(i)[0] != '.')
			{
				printl(s, path, cstrings.at(i));
			}
		}

		else if (flag%2 != 0 && flag%3 != 0 && flag%5 == 0) //-R case
		{
			if(cstrings.at(i)[0] != '.')
			{
				if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0] == '.')
				{
					cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
				}
			
				else if (s.st_mode & S_IFDIR)
				{
					cout << "\x1b[94m" << cstrings.at(i) << "\x1b[0m";
					cout << '/';
				}

				else if ((s.st_mode & S_IXUSR)&& cstrings.at(i)[0] == '.')
				{
					cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
				}	

				else if (s.st_mode & S_IXUSR)
				{
					cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
					cout << '*';
				}

				else 
				{
					cout << cstrings.at(i);
				}
				cout << "  ";
					
			}
		}
		if (cstrings.at(i)[0] != '.' && (s.st_mode & S_IFDIR))
		{
			cout << endl << endl;
			recursioncall(path, flag);
		}
	}
}


int main(int argc, char** argv)
{
	//	cout << "\x1b[94m\x1b[100m";
	vector <string> arguments;
	
	arguments.push_back(".");

	bool files = false;

	unsigned int flag = 1;

	//Commented out for the sake of compiling 
	
	for (int i = 1; i < argc; i++)
	{
		//Add cases of possibilities with -l, -a, -R.
		//All possible cases will be multiplied into flag
		//a, l, r = 2, 3, 5 respectively
		//Went through every possible combination of the 3
		if (!strcmp(argv[i] , "-a"))
		{
			flag *= 2;
		}

		else if (!strcmp(argv[i] , "-l"))
		{
			flag *= 3;
		}

		else if (!strcmp(argv[i], "-R"))
		{
			flag *= 5;
		}

		else if (!strcmp(argv[i], "-al"))
		{
			flag *= 6;
		}

		else if (!strcmp(argv[i], "-la"))
		{
			flag *= 6;
		}

		else if (!strcmp(argv[i], "-aR"))
		{
			flag *= 10;
		}

		else if (!strcmp(argv[i], "-Ra"))
		{
			flag *= 10;
		}

		else if (!strcmp(argv[i], "-lR"))
		{
			flag *= 15;
		}

		else if (!strcmp(argv[i], "-Rl"))
		{
			flag *= 15;
		}

		else if (!strcmp(argv[i], "-alR"))
		{
			flag *= 30;
		}

			else if (!strcmp(argv[i], "-aRl"))
			{
				flag *= 30;
			}

			else if (!strcmp(argv[i], "-Ral"))
			{
				flag *= 30;
			}
			
			else if (!strcmp(argv[i], "-laR"))
			{
				flag *= 30;
			}
			
			else if (!strcmp(argv[i], "-Rla"))
			{
				flag *= 30;
			}
			
			else if (!strcmp(argv[i], "-lRa"))			
			{
				flag *= 30;
			}

			//else if (!strcmp(argv[i], "
		else 
		{
			if (files == false)
			{
				files = true;
				string temp = argv[i]; //temp to store commands
				arguments.at(0) = temp;
			}

			else 
			{
				arguments.push_back(argv[i]);
			}
		}

	}

	if(files == false)
	{
		char temp[] = ".";
		recursioncall(temp, flag); //if implemented properly, rshell should be run here
	}

	else 
	{
		//similar to recursioncall function
		for (unsigned i = 0; i < arguments.size(); i++)
		{
			DIR *dirStream;
			dirent* currentdirectory;

			struct stat s;

			char placeholder[2000];
			char path[1000];

		//	bool file;
		//	bool test;

			if (!(dirStream = opendir(".")))
			{
				perror("opendir");
				exit(1);
			}

			while ((currentdirectory = readdir(dirStream)))
			{
				if (errno != 0)
				{
					perror("readdir");
					exit(1);
				}

				strcpy(placeholder, arguments.at(i).c_str());
				strcpy(path, "./"); 
				strcat(path, currentdirectory->d_name);

				if (stat(path, &s) == -1)	
				{
					perror("stat");
					exit(1);
				}

				if (!strcmp(placeholder, currentdirectory->d_name));
				{
		//			test = true;
					break;
				}

				if (closedir(dirStream) == -1)
				{
					perror("closedir");
					exit(1);
				}

		//		if (test && file)
		//		{
					recursioncall(placeholder, flag);
		//		}
			}
		}

	}
	
	return 0;
}







