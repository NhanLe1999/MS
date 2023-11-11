/*
 * Created: 01/01/2021 by QuanPA
 * TODO: Enable Firebase Database on Huawei devices
 */

package org.cocos2dx.cpp;

import android.service.autofill.Dataset;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.google.android.gms.tasks.Task;
import com.google.firebase.database.*;
import com.google.firebase.database.snapshot.IndexedNode;
import com.google.firebase.encoders.json.JsonDataEncoderBuilder;
import com.google.gson.JsonObject;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

class UserSetting {

}

public class MMFirebase implements DatabaseReference.CompletionListener {
    // Variables
     static private String firebaseURL = "https://mj-stories-169807-6c045.firebaseio.com/";
     static private FirebaseDatabase database;
     static private DatabaseReference dRef;     // a reference of database
     static private boolean isUserExist = false;
    static private String USER_REALTIME_VARIABLE = "realtime_variable";

     static private String keyRealTimeVariable = "realtime_variable";
     static private String keySkipCounterEnterInfo = "accountv3_skip_counter_enter_info";

     static private int currentProfileID;

     // Native methods
     static protected native void onCompletelyRetrieveData(DataSnapshot snapshot);
     static protected native void onCancelRetrieveData();
     static protected native void setValueForKey(String key, String value, int type);
     //static protected native void loadSkipCountNumber(String value);
     //static protected native void loadProfileInfo(String name, String avatar, int gender, int age, int profileID);
     static protected native void switchProfile(int profileID);
    // static protected native void loadProfileKey(String key, String value);
    // static protected native void loadLessonLearned(int profileID, int ID, int category);
   //  static protected native void loadStory(int profileID, int id, int timeRead);
     //static protected native void loadAudio(int profileID, int id, int time);
     //static protected native void loadLoveStory(int profileID, int id);
     //static protected native void loadLoveAudioBook(int profileID, int id);
     static protected native void callback();
     static protected native void voidStringCallBack(Boolean bool);
     //static protected native void switchCurrentProfileID();
     static protected native void CallVoidCallBack(int callbakc_id);

     static protected native void onCompleteReadData(String snapshot);

        // static methods
     static  public void getFirebase() {
        // Create an instance of Firebase Database with given URL
        database = FirebaseDatabase.getInstance(firebaseURL);
    }

    static public void getReference(int userId) {
         // Get Reference of a given User ID
        if (database == null) {
            // Firebase initialize failed.
            return ;
        }
        String urlRef =  "users/"+ Integer.toString(userId) + "/";
        dRef = database.getReference(urlRef);
        dRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                if (snapshot.getValue() == null) {
                    voidStringCallBack(false);
                } else {
                    /*
                     * From DataSnapShot, create a JSON object
                     * Pass created json object to a C++ function, this C++ function
                     *  will get data from json object and save to local database.
                     * JSON object contains information about user settings, user info, ...
                     * Each child of JSON object (user settings, ...) will be an array of keys and values, include the
                     *  case that child has only one or no property.
                     * For example:
                     *  user_id {
                     *    property:  { [ // array of keys and values
                     *          { key_1: value_1},
                     *          {key_2: value_2},
                     *          ...
                     *      ] },
                     *
                     *      ...
                     *
                     * }
                     */
                    JSONObject jsonObject = new JSONObject();
                    for(DataSnapshot child : snapshot.getChildren()) {
                        if(child.getKey().equals("user_settings")) {
                            GetUserSetting(child, jsonObject);
                        } else if(child.getKey().equals("user_info")) {
                            GetUserInfo(child, jsonObject);
                        } else if(child.getKey().equals(USER_REALTIME_VARIABLE)) {
                            GetRealTimeVariable(child, jsonObject);
                        } else if(child.getKey().equals("profiles")) {
                            GetUserProfiles(child, jsonObject);
                        }
                    }
                    onCompleteReadData(jsonObject.toString());
                }
            }

            @Override
            public void onCancelled( DatabaseError error) {
                voidStringCallBack(false);
            }
        });

    }

    @Override
    public void onComplete(DatabaseError error, DatabaseReference ref) {

    }

    static public  void SwitchProfile(int profileID) {
         switchProfile(profileID);
    }

