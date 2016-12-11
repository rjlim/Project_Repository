package com.example.pc01.hexapodcontrolv1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.*;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.UUID;

public class HexMainCtrl extends AppCompatActivity {

    private static final int RECIEVE_MESSAGE = 1;

    private static final UUID my_Bt_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private static String myBtAddress;

    static Handler btHandler;

    private Handler moveFwd_Handler, moveRev_Handler, moveRight_Handler, moveLeft_Handler;

    private StringBuilder sb = new StringBuilder();

    private BtThread Bt_connectThread;

    private Switch swt_EnableBt, swt_connectBt;                // enable bluetooth and disconnect bluetooth

    private DataParse parseString = new DataParse();

    private TextView txV_setAngle_A,
            txV_setAngle_B, txV_setAngle_C;                    // display the current value of the vertical sliders

    private TextView txV_servNum_A,
            txV_servNum_B, txV_servNum_C;                      // set the label for slider

    private TextView txV_CurrentDevice;                        // display the current selected data

    private TextView txV_Cell1, txV_Cell2;                     // display cell voltage

    private TextView txV_Throttle_value;                       // display how fast hexapod move

    private Button btn_Throttle_Up, btn_Throttle_Down;         // increase/Decrease hexapod movement

    private Button btn_FWD, btn_REV,
            btn_STOP, btn_RIGHT, btn_LEFT;                     // basic motion controls

    private Button btn_Inc_A, btn_Inc_B, btn_Inc_C,
            btn_Decr_A, btn_Decr_B, btn_Decr_C;                // precision change in slider value

    private Button btn_rst_pos;                                // reset button to set default position

    private TextView[] txV_servo_ID_Angle = new TextView[18];  // array of text view for servo number and angle

    // array of textview IDs for servos
    private final int[] servo_IDs_ = {R.id.txVs1, R.id.txVs2, R.id.txVs3,
            R.id.txVs4, R.id.txVs5, R.id.txVs6,
            R.id.txVs7, R.id.txVs8, R.id.txVs9,
            R.id.txVs10, R.id.txVs11, R.id.txVs12,
            R.id.txVs13, R.id.txVs14, R.id.txVs15,
            R.id.txVs16, R.id.txVs17, R.id.txVs18,};

    private VertSeekBar vsBar_SetAngle_A,
            vsBar_SetAngle_B, vsBar_SetAngle_C;         // vertical seek bar for setting angle

    private Spinner spinDeviceList, spinLegList;        // contain all the paired devices and leg segments

    private BluetoothAdapter mrBtAdapter;               // Bluetooth adapter for the device
    private ArrayAdapter<String> pairedDevAdapter,      // array adapter for paired bluetooth device
            ServoLegAdapter;                            // array adapter for list of leg segment
    private BroadcastReceiver btState;                  // broadcast receiver for status of the bluetooth in the device
    private BluetoothSocket mrBtSocket;                 // Socket for connecting bluetooth

    private String logTag = "btRJL";

    private int ServoNum_A,
            ServoNum_B, ServoNum_C;                  // servo number to be sent via bluetooth

    private String[] LegSegmentServoNum = {"Servo 1-3", "Servo 4-6",
            "Servo 7-9", "Servo 10-12",
            "Servo 13-15", "Servo 16-18",};


    // this section is being tested
    //--------/?//////////////////////////////////////////////////////////////////////
    private int delayMsFix = 150;       //
    private int throttleValue = 5;      // speed of hexapod movement

     /* ========================================================================================//
    ||                                    R U N N A B L E S                                     ||
    ===========================================================================================*/


    private Runnable onMoveFwd = new Runnable() {
        @Override
        public void run() {
            try {

                //set fix command with throttle control

                if ((mrBtAdapter.getState() == BluetoothAdapter.STATE_ON)) {
                    Bt_connectThread.write("20/" + throttleValue + "-");
                }
                Log.i(logTag, "Forward");
            } finally {
                moveFwd_Handler.postDelayed(onMoveFwd, delayMsFix);
            }
        }
    };

    private Runnable onMoveRev = new Runnable() {
        @Override
        public void run() {
            try {
                if ((mrBtAdapter.getState() == BluetoothAdapter.STATE_ON)) {
                    Bt_connectThread.write("21/" + throttleValue + "-");
                }
                Log.i(logTag, "Reverse");

            } finally {
                moveRev_Handler.postDelayed(onMoveRev, delayMsFix);
            }
        }
    };

