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

vector<string> parameters(bool& recur, size_t argc, char** params){
	vector<string> p;
	size_t i;
	size_t j;
	for(i = 1; i < argc; i++){
		if(params[i][0] == '-'){
			for(j = 1;params[i][j] != '\0'; j++){
				 if(params[i][j] == 'r') recur = true;
			}
		}
		else{
			p.push_back((string)params[i]);
		}
	}
	return p;
}

bool dot(const string param){
	if(param == "." || param == "..") return true;
	return false;
}

vector<string> files_inside(const string param){
	vector<string> my_files;
	DIR *my_directory = opendir(param.c_str());
	dirent *this_dir;
	if(my_directory != NULL)
	{
		while((this_dir = readdir(my_directory)))
		{
			if(this_dir == NULL)
			{
				perror("read directory");
				exit(1);
			}
			string foo = this_dir -> d_name;
			if(dot(foo));
			else
				my_files.push_back(foo);
		}
	}
	else
	{
		perror("open directory");
	}
	if(closedir(my_directory) == -1)
	{
		perror("close directory");
		exit(1);
	}
	return my_files;
}

void remove(const string param, const bool recur){ 
	struct stat s,d;
	size_t u, k;
	vector<string> lol;
	vector<string> kek;
	if(stat(param.c_str(), &d) == -1){
		return;
	}
	if(S_ISDIR(d.st_mode))
		kek = files_inside(param);
	else{
		if(unlink(param.c_str()) == -1)
			perror("unlink");
	}
	for(k = 0; k < kek.size(); k++){
		string bar = param + '/' + kek.at(k) ;
		if(stat(bar.c_str(), &s) == -1){
			//perror("stat");
			return;
		}
		if(S_ISDIR(s.st_mode) && recur){
			if(dot(kek.at(k)));
			else lol.push_back(bar);
		}
		else{
			if(unlink(bar.c_str()) == -1)
				perror("unlink");
		}
		for(u = 0; u < lol.size(); u++){
			remove(lol.at(u).c_str(), recur);	
		}
		
	}
	if(rmdir(param.c_str()) == -1){
		perror("unlink dir");
	}
}

int main(int argc, char** argv){
	if(argc == 1){
		cerr << "no argument" << endl;
		exit(1);
	}
	bool recur = false;
	vector<string> param = parameters(recur, argc, argv);
	size_t i;
	for(i = 0; i < param.size(); i++){
		remove(param.at(i), recur);	
	}
}








