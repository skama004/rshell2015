CC = g++
CFLAGS = -g -Wall -Werror -ansi -pedantic

all: rshell ls


rshell:
	@mkdir -p bin
	$(CC) $(CFLAGS) src/main.cpp -o bin/rshell

ls:
	@mkdir -p bin
	$(CC) $(CFLAGS) src/ls.cpp -o bin/rshell
