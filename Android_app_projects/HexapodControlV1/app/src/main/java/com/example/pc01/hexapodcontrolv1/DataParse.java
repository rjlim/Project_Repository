package com.example.pc01.hexapodcontrolv1;

/**
 * Created by pc01 on 5/09/2016.
 */
public class DataParse {

    private String[] CommandStreamArray, tempArray, CommandVal, CommandAddr;

    private boolean noDelimitA, noDelimitB;

    private int index = 0, x;

    float parseAddr, parseValue;

    // get the command string
    public void onDataParse(String CommandStream, String delimA, String delimB) {

        // check if the command stream contains "-" and "/"
        if (CommandStream.contains(delimA) && CommandStream.contains(delimA)) {
            noDelimitB = true;
            noDelimitA = true;
            // split the string that contains the required delimiter
            CommandStreamArray = CommandStream.split(delimA);
            // create a string array that will store the values of the commands
            CommandAddr = new String[CommandStreamArray.length];
            CommandVal = new String[CommandStreamArray.length];
            // loop that separate the command and command address and store
            // the separated values in two arrays one contains the command address
            // another contain the command value
            for (x = 0; x < CommandStreamArray.length; x++) {
                // display to terminal for debugging
                System.out.println(CommandStreamArray[x]);
                // check it the command contains the required delimiter
                if (CommandStreamArray[x].contains(delimB)) {
                    // split the command into two values
                    tempArray = CommandStreamArray[x].split(delimB);
                    // print to terminal for debugging
                    //System.out.println("addr:" + tempArray[index]);

                    // get all the command address to the temorary array
                    CommandAddr[x] = tempArray[index];

                    // print to terminal for debugging
                    //System.out.println("cmd :" + tempArray[index+1]);

                    // get all the command address to the temorary array
                    CommandVal[x] = tempArray[index + 1];
                }
            }
        }
        else
        {

        }
/*

        if (CommandAddressSize() == CommandValueSize()) {

            // loop through the array and do the specified tasks
            for (int Cntr = 0; Cntr < CommandAddressSize(); Cntr++) {

                // if array is empty
                if ((CommandAddressList()[Cntr] == null) &&
                        (CommandValueList()[Cntr] == null)) {

                    parseAddr = 0;
                    parseValue = 0;

                } else {
                    // if array contains a data transfer it to parseAddr and parseValue

                    // val1 contains the address
                    parseAddr = Float.parseFloat(CommandAddressList()[Cntr]);
                    // val2 contains the value of the address
                    parseValue = Float.parseFloat(CommandValueList()[Cntr]);

                }
            }
        }
*/

    }

    public float getAddressInFloat() {
        return parseAddr;
    }

    public float getValueInFloat() {
        return parseValue;
    }

    public int getAddressInt() {
        return (int) parseAddr;
    }

    public int getValueInt() {
        return (int) parseAddr;
    }


    // method for display all the commands address only
    public void getCommandAddress() {

        for (int x = 0; x < CommandAddr.length; x++) {
            System.out.println("cmd:" + CommandAddr[x]);
        }
    }

    // method for display all the commands values only
    public void getCommandValue() {
        for (int x = 0; x < CommandVal.length; x++) {
            System.out.println("addr:" + CommandVal[x]);
        }
    }

    //method to pass the array to another class
    public String[] CommandAddressList() {

        return CommandAddr;
    }

    //method to pass the array to another class
    public String[] CommandValueList() {

        return CommandVal;
    }

    // get the array size of the command address
    public int CommandAddressSize() {
        return CommandAddr.length;
    }

    // get the array size of the command address
    public int CommandValueSize() {
        return CommandVal.length;
    }

    public boolean containsDelimA() {
        return noDelimitA;
    }

    public boolean containsDelimB() {
        return noDelimitB;
    }
}
