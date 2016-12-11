/*=====================================================================
 * Author: Rey John Lim
 * Program: java arduino controller
 * Ver. 1.0
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * =====================================================================*/

package serialPortComms;

import java.awt.Color;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.TimeUnit;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComboBox;

import jssc.*;

import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JSeparator;
import javax.swing.JSlider;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JMenu;

public class SerComMain {

	/*
	 * ==================================== SERIAL PORT PARRAMETERS ==========================================
	 */

	int[] baudRates = { SerialPort.BAUDRATE_4800, SerialPort.BAUDRATE_9600,
			SerialPort.BAUDRATE_115200, SerialPort.BAUDRATE_128000,
			SerialPort.BAUDRATE_256000 };

	int[] dataRates = { SerialPort.DATABITS_5, SerialPort.DATABITS_6,
			SerialPort.DATABITS_7, SerialPort.DATABITS_8 };

	int[] stopBits = { SerialPort.STOPBITS_1, SerialPort.STOPBITS_2,
			SerialPort.STOPBITS_1_5 };

	int[] parityBits = { SerialPort.PARITY_EVEN, SerialPort.PARITY_ODD,
			SerialPort.PARITY_MARK, SerialPort.PARITY_SPACE,
			SerialPort.PARITY_NONE };

	int setBaudRate = 0, setDataBit = 0, setStopBit = 0, setParityBit = 0;

	/* ==================== GUI COMPONENTS =================================== */

	private JFrame frame;

	private JPanel serialSettingPanel;
	private JPanel ctrlPanel;

	private JComboBox<String> cbox_PortName;

	private JComboBox<Integer> cBox_baudRate, cBox_StopBits, cBox_DataBit,
			cBox_ParityBits;

	private JLabel lblBaudRate, lblDataBit, lblStopBit, lblParityBits,
			lblPortName, lblSelectPort, lblStatus;

	private JTextField tF_PortName, tF_Status;

	private JButton btnOpenPort, btnClosePort;

	private String setCommPortName;

	private SerialPort serialCommPort;
	private JTextField tFval_A, tFval_B, tFval_C, tFval_D;

	private JSlider ctrlSlider_A, ctrlSlider_B, ctrlSlider_C, ctrlSlider_D;
	private JButton btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9;
	private JLabel lbl_A, lbl_B, lbl_C, lbl_D;;
	private JSeparator separator_1;
	private JTextField tf_AnalogValue_A0;
	private JTextField tf_AnalogValue_A1;
	private JTextField tf_AnalogValue_A2;
	private JLabel lblServoMotorAngle;
	private JMenu mnFile;
	private JMenu mnHelp;
	private JMenuItem mntmAbout;
	private JMenuItem mntmArduinoCode;
	private JMenuItem mntmDocumentation;

