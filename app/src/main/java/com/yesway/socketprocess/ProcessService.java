package com.yesway.socketprocess;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.support.annotation.Nullable;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

/**
 * @author guojingbu
 * 关键服务被守护对象
 */

public class ProcessService extends Service {
    public static final String TAG = "ProcessService";
    private int i;
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Watcher watcher = new Watcher();
        watcher.creatWatcher(String.valueOf(Process.myUid()));
        watcher.connectMonitor();
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.i(TAG, "-----------run------: 服务开启中 "+i);
                i++;
            }
        },0,1000*3);
    }
}
