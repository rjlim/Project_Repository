/*===============================================
* Program Name: Quadruped Bot controller -	    *
* 				Controller Class                *
* -----------------------------------------------
* Version : 1.0 							    *
* -----------------------------------------------
* Author : Rey John Lim                         *
* -----------------------------------------------
* Description: controller class for Quadruped
* robot this code control all the UI in the FXML
* =============================================**/

package sample;
/*============== IMPORT SECTION ===============*/

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import jssc.*;

import java.net.URL;
import java.text.NumberFormat;
import java.util.ResourceBundle;


/*=============== INITIALIZE UI =======================*/
public class BotUIController implements Initializable {

	/*======== CREATE SLIDER OBJECTS ===========*/
	/* create a slider and reference it to FXML with initial value of
	* servo motor from 0 to 180 degree with initial value of 90 degrees
	* sliders can be also controlled by arrow keys during run time*/

	/* sliders are named based on location of each servo */

	// slider servo A1-A3
	@FXML
	Slider a1_slider = new Slider(0, 180, 90);
	@FXML
	Slider a2_slider = new Slider(0, 180, 90);
	@FXML
	Slider a3_slider = new Slider(0, 180, 90);

	// slider servo B1-B3
	@FXML
	Slider b1_slider = new Slider(0, 180, 90);
	@FXML
	Slider b2_slider = new Slider(0, 180, 90);
	@FXML
	Slider b3_slider = new Slider(0, 180, 90);

	// slider servo C1-C3
	@FXML
	Slider c1_slider = new Slider(0, 180, 90);
	@FXML
	Slider c2_slider = new Slider(0, 180, 90);
	@FXML
	Slider c3_slider = new Slider(0, 180, 90);

	// slider servo D1-D3
	@FXML
	Slider d1_slider = new Slider(0, 180, 90);
	@FXML
	Slider d2_slider = new Slider(0, 180, 90);
	@FXML
	Slider d3_slider = new Slider(0, 180, 90);


	/* =================== CREATE BUTTON OBJECT ========================== */
	/* button object for fine tune the angle of the servo same as the sliders
	* it is reference for FXML */

	// button for adding sequence
	@FXML
	Button btonAddSequenceServoA = new Button();
	@FXML
	Button btonAddSequenceServoB = new Button();
	@FXML
	Button btonAddSequenceServoC = new Button();
	@FXML
	Button btonAddSequenceServoD = new Button();


	// ====== A1-A3 Servo Increase Button ======
	@FXML
	Button a1_IncrBtn = new Button();
	@FXML
	Button a2_IncrBtn = new Button();
	@FXML
	Button a3_IncrBtn = new Button();

	// ====== A1-A3 Servo Decrease Button ======
	@FXML
	Button a1_DecrBtn = new Button();
	@FXML
	Button a2_DecrBtn = new Button();
	@FXML
	Button a3_DecrBtn = new Button();

	// ====== B1-B3 Servo Increase Button ======
	@FXML
	Button b1_IncrBtn = new Button();
	@FXML
	Button b2_IncrBtn = new Button();
	@FXML
	Button b3_IncrBtn = new Button();

	// ====== B1-B3 Servo Decrease Button ======
	@FXML
	Button b1_DecrBtn = new Button();
	@FXML
	Button b2_DecrBtn = new Button();
	@FXML
	Button b3_DecrBtn = new Button();

	// ====== C1-C3 Servo Increase Button ======
	@FXML
	Button c1_IncrBtn = new Button();
	@FXML
	Button c2_IncrBtn = new Button();
	@FXML
	Button c3_IncrBtn = new Button();

	// ====== C1-C3 Servo Decrease Button ======
	@FXML
	Button c1_DecrBtn = new Button();
	@FXML
	Button c2_DecrBtn = new Button();
	@FXML
	Button c3_DecrBtn = new Button();

	// ====== D1-D3 Servo Increase Button ======
	@FXML
	Button d1_IncrBtn = new Button();
	@FXML
	Button d2_IncrBtn = new Button();
	@FXML
	Button d3_IncrBtn = new Button();

	// ====== D1-D3 Servo Decrease Button ======
	@FXML
	Button d1_DecrBtn = new Button();
	@FXML
	Button d2_DecrBtn = new Button();
	@FXML
	Button d3_DecrBtn = new Button();

