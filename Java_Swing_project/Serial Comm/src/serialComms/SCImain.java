package serialComms;

import java.awt.Color;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JFrame;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.LayoutStyle.ComponentPlacement;

import jssc.*;

import javax.swing.JSpinner;
import javax.swing.JComboBox;

import com.sun.tracing.dtrace.Attributes;

public class SCImain {

	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					SCImain window = new SCImain();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public SCImain() {
		initialize();
	}

	/**
	 * Initialize the contents of the fr
	 * ame.
	 */
	private JFrame frame;
	private JTextField tfSerCom;		// textfield for serial comm
	JButton btnSend = new JButton("Send");
	
	JButton btnOn = new JButton("ON");
	
	JButton btnOff = new JButton("OFF");
	
	JComboBox serialport_comboBox = new JComboBox();
	
	
	SerialPort serialPort = new SerialPort("/dev/ttyUSB0");
	
	String[] portNames = SerialPortList.getPortNames();
  

	
	
	int angle_alpha, angle_theta;
	String x ="1/1";


	
	
	private void initialize() {
		frame = new JFrame("Serial Communication for Arduino");
		frame.setBounds(100, 100, 349, 200);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		tfSerCom = new JTextField();
		tfSerCom.setColumns(10);
		
		btnOff.setBackground(Color.GREEN);
		btnOn.setBackground(Color.RED);
	
		/*
		 * Note : add name list to combobox program started
		 * 			send data to via textfield when btnSend is pressed
		 * 
		 * */
	 
		
		btnOff.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				btnOff.setBackground(Color.GREEN);
				btnOn.setBackground(Color.RED);
				  
				/*
				   for(int i = 0; i < portNames.length; i++){
				        System.out.println(portNames[i]);
				    }
				*/	
				 try {
					serialPort.closePort();
				} catch (SerialPortException e1) {
					// TODO Auto-generated catch block
					System.out.println("Serial Port not Available");
				}//Close serial port
			}
		});
		btnOn.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				btnOn.setBackground(Color.GREEN);
				btnOff.setBackground(Color.RED);
				 try {
			            serialPort.openPort();//Open serial port
			            serialPort.setParams(SerialPort.BAUDRATE_115200, 
			                                 SerialPort.DATABITS_8,
			                                 SerialPort.STOPBITS_1,
			                                 SerialPort.PARITY_NONE);//Set params. Also you can set params by this string: serialPort.setParams(9600, 8, 1, 0);
			            
			            serialPort.closePort();//Close serial port
			        }
			        catch (SerialPortException ex) {
			        	System.out.println("Serial Port not Available");
			        }
				
			}
		});
		
		
		btnSend.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				
			
					tfSerCom.setText(Integer.toString(angle_theta));
					angle_theta++;
					
			}
		
		});
		
		
		
		
		GroupLayout groupLayout = new GroupLayout(frame.getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING, false)
						.addComponent(serialport_comboBox, Alignment.LEADING, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
						.addGroup(Alignment.LEADING, groupLayout.createSequentialGroup()
							.addComponent(btnOn, GroupLayout.PREFERRED_SIZE, 146, GroupLayout.PREFERRED_SIZE)
							.addGap(18)
							.addComponent(btnOff, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
						.addComponent(btnSend, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
						.addComponent(tfSerCom, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 311, Short.MAX_VALUE))
					.addContainerGap(14, Short.MAX_VALUE))
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGap(12)
					.addComponent(tfSerCom, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(btnSend)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
						.addComponent(btnOn)
						.addComponent(btnOff))
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(serialport_comboBox, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addGap(33))
		);
		frame.getContentPane().setLayout(groupLayout);
	}
}
