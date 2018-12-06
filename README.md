## KeyLogger
 version 0.0.2 DEVELOPMENT patch 

this is a Windows based keylogger, it has multiple usefull features!, **This is still in alpha testing and dev** some of these features  are still under active development and I will be commiting features ASAP, once all the core features to make This keylogger work asdesired, and the compile script is formalized I will regiser the version 0.1.0 release, that will be the first stable point. but this version does work to some extent.

currently Version 0.0.2 is capable of,

1.) logging keystrokes, 

2.) determining change in forground processes, 

3.) produceing timestamps for change in forground process

4.) logging change in foreground title,

5.) producing time stamps for title change

6.) logging changes to the clipboard

all the information collected by this program is formatted in xml, to view a sample output download the project, and open the "SampleOutput/ClipboardDemo.xml" in your brower, you will need "KeyloggerStyle.xsl", a detailed instruxtion list is under **Getting Startes / to view sample output**, make sure to check in regularly to see whats new!

# Getting Started
  compileing the keylogger can be done as followed,
  
  **step one, get gcc**
   go to [https://mingw-w64.org/doku.php#mingw-w64] and download gcc, this may install in a few different places, 
  **step two, locate compiler**
      
   locate %systemdrive%\...\MinGW, it will likley be  in "\Program Files\" or "\Program Files (x86)\"
    
   open "Command Prompt" as an Administrator and navigate to the MinGW\bin
    do one of the following
    
   **with window**   
   
       c:\ ... \ MinGw\bin>g++ \ ... \ keylogger\keyloggersoftware\main.cpp \ ... \ keylogger\keyloggersoftware\Sanitize_KeylogBuff.cpp  -o \ ... \ keylogger\keyloggersoftware\keylogger.exe
   
   **without window**
       
       c:\ ... \ MinGw\bin>g++ \ ... \ keylogger\keyloggersoftware\main.cpp \ ... \ keylogger\keyloggersoftware\Sanitize_KeylogBuff.cpp   -mwindows -o \ ... \ keylogger\keyloggersoftware\keylogger.exe
   
   
   
   **congradulations**, you can now run this exe and see what it can realy do,
   for now you must copy items off of the buffer into xml documents, this will change as the program inproves, please be patient
   
   **to view a sample output**
  
  1) download a copy of this repository. 
  
  2) unzip the package and navigate to sampleOutput, 
  
  3)  open either of the xml output files in your browser of choice
  [**Note:** the banking info is not real credentials]
  
  **Running and Generating xml's**
  
  step 1) start command prompt as administrator.
  
  step 2) navigate to your newley generate exe files, if done correctly they should be in keylogger software.
  
  step 3) type 
   
           keyloggerHidden.exe >> ./logged/log#.xml
         
   or
   
           keylogger.exe >> ./logged/log#.xml
           
  "#" is a number
  
  step 4) log data for some time,
  
  step 5) kill the keylogger process, the easiest way to do so is through the details tab on the task manager
  
  step 6) **for hidden version**
  
  it is possible that the last process was not totaly logged, in this event you must manualy correct the xml untill I can write the socket, **this is still in alpha testing and development, problems exist!** if you scroll to the bottom of the file it should look as follows 
    
            	</logged>
		</title>
	</Process>
            
  but it is possible to see something like 
  
            			</logged>
		</title>
	</Process>
	<Process>
		<ProcessHandle>0x2033392</ProcessHandle>
		<ProcessID>0x8088</ProcessID>
		<title>
			<ProcessTitle><![CDATA[Task Manager]]></ProcessTitl
  
  delete everything after the
  		
		</Process>
 tag so that your xml file looks as it should.
  
  step 7) at the bottom of the xml file after the 
                   
		  </process> 
tag, hit enter and add
  
                 </KeyLoggerMetaData>
	
at this point the end of your xml file should appear as follows

					</logged>
		          </title>
	          </Process>
     </KeyLoggerMetaData>
  
  save the xml file and view the fruites of your labor.
  
 if you see a page that looks as the sample did when you opened it then **Congradulations!** you have successfully created your first keylog capture!
 

# Roadmap to Version 0.1.0
 there are multiple features I wish to add before the 0.1.0.
 
 1) generate xml formatted process entries in a buffer that can be encoded and sent to a listening server
 	
	**compleated part 1** this process now generate xml in a memory buffer, and is timed to send loggs after each process is captured.
	**part 2** incompleate, encodeing and sending the data still has not been compleated.
	
 2) create a server process capable of retrieving xml process entries
 
 3) color code keylogger data captures and clipboard captures to make it easy to differentiate at a glance
 
 	**COMPLEATE** the xsl stylesheet will color code all captured data makeing it easy to build comprehensive logs
 
 4) write a sanitizer function for currently unsanitized xml objects. (process title, clipboard, etc)
  
# Developed and tested on the windows 10.
# you can contact the comunity on slack

https://join.slack.com/t/qu3b411keylogger/shared_invite/enQtNDkzMjg4NDQ5MjQ4LWU2N2IwOTY5MjU4YzdjODc0MzFhOGI4YWUxYmU4NGFlZTYyMGNlYTUyYmQ5OTVkOGU2NmU5NzZmMGJmMDA1MjI

# Contact a core team member via slack to report abuse of the code of coduct
  members:
     Qu3b411

# TODO
  1.) debug sanitizer and xml output -- done besides end tag which still must be added
  
  2.) write a sanitizer function for process titles. **done** still need allot of testing, feedback would be amazing!!!
  
  3.) restructure software for better output flow **done**
  
  4.) set up buffered socket communication
  
# please review it and let me know what Isues arise. 

# Known Issue
 1.) output is to the console has not been changed as of yet.
 
 2.) an end </KeyLoggerMetaData> must be manually added to xml coppied off of the console as of this moment
 