	/* ================== TEXTFIELD =====================*/
	/* textfield for showing the current angle of each servo */
	@FXML
	TextField a1_tFval = new TextField();
	@FXML
	TextField a2_tFval = new TextField();
	@FXML
	TextField a3_tFval = new TextField();

	@FXML
	TextField b1_tFval = new TextField();
	@FXML
	TextField b2_tFval = new TextField();
	@FXML
	TextField b3_tFval = new TextField();

	@FXML
	TextField c1_tFval = new TextField();
	@FXML
	TextField c2_tFval = new TextField();
	@FXML
	TextField c3_tFval = new TextField();

	@FXML
	TextField d1_tFval = new TextField();
	@FXML
	TextField d2_tFval = new TextField();
	@FXML
	TextField d3_tFval = new TextField();

	// text field for sending data to serial port
	@FXML
	TextField tf_SendData = new TextField();

	/*==============Text Area ================*/
	/*display data on recieve or send via serial port*/
	@FXML
	TextArea TerminalTextArea = new TextArea();
	@FXML
	TextArea TA_Sequence = new TextArea();



	/* ====================== PANES =========================== */
	/* container for each control of the UI*/
	@FXML
	Pane ctrlPane = new Pane();
	@FXML
	Pane SerialPortPane = new Pane();


	/* ====================== TOGGLE BUTTON ====================== */
	/* toggle button for connecting or disconnect serial connection*/
	@FXML
	ToggleButton togbtn = new ToggleButton();

	ToggleGroup togGroup = new ToggleGroup();

	/* ======================= COMBO BOX ========================== */
	/*combobox for the serial port paramenters each combobox
	* each combobox contains one parameter of the serial port*/

	// create combo box for serial port names
	@FXML
	ComboBox<String> portName = new ComboBox<>();
	// create combo box for serial port baud rate
	@FXML
	ComboBox<String> cboxBaud = new ComboBox<>();
	// create combo box for serial port data bit
	@FXML
	ComboBox<String> cboxDataBit = new ComboBox<>();
	// create combo box for serial port stop bit
	@FXML
	ComboBox<String> cboxStopBit = new ComboBox<>();
	// create combo box for serial port parity bit
	@FXML
	ComboBox<String> cboxParityBit = new ComboBox<>();

	// create serial port object
	SerialPort serialCommPort;

	/*================================================================
	* 				   			 VARIABLES 							 *
	*=============================================================== */

	// create a string array that will contain the port name
	String ComPortName[];
	int val1, val2 ;
	/* ====Serial Port parameters in Arrays===== */

	/* ======================== BAUD RATE======================= */

	String[] baudRatesString = { Integer.toString(SerialPort.BAUDRATE_4800),
								 Integer.toString(SerialPort.BAUDRATE_9600),
								 Integer.toString(SerialPort.BAUDRATE_115200),
								 Integer.toString(SerialPort.BAUDRATE_128000),
								 Integer.toString(SerialPort.BAUDRATE_256000) };

	/* ======================== DATA RATE================================ */

	String[] dataRatesString = { Integer.toString(SerialPort.DATABITS_5),
								 Integer.toString(SerialPort.DATABITS_6),
								 Integer.toString(SerialPort.DATABITS_7),
								 Integer.toString(SerialPort.DATABITS_8) };

	/* ======================== STOP BIT ARRAY======================== */
	/* stop bit in string format and be converted to float during selection in combobox*/
	String[] stopBitString = { "1", "1.5", "2" };

	/* ======================== PARITY BIT ARRAY======================== */
	String[] parityBitString = { "none", "even", "odd", "mark", "space" };

	// set initial value of parity and stop bit to 0
	int  selectParity = 0, selectStopBit = 0;

	// temporary String value;
	String tempStringVal;

	CmdDisassembler cmdDis = new CmdDisassembler();

	//setBaudRate = 0, setDataBit = 0, setStopBit = 0, setParityBit = 0,

