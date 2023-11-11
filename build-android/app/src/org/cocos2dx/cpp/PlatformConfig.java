package org.cocos2dx.cpp;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.database.Cursor;
import android.media.MediaDrm;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.provider.Settings;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

import com.earlystart.android.monkeyjunior.story.BuildConfig;
import com.facebook.device.yearclass.YearClass;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.NetworkInterface;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.List;
import java.util.UUID;


/**
 * Created by tongoclinh on 5/23/17.
 * EarlyStart
 */

@SuppressWarnings("unused") //native
public class PlatformConfig {

    private static AppActivity app = AppActivity._ac;
    private static final String LOG_TAG = "Platform Config";
    private  static String URI_GSF_CONTENT_PROVIDER = "content://com.google.android.gsf.gservices";

    private static boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

    private static String getGSFID(Context context) {

        Uri uri = Uri.parse("content://com.google.android.gsf.gservices");
        String[] params = new String[] { "android_id" };

        try
        {
            Cursor query = context.getContentResolver().query(uri, null, null, params, null);

            if (query == null) {
                return "";
            }

            if (!query.moveToFirst() || query.getColumnCount() < 2) {
                query.close();
                return "";
            }

            try
            {
                String toHexString = Long.toHexString(Long.parseLong(query.getString(1)));
                query.close();
                return toHexString.toUpperCase().trim();
            }
            catch (Exception e)
            {
                query.close();
                return "";
            }

        } catch (SecurityException e) {
            e.printStackTrace();
            return "";
        } catch (Exception e2) {
            e2.printStackTrace();
            return "";
        }
    }

