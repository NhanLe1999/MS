package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ProScoreAPI{
    public static native void onRequestSuccess( String dsa);
    public static native void onRequestFailure();
    public static native boolean isDevEnviroment();
    public static void sendPostRequest(String data)  {
        HashMap<String,String> params = new HashMap<String,String>();
        String[] bodyParts = data.split("&");
        for(int i=0;i<bodyParts.length; i++){
            System.out.println("----------------------"+bodyParts[i]);
        }
        for(int i =0 ;i < bodyParts.length-1;i++){ //last element is file
            params.put(bodyParts[i].split("=")[0],bodyParts[i].split("=")[1]);
        }
        String charset = "UTF-8";
        String requestURL = false ?  "https://api.dev.monkeyuni.com/lesson/api/v1/speech/verification?is_web=1&app_id=2" : "https://app.monkeyuni.net/lesson/api/v1/speech/verification?is_web=1&app_id=2";
        System.out.println(requestURL);
        String responseString="nullllllllll";
        try{
            MultipartUtility multipart = new MultipartUtility(requestURL, charset);
            for(Map.Entry<String, String> entry : params.entrySet()) {
                String key = entry.getKey();
                String value = entry.getValue();
                multipart.addFormField(key, value);
            }

            String fileKey = bodyParts[bodyParts.length-1].split("=")[0];
            String filePath = bodyParts[bodyParts.length-1].split("=")[1];
            try{
                multipart.addFilePart(fileKey, new File(filePath));
            }catch (FileNotFoundException e){
                System.out.println("--------------------something wrong with file path");
            }
            List<String> response = multipart.finish();
            System.out.println("SERVER REPLIED:");
            for (String line : response) {
                System.out.println("Upload Files Response:::" + line);
                responseString = line;
            }
            onRequestSuccess(responseString);
        }catch (Exception e){
            onRequestFailure();
            System.out.println("Errrrrrrrrrrrrrr:" + e.toString());

        }
    }
}
