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

/*
    curr_dir = getenv("PWD");
    if(NULL == curr_dir)
    {
        printf("\n ERROR : Could not get the working directory\n");
        return "error";
    }
*/
    //dp = opendir((const char*)curr_dir);
    dp=opendir("./uploads");
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
    //printf("size ptr %d\n count*256 %d\nsize ptr %d", *size_ptr, buf_size, size_ptr);
    char *file_names;
    file_names = malloc((sizeof(char) * (buf_size))+1);
    int i=0;
    //dp = opendir((const char*)curr_dir);
    dp=opendir("./uploads");
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

char* get_command_and_filename_str(int flag){
    printf("Please enter a filename: ");
    char* filename=malloc(32*sizeof(char));
    scanf("%s", filename);
    char upload_and_filename_str[40]="upload ";
    char download_and_filename_str[43]="download ";
    if(flag){
        strcat(upload_and_filename_str, filename);
        printf("%s\n", upload_and_filename_str);
        return upload_and_filename_str;
    }
    else {
        strcat(download_and_filename_str, filename);
        return download_and_filename_str;
    }
}

#include "../../libs/shared_utils.h"


int main(){
//    printf("Please enter a filename to %s: ", command);
//char command[6]="upload";
char* test;

gets(test);
 Request request;
 strcpy(request.request, test);
printf("%s", request.request);
    return 0;
}