	private void guiContent() {

		/*
		 * ================== PANEL CONTAINING ALL SELECTABLE PARAMETERS ===================================
		 */
		serialSettingPanel = new JPanel();
		serialSettingPanel.setLayout(null);
		serialSettingPanel.setBounds(12, 39, 468, 156);
		serialSettingPanel.setBorder(BorderFactory.createTitledBorder(
				BorderFactory.createEtchedBorder(), "Serial Port Settings"));

		lblSelectPort = new JLabel("Select Port :");
		lblSelectPort.setBounds(12, 24, 97, 20);
		serialSettingPanel.add(lblSelectPort);

		cbox_PortName = new JComboBox<String>();
		cbox_PortName.setBounds(111, 24, 126, 20);
		serialSettingPanel.add(cbox_PortName);

		lblBaudRate = new JLabel("BAUD Rate :");
		lblBaudRate.setBounds(12, 50, 97, 20);
		serialSettingPanel.add(lblBaudRate);

		cBox_baudRate = new JComboBox<Integer>();
		cBox_baudRate.setBounds(111, 50, 126, 20);
		serialSettingPanel.add(cBox_baudRate);

		lblDataBit = new JLabel("Data Bit :");
		lblDataBit.setBounds(12, 80, 81, 17);
		serialSettingPanel.add(lblDataBit);

		cBox_DataBit = new JComboBox<Integer>();
		cBox_DataBit.setBounds(111, 78, 126, 20);
		serialSettingPanel.add(cBox_DataBit);

		cBox_StopBits = new JComboBox<Integer>();
		cBox_StopBits.setBounds(326, 24, 126, 20);
		serialSettingPanel.add(cBox_StopBits);

		lblStopBit = new JLabel("Stop Bit :");
		lblStopBit.setBounds(249, 27, 81, 15);
		serialSettingPanel.add(lblStopBit);

		cBox_ParityBits = new JComboBox<Integer>();
		cBox_ParityBits.setBounds(326, 50, 126, 20);
		serialSettingPanel.add(cBox_ParityBits);

		lblParityBits = new JLabel("Parity Bit :");
		lblParityBits.setBounds(249, 52, 81, 16);
		serialSettingPanel.add(lblParityBits);

		btnOpenPort = new JButton("Connect");
		btnOpenPort.setBounds(340, 80, 112, 25);
		btnOpenPort.setBackground(Color.GREEN);
		serialSettingPanel.add(btnOpenPort);

		btnClosePort = new JButton("Disconnect");
		btnClosePort.setBounds(340, 113, 112, 25);
		btnClosePort.setBackground(Color.RED);
		btnClosePort.setForeground(Color.YELLOW);
		btnClosePort.setEnabled(false);
		serialSettingPanel.add(btnClosePort);

		/*
		 * ======================== PANEL CONTAINING CONTROL FOR INPUT AND * OUTPUT DATA ====================================
		 */
		ctrlPanel = new JPanel();
		ctrlPanel.setLayout(null);
		ctrlPanel.setBounds(12, 207, 468, 375);
		ctrlPanel.setBorder(BorderFactory.createTitledBorder(
				BorderFactory.createEtchedBorder(), "Control Panel"));

		lblPortName = new JLabel("Port Name :");
		lblPortName.setBounds(12, 25, 95, 15);
		ctrlPanel.add(lblPortName);

		tF_PortName = new JTextField();
		tF_PortName.setBounds(125, 23, 142, 19);
		ctrlPanel.add(tF_PortName);
		tF_PortName.setColumns(10);
		tF_PortName.setEditable(false);
		tF_PortName.setHorizontalAlignment(JTextField.CENTER);
		tF_PortName.setBackground(Color.LIGHT_GRAY);
		tF_PortName.setText("No Port Connected");

		lblStatus = new JLabel("Status :");
		lblStatus.setBounds(12, 52, 70, 15);
		ctrlPanel.add(lblStatus);

		tF_Status = new JTextField();
		tF_Status.setBounds(125, 50, 142, 19);
		ctrlPanel.add(tF_Status);
		tF_Status.setColumns(10);
		tF_Status.setEditable(false);
		tF_Status.setHorizontalAlignment(JTextField.CENTER);
		tF_Status.setBackground(Color.RED);
		tF_Status.setText("Disconnected");
		tF_Status.setForeground(Color.YELLOW);

		JSeparator separator = new JSeparator();
		separator.setBounds(12, 79, 444, 15);
		ctrlPanel.add(separator);

		/*
		 * ======================== ADD ALL PANEL TO FRAME  ========================
		 */

		frame.getContentPane().add(serialSettingPanel);
		frame.getContentPane().add(ctrlPanel);

		tFval_A = new JTextField("180");
		tFval_A.setBounds(12, 148, 33, 20);
		tFval_A.setHorizontalAlignment(JTextField.CENTER);
		ctrlPanel.add(tFval_A);
		tFval_A.setColumns(10);
		tFval_A.setEditable(false);
		tFval_A.setBackground(Color.BLACK);
		tFval_A.setForeground(Color.GREEN);

		ctrlSlider_A = new JSlider(0, 180);
		ctrlSlider_A.setOrientation(SwingConstants.VERTICAL);
		ctrlSlider_A.setBounds(12, 180, 33, 141);
		ctrlPanel.add(ctrlSlider_A);

		tFval_B = new JTextField("180");
		tFval_B.setHorizontalAlignment(SwingConstants.CENTER);
		tFval_B.setColumns(10);
		tFval_B.setBounds(57, 148, 33, 20);
		ctrlPanel.add(tFval_B);
		tFval_B.setEditable(false);
		tFval_B.setBackground(Color.BLACK);
		tFval_B.setForeground(Color.GREEN);

		ctrlSlider_B = new JSlider(0, 180);
		ctrlSlider_B.setOrientation(SwingConstants.VERTICAL);
		ctrlSlider_B.setBounds(57, 180, 33, 141);
		ctrlPanel.add(ctrlSlider_B);

		tFval_C = new JTextField("180");
		tFval_C.setHorizontalAlignment(SwingConstants.CENTER);
		tFval_C.setColumns(10);
		tFval_C.setBounds(102, 148, 33, 20);
		ctrlPanel.add(tFval_C);
		tFval_C.setEditable(false);
		tFval_C.setBackground(Color.BLACK);
		tFval_C.setForeground(Color.GREEN);

		ctrlSlider_C = new JSlider(0, 180);
		ctrlSlider_C.setOrientation(SwingConstants.VERTICAL);
		ctrlSlider_C.setBounds(102, 180, 33, 141);
		ctrlPanel.add(ctrlSlider_C);

		tFval_D = new JTextField("180");
		tFval_D.setHorizontalAlignment(SwingConstants.CENTER);
		tFval_D.setForeground(Color.GREEN);
		tFval_D.setEditable(false);
		tFval_D.setColumns(10);
		tFval_D.setBackground(Color.BLACK);
		tFval_D.setBounds(147, 148, 33, 20);
		ctrlPanel.add(tFval_D);

		ctrlSlider_D = new JSlider(0, 180);
		ctrlSlider_D.setOrientation(SwingConstants.VERTICAL);
		ctrlSlider_D.setEnabled(false);
		ctrlSlider_D.setBounds(147, 180, 33, 141);
		ctrlPanel.add(ctrlSlider_D);

		lbl_A = new JLabel("A");
		lbl_A.setHorizontalAlignment(JLabel.CENTER);
		lbl_A.setBounds(12, 121, 33, 15);
		ctrlPanel.add(lbl_A);

		lbl_B = new JLabel("B");
		lbl_B.setHorizontalAlignment(JLabel.CENTER);
		lbl_B.setBounds(57, 121, 33, 15);
		ctrlPanel.add(lbl_B);

		lbl_C = new JLabel("C");
		lbl_C.setHorizontalAlignment(JLabel.CENTER);
		lbl_C.setBounds(102, 121, 33, 15);
		ctrlPanel.add(lbl_C);

		lbl_D = new JLabel("D");
		lbl_D.setHorizontalAlignment(SwingConstants.CENTER);
		lbl_D.setBounds(147, 121, 33, 15);
		ctrlPanel.add(lbl_D);

		/*
		 * =====================DISABLED AFTER INITIALIZED  ============================================
		
		 */
		ctrlSlider_A.setEnabled(false);
		ctrlSlider_B.setEnabled(false);
		ctrlSlider_C.setEnabled(false);

		separator_1 = new JSeparator();
		separator_1.setOrientation(SwingConstants.VERTICAL);
		separator_1.setBounds(216, 106, 13, 257);
		ctrlPanel.add(separator_1);

		JLabel lblAnalogValue = new JLabel("Analog Value");
		lblAnalogValue.setBounds(308, 237, 105, 15);
		ctrlPanel.add(lblAnalogValue);

		JLabel lblA = new JLabel("A0 :");
		lblA.setBounds(265, 264, 38, 15);
		ctrlPanel.add(lblA);

		JLabel lblA_1 = new JLabel("A1 :");
		lblA_1.setBounds(265, 290, 44, 15);
		ctrlPanel.add(lblA_1);

		JLabel lblA_2 = new JLabel("A2 :");
		lblA_2.setBounds(265, 317, 38, 15);
		ctrlPanel.add(lblA_2);

		tf_AnalogValue_A0 = new JTextField("xxxx");
		tf_AnalogValue_A0.setBounds(299, 264, 125, 19);
		ctrlPanel.add(tf_AnalogValue_A0);
		tf_AnalogValue_A0.setColumns(10);
		tf_AnalogValue_A0.setEditable(false);
		tf_AnalogValue_A0.setBackground(Color.BLACK);
		tf_AnalogValue_A0.setForeground(Color.GREEN);
		tf_AnalogValue_A0.setHorizontalAlignment(SwingConstants.CENTER);

		tf_AnalogValue_A1 = new JTextField("xxxx");
		tf_AnalogValue_A1.setBounds(299, 290, 125, 19);
		ctrlPanel.add(tf_AnalogValue_A1);
		tf_AnalogValue_A1.setColumns(10);
		tf_AnalogValue_A1.setEditable(false);
		tf_AnalogValue_A1.setBackground(Color.BLACK);
		tf_AnalogValue_A1.setForeground(Color.GREEN);
		tf_AnalogValue_A1.setHorizontalAlignment(SwingConstants.CENTER);

		tf_AnalogValue_A2 = new JTextField("xxxx");
		tf_AnalogValue_A2.setBounds(299, 317, 125, 19);
		ctrlPanel.add(tf_AnalogValue_A2);
		tf_AnalogValue_A2.setColumns(10);
		tf_AnalogValue_A2.setEditable(false);
		tf_AnalogValue_A2.setBackground(Color.BLACK);
		tf_AnalogValue_A2.setForeground(Color.GREEN);
		tf_AnalogValue_A2.setHorizontalAlignment(SwingConstants.CENTER);

		JSeparator separator_2 = new JSeparator();
		separator_2.setBounds(229, 224, 227, 13);
		ctrlPanel.add(separator_2);

		lblServoMotorAngle = new JLabel("Servo Motor Angle");
		lblServoMotorAngle.setBounds(32, 93, 130, 15);
		ctrlPanel.add(lblServoMotorAngle);

		/*
		 * ========================== key pad button ===================================
		 */

		btn1 = new JButton("1");
		btn1.setEnabled(false);
		btn1.setBounds(229, 106, 57, 25);
		ctrlPanel.add(btn1);

		btn2 = new JButton("2");
		btn2.setEnabled(false);
		btn2.setBounds(299, 106, 57, 25);
		ctrlPanel.add(btn2);

		btn3 = new JButton("3");
		btn3.setEnabled(false);
		btn3.setBounds(367, 106, 57, 25);
		ctrlPanel.add(btn3);

		btn4 = new JButton("4");
		btn4.setBounds(229, 143, 57, 25);
		ctrlPanel.add(btn4);
		btn4.setEnabled(false);

		btn5 = new JButton("5");
		btn5.setBounds(298, 143, 57, 25);
		ctrlPanel.add(btn5);
		btn5.setEnabled(false);

		btn6 = new JButton("6");
		btn6.setBounds(367, 143, 57, 25);
		ctrlPanel.add(btn6);
		btn6.setEnabled(false);

		btn7 = new JButton("7");
		btn7.setEnabled(false);
		btn7.setBounds(229, 180, 57, 25);
		ctrlPanel.add(btn7);

		btn8 = new JButton("8");
		btn8.setEnabled(false);
		btn8.setBounds(299, 180, 57, 25);
		ctrlPanel.add(btn8);

		btn9 = new JButton("9");
		btn9.setEnabled(false);
		btn9.setBounds(367, 180, 57, 25);
		ctrlPanel.add(btn9);
		
		JMenuBar menuBar = new JMenuBar();
		menuBar.setBounds(0, 0, 512, 21);
		frame.getContentPane().add(menuBar);
		
		mnFile = new JMenu("File");
		menuBar.add(mnFile);
		
		mnHelp = new JMenu("Help");
		menuBar.add(mnHelp);
		
		mntmDocumentation = new JMenuItem("Documentation");
		mnHelp.add(mntmDocumentation);
		
		mntmArduinoCode = new JMenuItem("Arduino Code");
		mnHelp.add(mntmArduinoCode);
		
		mntmAbout = new JMenuItem("About");
		mnHelp.add(mntmAbout);

		/*
		 * =================== GET ALL SERIAL PORT NAME ============================================
		 */

		String[] portNames = SerialPortList.getPortNames();

		for (int i = 0; i < portNames.length; i++) {
			cbox_PortName.addItem(portNames[i]);
		}
		/*
		 * =========== ADD ALL SERIAL PORT PARAMETERS TO JCOMBO BOXES =================================
		 */

		for (int ix = 0; ix < baudRates.length; ix++) {
			cBox_baudRate.addItem(baudRates[ix]);
		}

		for (int iy = 0; iy < dataRates.length; iy++) {
			cBox_DataBit.addItem(dataRates[iy]);
		}

		for (int iz = 0; iz < stopBits.length; iz++) {
			cBox_StopBits.addItem(stopBits[iz]);
		}
		for (int iA = 0; iA < parityBits.length; iA++) {
			cBox_ParityBits.addItem(parityBits[iA]);
		}

		btn1.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent arg0) {
				btn1.setBackground(Color.BLUE);
				
			}
		});
		
		
		
		/*
		 * ======================== SLIDER CONTROL A ===========================================
		 */
		ctrlSlider_A.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				tFval_A.setText(Integer.toString(ctrlSlider_A.getValue()));

				try {
					serialCommPort.writeString("1/"
							+ Integer.toString(ctrlSlider_A.getValue()) + "-");


				} catch (SerialPortException e1) {
					e1.printStackTrace();
				}
			}
		});

		/*
		 * ======================== SLIDER CONTROL B =========================================
		 */
		ctrlSlider_B.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				tFval_B.setText(Integer.toString(ctrlSlider_B.getValue()));
				try {
					serialCommPort.writeString("2/"
							+ Integer.toString(ctrlSlider_B.getValue()) + "-");
					TimeUnit.MILLISECONDS.sleep(50);
					serialCommPort.writeString("5/"
							+ Integer.toString(ctrlSlider_B.getValue()) + "-");
					TimeUnit.MILLISECONDS.sleep(50);
					serialCommPort.writeString("8/"
							+ Integer.toString(ctrlSlider_B.getValue()) + "-");
					TimeUnit.MILLISECONDS.sleep(50);
					serialCommPort.writeString("11/"
							+ Integer.toString(ctrlSlider_B.getValue()) + "-");
				} catch (SerialPortException | InterruptedException e2) {
					// TODO: handle exception
				}
			}
		});

		/*
		 * ======================== SLIDER CONTROL C =========================================
		 */
		ctrlSlider_C.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				tFval_C.setText(Integer.toString(ctrlSlider_C.getValue()));
				try {
					serialCommPort.writeString("3/"
							+ Integer.toString(ctrlSlider_C.getValue()) + "-");
					
					serialCommPort.writeString("6/"
							+ Integer.toString(ctrlSlider_C.getValue()) + "-");
				
					serialCommPort.writeString("9/"
							+ Integer.toString(ctrlSlider_C.getValue()) + "-");
	
					serialCommPort.writeString("12/"
							+ Integer.toString(ctrlSlider_C.getValue()) + "-");
				} catch (SerialPortException x) {
					// TODO: handle exception
				}
			}
		});

		/*
		 * ========================== SLIDER CONTROL D ===============================================

		 */
		ctrlSlider_D.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				tFval_D.setText(Integer.toString(ctrlSlider_D.getValue()));
				try {
					// check servo number
					serialCommPort.writeString("3/"
							+ Integer.toString(ctrlSlider_D.getValue()) + "-");
					TimeUnit.MILLISECONDS.sleep(50);
				} catch (Exception e) {
					// TODO: handle exception
				}
			}
		});

		/*
		 * ======================= GET SELECTED SERIAL PORT NAME==============================
		 */
		cbox_PortName.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				setCommPortName = cbox_PortName.getSelectedItem().toString();
			}
		});

		/*
		 * ======================== GET SELECTED BAUD RATE =================================

		 */
		cBox_baudRate.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {

				setBaudRate = (int) cBox_baudRate.getSelectedItem();
				// System.out.println(setBaudRate);
			}
		});
		/*
		 * ======================== GET SELECTED DATABIT =================================
		 */
		cBox_DataBit.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				setDataBit = (int) cBox_DataBit.getSelectedItem();
				// System.out.println(setDataBit);
			}
		});
		/*
		 * ======================== GET SELECTED STOP BIT =================================
		 */
		cBox_StopBits.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				setStopBit = (int) cBox_StopBits.getSelectedItem();
				// System.out.println(setStopBit);
			}
		});

		/*
		 * ======================== GET SELECTED PARITY BIT =================================
		 */
		cBox_ParityBits.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				setParityBit = (int) cBox_ParityBits.getSelectedItem();
				// System.out.println(setParityBit);
			}
		});

		/*
		 * WHEN "OPEN PORT BUTTON" IS PRESSED, SET ALL PARAMETERS SELECTED BY
		 * COMBO BOX
		 */

		btnOpenPort.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				try {

					/*
					 * ================= SET ALL SELECTED PARAMETERS TO SERIAL PORT ========================
					 */

					serialCommPort = new SerialPort(setCommPortName); // Set serial port and serial port name
																		
					serialCommPort.openPort(); // Open serial port
					serialCommPort.setParams(setBaudRate, setDataBit,
							setStopBit, setParityBit); // Set parameters

					/*
					 * ================= CHANGE STATUS TO CONTROL PANEL====================================
					 */
					
					tF_Status.setText("Connected");
					tF_PortName.setText(setCommPortName);
					tF_Status.setBackground(Color.GREEN);
					tF_Status.setForeground(Color.BLACK);

					/*
					 * ================ DISABLE SELECTION TO COMBOBOX ==========================================
					 */
					
					cBox_baudRate.setEnabled(false);
					cBox_DataBit.setEnabled(false);
					cBox_ParityBits.setEnabled(false);
					cbox_PortName.setEnabled(false);
					cBox_StopBits.setEnabled(false);

					/*
					 * ================ DISABLE OPEN PORT BUTTON ================================================
					 */
					/*
					 * disable "Open Port" Button to prevent pressing it more
					 * than once when the port is already set. prevent
					 * unexpected changes to serial port when all parameters are
					 * set and the port is already open and connected to the
					 * device
					 */
					btnOpenPort.setEnabled(false);
					btnClosePort.setEnabled(true);

					// Write data to port
					// serialCommPort.writeBytes("This is a test string".getBytes());
					// Close serial port

					// System.out.println("Serial Port Open");
					/*
					 * ====================== ENABLE CONTROL PANEL IN CONTROL PANEL ==============================
					 */
					ctrlSlider_A.setEnabled(true);
					ctrlSlider_B.setEnabled(true);
					ctrlSlider_C.setEnabled(true);
					ctrlSlider_D.setEnabled(true);
					btn1.setEnabled(true);
					btn2.setEnabled(true);
					btn3.setEnabled(true);
					btn4.setEnabled(true);
					btn5.setEnabled(true);
					btn6.setEnabled(true);
					btn7.setEnabled(true);
					btn8.setEnabled(true);
					btn9.setEnabled(true);

					serialCommPort.addEventListener(new SerialPortEventListener() {

								@Override
								public void serialEvent(
										SerialPortEvent myPortEvent) {

									if (myPortEvent.isRXCHAR()) {

										try {
											tf_AnalogValue_A0
													.setText(serialCommPort
															.readString());
										} catch (SerialPortException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										}
									}

								}
							});

				} catch (SerialPortException ex) {
					// show message if port name is not selected
					JOptionPane
							.showMessageDialog(frame,
									"Please select port name.",
									"Serial Port Connection",
									JOptionPane.ERROR_MESSAGE);
				}
			}
		});

		/* WHEN "CLOSE PORT" IS PRESSED CLOSE THE SERIAL PORT */
		btnClosePort.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				try {

					/*
					 * ================= CHANGE STATUS TO CONTROL PANEL
					 * ========================================
					 */
					tF_PortName.setText("No Port Selected");
					tF_Status.setBackground(Color.RED);
					tF_Status.setText("Disconnected");
					tF_Status.setForeground(Color.YELLOW);

					/*
					 * ================ ENABLE SELECTION TO COMBOBOX
					 * ==========================================
					 */
					cBox_baudRate.setEnabled(true);
					cBox_DataBit.setEnabled(true);
					cBox_ParityBits.setEnabled(true);
					cbox_PortName.setEnabled(true);
					cBox_StopBits.setEnabled(true);

					/*
					 * ================ ENABLE OPEN PORT BUTTON
					 * ================================================
					 */
					btnOpenPort.setEnabled(true);
					btnClosePort.setEnabled(false);

					/*
					 * ================ CLOSE SERIAL PORT
					 * =======================================================
					 */
					serialCommPort.closePort();
					/*
					 * ====================== DISABLE CONTROL PANEL IN CONTROL
					 * PANEL ==============================
					 */
					ctrlSlider_A.setEnabled(false);
					ctrlSlider_B.setEnabled(false);
					ctrlSlider_C.setEnabled(false);
					ctrlSlider_D.setEnabled(false);
					btn1.setEnabled(false);
					btn2.setEnabled(false);
					btn3.setEnabled(false);
					btn4.setEnabled(false);
					btn5.setEnabled(false);
					btn6.setEnabled(false);
					btn7.setEnabled(false);
					btn8.setEnabled(false);
					btn9.setEnabled(false);

					// System.out.println("Serial Port Closed");
				} catch (Exception evnt) {
					// NO NEED TO SHOW THIS DIALOG BOX SINCE CLOSE BUTTON IS
					// DISABLED WHEN THE PORT IS ALREADY CLOSED
					JOptionPane
							.showMessageDialog(frame,
									"Serial Port is already Closed",
									"Serial Port Connection",
									JOptionPane.ERROR_MESSAGE);
				}

			}
		});
		/*
		 * ======================================================================
		 * ==================================
		 */

	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame("Serial Port to Arduino Interface ");
		frame.setBounds(100, 100, 504, 623);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		frame.setResizable(false);
		guiContent();
	}

	/**
	 * Create the application.
	 */
	public SerComMain() {
		initialize();
	}

	/**
	 * Launch the application.
	 */

	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					SerComMain window = new SerComMain();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
