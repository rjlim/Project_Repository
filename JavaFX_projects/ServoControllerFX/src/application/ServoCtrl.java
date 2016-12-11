package application;

import java.net.URL;
import java.text.NumberFormat;
import java.util.ResourceBundle;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleButton;
import jssc.*;

public class ServoCtrl implements Initializable {

    @FXML
    private Button IncrBtn = new Button();
    @FXML
    private Button DecrBtn = new Button();
    @FXML
    private ToggleButton toggleConnectSerial = new ToggleButton();
    @FXML
    private ToggleButton lightSwitcH = new ToggleButton();
    @FXML
    private Slider AngleSlider = new Slider(0, 180, 90);
    @FXML
    private TextField tfAngle = new TextField();
    @FXML
    private TextField tfSteps = new TextField();
    @FXML
    private TextField tfRecievedData = new TextField();

    SerialPort ServoSerialPort;

    private int tempstore = 0;


    public void disableControls () {
        AngleSlider.setDisable(true);
        lightSwitcH.setDisable(true);
        IncrBtn.setDisable(true);
        DecrBtn.setDisable(true);
        tfAngle.setDisable(true);
        tfSteps.setDisable(true);
        tfRecievedData.setDisable(true);
    }

    public void EnableControls () {
        AngleSlider.setDisable(false);
        lightSwitcH.setDisable(false);
        IncrBtn.setDisable(false);
        DecrBtn.setDisable(false);
        tfAngle.setDisable(false);
        tfSteps.setDisable(false);
        tfRecievedData.setDisable(false);
    }

    @Override
    public void initialize (URL location, ResourceBundle resources) {

        // add property to text field - it means that angle can be change by input from textield or
        // by moving the slider
        tfAngle.textProperty().bindBidirectional(AngleSlider.valueProperty(), NumberFormat.getIntegerInstance());

        //disable all port when starting the application
        disableControls();

        // slider listener
        AngleSlider.valueProperty().addListener(new ChangeListener<Number>() {

            @Override
            public void changed (ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                // TODO Auto-generated method stub
                try {
                    ServoSerialPort.writeString("1/" + ((int) AngleSlider.getValue()) + "-");
                } catch (SerialPortException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        });
    }

	/*--------- increase or decrease slider value using buttons---------------------------------*/

    public void IncrBtnPress (ActionEvent btnIncr) {

        // get
        tempstore = Integer.parseInt(tfSteps.getText());
        AngleSlider.setValue(Integer.parseInt(tfAngle.getText()) + tempstore);
        tfAngle.setText(Integer.toString((int) AngleSlider.getValue()));
    }

    public void DecrBtnPress (ActionEvent btnDecr) {
        tempstore = Integer.parseInt(tfSteps.getText());
        AngleSlider.setValue(Integer.parseInt(tfAngle.getText()) - tempstore);
        tfAngle.setText(Integer.toString((int) AngleSlider.getValue()));

    }

	/*-----------toggle Switch on light-----------------------------------*/

    public void lightSwitch (ActionEvent eve) throws InterruptedException {
        try {
            if (lightSwitcH.isSelected() == true) {
                lightSwitcH.setText("OFF");

				/*
                 * send the following string commands 3 = led number 1 = On, 0 =
				 * Off / = seperator - = wait for next command
				 * 
				 */
                // turn on led
                ServoSerialPort.writeString("3/1-");
                Thread.sleep(5);
            } else {
                // turn off led
                lightSwitcH.setText("ON");
                ServoSerialPort.writeString("3/0-");
            }
        } catch (SerialPortException e) {
            e.printStackTrace();
        }
    }

    /*-------------------- connect or disconnect port-------------------------*/
    public void toggleConnect (ActionEvent tgle) {

        // check if the toggle button is pressed
        if (toggleConnectSerial.isSelected() == true) {

            // enable control when serial port is connected
            EnableControls();

            // set the port name NOTE: port name may vary depending on your OS i.e. windows COM8 linux dev/ttySUSB1
            ServoSerialPort = new SerialPort("COM8");
            try {
                // set the toggle button text to disconnect
                toggleConnectSerial.setText("Disconnect");
                // open the serial port
                ServoSerialPort.openPort();
                // set the port parameters change baud rate according to your application
                // arduino default baud rate is 9600 simply change the BAUDRATE_115200 to BAUDRATE_9600
                ServoSerialPort.setParams(SerialPort.BAUDRATE_115200,
                        SerialPort.DATABITS_8,
                        SerialPort.STOPBITS_1,
                        SerialPort.PARITY_NONE);


                // listen for any command or value send by serial port
                ServoSerialPort.addEventListener(new SerialPortEventListener() {

                    public void serialEvent (SerialPortEvent arg0) {

                        try {
                            // read the data in string format
                            tfRecievedData.setText(ServoSerialPort.readString());

                        } catch (SerialPortException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }

                    }
                });


            } catch (SerialPortException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();

            }

        } else {
            // disable all control when port is disconnected
            disableControls();
            toggleConnectSerial.setText("Connect");
            try {
                ServoSerialPort.closePort();
            } catch (SerialPortException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

        }

    }

}
