package com.example.randomGenApp;

/**
 * Created by pc01 on 22/06/2016.
 */
public class StringSlitter {

    // cointain the strings splitted by a delimiter
    private String[] splittedString;

    public void StringSplitter(String str, String delim) {

        // check if the sequence of character contain the delimiter
        if (str.contains(delim)) {

            // split the string sequence using the given delimiter
            splittedString = str.split(delim);

        } else {
            splittedString = str.split(" "); // return a null value if condition is not met
        }
    }

    // return the length of the array
    public int getStrSplitLength() {
        return splittedString.length;
    }

    // get the value of the splitted string sequence;
    public String[] getStrSplitVal(){
        return splittedString;
    }
}
