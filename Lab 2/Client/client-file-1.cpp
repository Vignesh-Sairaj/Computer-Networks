#include <iostream>
#include <stdio.h>
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
  if(argc != 4)
    {
      cerr << "Usage syntax: ./client <ip_address> <port_no> <file_name>" << endl;
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

  FILE *fp;
  fp=fopen(argv[3],"ab");
  if(NULL==fp)
  {
    cout<<"XXXXXXXXXerror opening new fileXXXXXXXX";
    return 1;
  }

  char recvBuff[256];
  int bytesReceived = 0;

  
  while((bytesReceived=read(sockfd,recvBuff,256))>0)
  {
    cout<<"\nbytes recieved: "<<bytesReceived<<"\n";
    fwrite(recvBuff,1,bytesReceived,fp);
  }
  if(bytesReceived<0)
  {
    cout<<"\nXXXXXREAD ERRORXXXXXXXXXXXX\n";
  }

  fclose(fp);
  close(sockfd);
  return 0;
}
