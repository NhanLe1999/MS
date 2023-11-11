package org.cocos2dx.cpp;

import static com.clevertap.android.sdk.Utils.runOnUiThread;

import static java.lang.Math.abs;

import android.util.Log;

import com.mixpanel.android.java_websocket.util.Base64;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.ProtocolException;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.PrivateKey;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import org.apache.commons.io.FileUtils;

public class DownloadFile
{
    private static HttpURLConnection _httpURLConnection;
    private static String _pathFileDelete = "";
    private static File _fileDelete;
    private static int codestatusRe = -1;
    private static String messageRespone = "-1";
    private static URL urlRe = null;
    private static String _urlLink = "";

    private static Thread _thread = null;

    private static ByteArrayOutputStream _byByteArrayOutputStream;
    private static ByteArrayInputStream _byArrayInputStream;

    //private static Thread thread;
    static protected native void processEvent(int fileSizeReceive, long totalFieSize);
    static protected native void onSucessEvent();
    static protected native void onErrorEvent(String [] value);
    static protected native void getDataBy();


    public static void insertToListDownLoadFile(final String url, final String nameFile) throws IOException {
        _urlLink = url;
        _thread = new Thread(new Runnable() {
            public void run() {
                /*
                 * Do something
                 */
                InfoConnection infoConnection = new InfoConnection(url, nameFile);
                try {
                    createDownload(infoConnection, false);
                } catch (ProtocolException e) {
                    onErrorDownload("ProtocolException", e.getMessage(), e.getLocalizedMessage());
                } catch (FileNotFoundException e) {
                    onErrorDownload("FileNotFoundException", e.getMessage(), e.getLocalizedMessage());
                    e.printStackTrace();
                }
            }
        });
        _thread.start();
        _thread.setName(_urlLink);
    }

