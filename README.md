## KeyLogger
 version 0.0.3-beta
 
 this is a Windows based keylogger, it has multiple usefull features! as described below

currently Version 0.0.3 is capable of,

1.) logging keystrokes, 

2.) determining change in forground processes, 

3.) produceing timestamps for change in forground process

4.) logging change in foreground title,

5.) producing time stamps for title change

6.) logging changes to the clipboardo

7.) color codeing output so you can know the flow of events at a glance 

all the information collected by this program is formatted in xml, to view a sample output download the project, and open the "SampleOutput/ClipboardDemo.xml" in your brower, you will need "KeyloggerStyle.xsl", a detailed instruxtion list is under **Getting Startes / to view sample output**, make sure to check in regularly to see whats new!

# Getting Started
  compileing the keylogger can be done as followed,
  
  **step one, get gcc**
   go to [https://mingw-w64.org/doku.php#mingw-w64] and download gcc, this may install in a few different places, 
   
  **step two, locate compiler**
      
   locate %systemdrive%\...\MinGW, it will likley be  in "\Program Files\" or "\Program Files (x86)\"
   
   **step three, adding MinGw\bin to system path**
   
   	hit winkey+x and select "Powershell (admin)", then type the following command useing the path you found to MinGw
		
		[Enviroment]::SetEnviromentVariable("Path",$env:Path+";C:\ . . . \MinGw\bin\", [System.EnviromentVariableTarget]::Machine)
		
 **DO NOT:** forget the ";" before the start of your path.
 
 restart your computer and gcc will be a part of your system path
 **Step 4, Compileing the client **
 
   
**Step 5, Compileing server**
	
in the root directory of the keylogger their is a batch script named "compiler.bat", run this script and answer the provided questions
by the end of the script you should have the exe's
  
   
   **congradulations**, you can now run this exe and see what it can realy do!
   
   **to view a sample output**
  
  1) download a copy of this repository. 
  
  2) unzip the package and navigate to sampleOutput, 
  
  3)  open either of the xml output files in your browser of choice
  
  [**Note:** the banking info is not real credentials]
  
  **Running and Generating xml's**
  
  step 1) start command prompt as administrator.
  
  step 2) navigate to your newley generate exe files, if done correctly they should be in keylogger software.
  
  step 3) goto your keylogger client and do the following
   
          c:\ . . . \client> keyloggerHidden.exe 
         
   or
   
           c:\ . . . \client>keylogger.exe 
          
  step 4) then navigate to your server and do the following
  	  
	  c:\ . . . \server> keyloggerServer.exe >> log#.xml
	  
  in this case # represents a number
  
  step 4) log data for some time,
  
  step 5) kill the keylogger process and keyloggerServer process, the easiest way to do so is through the details tab on the task manager
  
 if you see a page that looks as the sample did when you opened it then **Congradulations!** you have successfully created your first keylog capture!
 

# Roadmap to Version 0.1.0
 there are multiple features I wish to add before the 0.1.0.
 
 1) generate xml formatted process entries in a buffer that can be encoded and sent to a listening server
 	
	**compleated part 1** this process now generate xml in a memory buffer, and is timed to send loggs after each process is captured.
	**partialy complete part 2** still need to write compile script
	
 2) create a server process capable of retrieving xml process entries
 	**PARTIALY COMPLETE** still need to suport multiple users 
 
 3) color code keylogger data captures and clipboard captures to make it easy to differentiate at a glance
 
 	**COMPLEATE** the xsl stylesheet will color code all captured data makeing it easy to build comprehensive logs
 
 4) write a sanitizer function for currently unsanitized xml objects. (process title, clipboard, etc)
 
 	**COMPLEATE** this program now sanitizes all incomeing data, 
  
# Developed and tested on the windows 10.
# you can contact the comunity on slack

https://join.slack.com/t/qu3b411keylogger/shared_invite/enQtNDkzMjg4NDQ5MjQ4LWU2N2IwOTY5MjU4YzdjODc0MzFhOGI4YWUxYmU4NGFlZTYyMGNlYTUyYmQ5OTVkOGU2NmU5NzZmMGJmMDA1MjI

# Contact a core team member via slack to report abuse of the code of coduct
  members:
     Qu3b411

# TODO
  1.) debug sanitizer and xml output -- **done** besides end tag which still must be added
  
  2.) write a sanitizer function for process titles. **done** still need allot of testing, feedback would be amazing!!!
  
  3.) restructure software for better output flow **done**
  
  4.) set up buffered socket communication **done** still need alot of testing, 
  
  5.) document all functions internal flow and external **not done** new documentation will be added to describe functions,
  
  6.) create compiler script to automate compilation options
  
# please review it and let me know what Isues arise. 

# Known Issue
  1.) an end </KeyLoggerMetaData> must be manually added to xml coppied off of the console as of this moment
 

