#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NUM_CLIENTS 2

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
  else
    {
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
  }


  
  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    char buf[50];
    
    temp=recv(client_fd_arr[i],buf,sizeof buf,0);
    cout /*<< "Bytes received=" << temp << endl*/ << "Received data from client=" << buf << endl;
    
    time_t now=time(0);
    char serv_msg[50];
    strcpy(serv_msg,ctime(&now));
    cout << "Current Server Time:" << serv_msg << endl;
    
    temp=send(client_fd_arr[i],serv_msg,sizeof serv_msg,0);
    cout << "Sent " << temp << " bytes" << endl;

    cout << "Sending message back to client\n";

    temp=send(client_fd_arr[i],buf,strlen(buf)+1,0);
    cout << "Sent " << temp << " bytes" << endl;
    
    close(client_fd_arr[i]);
  }
   
  close(sockfd);

  return 0;
}