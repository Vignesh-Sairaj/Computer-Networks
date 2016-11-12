#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc,char* argv[])
{
  if(argc != 3)
    {
      cerr << "Usage syntax: ./client <ip_address> <port_no>" << endl;
      return 0;
    }
  int sockfd;
  struct sockaddr_in my_addr;

  sockfd=socket(PF_INET,SOCK_STREAM,0);

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(atoi(argv[2]));
  my_addr.sin_addr.s_addr=inet_addr(argv[1]);
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=connect(sockfd,(struct sockaddr*)&my_addr, sizeof my_addr);

  if(temp==0)
    cout << "Connection established with server" << endl;
  else
    {
      cout << "Could not establish connection!" << endl;
      return -1;
    }

  int numElts;
  int array[50];
  int key;
  int ans;

  cout << "hoohaa" << endl;

  temp=recv(sockfd,&numElts,sizeof(int),0);
  cout << "hoohaa" << numElts << endl;
  temp=recv(sockfd,array,50*(sizeof(int)),0);

  for (int i = 0; i < numElts; ++i)
  {
    cout << array[i] << "\t" << endl;
  }

  temp=recv(sockfd,&key,sizeof(int),0);

  cout << "hoohaa" << key << endl;

  int yes = 0;

  for (int i = 0; i < numElts; ++i)
  {
    if (array[i] == key)
    {
      yes = 1;
    }
  }
  cout << "hoohaa" << endl;
  temp=send(sockfd,&yes,sizeof(int),0);
  cout << "hoohaa" << endl;
  temp=recv(sockfd,&ans,sizeof(int),0);

  cout << "Ans: " << ans << endl;

  close(sockfd);
  return 0;
}