	/* ============ INITIALIZE METHOD====================== */
	/* initialize all objects and UI properties */
	@Override
	public void initialize(URL location, ResourceBundle resources) {

		// terminal is not editable
		TerminalTextArea.setEditable(false);
		TerminalTextArea.setWrapText(true);
		TA_Sequence.setWrapText(true);

		// get the port Name of all the serial port connection
		ComPortName = SerialPortList.getPortNames();
		// array to a list for the combo box
		ObservableList<String> CommList = FXCollections.observableArrayList(ComPortName);
		ObservableList<String> BaudList = FXCollections.observableArrayList(baudRatesString);
		ObservableList<String> dataRateList = FXCollections.observableArrayList(dataRatesString);
		ObservableList<String> stopBitList = FXCollections.observableArrayList(stopBitString);
		ObservableList<String> parityBitList = FXCollections.observableArrayList(parityBitString);

		// add all list to combo box so it can be seen in the UI
		portName.setItems(CommList);
		cboxBaud.setItems(BaudList);
		cboxDataBit.setItems(dataRateList);
		cboxParityBit.setItems(parityBitList);
		cboxStopBit.setItems(stopBitList);

		// add toggle button to toggle group
		togGroup.getToggles().add(togbtn);

		/*
		start with disabled controls
		this prevent user from making unnecessary
		change in the serial port while its not connected
		*/
		DisableServoControls();

		/* set text properties to bidirectional it means that whenever you set the value of
		slider it will set also the value of the text field vice versa.*/

		/* set the textfield appearance to black background with green text color*/
		a1_tFval.textProperty().bindBidirectional(a1_slider.valueProperty(), NumberFormat.getIntegerInstance());
		a1_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		a2_tFval.textProperty().bindBidirectional(a2_slider.valueProperty(), NumberFormat.getIntegerInstance());
		a2_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		a3_tFval.textProperty().bindBidirectional(a3_slider.valueProperty(), NumberFormat.getIntegerInstance());
		a3_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		/*===================================================================== */
		b1_tFval.textProperty().bindBidirectional(b1_slider.valueProperty(), NumberFormat.getIntegerInstance());
		b1_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		b2_tFval.textProperty().bindBidirectional(b2_slider.valueProperty(), NumberFormat.getIntegerInstance());
		b2_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		b3_tFval.textProperty().bindBidirectional(b3_slider.valueProperty(), NumberFormat.getIntegerInstance());
		b3_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		/*===================================================================== */
		c1_tFval.textProperty().bindBidirectional(c1_slider.valueProperty(), NumberFormat.getIntegerInstance());
		c1_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		c2_tFval.textProperty().bindBidirectional(c2_slider.valueProperty(), NumberFormat.getIntegerInstance());
		c2_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		c3_tFval.textProperty().bindBidirectional(c3_slider.valueProperty(), NumberFormat.getIntegerInstance());
		c3_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		/*===================================================================== */
		d1_tFval.textProperty().bindBidirectional(d1_slider.valueProperty(), NumberFormat.getIntegerInstance());
		d1_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		d2_tFval.textProperty().bindBidirectional(d2_slider.valueProperty(), NumberFormat.getIntegerInstance());
		d2_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		d3_tFval.textProperty().bindBidirectional(d3_slider.valueProperty(), NumberFormat.getIntegerInstance());
		d3_tFval.setStyle("-fx-text-fill: #83f52c ; -fx-control-inner-background: Black");

		/*===============================================*
		*               SLIDER LISTENERS  				 *
		* ===============================================*/
		/*
		*  lister properties whenever the slider changes its value it run
		*  piece of program, in this case it will send the servo number
		*  and the servo angle in this format a/xxx-
		*  where: a = the servo id
		*  		  xxx = servo angle
		*  		  / = seperator
		*  		  - = terminator command end the command and wait for another set of command
		*/

		//======================= Property Slider for A1-A3 ==============================
		
		a1_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("1/"+((int) a1_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		a2_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("2/"+((int) a2_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		a3_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("3/"+((int) a3_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		//======================= Slider Property for B1-B3 ==============================

		b1_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("4/"+((int) b1_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		b2_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("5/"+((int) b2_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		b3_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("6/"+((int) b3_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		//========================= Slider Property for C1-C3 =================================
		c1_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("7/"+((int) c1_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		c2_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("8/"+((int) c2_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		c3_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("9/"+((int) c3_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		// ============================= Slider Property for D1-D3 =======================
		d1_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("10/"+((int) d1_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		d2_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("11/"+((int) d2_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});
		
		d3_slider.valueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {

				try {
					serialCommPort.writeString("12/"+((int) d3_slider.getValue())+"-");
				} catch (SerialPortException e) {

					e.printStackTrace();
				}
			}
		});

		//
		portName.setOnMouseClicked(new EventHandler<MouseEvent>() {
			@Override
			public void handle(MouseEvent event) {
				// remove all portnames in Combobox
				portName.getItems().clear();
				// and the updated ports
				portName.getItems().addAll(SerialPortList.getPortNames());
			}
		});


	}

	/*===========================INITIALIZE END=============================*/

	/*====================================*
	 *             UI METHODS             *
	 *====================================*/

	/* methods that the FXML UI call whenever a change in state in the UI
	* such as when the button is pressed or changing the text in the textfield*/

	// refresh port name list


	// select Stop bit
	public void stopBitIsSelected(ActionEvent btnPart) {
		switch (cboxStopBit.getValue()) {

		case "2":
			selectStopBit = SerialPort.STOPBITS_2;
			break;
		case "1.5":
			selectStopBit = SerialPort.STOPBITS_1_5;
			break;
		default:
			selectStopBit = SerialPort.STOPBITS_1;
			break;
		}
	}

	// select parity bit
	public void parityBitIsSelected(ActionEvent btnStop) {
		switch (cboxParityBit.getValue()) {

		case "even":
			System.out.println("even");
			selectParity = SerialPort.PARITY_EVEN;
			break;
		case "odd":
			System.out.println("odd");
			selectParity = SerialPort.PARITY_ODD;
			break;
		case "mark":
			System.out.println("mark");
			selectParity = SerialPort.PARITY_MARK;
			break;
		case "space":
			System.out.println("space");
			selectParity = SerialPort.PARITY_SPACE;
			break;
		default:
			System.out.println("none");
			selectParity = SerialPort.PARITY_NONE;
			break;
		}

	}

	/*
	 * ==== open or close the serial port when toggle button is pressed=======
	 */

	public void toggleBtn(ActionEvent tgle) {

		if (togbtn.isSelected() == true) {

			// set toggle button text to disconnect
			togbtn.setText("Disconnect");
			// disable all combobox to prevent the user from making unexpected
			SerialControlDisable();
			// enable controls to servo motor
			EnableServoControls();
			// set the port name
			serialCommPort = new SerialPort(portName.getValue());
			try {

				// open the serial port
				serialCommPort.openPort();
				// set the port parameters
				serialCommPort.setParams(Integer.parseInt(cboxBaud.getValue()),
										 Integer.parseInt(cboxDataBit.getValue()),
						                 selectStopBit, selectParity);

			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				System.out.println("Serial Port Unavailable");
			}
			// read serial port
			try {
				serialCommPort.addEventListener(new SerialPortEventListener() {
					@Override
					public void serialEvent(SerialPortEvent serialPortEvent) {
						try {
							tempStringVal = serialCommPort.readString();
							Thread.sleep(50);
							TerminalTextArea.appendText("Rx: " + tempStringVal);
							tempStringVal = null;
						} catch (SerialPortException e) {
							e.printStackTrace();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				});
			} catch (SerialPortException e) {
				e.printStackTrace();
			}

		} else {
			togbtn.setText("Connect");
			// enable serial port
			SerialControlEnable();
			// disable servo controls
			DisableServoControls();
			try {
				// close serial port
				serialCommPort.closePort();
			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

	}

	/*
	 * =========================================================================
	 */

	public void btnPressIncr_a1(ActionEvent eventbtnA1) throws InterruptedException {

		a1_slider.setValue(Integer.parseInt(a1_tFval.getText()) + 1);
		a1_tFval.setText(Integer.toString((int) a1_slider.getValue()));

		try {
			serialCommPort.writeString("1/" + Integer.toString((int) a1_slider.getValue()) + "-");

		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public void btnPressDncr_a1(ActionEvent eventbtnA1) {

		a1_slider.setValue(Integer.parseInt(a1_tFval.getText()) - 1);
		a1_tFval.setText(Integer.toString((int) a1_slider.getValue()));

		try {
			serialCommPort.writeString("1/" + Integer.toString((int) a1_slider.getValue()) + "-");

		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void btnPressIncr_a2(ActionEvent eventbtnA2) {

		a2_slider.setValue(Integer.parseInt(a2_tFval.getText()) + 1);
		a2_tFval.setText(Integer.toString((int) a2_slider.getValue()));
		
		try {
			serialCommPort.writeString("2/" + Integer.toString((int) a1_slider.getValue()) + "-");

		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void btnPressDncr_a2(ActionEvent eventbtnA2) {

		a2_slider.setValue(Integer.parseInt(a2_tFval.getText()) - 1);
		a2_tFval.setText(Integer.toString((int) a2_slider.getValue()));
	}

	public void btnPressIncr_a3(ActionEvent eventbtnA3) {

		a3_slider.setValue(Integer.parseInt(a3_tFval.getText()) + 1);
		a3_tFval.setText(Integer.toString((int) a3_slider.getValue()));
	}

	@FXML
	public void btnPressDncr_a3(ActionEvent eventbtnA3) {

		a3_slider.setValue(Integer.parseInt(a3_tFval.getText()) - 1);
		a3_tFval.setText(Integer.toString((int) a3_slider.getValue()));
	}

	/*
	 * =========================================================================
	 */

	public void btnPressIncr_b1(ActionEvent eventbtnB1) {

		b1_slider.setValue(Integer.parseInt(b1_tFval.getText()) + 1);
		b1_tFval.setText(Integer.toString((int) b1_slider.getValue()));
	}

	public void btnPressDncr_b1(ActionEvent eventbtnB1) {

		b1_slider.setValue(Integer.parseInt(b1_tFval.getText()) - 1);
		b1_tFval.setText(Integer.toString((int) b1_slider.getValue()));
	}

	public void btnPressIncr_b2(ActionEvent eventbtnB2) {

		b2_slider.setValue(Integer.parseInt(b2_tFval.getText()) + 1);
		b2_tFval.setText(Integer.toString((int) b2_slider.getValue()));
	}

	public void btnPressDncr_b2(ActionEvent eventbtnB2) {

		b2_slider.setValue(Integer.parseInt(b2_tFval.getText()) - 1);
		b2_tFval.setText(Integer.toString((int) b2_slider.getValue()));
	}

	public void btnPressIncr_b3(ActionEvent eventbtnB3) {

		b3_slider.setValue(Integer.parseInt(b3_tFval.getText()) + 1);
		b3_tFval.setText(Integer.toString((int) b3_slider.getValue()));
	}

	public void btnPressDncr_b3(ActionEvent eventbtnB3) {

		b3_slider.setValue(Integer.parseInt(b3_tFval.getText()) - 1);
		b3_tFval.setText(Integer.toString((int) b3_slider.getValue()));
	}

	/*
	 * =========================================================================
	 */

	public void btnPressIncr_c1(ActionEvent eventbtnC1) {

		c1_slider.setValue(Integer.parseInt(c1_tFval.getText()) + 1);
		c1_tFval.setText(Integer.toString((int) c1_slider.getValue()));
	}

	public void btnPressDncr_c1(ActionEvent eventbtnC1) {

		c1_slider.setValue(Integer.parseInt(c1_tFval.getText()) - 1);
		c1_tFval.setText(Integer.toString((int) c1_slider.getValue()));
	}

	public void btnPressIncr_c2(ActionEvent eventbtnC2) {

		c2_slider.setValue(Integer.parseInt(c2_tFval.getText()) + 1);
		c2_tFval.setText(Integer.toString((int) c2_slider.getValue()));
	}

	public void btnPressDncr_c2(ActionEvent eventbtnC2) {

		c2_slider.setValue(Integer.parseInt(c2_tFval.getText()) - 1);
		c2_tFval.setText(Integer.toString((int) c2_slider.getValue()));
	}

	public void btnPressIncr_c3(ActionEvent eventbtnC3) {

		c3_slider.setValue(Integer.parseInt(c3_tFval.getText()) + 1);
		c3_tFval.setText(Integer.toString((int) c3_slider.getValue()));
	}

	public void btnPressDncr_c3(ActionEvent eventbtnC3) {

		c3_slider.setValue(Integer.parseInt(c3_tFval.getText()) - 1);
		c3_tFval.setText(Integer.toString((int) c3_slider.getValue()));
	}

	/*
	 * =========================================================================
	 */

	public void btnPressIncr_d1(ActionEvent eventbtnD1) {

		d1_slider.setValue(Integer.parseInt(d1_tFval.getText()) + 1);
		d1_tFval.setText(Integer.toString((int) d1_slider.getValue()));
	}

	public void btnPressDncr_d1(ActionEvent eventbtnD1) {

		d1_slider.setValue(Integer.parseInt(d1_tFval.getText()) - 1);
		d1_tFval.setText(Integer.toString((int) d1_slider.getValue()));
	}

	public void btnPressIncr_d2(ActionEvent eventbtnD2) {

		d2_slider.setValue(Integer.parseInt(d2_tFval.getText()) + 1);
		d2_tFval.setText(Integer.toString((int) d2_slider.getValue()));
	}

	public void btnPressDncr_d2(ActionEvent eventbtnD2) {

		d2_slider.setValue(Integer.parseInt(d2_tFval.getText()) - 1);
		d2_tFval.setText(Integer.toString((int) d2_slider.getValue()));
	}

	public void btnPressIncr_d3(ActionEvent eventbtnD3) {

		d3_slider.setValue(Integer.parseInt(d3_tFval.getText()) + 1);
		d3_tFval.setText(Integer.toString((int) d3_slider.getValue()));
	}

	public void btnPressDncr_d3(ActionEvent eventbtnD3) {

		d3_slider.setValue(Integer.parseInt(d3_tFval.getText()) - 1);
		d3_tFval.setText(Integer.toString((int) d3_slider.getValue()));
	}


	// send data to serial port when send button is pressed by the user
	public void onSendData(){
		// echo the data send by the terminal
		TerminalTextArea.appendText("Tx: " + tf_SendData.getText()+"\n");

		if (togbtn.isSelected() == true){
			try {

				serialCommPort.writeString(tf_SendData.getText()+"\n");

			} catch (SerialPortException e) {
				// catch error when serial port is unavailable
				TerminalTextArea.appendText("Port error: Serial port is Unavailable");
			}
		}
	}

	public void onSendSequence(){

		cmdDis.onCommandStream(TA_Sequence.getText(),"-","/");

		if (cmdDis.CommandAddressSize() == cmdDis.CommandValueSize())
		{
			for (int Cntr = 0 ; Cntr < cmdDis.CommandAddressSize() ; Cntr++)
			{

				if ((cmdDis.CommandAddressList()[Cntr] == null) &&
						(cmdDis.CommandValueList()[Cntr] == null))
				{
					val1 = 666;
					val2 = 999;

					System.out.println( "Command Address: "+ Integer.toString(val1)+
							" Command Value: " + Integer.toString(val2));
				}
				else {
					val1 = Integer.parseInt(cmdDis.CommandAddressList()[Cntr]);
					val2 = Integer.parseInt(cmdDis.CommandValueList()[Cntr]);

					System.out.println( "Command Address: "+ Integer.toString(val1)+
							" Command Value: " + Integer.toString(val2));
				}
			}
		}

		if (togbtn.isSelected() == true){
			try {

				serialCommPort.writeString("CA: "+ Integer.toString(val1) +"CV: "+Integer.toString(val2)+"\n");

			} catch (SerialPortException e) {
				// catch error when serial port is unavailable
				TerminalTextArea.appendText("Port error: Serial port is Unavailable");
			}
		}


	}

	public  void onAddSequence_ServoA(){
		TA_Sequence.appendText("1/" + a1_tFval.getText()+"-" +
							   "2/" + a2_tFval.getText()+"-" +
				               "3/" + a3_tFval.getText()+"-" );

	}
	public  void onAddSequence_ServoB(){
		TA_Sequence.appendText("4/" + b1_tFval.getText()+"-" +
				               "5/" + b2_tFval.getText()+"-" +
			               	   "6/" + b3_tFval.getText()+"-"  );

	}
	public  void onAddSequence_ServoC(){
		TA_Sequence.appendText("7/" + c1_tFval.getText()+"-" +
				               "8/" + c2_tFval.getText()+"-" +
				               "9/" + c3_tFval.getText()+"-" );

	}
	public  void onAddSequence_ServoD(){
		TA_Sequence.appendText("10/" + d1_tFval.getText()+"-" +
				               "11/" + d2_tFval.getText()+"-" +
				               "12/" + d3_tFval.getText()+"-" );

	}

	// enable UI controls
	public void EnableServoControls() {
		
		a1_slider.setDisable(false);
		a2_slider.setDisable(false);
		a3_slider.setDisable(false);
		
		b1_slider.setDisable(false);
		b2_slider.setDisable(false);
		b3_slider.setDisable(false);
		
		c1_slider.setDisable(false);
		c2_slider.setDisable(false);
		c3_slider.setDisable(false);
		
		d1_slider.setDisable(false);
		d2_slider.setDisable(false);
		d3_slider.setDisable(false);
		
		a1_IncrBtn.setDisable(false);
		a2_IncrBtn.setDisable(false);
		a3_IncrBtn.setDisable(false);
		
		b1_IncrBtn.setDisable(false);
		b2_IncrBtn.setDisable(false);
		b3_IncrBtn.setDisable(false);
		
		c1_IncrBtn.setDisable(false);
		c2_IncrBtn.setDisable(false);
		c3_IncrBtn.setDisable(false);
		
		d1_IncrBtn.setDisable(false);
		d2_IncrBtn.setDisable(false);
		d3_IncrBtn.setDisable(false);
		
		a1_DecrBtn.setDisable(false);
		a2_DecrBtn.setDisable(false);
		a3_DecrBtn.setDisable(false);
		
		b1_DecrBtn.setDisable(false);
		b2_DecrBtn.setDisable(false);
		b3_DecrBtn.setDisable(false);
		
		c1_DecrBtn.setDisable(false);
		c2_DecrBtn.setDisable(false);
		c3_DecrBtn.setDisable(false);
		
		d1_DecrBtn.setDisable(false);
		d2_DecrBtn.setDisable(false);
		d3_DecrBtn.setDisable(false);
		
	}
	//disable UI controls
	public void DisableServoControls() {
		
		a1_slider.setDisable(true);
		a2_slider.setDisable(true);
		a3_slider.setDisable(true);
		
		b1_slider.setDisable(true);
		b2_slider.setDisable(true);
		b3_slider.setDisable(true);
		
		c1_slider.setDisable(true);
		c2_slider.setDisable(true);
		c3_slider.setDisable(true);
		
		d1_slider.setDisable(true);
		d2_slider.setDisable(true);
		d3_slider.setDisable(true);
		
		a1_IncrBtn.setDisable(true);
		a2_IncrBtn.setDisable(true);
		a3_IncrBtn.setDisable(true);
		
		b1_IncrBtn.setDisable(true);
		b2_IncrBtn.setDisable(true);
		b3_IncrBtn.setDisable(true);
		
		c1_IncrBtn.setDisable(true);
		c2_IncrBtn.setDisable(true);
		c3_IncrBtn.setDisable(true);
		
		d1_IncrBtn.setDisable(true);
		d2_IncrBtn.setDisable(true);
		d3_IncrBtn.setDisable(true);
		
		a1_DecrBtn.setDisable(true);
		a2_DecrBtn.setDisable(true);
		a3_DecrBtn.setDisable(true);
		
		b1_DecrBtn.setDisable(true);
		b2_DecrBtn.setDisable(true);
		b3_DecrBtn.setDisable(true);
		
		c1_DecrBtn.setDisable(true);
		c2_DecrBtn.setDisable(true);
		c3_DecrBtn.setDisable(true);
		
		d1_DecrBtn.setDisable(true);
		d2_DecrBtn.setDisable(true);
		d3_DecrBtn.setDisable(true);
	}
	// enable serial port settings
	public void SerialControlEnable() {
		cboxBaud.setDisable(false);
		cboxDataBit.setDisable(false);
		cboxParityBit.setDisable(false);
		cboxStopBit.setDisable(false);
		portName.setDisable(false);
	}
	// disable serial port settings
	public void SerialControlDisable() {
		cboxBaud.setDisable(true);
		cboxDataBit.setDisable(true);
		cboxParityBit.setDisable(true);
		cboxStopBit.setDisable(true);
		portName.setDisable(true);
	}
	// set initial value of the slider
	public void onSetInitialValue(){
		//====================================================
		// set initial value of the slider when the program starts
		a1_slider.setValue(12);
		a2_slider.setValue(95);
		a3_slider.setValue(90);

		b1_slider.setValue(168);
		b2_slider.setValue(73);
		b3_slider.setValue(90);

		c1_slider.setValue(22);
		c2_slider.setValue(108);
		c3_slider.setValue(90);

		d1_slider.setValue(173);
		d2_slider.setValue(85);
		d3_slider.setValue(90);
	}
}