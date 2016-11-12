#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define SERVPORTNO "9999"

using namespace std;



int main (void) {
	int status;
	struct addrinfo hints;
	struct addrinfo *servaddrinfo; // will point to the results
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	// hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	if ((status = getaddrinfo("localhost", SERVPORTNO, &hints, &servaddrinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	 	exit(1);
	}

	char ip4[INET_ADDRSTRLEN];
	struct sockaddr_in servaddr = *((struct sockaddr_in*)(servaddrinfo->ai_addr));
	inet_ntop(AF_INET, &(servaddr.sin_addr), ip4, INET_ADDRSTRLEN);
	cout << "Server ip is: " << ip4 << endl;

	int servfd = socket(PF_INET, SOCK_STREAM, 0);
	if(servfd == -1) {
		perror("Socket");
		exit(1);
	}

    freeaddrinfo(servaddrinfo);

    if( connect(servfd, (struct sockaddr*)&servaddr, sizeof servaddr) == -1) {
        perror("connect");
        exit(1);
    }

    cout << "Connected with server at " << ip4 << " at portno " << SERVPORTNO << " successfully!" << endl;

	close(servfd);
	return 0;
}
