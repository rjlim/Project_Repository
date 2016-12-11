package com.example.pc01.hexapodcontrolv1;

import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import static com.example.pc01.hexapodcontrolv1.HexMainCtrl.btHandler;


/**
 * Created by pc01 on 10/08/2016.
 */
public class BtThread extends Thread{
    private InputStream btInStream ;
    private OutputStream btOutStream ;

    private byte[] buffer = new byte[256];  // buffer store for the stream
    private int bytes; // bytes returned from read()

    public BtThread(BluetoothSocket btSocket) {

        try {
            btInStream = btSocket.getInputStream();
            btOutStream = btSocket.getOutputStream();
        } catch (IOException iOe) {
            btInStream = null;
            btOutStream = null;
        }
    }

    public void run() {

        // Keep listening to the InputStream until an exception occurs
        while (true) {
            try {
                // Read from the InputStream
                bytes = btInStream.read(buffer);        // Get number of bytes and message in "buffer"
                btHandler.obtainMessage(1, bytes, -1, buffer).sendToTarget();		// Send to message queue Handler
            } catch (IOException e) {
                break;
            }
        }
    }

    /* Call this from the main activity to send data to the remote device */
    public void write(String message) {
        Log.d(" ", "...Data to send: " + message + "...");
        byte[] msgBuffer = message.getBytes();
        try {
            btOutStream.write(msgBuffer);
        } catch (IOException e) {
            Log.d(" ", "...Error data send: " + e.getMessage() + "...");
        }
    }
}
