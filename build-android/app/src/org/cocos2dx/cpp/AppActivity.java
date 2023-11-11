/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.ComponentName;

import android.provider.Settings;
import android.speech.RecognizerIntent;
import android.Manifest;
import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.ActivityNotFoundException;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.location.Location;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.provider.MediaStore;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.core.content.ContextCompat;

import android.speech.SpeechRecognizer;
import android.util.DisplayMetrics;
import android.util.Log;
import com.clevertap.android.sdk.ActivityLifecycleCallback;
import com.clevertap.android.sdk.CTInboxListener;
import com.clevertap.android.sdk.inbox.CTInboxMessage;
import com.clevertap.android.sdk.product_config.CTProductConfigController;
import com.clevertap.android.sdk.pushnotification.CTPushNotificationListener;
import com.clevertap.android.sdk.CleverTapAPI;
import com.clevertap.android.sdk.InAppNotificationButtonListener;
import com.clevertap.android.sdk.InAppNotificationListener;
import com.clevertap.android.sdk.SyncListener;
import com.clevertap.android.sdk.displayunits.DisplayUnitListener;
import com.clevertap.android.sdk.displayunits.model.CleverTapDisplayUnit;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.earlystart.android.monkeyjunior.ai_speaking.camera_manager.media_activity.CameraActivity;
import com.earlystart.android.monkeyjunior.story.FullscreenActivity;
import com.earlystart.android.monkeyjunior.story.R;
import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookAuthorizationException;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.LoggingBehavior;

import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;

import com.google.android.gms.auth.api.phone.SmsRetrieverClient;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.common.api.ApiException;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.android.material.textfield.TextInputEditText;
import com.google.firebase.crashlytics.FirebaseCrashlytics;
import com.google.firebase.dynamiclinks.FirebaseDynamicLinks;
import com.google.firebase.dynamiclinks.PendingDynamicLinkData;
//import com.google.firebase.iid.FirebaseInstanceId;
//import com.google.firebase.iid.InstanceIdResult;
import com.google.firebase.messaging.MessageForwardingService;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.hbisoft.hbrecorder.HBRecorder;
import com.hbisoft.hbrecorder.HBRecorderListener;
import com.squareup.okhttp.Callback;
import com.squareup.okhttp.FormEncodingBuilder;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import java.io.File;
import java.io.IOException;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;
/*------------------------appplyer------------------------*/
import com.appsflyer.AppsFlyerLib;
import com.appsflyer.deeplink.DeepLink;
import com.appsflyer.deeplink.DeepLinkListener;
import com.appsflyer.deeplink.DeepLinkResult;
import com.google.android.gms.auth.api.phone.SmsRetriever;
/*--------------------------------------------------------*/
import static com.earlystart.android.monkeyjunior.story.R.string.GOOGLE_SECRET;
import static com.earlystart.android.monkeyjunior.story.R.string.GOOGLE_SERVER_CLIENT_ID;
import static com.earlystart.android.monkeyjunior.story.R.string.camera_activity;
import static com.earlystart.android.monkeyjunior.story.R.string.exo_track_unknown;
import static org.cocos2dx.cpp.SpeechRecognizerFragment.StartListening;

