#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define SERVPORTNO "9999"

#define MAX_TXT_BUF_SIZE 15000
#define WRD_SIZE 32
#define MSG_LEN 33
#define MSG_SIZE (33 * sizeof(int)

using namespace std;

int recvFrame(int servfd, int seqno, vector<int>& word);
string vectorToTxt(vector<int> intVec);
bool vrcDecode(std::vector<int> rcvMsg, int wrdSize);


ofstream recvOut;

int main (void) {
    srand(time(NULL));
    recvOut.open("receiverout.txt");


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

    int numFrames, numPaddedBits;

    //RECVINGNUMFRAMES
    if(recv(servfd, (void*)&numFrames, sizeof(numFrames), MSG_WAITALL) != sizeof(numFrames)) {
        fprintf(stderr, "Recv: too lazy to handle incomplete int (numFrames) recv\n");
        exit(1);
    }

    //RECVING NUMPADDEDBITS
    if(recv(servfd, (void*)&numPaddedBits, sizeof(numPaddedBits), MSG_WAITALL) != sizeof(numPaddedBits)) {
        fprintf(stderr, "Recv: too lazy to handle incomplete int (numPaddedBits) recv\n");
        exit(1);
    }

    vector<int> paddedVec (numFrames*WRD_SIZE);

    for (int i = 0; i < numFrames; ++i) {
        vector<int> word(MSG_LEN);
        cout << "i = " << i << "/" << numFrames << endl;
        int recvd;
        do {
            recvd = recvFrame(servfd, i, word);
            if (recvd) {
                recvOut << "Received frame " << i << "successfully, sending ack\n";
            }
            else
                recvOut << "Received frame " << i << "UNsuccessfully, sending NACK\n";
        } while(!recvd);
        copy(word.begin(), word.begin()+WRD_SIZE, paddedVec.begin() + i*WRD_SIZE);
    }

    for (int i = 0; i < numPaddedBits; ++i) {
        paddedVec.pop_back();
    }

    cout << "Final output:" << endl;
    cout << vectorToTxt(paddedVec);
    cout << endl;

    ofstream oFile;
    oFile.open("multiportCpy.cpp");

    oFile << vectorToTxt(paddedVec);

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
        ackMsg[0] = vrcDecode(word, WRD_SIZE);


        if (send(servfd, ackMsg, sizeof ackMsg, 0) != sizeof ackMsg) {
            fprintf(stderr, "recvFrame: too lazy to handle incomplete int (ack) send\n");
            exit(1);
        }
    } while (senderSeqno != seqno);
    
    return ackMsg[0];
}

string vectorToTxt(vector<int> intVec) {

    if (intVec.size() % 8 != 0) {
        throw runtime_error("ERROR: intVec size in vectorToTxt is invalid.\n");
    }

    string str;
    str.reserve(intVec.size()/8);

    for (unsigned int i = 0; i < intVec.size(); i+=8) {

        vector<int> byte(intVec.begin()+i, intVec.begin()+i+8);

        //cout << "bits: ";
        for (int j = 0; j < 8; ++j) {
            //cout << " " << byte[j] << " ";
        }
        //cout << endl;


        int ascii = 0;
        int j = 7;
        int placeVal = 1;
        while(j >= 0) {
            ascii += byte[j]*placeVal;
            placeVal*=2;
            j--;
        }
        str += (char)ascii;
        //cout << "ascii int = " << ascii << " :: ascii char = " << (char)ascii << endl;
    }
    return str;
}
bool vrcDecode(std::vector<int> rcvMsg, int wrdSize) {
    if(rcvMsg.size()%(wrdSize+1) != 0) { //One extra bit per word
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    int numWrds = rcvMsg.size()/(wrdSize+1);

    bool correct = true;

    for (int i = 0; i < numWrds; ++i)
    {
        int parity = 0;

        for (int j = (wrdSize+1)*i; j < (wrdSize+1)*(i+1); ++j)
        {
            parity = (rcvMsg[j] != 0) != (parity != 0); //xor
        }

        if (parity != 0) {
            correct = false;
            break;
        }
    }

    return correct;
}