//    static private void onCompletelyReadData(DataSnapshot snapshot) {
//        /*
//         * Completely read data from firebase
//         * Now use this data to set up user's settings.
//         */
//        try {
//            if(snapshot.getValue() == null) {
//                voidStringCallBack(false); ;
//            } else {
//                // Load users settings
//                if(snapshot.hasChild("user_settings")) {
//                    loadUserSetting(snapshot.child("user_settings"));
//                }
//
//                // Load cái count gì gì đó ở đây
//                if(snapshot.hasChild(keyRealTimeVariable)) {
//                    DataSnapshot checkSkipCount = snapshot.child(keyRealTimeVariable);
//                    if(checkSkipCount.hasChild(keySkipCounterEnterInfo)) {
//                        loadSkipCountNumber(checkSkipCount.child(keySkipCounterEnterInfo));
//                    }
//                }
//
//                // Load data for each profile
//
//                if(snapshot.hasChild("profiles")) {
//                    for(DataSnapshot child : snapshot.child("profiles").getChildren()) {
//                        int profileID = Integer.parseInt(child.getKey());
//                        if(profileID != 0) {
//                            // Change the current profileID to the profile that needs loading
//                            switchProfile(profileID);
//
//                            // Load profile info
//                            if(child.hasChild("profile_info")) {
//                                DataSnapshot profileInfo = child.child("profile_info");
//                                loadProfileInfo(profileInfo.child("name").getValue().toString(), profileInfo.child("avatar").getValue().toString(),
//                                        Integer.parseInt(profileInfo.child("gender").getValue().toString()), Integer.parseInt(profileInfo.child("age").getValue().toString()), profileID);
//
//                            }
//
//                            // Load profile keys
//                            if(child.hasChild("process_keys")) {
//                                DataSnapshot profileKeys = child.child("process_keys");
//                                loadProfileKeys(profileKeys);
//                            }
//
//                            // Load learned lessons, stories, audio books and love audio books
//                            String[] keyWord = { "lessons", "stories", "audiobooks", "love_stories", "love_audiobooks"};
//                            for(int type = 0; type < keyWord.length; type++) {
//                                loadIntForIntKey(child, profileID, type, keyWord[type]);
//                            }
//
//                        }
//                    }
//                }
//
//                // Switch back to using profileID
////                switchCurrentProfileID();
//
//                voidStringCallBack(true);
//            }
//        } catch (Exception e) {
//            String msg = e.getMessage();
//            System.out.println(msg);
//        }
//
//
//    }

    /**** TODO: LOAD USER'S DATA FROM FIREBASE DATABASE  ****/

    static private void loadUserSetting(DataSnapshot userSettings) {
        /*
         * Bring keys and values from userSetting into local database
         * keys are strings; values are integer, boolean or string.
         */
        try {
            for(DataSnapshot child : userSettings.getChildren()) {
                if(child.exists()) {
                    int type = 0;
                    String key = child.getKey();
                    Object value = child.getValue();
                    if(value instanceof  Integer || value instanceof Long) {
                        type = 1;
                    } else if (value instanceof  String) {
                        type = 0;
                    } else if(value instanceof Boolean) {
                        type = 2;
                    }

                    setValueForKey(key, value.toString(), type);
                }
            }
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }

    }


    static private void loadAudios(DataSnapshot audiobooks, int profileID) {
         for(DataSnapshot audiobook : audiobooks.getChildren()) {

         }
    }

    static private boolean isNumeric(String string) {
        if(string.length() == 0) {
            return false;
        }

        for(int i = 0; i < string.length(); i++) {
            Character c = string.charAt(i);
            if(!Character.isDigit(c)) {
                return false;
            }
        }

        return true;
    }

    static private void loadIntForIntKey(DataSnapshot parent, int profileID, int type, String keyWord) {
         /*
          * Load Integer value for Integer key
          * Depend on type
          *  type = 0: load learned lessons
          *  type = 1: load stories
          *  type = 2: load audio books
          *  type = 3: load love stories
          *  type = 4: load love audio books
          * key's type must be string and value's type must be integer, or else app crashes.
          * To prevent app from crashing when load or update with wrong data type, create some default cases, like:
          *  - if cannot parse value from string to integer, then do nothing
          *  - ...
          */

        try {
            if(parent.hasChild(keyWord)) {
                DataSnapshot fields = parent.child(keyWord);
                for(DataSnapshot child : fields.getChildren()) {
                    int key = 0, value = 0;

                    // Check key and value validate, if not, do nothing
//                    if(!isNumeric(child.getKey()) || !isNumeric(child.getValue().toString())) {
//
//                        return;
//                    }

                    key = Integer.parseInt(child.getKey());
                    value = Integer.parseInt(child.getValue().toString());

                }
            }
        } catch(Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    /***** TODO: UPDATE USER DATA IN FIREBASE DATABASE *****/
    /*
     * For each upload, we need the url that references to location in firebase database
     * Pair(s) of key (String type) and value (Object)
     */

    static public void updateTrackingWorksheetsSended(String urlRef, String key, String value) {
        /*
         * A C++ method calls this method
         * Update data specified by urlRef
         * Then call a created callback in C++ environment
         */
        try {
            HashMap<String, Object> updateRef = new HashMap<String, Object>();
            updateRef.put(key, value);
            dRef = database.getReference(urlRef);
            dRef.updateChildren(updateRef, new DatabaseReference.CompletionListener() {
                @Override
                public void onComplete(DatabaseError error,DatabaseReference ref) {
                    callback();
                }

            });
        } catch(Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static public void updateTrackingAndLearnedLesson(String key, String value, final boolean callback) {
        try {
            dRef = database.getReference();
            HashMap<String, Object> updateRef = new HashMap();
            updateRef.put(key, Integer.parseInt(value));
            dRef.updateChildren(updateRef, new DatabaseReference.CompletionListener() {
                @Override
                public void onComplete(@Nullable DatabaseError error, @NonNull DatabaseReference ref) {
                    if(callback) {
                        callback();
                    }
                }
            });
        } catch(Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static public void UpdateChild(String JSONString, final int callbackID) {
        try {
            dRef = database.getReference();
            JSONObject jsonObject = new JSONObject(JSONString);
            HashMap<String, Object> updateRef = new HashMap<String, Object>();
            for (Iterator<String> it = jsonObject.keys(); it.hasNext(); ) {
                String key = it.next();
                Object value = jsonObject.get(key);
                updateRef.put(key, value);
            }

            if(!updateRef.isEmpty()) {
                dRef.updateChildren(updateRef, new DatabaseReference.CompletionListener() {
                    @Override
                    public void onComplete(@Nullable DatabaseError error, @NonNull DatabaseReference ref) {
                        if(callbackID >= 0) {
                            CallVoidCallBack(callbackID);
                        }
//                      dRef.removeValue(this);
                        dRef = null;
                    }
                });
            }
        } catch (Exception e) {
            dRef = null;
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static public void removeChild(String path) {
        /*
         * set value at path'location to null
         */

        try {
            dRef = database.getReference(path);
            dRef.removeValue(new DatabaseReference.CompletionListener() {
                @Override
                public void onComplete(@Nullable DatabaseError error, @NonNull DatabaseReference ref) {
                    callback();
                }
            });
        } catch(Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void setCurrentProfileID(int profileID) {
        currentProfileID = profileID;
    }

    static private void GetProcessKeys(DataSnapshot snapshot, JSONArray json) {
        /*
         * Load process_keys
         * Each property is a string.
         */

        JSONArray keys = new JSONArray();
        try {
            for(DataSnapshot child: snapshot.getChildren()) {
                JSONObject value = new JSONObject();
                value.put(child.getKey(), child.getValue().toString());
                json.put(value);
            }
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void GetUserSetting(DataSnapshot snapshot, JSONObject json) {
        /*
         * Get user setting from snap shot and save in json-form in json.
         * each user setting is a string value.
         */
        JSONArray settings = new JSONArray();
        try {
            for(DataSnapshot child : snapshot.getChildren()) {
                JSONObject value = new JSONObject();
                try {
                    value.put(child.getKey().toString(), child.getValue().toString());
                    settings.put(value);
                } catch (Exception e) {
                    String msg = e.getMessage();
                    System.out.println(msg);
                }

            }

            json.put("user_settings", (Object) settings);
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void GetUserInfo(DataSnapshot snapshot, JSONObject json) {
        /*
         * Get use info from snap shot and save in json with JSON-FROM
         * user info is an array of string value
         */

        JSONArray info = new JSONArray();
        try {
            for(DataSnapshot child : snapshot.getChildren()) {
                JSONObject value = new JSONObject();
                value.put(child.getKey().toString(), child.getValue().toString());
                info.put(value);
            }

            json.put("user_info", (Object) info);
        } catch(Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void GetRealTimeVariable(DataSnapshot snapshot, JSONObject json) {
        /*
         * Get real time variable from snapshot and save in json by JSON-FORM
         * realtime variable is a string
         */
        JSONArray realTimeVar = new JSONArray();
        JSONObject containter = new JSONObject();
        try {
           for(DataSnapshot child : snapshot.getChildren()) {
               JSONObject value = new JSONObject();
               value.put(child.getKey(), Integer.parseInt(child.getValue().toString()));
               realTimeVar.put(value);
           }

//           containter.put(USER_REALTIME_VARIABLE, realTimeVar);
           json.put(USER_REALTIME_VARIABLE, (Object) realTimeVar);
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void LoadProfileInfo(DataSnapshot snapshot, JSONArray json) {
//        if(profileID > 0) {
        try {
            if(snapshot.hasChild("name")) {
                JSONObject tmp = new JSONObject();
                tmp.put("name", snapshot.child("name").getValue().toString());
                json.put(tmp);
            }
            if(snapshot.hasChild("avatar")) {
                JSONObject tmp = new JSONObject();
                tmp.put("avatar", snapshot.child("avatar").getValue().toString());
                json.put(tmp);
            }
            if(snapshot.hasChild("age")) {
                JSONObject tmp = new JSONObject();
                tmp.put("age", Integer.parseInt(snapshot.child("age").getValue().toString()));
                json.put(tmp);
            }
            if(snapshot.hasChild("gender")) {
                JSONObject tmp = new JSONObject();
                tmp.put("gender", Integer.parseInt(snapshot.child("gender").getValue().toString()));
                json.put(tmp);
            }
            if(snapshot.hasChild("levelinfo_js")) {
                JSONObject tmp = new JSONObject();
                tmp.put("levelinfo_js", snapshot.child("levelinfo_js").getValue().toString());
                json.put(tmp);
            }
            if(snapshot.hasChild("setting_js")) {
                JSONObject tmp = new JSONObject();
                tmp.put("setting_js", snapshot.child( "setting_js").getValue().toString());
                json.put(tmp);
            }

        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void LoadIntForKey(DataSnapshot snapshot, JSONArray json) {
        /*
         * Load Lessons, Audiobooks, love_audiobooks, ...
         * Each property has an integer value
         */
        try {
            for(DataSnapshot child : snapshot.getChildren()) {
                JSONObject tmp = new JSONObject();
                tmp.put(child.getKey(), child.getValue().toString());
                json.put(tmp);
            }
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }

    static private void GetUserProfiles(DataSnapshot snapshot, JSONObject json) {
        /*
         * Each profile contains
         * * profile_info
         * * lesson
         * * process key
         * * ...
         * For each profile, load profile's data.
         */
        JSONArray profile = new JSONArray();
        try {
            for(DataSnapshot child : snapshot.getChildren()) {

                String profileID = child.getKey();
                JSONArray user = new JSONArray();
                JSONObject userContainer = new JSONObject();

                if(child.hasChild("profile_info")) {
                    JSONArray profileInfo = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadProfileInfo(child.child("profile_info"), profileInfo);
                    container.put("profile_info", (Object) profileInfo);
                    user.put(container);
                }

                if(child.hasChild("lessons")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadIntForKey(child.child("lessons"), jsonArray);
                    container.put("lessons", (Object) jsonArray);
                    user.put(container);
                }

                if(child.hasChild("stories")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadIntForKey(child.child("stories"), jsonArray);
                    container.put("stories", (Object) jsonArray);
                    user.put(container);
                }

                if(child.hasChild("love_stories")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadIntForKey(child.child("love_stories"), jsonArray);
                    container.put("love_stories", (Object) jsonArray);
                    user.put(container);
                }

                if(child.hasChild("audiobooks")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadIntForKey(child.child("audiobooks"), jsonArray);
                    container.put("audiobooks", (Object) jsonArray);
                    user.put(container);
                }

                if(child.hasChild("love_audiobooks")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    LoadIntForKey(child.child("love_audiobooks"), jsonArray);
                    container.put("love_audiobooks", (Object) jsonArray);
                    user.put(container);
                }

                if(child.hasChild("process_keys")) {
                    JSONArray jsonArray = new JSONArray();
                    JSONObject container = new JSONObject();
                    GetProcessKeys(child.child("process_keys"), jsonArray);
                    container.put("process_keys", (Object) jsonArray);
                    user.put(container);
                }

                userContainer.put(profileID, user);
                profile.put(userContainer);
            }

            json.put("profiles", profile);
        } catch (Exception e) {
            String msg = e.getMessage();
            System.out.println(msg);
        }
    }
//    }
}
