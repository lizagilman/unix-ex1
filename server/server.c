#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>

#include "../libs/shared_utils.h"


void* get_in_addr(struct sockaddr *sa);
void handle_request(int connection);
void handle_ls_request(int connection);
void handle_downlaod_request();
//void handle_upload_request();
void handle_upload_request(int connection){
char* get_list_of_files();
void send_data_to_client(char* buffer, int connection);



int main(void)
{
    //Beej's code
    printf("change\nServer running\n");
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

	struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p = ai; p != NULL; p = p->ai_next) {
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) {
			continue;
		}

		// lose the pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}

		break;
	}

	// if we got here, it means we didn't get bound
	if (p == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    printf("Waiting for conections...\n");
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
					newfd = accept(listener,
						(struct sockaddr *)&remoteaddr,
						&addrlen);

					if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
                    }
                } else {
                    handle_request(i); // main server function
                    close(i);
                    printf("Ruquest handled.\n");
                    FD_CLR(i, &master);
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}

void handle_request(int connection){
    int numbytes;
    Request request;
    Request* request_ptr = &request;
    	if ((numbytes=recv(connection, request_ptr, sizeof(Request), 0)) == -1){
            printf("failed to receive request\n");
			perror("failed to recv");
            //denial_client(client);
			return;
        }

        printf("got %d\n", request.request);


        switch (request.request){

            case 1:
                 handle_ls_request(connection);
                 break;


            case 2:
                handle_upload_request(connection);
                break;

            case 3:
                printf("now do downlaid\n");
                break;
        }

        return;
        /*

        if(strncmp(request.request, "ls", 2)==0){
            handle_ls_request(connection);
        }


        else if(strncmp(request.request, "upload", 6)==0){

            printf("in if upload\n");
            //handle_upload_request(connection);
        }

        else if(strncmp(request.request, "download", 8)==0){
        // handle download request
*/

}


void handle_upload_request(int connection){
  printf("now do upload\n");

    return;
 }

void handle_ls_request(int connection){
    char* list_of_files;
    list_of_files = get_list_of_files();
    if(list_of_files != "error"){
        send_data_to_client(list_of_files, connection);
    }
    return;
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



