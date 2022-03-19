/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <stdbool.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 


void client_business (int sockfd) {
    char recv_buf[MAXDATASIZE];
    char send_buf[13] = "msg snum =  \0";

    // send
    for (int counter = 5; counter > 0; -- counter) {
        send_buf[11] = (char) (counter + '0');

        if (send(sockfd, send_buf, 13, 0) == -1)
            perror("send");
        printf("server: sent %s\n", send_buf);
    }

    // recv
    for (int counter = 10; counter > 0; -- counter) {
        memset(recv_buf, 0, sizeof(recv_buf));
        int numbytes;
        if ((numbytes = recv(sockfd, recv_buf, MAXDATASIZE - 1, 0)) != -1) {
            recv_buf[numbytes] = '\0';
            for (int temp = 0; temp < numbytes; temp += 13) {
                printf("server: received (length = %d) '%s'\n", numbytes, recv_buf + temp);
            }
        }
    }
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[]) {
    setbuf(stdout,NULL);
	int sockfd;

	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure


	client_business(sockfd);


    close(sockfd);
    return 0;
}

