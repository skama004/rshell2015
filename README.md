###rshell


##Introduction
rshell is my own personal bash Terminal that I created to be able to support commands trhough execvp. Rshell runs system calls using the fork(), execvp, and using the process id to run commands. rshell is also capable of supporting commands with connectors.


##Connectors
`||` - The or command will run the first command if it return true. If it returns false, then the second command will run. 

`&&` - The and connector will run both commands if both return true. If one returns false, then it will not run at all.

`;` - The semicolon will split up commands and if any command is true that specific comamnd will run .

`#` - Comments are mean that anything after the `#` will not be ran.


Returning true will return true. Returning false will return false. 

##Running rshell

 ```
 $ git clone  https://github.com/yourusername/rshell.git
 $ cd rshell
 $ git checkout hw0
 $ make
 $ bin/rshell
 ```
##Bugs/Limitations

1. When `ls` is passed through with whotes as `"ls"` there is an error.
2. Running `false && true || ` with any command after the or connector will not return what is expected. 
3. Multiple connectors (ex. `&&&`) will not output properly
4. Semicolon and a command such as pwd ; && ls will output, but error message from segfault check will output.
5. Use of inline tabs `\t` will not work peroperly when passed with other commands.
6. `cd` does not work
3. 
