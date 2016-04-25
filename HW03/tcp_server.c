/* server.c */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
//#define MESSAGE             "This is the message I'm sending back and forth"
#define QUEUE_SIZE          5

int main(int argc, char* argv[])
{
    int i,j,k,l;
    int key = 4;
    char MESSAGE[80];             // limiting the length of the message
    char ENCRYPT_MSG[80];         // copy for encryption
    int hSocket, hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    char pBuffer[BUFFER_SIZE];
    int nHostPort;

    if(argc < 2){
	     printf("\nUsage: server host-port\n");
	      return 0;
    }
    else {
	     nHostPort=atoi(argv[1]);
    }

    printf("\nStarting server");

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);

    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }
    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nBinding to port %d\n",nHostPort);

    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address))
                        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
 /*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as: fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );

        printf("Server:\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );

    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR){
        printf("\nCould not listen\n");
        return 0;
    }

    for(;;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);

	       printf("\nGot a connection\n");

         printf("* NOTICE: All messages will be converted into uppercase.\n");
         printf("Messages should not contain any special characters or numbers. *\n");
         printf("Please input your message: ");

// limit input character set to only use upper case letters, which
// are represented as 65 to 90 in the ASCII format.
// messageswill not have any special characters or numbers
        fgets(MESSAGE, 80, stdin);            //obtain message
        int LEN = strlen(MESSAGE);            // LEN = length of message

//CONVERT TO UPPER CASE LETTERS
        for (i=0; MESSAGE[i]!='\n'; i++){
          MESSAGE[i] = toupper(MESSAGE[i]);
          }

// DEBUGGING - print string length
//      printf("string length: %d\n",LEN);      //print string length

// fgets stores the final newline (\n) character in you buffer, so you
// must remove this if your code is not expecting it.
        if (LEN > 0 && MESSAGE[LEN-1] == '\n'){
          MESSAGE[LEN-1] = '\0';
        }

//DEBUGGING - print each character of the message
//  for (j=0; j< LEN; j++){
//    printf("Characters of MESSAGE are: %c\n", MESSAGE[j]);
//  }

//DEBUGGING - print the decimal value of each character of the message
//  for (j=0; j< LEN; j++){
//    printf("decimal form of MESSAGE is: %i\n", MESSAGE[j]);
//  }

        printf("\n Original Message: \"%s\"", MESSAGE);
// Copy contents of the original message into a new array (ENCRYPT_MSG)
        strcpy(ENCRYPT_MSG, MESSAGE);

// ENCRYPTION
        for (l = 0; ENCRYPT_MSG[l]!=0; l++){

        if (ENCRYPT_MSG[l]>=65 && ENCRYPT_MSG[l] < 86){

          ENCRYPT_MSG[l] += key;    //encrypt with key
        }
        else if(ENCRYPT_MSG[l] == 32){  // if 'space' leave it alone
          continue;
        }
      // need to wrap for w, x, y, and z
        else if(ENCRYPT_MSG[l] == 87){    // "W"
          ENCRYPT_MSG[l] = 65;            // wrap to "A"
        }
        else if(ENCRYPT_MSG[l] == 88){    //  "X"
          ENCRYPT_MSG[l] = 66;            // wrap to "B"
        }
        else if(ENCRYPT_MSG[l] == 89){    //   "Y"
          ENCRYPT_MSG[l] = 67;            // wrap to "C"
        }
        else if(ENCRYPT_MSG[l] == 90){    //   "Z"
          ENCRYPT_MSG[l] = 68;            //  wrap to "D"
        }
      // let other values, such as newline or line feed, go through
        else{
          continue;
        }
      }

        printf("\n Encrypted Message: \"%s\"\n", ENCRYPT_MSG);

        strcpy(pBuffer,ENCRYPT_MSG);

	      printf("\nSending \"%s\" to client",pBuffer);
        /* number returned by read() and write() is the number of bytes
        ** read or written, with -1 being that an error occured
        ** write what we received back to the server */
        write(hSocket,pBuffer,strlen(pBuffer)+1);
        /* read from socket into buffer */
        read(hSocket,pBuffer,BUFFER_SIZE);

        if(strcmp(pBuffer,ENCRYPT_MSG) == 0)
            printf("\nThe messages match");
        else
            printf("\nSomething was changed in the message");

	          printf("\nClosing the socket");
        /* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
         printf("\nCould not close socket\n");
         return 0;
        }
    }
}
