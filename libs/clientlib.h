
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
#include <stdlib.h>


void request_ls();
void upload_file_to_server();
void download_file_from_server();
int make_connection();
char* get_filename_frome_user(char* command);
void *get_in_addr(struct sockaddr *sa); //Beej

void request_ls(){
    int sockfd = make_connection();
    Request request = {"ls"};
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


char* get_filename_frome_user(char* command){
    printf("Please enter a filename to %s", command);
    char filename[50];
    scanf("%s", filename);
    return filename;
}


void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


