#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

void display();

void sighandler(int i)
{
	//signal(SIGINT, SIG_IGN);
	if (i == SIGINT)
	{
		cout << endl << flush;
	}

	else if (i == SIGTSTP)
	{
		cout << endl << flush;
	}
}

void executecmd(char **argv, int &status)
{
/*	string path = getenv("PATH");
		if (path.empty())
		{
			perror("getenv");
		}

		vector<string> dirPath;
		while(!(path.find(":") == string::npos))
		{
			dirPath.push_back(path.substr(0, path.find(":")));
			path = path.substr(path.find(":")+1);
		}
		
		int exec = 0;
		for (unsigned i = 0; i < dirPath.size(); i++)
		{
			string temp = argv[0];
			dirPath[i].append("/");
			dirPath[i].append(temp);
				if ((exec = (access(const_cast<char*> (dirPath[i].c_str()), X_OK))) == 0)
				{
					if (execv(const_cast<char*>(dirPath.at(i).c_str()), argv) == -1)
					{
						perror("execv");
						display();
						//exit(0);
					}
				}
		}

		if (exec == -1)
		{
			perror("access");
			exit(1);
		}*/

	 int pid = fork();
  
 	 if (pid == 0)
	 {
	 	 if (-1 == execvp(argv[0],argv))//Runs execvp to whatever arguments are passed through
		 {
			 status = -1;//Sets status to -1 to indicate 
	       perror("Error w/ execvp");
		 	// cout << "It works";
		 }
		 exit(1);//Return exit(1) if an error occured
		// pipes1(status);
	/*	if(strcmp(argv[0], "cd") == 0)
		{
			if(status == 1)
			{
				cout << "cd working" << endl;
				char *homedir = getenv("HOME");
				if(chdir(homedir) == -1)
				{
					perror("chdir");
				}
			}
			else
			{
				cout << "cd running " << endl;
				if(chdir(argv[1]) == -1)
				{
					perror("chdir2");
				}
			}
			//display();
			//exit(0);
		}*/
	 }
				
	else if (pid == -1)//If for returns an error
	{
		perror("Error w/ fork");
		exit(1);
	}


	else if (pid > 0)//If Parent id is still running
	{
		struct sigaction sig;

		sigignore(SIGINT);
		sigignore(SIGTSTP);
		if (wait(&status) == -1)
		{
			perror("Error w/ wait");
			exit(1);
		}

		memset(&sig, 0, sizeof(sig));

		sig.sa_handler = sighandler; 

		if (sigaction(SIGINT, &sig, NULL) == -1)
		{
			perror("sigaction");
		}

		if (sigaction(SIGTSTP, &sig, NULL) == -1)
		{
			perror("sigaction");
		}
	}
}



void tokenize(char **argvinput, char **argvoutput, char *operation, int& size)
{
	int i = 0;
	char *word = strtok(argvinput[0], operation); //Splits up the operation passed through

	while (word != NULL)
	{
		argvoutput[i] = word;

		word = strtok(NULL, operation);//tokenizes again and stores into word
		i++;
	}

	argvoutput[i] = 0;
	size = i; //Stores into size how many argvs were made

	delete[] word; //Deallocates memory 
		
}


void IOcheck(char** args) //Function to check for IO Redirection
{
	for (int i = 0; args[i] != '\0'; i++)
	{
		int fd = 0;
		if (!strcmp(args[i], "<"))
		{
			args[i] = 0;
			if ((fd = open(args[i+1], O_RDONLY)) == -1)
			{
				perror("open"); //Error checking for open
			}
			if ((dup2(fd, 0)) == -1)//Dup2 makes 0 the copy of fd
			{
				perror("dup2");//error checking
			}
		}

		else if (!strcmp(args[i], ">"))
		{
			if (!strcmp(args[i+1], ">")) //If >>
			{
				args[i] = 0;
				args[i+1] = 0;

				if ((fd = open(args[i+2], O_CREAT|O_WRONLY|O_APPEND, 0666)) == -1)
				{
					perror ("Second open");
				}

				if ((dup2(fd,1)) == -1)
				{
					perror ("Second dup2");
				}

				i++;
			}

			else // if just > 
			{
				args[i] = 0;

				if ((fd = open(args[i+1], O_CREAT|O_WRONLY|O_TRUNC, 0666)) == -1)
				{
					perror ("Third open");
				}

				if ((dup2(fd,1)) == -1)
				{
					perror ("Third dup2");
				}
			}
		}
	}
}

void replacestring(string &subject, const string& search, const string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search,pos)) != string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

void connectors(string &s) //Connector check
{
	replacestring(s, "<", " < ");
	replacestring(s, ">>", " >> ");
	replacestring(s, ">", " > ");
	replacestring(s, "|", " | ");
}

