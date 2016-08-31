#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


 char* get_file_names(int* size_ptr)
 {
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    char* error = "error";


    curr_dir = getenv("PWD");
    if(NULL == curr_dir)
    {
        printf("\n ERROR : Could not get the working directory\n");
        return "error";
    }

    dp = opendir((const char*)curr_dir);
    if(NULL == dp)
    {
        printf("\n ERROR : Could not open the working directory\n");
        return "error";
    }

    while(NULL != (dptr = readdir(dp))){
        count++;
    }
    int buf_size = 256*count;

    size_ptr =  &buf_size;
    printf("size ptr %d\n count*256 %d\nsize ptr %d", *size_ptr, buf_size, size_ptr);
    char *file_names;
    file_names = malloc((sizeof(char) * (buf_size))+1);
    int i=0;
    dp = opendir((const char*)curr_dir);
    if(NULL == dp)
    {
        printf("\n ERROR : Could not open the working directory\n");
        return "error";
    }
    while(NULL != (dptr = readdir(dp)))
    {
    strcat(file_names, dptr->d_name);
    strcat(file_names, " ");
    }
    printf("size of filenamse in f %d\n", strlen(file_names));
    return file_names;
 }


 int main(){
    char* file_names2;
    int* size;
    //int num=5;
    //size=&num;


   file_names2 = get_file_names(*size);
     printf("%d\n%d\n%d", &size,*size,size);
     printf("size of filenamse %d", strlen(file_names2));
      printf("size of filenamse %d", sizeof(file_names2));
//printf("%d", sizeof(file_names));
    //printf(file_names);

    return 0;
 }








