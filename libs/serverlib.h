
#include <dirent.h>

/*
void* get_in_addr(struct sockaddr *sa);
void handle_ls_request(int connection);
void handle_downlaod_request();
void handle_upload_request();
*/

char* get_list_of_files();
void send_data_to_client(char* buffer, int connection);
void handle_request(int connection);

void handle_ls_request(int connection){
    char* list_of_files;
    list_of_files = get_list_of_files();
    if(list_of_files != "error"){
        send_data_to_client(list_of_files, connection);
    }
 }


char* get_list_of_files() // based on example from 'http://www.go4expert.com/articles/developing-linux-utility-ls-c-t27426/'
 {
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    char* error = "error"; //error flag

    dp = opendir("./uploads");
    if(NULL == dp)
    {
        printf("\n ERROR : Could not open the working directory\n");
        return error;
    }

    while(NULL != (dptr = readdir(dp))){
        count++;
    }
    char *file_names;
    file_names = malloc((sizeof(char) * (256*count))+1);
    dp = opendir("./uploads");
    if(NULL == dp){
        printf("\n ERROR : Could not open the working directory\n");
        return error;
    }
    while(NULL != (dptr = readdir(dp))){
        strcat(file_names, dptr->d_name);
        strcat(file_names, " ");
    }
    return file_names;
 }

void send_data_to_client(char* buffer, int connection){
    int numbytes= 0;
    if((numbytes=send(connection,buffer, strlen(buffer) ,0))==-1){
        printf("Couldn not send data\n");
        return;
    }
    return;
}


void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

