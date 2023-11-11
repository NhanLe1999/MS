package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.util.Log;
import android.view.View;

import com.earlystart.android.monkeyjunior.story.R;
import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.GoogleApiClient;

import android.net.Uri;
import android.provider.Settings;

import static com.earlystart.android.monkeyjunior.story.R.string.GOOGLE_SERVER_CLIENT_ID;

public class MJHelper {

    public static int RC_SIGN_IN = 100;
    public static int RC_GET_TOKEN = 101;
    //private static String CLIENT_ID = "518533842801-irq3dmq54aupcltue9t86ffaoa1p0hno.apps.googleusercontent.com";
//    private static String CLIENT_ID = "518533842801-lu80s0fntoma1kns3q82s880eohe7phl.apps.googleusercontent.com";//"903853614382-t2ej1rhshogjtf7ks1rcav5b54ovvd6k.apps.googleusercontent.com" ;//"518533842801-7vogfmo1aie85dckvi9njpabnm60k631.apps.googleusercontent.com";//
    static GoogleSignInOptions gso;
    static GoogleSignInClient mGoogleSignInClient;
    
    static public void openDeviceAppSettings(){
        // Open app settings screen
        Intent intent = new Intent();
        intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        Uri uri = Uri.fromParts("package", AppActivity._ac.getPackageName(), null);
        intent.setData(uri);
        AppActivity.getContext().startActivity(intent);
    }

    static public void loginWithGoogle(){
        try {
            String CLIENT_ID = AppActivity._ac.getString(GOOGLE_SERVER_CLIENT_ID);
            GoogleSignInAccount alreadyloggedAccount = GoogleSignIn.getLastSignedInAccount(AppActivity._ac);
            gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                    .requestIdToken( CLIENT_ID)
                    .requestEmail()
                    .requestServerAuthCode(CLIENT_ID)
                    .build();
            if (alreadyloggedAccount != null) {

                Log.d("token", alreadyloggedAccount.getIdToken());
                Log.d("email", alreadyloggedAccount.getEmail());

//            gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN).requestEmail().build();

                GoogleSignIn.getClient(AppActivity._ac, gso).signOut();

            } else {
//            Log.d(TAG, "Not logged in");

            }

            // Build a GoogleSignInClient with the options specified by gso.
            mGoogleSignInClient = GoogleSignIn.getClient( AppActivity._ac,  gso);

            Intent signInIntent = mGoogleSignInClient.getSignInIntent();
            AppActivity._ac.startActivityForResult(signInIntent, RC_GET_TOKEN);
        } catch(Exception e) {
            e.printStackTrace();
        }

    }

    public static int ACCOUNT_KIT_REQUEST_CODE = 99099099;

    public static int APP_REQUEST_CODE = 99;

    static public void phoneLogin() {

    }


    static void phoneVerification(String phonenumber) {

    }

    static void share(String url) {
        Intent i = new Intent(Intent.ACTION_SEND);
        i.setType("text/plain");
        i.putExtra(Intent.EXTRA_SUBJECT, "Sharing URL");
        i.putExtra(Intent.EXTRA_TEXT, url);
        AppActivity._ac.startActivity(Intent.createChooser(i, "Share URL"));
    }

    static void startRecord() {
        AppActivity._ac.shareScreen();
    }

    static void stopRecord() {
        AppActivity._ac.stopRecord();
    }

    static boolean isRecording() {
        return AppActivity._ac.isRecording();
    }

    static boolean isScreenRecordingSupported() {
        return Build.VERSION.SDK_INT >= 21;
    }

    //static void setOneSignalEmail(String email) {

    //}
}