    private Runnable onMoveLeft = new Runnable() {
        @Override
        public void run() {
            try {
                if ((mrBtAdapter.getState() == BluetoothAdapter.STATE_ON)) {
                    Bt_connectThread.write("22/" + throttleValue + "-");
                }
                Log.i(logTag, "Left");
            } finally {
                moveLeft_Handler.postDelayed(onMoveLeft, delayMsFix);
            }
        }
    };

    private Runnable onMoveRight = new Runnable() {
        @Override
        public void run() {
            try {
                if ((mrBtAdapter.getState() == BluetoothAdapter.STATE_ON)) {
                    Bt_connectThread.write("23/" + throttleValue + "-");
                }
                Log.i(logTag, "Right");
            } finally {
                moveRight_Handler.postDelayed(onMoveRight, delayMsFix);
            }
        }
    };

    private void startMoveFwd() {
        onMoveFwd.run();
    }

    private void stopMoveFwd() {
        moveFwd_Handler.removeCallbacks(onMoveFwd);
    }


    private void startMoveRev() {
        onMoveRev.run();
    }

    private void stopMoveRev() {
        moveRev_Handler.removeCallbacks(onMoveRev);
    }


    private void startMoveLeft() {
        onMoveLeft.run();
    }

    private void stopMoveLeft() {
        moveLeft_Handler.removeCallbacks(onMoveLeft);
    }


    private void startMoveRight() {
        onMoveRight.run();
    }

    private void stopMoveRight() {
        moveRight_Handler.removeCallbacks(onMoveRight);
    }


