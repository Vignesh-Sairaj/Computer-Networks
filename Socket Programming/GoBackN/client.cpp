#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>

#define SERVPORTNO "9999"


#define SWS 2
#define RWS 2
#define NUMSEQ (RWS + SWS)

using namespace std;

int recvFrame(int servfd, int seqno, vector<int>& word);


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
    vector<int> word(32);


	close(servfd);
	return 0;
}


int recvFrame(int servfd, int seqno, vector<int>& word) {
/*
    recv int sequence num
    recv int sizeof word
    recv int word[]

    send [ack/nack, seqno]

    returns ack - 1
    nack - 0
*/
    int senderSeqno;
    int ackMsg[2] = {1, seqno};
    do {

        //cout << "First recv..." << endl;
        if(recv(servfd, (void*)&senderSeqno, sizeof(senderSeqno), MSG_WAITALL) != sizeof(senderSeqno)) {
            fprintf(stderr, "recvFrame: too lazy to handle incomplete int (senderSeqno) recv\n");
            exit(1);
        }

        //cout << "Second recv..." << endl;
        int wordSize;

        if(recv(servfd, (void*)&wordSize, sizeof(wordSize), MSG_WAITALL) != sizeof(wordSize) ) {
            fprintf(stderr, "recvFrame: too lazy to handle incomplete int (wordSize) recv\n");
            exit(1);
        }

        word.resize(wordSize/(sizeof(int)));
        //cout << "Third recv..." << endl;
        if ((recv(servfd, &word[0], wordSize, MSG_WAITALL)) == -1) {
            perror("recv");
            exit(1);
        }

        //Implement check


        if (send(servfd, ackMsg, sizeof ackMsg, 0) != sizeof ackMsg) {
            fprintf(stderr, "recvFrame: too lazy to handle incomplete int (ack) send\n");
            exit(1);
        }
    } while (senderSeqno != seqno);

    return ackMsg[0];
}
