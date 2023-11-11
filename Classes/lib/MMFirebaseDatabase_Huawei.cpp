
#include "MMFirebaseDatabase_Huawei.h"
#include <firebase/app.h>
#include <firebase/variant.h>
#include <firebase/database/database_reference.h>
#include "cocos2d.h"
#include "APProfileManager.h"
#include "MJDefault.h"
#include "APDatabase.h"
#include "MJDefinitions.h"
#include "manager/GettingUserInfoManager.h"
#include "StoryConfigManager.h"
#include "Key_Default_Model.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define USER_REALTIME_VARIABLE                      "realtime_variable"
#define USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY  "accountv3_skip_counter_enter_info"
#define USER_EXIST_TIMEOUT                          10.0f
#define RETRIEVE_DATA_FIREBASE_TIMEOUT_SCHEDULE     "retrieve_data_firebase_timeout"
using namespace mm;
using namespace firebase;
USING_NS_CC;


#include "container.h"

std::function<void(bool, std::string)> callBoolString = nullptr;

std::function<void()> updateUserSettingsCallback = nullptr;                     // callback_id = 0
std::function<void()> updateTrackingWorksheetsSendedCallback = nullptr;         // id = 1
std::function<void()> updateTrackingCallback = nullptr;                         // id = 2
std::function<void()> updateTrackingAndLearnedLessonsCallback = nullptr;        // id = 3
std::function<void()> updateProfileKeysCallback = nullptr;                      // id = 4
std::function<void()> updateLearnedLessonsCallback = nullptr;                   // id = 5
std::function<void()> updateReadStoriesCallback = nullptr;                      // id = 6
std::function<void()> updateReadAudiosCallback = nullptr;                       // id = 7
std::function<void()> updateLoveStoriesCallback = nullptr;                      // id = 8
std::function<void()> updateLoveAudiosCallback = nullptr;                       // id = 9
std::function<void()> deleteLoveStoriesCallback = nullptr;                      // id = 10
std::function<void()> deleteLoveAudiosCallback = nullptr;                       // id = 11
std::function<void()> updateStoriesCallback = nullptr;                          // id = 12
std::function<void()> updateProfileInfoCallback = nullptr;                      // id = 13
std::function<void()> updateActivityLearnOfLessonPhonicCallback = nullptr;                      // id = 14
std::function<void()> updateupdateProfileKeysByKeyAndValueCallback = nullptr;                   // id = 15
std::function<void()> updateActivityLearnOfLessonInReadingComprehensionCallback = nullptr;                      // id = 16

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_callback(JNIEnv *env, jclass clazz) {
        try {
            if(callBoolString) {
                if(_voidCallBack)
                {
                    _voidCallBack();
                    _voidCallBack = nullptr;
                }
            }
        }
        catch (const char * e) {

        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_voidStringCallBack(JNIEnv *env,
                                                                               jclass clazz,
                                                                               jobject boolean) {
        if(callBoolString) {
            if(boolean) {
                callBoolString(true, "");
            } else {
                callBoolString(false, "error occurred");
            }

            callBoolString = nullptr;
        }


    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_onCompletelyRetrieveData(JNIEnv *env, jclass thiz,
                                                                                     jobject snapshot) {
        // Hàm này được gọi khi hoàn thành việc retrieve data từ Firebase
        CCLOG("Completely Retrieve Data");
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_onCancelRetrieveData(JNIEnv *env, jclass thiz) {
        // Hàm này gọi khi có vấn để với việc retrieve data từ Firebase
    }


    /*
     * jstring = utf16-string
     * std::string = utf8-string
     * Convert jstring to std::string to pass string from JNI to C++
     */
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_setValueForKey(JNIEnv *env, jclass thiz,
                                                                           jstring key,
                                                                           jstring value, jint type) {
        std::string keyString;
        std::string valueString;
        const char *charArr;
        jboolean  isCopy;

        // Get key
        charArr = env->GetStringUTFChars(key, &isCopy);
        if(charArr) {
            keyString = charArr;
            env->ReleaseStringUTFChars(key, charArr);
        }

        // Get value
        charArr = env->GetStringUTFChars(value, &isCopy);
        if(charArr) {
            valueString = charArr;
            env->ReleaseStringUTFChars(value, charArr);
        }

        int itype = (int) type;
        if(type ==  1) {
            // value is an integer
            MJDEFAULT->setIntegerForKey(keyString, stoi(valueString));
        } else if(type == 0) {
            // value is a string
            MJDEFAULT->setStringForKey(keyString, valueString);
        } else if(type == 2) {
            // value is a boolean value
            if(valueString == "false") {
                MJDEFAULT->setBoolForKey(keyString, false);
            } else if(valueString == "true") {
                MJDEFAULT->setBoolForKey(keyString, true);
            }
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_MMFirebase_CallVoidCallBack(JNIEnv *env, jclass clazz, jint callback_id) {
        std::function<void()> voidCallback = nullptr;
        if(callback_id == 0) {
            voidCallback = updateUserSettingsCallback;
            updateUserSettingsCallback = nullptr;
        } else if(callback_id == 1) {
            voidCallback = updateTrackingWorksheetsSendedCallback;
            updateTrackingWorksheetsSendedCallback = nullptr;
        } else if(callback_id == 2) {
            voidCallback =  updateTrackingCallback;
            updateTrackingCallback = nullptr;
        } else if(callback_id == 3) {
            voidCallback = updateTrackingAndLearnedLessonsCallback;
            updateTrackingAndLearnedLessonsCallback = nullptr;
        } else if(callback_id == 4) {
            voidCallback = updateProfileKeysCallback;
            updateProfileKeysCallback = nullptr;
        } else if(callback_id == 5) {
            voidCallback = updateLearnedLessonsCallback;
            updateLearnedLessonsCallback = nullptr;
        } else if(callback_id == 6) {
            voidCallback = updateReadStoriesCallback;
            updateReadStoriesCallback = nullptr;
        } else if(callback_id == 7) {
            voidCallback =  updateReadAudiosCallback;
            updateReadAudiosCallback = nullptr;
        } else if(callback_id == 8) {
            voidCallback = updateLoveStoriesCallback;
            updateLoveStoriesCallback = nullptr;
        } else if(callback_id == 9) {
            voidCallback = updateLoveAudiosCallback;
            updateLoveAudiosCallback = nullptr;
        } else if(callback_id == 10) {
            voidCallback =  deleteLoveStoriesCallback;
            deleteLoveStoriesCallback = nullptr;
        } else if(callback_id == 11) {
            voidCallback = deleteLoveAudiosCallback;
            deleteLoveAudiosCallback = nullptr;
        } else if(callback_id == 12) {
            voidCallback = updateStoriesCallback;
        } else if(callback_id == 13) {
            voidCallback =  updateProfileInfoCallback;
            updateProfileInfoCallback = nullptr;
        }else if(callback_id == 14) {
            voidCallback =  updateActivityLearnOfLessonPhonicCallback;
            updateActivityLearnOfLessonPhonicCallback = nullptr;
        }else if(callback_id == 15) {
            voidCallback =  updateupdateProfileKeysByKeyAndValueCallback;
            updateupdateProfileKeysByKeyAndValueCallback = nullptr;
        }else if(callback_id == 16) {
            voidCallback =  updateActivityLearnOfLessonInReadingComprehensionCallback;
            updateActivityLearnOfLessonInReadingComprehensionCallback = nullptr;
        }

        if(voidCallback) {
            voidCallback();
        }
    }

std::string RealtimeDatabaseHuawei::createJSONString(rapidjson::Document &document, std::vector<std::pair<std::string, Variant>> pair, std::vector<int> type) {
    std::map<std::string, Variant>::iterator it;
    int i = 0;
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    document.SetObject();
    for(i = 0; i < pair.size(); i++) {
        std::string keyStr;
        keyStr = pair[i].first;
        Variant valueVar = pair[i].second;
        rapidjson::Value key(rapidjson::kObjectType);
        rapidjson::Value value(rapidjson::kObjectType);
        if(type[i] == 0) {
            // valueVar is a string
            std::string str = valueVar.mutable_string();
            value.SetString(str.c_str(), static_cast<rapidjson::SizeType> (str.length()), allocator);
        } else if(type[i] == 1) {
            // value is an integer
            int num = valueVar.int64_value();
            value.SetInt(num);
        } else if(type[i] == 2) {
            // valueVar has boolean type
            bool boo = valueVar.bool_value();
            value.SetBool(boo);
        } else {
            continue;
        }

        key.SetString(keyStr.c_str(), static_cast<rapidjson::SizeType> (keyStr.length()), allocator);
        document.AddMember(key, value, allocator);
    }

    rapidjson::StringBuffer  buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return string(buffer.GetString());
}
};


RealtimeDatabaseHuawei RealtimeDatabaseHuawei::getInstance() {
    static RealtimeDatabaseHuawei sharedInstance;
    return sharedInstance;
}

RealtimeDatabaseHuawei::RealtimeDatabaseHuawei() {

}

std::string RealtimeDatabaseHuawei::createArgStr(std::vector<std::string> keys, std::vector<std::string> values, std::vector<int> types) {
    std::string cArgStr = "";       // C++ Argument String
    for(int i = 0; i < keys.size(); i++) {
        cArgStr = cArgStr + keys[i] + "#" + values[i] + "#" + to_string(types[i]) + "%";
    }
    return cArgStr;
}

#pragma Save Data
void RealtimeDatabaseHuawei::saveData() 
{
    updateUserSettings();
}

#include "MSLessonManager.h"

void RealtimeDatabaseHuawei::updateTrackingWorksheetsSended(int user_id, int profile_id, const std::string& value, std::function<void()> callback){
    std::string path_process;
    std::vector<std::pair<std::string, Variant>> multiPath;
    path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    _voidCallBack = callback;
    auto key_ws_sent = StringUtils::format(key_mat_worksheet_sent_x_y, user_id, profile_id);
    std::vector<int> types;
    types.push_back(0);
    if(callback) {
        updateTrackingWorksheetsSendedCallback = callback;
    }
    multiPath.push_back({ path_process + key_ws_sent, Variant(value) });
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, multiPath, types);
}

void RealtimeDatabaseHuawei::updateTracking(mj::model::LessonInfo lesson,mj::model::ActInfo info,bool update_last_activity, std::function<void()> callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();
    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    std::vector<std::string> keys;
    std::vector<std::string> values;
    std::vector<int> types;
    if(info.state == mj::model::ActInfo::State::LEARNED)
    {
        // nếu là MS và Lesson:Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(info.lesson_id))) {
            keys.push_back(path_lesson + StringUtils::format("%d/", info.lesson_id));
            values.push_back(to_string(ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id)));
            types.push_back(1);
        } else {
            keys.push_back(path_lesson + StringUtils::format("%d/", info.lesson_id));
            values.push_back(to_string(ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson.category_id)));
            types.push_back(1);
        }
    }
    if(update_last_activity)
    {
        keys.push_back(path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id));
        values.push_back(to_string(info.activity_id));
        types.push_back(0);
    }
    keys.push_back(path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id));
    values.push_back(to_string(info.lesson_id));
    types.push_back(0);

    if(callback) {
        _voidCallBack = callback;
    }
    std::string cArgStr = createArgStr(keys, values, types);

    if(callback) {
        _voidCallBack = callback;
    }
    JniHelper::callStaticVoidMethod(className, updateMethod, cArgStr);
}

