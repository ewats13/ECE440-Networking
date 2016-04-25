/* client.c	*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int  main(int argc, char* argv[])
{
    char MESSAGE[80];
    int i;
    int key =4;
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;

    if(argc < 3)
      {
	printf("\nUsage: client host-name host-port\n");
	return 0;
      }
    else
      {
	strcpy(strHostName,argv[1]);
	nHostPort=atoi(argv[2]);
      }

    printf("\nMaking a socket");
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nConnecting to %s on port %d",strHostName,nHostPort);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address))
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }

    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
  //copy incoming encrypted message into MESSAGE
    strcpy(MESSAGE, pBuffer);

// DECRYPT MESSAGE

    for (i=0; MESSAGE[i]!= 0; i++){

      if (MESSAGE[i]>= 69 && MESSAGE[i] < 91){

        MESSAGE[i]-= key;
      }
      else if(MESSAGE[i] == 32){
        continue;
      }
      else if(MESSAGE[i] == 65){
        MESSAGE[i] = 87;
      }
      else if(MESSAGE[i] == 66){
        MESSAGE[i] = 88;
      }
      else if(MESSAGE[i] == 67){
        MESSAGE[i] = 89;
      }
      else if(MESSAGE[i] == 68){
        MESSAGE[i] = 90;
      }
      else{
        continue;
      }
    }

    printf("\nReceived \"%s\" from server\n",pBuffer);
    /* write what we received back to the server */
    write(hSocket,pBuffer,nReadAmount);
    printf("\nWriting \"%s\" to server",pBuffer);

    printf("\nDecrypted Messaged: \"%s\"\n", MESSAGE);

    printf("\nClosing socket\n");
    /* close socket */
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
}
