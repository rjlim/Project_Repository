package randGenScr;


import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;

import java.net.URL;
import java.util.ResourceBundle;

public class Ctrl_UI implements Initializable {

    @FXML
    private Button btn_getValue = new Button();
    @FXML
    private TextArea tA_StringSeed = new TextArea();
    @FXML
    private TextField tf_RandVal = new TextField();
    @FXML
    private TextField tf_spacer = new TextField();

    // call the string splitter class
    private SplitStringContent getSplit = new SplitStringContent();
    // call the random string generator
    private GenRandString randyString = new GenRandString();

    // storage array for the set of strings to be shown
    private String[] myStringContent;

    private String tAcontents, sSpacer;

    @Override
    public void initialize(URL location, ResourceBundle resources) {

        tA_StringSeed.setText("1,2,3,4,5,6,7,8,9");

        btn_getValue.setOnAction(event -> {

            // check if the text area is empty
            if (!tA_StringSeed.getText().trim().isEmpty()) {

                // clear the contents of the textfield
                tf_RandVal.clear();
                // get the contents of the text area
                tAcontents = tA_StringSeed.getText();
                //spit the contents from the text area and store it in a array

                 sSpacer = tf_spacer.getText();

                getSplit.StringSplitter(tAcontents, sSpacer);
                // get the stored value and display it to the textfield
                myStringContent = getSplit.getStrSplitVal();
                for (int x = 0; x < 10; x++) {
                    // double for loop generate the spacer and number of characters
                    tf_RandVal.appendText(randyString.generateChar(myStringContent));
                }
            } else {
                tf_RandVal.appendText(" ");
            }
        });

        // limit number of character to 1 only
        tf_spacer.textProperty().addListener( (observable,oldValue,newValue)-> {
            if(newValue.length() > 1) tf_spacer.setText(oldValue);
        });

    }

}
