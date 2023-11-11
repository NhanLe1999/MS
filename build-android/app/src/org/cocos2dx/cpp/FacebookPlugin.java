package org.cocos2dx.cpp;

import android.os.Bundle;
import android.util.Log;
import android.content.Intent;

import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.Iterator;

public class FacebookPlugin {
    public static int FACEBOOK_LOGIN_REQUEST_CODE = 64206;
    static AppEventsLogger logger;


    static void init() {
        logger = AppEventsLogger.newLogger(AppActivity._ac);
    }

    static Bundle jsonToBundle(JSONObject jsonObject) throws JSONException {
        Bundle bundle = new Bundle();
        Iterator iter = jsonObject.keys();
        while(iter.hasNext()){
            String key = (String)iter.next();
            String value = jsonObject.getString(key);
            bundle.putString(key,value);
        }
        return bundle;
    }

    static void logEvent(String name, String param) {
        Log.d("facebook analytics", param);
        try {
            JSONObject props = new JSONObject(param);
            logger.logEvent(name, jsonToBundle(props));
        } catch (Exception e) {
        }
    }

    static void setUserProperties(String param) {
        Log.d("facebook analytics", param);
        try {
            JSONObject props = new JSONObject(param);
           // AppEventsLogger.updateUserProperties(jsonToBundle(props), null);
        } catch (Exception e) {
        }
    }

    static void setIdentifier(String identifier) {
        Log.d("facebook analytics", identifier);
        AppEventsLogger.setUserID(identifier);
    }



}
