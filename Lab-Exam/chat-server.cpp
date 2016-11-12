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
  else {
    cout << "Failed to bind socket" << endl;
    return -1;
  }


  listen(sockfd, NUM_CLIENTS);
  
  int client_fd_arr[NUM_CLIENTS]; //Array that contains client_fd's
  char nicknames[NUM_CLIENTS][50];
  
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

    //Role assignment -- which client is client 1, etc.
    char msg[100];
    sprintf(msg, "%d", i+1);
    temp=send(client_fd,msg,strlen(msg)+1,0);
    
    //Nickname
    char nick[50];
    temp=recv(client_fd,nick,sizeof nick,0);
    strcpy(nicknames[i], nick);
  }

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    cout << nicknames[i] << endl;
  }



  {
    FILE *fp = fopen("chat1.txt", "w");
    fclose(fp);
  }

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    cout << nicknames[i] << endl;
  }

  cout << endl << "Chat 1 about to commence..." << endl;



  while(true) {
    char buf[256];
    temp=recv(client_fd_arr[0],buf,sizeof buf,0);
    cout << "Rcvd " << temp << " bytes from: " << nicknames[0] << endl;

    char msg[512];

    sprintf(msg, "%s>\t%s", nicknames[0], buf); //Append nickname to message

    temp=send(client_fd_arr[1],msg,strlen(msg)+1,0);
    cout << "Sent " << temp << " bytes to: " << nicknames[1] << endl;

    char newline[] = "\n";
    strcat(msg, newline);
    
    FILE *fp = fopen("chat1.txt", "a");
    fprintf(fp, "%s", msg);
    fclose(fp);

    char quit[10] = "quit";

    //send quit signal to other client to stop receiving
    if (strcmp(buf, quit) == 0) {
      temp=send(client_fd_arr[1],buf,strlen(buf)+1,0);
      cout << "Sent " << temp << " quit signal to: " << nicknames[1] << endl;
      cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
      return 0;
    }
    
    {
      char buf[256];
      temp=recv(client_fd_arr[1],buf,sizeof buf,0);
      cout << "Rcvd " << temp << " bytes from: " << nicknames[1] << endl;

      char msg[512];

      sprintf(msg, "%s>\t%s", nicknames[1], buf); //Append nickname to message

      temp=send(client_fd_arr[0],msg,strlen(msg)+1,0);
      cout << "Sent " << temp << " bytes to: " << nicknames[0] << endl;

      char newline[] = "\n";
      strcat(msg, newline);
      
      FILE *fp = fopen("chat1.txt", "a");
      fprintf(fp, "%s", msg);
      fclose(fp);

      char quit[10] = "quit";

      //send quit signal to other client to stop receiving
      if (strcmp(buf, quit) == 0) {
        temp=send(client_fd_arr[0],buf,strlen(buf)+1,0);
        temp=send(client_fd_arr[0],buf,strlen(buf)+1,0);
        cout << "Sent " << temp << " quit signal to: " << nicknames[0] << endl;
        cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
        sleep(10);
        return 0;
      }
    }
  }

