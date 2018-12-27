#ifndef SERVERINTERPROCESS_H_INCLUDED
#define SERVERINTERPROCESS_H_INCLUDED

typedef struct socketlist
{
    SOCKET* target;
    struct socketlist* nextTarget;
    char* name;
    long IPADDR;
    HANDLE FileDescriptor;
}SOCKETLIST;


#endif // SERVERINTERPROCESS_H_INCLUDED