void mm::RealtimeDatabaseHuawei::updateTrackingAndLearnedLessons(mj::model::LessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId, std::function<void()> callback)
{
    auto user_id		= ap::Database::getInstance().getUserInfo()._id;
    auto profile_id		= ap::ProfileManager::getInstance().getCurrentProfileId();
    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    int value;
    if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
    {
        if (update_last_activity)
        {
            int last_act_id = ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id);

            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);

            value = info.activity_id;

            if (!activities.empty() && activities.back().activity_id == last_act_id) {

                value = 1;
            }
        }
    }

    if (FileUtils::getInstance()->isFileExist(k_filePath))
    {
        std::string learnedLessonsPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
        try
        {
            std::string query = lessonId != 0 ? StringUtils::format("select * from tbl_lesson_learned where id=%d", lessonId) : "SELECT * from tbl_lesson_learned";
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
            SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_learned");
            std::vector<std::pair<std::string, Variant>> multiPath;
            std::vector<int> types;
            // nếu là MS và Lesson:Comprehension
            if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
            {
                if (update_last_activity)
                {
                    multiPath.push_back({ path_lesson + StringUtils::format("%d/", info.lesson_id), value });
                    types.push_back(1);
                }
            }
                // code cũ
            else if (info.state == mj::model::ActInfo::State::LEARNED)
            {
                multiPath.push_back({ path_lesson + StringUtils::format("%d/", info.lesson_id), Variant(lesson.category_id) });
                types.push_back(1);
            }

            if (update_last_activity)
            {
                multiPath.push_back({ path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id) ,Variant(std::to_string(info.activity_id)) });
                types.push_back(0);
            }

            multiPath.push_back({ path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id) ,Variant(std::to_string(info.lesson_id)) });
            types.push_back(0);

            while (stmt_lesson.executeStep())
            {
                int lesson_id = stmt_lesson.getColumn(0).getInt();
                int category_id = stmt_lesson.getColumn(1).getInt();
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson)) {
                    multiPath.push_back({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
                    types.push_back(1);
                } else {
                    multiPath.push_back({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                    types.push_back(1);
                }
            }

            if(callback) {
                updateTrackingAndLearnedLessonsCallback = callback;
            }

            rapidjson::Document document;
            std::string jsonString = createJSONString(document, multiPath, types);
            JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 3);
            }
            catch (const std::exception&)
            {

            }
    }
}
void mm::RealtimeDatabaseHuawei::updateTrackingAndLearnedLessonsAISpeaking(mj::model::LessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId, std::function<void()> callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();
    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    int value;
    if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
    {
        if (update_last_activity)
        {
            int last_act_id = ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id);

            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);

            value = info.activity_id;

            if (!activities.empty() && activities.back().activity_id == last_act_id) {

                value = 1;
            }
        }
    }

    if (FileUtils::getInstance()->isFileExist(k_filePath))
    {
        std::string learnedLessonsPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);
        try
        {
            std::string query = lessonId != 0 ? StringUtils::format("select * from tbl_lesson_ai_speaking_learned where id=%d", lessonId) : "SELECT * from tbl_lesson_ai_speaking_learned";
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
            SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_ai_speaking_learned");
            std::vector<std::pair<std::string, Variant>> multiPath;
            std::vector<int> types;
            // nếu là MS và Lesson:Comprehension
            if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
            {
                if (update_last_activity)
                {
                    multiPath.push_back({ path_lesson + StringUtils::format("%d/", info.lesson_id), value });
                    types.push_back(1);
                }
            }
            // code cũ
            else if (info.state == mj::model::ActInfo::State::LEARNED)
            {
                multiPath.push_back({ path_lesson + StringUtils::format("%d/", info.lesson_id), Variant(lesson.category_id) });
                types.push_back(1);
            }

            if (update_last_activity)
            {
                multiPath.push_back({ path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id) ,Variant(std::to_string(info.activity_id)) });
                types.push_back(0);
            }

            multiPath.push_back({ path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id) ,Variant(std::to_string(info.lesson_id)) });
            types.push_back(0);

            while (stmt_lesson.executeStep())
            {
                int lesson_id = stmt_lesson.getColumn(0).getInt();
                int category_id = stmt_lesson.getColumn(1).getInt();
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson)) {
                    multiPath.push_back({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
                    types.push_back(1);
                }
                else {
                    multiPath.push_back({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                    types.push_back(1);
                }
            }

            if (callback) {
                updateTrackingAndLearnedLessonsCallback = callback;
            }

            rapidjson::Document document;
            std::string jsonString = createJSONString(document, multiPath, types);
            JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 3);
        }
        catch (const std::exception&)
        {

        }
    }
}
void RealtimeDatabaseHuawei::updateUserSettings() {
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    updateUserSettings(user_id, nullptr);
}