/*  //Chat 1, clients 1 & 2
  for (int i = 0; i < 2; ++i) {
    
    fflush(NULL);
    pid_t pid = fork();

    if (pid<0) {
      cerr << "FORK ERROR\n";
      exit(1);
    }

    if (pid == 0) // Child
    {
      if (i == 0) // client 1
      {
        char welcome_msg[100] = "Chat session 1 commencing...\nYou are talking to: ";
        strcat(welcome_msg, nicknames[1]); //Client 2

        int temp=send(client_fd_arr[0],welcome_msg,strlen(welcome_msg)+1,0);

        while(true) {
          char buf[256];
          temp=recv(client_fd_arr[0],buf,sizeof buf,0);
          cout << "Rcvd " << temp << " bytes from: " << nicknames[0] << endl;

          char msg[512];

          sprintf(msg, "%s>\t%s", nicknames[0], buf); //Append nickname to message

          temp=send(client_fd_arr[1],msg,strlen(msg)+1,0);
          cout << "Sent " << temp << " bytes to: " << nicknames[1] << endl;

          char newline[] = "\n";
          strcat(msg, newline);
          
          FILE *fp = fopen("chat1.txt", "a");
          fprintf(fp, "%s", msg);
          fclose(fp);

          char quit[10] = "quit";

          //send quit signal to other client to stop receiving
          if (strcmp(buf, quit) == 0) {
            temp=send(client_fd_arr[1],buf,strlen(buf)+1,0);
            temp=send(client_fd_arr[1],buf,strlen(buf)+1,0);
            cout << "Sent " << temp << " quit signal to: " << nicknames[1] << endl;
            cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
            fflush(0);
            sleep(10);
            return 0;
          }
        }
      }

      else if (i == 1) // Client 2
      {
        char welcome_msg[100] = "Chat session 1 commencing...\nYou are talking to: ";
        strcat(welcome_msg, nicknames[0]); //Client 1

        int temp=send(client_fd_arr[1],welcome_msg,strlen(welcome_msg)+1,0);

        while(true) {
          char buf[256];
          temp=recv(client_fd_arr[1],buf,sizeof buf,0);
          cout << "Rcvd " << temp << " bytes from: " << nicknames[1] << endl;

          char msg[512];

          sprintf(msg, "%s>\t%s", nicknames[1], buf); //Append nickname to message

          temp=send(client_fd_arr[0],msg,strlen(msg)+1,0);
          cout << "Sent " << temp << " bytes to: " << nicknames[0] << endl;

          char newline[] = "\n";
          strcat(msg, newline);
          
          FILE *fp = fopen("chat1.txt", "a");
          fprintf(fp, "%s", msg);
          fclose(fp);

          char quit[10] = "quit";

          //send quit signal to other client to stop receiving
          if (strcmp(buf, quit) == 0) {
            temp=send(client_fd_arr[0],buf,strlen(buf)+1,0);
            temp=send(client_fd_arr[0],buf,strlen(buf)+1,0);
            cout << "Sent " << temp << " quit signal to: " << nicknames[0] << endl;
            cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
            sleep(10);
            return 0;
          }
        }
      }
    }
  }*/

  wait(NULL); wait(NULL);

  cout << endl << "Chat 2 about to commence..." << endl;

  {
    FILE *fp = fopen("chat2.txt", "w");
    fclose(fp);
  }

  //Chat 2, clients 2 & 3
  for (int i = 0; i < 2; ++i) {
    
    fflush(NULL);
    pid_t pid = fork();

    if (pid<0) {
      cerr << "FORK ERROR\n";
      exit(1);
    }

    if (pid == 0) // Child
    {
      if (i == 0) // client 2
      {
        char welcome_msg[100] = "Chat session 2 commencing...\nYou are talking to: ";
        strcat(welcome_msg, nicknames[2]); //Client 3

        int temp=send(client_fd_arr[1],welcome_msg,strlen(welcome_msg)+1,0);

        while(true) {
          char buf[256];
          temp=recv(client_fd_arr[1],buf,sizeof buf,0);
          cout << "Rcvd " << temp << " bytes from: " << nicknames[1] << endl;

          char msg[512];

          sprintf(msg, "%s>\t%s", nicknames[1], buf); //Append nickname to message


          //Send to client 3
          temp=send(client_fd_arr[2],msg,strlen(msg)+1,0);
          cout << "Sent " << temp << " bytes to: " << nicknames[2] << endl;

          char newline[] = "\n";
          strcat(msg, newline);
          
          FILE *fp = fopen("chat2.txt", "a");
          fprintf(fp, "%s", msg);
          fclose(fp);

          char quit[10] = "quit";

          //send quit signal to other client to stop receiving
          if (strcmp(buf, quit) == 0) {
            temp=send(client_fd_arr[2],buf,strlen(buf)+1,0);
            temp=send(client_fd_arr[2],buf,strlen(buf)+1,0);
            cout << "Sent " << temp << " quit signal to: " << nicknames[2] << endl;
            cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
            sleep(10);
            return 0;
          }
        }
      }

      else if (i == 1) // Client 3
      {
        char welcome_msg[100] = "Chat session 2 commencing...\nYou are talking to: ";
        strcat(welcome_msg, nicknames[1]); //Client 2

        int temp=send(client_fd_arr[2],welcome_msg,strlen(welcome_msg)+1,0);

        while(true) {
          char buf[256];
          temp=recv(client_fd_arr[2],buf,sizeof buf,0);
          cout << "Rcvd " << temp << " bytes from: " << nicknames[2] << endl;

          char msg[512];

          sprintf(msg, "%s>\t%s", nicknames[2], buf); //Append nickname to message


          //Send to client 2
          temp=send(client_fd_arr[1],msg,strlen(msg)+1,0);
          cout << "Sent " << temp << " bytes to: " << nicknames[1] << endl;

          char newline[] = "\n";
          strcat(msg, newline);
          
          FILE *fp = fopen("chat2.txt", "a");
          fprintf(fp, "%s", msg);
          fclose(fp);

          char quit[10] = "quit";

          //send quit signal to other client to stop receiving
          if (strcmp(buf, quit) == 0) {
            temp=send(client_fd_arr[1],buf,strlen(buf)+1,0);
            temp=send(client_fd_arr[1],buf,strlen(buf)+1,0);
            cout << "Sent " << temp << " quit signal to: " << nicknames[1] << endl;
            cout << endl << "Quitting Server broadcast..." << endl; fflush(0);
            sleep(10);
            return 0;
          }
        }
      }
    }
  }

  wait(NULL); wait(NULL);


  FILE* fp1 = fopen("chat1.txt", "r");
  if(fp1==NULL)
  {
    cout<<"\nXXXXXFILE OPEN ERRORXXXXXX\n";
    return 1;
  }
  /* Read data from file and send it */
  while(1)
  {
    unsigned char buff[256]={0};
    int nread =fread(buff,1,256,fp1);
    cout<<"\nBYTES READ:"<<nread;
    if(nread>0)
    {
      cout<<"\n=====SENDING========\n";
      write(client_fd_arr[3],buff,nread);
    }
    if(nread<256)
    {
      if(feof(fp1))
        cout<<"\n=====END OF FILE======\n";
      if(ferror(fp1))
        cout<<"\nXXXXXXERROR READING\n";
      break;
    }
  }


  FILE* fp2 = fopen("chat2.txt", "r");
  if(fp2==NULL)
  {
    cout<<"\nXXXXXFILE OPEN ERRORXXXXXX\n";
    return 1;
  }
  /* Read data from file and send it */
  while(1)
  {
    unsigned char buff[256]={0};
    int nread =fread(buff,1,256,fp2);
    cout<<"\nBYTES READ:"<<nread;
    if(nread>0)
    {
      cout<<"\n=====SENDING========\n";
      write(client_fd_arr[3],buff,nread);
    }
    if(nread<256)
    {
      if(feof(fp2))
        cout<<"\n=====END OF FILE======\n";
      if(ferror(fp2))
        cout<<"\nXXXXXXERROR READING\n";
      break;
    }
  }

  for (int i = 0; i < NUM_CLIENTS; ++i)
  {
    close(client_fd_arr[i]);
  }

  return 0;
}  
