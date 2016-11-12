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


  pid_t pid = fork();

  if (pid<0) {
    cerr << "FORK ERROR\n";
    exit(1);
  }

  else if (pid > 0){ //Parent: send
    while(true) {
      int temp;
      char buf[100];
      cout << endl <<"Enter the message to server: ";
      cin.getline(buf, 100);

      temp=send(sockfd,buf,strlen(buf)+1,0);

      char quit[10] = "quit";

      if (strcmp(buf, quit) == 0) {
        break;
      }
    }
  }
  
  else { //Child: Recv
    while(true) {
      int temp;
      char buf[100];

      temp=recv(sockfd,buf,sizeof buf,0);
      cout << endl << "Server> " << buf << endl << "Your reply>" << endl;

      char quit[10] = "quit";

      if (strcmp(buf, quit) == 0) {
        break;
      }
    }
  }

  close(sockfd);
  return 0;
}
