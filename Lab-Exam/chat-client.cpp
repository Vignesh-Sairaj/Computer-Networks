#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace std;

int main(int argc,char* argv[])
{
  if(argc != 4)
    {
      cerr << "Usage syntax: ./client <ip_address> <port_no> <nickname>" << endl;
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

  char role[100];
  if (recv(sockfd,role,sizeof role,0) == -1) {
    cerr << "Error: No role" << endl;
    exit(1);
  }

  int roleInt = atoi(role);

  cout << "Your role: " << roleInt << endl;

  //Sending nickname
  temp=send(sockfd, argv[3], strlen(argv[3])+1, 0);

  if (roleInt == 1)
  {


    while(true) {

      { // send
        int temp;
        char buf[256];
        cout << endl <<"Chat 1: ";
        cin.getline(buf, 256);

        temp=send(sockfd,buf,strlen(buf)+1,0);

        char quit[10] = "quit";

        if (strcmp(buf, quit) == 0) {
          cout << "Quitting client-send..." << endl;
          return 0;
        }
      }

      int temp;
      char buf[256];

      temp=recv(sockfd,buf,sizeof buf,0);
      if (temp == 0) {
        cout << "Quitting client-receive(temp)..." << endl;
        break;
      }

      cout << endl << buf << endl << endl << "Your reply>" << endl;

      char quit[10] = "quit";

      if (strcmp(buf, quit) == 0) {
        cout << "Quitting client-receive..." << endl;
        break;
      }
    }







/*    char welcome_msg[100];
    if (recv(sockfd,welcome_msg,sizeof welcome_msg,0) <= 0) {
      cerr << "Error: No welcome_msg" << endl;
      exit(1);
    }
    cout << welcome_msg << endl;



    for (int i = 0; i < 2; ++i)
    {
      pid_t pid = fork();

      if (pid == 0 && i == 0) //recv
      {
        while(true) {
          int temp;
          char buf[256];

          temp=recv(sockfd,buf,sizeof buf,0);
          if (temp == 0) {
            cout << "Quitting client-receive(temp)..." << endl;
            return 0;
          }

          cout << endl << buf << endl << endl << "Your reply>" << endl;

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-receive..." << endl;
            return 0;
          }
        }
      }

      else if (pid == 0 && i == 1) // send
      {
        while(true) {
          int temp;
          char buf[256];
          cout << endl <<"Chat 1: ";
          cin.getline(buf, 256);

          temp=send(sockfd,buf,strlen(buf)+1,0);

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-send..." << endl;
            return 0;
          }
        }
      }
    }

    wait(NULL); wait(NULL);*/

  }

  else if (roleInt == 2)
  {
/*    {
      char welcome_msg[100];
      if (recv(sockfd,welcome_msg,sizeof welcome_msg,0) <= 0) {
        cerr << "Error: No welcome_msg" << endl;
        exit(1);
      }
      cout << welcome_msg << endl;      
    }


    for (int i = 0; i < 2; ++i)
    {
      pid_t pid = fork();

      if (pid == 0 && i == 0) //recv
      {
        while(true) {
          int temp;
          char buf[256];

          temp=recv(sockfd,buf,sizeof buf,0);
          
          if (temp == 0){
            cout << "Quitting client-receive(temp)..." << endl;
            return 0;
          }

          cout << endl << buf << endl << endl << "Your reply>" << endl;

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-receive..." << endl;
            return 0;
          }
        }
      }

      else if (pid == 0 && i == 1) // send
      {
        while(true) {
          int temp;
          char buf[256];
          cout << endl <<"Chat 1: ";
          cin.getline(buf, 256);

          temp=send(sockfd,buf,strlen(buf)+1,0);

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-send..." << endl;
            return 0;
          }
        }
      }
    }

    wait(NULL); wait(NULL);


    {
      char welcome_msg[100];
      if (recv(sockfd,welcome_msg,sizeof welcome_msg,0) <= 0) {
        cerr << "Error: No welcome_msg" << endl;
        exit(1);
      }
      cout << welcome_msg << endl;
    }

    for (int i = 0; i < 2; ++i)
    {
      pid_t pid = fork();

      if (pid == 0 && i == 0) //recv
      {
        while(true) {
          int temp;
          char buf[256];

          temp=recv(sockfd,buf,sizeof buf,0);
          
          if (temp == 0){
            cout << "Quitting client-receive(temp)..." << endl;
            return 0;
          }

          cout << endl << buf << endl << endl << "Your reply>" << endl;

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-receive..." << endl;
            return 0;
          }
        }
      }

      else if (pid == 0 && i == 1) // send
      {
        while(true) {
          int temp;
          char buf[256];
          cout << endl <<"Chat 2: ";
          cin.getline(buf, 256);

          temp=send(sockfd,buf,strlen(buf)+1,0);

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-send..." << endl;
            return 0;
          }
        }
      }
    }

    wait(NULL); wait(NULL);*/
  }

  else if (roleInt == 3)
  {
    {
      char welcome_msg[100];
      if (recv(sockfd,welcome_msg,sizeof welcome_msg,0) <= 0) {
        cerr << "Error: No welcome_msg" << endl;
        exit(1);
      }
      cout << welcome_msg << endl;
    }

    for (int i = 0; i < 2; ++i)
    {
      pid_t pid = fork();

      if (pid == 0 && i == 0) //recv
      {
        while(true) {
          int temp;
          char buf[256];

          temp=recv(sockfd,buf,sizeof buf,0);
          
          if (temp == 0) {
            cout << "Quitting client-recv..." << endl;
            return 0;
          }

          cout << endl << buf << endl << endl << "Your reply>" << endl;

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-recv..." << endl;
            return 0;
          }
        }
      }

      else if (pid == 0 && i == 1) // send
      {
        while(true) {
          int temp;
          char buf[256];
          cout << endl <<"Chat 2: ";
          cin.getline(buf, 256);

          temp=send(sockfd,buf,strlen(buf)+1,0);

          char quit[10] = "quit";

          if (strcmp(buf, quit) == 0) {
            cout << "Quitting client-send..." << endl;
            return 0;
          }
        }
      }
    }

    wait(NULL); wait(NULL);
  }

  else if (roleInt == 4)
  {
    {
      FILE *fp1;
      fp1=fopen("chat1_client.txt","ab");
      if(NULL==fp1)
      {
        cout<<"XXXXXXXXXerror opening new fileXXXXXXXX";
        return 1;
      }

      char recvBuff[256];
      int bytesReceived = 0;

      
      while((bytesReceived=read(sockfd,recvBuff,256))>0)
      {
        cout<<"\nbytes recieved: "<<bytesReceived<<"\n";
        fwrite(recvBuff,1,bytesReceived,fp1);
      }
      if(bytesReceived<0)
      {
        cout<<"\nXXXXXREAD ERRORXXXXXXXXXXXX\n";
      }

      fclose(fp1);
    }

    {
      FILE *fp2;
      fp2=fopen("chat2_client.txt","ab");
      if(NULL==fp2)
      {
        cout<<"XXXXXXXXXerror opening new fileXXXXXXXX";
        return 1;
      }

      char recvBuff[256];
      int bytesReceived = 0;

      
      while((bytesReceived=read(sockfd,recvBuff,256))>0)
      {
        cout<<"\nbytes recieved: "<<bytesReceived<<"\n";
        fwrite(recvBuff,1,bytesReceived,fp2);
      }
      if(bytesReceived<0)
      {
        cout<<"\nXXXXXREAD ERRORXXXXXXXXXXXX\n";
      }

      fclose(fp2);
    }
  }


  sleep(10);

  close(sockfd);
  return 0;
}