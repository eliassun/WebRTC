package com.voipeye.sip4webrtc;

public class AppLog {
    static final String TAG = "[sipsdk]AppLog";
    public static void logToJava(int type, int level, String msg) {
        Logger.newLog(type, level, msg);
    }
}
