#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define conn 8

void error(char *m) {
	perror(m);
	exit(1);
}



int main() {

	int sockfd[conn], newsockfd[conn], portno[conn], clilen;
	int buffer[256];
	char buffer1[256];
	struct sockaddr_in serv[conn], cli[conn];
	int o,i,n,r,s,u=0;
	pid_t pid;

	for(o=0;o<conn;o++) {
		sockfd[o] = socket(AF_INET, SOCK_STREAM, 0);

		if(sockfd[o] <0) {
			error("socket");
		}

		bzero((char *)&serv[o], sizeof(serv[o]) );

		printf("enter three port nos ");
		char p1[5];
		scanf("%s",p1);
		portno[o] = atoi(p1);

		serv[o].sin_family = AF_INET;
		serv[o].sin_addr.s_addr = INADDR_ANY;
		serv[o].sin_port = htons(portno[o]);

		if(bind(sockfd[o], (struct sockaddr *)&serv[o], sizeof(serv[o])) < 0) {
			error("binding");
		}

	}
	for(o=0;o<conn;o++) {
		listen(sockfd[o], 20);
		printf("waiting for client %d  \n",o+1);
		if(o<5) {
			//printf("inside if \n");
		newsockfd[o] = accept(sockfd[o], (struct sockaddr *) &cli[o], &clilen);}
		else {
			//break;
			//printf("inside else1\n ");
			//s=7;
		    //newsockfd[o] = accept(sockfd[s], (struct sockaddr *) &cli[s], &clilen);
		    //printf("inside else2\n ");
		}
		printf("\nclient %d is connected \n",o+1);
	}

	newsockfd[5] = accept(sockfd[7], (struct sockaddr *) &cli[5], &clilen);
	printf("\nclient 6 is connected \n");
	newsockfd[6] = accept(sockfd[7], (struct sockaddr *) &cli[6], &clilen);
	printf("\nclient 7 is connected \n");
	newsockfd[7] = accept(sockfd[7], (struct sockaddr *) &cli[7], &clilen);
	printf("\nclient 8 is connected \n");

	for(o=0;o<conn;o++) {
	    pid = fork();
	    if(pid<0) {
	        printf("fork error");
	        return 0;
	    }
	    else {
	        if(pid==0) {
				switch(o) {
					int r,ne,ne1,k=1;
					int op[2];
					op[0]=5;

					int y;


					case 0:
						printf("fibbo process started\n");
						while(1) {
							int no;

							n = read(newsockfd[0], &no, sizeof(no));
							printf("client %d is read\n",o+1);
							no = ntohl(no);
                            {int fin[no];
							if(o==0) {

								int a=1,b=1,c;
								for(i=0;i<no;i++) {
									c=a+b;
									fin[i]=c;

									a=b;
									b=c;
								}
								n=write(newsockfd[0], fin, sizeof(fin));printf("client %d is write\n",o+1);}
							}
						}
						break;
					case 1:
						printf("factorial process started\n");
						while(1) {
							int f=1;
							int no;
							n = read(newsockfd[1], &no, sizeof(no));
							printf("client %d is read\n",o+1);
							no = ntohl(no);
							for(i=1;i<=no;i++) {
								f=f*i;
							}
							f = htonl(f);
							n=write(newsockfd[1], &f, sizeof(f));
							printf("client %d is write\n",o+1);
						}
						break;
					case 2:
						printf("prime check process started\n");
						while(1) {
							int no,f=1;
							char f1[] = "no";
							char t[] = "prime";
							n = read(newsockfd[2], &no, sizeof(no));
							printf("client %d is read\n",o+1);
							no = ntohl(no);
							for(i=no-1;i>1;i--) {
								if(no%i==0) {
									f=0;
									f = htonl(f);
									n = write(newsockfd[2], &f, sizeof(f));
									printf("client %d is write\n",o+1);
									break;
								}
							}
							if(f==1) {
								f= htonl(f);
								n = write(newsockfd[2], &f, sizeof(f));
								printf("client %d is write\n",o+1);
							}
						}
						break;
					case 3:
						printf("check perfect square process started\n");
						while(1) {
							int no,f=0;
							char f1[] = "no";
							char t[] = "perfectsquare";
							n = read(newsockfd[3], &no, sizeof(no));
							printf("client %d is read\n",o+1);
							no = ntohl(no);
							for(i=0;i<=no;i++) {
								if(no==i*i) {
									f=1;
									f= htonl(f);
									n = write(newsockfd[3], &f, sizeof(f));
									printf("client %d is write\n",o+1);
									break;
								}
							}
							if(f==0) {
								f=htonl(f);
								n = write(newsockfd[o], &f, sizeof(f));
								printf("client %d is write\n",o+1);
							}
						}
						break;
					case 4:

						printf("random no process started\n");
						printf("client %d is read\n",o+1);
						n = read(newsockfd[o],&k, sizeof(1));
						k=ntohl(k);
						printf("client rece %d\n",k);
						//time_t t;
						//srand((unsigned) time(&t));
						srand(time(NULL));
							r=rand()%8+1;
							if(r==5) {
								r=r+1;
							}
							printf("client %d is write %d\n",o+1,r);
							r = htonl(r);
							n = write(newsockfd[4], &r, sizeof(r));
							//printf("client %d is write %d\n",o+1,r);
					    //printf("enter choice \n");
					    //scanf("%d",&y);

					    op[0]=5;
						n = write(newsockfd[o], op, sizeof(op));

						printf("client %d is write \n",o+1);
						r=ntohl(r);
				        op[0]=5;
						n = write(newsockfd[r-1],op , sizeof(op));

						printf("client %d is write \n",r);
						listen(sockfd[5],5);
						ne = accept(sockfd[5], (struct sockaddr *) &cli[5], &clilen);
						ne1 = accept(sockfd[5], (struct sockaddr *) &cli[5], &clilen);
						FILE *fp;

						pid = fork();

						if(pid<0) {
							printf("error in fork");
							return 0;
						}
						else {
							char y[] = "kill";
							if(pid==0) {

								for(;;) {
									bzero(buffer1, 256);
									n = recv(ne, buffer1, 256,0);
									fp = fopen("backup.txt","a+");
									fprintf(fp, "%s\n",buffer1 );
									fclose(fp);
									if(n < 0) {
										error("read" );
									}
									if(buffer1[0]=='k') {
										printf("server closing ");
										break;
									}
									n=send(ne1, buffer1, sizeof(buffer1),0);
									if(n < 0) {
										error("write");
									}
								}


								{
						int ne,ne1;
						FILE *fp;
						int arr[conn];
						printf("file transfer process started\n");
						listen(sockfd[o],5);

						ne = accept(sockfd[6], (struct sockaddr *) &cli[6], &clilen);
						printf("connection 1 for file trasfer accepted\n");
						ne1 = accept(sockfd[6], (struct sockaddr *) &cli[6], &clilen);
						printf("connection 2 for file transfer  accepted\n");

						fp = fopen("backup.txt","a+");
						while(1) {

							unsigned char buff[256]={0};
							int nread =fread(buff,1,256,fp);
							//cout<<"\nBYTES READ:"<<nread;
							if(nread>0)
							{
								printf("\n=====SENDING========\n");
								write(ne,buff,nread);
							}
							if(nread<256)
							{
								if(feof(fp))
									printf("\n=====END OF FILE======\n");
								if(ferror(fp))
									printf("\nXXXXXXERROE READING\n");
								break;
							}
						}
						fclose(fp);
						fp = fopen("backup.txt","a+");
						while(1) {

							unsigned char buff[256]={0};
							int nread =fread(buff,1,256,fp);
							//"\nBYTES READ:"<<nread;
							if(nread>0)
							{
								printf("\n=====SENDING========\n");
								write(ne1,buff,nread);
							}
							if(nread<256)
							{
								if(feof(fp))
									printf("\n=====END OF FILE======\n");
								if(ferror(fp))
									printf("\nXXXXXXERROE READING\n");
								break;
							}
						}
					fclose(fp);


						}


							}
							else {
								FILE *fp;



								for(;;) {
									bzero(buffer1, 256);
									n = recv(ne1, buffer1, 256,0);
									fp= fopen("backup.txt","a+");

									fprintf(fp, "\t\t\t\t\t\t\t\t%s\n",buffer1 );
									fclose(fp);
									if(n < 0) {
										error("read" );
									}
									if(buffer1[0]=='k') {
										printf("server closing ");
										break;
									}
									n=send(ne, buffer1, sizeof(buffer1),0);
									if(n < 0) {
										error("write");
									}


								}



							}
						}



						break;
				/*	case 5:
						printf("chat process started\n");
						break;
					}*/
	        }

                  /*  else(pid>0 && o==conn-1)
                     return 0;*/


	        }

	    }}
/*				while(1) {
					if(o==0) {
						int no;
						n = read(newsockfd[o], &no, sizeof(no));
						no = ntohl(no);

						if(o==0) {
							int fin[no];
							int a=1,b=1,c;
							for(i=0;i<no;i++) {
								c=a+b;
								fin[i]=c;

								a=b;
								b=c;
							}
							n=write(newsockfd[o], fin, sizeof(fin));
						}
					}

					else if(o==1) {
						int f=1;
						int no;
						n = read(newsockfd[o], &no, sizeof(no));
						no = ntohl(no);
						for(i=1;i<=no;i++) {
							f=f*i;
						}
						f = htonl(f);
						n=write(newsockfd[o], &f, sizeof(f));

					}

					else if (o==2) {
						int no,f=0;
						char f[] = "false";
						char t[] = "true";
						n = read(newsockfd[o], &no, sizeof(no));
						no = ntohl(no);
						for(i=no-1;i>0;i--) {
							if(no%i==0) {
								f=1;
								n = write(newsockfd[o], f, sizeof(f));
							}
						}
						if(f==0) {
							n = write(newsockfd[o], t, sizeof(t));
						}

					}

					else if(o==3) {
						int no,f=0;
						char f[] = "false";
						char t[] = "true";
						n = read(newsockfd[o], &no, sizeof(no));
						no = ntohl(no);
						for(i=0;i<=no;i++) {
							if(no==i*i) {
								f=1;
								n = write(newsockfd[o], t, sizeof(t));
							}
						}
						if(f==0) {
							n = write(newsockfd[o], f, sizeof(f));
						}


					}

					else if(o==4) {
						r=srand()%8+1;
						if(r==7) {
							r=r+1;
						}
						r = ntohl(r);
						n = write(newsockfd[o], &r, sizeof(r));

					}

					else if(o==5) {
						FILE *fp;
						fp = fopen("backup.txt","a+");
						pid = fork();

						if(pid<0) {
							printf("error in fork");
							return 0;
						}
						else {
							if(pid==0) {
								for(;;) {
									bzero(buffer, 256);
									n = read(newsockfd[o], buffer1, 256);
									fprintf(fp, "\n%s",buffer1 );
									if(n < 0) {
										error("read" );
									}
									if(strcmp(y,buffer)== 0) {
										printf("server closing ");
										return 0;
									}
									n=write(newsockfd[r], buffer1, sizeof(buffer1));
									if(n < 0) {
										error("write");
									}
								}
							}
							else {

								for(;;) {
									bzero(buffer, 256);
									n = read(newsockfd[r], buffer1, 256);
									fprintf(fp, "\n\t\t\t\t\t\t\t\t%s",buffer1 );
									if(n < 0) {
										error("read" );
									}
									if(strcmp(y,buffer)== 0) {
										printf("server closing ");
										return 0;
									}
									n=write(newsockfd[o], buffer1, sizeof(buffer1));
									if(n < 0) {
										error("write");
									}


								}


							}
						}


					}

					else if(o==6) {


					}




				 }
			}
			else if(pid>0 && o == conn-1) {
				return 0;
			}
		}*/
}
