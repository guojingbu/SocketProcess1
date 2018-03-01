package com.yesway.socketprocess;

/**
 * @author guojingbu
 */

public class Watcher {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public native void creatWatcher(String userId);
    public native void connectMonitor();




}
