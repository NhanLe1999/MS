package org.cocos2dx.cpp;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.zip.GZIPInputStream;

public class ReadRequest {

    static protected native void onRequestSucess(String respone, Boolean isSucess);
    static protected native void onRequestError(int code, String message, String respone);

    private static void getRequest(String url, String header1, String header2)
    {
        URL url1 = null;
        try { url1 = new URL(url);

            HttpURLConnection httpURLConnection = (HttpURLConnection)url1.openConnection();

            if(httpURLConnection != null) {

                String[] valueHeader1 = header1.split(":");
                String[] valueHeader2 = header2.split(":");

                if(valueHeader1.length == 2)
                {
                    httpURLConnection.setRequestProperty(valueHeader1[0], valueHeader1[1]);
                }
                if(valueHeader2.length == 2)
                {
                    httpURLConnection.setRequestProperty(valueHeader2[0], valueHeader2[1]);
                }

                httpURLConnection.setConnectTimeout(30 * 1000);
                httpURLConnection.setRequestMethod("GET");
                httpURLConnection.connect();

                System.out.println(httpURLConnection.getResponseCode());

                int reponeCode = httpURLConnection.getResponseCode();

                if(reponeCode == HttpURLConnection.HTTP_OK)
                {
                    BufferedReader in = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream()));
                    String inputLine;
                    StringBuffer response = new StringBuffer();

                    while ((inputLine = in.readLine()) != null) {
                        response.append(inputLine);
                    }
                    in.close();

                    if(response != null)
                    {
                        onRequestSucess(response.toString(), true);
                    }
                    String re = response.toString();
                }
                else{
                    //error error code != 200
                    onRequestError(reponeCode, httpURLConnection.getResponseMessage(), "None");
                    //onRequestSucess("error error code != 200" + String.valueOf(reponeCode), false);
                }
                httpURLConnection.disconnect();
            }
            else{
              //  onRequestSucess("httpURLConnection null", false);
                onRequestError(-1, "httpURLConnection null", "None");
                //errror
                //httpURLConnection null
            }


        } catch (MalformedURLException e) {
            //error catch
            onRequestError(-1, "cath MalformedURLException", "None");
           // onRequestSucess(e.getMessage(), false);
            e.printStackTrace();
        } catch (IOException e) {
            //error catchonRequestError
            onRequestError(-1, "catch IOException", "None");

            //onRequestSucess(e.getMessage(), false);
            e.printStackTrace();
        }
    }

    private static void postRequest(String url, String urlParameters, String header1, String header2)
    {
        byte[] postData       = urlParameters.getBytes(StandardCharsets.UTF_8 );
        int    postDataLength = postData.length;

        String[] valueHeader1 = header1.split(":");
        String[] valueHeader2 = header2.split(":");

        URL url1 = null;
        try {
            url1 = new URL(url);

            if(url1 != null)
            {
                HttpURLConnection httpURLConnection = (HttpURLConnection)url1.openConnection();

                if(httpURLConnection != null)
                {
                    if(valueHeader1.length == 2)
                    {
                        httpURLConnection.setRequestProperty(valueHeader1[0], valueHeader1[1]);
                    }
                    if(valueHeader2.length == 2)
                    {
                        httpURLConnection.setRequestProperty(valueHeader2[0], valueHeader2[1]);
                    }
                    httpURLConnection.setDoOutput( true );
                    httpURLConnection.setInstanceFollowRedirects( false );
                    httpURLConnection.setRequestMethod( "POST" );
                    httpURLConnection.setRequestProperty( "Content-Length", Integer.toString( postDataLength ));
                    httpURLConnection.setUseCaches( false );
                    try( DataOutputStream wr = new DataOutputStream( httpURLConnection.getOutputStream())) {
                        wr.write( postData );
                    }

                    int statusCode = -1;
                    statusCode = httpURLConnection.getResponseCode();
                    String post = "", message = "";
                    message = httpURLConnection.getResponseMessage();
                    if (httpURLConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {

                        try (BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream()))) {
                            String line;
                            while ((line = bufferedReader.readLine()) != null) {
                                post = post + line;
                            }
                        }
                    }
                    else{
                        //error != 200
                        onRequestSucess("error error code != 200" + String.valueOf(httpURLConnection.getResponseCode()), false);
                    }

                    if(post != "")
                    {
                        onRequestSucess(post, true);
                    }
                    else{
                        onRequestError(statusCode, httpURLConnection.getResponseMessage(), "NONE");
                   //     onRequestSucess("post null", false);
                        //error != 200
                    }
                }
                httpURLConnection.disconnect();
                String post1 = postData.toString();
                int kk = 0;
            }


        } catch (MalformedURLException e) {
            //catch
            onRequestError(-1, "MalformedURLException", "none");
         //   onRequestSucess(e.getMessage(), false);
            e.printStackTrace();
        } catch (ProtocolException e) {
            //catch
            onRequestError(-1, "ProtocolException", "none");
            //onRequestSucess(e.getMessage(), false);
            e.printStackTrace();
        } catch (IOException e) {
            //catchProtocolException
            onRequestError(-1, "IOException", "none");
           // onRequestSucess(e.getMessage(), false);
            e.printStackTrace();
        }
    }

    public static void createReadRequest(String url, int typeReQuest, String parameters, String header1, String header2)
    {
        if(typeReQuest == 0)
        {
            getRequest(url, header1, header2);
        }else{
            postRequest(url, parameters, header1, header2);
        }
    }

}
