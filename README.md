## KeyLogger
 version 0.0.2 DEVELOPMENT patch 

this is a Windows based keylogger, it has multiple features, some of which are still under development, once all the core features to make it work in the desired manner are compleated, and the compile script is formalized I will regiser the version 0.1.0 release, 

currently Version 0.0.2 is capable of,

1.) logging keystrokes, 

2.) determining change in forground processes, 

3.) produceing timestamps for change in forground process

4.) logging change in foreground title,

5.) producing time stamps for title change

6.) logging changes to the clipboard

all the information collected by this program is formatted in xml, to view a sample output download the project, and open the "SampleOutput/ClipboardDemo.xml" in your brower, you will need "KeyloggerStyle.xsl" to view the formatting o

# Getting Started
  compileing the keylogger can be done as followed,
  
  **step one, get gcc**
   go to [https://mingw-w64.org/doku.php#mingw-w64] and download gcc, this may install in a few different places, 
  **step two, locate compiler**
      
   locate %systemdrive%\...\MinGW, it will likley be  in "\Program Files\" or "\Program Files (x86)\"
    
   open "Command Prompt" as an Administrator and navigate to the MinGW\bin
    do one of the following
    
   **with window, for now this is the only real option untill updates are compleated**   
   
       c:\ ... \ MinGw\bin>gcc \ ... \ keylogger\keyloggersoftware\main.cpp \ ... \ keylogger\keyloggersoftware\Sanitize_KeylogBuff.cpp -static-libgcc -lstdc++  -o \ ... \ keylogger\keyloggersoftware\keylogger.exe
   
   **without window, once buffered io works with sockets Ill be wrighting a compiler script, but for now you can use**
       
       c:\ ... \ MinGw\bin>gcc \ ... \ keylogger\keyloggersoftware\main.cpp \ ... \ keylogger\keyloggersoftware\Sanitize_KeylogBuff.cpp -static-libgcc -lstdc++  -mwindows -o \ ... \ keylogger\keyloggersoftware\keylogger.exe
   
   
   
   **congradulations**, you can now run this exe and see what it can realy do,
   for now you must copy items off of the buffer into xml documents, this will change as the program inproves, please be patient
   
   **to view a sample output**
  
  1) download a copy of this repository. 
  
  2) unzip the package and navigate to sampleOutput, 
  
  3)  open either of the xml output files in your browser of choice
  [**Note:** the banking info is not real credentials]
             
# Roadmap to Version 0.1.0
 there are multiple features I wish to add before the 0.1.0.
 
 1) generate xml formatted process entries in a buffer that can be encoded and sent to a listening server
 
 2) create a server process capable of retrieving xml process entries
 
 3) color code keylogger data captures and clipboard captures to make it easy to differentiate at a glance.
 
 4) write a sanitizer function for currently unsanitized xml objects. (process title, clipboard, etc)
 
# Developed and tested on the windows 10.
# you can contact the comunity on slack

https://join.slack.com/t/qu3b411keylogger/shared_invite/enQtNDkzMjg4NDQ5MjQ4LWU2N2IwOTY5MjU4YzdjODc0MzFhOGI4YWUxYmU4NGFlZTYyMGNlYTUyYmQ5OTVkOGU2NmU5NzZmMGJmMDA1MjI

# Contact a core team member via slack to report abuse of the code of coduct
  members:
     Qu3b411

# TODO
  1.) write a sanitizer function for xml from the clipboard 
  
  2.) write a sanitizer function for process titles.
  
  3.) restructure software for better output flow
  
  4.) set up buffered socket communication
  
# please review it and let me know what Isues arise. 

# Known Issue
 1.) output is to the console and has not been changed as of yet.
 
 2.) special characters in process's will create malformated xml output that cannot be styled
 
 3.) an end </KeyLoggerMetaData> must be manually added to xml coppied off of the console as of this moment
 
 4.) special characters in clipbord may result in invalid xml output, this means xsl styleing cannot take place.
