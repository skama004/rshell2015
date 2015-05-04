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
 
##Bugs/Limitations
1. The formatting for the -l flag is slightly off.
2. Outputs have too big spaces between them
3. consistently running bin/ls causes memory leaks
4. -R outputs everything from the deepest folder. Not every folder.
