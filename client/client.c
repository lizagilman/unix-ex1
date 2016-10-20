#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "../libs/shared_utils.h"


void request_ls();
void upload_file_to_server();
void download_file_from_server();
int make_connection();
void download_file_from_server();
//char* get_command_and_filename_str(int flag);
void *get_in_addr(struct sockaddr *sa); //Beej


int main(){
    char request[64];
    //char* filename = malloc(sizeof(char));
    for(;;){
        printf("Enter a commad: upload <filename> / download <filename> / ls\n For example upload test.c / ls \n   ");
        gets(request);
        if(strcmp(request,"ls") == 0){
            request_ls();
            continue;
        }
        if (strncmp(request,"download", 8) == 0 ){
           //filename=get_command_and_filename_str(0);
           //handle-download_request();
           download_file_from_server();
           continue;
        }

        if (strncmp(request,"upload", 6) == 0 ){
        printf("in cmp up you entered %s\n", request);
           //filename=get_filename_str(1);
            upload_file_to_server();
            continue;
           //handle-download_request();
        } else {
            printf("illegal command, please try again\n");
        }
    }
}


/*
char* get_command_and_filename_str(int flag){
    printf("Please enter a filename: ");
    char* filename=malloc(sizeof(char));
    scanf("%s", filename);
    char upload_and_filename_str[32]="upload ";
    char download_and_filename_str[32]="download ";
    if(flag){
        strcat(upload_and_filename_str, filename);
        return upload_and_filename_str;
    }
    else {
        strcat(download_and_filename_str, filename);
        return download_and_filename_str;
    }
}
*/

void upload_file_to_server(){
    int sockfd = make_connection();
    size_t numbytes;
    struct stat *file_status = malloc(sizeof(struct stat));
    //char* filename;
    //filename = "tirgul3.c";
    char filename[] = "tirgul3.c";
    int fd = open(filename,O_RDONLY);
    if(fd==-1){
    printf("Error.No such file.\n");
    return;
    }
    size_t size_of_file_name = strlen(filename);
    int status = stat(filename,file_status);
        if(status==-1){
            perror("error:");
            return;
    }
    off_t  file_size =  file_status->st_size;
    Request request;
    request.request = 2;
    request.file_size = file_size;
    request.size_of_file_name = size_of_file_name;
    Request* request_ptr = &request;
    if ((numbytes = send(sockfd, request_ptr, sizeof(Request), 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

    Response *response;
    if ((numbytes = recv(sockfd, response,sizeof(Response), 0)) == -1) {
                perror("recv");
                exit(1);
    }
    if(*response==OK){
        size_t send_size = 0;
        if ((numbytes = send(sockfd, filename,size_of_file_name, 0)) == -1) {    // sendin file name
	    perror("recv");
	    exit(1);
        }



        char buffer[REQUEST_SIZE];
        int byte_send = 0;
        int size_read = 0;
        int error_count = 0;
        while(byte_send <= request.file_size){  // read and sending file
            size_read = read(fd,buffer,REQUEST_SIZE);
                  numbytes = 0;
                       if ((numbytes = send(sockfd,buffer+numbytes,REQUEST_SIZE, 0)) == -1) {    // sendin file content
                          perror("recv");
                           exit(1);
                        }

                  byte_send+=  numbytes;
                  error_count = 0;

        }
        if ((numbytes = recv(sockfd, response,sizeof(Response), 0)) == -1) {
                perror("recv");
                exit(1);
       }
     if(*response==OK){
    printf("Stored file in server done\n");
    }

        else{
                printf("Failed to story fail. Try again\n");
            }

     }
      else{
      printf("Failed to story fail. Try again\n");
        }

  }






/* old
void upload_file_to_server(){
    int sockfd = make_connection();

    Request request = {2}; // flag of upload request
    Request* request_ptr = &request;
    int numbytes;
    printf("gonna send #1\n");
    if ((numbytes = send(sockfd, request_ptr, sizeof(request_ptr), 0)) == -1){
        printf("Failed to send ls request to the server.\n");
	    perror("ERROR writing to socket");
	    exit(1);
    }


    //int sockfd = connect_to_server(IP);
    char* filename;
    filename = "tirgul3.c";
    size_t numbytes_filename;
    struct stat *file_status = malloc(sizeof(struct stat));
    int fd = open(filename,O_RDONLY);
    if(fd==-1){
        printf("Can not open file, try again.\n");
        return;
    }
    size_t filename_size = strlen(filename);
    int status = stat(filename,file_status);
        if(status==-1){
            perror("status error:");
            return;
    }
    off_t file_size =  file_status->st_size;
    size_t send_size = 0;
    if ((numbytes = send(sockfd, filename,filename_size, 0)) == -1) {    // sendin file name
	    perror("recv 139");
	    exit(1);
    }
    //Request request;
    //request.request = {2};

    // send file size and size of filename

    //request.file_size = file_size;
   // request.size_of_file_name = filename_size;
   // Request* request_ptr2 = &request;




    if(*answer==ACCEPT){

    }


   Request request2;

     request2.file_size = file_size;
   request2.size_of_file_name = filename_size;

    Request* request_ptr2 = &request2;
printf("gonna send #2\n");
    // sned request with file size filename size
    if ((numbytes_filename = send(sockfd, request_ptr2, sizeof(Request), 0)) == -1) {
	    perror("send error");
	    exit(1);
	}



	// send the filename
    if ((numbytes = send(sockfd, filename, size_of_file_name, 0)) == -1) {    // sendin file name
	    perror("recv");
	    exit(1);
	}



}
*/


void download_file_from_server(){
    int sockfd = make_connection();
    Request request = {3}; // flag of download request
    Request* request_ptr = &request;
    int numbytes;
    if ((numbytes = send(sockfd, request_ptr, sizeof(request_ptr), 0)) == -1){
        printf("Failed to send ls request to the server.\n");
	    perror("ERROR writing to socket");
	    exit(1);
    }
}


void request_ls(){
    int sockfd = make_connection();
    Request request = {1}; // flag of ls request
    Request* request_ptr = &request;
    int numbytes;
    if ((numbytes = send(sockfd, request_ptr, sizeof(request_ptr), 0)) == -1){
        printf("Failed to send ls request to the server.\n");
	    perror("ERROR writing to socket");
	    exit(1);
    }

	char buffer[2048];
	    if ((numbytes = recv(sockfd, buffer, 2048, 0)) == -1) {
	    printf("Error in server\n");
	    perror("recv");
	    exit(1);
	}
	printf("\n >> %s\n\n",buffer);
    return;
}


int make_connection(){ //Beej
	int sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	char server[] = "localhost";

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(server, PORT, &hints, &servinfo)) != 0)

	 {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}


	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("socket() failed");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("connect() failed");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Connection failed\n\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	//printf("connecting to %s\n", s);
	freeaddrinfo(servinfo); // all done with this structure
    return sockfd;
}




void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




