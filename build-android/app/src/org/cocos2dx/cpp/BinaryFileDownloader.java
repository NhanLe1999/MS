package org.cocos2dx.cpp;

import android.util.Log;
import android.view.animation.AnimationUtils;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ProtocolException;
import java.net.URL;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import static java.lang.Math.abs;
import static java.lang.Math.nextUp;

import androidx.annotation.NonNull;

public class BinaryFileDownloader {

    private static OkHttpClient _client = null;
    private static Call _call = null;
    private static File _fileDelete = null;
    private static OutputStream _outputStream = null;

    static protected native void processEvent(int fileSizeReceive, long totalFieSize);
    static protected native void onSucessDownload();
    static protected native void onErrorDownload(String [] value);

    public static void insertToListDownLoadFile(final String url, final String nameFile) throws IOException {
        Thread thread = new Thread(new Runnable() {
            public void run() {
                /*
                 * Do something
                 */
                downloadFile(url, nameFile);

            }
        });
        thread.start();
    }

    public static void downloadFile(final String url, final String pathFile)
    {
        final OkHttpClient client = new OkHttpClient();
        _client = client;

        Request Rq = new Request.Builder().url(url).get().build();
        //Request Rq = new Request.Builder().url("http://cdn.kiddyscience.com/live/audio-book/audio/0/f0c7892fa7e2d6692d68dea55cf19437.mp3").get().build();

        client.newCall(Rq).enqueue(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull IOException e) {
                onErrorDownload(null);
            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                String message = "None";
                int respnseCode = -1;
                if(response != null)
                {
                    message = response.message();
                    respnseCode = response.code();

                    if(respnseCode == 200)
                    {
                        System.out.println("" + response.headers().toString());
                        System.out.println("" + response.body().contentLength());

                        InputStream inputStream = response.body().byteStream();

                        if(inputStream != null)
                        {
                            final long contentLength = (long) response.body().contentLength();

                            String pathFileFalse = pathFile + "monkey";

                            File filedownload = new File(pathFileFalse);
                            _fileDelete = filedownload;

                            OutputStream fileOutputStream = new FileOutputStream(filedownload);

                            System.out.println("writing file " + contentLength);

                            long downloaded = 0;

                            byte[] BUFFER_SIZE = new byte[1024]; //File Corrupt

                            double precenBegin = 0.0;

                            while (true) {
                                int byteRead = inputStream.read(BUFFER_SIZE);
                                if (byteRead == -1) {
                                    break;
                                }

                                downloaded += byteRead;

                                double precen = (double) downloaded / (double) contentLength;

                                double p = abs(precenBegin - precen);

                                if(abs(precenBegin - precen) > 0.08 || downloaded == contentLength)
                                {
                                    //processEvent
                                    precenBegin = precen;


                                    processEvent((int) downloaded,contentLength);
                                }

                                fileOutputStream.write(BUFFER_SIZE, 0, byteRead);

                                System.out.println(" " + downloaded + "/" + contentLength + " = " + ((downloaded / contentLength) * 100));

                            }

                            fileOutputStream.flush();
                            fileOutputStream.close();

                            if(downloaded == contentLength && filedownload != null || contentLength == -1)
                            {
                                //rename file
                                File FileTrue = new File(pathFile);
                                if(filedownload.renameTo(FileTrue))
                                {
                                    onSucessDownload();
                                    //sucess event
                                }else{

                                    //deleteFile and retr
                                    onErrorDownload(null);
                                }

                            }else{
                                //delete file and downloaf lai
                                onErrorDownload(null);
                            }

                            System.out.println("file closed");

                        }else{
                            //inputStream null
                            onErrorDownload(null);
                        }
                    }else{
                        String statusCode = String.valueOf(respnseCode);
                        String [] data = new String[4];

                        data[0] = statusCode;
                        data[1] = "ipAddress";
                        data[2] = message;
                        data[3] = "lost_connection";
                        onErrorDownload(data);
                        disconnectionDownload();
                    }

                }else{
                    //response null
                    onErrorDownload(null);
                }
            }
        });

    }

    public static void deleteFile(File file)
    {
        if(file != null)
        {
            if(file.delete())
            {
                Log.d("delete", "sucess: ");

            }else{
                Log.d("delete", "error: ");
            }
        }
    }


    public static void disconnectionDownload()
    {
        if(_call != null)
        {
            _call.cancel();
            _call = null;
        }

        if(_client != null)
        {
            _client = null;
        }

        if(_outputStream != null)
        {
            try {
                _outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        deleteFile(_fileDelete);
    }
}
