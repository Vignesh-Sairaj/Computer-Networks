#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NUM_CLIENTS 4

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
  
  int client_fd_arr[NUM_CLIENTS]; //Array that contains client_fd's
  
  //Array zero-indexed, i.e., client 1's fd = client_fd_arr[0]
  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    struct sockaddr_in client_addr;
    socklen_t client_size=sizeof client_addr;
    int client_fd=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);

    if(client_fd > 0)
      cout << "Accepted incoming connection from " << ntohl(client_addr.sin_addr.s_addr) << ":" << ntohs(client_addr.sin_port) << endl;
    else {
      cout<<"\nXXXXXXXconnection failureXXXXXXX\n";
      return 1;
    }
    client_fd_arr[i] = client_fd;
  }



  int array[50];

  int n;

  cout << "Enter n: ";
  cin >> n;
  cout << endl;


  for (int i = 0; i < n; ++i)
  {
    cout << "Enter array[" << i << "]" << ": ";
    cin >> array[i];
  }

  cout << "\nArray = { ";
  for (int i = 0; i < n; ++i)
  {
    cout << array[i]  << "\t";
  }
  cout << "}" << endl;

  int key;
  cout << "Enter key: ";
  cin >> key;


  int q = n/NUM_CLIENTS;
  int r = n%NUM_CLIENTS;

  int qplusr = q+r;

  for (int i = 0; i < NUM_CLIENTS-1; ++i)
  {
    temp=send(client_fd_arr[i],&q,sizeof(int),0);
    temp=send(client_fd_arr[i],(array + i*q),q*(sizeof(int)),0);
  }

  cout << "hoohaa" << endl;

  temp=send(client_fd_arr[(NUM_CLIENTS-1)],&qplusr,sizeof(int),0);
  temp=send(client_fd_arr[(NUM_CLIENTS-1)],(array + (NUM_CLIENTS-1)*q),(q+r)*(sizeof(int)),0);

  cout << "hoohaa" << endl;

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    temp=send(client_fd_arr[i],&key,sizeof(int),0);
    cout << temp << endl;
  }

  cout << "hoohaa" << endl;

  int yes[NUM_CLIENTS];
  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    temp = recv(client_fd_arr[i], &yes[i], sizeof(int), 0);
  }

  cout << "hoohaa" << endl;

  int ans = yes[0] || yes[1] || yes[2] || yes[3];

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    temp=send(client_fd_arr[i],&ans,sizeof(int),0);
  }
  

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    close(client_fd_arr[i]);
  }
   
  close(sockfd);

  return 0;
}