    /* ========================================================================================//
    ||          I N I T I A L I Z E   U S E R   I N T E R F A C E                              ||
    ===========================================================================================*/
    private void userInterfaceSetup() {
        // set activity title
        setTitle("Hexapod Controller v1 by R.J.L");

        moveFwd_Handler = new Handler();
        moveRev_Handler = new Handler();
        moveLeft_Handler = new Handler();
        moveRight_Handler = new Handler();

        swt_EnableBt = (Switch) findViewById(R.id.sw_enableBt);
        swt_connectBt = (Switch) findViewById(R.id.sw_disConnect_bt);

        spinDeviceList = (Spinner) findViewById(R.id.spn_pairedDevice);
        spinLegList = (Spinner) findViewById(R.id.spinLegSegment);

        txV_CurrentDevice = (TextView) findViewById(R.id.tv_selectedDevice);
        txV_setAngle_A = (TextView) findViewById(R.id.tvDispAngleA);
        txV_setAngle_B = (TextView) findViewById(R.id.tvDispAngleB);
        txV_setAngle_C = (TextView) findViewById(R.id.tvDispAngleC);
        txV_servNum_A = (TextView) findViewById(R.id.servNumA);
        txV_servNum_B = (TextView) findViewById(R.id.servNumB);
        txV_servNum_C = (TextView) findViewById(R.id.servNumC);

        txV_Cell1 = (TextView) findViewById(R.id.tv_Cell1);
        txV_Cell2 = (TextView) findViewById(R.id.tv_Cell2);

        txV_Throttle_value = (TextView) findViewById(R.id.txV_Throt_Val);

        for (int IdNum = 0; IdNum < txV_servo_ID_Angle.length; IdNum++) {
            txV_servo_ID_Angle[IdNum] = (TextView) findViewById(servo_IDs_[IdNum]);
        }

        btn_Throttle_Down = (Button) findViewById(R.id.btn_Throt_Dwn);
        btn_Throttle_Up = (Button) findViewById(R.id.btn_Throt_Up);

        btn_rst_pos = (Button) findViewById(R.id.btnRstPos);

        btn_FWD = (Button) findViewById(R.id.btnFWD);
        btn_REV = (Button) findViewById(R.id.btnREV);
        btn_STOP = (Button) findViewById(R.id.btnSTOP);
        btn_RIGHT = (Button) findViewById(R.id.btnRIGHT);
        btn_LEFT = (Button) findViewById(R.id.btnLEFT);

        btn_Decr_A = (Button) findViewById(R.id.btnDecrA);
        btn_Decr_B = (Button) findViewById(R.id.btnDecrB);
        btn_Decr_C = (Button) findViewById(R.id.btnDecrC);

        btn_Inc_A = (Button) findViewById(R.id.btnIncA);
        btn_Inc_B = (Button) findViewById(R.id.btnIncB);
        btn_Inc_C = (Button) findViewById(R.id.btnIncC);

        vsBar_SetAngle_A = (VertSeekBar) findViewById(R.id.vseekA);
        vsBar_SetAngle_B = (VertSeekBar) findViewById(R.id.vseekB);
        vsBar_SetAngle_C = (VertSeekBar) findViewById(R.id.vseekC);

        vsBar_SetAngle_A.setMax(180);
        vsBar_SetAngle_B.setMax(180);
        vsBar_SetAngle_C.setMax(180);

        pairedDevAdapter = new ArrayAdapter<>(getBaseContext(), android.R.layout.simple_spinner_dropdown_item, 0);
        ServoLegAdapter = new ArrayAdapter<>(getBaseContext(), android.R.layout.simple_spinner_dropdown_item, 0);

        // add something into the spinner to check if spinner is working propery
        pairedDevAdapter.add("sample 1");
        pairedDevAdapter.add("sample 2");
        pairedDevAdapter.add("sample 3");

        // add leg segments into the array adapter
        for (String aLegSegmentServoNum : LegSegmentServoNum) {
            ServoLegAdapter.add(aLegSegmentServoNum);
        }

        // set the contents of the spinner
        spinLegList.setAdapter(ServoLegAdapter);
        spinDeviceList.setAdapter(pairedDevAdapter);

        mrBtAdapter = BluetoothAdapter.getDefaultAdapter(); // get bluetooth adapter

        btSupportCheck();   // check if this device supports bluetooth

        btState = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                int btCurrentState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1);
                //  int prevBtState = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_STATE, -1);
                switch (btCurrentState) {
                    case (BluetoothAdapter.STATE_TURNING_ON): {
                        // Toast.makeText(getBaseContext(), "Bluetooth is turning ON", Toast.LENGTH_SHORT).show();
                        break;
                    }
                    case (BluetoothAdapter.STATE_ON): {
                        Toast.makeText(getBaseContext(), "Bluetooth is ON", Toast.LENGTH_SHORT).show();
                        getPairedBluetoothDevice();
                        enableControls();
                        break;
                    }
                    case (BluetoothAdapter.STATE_TURNING_OFF): {
                        // Toast.makeText(getBaseContext(), "Bluetooth is turning OFF", Toast.LENGTH_SHORT).show();
                        break;
                    }
                    case (BluetoothAdapter.STATE_OFF): {
                        Toast.makeText(getBaseContext(), "Bluetooth is OFF", Toast.LENGTH_SHORT).show();
                        disableControls();
                        break;
                    }
                    case (BluetoothAdapter.STATE_CONNECTED): {
                        Toast.makeText(getBaseContext(), "Bluetooth is Connected ", Toast.LENGTH_SHORT).show();
                        enableControls();
                        break;
                    }
                    case (BluetoothAdapter.STATE_DISCONNECTED): {
                        Toast.makeText(getBaseContext(), "Bluetooth Disconnected ", Toast.LENGTH_SHORT).show();
                        break;
                    }
                }
            }
        };

        btHandler = new Handler() {
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case RECIEVE_MESSAGE:                                             // if receive massage
                        byte[] readBuf = (byte[]) msg.obj;
                        String strIncom = new String(readBuf, 0, msg.arg1);           // create string from bytes array
                        sb.append(strIncom);                                          // append string
                        int endOfLineIndex = sb.indexOf("\r\n");                      // determine the end-of-line
                        if (endOfLineIndex > 0) {                                     // if end-of-line,
                            String sbprint = sb.substring(0, endOfLineIndex);         // extract string

                            try {
                                StringBreak(sbprint);
                                Log.i(logTag, sbprint);
                            } catch (NumberFormatException nfe) {
                                Toast.makeText(getBaseContext(), "Bluetooth Data Error", Toast.LENGTH_SHORT).show();
                            }
                            sb.delete(0, sb.length());                                        // and clear
                        }
                        //  Log.i(logTag, "...String:"+ sb.toString() );
                        break;
                }
            }
        };
    }

    /* ========================================================================================//
    || B R E A K   D O W N   I N C O M I N G   D A T A   F R O M   B L U E T O O TH            ||
    ===========================================================================================*/
    private void StringBreak(String content) {

        //set identifier for an incoming data
        parseString.onDataParse(content, "-", "/");
        // temp storage of values
        float val1, val2;


        // check if array size are equal
        if (parseString.CommandAddressSize() == parseString.CommandValueSize()) {

            // loop through the array and do the specified tasks
            for (int Cntr = 0; Cntr < parseString.CommandAddressSize(); Cntr++) {

                // if array is empty
                if ((parseString.CommandAddressList()[Cntr] != null) &&
                        (parseString.CommandValueList()[Cntr] != null)) {

                    // if array contains a data transfer it to val1 and val2
                    // val1 contains the address
                    val1 = Float.parseFloat(parseString.CommandAddressList()[Cntr]);
                    // val2 contains the value of the address
                    val2 = Float.parseFloat(parseString.CommandValueList()[Cntr]);
                    // Log.i(logTag, val1 + ":" + val2);
                    // cast float to direct assignment of values
                    // display the voltage of Cell 1
                    if (val1 == (float) 1.1) {
                        txV_Cell1.setText(Float.toString(val2) + " V");
                    } else if (val1 == (float) 2.1) {
                        // display the voltage of Cell 1
                        txV_Cell2.setText(Float.toString(val2) + " V");
                    }
                }
            }
        }
    }

    /* ========================================================================================//
    || C H E C K   B L U E T O O T H   S U P P O R T   O N   T H I S    D E V I C E            ||
    ===========================================================================================*/
    private void btSupportCheck() {
        // if there is no bluetooth support on thi device
        if (mrBtAdapter == null) {
            Toast.makeText(getBaseContext(), "This device does not support Bluetooth", Toast.LENGTH_LONG).show();
            swt_EnableBt.setClickable(false);
        } else {
            swt_EnableBt.setClickable(true);
            if (!mrBtAdapter.isEnabled()) {
                Toast.makeText(getBaseContext(), "Bluetooth is OFF", Toast.LENGTH_LONG).show();
            } else {
                swt_EnableBt.setChecked(true);
                Toast.makeText(getBaseContext(), "Bluetooth is ON", Toast.LENGTH_LONG).show();
                getPairedBluetoothDevice();
            }
        }
    }

    /*=========================================================================================//
    ||                G E T   P A I R E D   B L U E T O O T H   D E V I C E                    ||
    ===========================================================================================*/
    private void getPairedBluetoothDevice() {
        pairedDevAdapter.clear();
        Set<BluetoothDevice> setBtDevice = mrBtAdapter.getBondedDevices();
        if (setBtDevice.size() > 0) {
            for (BluetoothDevice btxDevice : setBtDevice) {
                pairedDevAdapter.add(btxDevice.getName() + "   " + btxDevice.getAddress());
            }
        }
    }

    /* ========================================================================================//
    ||           C R E A T E    B L U E T O O T H    S O C K E T                               ||
    ===========================================================================================*/
    private BluetoothSocket createBtSocket(BluetoothDevice btDevice) throws IOException {
        if (Build.VERSION.SDK_INT >= 10) {
            try {
                final Method m = btDevice.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", UUID.class);
                return (BluetoothSocket) m.invoke(btDevice, my_Bt_UUID);
            } catch (Exception e) {
                Log.e(logTag, "Could not create Insecure RFComm Connection", e);
            }
        }
        return btDevice.createRfcommSocketToServiceRecord(my_Bt_UUID);
    }

    /* ========================================================================================//
    ||           C O N N E C T    B L U E T O O T H    D E V I C E                             ||
    ===========================================================================================*/
    private void connectBluetoothDevice() {
        BluetoothDevice btDeviceConnect = mrBtAdapter.getRemoteDevice(myBtAddress);

        try {
            mrBtSocket = createBtSocket(btDeviceConnect);
        } catch (IOException ebt) {
            Log.i(logTag, "Something Wrong here - connect remote");
        }
        // discovery is resource intensive and must be disable
        mrBtAdapter.cancelDiscovery();
        Log.i(logTag, "Establishing Connection....");
        try {
            mrBtSocket.connect();
            Log.i(logTag, "Connected...");
        } catch (IOException Esocket) {
            try {
                mrBtSocket.close();
            } catch (IOException e) {
                Log.i(logTag, "FATAL ERROR: unable to close socket during " +
                        "connection failure - connectBtRemoteDevice() " + e.getMessage());
            }
        }
        Bt_connectThread = new BtThread(mrBtSocket);
        Bt_connectThread.start();
    }

    /* ========================================================================================//
    ||                        O N   R E S U M E   M E T H O D                                 ||
    ===========================================================================================*/

    @Override
    protected void onResume() {
        super.onResume();
        disableControls();
    }

    /* ========================================================================================//
    ||                         O N   C R E A T E   M E T H O D                               ||
    ===========================================================================================*/

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.hex_main_layout);

        userInterfaceSetup();


        // Listener for enabling bluetooth on the device
        swt_EnableBt.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean boolie) {
                if (boolie) {

                    IntentFilter btStateChange = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);

                    String eN_btIntent = BluetoothAdapter.ACTION_REQUEST_ENABLE;

                    registerReceiver(btState, btStateChange);
                    startActivityForResult(new Intent(eN_btIntent), 0);

                } else {
                    mrBtAdapter.disable();
                    // Toast.makeText(getBaseContext(), "Bluetooth is turned OFF", Toast.LENGTH_LONG).show();
                }
            }
        });

        // spinner listener for the paired devices
        spinDeviceList.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {

                String[] MAC_addr;  // create temporary string array for extracting MAC address

                // set the text view to the selected current device
                txV_CurrentDevice.setText(pairedDevAdapter.getItem(i));

                // check if the bluetooth is enabled
                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {

                    // refresh the list of paired device
                    getPairedBluetoothDevice();

                    // check if the list contains a 3 space
                    if (pairedDevAdapter.getItem(i).contains("   ")) {

                        // split the contents into two part and store it in a array
                        MAC_addr = pairedDevAdapter.getItem(i).split("   ");

                        // get the second element of the array which contain the mac address of the bluetooth
                        myBtAddress = MAC_addr[1];

                        Log.i(logTag, "Selected Bt Address ->> " + myBtAddress);
                        // txV_data.append("Selected Bt Address ->> " + myBtAddress + "\n");
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        // select leg segment to control manually
        spinLegList.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int pos, long l) {

                if (ServoLegAdapter.getItem(pos).equals("Servo 1-3")) {
                    txV_servNum_A.setText("S1");
                    txV_servNum_B.setText("S2");
                    txV_servNum_C.setText("S3");
                    ServoNum_A = 1;
                    ServoNum_B = 2;
                    ServoNum_C = 3;
                } else if (ServoLegAdapter.getItem(pos).equals("Servo 4-6")) {
                    txV_servNum_A.setText("S4");
                    txV_servNum_B.setText("S5");
                    txV_servNum_C.setText("S6");
                    ServoNum_A = 4;
                    ServoNum_B = 5;
                    ServoNum_C = 6;
                } else if (ServoLegAdapter.getItem(pos).equals("Servo 7-9")) {
                    txV_servNum_A.setText("S7");
                    txV_servNum_B.setText("S8");
                    txV_servNum_C.setText("S9");
                    ServoNum_A = 7;
                    ServoNum_B = 8;
                    ServoNum_C = 9;
                } else if (ServoLegAdapter.getItem(pos).equals("Servo 10-12")) {
                    txV_servNum_A.setText("S10");
                    txV_servNum_B.setText("S11");
                    txV_servNum_C.setText("S12");
                    ServoNum_A = 10;
                    ServoNum_B = 11;
                    ServoNum_C = 12;
                } else if (ServoLegAdapter.getItem(pos).equals("Servo 13-15")) {
                    txV_servNum_A.setText("S13");
                    txV_servNum_B.setText("S14");
                    txV_servNum_C.setText("S15");
                    ServoNum_A = 13;
                    ServoNum_B = 14;
                    ServoNum_C = 15;
                } else if (ServoLegAdapter.getItem(pos).equals("Servo 16-18")) {
                    txV_servNum_A.setText("S16");
                    txV_servNum_B.setText("S17");
                    txV_servNum_C.setText("S18");
                    ServoNum_A = 16;
                    ServoNum_B = 17;
                    ServoNum_C = 18;
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        swt_connectBt.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if (isChecked) {
                    if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                        swt_connectBt.setText("Connected");
                        connectBluetoothDevice();
                        enableControls();
                    }
                    // connect to the remote bluetooth device
                } else {
                    swt_connectBt.setText("Disconnected");
                    if (mrBtAdapter.getState() == BluetoothAdapter.STATE_OFF) {
                        disableControls();
                        Bt_connectThread.interrupt();
                    }
                }
            }
        });


        btn_FWD.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {

                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                    startMoveFwd();
                }

                if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    stopMoveFwd();
                }
                return true;
            }
        });

        btn_REV.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {

                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                    startMoveRev();
                }

                if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    stopMoveRev();
                }
                return true;
            }
        });


        btn_RIGHT.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                    startMoveRight();
                }
                if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    stopMoveRight();
                }
                return true;
            }
        });

        btn_LEFT.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                    startMoveLeft();
                }
                if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    stopMoveLeft();
                }
                return true;
            }
        });

        btn_STOP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                    Bt_connectThread.write("23/0-");
                }

            }
        });

        btn_Throttle_Up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                int val_A = Integer.parseInt((String) txV_Throttle_value.getText());
                val_A = val_A + 1;
                txV_Throttle_value.setText(Integer.toString(val_A));
                throttleValue = val_A;


                if (val_A > 10 ) {
                    val_A = 10;
                    txV_Throttle_value.setText(Integer.toString(val_A));
                    throttleValue = val_A;
                }
            }
        });

        btn_Throttle_Down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int val_B = Integer.parseInt((String) txV_Throttle_value.getText());
                val_B = val_B - 1;
                txV_Throttle_value.setText(Integer.toString(val_B));
                throttleValue = val_B;


                if (val_B < 1 ) {
                    val_B = 1;
                    txV_Throttle_value.setText(Integer.toString(val_B));
                    throttleValue = val_B;
                }
            }
        });


        btn_rst_pos.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                    Bt_connectThread.write("24/0-");
                }
            }
        });

        btn_Inc_A.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_A.setProgress(vsBar_SetAngle_A.getProgress() + 1);

            }
        });

        btn_Decr_A.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_A.setProgress(vsBar_SetAngle_A.getProgress() - 1);
            }
        });

        btn_Inc_B.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_B.setProgress(vsBar_SetAngle_B.getProgress() + 1);

            }
        });

        btn_Decr_B.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_B.setProgress(vsBar_SetAngle_B.getProgress() - 1);
            }
        });


        btn_Inc_C.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_C.setProgress(vsBar_SetAngle_C.getProgress() + 1);

            }
        });

        btn_Decr_C.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                vsBar_SetAngle_C.setProgress(vsBar_SetAngle_C.getProgress() - 1);
            }
        });

        vsBar_SetAngle_A.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                txV_setAngle_A.setText(Integer.toString(i));
                txV_servo_ID_Angle[ServoNum_A - 1].setText("S" + Integer.toString(ServoNum_A) + ": " + Integer.toString(i));

                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                    Bt_connectThread.write("-" + Integer.toString(ServoNum_A) + "/" + Integer.toString(i));
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        vsBar_SetAngle_B.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                txV_setAngle_B.setText(Integer.toString(i));
                txV_servo_ID_Angle[ServoNum_B - 1].setText("S" + Integer.toString(ServoNum_B) + ": " + Integer.toString(i));
                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                    Bt_connectThread.write("-" + Integer.toString(ServoNum_B) + "/" + Integer.toString(i));
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        vsBar_SetAngle_C.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                txV_setAngle_C.setText(Integer.toString(i));
                txV_servo_ID_Angle[ServoNum_C - 1].setText("S" + Integer.toString(ServoNum_C) + ": " + Integer.toString(i));
                if (mrBtAdapter.getState() == BluetoothAdapter.STATE_ON) {
                    Bt_connectThread.write("-" + Integer.toString(ServoNum_C) + "/" + Integer.toString(i));
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        stopMoveRev();
        stopMoveFwd();
    }

    /* ========================================================================================//
        ||           S T A R T     B L U E T O O T H      D I C O V E R Y                          ||
        ===========================================================================================*/
    private void startBluetoothDiscovery() {
        // this method is not used device must be paired in the android device settings

    }
   /* ========================================================================================//
    ||                         E N A B L E    C O N T R O L S                                  ||
    ===========================================================================================*/

    private void enableControls() {

    }

    /* ========================================================================================//
    ||                         D I S A B L E    C O N T R O L S                                ||
    ===========================================================================================*/
    private void disableControls() {

    }
}
