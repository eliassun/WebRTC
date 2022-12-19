package com.voipeye.sip4webrtc;


public class Logger {
    static final String TAG = "[sipsdk]Logger";
    public static void newLog(int type, int level, String msg) {
        android.util.Log.i(TAG, msg);
    }
}
