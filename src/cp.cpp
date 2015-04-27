// CP.CPP

#include <cstdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

#include "cp.h"

int main(int argc, const char** argv) {
    
    if(argc != 3 && argc != 4) {
        std::cout << "Invalid number of arguments" << std::endl;
        exit(1);
    }

    const char* _src = argv[1];
    const char* _dst = argv[2];
    
    stdcopy(_src, _dst); 

    return 0;
}


int stlcopy(const char* srcpath, const char* dstpath) {
    
    std::ifstream srcfile(srcpath);
    std::ofstream dstfile(dstpath);
    char c;
   
    while(srcfile.get(c))
        dstfile.put(c);

    srcfile.close();

    return 0;
}


int rwcopy(const char* srcpath, const char* dstpath) {

    

    return 0;
}
