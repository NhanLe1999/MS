package org.cocos2dx.cpp;
import android.util.Log;
import com.mixpanel.android.mpmetrics.MixpanelAPI;
import org.json.JSONObject;

/**
 * Created by tongoclinh on 12/14/17.
 */

public class MixpanelPlugin {

    static MixpanelAPI mixpanel;

    static void init() {
        mixpanel = MixpanelAPI.getInstance(AppActivity._ac, "e145b4844f53c063c4249c09d797308e");
    }

    static void logEvent(String name, String param) {
        try {
            Log.d("Mixpanel", param);
            JSONObject props = new JSONObject(param);
            mixpanel.track(name, props);
        } catch (Exception e) {
            Log.d("Mixpanel", "Can not log event");
        }
    }

    static void setIdentifier(String identifier) {
        mixpanel.identify(identifier);
    }
}
