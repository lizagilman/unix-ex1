
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define REQUEST_SIZE 4086
#define MAX_FILE_SIZE  4086
#define PORT "8567"

typedef enum {ERROR,OK} Response;

struct Request{
    int request;
    off_t  file_size;
    size_t size_of_file_name;
};
typedef struct Request Request;