@SuppressWarnings("JniMissingFunction")
public class AppActivity extends Cocos2dxActivity implements HBRecorderListener, CTInboxListener,
        InAppNotificationButtonListener, DisplayUnitListener, InAppNotificationListener, CTPushNotificationListener {

    private static final int PERMISSION_REQUEST_CODE = 666;
    public static AppActivity _ac;
    static int countNumberNotification = 0;
    private static final int CONTENT_VIEW_ID = 10101010;

    IAPHelper _iapHelper;
    private boolean key_build_Huawei = false;
    private static final String TAG = "MainActivity";
    private static final int PERMISSION_CODE = 4793;
    private int mScreenDensity;
    private MediaProjectionManager mProjectionManager;
    private int mDisplayWidth;
    private int mDisplayHeight;
    private MediaProjection mMediaProjection;
    private VirtualDisplay mVirtualDisplay;
    private MediaProjectionCallback mMediaProjectionCallback;
    private MediaRecorder mMediaRecorder;
    private boolean mRecording;

    private NotificationManagerCompat notificationManagerAppinbox;
    private EditText editTextTitle;
    private EditText editTextMessage;

    private String mFilePath;

    private static final String cameraPerm = Manifest.permission.CAMERA;
    private String readedDeepLink = "";
    private String myDeepLink = "";
    public static final String CHANNEL_ID = "Notification appinbox";
    private static Vector<String> titleAppinbox = new Vector<String>(20);
    private static Vector<String> messageAppinbox = new Vector<String>(20);

    private static Context mContext;
    private static boolean isChanggeIcon = true;

    public static boolean s_isFullScreenActivityPause = false;

    public static Context getContext() {
        return mContext;
    }

    private static final int REQUEST_RECORD_AUDIO_PERMISSION_SCREEN = 200;
    private static final int REQUEST_RECORD_AUDIO_PERMISSION_MAX = 201;
    private static final int REQUEST_USE_CAMERA_PERMISSION = 202;
    private static final int REQUEST_RECORD_IMAGE_PERMISSION_MAX = 203;
    private static final int REQUEST_SPEECH_TO_TEXT = 10;

    private static final int REQ_USER_CONSENT = 909;
    SmsReceiver smsBroadcastReceiver;
    TextInputEditText etOTP;

    private boolean permissionToRecordAccepted = false;
    private boolean permissionToWriteStorage = false;
    public boolean permissionToUseCamera = false;
    private String[] cameraPermissions = { Manifest.permission.CAMERA };


    // screen recoder
    private static HBRecorder hbRecorder = null;

    private static final int SCREEN_RECORD_REQUEST_CODE = 777;
    private static final int PERMISSION_REQ_ID_RECORD_AUDIO = 22;
    private static final int PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE = PERMISSION_REQ_ID_RECORD_AUDIO + 1;

    private boolean hasPermissions = false;

    private CleverTapAPI clevertapDefaultInstance = null;
    private static CleverTapAPI cleverTapDefaultInstance1 = null;

    static protected native void onDestroyCallback();

    static protected native void onAddNotificationAppinbox();

    static protected native String getStringLanguageForKey(String key);

    static protected native void getDataObjectArray(String[] key);

    static protected native int getCountry();

    static protected native int getTimeStamp();

    static protected native void setInForKeyChangeIcon(int valueContry);

    static protected native int getInForKeyChangeIcon();

    static protected native boolean getBoolForKeyChangeIcon();

    static protected native void getMessageOTP(String message);
    static protected native void pushEnvetLostConnectionToClevertab(String [] value);

    static protected native void onRequestSucceed_sec(String message);

    private String pendingDeeplinkUrl = "";
    private String text = "";
    public static String[] dataObjectArray;
    private Bundle savedInstanceState;


    //SPEECH RECOGNIZER
    static public SpeechRecognizer sr;
    static public SpeechRecognizerFragment.SpeechRecognitionListener speechListener = new SpeechRecognizerFragment.SpeechRecognitionListener();

    static private boolean _disableStartRecordGameProScore = false;
    static public void setDisableStartRecordingGameProscore() {
        _disableStartRecordGameProScore = true;
    }

    protected void onCreate(Bundle savedInstanceState) {

        // PermissionManager.check(this, Manifest.permission.RECEIVE_SMS,
        // SMS_REQUEST_CODE);

        ActivityLifecycleCallback.register(getApplication());

        super.setEnableVirtualButton(false);

        super.onCreate(savedInstanceState);

        this.savedInstanceState = savedInstanceState;
        // Workaround in
        // https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root acApptivity into an existing
            // task
            // so just quietly finish and go away, dropping the user back into the activity
            // at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }

        // Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
        _ac = this;
        mContext = this;
        FirebaseCrashlytics crashlytics = FirebaseCrashlytics.getInstance();
        crashlytics.setCrashlyticsCollectionEnabled(true);

        AudiobookHelper.initMediaSessionCompat();

        permissionToRecordAccepted = getPackageManager().checkPermission(Manifest.permission.RECORD_AUDIO,
                getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
        {
            permissionToWriteStorage = getPackageManager().checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;

        }else{
            permissionToWriteStorage = true;
        }
        permissionToUseCamera = getPackageManager().checkPermission(cameraPerm,
                getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;

        if (!key_build_Huawei) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    _iapHelper = IAPHelper.getInstance();
                }
            });
        }

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        mDisplayWidth = metrics.widthPixels;
        mDisplayHeight = metrics.heightPixels;
        mScreenDensity = metrics.densityDpi;

        mRecording = false;

        try {
            getGLSurfaceView().setMultipleTouchEnabled(false);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (Build.VERSION.SDK_INT >= 21) {
            mProjectionManager = (MediaProjectionManager) getSystemService(Context.MEDIA_PROJECTION_SERVICE);
            mMediaProjectionCallback = new MediaProjectionCallback();
        }

        // tracking for the closing application.

        // register notification chanel for >= android 8
        // if we not register , the notification will not works
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // Configure the notification channel.
            NotificationChannel channel = new NotificationChannel(
                    AudiobookHelper.CHANNEL_ID,
                    "Playlist Notification Channel",
                    NotificationManager.IMPORTANCE_DEFAULT);
            channel.setDescription("Playlist Notification Channel");
            channel.enableLights(true);
            channel.setVibrationPattern(new long[] { 0 });// disable vibration
            channel.enableVibration(true);

            NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            notificationManager.createNotificationChannel(channel);

            // startForegroundService(new Intent(this, ClosingService.class));
        } else {
            // startService(new Intent(this, ClosingService.class));
        }

        // screen recoder only support android 5+
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            // Init HBRecorder
            hbRecorder = new HBRecorder(this, this);
            hbRecorder.setVideoEncoder("H264");
            hbRecorder.isAudioEnabled(true);
            hbRecorder.setOutputPath(
                    Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES).getAbsolutePath());
            hbRecorder.setAudioBitrate(128000);
            hbRecorder.setAudioSamplingRate(44100);
        }

        CleverTapAPI cleverTapAPI = CleverTapAPI.getDefaultInstance(getApplicationContext());

        CTProductConfigController productConfigInstance = cleverTapAPI.productConfig();

        clevertapDefaultInstance = CleverTapAPI.getDefaultInstance(getApplicationContext());

        if (clevertapDefaultInstance != null) {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {

                String sound = "monkey.mp3", channelId = "EarlyStart", channelName = "EarlyStart";

                {
                    // scope for auto delete
                    // tmp fix bug push noti channel sound
                    // register channel with sound
                    NotificationManager notificationManager = (NotificationManager) this
                            .getSystemService(NOTIFICATION_SERVICE);
                    String soundfile = "";
                    Uri soundUri = null;
                    if (!sound.isEmpty()) {
                        if (sound.contains(".mp3") || sound.contains(".ogg") || sound.contains(".wav")) {
                            soundfile = sound.substring(0, (sound.length() - 4));
                        }
                        if (!soundfile.isEmpty()) {
                            soundUri = Uri.parse(ContentResolver.SCHEME_ANDROID_RESOURCE + "://" + this
                                    .getPackageName() + "/raw/" + soundfile);
                        }

                        NotificationChannel notificationChannel = new NotificationChannel(channelId,
                                channelName,
                                NotificationManager.IMPORTANCE_HIGH);
                        notificationChannel.setShowBadge(true);
                        if (soundUri != null) {
                            notificationChannel.setSound(soundUri,
                                    new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_NOTIFICATION)
                                            .build());
                        }
                        notificationManager.createNotificationChannel(notificationChannel);
                    }
                }
                clevertapDefaultInstance.createNotificationChannel(getApplicationContext(), channelId, channelName,
                        "Monkey is the best education app for children.", NotificationManager.IMPORTANCE_MAX, true,
                        sound);
            }

            clevertapDefaultInstance.setCTPushNotificationListener(this);
            clevertapDefaultInstance.setInAppNotificationButtonListener(this);

            clevertapDefaultInstance.setInAppNotificationListener(this);
            // clevertapDefaultInstance.setDisplayUnitListener(this);

            CleverTapAPI.setDebugLevel(CleverTapAPI.LogLevel.DEBUG); // Set Log level to DEBUG log warnings or other
                                                                     // important messages

            clevertapDefaultInstance.setCTNotificationInboxListener(this);
            clevertapDefaultInstance.initializeInbox();
        }

        parseShortLink("");

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Intent intent = getIntent();

        String action = intent.getAction();

        Uri data = intent.getData();

        if (action != null && data != null) {

            String url = data.toString();

            if (url.contains("monkeystories.vn")) {
                pendingDeeplinkUrl = url;
            }
        }

        mute();
        // startService(new Intent(this, YourService.class));
        /*
         * 
         * EditText editText = (EditText) findViewById(R.string.text_message_appinbox);
         * this.registerForContextMenu(editText);
         */
        AppsFlyerLib appsflyer = AppsFlyerLib.getInstance();
        appsflyer.subscribeForDeepLink(new DeepLinkListener() {
            @Override
            public void onDeepLinking(@NonNull DeepLinkResult deepLinkResult) {
                //monkey note: this implementation currently not processing any task in flow of mj

                DeepLinkResult.Status dlStatus = deepLinkResult.getStatus();
                if (dlStatus == DeepLinkResult.Status.FOUND) {
                    Log.d("appsflyer", "Deep link found");
                } else if (dlStatus == DeepLinkResult.Status.NOT_FOUND) {
                    Log.d("appsflyer", "Deep link not found");
                    return;
                } else {
                    // dlStatus == DeepLinkResult.Status.ERROR
                    DeepLinkResult.Error dlError = deepLinkResult.getError();
                    Log.d("appsflyer", "There was an error getting Deep Link data: " + dlError.toString());
                    return;
                }
                DeepLink deepLinkObj = deepLinkResult.getDeepLink();
                try {
                    Log.d("appsflyer", "The DeepLink data is: " + deepLinkObj.toString());
                } catch (Exception e) {
                    Log.d("appsflyer", "DeepLink data came back null");
                    return;
                }
                // An example for using is_deferred
                if (deepLinkObj.isDeferred()) {
                    Log.d("appsflyer", "This is a deferred deep link");
                } else {
                    Log.d("appsflyer", "This is a direct deep link");
                }
            }
        });

        /*
        Uri uri = this.getIntent().getData();
        if (uri != null) {
            onReciveDeppLink(uri.toString());
        }*/
        startSmartUserConsent();

        //Log.d(TAG, "StartListeningCalled");
        sr = SpeechRecognizer.createSpeechRecognizer(this);
        sr.setRecognitionListener(speechListener);

    }

    private void startSmartUserConsent() {

        SmsRetrieverClient client = SmsRetriever.getClient(this);
        if (client != null) {
            client.startSmsUserConsent(null);
        }
    }

    private void getOtpFromMessage(String message) {

        Pattern otpPattern = Pattern.compile("(|^)\\d{6}");
        Matcher matcher = otpPattern.matcher(message);
        if (matcher.find()) {

            etOTP.setText(matcher.group(0));
        }
    }

    private void closeBroadcastReceiver() {
        unregisterReceiver(smsBroadcastReceiver);
    }

    private void registerBroadcastReceiver() {

        smsBroadcastReceiver = new SmsReceiver();

        smsBroadcastReceiver.smsBroadcastReceiverListener = new SmsReceiver.SmsBroadcastReceiverListener() {
            @Override
            public void onSuccess(Intent intent) {

                startActivityForResult(intent, REQ_USER_CONSENT);

            }

            @Override
            public void onFailure() {

            }
        };

        IntentFilter intentFilter = new IntentFilter(SmsRetriever.SMS_RETRIEVED_ACTION);
        registerReceiver(smsBroadcastReceiver, intentFilter);

    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public void startRecordingScreen() {
        MediaProjectionManager mediaProjectionManager = (MediaProjectionManager) getSystemService(
                Context.MEDIA_PROJECTION_SERVICE);
        Intent permissionIntent = mediaProjectionManager != null ? mediaProjectionManager.createScreenCaptureIntent()
                : null;
        startActivityForResult(permissionIntent, SCREEN_RECORD_REQUEST_CODE);
    }

    public void requestCameraPermission() {
        ActivityCompat.requestPermissions(AppActivity._ac, cameraPermissions, REQUEST_USE_CAMERA_PERMISSION);
    }

    public static boolean checkPermissionNotificationAndroid()
    {
        return NotificationManagerCompat.from(AppActivity._ac.getApplicationContext()).areNotificationsEnabled();
    }

    public static void  requestPermissionNotificationAndroid()
    {
        _ac.requestPermissionNotification();
    }

    public void requestPermissionNotification()
    {
        Context context = AppActivity._ac.getApplicationContext();
        Intent intent = new Intent();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            intent.setAction(Settings.ACTION_APP_NOTIFICATION_SETTINGS);
            intent.putExtra(Settings.EXTRA_APP_PACKAGE, context.getPackageName());
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
            intent.setAction("android.settings.APP_NOTIFICATION_SETTINGS");
            intent.putExtra("app_package", context.getPackageName());
            intent.putExtra("app_uid", context.getApplicationInfo().uid);
        } else {
            intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
            intent.addCategory(Intent.CATEGORY_DEFAULT);
            intent.setData(Uri.parse("package:" + context.getPackageName()));
        }
        startActivity(intent);
    }



    private boolean hasAllPermissionsGranted(@NonNull int[] grantResults) {
        for (int grantResult : grantResults) {
            if (grantResult == PackageManager.PERMISSION_DENIED) {
                return false;
            }
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case REQUEST_RECORD_AUDIO_PERMISSION_SCREEN:
                for (int i = 0; i < permissions.length; i++) {
                    if (permissions[i].equals(Manifest.permission.RECORD_AUDIO)) {
                        permissionToRecordAccepted = (grantResults[i] == PackageManager.PERMISSION_GRANTED);
                    } else if (permissions[i].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                        permissionToWriteStorage = (grantResults[i] == PackageManager.PERMISSION_GRANTED);
                    }
                }

                MSRecord.permissionToRecordAccepted = permissionToRecordAccepted;

                if (permissionToRecordAccepted && permissionToWriteStorage) {
                    shareScreen();
                } else {
                    toastOnUIThread(isVietnameseDisplay() ? "Một số quyền bị từ chối! Không thể bắt đầu ghi!"
                            : "Some permissions are denied ! Cannot start recorder !");
                }
                break;
            case REQUEST_RECORD_AUDIO_PERMISSION_MAX:
                if (grantResults.length == 2) {
                    MSRecord.permissionToRecordAccepted = (grantResults[0] == PackageManager.PERMISSION_GRANTED
                            && grantResults[1] == PackageManager.PERMISSION_GRANTED);
                    permissionToRecordAccepted = (grantResults[0] == PackageManager.PERMISSION_GRANTED
                            && grantResults[1] == PackageManager.PERMISSION_GRANTED);
                }

                if(grantResults.length == 1)
                {
                    MSRecord.permissionToRecordAccepted = grantResults[0] == PackageManager.PERMISSION_GRANTED;
                    permissionToRecordAccepted = MSRecord.permissionToRecordAccepted;
                }

                if (MSRecord.permissionToRecordAccepted) {
                    if(_disableStartRecordGameProScore == false)
                    {
                        //MSRecord.startRecording();
                    }
                    _disableStartRecordGameProScore = false;
                } else {
                    final int contry = getCountry();
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(AppActivity._ac,
                                    contry == 1
                                            ? "Unable to access microphone, please check system settings.Open Settings > Apps->Monkey Stories->Permissions- > Microphone > Monkey -> Allow."
                                            : contry == 2
                                                    ? "คุณยังไม่เปิดอนุญาตให้แอปพลิเคชันใช้ไมโครโฟนไปที่การตั้งค่า -> แอพ ->Monkey Stories -> สิทธิ์ -> ไมโครโฟน -> อนุญาต"
                                                    : contry == 3
                                                            ? "Anda belum mengijinkan penggunaan micro. Masuk Pengaturan -> Aplikasi-> Monkey Stories -> Izin->Mikrofon->Izinkan."
                                                            : contry == 4
                                                                    ? "Bạn chưa cấp phép cho ứng dụng sử dụng micro. Vào Cài đặt -> Ứng dụng-> Monkey Stories ->Quyền -> Micro-> Cho phép."
                                                                    : "Bạn chưa cấp phép cho ứng dụng sử dụng micro. Vào Cài đặt -> Ứng dụng-> Monkey Stories ->Quyền -> Micro-> Cho phép."

                                    , Toast.LENGTH_SHORT).show();
                        }
                    });
                }
                break;
            case REQUEST_USE_CAMERA_PERMISSION:
                permissionToUseCamera = hasAllPermissionsGranted(grantResults);
                if (permissionToUseCamera) {
                    startScan(10);
                } else {
                    QRReaderHelper.onScanClosed();
                }
                break;
            case REQUEST_RECORD_IMAGE_PERMISSION_MAX:
                permissionToWriteStorage = hasAllPermissionsGranted(grantResults);
                // permissionToWriteStorage = true;
                saveScreenshot(screenshotURL);
                break;
            case PERMISSION_REQ_ID_RECORD_AUDIO:
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
                    {
                        checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE);
                    }
                } else {
                    hasPermissions = false;
                    String msg = isVietnameseDisplay() ? "Bạn không cấp quyền cho " : "No permission for ";
                    msg += Manifest.permission.RECORD_AUDIO;
                    toastOnUIThread(msg);
                }
                break;
            case PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE:
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    hasPermissions = true;
                    // Permissions was provided
                    // Start screen recording
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        startRecordingScreen();
                        // onStartRecording();
                    }
                } else {
                    hasPermissions = false;

                    String msg = isVietnameseDisplay() ? "Bạn không cấp quyền cho " : "No permission for ";
                    msg += Manifest.permission.WRITE_EXTERNAL_STORAGE;
                    toastOnUIThread(msg);
                }
                break;
        }
    }

    public void rateMe() {
        try {
            startActivity(new Intent(Intent.ACTION_VIEW,
                    Uri.parse("market://details?id=" + getPackageName())));
        } catch (ActivityNotFoundException e) {
            startActivity(new Intent(Intent.ACTION_VIEW,
                    Uri.parse("http://play.google.com/store/apps/details?id=" + getPackageName())));
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public boolean initRecorder() {
        if (!permissionToRecordAccepted || !permissionToWriteStorage) {

            String [] recordAudioPermissions;

            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
            {
                recordAudioPermissions = new String[] { Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE };
            }else{
                recordAudioPermissions = new String[] { Manifest.permission.RECORD_AUDIO};
            }
                ActivityCompat.requestPermissions(this, recordAudioPermissions, REQUEST_RECORD_AUDIO_PERMISSION_SCREEN);
            return false;
        }

        if (isRecording()) {
            stopRecordAndStart();
            // stopRecord();
            return false;
        }

        if (mMediaRecorder != null) {

            mFilePath = getFilePath();
            mMediaRecorder.setOutputFile(mFilePath);
            return true;
        }

        mMediaRecorder = new MediaRecorder();
        mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);
        mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
        mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
        mMediaRecorder.setVideoEncodingBitRate(2000000);
        mMediaRecorder.setVideoFrameRate(30);

        // int mVideoHeight = Math.min(720, mDisplayHeight);
        // int mVideoWidth = (int)(mVideoHeight * 1.0 / mDisplayHeight * mDisplayWidth);
        // Log.d("XXX","mVideoWidth:"+mVideoWidth+"x"+mVideoHeight);
        // Log.d("XXX","mDisplayWidth:"+mDisplayWidth+"x"+mDisplayHeight);

        mMediaRecorder.setVideoSize(mDisplayWidth, mDisplayHeight);
        mFilePath = getFilePath();
        mMediaRecorder.setOutputFile(mFilePath);

        return true;
    }

    public String getFilePath() {
        final String directory = Environment.getExternalStorageDirectory() + File.separator + "Recordings";
        if (!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            Toast.makeText(this, "Failed to get External Storage", Toast.LENGTH_SHORT).show();
            return null;
        }
        final File folder = new File(directory);
        boolean success = true;
        if (!folder.exists()) {
            success = folder.mkdirs();
        }
        String filePath;
        if (success) {
            String videoName = ("capture_" + getCurSysDate() + ".mp4");
            filePath = directory + File.separator;// + videoName;
        } else {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(AppActivity._ac, "Failed to create Recordings directory", Toast.LENGTH_SHORT).show();
                }
            });
            return null;
        }
        return filePath;
    }

    public String getCurSysDate() {
        return new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(new Date());
    }

    private boolean prepareRecorder() {
        try {
            mMediaRecorder.prepare();
            return true;
        } catch (IllegalStateException | IOException e) {
            e.printStackTrace();

            toastOnUIThread(isVietnameseDisplay() ? "Vùi lòng chờ 3s và nhấn để thử lại !"
                    : "Please wait 3s and press the button to try again !");

            return false;
        }
    }

    // FACEBOOK LOGIN
    public static CallbackManager mCallbackManager;

    protected static native void onFacebookFailed();

    protected static native void onFacebookLoginFinished(String token, String email);

    protected native void onFacebookLoginFailed();

    static void loginFacebook() {
        //FacebookSdk.sdkInitialize(AppActivity._ac.getApplicationContext());
        FacebookSdk.setIsDebugEnabled(true);
        FacebookSdk.addLoggingBehavior(LoggingBehavior.INCLUDE_ACCESS_TOKENS);
        mCallbackManager = CallbackManager.Factory.create();
        LoginManager.getInstance().registerCallback(mCallbackManager,
                new FacebookCallback<LoginResult>() {
                    @Override
                    public void onSuccess(final LoginResult loginResult) {
                        Log.d("Success", "Login");
                        GraphRequest request = GraphRequest.newMeRequest(
                                loginResult.getAccessToken(),
                                new GraphRequest.GraphJSONObjectCallback() {
                                    @Override
                                    public void onCompleted(
                                            JSONObject object,
                                            GraphResponse response) {
                                        // Application code
                                        Log.v("LoginActivity", object.toString());
                                        String token = String.valueOf(AccessToken.getCurrentAccessToken().getToken());
                                        try {
                                            String email = object.getString("email").toString();
                                            onFacebookLoginFinished(token, email);
                                        } catch (JSONException e) {
                                            onFacebookLoginFinished(token, "");
                                        }
                                    }
                                });
                        Bundle parameters = new Bundle();
                        parameters.putString("fields", "id,name,email,gender, birthday");
                        request.setParameters(parameters);
                        request.executeAsync();

                    }

                    @Override
                    public void onCancel() {
                        onFacebookFailed();
                        Log.d("Fail", "Login cancel");

                    }

                    @Override
                    public void onError(FacebookException exception) {
                        if (exception instanceof FacebookAuthorizationException) {
                            if (AccessToken.getCurrentAccessToken() != null) {
                                LoginManager.getInstance().logOut();
                            }
                        }
                        onFacebookFailed();
                        Log.d("Fail", "Login" + exception.toString());
                    }

                });
        LoginManager.getInstance().logInWithReadPermissions(AppActivity._ac, Arrays.asList("public_profile", "email"));

    }

    protected void onSpeechToTextResult(String messenge){
        onRequestSucceed_sec(messenge);
        Log.e(TAG, messenge );
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_SPEECH_TO_TEXT){

            if(resultCode == RESULT_OK && data != null){
                ArrayList<String> result = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
                String output = result.get(0);
                onRequestSucceed_sec(output);
            }
        }

        if (requestCode == REQ_USER_CONSENT) {

            if ((resultCode == RESULT_OK) && (data != null)) {

                String message = data.getStringExtra(SmsRetriever.EXTRA_SMS_MESSAGE);
                getMessageOTP(message);
            }

        }

        if (requestCode == SCREEN_RECORD_REQUEST_CODE) {
            mRecording = (resultCode == RESULT_OK);

            if (resultCode == RESULT_OK) {
                hbRecorder.startScreenRecording(data, resultCode);

                onStartRecording();
            } else {
                Toast.makeText(this, isVietnameseDisplay() ? "Bạn cần cấp quyền quay phim màn hình !"
                        : "You need to grant screen record permission !", Toast.LENGTH_SHORT).show();
            }

            return;
        }

        if (FacebookSdk.isFacebookRequestCode(requestCode)) {
            if (resultCode == Activity.RESULT_OK) {
                mCallbackManager.onActivityResult(requestCode, resultCode, data);
            } else {
                onFacebookFailed();
            }
        }

        if (requestCode == MJHelper.RC_GET_TOKEN) {

            // The Task returned from this call is always completed, no need to attach
            // a listener.
            Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
            try {
                GoogleSignInAccount account = task.getResult(ApiException.class);
                Log.d("signinresult", "SUCCESS");
                Log.d("email", account.getEmail());
                // onGoogleSignInFinished(account.getIdToken(), account.getEmail());

                final String account_email = account.getEmail();
                OkHttpClient client = new OkHttpClient();
                RequestBody requestBody = new FormEncodingBuilder()
                        .add("grant_type", "authorization_code")
                        .add("client_id", AppActivity._ac.getString(GOOGLE_SERVER_CLIENT_ID)) // "903853614382-t2ej1rhshogjtf7ks1rcav5b54ovvd6k.apps.googleusercontent.com")
                                                                                              // //
                        .add("client_secret", AppActivity._ac.getString(GOOGLE_SECRET)) // "8K_NTICKdEqYVFotWYSES4vK")
                                                                                        // //.add("client_secret",
                                                                                        // "RbpuF1Z60aRI4l-yL1PmylkL")
                        .add("redirect_uri", "")
                        .add("code", account.getServerAuthCode())
                        .build();
                final Request request = new Request.Builder()
                        .url("https://www.googleapis.com/oauth2/v4/token")
                        .post(requestBody)
                        .build();

                client.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onFailure(final Request request, final IOException e) {
                        Log.e("get access token fail", e.toString());
                        onGoogleSignInFailed();
                    }

                    @Override
                    public void onResponse(Response response) throws IOException {
                        try {
                            JSONObject jsonObject = new JSONObject(response.body().string());
                            final String message = jsonObject.toString(5);
                            Log.i("get accesstoken success", message);
                            if (jsonObject.has("access_token")) {
                                String access_token = jsonObject.getString("access_token");
                                Log.e("access token", access_token);
                                // SignIn successfull
                                onGoogleSignInFinished(access_token, account_email);
                            } else {
                                onGoogleSignInFailed();
                            }
                        } catch (JSONException e) {
                            onGoogleSignInFailed();
                            e.printStackTrace();
                        }
                    }
                });

            } catch (ApiException e) {
                // The ApiException status code indicates the detailed failure reason.
                Log.w(TAG, "signInResult:failed code=" + e.getMessage());
                onGoogleSignInFailed();
            }
        }

        if (requestCode != PERMISSION_CODE) {
            Log.e(TAG, "Unknown request code: " + requestCode);
            super.onActivityResult(requestCode, resultCode, data);
            return;
        }

        if (resultCode != RESULT_OK) {

            Toast.makeText(this, isVietnameseDisplay() ? "Bạn cần cấp quyền quay phim màn hình !"
                    : "You need to grant screen record permission !", Toast.LENGTH_SHORT).show();
            mRecording = false;
            super.onActivityResult(requestCode, resultCode, data);
            return;
        }

        mRecording = true;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            mMediaProjection = mProjectionManager.getMediaProjection(resultCode, data);
            mMediaProjection.registerCallback(mMediaProjectionCallback, null);
            mVirtualDisplay = createVirtualDisplay();
        }
        mMediaRecorder.start();
        onStartRecording();
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onStart() {
        super.onStart();

        AudiobookHelper.onStart();

        Intent intent = getIntent();

        if (intent != null) {
            String action = intent.getAction();
            Uri data = intent.getData();

            if (action != null && data != null) {
                // Log.d("MSStories", "intent action" + action +
                // data.getQueryParameter("status"));
                // String status = data.getQueryParameter("status");
            }
        }
    }

    @Override
    public void onStop() {
        super.onStop();
        AudiobookHelper.onStop();
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private VirtualDisplay createVirtualDisplay() {
        VirtualDisplay v = mMediaProjection.createVirtualDisplay("MainActivity",
                mDisplayWidth, mDisplayHeight, mScreenDensity,
                DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                mMediaRecorder.getSurface(), null /* Callbacks */, null /* Handler */);
        if (v == null) {
            Log.d(TAG, "Cannot create virtual display");
        }

        return v;
    }

    // Check if permissions was granted
    private boolean checkSelfPermission(String permission, int requestCode) {
        if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[] { permission }, requestCode);
            return false;
        }
        return true;
    }

    void shareScreen() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            // first check if permissions was granted
            if (checkSelfPermission(Manifest.permission.RECORD_AUDIO, PERMISSION_REQ_ID_RECORD_AUDIO)) {
                hasPermissions = true;
            }

            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE))
            {
                hasPermissions = true;
            }

            if (hasPermissions) {
                if (hbRecorder.isBusyRecording()) {
                    // toastOnUIThread("video dang quay nen stop nhe!");

                    hbRecorder.stopScreenRecording();
                }

                if (hbRecorder != null) {
                    hbRecorder.setOutputFormat("DEFAULT");
                    hbRecorder.setFileName("capture_" + getCurSysDate());
                }

                toastOnUIThread(isVietnameseDisplay() ? "Bắt đầu quay video..." : "the screen recorder is starting...");
                startRecordingScreen();
                onStartRecording();
            } else {
                // toastOnUIThread(isVietnameseDisplay()?"Một số quyền bị từ chối! Không thể bắt
                // đầu ghi!":"Some permissions are denied ! Cannot start recorder !");
            }
        } else {
            toastOnUIThread(isVietnameseDisplay()
                    ? "Thiết bị của bạn không được hỗ trợ ghi màn hình !Tính năng này yêu cầu android 5+ ! "
                    : "Your device is not supported screen recording ! This feature requires android 5+!");
            // showLongToast("This library requires API 21>");
        }

        // boolean init = initRecorder();
        // if (init) {
        // boolean prepared = prepareRecorder();
        // if (prepared) {
        // if (mMediaProjection == null) {
        // startActivityForResult(mProjectionManager.createScreenCaptureIntent(),
        // PERMISSION_CODE);
        // return;
        // }
        // mRecording = true;
        // mVirtualDisplay = createVirtualDisplay();
        // mMediaRecorder.start();
        // onStartRecording();
        // } else {
        // mMediaRecorder = null;
        // }
        // }
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    void stopRecord() {
        mRecording = false;

        if (hbRecorder != null) {
            if (hbRecorder.isBusyRecording()) {
                hbRecorder.stopScreenRecording();

                String msg = (isVietnameseDisplay()
                        ? "Tệp ghi hình đã được lưu vào Bộ sưu tập!\n" + hbRecorder.getFilePath()
                        : "File is saved to Gallery\n" + hbRecorder.getFilePath());

                showVideoPathPopup(msg);

                toastOnUIThread(msg, Toast.LENGTH_LONG);
            }
            // stopScreenSharing();
        } else if (hbRecorder == null) {
            toastOnUIThread(
                    isVietnameseDisplay() ? "Không thể ghi âm. Mã lỗi 3592 \n" : "Cannt record. Error code 3592\n",
                    Toast.LENGTH_LONG);
        } else if (hbRecorder != null) {

        }
        // else
        // {
        // stoptoastOnUIThread( isVietnameseDisplay()?"Không có trình quay phim màn hình
        // nào được chạy !":"Screen recorder is not running!");
        // }

        // on some devices , we cannot record screen video as config at initRecorder
        // function ! ( ex: vivo v9 ) Here is explanation !
        // If we change config, may be works ! But it's dangerous at the moment !
        // so, we will take a look at it if we have a lot of free time
        // https://stackoverflow.com/questions/51332386/mediarecorder-and-videosource-surface-stop-failed-1007-a-serious-android-bug?noredirect=1&lq=1
        // com.earlystart.android.monkeyjunior.story E/MediaRecorder: stop failed: -1007
        // runOnUiThread(new Runnable() {
        // @Override
        // public void run() {
        // if (mRecording) {
        // mRecording = false;
        // String msg = "";
        //
        // //add try-catch to fix crash on un-support devices ( ex vivo v9 ).
        // try
        // {
        // mMediaRecorder.stop();
        // mMediaRecorder.reset();
        // }
        // catch (Exception ex)
        // {
        // msg = "Sop failed ! Your device is not supported screen recording !";
        // }
        //
        // if(msg.isEmpty())
        // {
        // //Log.v(TAG, "Recording Stopped");
        // msg = "File is saved to Gallery";
        // if (isVietnameseDisplay())
        // {
        // msg = "Tệp ghi hình đã được lưu vào Bộ sưu tập!";
        // }
        // }
        //
        // Toast.makeText(AppActivity._ac, msg, Toast.LENGTH_SHORT).show();
        //
        // }
        // stopScreenSharing();
        // }
        // });
    }

    void stopRecordAndStart() {
        runOnUiThread(new Runnable() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void run() {
                if (mRecording) {
                    mRecording = false;
                    mMediaRecorder.stop();
                    mMediaRecorder.reset();
                    Log.v(TAG, "Recording Stopped");
                    String msg = "File is saved to Gallery";
                    if (isVietnameseDisplay()) {
                        msg = "Tệp ghi hình đã được lưu vào Bộ sưu tập!";
                    }
                    Toast.makeText(AppActivity._ac, msg, Toast.LENGTH_SHORT).show();
                }
                stopScreenSharing();
                shareScreen();
            }
        });
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    void stopScreenSharing() {
        if (mVirtualDisplay == null) {
            return;
        }
        mVirtualDisplay.release();

        ContentValues values = new ContentValues();
        values.put(MediaStore.Video.Media.DATA, mFilePath);
        ContentResolver resolver = getContentResolver();
        resolver.insert(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, values);
        // mMediaRecorder.release();
    }

    // @Override
    // protected void onResume() {
    // super.onResume();
    // qrEader.initAndStart(mQRSurfaceView);
    // }
    //
    // @Override
    // protected void onPause() {
    // super.onPause();
    // qrEader.releaseAndCleanup();
    // }

    @Override
    public void onDestroy() {
        DownloadFile.disConnectDownload();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            showNotificationForAppinbox();
        }
        onDestroyCallback();
        super.onDestroy();
         if (mMediaProjection != null) {
         mMediaProjection.stop();
        mMediaProjection = null;
         }

        AudiobookHelper.onStop();
         AudiobookHelper.scheduleOff(0);
        System.exit(0);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    boolean isRecording() {
        if (hbRecorder == null) {
            return false;
        }
        return hbRecorder.isBusyRecording();
    }

    @Override
    public void onPointerCaptureChanged(boolean hasCapture) {

    }

    @Override
    public void inboxDidInitialize() {

    }

    @Override
    public void inboxMessagesDidUpdate() {

    }

    // android inapp notification.
    @Override
    public void onInAppButtonClick(HashMap<String, String> hashMap) {

        if (hashMap != null && hashMap.containsKey("deeplink")) {

            String url = hashMap.get("deeplink");
            pendingDeeplinkUrl = url;
            processQRCode(url);

        }
    }

    @Override
    public void onDisplayUnitsLoaded(ArrayList<CleverTapDisplayUnit> units) {
        for (int i = 0; i < units.size(); i++) {
            CleverTapDisplayUnit unit = units.get(i);
            // prepareDisplayView(unit);
        }
    }

    @Override
    public boolean beforeShow(Map<String, Object> extras) {
        return true;
    }

    @Override
    public void onDismissed(Map<String, Object> extras, @Nullable Map<String, Object> actionExtras) {

        if (actionExtras != null && actionExtras.containsKey("deeplink")) {

            String url = actionExtras.get("deeplink").toString();

            pendingDeeplinkUrl = url;

            processQRCode(url);
        }
    }

    @Override
    public void onNotificationClickedPayloadReceived(HashMap<String, Object> actionExtras) {

        if (actionExtras != null) {

            for (Map.Entry<String, Object> entry : actionExtras.entrySet()) {

                Object value = entry.getValue();

                if (value != null) {

                    String url = value.toString();

                    if (url.contains("monkeystories.vn")) {

                        pendingDeeplinkUrl = url;

                        processQRCode(url);

                        break;
                    }
                }
            }
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private class MediaProjectionCallback extends MediaProjection.Callback {
        @Override
        public void onStop() {
            if (mRecording) {
                mRecording = false;
                mMediaRecorder.stop();
                mMediaRecorder.reset();
                Log.v(TAG, "Recording Stopped");
            }
            mMediaProjection = null;
            stopScreenSharing();
            Log.i(TAG, "MediaProjection Stopped");
        }
    }

    protected native void onDebugEpoch(int value);

    protected native void onOnepaySuccess();

    protected native void onOnepayFailure();

    protected native void onStartRecording();

    protected native boolean isVietnameseDisplay();

    protected native void onAccountKitFinished(String token, String phonenumber);

    protected native void onAccountKitFailed();

    protected native void onGoogleSignInFinished(String token, String email);

    protected native void onGoogleSignInFailed();

    protected native void showVideoPathPopup(String msg);

    @Override
    protected void onResume() {
        super.onResume();

        parseShortLink("");

        if (s_isFullScreenActivityPause) {
            // Log.d("FullAc|startScan", "onResume");
            startScan(99);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    public void startScan(int buttonTop) {

        if (!permissionToUseCamera) {
            // QRReaderHelper.onScanClosed();
            toastOnUIThread(isVietnameseDisplay() ? "Bạn cần cấp quyền truy xuất vào camera !"
                    : "You need to grant permission for the camera !");
            requestCameraPermission();
            return;
        }
        // qrButtonTop = buttonTop;
        shouldPause = false;
        s_isFullScreenActivityPause = false;
        Intent intent = new Intent(this, FullscreenActivity.class);
        // intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        startActivity(intent);
    }

    public void stopScan() {
        shouldPause = true;
    }

    public String getReadedDeepLink() {
        // Log.d(TAG, "Get :" + readedDeepLink);
        return readedDeepLink;

    }

    public void clearDeepLinkData() {
        getIntent().setData(null);
        getIntent().replaceExtras(new Bundle());
    }

    public void parseShortLinkQRCameraCode(final String i_QRCodeLink) {
        onCameraQRCodeCallback(i_QRCodeLink);
    }

    public void parseShortLink(final String i_QRCodeLink)
    {
        if(i_QRCodeLink.isEmpty())
        {
            FirebaseDynamicLinks.getInstance()
                    .getDynamicLink(getIntent())
                    .addOnSuccessListener(this, new OnSuccessListener<PendingDynamicLinkData>() {
                        @Override
                        public void onSuccess(PendingDynamicLinkData pendingDynamicLinkData)
                        {
                            // Get deep link from result (may be null if no link is found)
                            Uri deepLink = null;

                            if (pendingDynamicLinkData != null)
                            {
                                deepLink = pendingDynamicLinkData.getLink();
                            }

                            if (deepLink != null)
                            {
                                //xu ly deeplink tu click vao notification,click vao deeplink... ma khong phai la tu quet QR code.
                                processQRCode(deepLink.toString());
                            }

                            Log.d("QREader", "parseShortLink|myDeepLink:" + myDeepLink);

                        }
                    })
                    .addOnFailureListener(this, new OnFailureListener()
                    {
                        @Override
                        public void onFailure(@NonNull Exception e)
                        {
                            Log.d("QREader", "parseShortLink|ErrmyDeepLink:" + myDeepLink);
                        }
                    });
        }
        else
        {
            FirebaseDynamicLinks.getInstance()
                    .getDynamicLink(Uri.parse(i_QRCodeLink))
                    .addOnSuccessListener(this, new OnSuccessListener<PendingDynamicLinkData>() {

    @Override
                        public void onSuccess(PendingDynamicLinkData pendingDynamicLinkData)
                        {
                            // Get deep link from result (may be null if no link is found)
                            Uri deepLink = null;

                            if (pendingDynamicLinkData != null)
                            {
                                deepLink = pendingDynamicLinkData.getLink();
                            }

                            myDeepLink = ((deepLink != null) ? deepLink.toString() :  "error");

                            Log.d("QREader", "parseShortLink|myDeepLink:" + myDeepLink);

                            //xu ly deeplink tu quet qr code
                            QRReaderHelper.onDeeplinkHandled(myDeepLink);
                        }
                    })
                    .addOnFailureListener(this, new OnFailureListener()
                    {
                        @Override
                        public void onFailure(@NonNull Exception e)
                        {
                            Log.d("QREader", "parseShortLink|ErrmyDeepLink:" + myDeepLink);

                            myDeepLink = "error";

                            //xu ly deeplink tu quet qr code
                            QRReaderHelper.onDeeplinkHandled(myDeepLink);
                        }
                    });
        }

    }

    public String getDeepLink() {
        return myDeepLink;
    }

    public void cleanDeepLink() {
        myDeepLink = "";
    }

    public void cleanReadedDeepLink() {
        // Log.d(TAG, "Clean :" + readedDeepLink);
        readedDeepLink = "";
    }

    // The key in the intent's extras that maps to the incoming message's message
    // ID. Only sent by
    // the server, GmsCore sends EXTRA_MESSAGE_ID_KEY below. Server can't send that
    // as it would get
    // stripped by the client.
    private static final String EXTRA_MESSAGE_ID_KEY_SERVER = "message_id";

    // An alternate key value in the intent's extras that also maps to the incoming
    // message's message
    // ID. Used by upstream, and set by GmsCore.
    private static final String EXTRA_MESSAGE_ID_KEY = "google.message_id";

    // The key in the intent's extras that maps to the incoming message's sender
    // value.
    private static final String EXTRA_FROM = "google.message_id";

    @Override
    protected void onNewIntent(Intent intent) {
        if (intent != null) {
            super.onNewIntent(intent);
            // If we do not have a 'from' field this intent was not a message and should not
            // be handled. It
            // probably means this intent was fired by tapping on the app icon.
            Bundle extras = intent.getExtras();
            if (extras != null) {
                String from = extras.getString(EXTRA_FROM);
                String messageId = extras.getString(EXTRA_MESSAGE_ID_KEY);
                if (messageId == null) {
                    messageId = extras.getString(EXTRA_MESSAGE_ID_KEY_SERVER);
                }
                if (from != null && messageId != null) {
                    Intent message = new Intent(this, MessageForwardingService.class);
                    if (message != null) {
                        message.setAction(MessageForwardingService.ACTION_REMOTE_INTENT);
                        message.putExtras(intent);
                        startService(message);
                    }
                }
            }
            if(intent != null)
            {
                setIntent(intent);
            }

            //deeplink android
            /*{
                Intent intentDeeplink = getIntent();
                if(intentDeeplink != null)
                {
                    String action = intentDeeplink.getAction();
                    Uri data = intentDeeplink.getData();
                    if(data != null)
                    {
                        String url = data.toString();
                        processQRCode(url);
                        String kk = url;
                    }
                }
            }*/
        }

    }

    private String screenshotURL = "";

    public void saveScreenshot(String url) {
        screenshotURL = url;
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (permissionToWriteStorage) {
                    MediaStore.Images.Media.insertImage(getContentResolver(), BitmapFactory.decodeFile(screenshotURL),
                            "Vmonkey_screenshot", "");
                    String msg = "File is saved to Gallery";
                    if (isVietnameseDisplay()) {
                        msg = "Ảnh đã được lưu vào Bộ sưu tập!";
                    }
                    Toast.makeText(AppActivity._ac, msg, Toast.LENGTH_SHORT).show();

                } else {
                    String[] permission = { Manifest.permission.WRITE_EXTERNAL_STORAGE };
                    ActivityCompat.requestPermissions(AppActivity._ac, permission, REQUEST_RECORD_IMAGE_PERMISSION_MAX);
                }
            }
        });
    }

    private void toastOnUIThread(final String i_string, final int i_timeLength) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(AppActivity._ac, i_string, i_timeLength).show();
            }
        });
    }

    public void toastOnUIThread(final String i_string) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(AppActivity._ac, i_string, Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void HBRecorderOnStart() {

    }

    @Override
    public void HBRecorderOnComplete() {
        // This is called once the file was created
    }

    @Override
    public void HBRecorderOnError(int errorCode, String reason) {

    }

    @Override
    public void HBRecorderOnPause() {

    }

    @Override
    public void HBRecorderOnResume() {

    }

    public boolean isVietnameseLanguage() {
        return isVietnameseDisplay();
    }

    private void setClevertapLocation(Map<String, Object> newProfile) {
        if (newProfile.isEmpty()) {
            return;
        }

        if (newProfile.containsKey("country") && newProfile.containsKey("latitude")
                && newProfile.containsKey("longitude")) {
            String country = (String) newProfile.get("country");
            double latitude = (double) newProfile.get("latitude");
            double longitude = (double) newProfile.get("longitude");

            Location location = new Location(country);
            location.setLatitude(latitude);
            location.setLongitude(longitude);

            Log.d("Clevertap|pushProfile:", "set location" + location.toString());

            CleverTapAPI.getDefaultInstance(getApplicationContext()).setLocation(location);
        }
    }

    private Map<String, Object> removeEmptyCleverTapData(Map<String, Object> newProfile) {
        if (newProfile.isEmpty()) {
            return newProfile;
        }

        if (newProfile.containsKey("Name")) {
            String name = (String) newProfile.get("Name");

            if (name.isEmpty()) {
                newProfile.remove("Name");
            }
        }

        if (newProfile.containsKey("parent_name")) {
            String name = (String) newProfile.get("parent_name");

            if (name.isEmpty()) {
                newProfile.remove("parent_name");
            }
        }

        if (newProfile.containsKey("device_name")) {
            String name = (String) newProfile.get("device_name");

            if (name.isEmpty()) {
                newProfile.remove("device_name");
            }
        }

        if (newProfile.containsKey("parent_id")) {
            String name = (String) newProfile.get("parent_id");

            if (name.isEmpty()) {
                newProfile.remove("parent_id");
            }
        }

        if (newProfile.containsKey("type")) {
            String name = (String) newProfile.get("type");

            if (name.equals("child") && newProfile.containsKey("number_of_children")) {
                newProfile.remove("number_of_children");
            }
        }

        return newProfile;
    }

    private void pushProfile(final String i_jsonString, boolean i_isUpdate) {
        if (clevertapDefaultInstance == null || i_jsonString.isEmpty() || !isJSONObjectValid(i_jsonString)) {
            Log.d("Clevertap|pushProfile", " cannot pushProfile");

            return;
        }

        Map<String, Object> newProfile = new Gson().fromJson(
                i_jsonString, new TypeToken<HashMap<String, Object>>() {
                }.getType());

        newProfile = removeEmptyCleverTapData(newProfile);

        if (newProfile.isEmpty()) {
            return;
        }

        if (i_isUpdate) {
            CleverTapAPI.getDefaultInstance(getApplicationContext()).pushProfile(newProfile);

            setClevertapLocation(newProfile);
        } else {
            SyncListener listener1 = new SyncListener() {
                @Override
                public void profileDataUpdated(JSONObject updates) {

                }

                @Override
                public void profileDidInitialize(String CleverTapID) {
                    Map<String, Object> profileUpdateData = new Gson().fromJson(
                            i_jsonString, new TypeToken<HashMap<String, Object>>() {
                            }.getType());

                    Log.d("Clevertap|pushProfile:",
                            "called callback|HashMap update data:" + profileUpdateData.toString());

                    profileUpdateData = removeEmptyCleverTapData(profileUpdateData);

                    if (profileUpdateData.isEmpty()) {
                        return;
                    }

                    CleverTapAPI.getDefaultInstance(getApplicationContext()).pushProfile(profileUpdateData);

                    setClevertapLocation(profileUpdateData);

                    onCleverTapCallback();
                }
            };

            CleverTapAPI.getDefaultInstance(getApplicationContext()).setSyncListener(listener1);

//            if (newProfile.containsKey("Phone")) {
//                newProfile.remove("Phone");
//            }
//
//            if (newProfile.containsKey("Email")) {
//                newProfile.remove("Email");
//            }

            Log.d("Clevertap|pushProfile", "HashMap remove email:" + newProfile.toString());

            CleverTapAPI.getDefaultInstance(getApplicationContext()).onUserLogin(newProfile);

            Log.d("Clevertap|pushProfile:", i_jsonString);
            Log.d("Clevertap|pushProfile:", "---------------------------------------------------------------------");
        }
    }

    // currently,dont use this function
    private void updateSequenceProfileData(final String[] i_jsonStringArr, final int i_index) {
        Log.d("Clevertap|", "i_jsonString.length:" + i_jsonStringArr.length + " ,currentID:" + i_index);

        if (i_jsonStringArr.length == 0 || i_index >= i_jsonStringArr.length) {
            Log.d("Clevertap|", "i_jsonStringArr.length == 0 ||i_index >=  i_jsonStringArr.length");

            return;
        }

        for (int i = 0; i < i_jsonStringArr.length; i++) {
            Log.d("Clevertap|", "i_jsonStringArr[" + i + "]" + i_jsonStringArr[i]);
        }

        Log.d("Clevertap", "---------------------------------------------------------------------");

        String currentProfileJson = i_jsonStringArr[i_index];

        if (currentProfileJson.isEmpty() || !isJSONObjectValid(currentProfileJson)) // make sure that is valid json.
        {
            Log.d("Clevertap|", "clevertap cannot pushProfile");

            return;
        }

        Map<String, Object> newProfile = new Gson().fromJson(currentProfileJson,
                new TypeToken<HashMap<String, Object>>() {
                }.getType());

        if (newProfile.isEmpty()) {
            Log.d("Clevertap|", "newProfile.isEmpty()");

            return;
        }

        SyncListener listener1 = new SyncListener() {
            @Override
            public void profileDataUpdated(JSONObject updates) {

            }

            @Override
            public void profileDidInitialize(String CleverTapID) {

                Map<String, Object> profileUpdateData = new Gson().fromJson(
                        i_jsonStringArr[i_index], new TypeToken<HashMap<String, Object>>() {
                        }.getType());

                Log.d("Clevertap",
                        "i_index:" + i_index + ", pushProfile|HashMap update data:" + profileUpdateData.toString());
                Log.d("Clevertap", "---------------------------------------------------------------------");

                CleverTapAPI.getDefaultInstance(getApplicationContext()).pushProfile(profileUpdateData);

                updateSequenceProfileData(i_jsonStringArr, i_index + 1);
            }
        };

        CleverTapAPI.getDefaultInstance(getApplicationContext()).setSyncListener(listener1);

        if (newProfile.containsKey("Email")) {
            newProfile.remove("Email");
        }

        if (newProfile.containsKey("Phone")) {
            newProfile.remove("Phone");
        }

        Log.d("Clevertap", "pushProfile|HashMap remove email:" + newProfile.toString());

        CleverTapAPI.getDefaultInstance(getApplicationContext()).onUserLogin(newProfile);

        Log.d("Clevertap", "Clevertap|pushProfile:" + currentProfileJson);
        Log.d("Clevertap", "---------------------------------------------------------------------");

    }
    private String getCleverTabAPIID() {
        return  CleverTapAPI.getDefaultInstance(getApplicationContext()).getCleverTapID();

    }
    private void pushEvent(String i_eventName, String i_properties) {
        if (i_eventName.isEmpty()) {
            Log.d("Clevertap|pushEvent", "cannot push CleverTap event.Because the i_eventName is empty.");

            return;
        }

        if (i_properties.isEmpty()) {
            CleverTapAPI.getDefaultInstance(getApplicationContext()).pushEvent(i_eventName);
        } else {
            HashMap<String, Object> properties = new Gson().fromJson(
                    i_properties, new TypeToken<HashMap<String, Object>>() {
                    }.getType());

            List<String> convertToDateEventNames = Arrays.asList("time_purchased", "time_activated", "time_expired");

            for (int i = 0; i < convertToDateEventNames.size(); i++) {
                String key = convertToDateEventNames.get(i);

                if (properties.containsKey(key)) {
                    String value = (String) properties.get(key);

                    Date date = new Date(Long.parseLong(value) * 1000L);// miliseconds

                    properties.remove(key);

                    properties.put(key, date); // convert to date format.
                }
            }

            Log.d("Clevertap|pushEvent", "i_properties:" + i_properties);

            CleverTapAPI.getDefaultInstance(getApplicationContext()).pushEvent(i_eventName, properties);

        }
    }

    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        // Checks the orientation of the screen
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            Toast.makeText(this, "landscape", Toast.LENGTH_SHORT).show();
        } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
            Toast.makeText(this, "portrait", Toast.LENGTH_SHORT).show();
        }
    }

    private void pushEventClickNotificationAppinbox(String idInboxMessage, int type) {
        // type == 0 view, type == 1 click
        CleverTapAPI cleverTapDefaultInstance = CleverTapAPI.getDefaultInstance(this);

        if (cleverTapDefaultInstance != null) {
            if (type == 0) {
                cleverTapDefaultInstance.pushInboxNotificationViewedEvent(idInboxMessage);
            } else {
                cleverTapDefaultInstance.pushInboxNotificationClickedEvent(idInboxMessage);
            }
        }
    }

    private void getDataAppInbox() {
        // AppActivity._ac.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        CleverTapAPI cleverTapDefaultInstance = CleverTapAPI.getDefaultInstance(this);

        if (cleverTapDefaultInstance != null) {
            // Set the Notification Inbox Listener
            cleverTapDefaultInstance.setCTNotificationInboxListener(this);
            // Initialize the inbox and wait for callbacks on overridden methods
            cleverTapDefaultInstance.initializeInbox();
            // inboxDidInitialize(cleverTapDefaultInstance);
        }

        if (cleverTapDefaultInstance != null) {
            ArrayList<CTInboxMessage> messagesValue = cleverTapDefaultInstance.getUnreadInboxMessages();
            dataObjectArray = new String[messagesValue.size()];
            for (int i = 0; i < messagesValue.size(); i++) {
                CTInboxMessage x = messagesValue.get(i);
                JSONObject _data = x.getData();
                dataObjectArray[i] = _data.toString();
            }
        }
        getDataObjectArray(dataObjectArray);
    }

    // call static method
    public static void sClearData() {
        AppActivity._ac.clearDeepLinkData();
    }

    public static void testCrash() {
        // Hàm test crash firebase
        throw new RuntimeException("test crash");
    }

    // call static method
    public static void sRateMe() {
        AppActivity._ac.rateMe();
    }

    public static void sPushProfile(String i_jsonString, boolean i_isUpdate) {
        AppActivity._ac.pushProfile(i_jsonString, i_isUpdate);
    }

    public static void loginDummyProfileCT(String dummyProfileData) {
        Map<String, Object> dummyProfile = new Gson().fromJson(
                dummyProfileData, new TypeToken<HashMap<String, Object>>() {
                }.getType()
        );
        CleverTapAPI.getDefaultInstance(_ac).onUserLogin(dummyProfile);

    }
    public static void fetchProductConfig() {
        CleverTapAPI cleverTapAPI = CleverTapAPI.getDefaultInstance(_ac.getApplicationContext());
        cleverTapAPI.productConfig().fetch();
    }

    public static void activeClevertap() {
        CleverTapAPI cleverTapAPI = CleverTapAPI.getDefaultInstance(_ac.getApplicationContext());
        cleverTapAPI.productConfig().activate();
    }

    public static boolean getProductConfigBoolValue(String key) {
        CleverTapAPI cleverTapAPI = CleverTapAPI.getDefaultInstance(_ac.getApplicationContext());
        boolean usingElsaAPI = cleverTapAPI.featureFlag().get(key, false);
        return usingElsaAPI;
    }

    public static void sPushEvent(String i_eventName, String i_jsonString) {
        AppActivity._ac.pushEvent(i_eventName, i_jsonString);
    }

    public static String sGetCleverTabID() {
        return  AppActivity._ac.getCleverTabAPIID();
    }
    // currently,dont use this function
    public static void sUpdateSequenceProfileData(String[] i_squenceInfo) {
        AppActivity._ac.updateSequenceProfileData(i_squenceInfo, 0);
    }

    // currently,dont use this function
    public static void sInitAppInbox() {

    }

    public boolean isJSONObjectValid(String test) {
        try {
            new JSONObject(test);
        } catch (JSONException ex) {
            return false;
        }
        return true;
    }

    public static boolean checkPermissionsForAudio() {

        int checkPermission1 = 1;
        int checkPermission = ContextCompat.checkSelfPermission(AppActivity._ac, Manifest.permission.RECORD_AUDIO);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S)
        {
            checkPermission1 = 0;
        }else{
            checkPermission1 = ContextCompat.checkSelfPermission(AppActivity._ac, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }

        return (checkPermission == 0 && checkPermission1 == 0);
    }

    public static void pushInboxNotificationViewedEvent(String idInboxMessage, int type) {
        AppActivity._ac.pushEventClickNotificationAppinbox(idInboxMessage, type);
    }

    public static void getStringDataAppInbox() {
        AppActivity._ac.getDataAppInbox();
    }

    public static void Thaydoimanhinh() {
        AppActivity._ac.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    public static String getPendingDeeplink() {

        String result = AppActivity._ac.pendingDeeplinkUrl;

        if (!AppActivity._ac.pendingDeeplinkUrl.isEmpty()) {
            AppActivity._ac.pendingDeeplinkUrl = "";
        }

        return result;
    }

    protected native void onCleverTapCallback();

    protected native void processQRCode(String str);

    protected native void onCameraQRCodeCallback(String str);
    public static native void onReciveDeppLink(String url);

    public void revertIcon() {
        // Timestamp ts = new Timestamp(getTimeStamp());
        try {
            Timestamp ts = new Timestamp(getTimeStamp() * 1000L);
            Date date = new Date(ts.getTime());

            int mon = date.getMonth() + 1;
            int year = date.getYear() + 1900;

            boolean isChangeIocon = (mon >= 8 && year == 2021);

            int country = getCountry();
            PackageManager pac = getPackageManager();

            if (country == 2 && isChangeIocon) {
                setInForKeyChangeIcon(-2);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconTH"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
                return;
            }

            if (country == 4 && isChangeIocon) {
                setInForKeyChangeIcon(-2);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconVN"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
                return;
            }

            if (country == 0 && isChangeIocon) {
                setInForKeyChangeIcon(-2);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconEN"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
                return;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @SuppressLint({ "WrongConstant", "LongLogTag" })
    public void changeIcon() {
        try {
            Timestamp ts = new Timestamp(getTimeStamp() * 1000L);
            Date date = new Date(ts.getTime());

            int day = date.getDate();
            int mon = date.getMonth() + 1;
            int year = date.getYear() + 1900;

            // AppActivity.this.e
            int country = getCountry();
            PackageManager pac = getPackageManager();
            boolean isChangeIcon = (day <= 31 && mon == 7 && year == 2021);

            if (country == 2 && isChangeIcon) {
                Log.d("ppppppppppppppppppppp", "pppppppppppppppppppppppppppppppppp");
                setInForKeyChangeIcon(-1);
                // setInForKeyChangeIcon(2);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconTH"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);

                return;
            }

            if (country == 4 && isChangeIcon) {
                Log.d("kkkkkkkkkkkkkkkkkkkkk", "kkkkkkkkkkkkkkkkkkkkkkkkk");
                setInForKeyChangeIcon(-1);
                // setInForKeyChangeIcon(4);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconVN"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
                return;
            }

            if (country == 0 && isChangeIcon) {
                Log.d("kkkkkkkkkkkkkkkkkkkkk", "kkkkkkkkkkkkkkkkkkkkkkkkk");
                setInForKeyChangeIcon(-1);
                // setInForKeyChangeIcon(4);
                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.RevertIcon1"),
                        PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);

                pac.setComponentEnabledSetting(
                        new ComponentName(AppActivity.this, "com.earlystart.android.monkeyjunior.story.ChangeIconEN"),
                        PackageManager.COMPONENT_ENABLED_STATE_ENABLED, PackageManager.DONT_KILL_APP);
                return;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public void thongbao(Vector<String> title, Vector<String> message, int numberTime) {
        // AppActivity.this.setTitle();
        long timeAlarm = System.currentTimeMillis();
        long tenSeconds = 1000 * 5 * 60;
        long idNotification = timeAlarm + tenSeconds;

        Intent intent = new Intent(AppActivity.this, ReminderBroadcast.class);
        ///
        intent.putExtra("title", title.get(0));
        intent.putExtra("message", message.get(0));
        intent.putExtra("muchNotification", title.size() > 1);
        // intent.put
        //
        String text = title.remove(0);
        String text1 = message.remove(0);
        intent.putExtra("idNotification", idNotification);
        ///
        if (title.size() > 0) {
            Object[] objArrayTitle = title.toArray();
            Object[] objArrayMessage = message.toArray();

            if (objArrayTitle != null && objArrayMessage != null) {
                String[] arrayStringTitle = Arrays.copyOf(objArrayTitle, objArrayTitle.length, String[].class);
                String[] arrayStringMessage = Arrays.copyOf(objArrayMessage, objArrayMessage.length, String[].class);
                intent.putExtra("listTitle", arrayStringTitle);
                intent.putExtra("listMessage", arrayStringMessage);
            }
        }
        final int flag =  Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE : PendingIntent.FLAG_UPDATE_CURRENT;

        PendingIntent pendingIntent = PendingIntent.getBroadcast(AppActivity.this, numberTime, intent,
                flag);

        AlarmManager alarmManager = (AlarmManager) getSystemService(ALARM_SERVICE);

        alarmManager.setAndAllowWhileIdle(AlarmManager.RTC_WAKEUP, timeAlarm + tenSeconds, pendingIntent);

    }

    public static void setTitleAndMessageForNotification(String title, String message) {
        titleAppinbox.add(title);
        messageAppinbox.add(message);
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static void showNotificationForAppinbox() {
        if (titleAppinbox.size() > 0 && messageAppinbox.size() == titleAppinbox.size()) {
            AppActivity._ac.thongbao(titleAppinbox, messageAppinbox, 0);
        }
    }

    private void mute() {
        // mute audio
        try {
            AudioManager amanager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
            amanager.setStreamMute(AudioManager.STREAM_NOTIFICATION, true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void unmute() {
        // unmute audio
        try {
            AudioManager amanager = (AudioManager) AppActivity._ac.getSystemService(Context.AUDIO_SERVICE);
            amanager.setStreamMute(AudioManager.STREAM_NOTIFICATION, false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void registerBroadcastSmsReceiver() {
        AppActivity._ac.registerBroadcastReceiver();
    }

    static void closeBroadcastSmsReceiver() {
        AppActivity._ac.closeBroadcastReceiver();
    }

    static CameraActivity cameraActivity = null;

    void InitCamera(int cameraViewWidth, int cameraViewHeight, int cameraPointX, int cameraPointY) {

        shouldPause = false;
        s_isFullScreenActivityPause = false;
        cameraActivity = new CameraActivity(AppActivity._ac, cameraViewWidth, cameraViewHeight, cameraPointX,
                cameraPointY);
        // cameraActivity.onCaptureClick(false);
    }

    static void OpenCamera() {
        if (AppActivity._ac != null) {
            {
                int cameraViewWidth = 200, cameraViewHeight = 150, cameraPointX = 0, cameraPointY = 0;
                AppActivity._ac.InitCamera(cameraViewWidth, cameraViewHeight, cameraPointX, cameraPointY);
            }
        }
    }

    static void moveFrontCameraToPos(int pointX, int pointY, int cameraSizeWidth, int cameraSizeHeight) {
        if (cameraActivity != null) {
            // cameraActivity.UpdateTextureViewPointion(pointX, pointY);
            // cameraActivity.updateTextureViewSize(cameraSizeWidth, cameraSizeHeight);
        }

    }

    static void addViewToCamera(int pointX, int pointY, int cameraSizeWidth, int cameraSizeHeight) {

    }

    static Rect convertCocosPositionToIOSScreenPos(int pointX, int pointY, int cameraSizeWidth, int cameraSizeHeight) {
        Rect rect = new Rect(pointX, pointY, pointX + cameraSizeWidth, pointY + cameraSizeHeight);
        return rect;
    }

    static void StopCamera() {
        // cameraFragment.dismiss();
    }

    static void SaveVideo() {
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)
    public static float getFreeInternalMemory() {
        StatFs statFs = new StatFs(Environment.getExternalStorageDirectory().getPath());
        long free = statFs.getBlockSizeLong() * statFs.getAvailableBlocksLong();
        long megAvailable = free / (1024 * 1024 * 1024);
        return (long) megAvailable;
    }

    public static void pushEventLostConnection(String link, String errorType, String type, String errorcode, String countRetry, String ipAndspeed) throws ProtocolException {
        URL url = null;
        try {

            String [] dataObjectArrayOfValue = new String[8];
            dataObjectArrayOfValue[0] = link;
            dataObjectArrayOfValue[1] = errorType;
            dataObjectArrayOfValue[2] = type;
            dataObjectArrayOfValue[3] = errorcode;
            dataObjectArrayOfValue[4] = countRetry;
            dataObjectArrayOfValue[5] = ipAndspeed;

            dataObjectArrayOfValue[6] = "None";
            dataObjectArrayOfValue[7] = "None";

            url = new URL(link);

            if(url == null)
            {
                pushEnvetLostConnectionToClevertab(dataObjectArrayOfValue);
                return;
            }

            HttpURLConnection connection = (HttpURLConnection)url.openConnection();
            if(connection == null)
            {
                pushEnvetLostConnectionToClevertab(dataObjectArrayOfValue);
                return;
            }
            connection.setRequestMethod("GET");
            connection.connect();


            int code = connection.getResponseCode();

            String statusCode = String.valueOf(code);

            String host = url.getHost();

            InetAddress address = InetAddress.getByName(host);

            String ipAddress = address.getHostAddress();

            System.out.println("Response code of the object is "+code);

            dataObjectArrayOfValue[6] = ipAddress;
            dataObjectArrayOfValue[7] = statusCode;

            pushEnvetLostConnectionToClevertab(dataObjectArrayOfValue);

        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private static void deleteFilePng(String forderPath)
    {
        File foder = new File(forderPath);

        File [] listFileInFoder = foder.listFiles();

        for(int i = 0; i < listFileInFoder.length; i++)
        {
            if(listFileInFoder[i] == null)
            {
             continue;
            }
            if(listFileInFoder[i].isFile())
            {
                String nameFile = listFileInFoder[i].getName();

                if((nameFile.indexOf(".png") != -1) || (nameFile.indexOf(".jpg") != -1))
                {
                    if(listFileInFoder[i].delete())
                    {
                        int cc = 0;
                    }else{
                        int kk = 0;
                    }
                }
            }
        }
    }
    public static String getClevertapIdCpp() {
        return _ac.getCleverTapIDSync();
    }
    private String getCleverTapIDSync() {
        return CleverTapAPI.getDefaultInstance(getApplicationContext()).getCleverTapID();
    }

}
