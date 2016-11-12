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

#define NUM_CLIENTS 2

using namespace std;

int main(int argc,char* argv[])
{
  if(argc !=3)
    {
      cerr << "Usage syntax: ./server <port_no> <file_name>" << endl;
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
  /* Open the file to be transfered */
  FILE *fp=fopen(argv[2],"rb");
  if(fp==NULL)
  {
    cout<<"\nXXXXXFILE OPEN ERRORXXXXXX\n";
    return 1;
  }
  /* Read data from file and send it */
  while(1)
  {
    unsigned char buff[256]={0};
    int nread =fread(buff,1,256,fp);
    cout<<"\nBYTES READ:"<<nread;
    if(nread>0)
    {
      cout<<"\n=====SENDING========\n";
      write(client_fd_arr[i],buff,nread);
    }
    if(nread<256)
    {
      if(feof(fp))
        cout<<"\n=====END OF FILE======\n";
      if(ferror(fp))
        cout<<"\nXXXXXXERROE READING\n";
      break;
    }
  }
  close(client_fd_arr[i]);
}

   
  close(sockfd);

  return 0;
}
