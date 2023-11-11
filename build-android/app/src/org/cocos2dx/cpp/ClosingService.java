package org.cocos2dx.cpp;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

import androidx.annotation.Nullable;

public class ClosingService extends Service {

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        super.onTaskRemoved(rootIntent);

        // Handle application closing
        //fireClosingNotification();
        try
        {
            AudiobookHelper.stop();
        }
        catch (Exception e)
        {

        }

        // Destroy the service
        stopSelf();
    }
}