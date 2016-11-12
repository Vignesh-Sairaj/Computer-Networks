#include <bits/stdc++.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORTNO "9999"

using namespace std;

#define MAX_TXT_BUF_SIZE 15000
#define WRD_SIZE 32
#define MSG_LEN 33
#define MSG_SIZE (33 * sizeof(int))
;

vector<int> vrcEncode(vector<int> msg, int wrdSize);
bool vrcDecode(std::vector<int> rcvMsg, int wrdSize);
vector<int> txtToVector(string str);
string vectorToTxt(vector<int> intVec);

int sendFrame(int clientfd, int seqno, vector<int> word);

ofstream sendOut;

int main (void) {
    srand(time(NULL));
    sendOut.open("senderout.txt");

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
    int clientfd = accept(myfd, (struct sockaddr*)&clientaddr , &clientaddrlen);
    if (clientfd == -1) {
        perror("accept");
        exit(1);
    }

    char clientip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientaddr.sin_addr), clientip, INET_ADDRSTRLEN);
	cout << "Successfully accepted connection from: " << clientip << endl;

    //READING INPUT
    char textbuf[MAX_TXT_BUF_SIZE];
    std::ifstream t ("multiport.cpp");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    vector<int> orgMsg = txtToVector(str);

    //ZERO PADDING
    int numPaddedBits = (WRD_SIZE - orgMsg.size()%WRD_SIZE)%WRD_SIZE;
    vector<int> paddedMsg(orgMsg.begin(), orgMsg.end());
    for (int i = 0; i < numPaddedBits; ++i) {
        paddedMsg.push_back(0);
    }
    int numFrames = paddedMsg.size()/WRD_SIZE;

    //SENDING NUMFRAMES
    if(send(clientfd, (void*)&numFrames, sizeof(numFrames), 0) != sizeof(numFrames)) {
        fprintf(stderr, "SendFrame: too lazy to handle incomplete int (numFrames) send\n");
        exit(1);
    }

    //SENDING NUMPADDEDBITS
    if(send(clientfd, (void*)&numPaddedBits, sizeof(numPaddedBits), 0) != sizeof(numPaddedBits)) {
        fprintf(stderr, "Send: too lazy to handle incomplete int (numPaddedBits) send\n");
        exit(1);
    }

    vector<int> vrcMsg = vrcEncode(paddedMsg, WRD_SIZE);

    for (int i = 0; i < numFrames; ++i) {
        cout << "i = " << i << "/" << numFrames << endl;
        int ack;

        do {
            sendOut << "Sending frame " << i << endl;
            vector<int> word(vrcMsg.begin() + i*MSG_LEN, vrcMsg.begin() + (i+1)*MSG_LEN);
            if(rand()%4 == 0){
                word[0] = word[0] == 0; //flip
                sendOut << "corrupting..." << endl;
                vector<int> disp(word.begin(), word.begin()+WRD_SIZE);
                sendOut << vectorToTxt(disp) << endl;
                sendOut << "recvd nack\n" << endl << endl;
            }
            else {
                vector<int> disp(word.begin(), word.begin()+WRD_SIZE);
                sendOut << vectorToTxt(disp) << endl;
                sendOut << "recvd ack\n" << endl << endl;
            }

            ack = sendFrame(clientfd, i, word);
        } while(!ack);
    }

	close(myfd);
    close(clientfd);
	return 0;
}


vector<int> txtToVector(string str) {
    vector<int> ans;
    ans.reserve(str.size()*8);

    for (unsigned int i = 0; i < str.size(); ++i) {
        int ascii = (int)str[i];
        //
        //out << "ascii char = " << (char)ascii << " :: ascii int = " << ascii << endl;
        vector<int> byte(8, 0);
        int j = 7;
        while(ascii > 0) {
            byte[j] = ascii%2;
            ascii /= 2;
            j--;
        }
        //
        //cout << "bits: ";
        for (int j = 0; j < 8; ++j) {
            ans.push_back(byte[j]);
            //cout << " " << byte[j] << " ";
        }
        //cout << endl;
    }
    return ans;
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

vector<int> vrcEncode(vector<int> msg, int wrdSize) {
    if(msg.size()%(wrdSize) != 0) { 
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    int numWrds = msg.size()/wrdSize;
    std::vector<int> transMsg;

    transMsg.reserve(msg.size() + numWrds);

    for (int i = 0; i < numWrds; ++i)
    {
        int parity = 0;

        for (int j = wrdSize*i; j < wrdSize*(i+1); ++j)
        {
            parity = (msg[j] != 0) != (parity != 0); //xor
            transMsg.push_back(msg[j]);
        }

        transMsg.push_back(parity);
    }

    return transMsg;
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