    public static void createDownload(InfoConnection infoConnection, boolean isReturn) throws ProtocolException, FileNotFoundException {
        {
            try {

                URL url1 = new URL(infoConnection.getUrl());
                 urlRe = url1;
                HttpURLConnection httpURLConnection = (HttpURLConnection)url1.openConnection();
                _httpURLConnection = httpURLConnection;
                if(httpURLConnection != null)
                {
                    httpURLConnection.setConnectTimeout(30 * 1000);
                    httpURLConnection.setRequestMethod("GET");

                    httpURLConnection.connect();

                    int responseCode = httpURLConnection.getResponseCode();
                    messageRespone = httpURLConnection.getResponseMessage();
                    codestatusRe = responseCode;

                   if(responseCode ==HttpURLConnection.HTTP_OK)
                    {
                        String newPath = infoConnection.getPathFile();

                        String pathFile = infoConnection.getPathFile() + "monkey";
                        _pathFileDelete = pathFile;
                        if(pathFile != null)
                        {
                            File foder = new File(pathFile);
                            _fileDelete = foder;
                            if(foder != null)
                            {
                                FileOutputStream fileOutput = new FileOutputStream(foder);
                                if(fileOutput != null)
                                {
                                    InputStream inputStream = new BufferedInputStream(url1.openStream());
                                    if(inputStream != null)
                                    {
                                        String lengthFile = httpURLConnection.getHeaderField("Content-Length");

                                        byte[] buffer = new byte[(int) 1024];
                                        int bufferLength = 0;
                                        int bufferLength2 = 0;
                                        double precenBegin = 0.0;

                                        if(lengthFile != null && lengthFile != "0")
                                        {
                                            long contentFileSizeDownload = Long.parseLong(httpURLConnection.getHeaderField("Content-Length"));

                                            while ( (bufferLength = inputStream.read(buffer)) > 0 ) {
                                                Log.d("MYINT", "size: " + bufferLength);
                                                bufferLength2 = bufferLength2 + bufferLength;

                                                double precen = (double)bufferLength2 / (double)contentFileSizeDownload;

                                                //if(abs(precen - precenBegin) > 0.05 || bufferLength2 == contentFileSizeDownload)
                                                {
                                                    precenBegin = precen;
                                                    processEvent(bufferLength2,contentFileSizeDownload);
                                                }
                                                fileOutput.write(buffer, 0, bufferLength);
                                            }

                                            fileOutput.close();
                                            if(bufferLength2 == contentFileSizeDownload)
                                            {
                                                File newFile = new File(newPath);

                                                codestatusRe = -1;
                                                messageRespone = "-1";
                                                urlRe = null;

                                                if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                                                    Path old = Paths.get(pathFile);
                                                    Path newF = Paths.get(newPath);

                                                    Files.move(old, newF);
                                                    onSucessEvent();
                                                }else{
                                                    if(foder.renameTo(newFile))
                                                    {
                                                        onSucessEvent();
                                                    }else{
                                                        if(!isReturn)
                                                        {
                                                            deleteFile(_fileDelete);
                                                            onErrorEvent(null);
                                                        }
                                                        else{
                                                            onErrorEvent(null);
                                                        }
                                                    }
                                                }
                                              //  onRenameFileWhenDownloadSucess(infoConnection, foder, newFile, isReturn);
                                            }else{
                                                if(!isReturn)
                                                {
                                                    deleteFile(_fileDelete);
                                                    createDownload(infoConnection, true);
                                                }
                                                else{
                                                    onErrorEvent(null);
                                                }
                                            }
                                        }else{

                                            int count = 0;
                                            long numberT = 100;
                                            while ( (bufferLength = inputStream.read(buffer)) > 0 ) {
                                                count =+ 2;
                                                if(count == 100)
                                                {
                                                    count = 99;
                                                }
                                                double precen = (double)count / (double)numberT;
                                                if(abs(precen - precenBegin) > 0.1)
                                                {
                                                    precenBegin = precen;
                                                    processEvent(bufferLength2,numberT);
                                                }
                                                fileOutput.write(buffer, 0, bufferLength);
                                            }
                                            processEvent((int)numberT,numberT);
                                            fileOutput.close();

                                            File newFile = new File(newPath);

                                            codestatusRe = -1;
                                            messageRespone = "-1";
                                            urlRe = null;

                                            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                                                Path old = Paths.get(pathFile);
                                                Path newF = Paths.get(newPath);

                                                Files.move(old, newF);
                                                onSucessEvent();
                                            }else {
                                                if(foder.renameTo(newFile))
                                                {
                                                    onSucessEvent();
                                                }else{
                                                    if(!isReturn)
                                                    {
                                                        deleteFile(_fileDelete);
                                                        onErrorEvent(null);
                                                    }
                                                    else{
                                                        onErrorEvent(null);
                                                    }
                                                }
                                            }

                                           // onRenameFileWhenDownloadSucess(infoConnection, foder, newFile, isReturn);
                                        }
                                    }
                                    else {
                                        if(codestatusRe == -1 || messageRespone == "-1" || urlRe == null)
                                        {
                                            onErrorDownload("input_stream_error", infoConnection.getUrl(), infoConnection.getPathFile());
                                        }else{
                                            onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                                        }
                                    }
                                }else{

                                    if(codestatusRe == -1 || messageRespone == "-1" || urlRe == null)
                                    {
                                        onErrorEvent(null);
                                        disConnectDownload();
                                    }else{
                                        onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                                    }
                                }
                            }
                        }else {

                            if(codestatusRe == -1 || messageRespone == "-1" || urlRe == null)
                            {
                                onErrorDownload("path_file_error", infoConnection.getUrl(), infoConnection.getPathFile());
                            }else{
                                onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                            }
                        }
                    }else{
                           if(codestatusRe == -1 || messageRespone == "-1" || urlRe == null)
                           {
                               onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                               httpURLConnection.disconnect();
                           }else{
                               onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                           }
                       }

                    _pathFileDelete = "";
                    _fileDelete = null;

                    httpURLConnection = null;
                }else{
                    onErrorEvent(null);
                }
            } catch (IOException e) {
                if(codestatusRe == -1 || messageRespone == "-1" || urlRe == null)
                {
                    onErrorDownload("write_file_download_error", e.getMessage(), e.getLocalizedMessage());
                }else{
                    if(codestatusRe == 200)
                    {
                        onErrorEvent(null);
                    }else{
                        onDownloadError(String.valueOf(codestatusRe), messageRespone, urlRe);
                    }
                }
                e.printStackTrace();
            }
        }
    }

    private static void onRenameFileWhenDownloadSucess(InfoConnection infoConnection, File fileOld, File newFile, boolean isReturn)
    {
        codestatusRe = -1;
        messageRespone = "-1";
        urlRe = null;

        if(fileOld.renameTo(newFile))
        {
            onSucessEvent();
            disConnectDownload();
        }else{
            if(!isReturn)
            {
                deleteFile(_fileDelete);
                onErrorEvent(null);
            }
            else{
                onErrorEvent(null);
            }
        }

    }

    public static void onDownloadError(String code, String message, URL url)
    {
        codestatusRe = -1;
        messageRespone = "-1";
        urlRe = null;
        if(code == "-1" || message == "-1" || url == null)
        {
            onErrorEvent(null);
            return;
        }
        String ipAddress = "None";
        String host = "None";

        if(url != null)
        {
            host = url.getHost();
        }

        InetAddress address = null;
        try {
            if(host != "None")
            {
                address = InetAddress.getByName(host);
            }
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        if(address != null)
        {
            ipAddress = address.getHostAddress();
        }

        String [] data = new String[4];

        data[0] = code;
        data[1] = ipAddress;
        data[2] = message;
        data[3] = "lost_connection";
        onErrorEvent(data);
        disConnectDownload();
    }

    private static void deleteFile(File file)
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

    public static void disConnectDownload()
    {
        if(_thread != null)
        {
            _thread.interrupt();
            _thread = null;
        }
        codestatusRe = -1;
        messageRespone = "-1";
        urlRe = null;
        if(_httpURLConnection != null)
        {
            _httpURLConnection.disconnect();
        }

        deleteFile(_fileDelete);
    }

    public static void onErrorDownload(String nameError, String message1, String message2)
    {
        codestatusRe = -1;
        messageRespone = "-1";
        urlRe = null;

        String copyMessage1 = "None";
        String copyMessage2 = "None";

        if(message1 != null)
        {
            byte[] encodeMessage1 = Base64.encodeBytesToBytes(message1.getBytes());
            copyMessage1 = new String(encodeMessage1, Charset.forName("UTF-8"));
        }

        if(message2 != null)
        {
            byte[] encodeMessage2 = Base64.encodeBytesToBytes(message1.getBytes());
            copyMessage2 = new String(encodeMessage2, Charset.forName("UTF-8"));
        }
        String [] data = new String[3];
        data[0] = copyMessage1;
        data[1] = copyMessage2;
        data[2] = nameError;
        onErrorEvent(data);
        disConnectDownload();
    }

    public static void unpackArchive(HttpURLConnection http, String filePath) throws IOException {
        File zipFile = new File(filePath);
        ZipFile zip = new ZipFile(zipFile);
        InputStream in = http.getInputStream();
        ZipInputStream zis = new ZipInputStream(in);
        ZipEntry entry;
        while ((entry = zis.getNextEntry()) != null) {
            System.out.println("entry: " + entry.getName() + ", "
                    + entry.getSize());
            BufferedReader bufferedeReader = new BufferedReader(new InputStreamReader(zip.getInputStream(entry)));
            String line = bufferedeReader.readLine();
            while (line != null) {
                System.out.println(line);
                line = bufferedeReader.readLine();
            }
            bufferedeReader.close();
        }
    }

}