void pipes1 (vector <string> cmdlist)
{
	vector<string> pt1;
	vector<string> pt2;

	unsigned i;
	for (i = 0; i <cmdlist.size() && cmdlist.at(i) != "|"; i++) //checks | 
	{
		pt1.push_back(cmdlist.at(i));
	}

	for (i = i+1; i <cmdlist.size(); i++)
	{
		pt2.push_back(cmdlist.at(i));
	}

	if (pt2.size() == 0) //if no pipes
	{
		unsigned size = cmdlist.size();
		char** argument = new char* [size+1];
		argument[size] = '\0';

		for (unsigned i = 0; i < size; i++)
		{
			argument[i] = new char[cmdlist.at(i).size()];
			strcpy(argument[i], cmdlist.at(i).c_str());
		}
		IOcheck(argument);

		if (execvp(argument[0], argument) == -1)
		{
			delete[] argument;
			perror("execvp");
			exit(1);
		}
	}

	else
	{
		int fd[2];
		if (pipe(fd) == -1)
			perror("pipe");
			int forktest = fork();
			if (forktest == -1)
			{
				perror("fork");
			}

		else if (forktest == 0)
		{
			if (dup2(fd[1], 1) == -1)
			{
				perror("dup2");
			}

			if (-1 == close(fd[0]))
			{
				perror("close");	
			}

			unsigned size = pt1.size();

			char** argument = new char*[size + 1];
			argument[size] = '\0';

			for (unsigned i = 0; i < size; i++)
			{
				argument[i] = new char[pt1.at(i).size()];
				strcpy(argument[i], pt1.at(i).c_str());
			}

			IOcheck(argument);

			if (execvp(argument[0], argument) == -1)
			{
				delete [] argument;
				perror ("execvp");
				exit(1);
			}
		}

		else //All error checking
		{
			int status = 0;
			//int restore = 0;

			if (waitpid(-1, &status, 0) == -1)
			{
				perror("waitpid");
			}
			int restore = 0;
			if ((restore=dup(0)) == -1)
			{
				perror("dup");
			}

			if (dup2(fd[0], 0) == -1)
			{
				perror("dup2");
			}

			if (close(fd[1]) == -1)
			{
				perror("close");
			}
			
			pipes1(pt2);

			if (dup2(restore,0) == -1)
			{
				perror("dup2");
			}
		}
	}
}

void initializetok(char *charinput, char** argv)//Use this to initialize first token
{
	char* store = strtok(charinput, ""); //Tokenizes the input and stores it into store
	int i = 0;
	while (store != NULL) //Keeps going until the stored word is null
	{
		argv[i] = store; //Passes the stored word in argument array

		store = strtok(NULL, ""); //Tokenizes null word again and stores yet again
		i++;
	}
	/*
	int forktest = fork();
	if (forktest == -1)
	{
		perror("fork");
		exit(1);
	}

	else if (forktest == 0)
	{
		vector<string> v (&store, &store + 10);
		pipes1(&store);
	}
	*/


	argv[i] = 0;
	delete [] store; //Deallocates memory
}


