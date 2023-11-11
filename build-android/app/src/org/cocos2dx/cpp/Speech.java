package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.speech.RecognitionListener;
import android.util.Log;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Locale;

public class Speech  {
    protected void onCreate(Bundle savedInstanceState){
        AppActivity._ac.onCreate(savedInstanceState);
    }

    static public void getSpeechInput() {

        Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, "en-US");

        if (intent.resolveActivity(AppActivity._ac.getPackageManager()) != null) {
            AppActivity._ac.startActivityForResult(intent, 10);
        } else {
            Toast.makeText(AppActivity._ac, "Your Device Don't Support Speech Input", Toast.LENGTH_SHORT).show();
        }

     }

    static public void onActivityResult(int requestCode, int resultCode, Intent data) {
        AppActivity._ac.onActivityResult(requestCode, resultCode, data);

        switch (requestCode)
        {
            case 10:
                if(resultCode == AppActivity.RESULT_OK && null!=data){
                    ArrayList<String> result = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
                    String output = result.get(0);
                    Log.e("ket_qua: ", output);
                }
        }
    }
}