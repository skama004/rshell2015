// CP.CPP

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

#include "Timer.h"
#include "cp.h"


int main(int argc, const char** argv) {
    
    if(argc != 3 && argc != 4) {
        std::cout << "Invalid number of arguments" << std::endl;
        exit(1);
    }

    const char* _src = argv[1];
    const char* _dst = argv[2];

    if(argc == 4) {
        if(strcmp(argv[3], "-all") == 0) {
            // run and time all methods



        } else {
            std::cout << "invalid argument: " << argv[3] << std::endl;
            exit(1); 
        }
    } else {
        // efficient shit
        rwcopy(_src, _dst); 
    }

    return 0;
}


int stlcopy(const char* src_path, const char* dst_path) {
    
    std::ifstream src_file(src_path);
    std::ofstream dst_file(dst_path);
    char c;
   
    while(src_file.get(c))
        dst_file.put(c);

    src_file.close();

    return 0;
}


int rwcopy(const char* src_path, const char* dst_path) {
    
    int src_fd, dst_fd;
    int src_flags = O_RDONLY;
    int dst_flags = O_WRONLY | O_CREAT;
    mode_t dst_mode = S_IRUSR | S_IWUSR;
    if( (src_fd = open(src_path, src_flags)) == -1) { perror("open src"); exit(1); }    
    if( (dst_fd = open(dst_path, dst_flags, dst_mode)) == -1) { 
        perror("open dst"); exit(1); 
    }
  
    char* c = NULL;
    int ret;
    while( (ret = read(src_fd, &c, 1)) != 0)
       if(write(dst_fd, &c, 1) == -1) { perror("write dest"); exit(1); } 

    if(ret == -1) { perror("read source"); exit(1); }
    
    if(close(src_fd) == -1) { perror("source close"); exit(1); }
    if(close(dst_fd) == -1) { perror("dest close"); exit(1); }

    return 0;
}


int bufcopy(const char* src_path, const char* dst_path) {

    int src_fd, dst_fd;
    int src_flags = O_RDONLY;
    int dst_flags = O_WRONLY | O_CREAT;
    mode_t dst_mode = S_IRUSR | S_IWUSR;
    if( (src_fd = open(src_path, src_flags)) == -1) { perror("open src"); exit(1); }    
    if( (dst_fd = open(dst_path, dst_flags, dst_mode)) == -1) { 
        perror("open dst"); exit(1); 
    }
    
    struct stat *stat_buf = NULL;
    if(stat(src_path, stat_buf) == -1) { perror("stat src"); exit(1); }

    off_t buf_size = stat_buf->st_size;
  
    char* buf = NULL;
    if(read(src_fd, &buf, buf_size) == -1) { perror("read src"); exit(1); } 
    if(write(dst_fd, &buf, buf_size) == -1) { perror("write dest"); exit(1); } 

    if(close(src_fd) == -1) { perror("source close"); exit(1); }
    if(close(dst_fd) == -1) { perror("dest close"); exit(1); }

    return 0;
}
