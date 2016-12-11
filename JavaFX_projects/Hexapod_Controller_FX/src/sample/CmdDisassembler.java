package sample;

/**
 * Created by pc01 on 21/03/2016.
 */
public class CmdDisassembler {


    private String[] CommandStreamArray, tempArray, CommandVal, CommandAddr;

    private int index = 0;

    // get the command string
    public void onCommandStream(String CommandStream, String delimA, String delimB) {

        // check if the command stream contains "-"
        if (CommandStream.contains(delimA)) {
            // split the string that contains the required delimiter
            CommandStreamArray = CommandStream.split(delimA);
            // create a string array that will store the values of the commands
            CommandAddr = new String[CommandStreamArray.length + 1];
            CommandVal = new String[CommandStreamArray.length + 1];

            // loop that separate the command and command address and store
            // the separated values in two arrays one contains the command address
            // another contain the command value
            for (int x = 0; x < CommandStreamArray.length; x++) {
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
        // do nothing if the string does not contain the delimiter
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
}