void stringtoken(string input)
{
	char *inputchar = new char[input.length() + 1]; //Creates a char pointer from string;
	char *exitcmd = new char[5];
	bool semicol = false;
	bool andd = false;
	bool orr = false; 
	bool space = true;

	char *semicols = new char[3];
	char *ands = new char [3];
	char *ors = new char [3];
	char *spaces = new char[3];
	//newly allocate memory to keep track of all the operators 
	//also create bool values to determine tests whether they work or not
	//
	strcpy(inputchar, input.c_str());
	strcpy(semicols, ";");
	strcpy(ands,"&&");
	strcpy(ors, "||");
	strcpy(spaces, " \n");
	strcpy(exitcmd, "exit");
	//Strcpy will check if the operations passed through are in the string
	
	if (input.find(";") != string::npos)
	{
		semicol = true; //If semicolon is found and is less than the size, then bool value will be set to true to determine it is there
	}

	if (input.find("&&") != string::npos)
	{
		andd = true;//Same with and operator
	}
		
	if (input.find("||") != string::npos)
	{
		orr = true;//Same with or operator
	}
	//Sets the value to be true if operation is found in string
	

	char **argvsemicol = new char*[strlen(inputchar)];
	char **argvands = new char*[strlen(inputchar)];
	char **argvors = new char*[strlen(inputchar)];
	char **argvspaces = new char*[strlen(inputchar)];
	char **argvinp = new char*[strlen(inputchar)];
	//Newly allocates memory again for operators passed through arguments
	//strlen return the size of char array
	//so newly allocated memory 
	
	initializetok(inputchar, argvinp);//Call initalizetok function to pass in parameters and set it up for further breakdown

	int status = 0;//Acts as status of whether operators are found or not
	int opstate = 0; //Acts as a check for operators

	if (semicol || andd || orr || space) //Checks for all 4 operators
	{
		int size = 0;

		tokenize(argvinp, argvsemicol, semicols, size);//Tokenizes the arguments for semicolons

		for (int i = 0; i < size; i++)
		{
			if (andd || orr || space)
			{
				int size1 = 0; 
				
				tokenize(argvsemicol, argvands, ands, size1);//Tokenizes the arguments for ands
				
				for (int j = 0; j < size1; j++)
				{
					if (orr || space)
					{
						int size2 = 0;

						tokenize(argvands, argvors, ors, size2);//Tokenizes the arguments for ors

						for (int h = 0; h < size2; h++)
						{
							if (space)
							{
								int size3 = 0;

								tokenize(argvors, argvspaces, spaces, size3);//Tokenizes the arguments for spaces

								if (argvspaces[0] == NULL)
								{
									cerr << "error with execvp" << endl; //Outputs an error to determine if segault or not. Outputs an error, but still runs program. *Update in README*
								}
								else if (strcmp(argvspaces[0], exitcmd) == 0)
								{
									exit(0);//Exits without an error
								}
								else 
								{
									executecmd(argvspaces, status);//Calls commands for the arguments and runs execvp within that function
								}
							}
							if (status == 0) //Will return 0 if it failed
							{
								h = size2;
								opstate = -1;//Sets the operation status to -1 
							}
							else 
							{
								for (int k = 0; k < size2; k++)
								{
									argvors[k] = argvors[k+1]; //arguments of ors get copied over to 1+
								}
							}

						}
					}
					
					if (status == 0 || (ors&&opstate == -1))
					{
						for (int l = 0; l < size1; l++)
						{
							argvands[l] = argvands[l+1];
							status = 200;
						}
					}
					else
					{
						char *fals = new char[1];
						strcpy(fals, "");
						argvands[0] = fals;
				//		connectors(fals);
						delete [] fals;
					}
			}
			
		}
		for (int m = 0; m < size; m++)
		{
			argvsemicol[m] = argvsemicol[m+1];
		}

	}
	delete [] argvands;
	delete [] argvors;
	delete [] argvsemicol;
	delete [] argvspaces;
	delete [] argvinp;
	delete [] semicols;
	delete [] ands;
	delete [] ors;
	delete [] exitcmd;
	delete [] inputchar;
	delete [] spaces;
	//Deletes all allocated memory to ensure no memory leak. ****IF MEMORY LEAK OCCURS GO THROUGH CODE AGAIN****
	}
}

