package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

public class NotificationReceive extends BroadcastReceiver {
    @Override
    public void onReceive (Context context, Intent intent)
    {
        String message = intent.getStringExtra("Monkey stories");
        Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
    }
}
