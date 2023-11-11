/****************************************************************************
 Copyright (c) 2015 Ha Anh Man.
 ****************************************************************************/
package org.cocos2dx.lib;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Point;
import android.os.Build;
import android.text.Html;
import android.util.DisplayMetrics;
import android.util.Log;

import java.util.Calendar;
import java.text.Normalizer;
import java.util.regex.Pattern;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.view.Display;
import android.view.WindowManager;

//import com.crashlytics.android.Crashlytics;

import com.google.firebase.abt.FirebaseABTesting;
import com.google.firebase.crashlytics.FirebaseCrashlytics;

import org.cocos2dx.cpp.AppActivity;

class ConnectionDetector {

    /**
     * Checking for all possible internet providers
     * **/
    public static boolean isConnectingToInternet(Context context) {
        if (context == null) {
            throw new IllegalArgumentException("context can not be null.");
        }
        ConnectivityManager connectivity = (ConnectivityManager) context
                .getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivity != null) {
            NetworkInfo[] info = connectivity.getAllNetworkInfo();
            if (info != null) {
                for (NetworkInfo anInfo : info) {
                    if (anInfo.getState() == NetworkInfo.State.CONNECTED) {
                        return true;
                    }
                }
            }

        }
        return false;
    }
}

public class EduNativeBridge extends Activity {
    public static Cocos2dxActivity _activity = AppActivity._ac;
    static double mWidthPixels, mHeightPixels;
    public static boolean utf8StringCompare(final String str1, final String str2) {
        String my_str1 = str1.toLowerCase();
        String my_str2 = str2.toLowerCase();
//        Log.e("test", "str1: " + my_str1 + " - length: " + my_str1.length());
//        Log.e("test", "str2: " + my_str2 + " - length: " + my_str2.length());

        if(my_str1.equals(my_str2)) {
            return true;
        }
        return false;
    }

    public static int utf8LengthOfString(final String str) {
        return str.length();
    }

    public static String utf8characterAtIndex(final String str,final int index) {
        String character = String.valueOf(str.charAt(index));
        return character;
    }

    public static String toLowerCaseString(final String str) {
        Log.d("", "toLowerCaseString: " + str);
        return str.toLowerCase();
    }

    public static String toUpperCaseString(final String str) {
        return str.toUpperCase();
    }

    public static String hsFindString(final String subject_str, final String find_str){
        String _map = "";
        for (int i = -1; (i = subject_str.indexOf(find_str, i + 1)) != -1; ) {
            _map += i;
            _map += ",";
        }
// find all occurrences forward

        return _map;
    }

    public static String removeAccent(String s) {

        String temp = Normalizer.normalize(s, Normalizer.Form.NFD);
        Pattern pattern = Pattern.compile("\\p{InCombiningDiacriticalMarks}+");
        return pattern.matcher(temp).replaceAll("");
    }

    public static String hsSubString(final String str, final String indexChar, final String length) {
        int _index = new Integer(indexChar);
        int _length = new Integer(length);

        Log.d("", "hsSubString: " + _index + "|" + (_index+_length) +"|" + str);

        return str.substring(_index, _index+_length);
    }

    public static boolean networkAvaiable() {
        Log.d("hoisungcheck", "networkAvaiable");
        //_activity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
        Boolean isInternetPresent = ConnectionDetector.isConnectingToInternet(_activity.getApplicationContext());
        return isInternetPresent;
    }

    public static void sendEmail(final String subject, final String content, final String attach) {
        try {
            Intent i = new Intent(Intent.ACTION_SEND);
//            i.setType("message/rfc822");
            i.setType("plain/text");
            String[] to = {"support@monkeyjunior.com"};
            i.putExtra(Intent.EXTRA_EMAIL, to);
            i.putExtra(Intent.EXTRA_SUBJECT, subject);
            i.putExtra(Intent.EXTRA_TEXT, Html.fromHtml(content));

//            Uri URI = Uri.parse(attach);
//            Log.e("EduNativeBridge", URI.toString());
//            i.putExtra(Intent.EXTRA_STREAM, URI);
            _activity.startActivity(Intent.createChooser(i, "Select application"));
        } catch (Exception e) {
            Log.e("EduNativeBridge", e.getMessage());
        }
    }

    public static float getTimeZoneOffset() {
        Calendar calendar = Calendar.getInstance();
        float offset = (float)calendar.get(calendar.ZONE_OFFSET)/1000/3600.0f;
        Log.e("anhmantk", "offset: " + offset);
        return offset;
    }
    public static void setCrashlyticsKeyValue(String key, String value) {
        FirebaseCrashlytics.getInstance().setCustomKey(key, value);
    }

    public static double getInchDevice(){
        setRealDeviceSizeInPixels();
        DisplayMetrics dm = new DisplayMetrics();
        _activity.getWindowManager().getDefaultDisplay().getMetrics(dm);
        double x = Math.pow(mWidthPixels/dm.xdpi,2);
        double y = Math.pow(mHeightPixels/dm.ydpi,2);
        double screenInches = Math.sqrt(x+y);
        Log.d("debug","Screen inches : " + screenInches);
        return screenInches;
    }

    private static void setRealDeviceSizeInPixels() {
        WindowManager windowManager = _activity.getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics displayMetrics = new DisplayMetrics();
        display.getMetrics(displayMetrics);


        // since SDK_INT = 1;
        mWidthPixels = displayMetrics.widthPixels;
        mHeightPixels = displayMetrics.heightPixels;

        // includes window decorations (status bar/menu bar)
        if (Build.VERSION.SDK_INT >= 14 && Build.VERSION.SDK_INT < 17) {
            try {
                mWidthPixels = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
                mHeightPixels = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
            } catch (Exception ignored) {
            }
        }

        // includes window decorations (status bar/menu bar)
        if (Build.VERSION.SDK_INT >= 17) {
            try {
                Point realSize = new Point();
                Display.class.getMethod("getRealSize", Point.class).invoke(display, realSize);
                mWidthPixels = realSize.x;
                mHeightPixels = realSize.y;
            } catch (Exception ignored) {
            }
        }
    }
}
