package com.earlystart.android.monkeyjunior.ai_speaking.camera_manager.media_activity;


import android.content.Context;
import android.content.res.AssetManager;
import android.net.Uri;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

public class VideoManager {
    Context mContext;
    public VideoManager(Context context){
        mContext = context;
    }
    public void mergeMultiVideo() {
        AssetManager am = mContext.getAssets();
        List<String> mapList;
        try {
            mapList = Arrays.asList(am.list(""));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Uri uri = Uri.fromFile(new File("//assets/user_video0.mp4"));
        String testfilepath = uri.getPath();
        File file = new File(testfilepath);
        File extractFile = mContext.getFileStreamPath("user_video0.mp4");
        boolean fileexit = file.exists();
//Do something
        String setInputCommand = "ffmpeg -i a.mp4 -i user_video0.mp4 -i image.jpg -i main.jpg -filter_complex  out11.mp4";
        String settingComand = '"'+ "[0]scale=722:-1[hi];[hi]format=yuva420p,geq=lum='p(X,Y)':a='if(gt(abs(W/2-X),W/2-20)*gt(abs(H/2-Y),H/2-20)," +
        "if(lte(hypot(20-(W/2-abs(W/2-X)),20-(H/2-abs(H/2-Y))),20),255,0),255)'[vid];[3][vid]overlay=159:112[bgandv1];"+
        "[1]setpts=PTS+5/TB[fg1];[fg1]transpose=1[fg];[fg]scale=722:-1[hix];"+
        "[hix]format=yuva420p,geq=lum='p(X,Y)':a='if(gt(abs(W/2-X),W/2-20)*gt(abs(H/2-Y),H/2-20),if(lte(hypot(20-(W/2-abs(W/2-X)),20-(H/2-abs(H/2-Y))),20),255,0),255)'[vid1];"+
        "[bgandv1][vid1]overlay=1035:112[bg];[2]scale=-1:435[hix1];"+
        "[hix1]format=yuva420p,geq=lum='p(X,Y)':a='if(gt(abs(W/2-X),W/2-20)*gt(abs(H/2-Y),H/2-20),if(lte(hypot(20-(W/2-abs(W/2-X)),20-(H/2-abs(H/2-Y))),20),255,0),255)'[bg2];"+
        "[bg][bg2]overlay=666:584" + '"';
        String setOutputCommand = "out11.mp4";

        String finalCommand = setInputCommand + settingComand + setOutputCommand;
    }
}
