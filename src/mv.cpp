#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>
using namespace std;

int main(int argc, char** argv){
	if(argc > 4){
		cerr << "more than 2 arguments" << endl;
		exit(1);
	}
	if(argc == 1){
		cerr << "no arguments" << endl;
		exit(1);
	}
	if(argc == 2){
		cerr << "missing destination" << endl;
		exit(1);
	}
	struct stat s, d;
	bool destination = false;
	if(stat(argv[1], &s) == -1){
		perror("stat");
		exit(1);
	}
	if(stat(argv[2], &d) == -1){
		destination = true;
	}
	string source = argv[1];
	string dest = argv[2];
	if(S_ISDIR(d.st_mode)){
		dest = dest + '/' + source;
		if(link(source.c_str(), dest.c_str()) == -1)
			perror("link");
		if(unlink(source.c_str()) == -1)
			perror("unlink");
	}
	else if(destination){
		if(link(source.c_str(), dest.c_str()) == -1){
			perror("link");
			exit(1);
		}
		if(unlink(source.c_str()) == -1){
			perror("unlink");
			exit(1);
		}
	}
}