void display()
{
	//signal(SIGINT, sighandler);

	struct sigaction sig;
	memset(&sig, 0, sizeof(sig));

	sig.sa_handler = sighandler;
	
	if(sigaction(SIGINT, &sig, NULL) == -1)
	{
		perror("sigaction");
	}

	char* cwd; //current working directory

	char buffer[PATH_MAX + 1];

	cwd = getcwd(buffer, PATH_MAX + 1);

	if(cwd == NULL)
	{
		perror("getcwd");
	}

	if (-1 == setenv("OLDPWD", cwd, 1))
	{
		perror("setenv");
	}

  	while (true)
	{
		string prompt; //Created a string for the prompt ($ and hopefully login)
		char host[333];

   	if (getlogin() == NULL) //Gets login info
   	{
      	perror("Error w/ getlogin()"); //Error if null
   	}	

   	if (gethostname(host, 300) != 0) //Get host info and writes it into char array with size
   	{
      	perror("Error w/ gethostname()"); //Error if does not return 0
   	}
		//Moved to inside infinite loop

		char *path; 

		path = getenv ("HOME");
		
		if (path == NULL)
		{
			perror("getenv");
		}

		char* cwd; //current working directory

		char buffer[PATH_MAX + 1];

		cwd = getcwd(buffer, PATH_MAX + 1);

		if(cwd == NULL)
		{
			perror("getcwd");
		}	

		string path2 = path;
		string path3 = cwd;
		string prompt2;

		if (path3.find(path2) != string::npos)
		{
			prompt2 = path3.substr(path2.size(), path3.size());
			prompt2 = "~" + prompt2;
		}	

		else
		{
			prompt2 = path3;
		}

		if (getlogin() != NULL && gethostname(host, 300) == 0)
   	{
      	for (int i = 0; i < 50; i++)
      	{
        		if (host[i] == '.')
         	{
            	host[i] = '\0';
         	}
         	prompt = getlogin();
         	prompt = prompt+"@"+host+ prompt2 +  " $ "; //Should print skama004@hammer $
      	}
   	}

   	else
  	 	{
      	prompt = prompt2 + "$ "; //If login and hostname fail pastes just $
		}

		string userinp;


		cout << prompt;

		getline(cin, userinp);

		if (userinp.find("#") != string::npos) //Basically if anything is in it
		{
			userinp.resize(userinp.find("#"));//Resizes if find comment returns -1	
		}
		
		bool checkforcd = false;
		
		//connectors(userinp);
		
		if (userinp.find("cd") != string::npos)
		{
			char *space = new char[3]; //newly allocates memory to check for spaces
			strcpy(space, " ");
			char *inputc = new char [userinp.length() + 1]; //newly allocates memory to make a char* from string
			strcpy(inputc, userinp.c_str());

			char **argvinput = new char*[strlen(inputc)]; //similar to what was done in stringtoken
			char **argvspaces = new char*[strlen(inputc)];

			int size;

			initializetok(inputc, argvinput);
			tokenize(argvinput, argvspaces, space, size);

			string check = argvspaces[0];

			if (check == "cd")
			{
				checkforcd = true;
				
				char *cd = new char[3];
				char *spa = new char[3];

				strcpy(cd, "cd");
				strcpy(spa, " ");

				char *input = new char[userinp.length() + 1];

				strcpy(input, userinp.c_str());

				char **argvinput2 = new char *[strlen(input)];
				char **argvspaces2 = new char *[strlen(input)];

				int sz;

				initializetok(input, argvinput2);
				tokenize(argvinput2, argvspaces2, spa, sz);

				string check2 = argvspaces2[0];
				
				if (check2 != "cd" || sz > 2)
				{
					cerr << "Error: cd" << endl;

					delete [] cd;
					delete [] spa;
					delete [] input;
					delete [] argvinput2;
					delete [] argvspaces2;
					
					return;
				}

				char *cwd2;
				char *path2;

				char buff[PATH_MAX + 1];

				if (argvspaces2[1] == NULL) //If just cd by itself
				{
					cwd2 = getcwd(buff, PATH_MAX + 1);

					if (cwd2 == NULL)
					{
						perror("getcwd");
					}

					if (setenv("OLDPWD", cwd2, 1) == -1)
					{
						perror("setenv");	
					}

					path2 = getenv("HOME");

					if (path2 == NULL)
					{
						perror("getenv");
					}

					if (chdir(path2)== -1)
					{
						perror("chdir");
					}

					if (setenv("PWD", path2, 1) == -1)
					{
						perror("So much error checking.."); //Remove this before submitting
						path2 = getenv ("OLDPWD");
					}
					delete [] cd;
					delete [] spa;
					delete [] input;
					delete [] argvinput2;
					delete [] argvspaces2;
					
					return;
				}
				
				check2 = argvspaces2[1];
				
				if (check2 == "-") //cd - 
				{
					cwd2 = getcwd(buff, PATH_MAX + 1);

					if (cwd2 == NULL)
					{
						perror("getcwd");
					}
						
					path2 = getenv("OLDPWD");

					if (path2 == NULL)
					{
						perror("getenv");
					}

				//	if (setenv("OLDPWD", cwd2, 1) == -1)
				//	{
				//		perror("setenv");	
				//	}

					if (chdir(path2) == -1)
					{
						perror("chdir");
					}
					
					if (setenv("PWD", path2, 1) == -1)
					{
						perror("setenv(PWD)");
						//path2 = getenv ("OLDPWD");
					}

					if (setenv("OLDPWD", cwd2, 1) == -1)
					{
						perror("setenv(OLDPWD)"); 
						//path2 = getenv ("OLDPWD");
					}

				}

				else //cd with path name
				{
					cwd2 = getcwd(buff, PATH_MAX + 1);

					if (cwd2 == NULL)
					{
						perror("getcwd");
					}
					
					if (setenv("OLDPWD", cwd2, 1) == -1)
					{
						perror("setenv(OLDPWD)"); 
					}

					if (chdir(check2.c_str()) == -1)
					{
						perror("chdir");
					}
				
					cwd2 = getcwd(buff, PATH_MAX + 1);

					if (cwd2 == NULL)
					{
						perror("getcwd");
					}
					
					if (setenv("PWD", cwd2, 1) == -1)
					{
						perror("setenv(PWD)"); 
					}
					
				}
				
				delete [] cd;
				delete [] spa;
				delete [] input;
				delete [] argvinput2;	
				delete [] argvspaces2;
					
			}

			delete [] space;
			delete [] inputc;
			delete [] argvinput;
			delete [] argvspaces;
		}
		
		if (!checkforcd)
		{
			stringtoken(userinp);
		}
	}
}

int main()
{
	display();//Calls display function

	return 0;
		
}
	
