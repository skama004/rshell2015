// CP.CPP

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <iostream>
#include <fstream>

#include "cp.h"

int main(int argc, const char** argv) {
    
    if(argc != 3 && argc != 4) {
        std::cout << "Invalid number of arguments" << std::endl;
        return 1;
    }

    const char* _src = argv[1];
    const char* _dst = argv[2];
    
    stdcopy(_src, _dst); 

    return 0;
}


int stdcopy(const char* srcpath, const char* dstpath) {
    
    std::ifstream srcfile(srcpath);
    std::ofstream dstfile(dstpath);
    char c;
   
    while(srcfile.get(c))
        dstfile.put(c);

    srcfile.close();

    return 0;
}



