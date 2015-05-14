#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

//char semicols = " ; ";
//char ands = " && ";
//char ors = " || ";
//char spaces = " ";

void executecmd(char **argv, int &status)
{
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
	 }
				
	else if (pid == -1)//If fork returns an error
	{
		perror("Error w/ fork");
		exit(1);
	}


	else if (pid > 0)//If Parent id is still running
	{
		if (wait(&status) == -1)
		{
			perror("Error w/ wait");
			exit(1);
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

void initializetok(char *charinput, char** argv)//Use this to initialize first token
{
	char *store = strtok(charinput, ""); //Tokenizes the input and stores it into store
	int i = 0;
	while (store != NULL) //Keeps going until the stored word is null
	{
		argv[i] = store; //Passes the stored word in argument array

		store = strtok(NULL, ""); //Tokenizes null word again and stores yet again
		i++;
	}

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
	
	if (input.find(";") < input.size())
	{
		semicol = true; //If semicolon is found and is less than the size, then bool value will be set to true to determine it is there
	}

	if (input.find("&&") < input.size())
	{
		andd = true;//Same with and operator
	}
		
	if (input.find("||") < input.size())
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
									cerr << "Error!!"; //Outputs an error to determine if segault or not. Outputs an error, but still runs program. *Update in README*
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

   if (getlogin() != NULL && gethostname(host, 300) == 0)
   {
      for (int i = 0; i < 50; i++)
      {
         if (host[i] == '.')
         {
            host[i] = '\0';
         }
         prompt = getlogin();
         prompt = prompt+"@"+host+ " $ "; //Should print skama004@hammer $
      }
   }

   else
   {
      prompt = "$ "; //If login and hostname fail pastes just $
	}

	string userinp;

	while (true)
	{
		cout << prompt;

		getline(cin, userinp);

		if (userinp.find("#") != string::npos) //Basically if anything is in it
		{
			userinp.resize(userinp.find("#"));//Resizes if find comment returns -1	
		}
		stringtoken(userinp);

	}
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
				perror("open"); //error checking for open
			}
			if ((dup2(fd, 0)) == -1) //dup2 makes 0 the copy of fd
			{
				perror("dup2"); //error checking for dup2
			}
		}

		else if (!strcmp(args[i], ">"))
		{
			if(!strcmp(args[i+1], ">")) //If >> 
			{
				args[i] = 0;
				args[i+1] = 0;

				if ((open(args[i+2], O_CREAT|O_WRONLY|O_APPEND, 0666)) == -1)
				{
					perror("Second open");
				}

				if((dup2(fd, 1)) == -1)
				{
					peror("Second dup2");	
				}
				i++;
			}

			else
			{
				args[i] = 0;

				if ((open(args[i+1], O_CREAT|O_WRONLY|O_TRUNC, 0666)) == -1)
				{
					perror("Third open");
				}

				if ((dup2(fd,1)) == -1)
				{
					perror("Third dup2");
				}
			}
		}
	}
}


int main()
{
	display();//Calls display function

	return 0;
		
}
	
