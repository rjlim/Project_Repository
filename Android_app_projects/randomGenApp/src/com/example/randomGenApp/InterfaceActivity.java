package com.example.randomGenApp;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class InterfaceActivity extends Activity
{

    Button genButton;
    TextView mytext;
    TextView randResult;
    StringSlitter splitX;
    RandomStringGen ranX;


    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        genButton = (Button) findViewById(R.id.btn1);
        mytext  = (TextView) findViewById(R.id.etxt);
        randResult = (TextView) findViewById(R.id.result);

        genButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                randResult.setText("");
                splitX = new StringSlitter();

                splitX.StringSplitter(String.valueOf(mytext.getText()),","); // split string

               int len = splitX.getStrSplitLength(); //get the array length of splitted string

                String[] strArray = splitX.getStrSplitVal();  // get the array containing the splitted string

                ranX = new RandomStringGen();

                for (int i = 0; i < len ; i++)
                {
                    randResult.append(ranX.generateChar(strArray));
                }

            }
        });
    }

}
