#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NUM_CLIENTS 3

using namespace std;

int main(int argc,char* argv[])
{
  if(argc !=2)
    {
      cerr << "Usage syntax: ./server <port_no>" << endl;
      return 0;
    }
  int sockfd;
  struct sockaddr_in my_addr;

  sockfd=socket(PF_INET,SOCK_STREAM,0);  //Creating the socket

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(atoi(argv[1]));
  my_addr.sin_addr.s_addr=INADDR_ANY;
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=bind(sockfd, (struct sockaddr *)&my_addr, (sizeof my_addr)); //Binding the socket to the address

  if(temp==0)
    cout << "Server started running at "<< inet_ntoa(my_addr.sin_addr)  << ":" << argv[1] << endl;
  else {
    cout << "Failed to bind socket" << endl;
    return -1;
  }



  listen(sockfd, NUM_CLIENTS);

  int client_fd_arr[NUM_CLIENTS];
  
  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    struct sockaddr_in client_addr;
    socklen_t client_size=sizeof client_addr;
    int client_fd=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);

    if(client_fd > 0)
      cout << "Accepted incoming connection" << endl;
    else {
      cout<<"\nXXXXXXXconnection failureXXXXXXX\n";
      return 1;
    }
    client_fd_arr[i] = client_fd;

    //Welcome msg
    char msg[100] = "Hello!\nWelcome to the chat server application.\nHenceforth you shall be identified by your id: ";
    char tmp[50];
    sprintf(tmp, "%d", client_fd);
    strcat(msg, tmp);

    temp=send(client_fd,msg,strlen(msg)+1,0);
  }


  cout << "\n\nClients connected (sockfd):\n";

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    cout << ">> [" << i << "] sockfd - " << client_fd_arr[i] << endl;
  }


  int numSel;

  do {
    cout << "\n\n" << NUM_CLIENTS << " clients connected, enter number of clients to host conference: ";
    cin >> numSel;
  } while (numSel <= 0 || numSel > NUM_CLIENTS);

  int selClientInd[numSel];

  for (int i = 0; i < numSel; ++i) {
    do {
      cout << "Enter index of client " << i <<" to send message to (b/w 0 and " << NUM_CLIENTS-1 << "): ";
      cin >> selClientInd[i];
    } while (selClientInd[i] < 0 || selClientInd[i] > NUM_CLIENTS-1);
  }


  int selArr[NUM_CLIENTS];

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    selArr[i] = 0;
  }

  for (int i = 0; i < numSel; ++i)
  {
    selArr[selClientInd[i]] = 1;
  }


  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    if(selArr[i] == 1) { // If selected

      //Part of conference
      char buf[] = "1";
      int temp=send(client_fd_arr[i],buf,strlen(buf)+1,0);

      if(temp == -1) {
        cout << "Error: not sent" << endl;
        exit(1);
      }
    }

    else {
      //NOT part of conference
      char buf[] = "0";
      temp=send(client_fd_arr[i],buf,strlen(buf)+1,0);
    }
  }

  int connfd;

  cout << "\n\nStarting conference...\n" << endl;

  //FORK once for each client
  for (int i = 0; i < numSel; ++i)
  {
    connfd = client_fd_arr[selClientInd[i]];
    pid_t pid = fork();

    if (pid<0) {
      cerr << "FORK ERROR\n";
      exit(1);
    }

    else if(pid == 0) {
      break;
    }

    //Last remaining parent after numSel forks
    else if (pid > 0 && i == numSel-1)
    {
      return 0;
    }
  }

  //Server now forked into numSel processes, each tends to a different connfd
  while(true) {
    int temp;
    char buf[100];

    temp=recv(connfd,buf,sizeof buf,0);
    cout << "Rcvd " << temp << " bytes from clientfd: " << connfd << endl;

    char sendStr[150];
    sprintf(sendStr, "%d> ", connfd);
    strncat(sendStr, buf, 100);

    //Broadcast
    for (int i = 0; i < numSel; ++i)
    {
      if (client_fd_arr[selClientInd[i]] != connfd)
      {
        temp=send(client_fd_arr[selClientInd[i]],sendStr,strlen(sendStr)+1,0);
        cout << "Sent " << temp << " bytes to clientfd: " << client_fd_arr[selClientInd[i]] << endl;
      }
    }

    char quit[10] = "quit";

    if (strcmp(buf, quit) == 0) {
      temp=send(connfd,buf,strlen(buf)+1,0);
      cout << "Sent " << temp << " quit signal to clientfd: " << connfd << endl;
      sleep(1);
      break;
    }
  }

  close(connfd);
  return 0;
}
