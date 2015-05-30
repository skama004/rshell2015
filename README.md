#rshell


###Introduction
rshell is my own personal bash Terminal that I created to be able to support commands trhough execvp. Rshell runs system calls using the fork(), execvp, and using the process id to run commands. rshell is also capable of supporting commands with connectors.


###Connectors
`||` - The or command will run the first command if it return true. If it returns false, then the second command will run. 

`&&` - The and connector will run both commands if both return true. If one returns false, then it will not run at all.

`;` - The semicolon will split up commands and if any command is true that specific comamnd will run .

`#` - Comments are mean that anything after the `#` will not be ran.


Returning true will return true. Returning false will return false. 

###Running rshell

 ```
 $ git clone  https://github.com/skama004/rshell.git
 $ cd rshell
 $ git checkout hw0
 $ make
 $ bin/rshell
 ```
###Bugs/Limitations

1. When `ls` is passed through with whotes as `"ls"` there is an error.
2. Running `false && true || ` with any command after the or connector will not return what is expected. 
3. Multiple connectors (ex. `&&&`) will not output properly
4. Semicolon and a command such as pwd ; && ls will output, but error message from segfault check will output.
5. Use of inline tabs `\t` will not work peroperly when passed with other commands.
6. `cd` does not work

#LS 

ls is a bash command that lists all folder and files in your directories. ls was implemented using different flags `-l` `-a` `-R`. 


###Flags

`-a` lists hidden files.

`-l` lists descriptions of each file or foler using 

1. Access rights
2. Number of links
3. Number of blocks
4. User name
5. Group name
6. Date last modified

`-R` recursively calls ls in all folders and lists every file.

###How to Run
 ```
 $ git clone  https://github.com/skama004/rshell.git
 $ cd rshell
 $ git checkout hw1
 $ make
 $ bin/ls
 ```
 
###Bugs/Limitations
1. The formatting for the -l flag is slightly off.
2. Outputs have too big spaces between them
3. consistently running bin/ls causes memory leaks
4. -R outputs everything from the deepest folder. Not every folder.


#I/O Redirection and Piping
Input and output redirection allows input and output beyond the use of stdin, stdout, stderr.

Piping is used to connect multiple data processes. 

###How to
I/O Redirection uses 3 symbols ('<', '>', ">>")
1. < is used for stdin. wc -l < main.cpp will count the amount of lines in main.cpp and output to stdout.  
2. > is used for stdout. ls > output1.txt will store the contents outputted by ls into output1.txt
3. >> is used similarly to > but instead of replacing all of the contents, it will append the contents.

Piping used only 1 symbol ('|')
1. | is generally used to chain commands together. history | tail -10 will list the last 10 commands executed

###How to Run
 ```
 $ git clone  https://github.com/skama004/rshell.git
 $ cd rshell
 $ git checkout hw2
 $ make
 $ bin/rshell
 ```

###Bugs/Limitations
I/O Redirection outputs an error everytimme used.
Piping does not work as of 5/17/15 

Will continue to work on it.

#Signals
As of May 29th, 2015, rshell is now capable of using the PATH variable to find commands, use of the "cd" (change directory) command, and using ^C to interrupt a process. 

###How to Run
 ```
 $ git clone  https://github.com/skama004/rshell.git
 $ cd rshell
 $ git checkout hw3
 $ make
 $ bin/rshell
 ```
###Known Bugs/Limitations
1. Cannot handle multiple connectors with cd.
 'echo test && cd' will not work

2. cd ~ will not work

3. cd can only handle two parameters. anything more will result in an error. 

4. Error message will also be displayed sometimes when running. cd will still work properly, but error messages come randomly. 
'do_ypcall: clnt_call: RPC: Unable to send; errno = Operation not permitted'

If any more bugs please report to skama004@ucr.edu. 
