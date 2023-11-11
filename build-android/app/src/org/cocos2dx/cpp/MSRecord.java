package org.cocos2dx.cpp;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaPlayer;
import android.media.MediaRecorder;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.util.Log;
import android.os.Environment;
import android.widget.Toast;

import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MSRecord {
//doc: https://www.edumobile.org/android/audio-recording-in-wav-format-in-android-programming/
    private static final int RECORDER_BPP = 16;

    private static final int RECORDER_SAMPLERATE = 22050;//original : 44100
    private static final int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_STEREO;
    private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;

    private static AudioRecord recorder = null;
    private static int bufferSize = 0;
    private static Thread recordingThread = null;
    private static boolean isRecording = false;


    private static AppActivity app = AppActivity._ac;
    private static final String LOG_TAG = "MSRecord";
    public static final int REQUEST_RECORD_AUDIO_PERMISSION_MAX = 201;
    private static String mFileName = null;

    private static MediaPlayer   mPlayer = null;

    // Requesting permission to RECORD_AUDIO
    static boolean permissionToRecordAccepted = false;

    static final String recordFolder = AppActivity._ac.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS) + File.separator + "Speaking";

    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode){
            case REQUEST_RECORD_AUDIO_PERMISSION_MAX:
                permissionToRecordAccepted  = grantResults[0] == PackageManager.PERMISSION_GRANTED;
                break;
        }

    }

    static void startPlaying() {
        if (!permissionToRecordAccepted) {
            return;
        }
        Log.e(LOG_TAG, "hoisung --- start play audio");
        mPlayer = new MediaPlayer();
        try {
            mPlayer.setDataSource(mFileName);
            mPlayer.setVolume(1.0f,1.0f);
            mPlayer.prepare();
            mPlayer.start();
        } catch (IOException e) {
            Log.e(LOG_TAG, "prepare() failed");
        }
    }

    static void stopPlaying() {
        if (!permissionToRecordAccepted) {
            return;
        }
        Log.e(LOG_TAG, "hoisung --- stop play audio");
        mPlayer.stop();
        mPlayer.release();
        mPlayer = null;
    }

    public static String getCurSysDate() {
        return new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(new Date());
    }

    public static String getFilePath() {
//        final String directory = Environment.getExternalStorageDirectory() + File.separator + "Speaking";
        final String directory = recordFolder;
        if (!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            Toast.makeText(AppActivity._ac, "Failed to get External Storage", Toast.LENGTH_SHORT).show();
            return null;
        }
        final File folder = new File(directory);
        boolean success = true;
        if (!folder.exists()) {
            success = folder.mkdirs();
        }
        String filePath;
        if (success) {
//            String videoName = ("record_" + getCurSysDate() + ".3gp");
            String videoName = "mjRecord.wav";
            filePath = directory + File.separator + videoName;
        } else {
            AppActivity._ac.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(AppActivity._ac, "Failed to create Recordings directory", Toast.LENGTH_SHORT).show();
                }
            });
            return null;
        }
        return filePath;
    }
    public static String getRawFilePath() {
        final String directory = recordFolder;
        if (!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            Toast.makeText(AppActivity._ac, "Failed to get External Storage", Toast.LENGTH_SHORT).show();
            return null;
        }
        final File folder = new File(directory);
        boolean success = true;
        if (!folder.exists()) {
            success = folder.mkdirs();
        }
        String filePath;
        if (success) {
            String videoName = "tmp.raw";
            filePath = directory + File.separator + videoName;
        } else {
            AppActivity._ac.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(AppActivity._ac, "Failed to create Recordings directory", Toast.LENGTH_SHORT).show();
                }
            });
            return null;
        }
        return filePath;
    }

    public static int getPermissionStatus(){

        boolean writeExternalStoragePermission = true;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
        {
            writeExternalStoragePermission =  AppActivity._ac.getPackageManager().checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, AppActivity._ac.getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;
        }
        boolean recordPermission = AppActivity._ac.getPackageManager().checkPermission (Manifest.permission.RECORD_AUDIO, AppActivity._ac.getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;
        if(recordPermission && writeExternalStoragePermission){
            System.out.println("record permission granted. sdk version: " + Build.VERSION.SDK_INT);
            return 1; //granted
        }
        int askedTimes = Cocos2dxHelper.getIntegerForKey("ASK_RECORD_PERMISSION_TIMES",0);
        if(askedTimes == 0){
            return 3;//undertermined
        }
        boolean canShowRecordPermissionPopup = true;
        boolean canShowWriteExternalStoragePermissionPopup = true;
        //shouldShowRequestPermissionRationale return false if user check don't ask again or never be asked before
        if(Build.VERSION.SDK_INT >= 30){
//            canShowRecordPermissionPopup = AppActivity._ac.shouldShowRequestPermissionRationale(Manifest.permission.RECORD_AUDIO);
//            canShowWriteExternalStoragePermissionPopup = AppActivity._ac.shouldShowRequestPermissionRationale(Manifest.permission.WRITE_EXTERNAL_STORAGE);
//            if(canShowRecordPermissionPopup || canShowWriteExternalStoragePermissionPopup){//second first time asked
//                return 2;
//            }
            return 2;
        }else if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            canShowRecordPermissionPopup = AppActivity._ac.shouldShowRequestPermissionRationale(Manifest.permission.RECORD_AUDIO);
            canShowWriteExternalStoragePermissionPopup = AppActivity._ac.shouldShowRequestPermissionRationale(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            System.out.printf("\nshouldShowRequestPermissionRationale record - write file: " + canShowRecordPermissionPopup + " - " + canShowWriteExternalStoragePermissionPopup);
            if(!canShowRecordPermissionPopup || !canShowWriteExternalStoragePermissionPopup){//nerver ask again
                return 2;//denied
            }else{
                return 3;//undertermined
            }
        } else {//android < 5
            return 3;
        }
    }

    static void leadUserToGrantPermissionSetting() {
        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        Uri uri = Uri.fromParts("package", AppActivity._ac.getApplicationContext().getPackageName(), null);
        intent.setData(uri);
        AppActivity._ac.startActivity(intent);
    }

    static void askPermission() {

        String [] permissions;
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
        {
            permissions = new String[] { Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE };
        }else{
            permissions = new String[] { Manifest.permission.RECORD_AUDIO};
        }
        ActivityCompat.requestPermissions(AppActivity._ac, permissions, REQUEST_RECORD_AUDIO_PERMISSION_MAX);
        Cocos2dxHelper.setIntegerForKey("ASK_RECORD_PERMISSION_TIMES",Cocos2dxHelper.getIntegerForKey("ASK_RECORD_PERMISSION_TIMES",0)+1);
    }

    static void startRecording() {

        permissionToRecordAccepted = AppActivity._ac.getPackageManager().checkPermission (Manifest.permission.RECORD_AUDIO, AppActivity._ac.getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S)
        {
            permissionToRecordAccepted = permissionToRecordAccepted && (AppActivity._ac.getPackageManager().checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, AppActivity._ac.getApplicationContext().getPackageName()) == PackageManager.PERMISSION_GRANTED);
        }

        if (!permissionToRecordAccepted) {
            String [] permissions = {Manifest.permission.RECORD_AUDIO};
            ActivityCompat.requestPermissions(AppActivity._ac, permissions, REQUEST_RECORD_AUDIO_PERMISSION_MAX);

            return;
        }

        Log.e(LOG_TAG, "hoisung --- start record");
        mFileName = getFilePath();
//        mFileName += "/audiorecordtest.3gp";
        Log.e(LOG_TAG, "hoisung --- start record  --- " + mFileName);



        bufferSize = AudioRecord.getMinBufferSize(8000,
                AudioFormat.CHANNEL_IN_MONO,
                AudioFormat.ENCODING_PCM_16BIT);

        recorder = new AudioRecord(MediaRecorder.AudioSource.MIC,
                RECORDER_SAMPLERATE, RECORDER_CHANNELS,RECORDER_AUDIO_ENCODING, bufferSize);
        int i = recorder.getState();
        if(i==1)
            recorder.startRecording();

        isRecording = true;
        recordingThread = new Thread(new Runnable() {

            @Override
            public void run() {
                writeAudioDataToFile(getRawFilePath());
            }
        },"AudioRecorder Thread");

        recordingThread.start();

    }

    static void stopRecording() {
        if(null != recorder){
            isRecording = false;
            recorder.stop();
            recorder.release();
            recorder = null;
            recordingThread = null;
        }

        copyWaveFile(getRawFilePath(),getFilePath());
        deleteTempFile();
    }

    static void writeAudioDataToFile(String tmpFilePath){
        byte data[] = new byte[bufferSize];
        String filename =tmpFilePath;//tmp
        FileOutputStream os = null;

        try {
            os = new FileOutputStream(filename);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        int read = 0;

        if(null != os){
            while(isRecording){
                read = recorder.read(data, 0, bufferSize);

                if(AudioRecord.ERROR_INVALID_OPERATION != read){
                    try {
                        os.write(data);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            try {
                os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    private static void deleteTempFile() {
        File file = new File(getRawFilePath());

        file.delete();
    }
    private static void copyWaveFile(String inFilename,String outFilename){
        FileInputStream in = null;
        FileOutputStream out = null;
        long totalAudioLen = 0;
        long totalDataLen = totalAudioLen + 36;
        long longSampleRate = RECORDER_SAMPLERATE;
        int channels = 2;
        long byteRate = RECORDER_BPP * RECORDER_SAMPLERATE * channels/8;

        byte[] data = new byte[bufferSize];

        try {
            in = new FileInputStream(inFilename);
            out = new FileOutputStream(outFilename);
            totalAudioLen = in.getChannel().size();
            totalDataLen = totalAudioLen + 36;


            WriteWaveFileHeader(out, totalAudioLen, totalDataLen,
                    longSampleRate, channels, byteRate);

            while(in.read(data) != -1){
                out.write(data);
            }

            in.close();
            out.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private static void WriteWaveFileHeader(
            FileOutputStream out, long totalAudioLen,
            long totalDataLen, long longSampleRate, int channels,
            long byteRate) throws IOException {

        byte[] header = new byte[44];

        header[0] = 'R'; // RIFF/WAVE header
        header[1] = 'I';
        header[2] = 'F';
        header[3] = 'F';
        header[4] = (byte) (totalDataLen & 0xff);
        header[5] = (byte) ((totalDataLen >> 8) & 0xff);
        header[6] = (byte) ((totalDataLen >> 16) & 0xff);
        header[7] = (byte) ((totalDataLen >> 24) & 0xff);
        header[8] = 'W';
        header[9] = 'A';
        header[10] = 'V';
        header[11] = 'E';
        header[12] = 'f'; // 'fmt ' chunk
        header[13] = 'm';
        header[14] = 't';
        header[15] = ' ';
        header[16] = 16; // 4 bytes: size of 'fmt ' chunk
        header[17] = 0;
        header[18] = 0;
        header[19] = 0;
        header[20] = 1; // format = 1
        header[21] = 0;
        header[22] = (byte) channels;
        header[23] = 0;
        header[24] = (byte) (longSampleRate & 0xff);
        header[25] = (byte) ((longSampleRate >> 8) & 0xff);
        header[26] = (byte) ((longSampleRate >> 16) & 0xff);
        header[27] = (byte) ((longSampleRate >> 24) & 0xff);
        header[28] = (byte) (byteRate & 0xff);
        header[29] = (byte) ((byteRate >> 8) & 0xff);
        header[30] = (byte) ((byteRate >> 16) & 0xff);
        header[31] = (byte) ((byteRate >> 24) & 0xff);
        header[32] = (byte) (2 * 16 / 8); // block align
        header[33] = 0;
        header[34] = RECORDER_BPP; // bits per sample
        header[35] = 0;
        header[36] = 'd';
        header[37] = 'a';
        header[38] = 't';
        header[39] = 'a';
        header[40] = (byte) (totalAudioLen & 0xff);
        header[41] = (byte) ((totalAudioLen >> 8) & 0xff);
        header[42] = (byte) ((totalAudioLen >> 16) & 0xff);
        header[43] = (byte) ((totalAudioLen >> 24) & 0xff);

        out.write(header, 0, 44);
    }

}