#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORTNO "9999"

using namespace std;



int main (void) {
	int status;
	struct addrinfo hints;
	struct addrinfo *myaddrinfo; // will point to the results
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	if ((status = getaddrinfo(NULL, MYPORTNO, &hints, &myaddrinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	 	exit(1);
	}
	// //TRAVERSE THE LINKED LIST
	// struct addrinfo *res = myaddrinfo;
	// char ipstr[INET6_ADDRSTRLEN];
	//
	// for(struct addrinfo *p = res;p != NULL; p = p->ai_next) {
	// 	void *addr;
	// 	char *ipver;
	// 	// get the pointer to the address itself,
	// 	// different fields in IPv4 and IPv6:
	// 	if (p->ai_family == AF_INET) { // IPv4
	// 	struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
	// 	addr = &(ipv4->sin_addr);
	// 	ipver = "IPv4";
	// 	} else { // IPv6
	// 	struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
	// 	addr = &(ipv6->sin6_addr);
	// 	ipver = "IPv6";
	// 	}
	// 	// convert the IP to a string and print it:
	// 	inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
	// 	printf(" %s: %s\n", ipver, ipstr);
	// }

	char ip4[INET_ADDRSTRLEN];
	struct sockaddr_in myaddr = *((struct sockaddr_in*)(myaddrinfo->ai_addr));
	inet_ntop(AF_INET, &(myaddr.sin_addr), ip4, INET_ADDRSTRLEN);
	cout << "My ip is: " << ip4 << endl;

	int myfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(myfd == -1) {
		perror("Socket");
		exit(1);
	}

	//To reuse portno
	int yes=1;
	if (setsockopt(myfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
		perror("setsockopt");
		exit(1);
	}

	//BIND
	if( bind(myfd, (struct sockaddr *)&myaddr, sizeof myaddr) == -1) {
		perror("bind");
		exit(1);
	}

	close(myfd);
	freeaddrinfo(myaddrinfo);
	return 0;
}