    private static  String getMediaDrmId() {

        String mediaDrmID = "";

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {

            UUID wideVineUuid = new UUID(-0x121074568629b532L, -0x5c37d8232ae2de13L);
            MediaDrm wvDrm = null;

            try
            {
                wvDrm = new MediaDrm(wideVineUuid);
                byte[] wideVineId = wvDrm.getPropertyByteArray(MediaDrm.PROPERTY_DEVICE_UNIQUE_ID);
                mediaDrmID =   Base64.encodeToString(wideVineId, Base64.NO_WRAP);

            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (wvDrm != null) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                        wvDrm.close();
                    } else {
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
                            wvDrm.release();
                        }
                    }
                }
            }
        }

        return  mediaDrmID;
    }

    @SuppressWarnings("unused") //native method
    static String getDeviceId() {

        String uuid = "";

        if (isExternalStorageWritable()) {
            Log.d(LOG_TAG, Environment.getExternalStorageDirectory().toString());
            String path = Environment.getExternalStorageDirectory().toString();
            File uuid_file = new File(path, ".monkey_junior_config.cached");
            try {
                if (uuid_file.exists()) {
                    InputStream is = new FileInputStream(uuid_file);
                    BufferedReader buf = new BufferedReader(new InputStreamReader(is));

                    String line = buf.readLine();
                    StringBuilder sb = new StringBuilder();

                    while(line != null){
                        sb.append(line).append("\n");
                        line = buf.readLine();
                    }

                    uuid = sb.toString();
                } else {
                    uuid = UUID.randomUUID().toString();
                    if (uuid_file.createNewFile()) {
                        FileOutputStream fOut = new FileOutputStream(uuid_file);
                        OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
                        myOutWriter.append(uuid);
                        myOutWriter.close();
                        fOut.flush();
                        fOut.close();
                    }
                }
            } catch (IOException e) {
                Log.e("Exception", "File write failed: " + e.toString());
            }
        }

        Log.d(LOG_TAG, uuid);

        String android_id = Settings.Secure.getString(app.getContentResolver(), Settings.Secure.ANDROID_ID);
        Log.d("Platform Config", android_id);

        String macaddr = "";


        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.Q) {

            String gsfID = getGSFID(app.getApplicationContext());
            String mediaDrmID = getMediaDrmId();

            macaddr = gsfID.isEmpty()?mediaDrmID:gsfID;

            Log.d("Platform|gsfID", gsfID);
            Log.d("Platform|mediaDrmID", mediaDrmID);
        }
        else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            WifiManager manager = (WifiManager) app.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            WifiInfo info = manager.getConnectionInfo();
            macaddr = info.getMacAddress();
        } else  {
            try {
                String interfaceName = "wlan0";
                List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
                for (NetworkInterface intf : interfaces) {
                    if (!intf.getName().equalsIgnoreCase(interfaceName)) {
                        continue;
                    }

                    byte[] mac = intf.getHardwareAddress();
                    if (mac == null) {
                        macaddr = "";
                    } else {
                        StringBuilder buf = new StringBuilder();
                        for (byte aMac : mac) {
                            buf.append(String.format("%02X:", aMac));
                        }
                        if (buf.length() > 0) {
                            buf.deleteCharAt(buf.length() - 1);
                        }
                        macaddr = buf.toString();
                        break;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } // for now eat exceptions
        }
        Log.d("Platform Config", macaddr);

        macaddr = (macaddr.equals("02:00:00:00:00:00") ? "" : macaddr);

        macaddr = macaddr + "|" + android_id + "|" + uuid;

        Log.d("Platform Config F", macaddr);

        return macaddr;
    }

    static String getAppVersion() {
        return BuildConfig.VERSION_NAME;
//        try {
//            PackageInfo pInfo = app.getPackageManager().getPackageInfo(app.getPackageName(), 0);
//            return String.valueOf(pInfo.versionName);
//        } catch (PackageManager.NameNotFoundException e) {
//            e.printStackTrace();
//            return "0";
//        }
    }

    static String getDeviceInfoShort() {
        String info = getAppVersion() + "|";
        info += Build.VERSION.RELEASE + "|";
        info += Build.MANUFACTURER
                + "|" + Build.MODEL + "|" + Build.VERSION.RELEASE
                + "|" + Build.VERSION_CODES.class.getFields()[android.os.Build.VERSION.SDK_INT].getName();

        return info;
    }

    static String getDeviceInfo() {
        String info = "Application Version: " + getAppVersion() + "\n";
        info += "OS: Android\n";
        info += "Model: " + Build.MANUFACTURER
                + " " + Build.MODEL + "\n";
        info += "System Version: " + Build.VERSION.RELEASE + Build.VERSION_CODES.class.getFields()[android.os.Build.VERSION.SDK_INT].getName();

        return info;
    }

    static void phoneSupport(String phoneNumber) {
        Intent intent = new Intent(Intent.ACTION_DIAL);
        intent.setData(Uri.parse("tel:" + phoneNumber));
        app.startActivity(intent);
    }

    static void mailSupport(String subject, String body, String email)
    {
        Intent i = new Intent(Intent.ACTION_SENDTO);
        i.setType("text/plain");
        i.putExtra(Intent.EXTRA_EMAIL  , new String[]{email});
        i.putExtra(Intent.EXTRA_SUBJECT, subject);
        i.putExtra(Intent.EXTRA_TEXT   , body);
        i.setData(Uri.parse("mailto:"));
        try
        {
            AppActivity._ac.startActivity(Intent.createChooser(i, "Send mail..."));
        }
        catch (android.content.ActivityNotFoundException ex) {
            Toast.makeText(AppActivity._ac, "There are no email clients installed.", Toast.LENGTH_SHORT).show();
        }
    }

    static String getAppName() {
        try {
            PackageManager pm = app.getPackageManager();
            ApplicationInfo appInfo = pm.getApplicationInfo(app.getPackageName(), 0);
            String label = pm.getApplicationLabel(appInfo).toString();
            return label;
        } catch (NameNotFoundException e) {
            return "";
        }
    }

    static String getPackageName() {
        return BuildConfig.APPLICATION_ID;
    }

    static int getYearClass() {
        return YearClass.get(app);
    }

    static boolean isAppInstalled(String appName) {

        boolean found = true;

        try {

            app.getPackageManager().getPackageInfo(appName, 0);
        } catch (PackageManager.NameNotFoundException e) {

            found = false;
        }

        return found;
    }

    static void shareFacebook(String url){

        ShareLinkContent content = new ShareLinkContent.Builder()
                .setContentUrl(Uri.parse(url))
                .setQuote("Con mình đang học tiếng Anh trên Monkey Stories và mình thấy rất hữu ích. Bạn có thể đăng ký để học miễn phí theo link ở đây:")

                .build();
        ShareDialog.show(app, content);
    }

    static boolean saveScreenShot(String url){
        if(app != null){
            Log.d("android","shareScreenShot: vào nào vào nào");
            app.saveScreenshot(url);

            return true;
        }
        return false;
    }
}
