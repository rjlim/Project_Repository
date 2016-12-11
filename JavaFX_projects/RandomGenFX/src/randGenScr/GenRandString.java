package randGenScr;
import java.util.Random;
/**
 This code will genarate a random string

 from a given string array

 */
public class GenRandString {


    // generate a random string with only one character
    String generateChar(String[] stringSeedArray) {
        // integer for the random index
        int getIndex;
        // string container for one value of char
        String getCharValue;
        // generate a random index from 1 to the max length
        // of the given array

        // get the value of the array with a random index
        try {
            getIndex = randomIntVal(0, stringSeedArray.length);
            getCharValue = stringSeedArray[getIndex];
            return getCharValue;
        } catch (NullPointerException ne) {
            return null;
        }
        // return the one char string.
    }


    // generate a random integer from a given max and min value
    private int randomIntVal(int Rmin, int Rmax) {
        // create a random object
        Random randomVal = new Random();
        // get a random int from the random generator
        int randomGenVal = randomVal.nextInt(Rmax - Rmin) + Rmin;
        // return the value
        return randomGenVal;
    }
}
