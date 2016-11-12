#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>

#define MYPORTNO "9999"
#define MSGLEN 12 // including seqno

#define SWS 2
#define RWS 2
#define NUMSEQ (RWS + SWS)
#define TIME 1

using namespace std;


int clientfd;
vector<int> MSG {-1, 666, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

bool nackYet = false;
int oldWinSeqNo = 0;
int newWinSeqNo = oldWinSeqNo;
list<int> currSeqNo(0);


void* recvThread(void* arg);
int sendFrame(int clientfd, int seqno, vector<int> word);

pthread_mutex_t mutex;

int main (void) {

    pthread_mutex_init(&mutex, NULL);

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

	char ip4[INET_ADDRSTRLEN];
	struct sockaddr_in myaddr = *((struct sockaddr_in*)(myaddrinfo->ai_addr));
	inet_ntop(AF_INET, &(myaddr.sin_addr), ip4, INET_ADDRSTRLEN);
	cout << "My ip is: " << ip4 << endl;

	int myfd = socket(PF_INET, SOCK_STREAM, 0);
	if(myfd == -1) {
		perror("Socket");
		exit(1);
	}

    freeaddrinfo(myaddrinfo);

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

    //listen
    if( listen(myfd, 10) == -1) {
        perror("listen");
        exit(1);
    }


    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof clientaddr;

    //ACCEPT
    clientfd = accept(myfd, (struct sockaddr*)&clientaddr , &clientaddrlen);
    if (clientfd == -1) {
        perror("accept");
        exit(1);
    }

    char clientip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientaddr.sin_addr), clientip, INET_ADDRSTRLEN);
	cout << "Successfully accepted connection from: " << clientip << endl;

    pthread_t tid;
    pthread_create(&tid, NULL, &recvThread, NULL);


    for(int i=0; i < SWS; i++) {
        MSG[1] = (i+oldWinSeqNo)%NUMSEQ;

        if(send(clientfd, (void*)&MSG[0], MSG.size()*(sizeof int), 0) != MSG.size()*(sizeof int) ) {
            fprintf(stderr, "SendFrame: too lazy to handle incomplete int array send\n");
            exit(1);
        }
    }

    while(true) {
        sleep(TIME);


    }

    pthread_join(tid, NULL);
	close(myfd);
    close(clientfd);
	return 0;
}



void* recvThread(void* arg) {
    /*
        recvs msg in format [ACK/NACK, Fseqnum]
        ACK - 1
    */


    while(true) {
        int recvMsg[2];
        if (recv(clientfd, recvMsg, sizeof recvMsg, MSG_WAITALL) != sizeof recvMsg) {
            fprintf(stderr, "SendFrame: too lazy to handle array of ints[2] (ack/nack) recv\n");
            exit(1);
        }

        pthred_mutex_lock(&mutex);
        if(recvMsg[0] == 1) { //ACK
            if(recvMsg[1] < 0 || n > SWS) {
                fprintf(stderr, "Junk seqnumber recvd\n");
                exit(1);
            }
            if(recvMsg[1] != (oldWinSeqNo+RWS)%NUMSEQ) {
                fprintf(stdout, "Wrong ACK (possibly delayed) recv\n");
                continue;
            }
            else {
                newWinSeqNo = recvMsg[1];
            }
        }

        else if (recvMsg[0] == 0){ //NACK
            int n = recvMsg[1];
            if(n < 0 || n > SWS) {
                fprintf(stderr, "Junk seqnumber recvd\n");
                exit(1);
            }

            currSeqNo.push_back(n);
            nackYet = true;
        }

        else {
            fprintf(stderr, "Recvd junk val for recvMsg[0] (ack/nack) recvd\n");
            exit(1);
        }

        pthred_mutex_unlock(&mutex);
    }


    pthread_exit(NULL);
}



int sendFrame(int clientfd, int seqno, vector<int> word) {
/*
    sends int sequence num
    Sends int sizeof word
    sends int word[]

    recv [ack/nack, seqno]

    returns ack - 1
    nack - 0
*/

    int recvMsg[2];

    do {

        //cout << "First send..." << endl;
        if(send(clientfd, (void*)&seqno, sizeof(seqno), 0) != sizeof(seqno)) {
            fprintf(stderr, "SendFrame: too lazy to handle incomplete int (seqno) send\n");
            exit(1);
        }

        int wordSizeLeft = word.size()*sizeof(int);

        //cout << "Second send..." << endl;
        if(send(clientfd, (void*)&wordSizeLeft, sizeof(wordSizeLeft), 0) != sizeof(wordSizeLeft) ) {
            fprintf(stderr, "SendFrame: too lazy to handle incomplete int (wordSize) send\n");
            exit(1);
        }

        void* ptr = (void*)&word[0];

        //cout << "Third sends..." << endl;
        while(wordSizeLeft > 0) {
            int sent;
            if ((sent = send(clientfd, ptr, wordSizeLeft, 0)) == -1) {
                perror("send");
                exit(1);
            }
            unsigned char* charptr = (unsigned char*)ptr + sent;
            ptr = (void*)charptr;
            wordSizeLeft -= sent;
        }

        if (recv(clientfd, recvMsg, sizeof recvMsg, MSG_WAITALL) != sizeof recvMsg) {
            fprintf(stderr, "SendFrame: too lazy to handle incomplete int (ack) recv\n");
            exit(1);
        }
    } while (recvMsg[1] != seqno);

    //cout << "returning...";
    return recvMsg[0];
}
