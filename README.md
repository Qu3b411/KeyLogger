# KeyLogger Version 0.1.0
 this is a Windows based keylogger, it has multiple usefull features! as described below,


currently Version 0.1.0 is capable of:

**1.) logging keystrokes:** includeing several of the function keys!

**2.) determining change in forground processes:** so you can see the workflow of the targets computer!

**3.) produceing timestamps for change in forground process:** so you can better corelate events!

**4.) logging change in foreground title:** even the inbetween steps that you wouldnt normaly see!

**5.) producing time stamps for title change:** to better the corelation of events!

**6.) logging changes to the clipboard:** to get all sensitive data you would otherwise not see!

**7.) color codeing output** so you can know the flow of events at a glance 

**8.) supports  socket level communication.**
		
		[NOTE:] Data is sent back to the server after every process, 
			if the socket for some reason fails then this server
			will store all logs in memory and attempts to 
			esstablish a connection to send data back to the server.

**9.) client side xml formatting.**
	
		[NOTE:] after a disconnect the server currently prints out the 
		ending xml tag so you can view the log in your web browser of 
		choice.


**Sample Output:** can be viewed to get a feal for what the output should look like, detailed instructions can be found by going to the **Getting Started/ to view sample output** section of the **README** 

# Getting Started
  **There are two ways to get this keylogger up and running**
  	
 **1.) Manually compile the script:** This is the encouraged manner of getting ahold of this script
 
 **2.) download the release version** This might not support the latest and greatest, but it will give you a good idea as to what it can do!!!
 
 **Reasons to manually compile** 
 	
1.) version releases will be done only after all items in the **Roadmap to Version #.#.#** section of the **README** have been compleated. 

2.) Version releases will be compiled on the loop back address, this is by design, a long term goal of this project is to have it run completely in memory, compileing with a config file would defeate this goal!

**Reasons to download version release**

if your only looking to preview what this keylogger version can do then downloading the compiled version will give you a preocompiled binary with the server and both client configurations.
 
 
  
  **compileing the keylogger can be done as follows**
  
  **step one, get gcc**
   go to [https://mingw-w64.org/doku.php#mingw-w64] and download gcc, 
   
  **step two, locate compiler**
      
   locate %systemdrive%\...\MinGW, it will likley be  in "\Program Files\" or "\Program Files (x86)\"
   
   **step three, adding MinGw\bin to system path**
   
   	hit winkey+x and select "Powershell (admin)", then type the following command useing the path you found to MinGw
		
		[Environment]::SetEnvironmentVariable("Path",$env:Path+";C:\ . . . \MinGw\bin\", [System.EnvironmentVariableTarget]::Machine)
		
 **DO NOT:** forget the ";" before the start of your path. this is an important delimiter.
 
 restart your computer and gcc will be a part of your system path
 
 **Step four, Configuration**
 
 If their is a server ip that you want the client to connect to then you must change the ip address in the **KeyLogger\KeyLoggerSoftware\Client** folder, open the **KeyLogger_clientCom.h** file and find 
 
 	#define IP_ADDRESS "127.0.0.1"

Edit it to the desired external ip address that your server is connecting to. it should appear as follows

	#define IP_ADDRESS "#.#.#.#"
	
**Save your edit and continue on to compile**
 
 **Step five: Compileing**
 
Open Command prompt and navigate to the root directory of this program and find the batch script named "compiler.bat", if your prompted expand the details and click "Run Anyway" run this script and answer the provided questions
by the end of the script you should have the exe's, 
  
   
   **congradulations**, you can now run this exe and see what it can realy do!

**Compiled versions** can be run as soon as they are downloaded.
   
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
  
  step 5) log data for some time,
  
  step 6) kill the keylogger process and the keyloggerServer will seld terminate shortly their after, the easiest way to kill the process is through the details tab on the task manager.
  
 if you see a page that looks as the sample did, then **Congradulations!** you have successfully created your first keylog capture!
 

# Roadmap to Version 0.2.0

| Version Spec |      Client Side Requirements        | done |                   Server Side Requirements                | done |
| ------------ |:------------------------------------:|:----:|:---------------------------------------------------------:| ----:|
|    0.1.1     | Limit unsent buffer size before exit |  NO  |     set file names to connecting ip address_TimeStamp     |  YES   |
|              |    				      |  N/A | client termination loops back to start new client session |  YES  |
|	       |                                      |  N/A | start the javac implimintations for the GUI, GUI will not be supported at this time | NO |
|    0.1.2     |  build in click screen captures      |  NO  |                   build multi-client support              |  Yes will need some work |
|              |                                      |      |                                                           |      |
|    0.1.3     | hook enum process and remove pid     | N/A  |                    build a minimal javac GUI              |  NO  |
|              |                                      | N/A  |           release compiler script that supports GUI       |  NO  |
|    0.1.4     |                                      | N/A  |                         polish off the GUI                |  NO  |
|    0.2.0     | design a  roadmap to  support client side encryption | NO | design a  roadmap to  support server side encryption | NO |

 **This is a rough outline** I am hopeing to have some support from the community so that this goes alot faster!, it may take some time tp get to the 0.2.0, but after that we can get into thje real fun of actually implementing these designs, please by all means open up a pull and start codeing, 

# you can contact the comunity on slack

https://join.slack.com/t/qu3b411keylogger/shared_invite/enQtNDkzMjg4NDQ5MjQ4LWU2N2IwOTY5MjU4YzdjODc0MzFhOGI4YWUxYmU4NGFlZTYyMGNlYTUyYmQ5OTVkOGU2NmU5NzZmMGJmMDA1MjI

# Contact a core team member via slack to report abuse of the code of coduct
  members:
     Qu3b411

# TODO
  
  1.) document all functions internal flow and external **not done** new documentation will be added to describe functions.
  
  2.) find the bug aflicting the server which limits client connections and write a patch!!!
  
# please review it and let me know what Isues arise. 
#### Windows has blocked the binary release as sevear malware, this is a demo sample of a keylogger, and a form of spyware, this program is intended for educational purposes, if your still interested in testing the binarys it may be prudent to disable windows defender on your testing virtual machine, please do not use this on your every day machine
