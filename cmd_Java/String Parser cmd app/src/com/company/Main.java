package com.company;

import java.util.Scanner;

public class Main {


    public static void main(String[] args) {


        float val1, val2;

    CommandParse CommandSplitter = new CommandParse();

        Scanner StringScan = new Scanner(System.in);

        System.out.print("Enter something : ");
        // input string
        String InputString = StringScan.next();

        // split incoming string that contains "-"
        CommandSplitter.onCommandStream(InputString,"-","/");
        //CommandSplitter.getCommandAddress();
        //CommandSplitter.getCommandValue();

        if (CommandSplitter.CommandAddressSize() == CommandSplitter.CommandValueSize())
        {
            for (int Cntr = 0 ; Cntr < CommandSplitter.CommandAddressSize() ; Cntr++)
            {

                if ((CommandSplitter.CommandAddressList()[Cntr] == null) &&
                        (CommandSplitter.CommandValueList()[Cntr] == null))
                {
                    val1 = 0;
                    val2 = 0;

                    System.out.println( "Command Address: "+ Float.toString(val1)+
                            " Command Value: " + Float.toString(val2));
                }
                else {
                    val1 = Float.parseFloat(CommandSplitter.CommandAddressList()[Cntr]);
                    val2 = Float.parseFloat(CommandSplitter.CommandValueList()[Cntr]);

                    System.out.println( "Command Address: "+ Float.toString(val1)+
                            " Command Value: " + Float.toString(val2));
                }
            }
        }


    }
}
