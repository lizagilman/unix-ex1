
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT "2003"



struct Request{
    int request;
    off_t  file_size;
    size_t size_of_file_name;
};
typedef struct Request Request;




