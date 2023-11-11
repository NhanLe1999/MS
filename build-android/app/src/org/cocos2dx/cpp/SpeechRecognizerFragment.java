package org.cocos2dx.cpp;

import android.Manifest;
import android.app.Fragment;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.speech.RecognitionListener;
import android.speech.RecognizerIntent;
import android.speech.SpeechRecognizer;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxEditBox;
import org.cocos2dx.lib.Cocos2dxHelper;

import java.util.ArrayList;
import java.util.IllformedLocaleException;
import java.util.Locale;

public class SpeechRecognizerFragment extends Fragment
{
    //Tag to follow logcats
    private static final String TAG = "SpeechRecognizer";
    // Singleton instance.
    public static SpeechRecognizerFragment instance;

    private static boolean _isRecord = true;

    private static boolean _isRecording = false;

    //UNITY CONTEXT
    String gameObjectName;
    String resultCallbackName = "OnResult";
    String errorCallbackName = "OnError";
    public static void SendUnityResults(String results)
    {
        //UnityPlayer.UnitySendMessage(instance.gameObjectName, instance.resultCallbackName, results);
        Log.d(TAG, results);
    }
    public static void SendUnityError(ERROR error)
    {
        String errorString = String.valueOf(error);
        //UnityPlayer.UnitySendMessage(instance.gameObjectName, instance.errorCallbackName, errorString);
        Log.d(TAG, errorString);
    }
    public static void SetUp(String gameObjectName)
    {
        instance = new SpeechRecognizerFragment();
        instance.gameObjectName = gameObjectName; // Store 'GameObject' reference
        //UnityPlayer.currentActivity.getFragmentManager().beginTransaction().add(instance, SpeechRecognizerFragment.TAG).commit();
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setRetainInstance(true);
    }

    //SPEECH RECOGNIZER
    //static public SpeechRecognitionListener speechListener = new SpeechRecognitionListener();
    private static boolean isLanguageSpecified = false;
    private static String language = "en-US";
    private static int maxResults = 10;

    public enum ERROR
    {
        UNKNOWN, INVALID_LANGUAGE_FORMAT
    }


    static public void StartListening()
    {
        AppActivity._ac.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                Intent intentSpeechToText = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
                intentSpeechToText.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
                intentSpeechToText.putExtra(RecognizerIntent.EXTRA_LANGUAGE, "en-US");
                intentSpeechToText.putExtra(RecognizerIntent.EXTRA_PARTIAL_RESULTS, true);

                if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
                    try
                    {
                        AppActivity._ac.sr.startListening(intentSpeechToText);
                    }
                    catch (ActivityNotFoundException a)
                    {
                        Log.d(TAG, a.toString());
                    }
                }
            }
        });
    }
    public void StartListening(boolean isContinuous, String language, int maxResults)
    {
        SetContinuousListening(isContinuous);
        SetLanguage(language);
        SetMaxResults(maxResults);
        StartListening();
    }
    static public void StopListening()
    {
        AppActivity._ac.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if(_isRecording) {
                    Log.d(TAG, "stop Listening");
                    AppActivity._ac.sr.stopListening();
                }
                else
                {
                    Log.d(TAG, "Cancel Listening");
                    AppActivity._ac.sr.cancel();
                    AppActivity._ac.onSpeechToTextResult("");

                }
            }
        });
    }
    public void RestartListening()
    {
        StopListening();
        StartListening();
    }
    public void SetContinuousListening(boolean isContinuous)
    {
        AppActivity._ac.speechListener.continuousListening = isContinuous;
    }
    public void SetLanguage(String newLanguage)
    {
        try
        {
            Locale locale = new Locale.Builder().setLanguageTag(newLanguage).build();
            language = newLanguage;
            isLanguageSpecified = true;
        }catch (IllformedLocaleException exception)
        {
            isLanguageSpecified = false;
            SendUnityError(ERROR.INVALID_LANGUAGE_FORMAT);
        }
    }
    public void SetMaxResults(int newMaxResults)
    {
        maxResults = newMaxResults;
    }

    //SPEECH RECOGNIZER_LISTENER
    static class SpeechRecognitionListener implements RecognitionListener
    {
        private ArrayList<String> resultData = new ArrayList<>();
        public boolean continuousListening = false;

        public void onReadyForSpeech(Bundle params) {

            _isRecording = true;

            Log.d(TAG, "onReadyForSpeech");
        }

        public void onBeginningOfSpeech() {
            Log.d(TAG, "onBeginningOfSpeech");
        }

        public void onRmsChanged(float rmsdB) {
            Log.d(TAG, "onRmsChanged");
        }

        public void onBufferReceived(byte[] buffer) {
            Log.d(TAG, "onBufferReceived");
        }

        public void onEndOfSpeech() {
            Log.d(TAG, "onEndofSpeech");
            _isRecording = false;
        }

        public void onError(int error)
        {
            _isRecording = false;
            if(continuousListening){}
                //RestartListening();
        }

        /**
         * Called when recognition results are ready.
         *
         * @param results the recognition results. To retrieve the results in {@code
         *                ArrayList<String>} format use {@link Bundle#getStringArrayList(String)} with
         *                {@link SpeechRecognizer#RESULTS_RECOGNITION} as a parameter. A float array of
         *                confidence values might also be given in {@link SpeechRecognizer#CONFIDENCE_SCORES}.
         */
        @Override
        public void onResults(Bundle results)
        {
            Log.d(TAG, "onResult");
            StringBuilder str = new StringBuilder();
            resultData = results.getStringArrayList(SpeechRecognizer.RESULTS_RECOGNITION);
            if (resultData != null)
            {
                str.append(resultData.get(0));
                for (int i = 1; i < resultData.size(); i++)
                {
                    str.append("~").append(resultData.get(i));
                }
            }

            AppActivity._ac.onSpeechToTextResult(str.toString());
            if(continuousListening){}
                //RestartListening();
        }

        public void onPartialResults(Bundle partialResults) {
            Log.d(TAG, "onPartialResults");
        }

        public void onEvent(int eventType, Bundle params) {
            Log.d(TAG, "onEvent " + eventType);
        }
    }
}