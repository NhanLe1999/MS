package org.cocos2dx.cpp;

import static android.app.Notification.VISIBILITY_PUBLIC;
import static android.content.Context.ALARM_SERVICE;

import android.annotation.SuppressLint;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.RequiresApi;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import com.earlystart.android.monkeyjunior.story.R;

import java.util.Arrays;
import java.util.Vector;

public class ReminderBroadcast extends BroadcastReceiver {

    @RequiresApi(api = Build.VERSION_CODES.O)
    @SuppressLint("ResourceType")
    @Override
    public void onReceive(Context context, Intent intent)
    {
        String title = "";
        String message = "";
        Bundle bundle = intent.getExtras();
         long idChanal = bundle.getLong("idNotification");
        boolean muchNotification = bundle.getBoolean("muchNotification");

        title = bundle.getString("title");
        message = bundle.getString("message");
        createNotificationAppinbox(context, title, message, idChanal);
        if(muchNotification)
        {
            String [] ArrayTitle = bundle.getStringArray("listTitle");
            String [] ArrayMessage = bundle.getStringArray("listMessage");

            Vector<String> vectorTitle = new Vector<String>(20);
            Vector<String> vectorMessage = new Vector<String>(20);

            for(int i = 0; i < ArrayTitle.length; i++)
            {
                vectorTitle.add(ArrayTitle[i]);
                vectorMessage.add(ArrayMessage[i]);
            }

            setNotificationContinue(context, vectorTitle, vectorMessage);
        }

    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public void createNotificationAppinbox(Context context, String title, String message, long idChanal)
    {
        try
        {
            AudioManager amanager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            amanager.setStreamMute(AudioManager.STREAM_NOTIFICATION, false);
        } catch (Exception e) {
            e.printStackTrace();
        }
        String title1 = title;
        String message1 = message;
        Intent activityIntent = new Intent(context, org.cocos2dx.cpp.AppActivity.class);
        final int flag =  Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE : PendingIntent.FLAG_UPDATE_CURRENT;

        PendingIntent contentIntent = PendingIntent.getActivity(context, 0, activityIntent, flag);

        Intent broadcastIntent = new Intent(context, NotificationReceive.class);
        broadcastIntent.putExtra("Nhanne", message1);
        PendingIntent actionIntent = PendingIntent.getBroadcast(context, 0, broadcastIntent, flag);

        AudioAttributes audioAttributes = new AudioAttributes.Builder()
                .setContentType(AudioAttributes. CONTENT_TYPE_SONIFICATION )
                .setUsage(AudioAttributes. USAGE_ALARM )
                .build() ;
        Uri defaultSoundUri = Uri.parse(ContentResolver.SCHEME_ANDROID_RESOURCE + "://" + context.getPackageName() + "/" + R.raw.monkey);

        @SuppressLint("WrongConstant") Notification builder = new NotificationCompat.Builder(context, title)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(title1)
                .setContentText(message1)
                .setPriority(NotificationCompat.PRIORITY_HIGH)
                .setCategory(NotificationCompat.CATEGORY_MESSAGE)
                .setContentIntent(contentIntent)
                .setAutoCancel(true)
                .setOnlyAlertOnce(true)
                .setNumber(1)
                .setSound(defaultSoundUri)
                .setVisibility(VISIBILITY_PUBLIC)
                .build();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
        {
            CharSequence name = "Monkey stories";
            String description = "Toi yeu monkey";
            int importance = NotificationManager.IMPORTANCE_HIGH;
            NotificationChannel channel = new NotificationChannel(title, name, importance);
            channel.setDescription(description);
            channel.setSound(defaultSoundUri, audioAttributes);

            NotificationManager notificationManager = context.getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }

        NotificationManagerCompat notification = NotificationManagerCompat.from(context);
        notification.notify((int) idChanal, builder);
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public void setNotificationContinue(Context context, Vector<String> title, Vector<String> message)
    {
        
        if(title.isEmpty()||message.isEmpty()) {
            return;
        }

        long timeAlarm = System.currentTimeMillis();
        long tenSeconds = 1000 * 10;
        long idNotification = timeAlarm + tenSeconds;

        Intent intent = new Intent(context, ReminderBroadcast.class);

        intent.putExtra("title", title.get(0));
        intent.putExtra("message", message.get(0));
        intent.putExtra("muchNotification", title.size() > 1);
        //
        String text = title.remove(0);
        String text1 = message.remove(0);
        intent.putExtra("idNotification", idNotification);
        ///
        if(title.size() > 0)
        {
            Object[] objArrayTitle = title.toArray();
            Object[] objArrayMessage = message.toArray();

            String[] arrayStringTitle = Arrays.copyOf(objArrayTitle, objArrayTitle.length, String[].class);
            String[] arrayStringMessage = Arrays.copyOf(objArrayMessage, objArrayMessage.length, String[].class);

            intent.putExtra("listTitle", arrayStringTitle);
            intent.putExtra("listMessage", arrayStringMessage);
        }
        final int flag =  Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE : PendingIntent.FLAG_UPDATE_CURRENT;
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, 0, intent, flag);

        AlarmManager alarmManager = (AlarmManager) context.getSystemService(ALARM_SERVICE);

        alarmManager.setAndAllowWhileIdle(AlarmManager.RTC_WAKEUP, timeAlarm + tenSeconds, pendingIntent);
    }

}
