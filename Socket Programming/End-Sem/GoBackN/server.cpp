#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORTNO "9999"
#define RECP_PROB_FAILURE 4
#define NUMSEQ 8
#define SWS 7
#define RWS 1

using namespace std;


void disp(vector<int> v);

int sendFrame(int clientfd, int seqno, vector<int> word);
vector<int> genBern(int size);

vector<int> word {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
vector<int> refWord {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

ofstream sendOut;

int main (void) {
    sendOut.open("senderout.txt");

    srand(time(NULL));

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
    //accept
    int clientfd = accept(myfd, (struct sockaddr*)&clientaddr , &clientaddrlen);
    if (clientfd == -1) {
        perror("accept");
        exit(1);
    }

    char clientip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientaddr.sin_addr), clientip, INET_ADDRSTRLEN);
    cout << "Successfully accepted connection from: " << clientip << endl;

    double currTime = 0;

    int i = 0;

    //exp
    while(true) {

        for (int j = 0; j < SWS; ++j) {

            cout << "i = " << i << "/" << 10 << endl;

            int ack;
            {

                sendOut << "Sending frame " << i << endl;
                int choice;
                do{
                    cout << "Enter 1 to send correctly, 2 to corrupt: ";
                    cin >> choice;
                }while(choice < 1 || choice > 2);

                if(choice == 2){
                    sendOut << "corrupting..." << endl;
                    vector<int> error = genBern(10);

                    for (int i = 0; i < 10; ++i) {
                        if(error[i]) {
                            word[i] = (word[i]==0);//bit flip
                        }
                    }

                    disp(word);
                    ack = sendFrame(clientfd, i, word);
                    sendOut << "recvd nack " << i << endl << endl;
                    word = refWord;
                    break;
                }
                else {
                    disp(word);
                    ack = sendFrame(clientfd, i, word);
                    if (ack == 1)
                        ;
                    else
                        sendOut << "timeout\n" << endl << endl;
                        break;
                }

                i = (i+1)%NUMSEQ;
                word = refWord; //reset
            }
        }
        sendOut << "recvd ack\n" << endl << endl;
    }

    //cout << "word1 == word2 = " << equal(word1.begin(), word1.end(), word2.begin())<< endl;

    close(myfd);
    //close(clientfd);
    return 0;
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

vector<int> genBern(int size) {
    vector<int> ans(size);

    for (int i = 0; i < size; ++i) {
        if(rand()%RECP_PROB_FAILURE == 0) {
            ans[i] = 1;
        }
        else
            ans[i] = 0;
    }
    return ans;
}

void disp(vector<int> v) {
    sendOut << "{";
    for (int i = 0; i < v.size(); ++i)
    {
        sendOut << " " << v[i] << " ";
    }
    sendOut << "}" << endl;
} 