void RealtimeDatabaseHuawei::updateUserSettings(int user_id, std::function<void()> callback) {
    std::string settings_path = cocos2d::StringUtils::format("users/%d/user_settings/",user_id);
    std::vector<std::string> key_string = {
            "mj_license_key"
    };

    std::vector<std::string> key_bool = {
            "mj_onboarding_part3", "mj_onboarding_part2", "key_is_licence",key_first_time_created_profile, key_data_converted,key_onboarding_lessons, key_lesson_learn
    };

    std::vector<std::string> key_integer = {
            "key_count_guide_story", key_current_profile_id, "key_lang_display"
    };

    std::vector<int> types;
//
    std::vector< std::pair<std::string, Variant>> setting_values;

    if(MJDEFAULT->getBoolForKey("mj_onboarding_license")){
        types.push_back(2);
        setting_values.push_back({settings_path + "mj_onboarding_license",Variant(MJDEFAULT->getBoolForKey("mj_onboarding_license"))});
    }
//
//
    for(auto key : key_string){
        types.push_back(0);
        setting_values.push_back({settings_path + key,Variant(MJDEFAULT->getStringForKey(key,""))});
    }
//
    for(auto key : key_bool){
        types.push_back(2);
        setting_values.push_back({settings_path + key,Variant(MJDEFAULT->getBoolForKey(key))});
    }
//
    for(auto key : key_integer){
        types.push_back(1);
        if(key == "key_lang_display"){
            setting_values.push_back({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
        } else if(key == key_current_profile_id)
        {
            if(MJDEFAULT->getIntegerForKey(key)>0)
            {
                setting_values.push_back({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
                
            }
        }
        else{
            setting_values.push_back({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
        }
    }

    if(callback) {
        updateUserSettingsCallback = callback;
    }

    rapidjson::Document document;
    std::string jsonString = createJSONString(document, setting_values, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 0);
}

void RealtimeDatabaseHuawei::updateProfileInfo(ap::Profile profile, std::function<void()> callback) {
    auto user = ap::Database::getInstance().getUserInfo();
    auto path_profile = cocos2d::StringUtils::format("users/%d/profiles/%d/profile_info/",user._id, profile.profile_id);
    std::string temp = profile.avatar;
    if (!(profile.avatar.find("mjstory/avatar/") != std::string::npos)) {
        temp = "mjstory/avatar/" + profile.avatar;
    }

    std::vector<int> types;
    std::vector<std::pair<std::string, Variant>> data;
    data.push_back({path_profile + "id",Variant(profile.profile_id)});
    types.push_back(1);

    data.push_back( {path_profile + "name",Variant(profile.name)});
    types.push_back(0);

    data.push_back({path_profile + "avatar",Variant(temp)});
    types.push_back(0);

    data.push_back({path_profile + "gender",Variant(profile.gender == ap::Profile::MALE?1:2)});
    types.push_back(1);

    data.push_back({path_profile + "age",Variant(profile.age)});
    types.push_back(1);

    rapidjson::Document  document;
    std::string jsonString = createJSONString(document, data, types);
    if(callback) {
        updateProfileInfoCallback = callback;
    }

    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 13);

}

#include "HSLib.h"
void RealtimeDatabaseHuawei::updateProfileKeys(int user_id,int profile_id, std::function<void()> callback){
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    if (!FileUtils::getInstance()->isFileExist(k_filePath))
    {
        CCLOG("cannot excute updateLearnedLessons function ! Because the %s was not found !", k_filePath.c_str());
        return;
    }
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt(db, "SELECT * from tbl_profile");
    std::vector<int> types;

    std::vector<std::pair<std::string,Variant>> data;
    try {
        while (stmt.executeStep()) {
            std::string key = StringUtils::toString(stmt.getColumn(0).getText());

            key = replaceString(key, ".", "-");
            auto value = XHSLib->giaimaString( stmt.getColumn(1).getText());
            std::cout << "row (" << key << ", \"" << value << "\")\n";
            types.push_back(0);
            data.push_back({path + "/" + key, Variant(value)});
        }
    }
    catch (std::exception & e) {
        std::cout << "db exception: " << e.what() << std::endl;
    }

    if(callback) {
        updateProfileKeysCallback = callback;
    }

    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);

    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 4);
}

void RealtimeDatabaseHuawei::updateActivityLearnOfLessonPhonic(int user_id, int profile_id,int grade, int numberActivity, std::function<void()> i_callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/activityPhonic/", user_id, profile_id);
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    types.push_back(1);
    data.push_back({path + StringUtils::format("%d/", grade), Variant(numberActivity)});

    if(i_callback) {
        updateLearnedLessonsCallback = i_callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 14);
}

void RealtimeDatabaseHuawei::updateActivityLearnOfLessonInReadingComprehension(int profileID, std::string keyActivityLearn, int idActivity, std::function<void()> i_callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/activity_comprehension/", user_id, profileID);
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    data.push_back({ path + keyActivityLearn, Variant(idActivity) });
    types.push_back(1);

    if(i_callback) {
        updateActivityLearnOfLessonInReadingComprehensionCallback = i_callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 16);
}

void RealtimeDatabaseHuawei::updateProfileKeysByKeyAndValue(int profile_id, std::string key, int value, std::function<void()> i_callback)
{
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string valueSting = std::to_string(value);

    data.push_back({ path + "/" + key, Variant(valueSting) });
    types.push_back(0);

    if(i_callback) {
        updateupdateProfileKeysByKeyAndValueCallback = i_callback;
    }

    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 15);

}


std::string RealtimeDatabaseHuawei::replaceString(std::string subject, const std::string search, const std::string replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void RealtimeDatabaseHuawei::updateLearnedLessons(int user_id, int profile_id, std::function<void()> callback) {
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    if (!FileUtils::getInstance()->isFileExist(k_filePath))
    {
        CCLOG("cannot excute updateLearnedLessons function ! Because the %s was not found !", k_filePath.c_str());
        return;
    }
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_learned");
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    while (stmt_lesson.executeStep()) {
        int lesson_id = stmt_lesson.getColumn(0).getInt();
        int category_id = stmt_lesson.getColumn(1).getInt();

        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {
            types.push_back(1);
            data.push_back({path + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id)});
        } else {
            types.push_back(1);
            data.push_back({path + StringUtils::format("%d/", lesson_id), Variant(category_id)});
        }
    }
    if(callback) {
        updateLearnedLessonsCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);

    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 5);
}

void RealtimeDatabaseHuawei::updateLearnedLessonsAISpeaking(int user_id, int profile_id, std::function<void()> callback) {
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    if (!FileUtils::getInstance()->isFileExist(k_filePath))
    {
        CCLOG("cannot excute updateLearnedLessons aispeaking function ! Because the %s was not found !", k_filePath.c_str());
        return;
    }
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_ai_speaking_learned");
    std::vector<int> types;
    std::vector<std::pair<std::string, Variant>> data;
    while (stmt_lesson.executeStep()) {
        int lesson_id = stmt_lesson.getColumn(0).getInt();
        int category_id = stmt_lesson.getColumn(1).getInt();

        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {
            types.push_back(1);
            data.push_back({ path + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
        }
        else {
            types.push_back(1);
            data.push_back({ path + StringUtils::format("%d/", lesson_id), Variant(category_id) });
        }
    }
    if (callback) {
        updateLearnedLessonsCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);

    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 5);
}
void RealtimeDatabaseHuawei::updateReadStories(int user_id,int profile_id, int story_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/stories/", user_id, profile_id);
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    if (!FileUtils::getInstance()->isFileExist(k_filePath))
    {
        CCLOG("RealtimeDatabaseHuawei::updateReadStories| cannot updateReadStories: %d ! Because the %s was not found !", profile_id, k_filePath.c_str());

        return;
    }
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    std::string  query = story_id != 0 ? StringUtils::format("select * from tbl_story_read where id_story_lang=%d", story_id) : "SELECT * from tbl_story_read";
    SQLite::Statement stmt_lesson(db, query);
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    while (stmt_lesson.executeStep())
    {
        int story_id = stmt_lesson.getColumn(0).getInt();
        int time_read = stmt_lesson.getColumn(2).getInt();

        auto storyPath = path + StringUtils::format("%d/", story_id);

        CCLOG("RealtimeDatabaseHuawei::updateReadStories|storyPath: %s ",storyPath.c_str());
        types.push_back(1);

        data.push_back({storyPath, Variant(time_read)});
    }
    if(callback) {
        updateReadStoriesCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 6);
}

void RealtimeDatabaseHuawei::updateReadAudios(int user_id,int profile_id, int audio_id, std::function<void()>callback){
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/audiobooks/", user_id, profile_id);
    SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);
    std::string query = audio_id != 0 ? StringUtils::format("select * from tbl_audio_read where audio_id=%d", audio_id) : "SELECT * from tbl_audio_read";
    SQLite::Statement stmt_lesson(db, query);
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    while (stmt_lesson.executeStep()) {
        int audio_id = stmt_lesson.getColumn(0).getInt();
        int time_read = stmt_lesson.getColumn(2).getInt();
        types.push_back(1);

        data.push_back({path + StringUtils::format("%d/", audio_id), Variant(time_read)});
    }
    if(callback) {
        updateReadAudiosCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 7);
}

void RealtimeDatabaseHuawei::updateLoveStories(int user_id,int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_stories/", user_id, profile_id);
    SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_love_stories");
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    try
    {
        while (stmt_lesson.executeStep())
        {
            int audio_id = stmt_lesson.getColumn(0).getInt();
            types.push_back(1);
            data.push_back({path + StringUtils::format("%d/", audio_id), Variant(0)});
        }
    }
    catch (std::exception & e)
    {
        CCLOG("RealtimeDatabaseHuawei::updateLoveStories|error| cannot excute this function");
    }
    if(types.empty())
    {
        if(callback) {
            _voidCallBack = callback;
        }
    }
    if(callback) {
        updateLoveStoriesCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 8);
}

void RealtimeDatabaseHuawei::updateLoveAudios(int user_id,int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_audiobooks/", user_id, profile_id);
    SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_love_audiobooks");
    std::vector<int> types;
    std::vector<std::pair<std::string,Variant>> data;
    try
    {
        while (stmt_lesson.executeStep())
        {
            int audio_id = stmt_lesson.getColumn(0).getInt();
            types.push_back(1);
            data.push_back({path + StringUtils::format("%d/", audio_id), Variant(0)});
        }
    }
    catch (std::exception & e)
    {
        CCLOG("RealtimeDatabaseHuawei::updateLoveAudios|error| cannot excute this function");
    }
    if(callback) {
        updateLoveAudiosCallback = callback;
    }
    rapidjson::Document document;
    std::string jsonString = createJSONString(document, data, types);
    JniHelper::callStaticVoidMethod(className, updateMethod, jsonString, 9);
}

void RealtimeDatabaseHuawei::deleteLoveStories(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback )
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_stories/%d", user_id, profile_id,i_storyID);
    if(i_callback) {
        _voidCallBack = i_callback;
    }
    JniHelper::callStaticVoidMethod(className, removeMethod, path);
}

void RealtimeDatabaseHuawei::deleteLoveAudios(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback )
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_audiobooks/%d", user_id, profile_id,i_storyID);
    if(i_callback) {
        _voidCallBack = i_callback;
    }
    JniHelper::callStaticVoidMethod(className, removeMethod, path);
}
#endif

