//
//  MMFirebaseDatabase.cpp
//  MonkeyMath
//
//  Created by MacOne on 10/7/19.
//

#include "MMFirebaseDatabase.h"
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
#include "MJPlatformConfig.h"
#include "MSNewFirebase.h"
#include "APPushFirestoreDataAPI.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#ifdef _MSC_VER
#undef GetObject
#endif
#include <VersionHelpers.h>
#endif

#define USER_REALTIME_VARIABLE                      "realtime_variable"
#define USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY  "accountv3_skip_counter_enter_info"
#define USER_EXIST_TIMEOUT                          10.0f
#define RETRIEVE_DATA_FIREBASE_TIMEOUT_SCHEDULE     "retrieve_data_firebase_timeout"
using namespace mm;
using namespace firebase;
USING_NS_CC;

RealtimeDatabase RealtimeDatabase::getInstance() {
    static RealtimeDatabase sharedInstance;
    return sharedInstance;
}

RealtimeDatabase::RealtimeDatabase() {
    auto app = ::firebase::App::GetInstance();
    if (!app) {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        app = ::firebase::App::Create(::firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        app = ::firebase::App::Create(::firebase::AppOptions());
    #else
        auto opt_string = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services.json");
        auto options = ::firebase::AppOptions::LoadFromJsonConfig(opt_string.c_str());
        app = ::firebase::App::Create(* options);
    #endif
    }

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		database = ::firebase::database::Database::GetInstance(app);
	}
	else
	{
		database = ::firebase::database::Database::GetInstance(app, "https://mj-stories-169807-6c045.firebaseio.com/");
	}
    
    if(database) {
        database->set_persistence_enabled(true);
    }
}

#pragma Save Data
void RealtimeDatabase::saveData() 
{
    CCLOG("REALTIME DATABASE saveData");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (!IsWindows8OrGreater()) {
        RealtimeDatabase::getInstance().updateUserSettingsTest([] { });
    }
    else {
        RealtimeDatabase::getInstance().multipathUpdates();
    }
#else
    RealtimeDatabase::getInstance().multipathUpdates();
#endif
}

void RealtimeDatabase::isUserExist(int user_id, std::function<void(bool,std::string)>callback, std::function<void(float)> progressCallback) {
    isUserExistForUsersUseWindows(user_id, callback, "");
    return;
}

void RealtimeDatabase::loadProfileData(int profileID, std::function<void()> callback) {
    loadProfileDataForUserWindow(profileID, callback);
    return;
}

void RealtimeDatabase::loadKeyNewLibApp(int profileId, std::function<void()> callback)
{
    const auto k_url = StringUtils::format("https://mj-stories-169807-6c045.firebaseio.com/users/%d/app.json", profileId);

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(k_url);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {

        if (!response->isSucceed()) {
            if (callback) {
                callback();
            }
            return;
        }

        auto buffer = *response->getResponseData();

        auto buffer_string = std::string(buffer.begin(), buffer.end());

        if (buffer_string.empty()) {
            if (callback) {
                callback();
            }
            return;
        }

        CCLOG("buffer_string:%s", buffer_string.c_str());

        rapidjson::Document doc;

        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)buffer_string.c_str());

        if (doc.HasParseError()) {
            if (callback) {
                callback();
            }
            return;
        }

        if (!doc.IsObject()) {

            if (callback) {
                callback();
            }

            return;
        }
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        
        if(JSON_SAFE_CHECK(doc, "new_api_android", Bool))
        {
            auto newApiAndroid = doc["new_api_android"].GetBool();
            MJDEFAULT->setBoolForKey("new_lib_api", newApiAndroid);
        }
        
        if(JSON_SAFE_CHECK(doc, "new_lib_download_android", Bool))
        {
            auto newLibDownloadAndroid = doc["new_lib_download_android"].GetBool();
            MJDEFAULT->setBoolForKey(key_turn_on_new_download, newLibDownloadAndroid);
        }
        
#else
        if(JSON_SAFE_CHECK(doc, "new_api_ios", Bool))
        {
            auto newApiIos = doc["new_api_ios"].GetBool();
            MJDEFAULT->setBoolForKey("new_lib_api", newApiIos);
        }
        
        if(JSON_SAFE_CHECK(doc, "new_lib_download_ios", Bool))
        {
            auto newApiIos= doc["new_lib_download_ios"].GetBool();
            MJDEFAULT->setBoolForKey(key_turn_on_new_download, newApiIos);
        }
        
#endif
        if (JSON_SAFE_CHECK(doc, "push_error_firebase", Bool)) {
            auto push_error_firebase = doc["push_error_firebase"].GetBool();
            MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, push_error_firebase);
        }

        if (JSON_SAFE_CHECK(doc, "key_push_event_AB_testing", Bool)) {
            auto push_error_firebase = doc["key_push_event_AB_testing"].GetBool();
            MJDEFAULT->setBoolForKey("key_push_event_AB_testing", push_error_firebase);
        }

        
        if (JSON_SAFE_CHECK(doc, "key_push_event_firestore", Bool)) {
            auto key = doc["key_push_event_firestore"].GetBool();
            MJDEFAULT->setBoolForKey("key_push_event_firestore", key);
        }
        

        if (JSON_SAFE_CHECK(doc, "debugkey_auto_audiobook", Bool)) {
            auto debugkey_auto_audiobook = doc["debugkey_auto_audiobook"].GetBool();
            MJDEFAULT->setBoolForKey("debugkey_auto_audiobook", debugkey_auto_audiobook);
        }

        if (JSON_SAFE_CHECK(doc, "debugkey_skip_story", Bool)) {
            auto debugkey_skip_story = doc["debugkey_skip_story"].GetBool();
            MJDEFAULT->setBoolForKey("debugkey_skip_story", debugkey_skip_story);
        }

        if (JSON_SAFE_CHECK(doc, "key_is_auto_play_stroy", Bool)) {
            auto debugkey_skip_story = doc["key_is_auto_play_stroy"].GetBool();
            MJDEFAULT->setBoolForKey("key_is_auto_play_stroy", debugkey_skip_story);
        }
        
        
        if (JSON_SAFE_CHECK(doc, "is_submit", Bool)) {
            auto is_submit = doc["is_submit"].GetBool();
            MJDEFAULT->setBoolForKey(key_is_submit, is_submit);
        }
        
        if (callback) {
            callback();
        }

    });

    cocos2d::network::HttpClient::getInstance()->send(request);

    request->release();
}

void mm::RealtimeDatabase::loadProfileDataForUserWindow(int profileID, std::function<void()> callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;

    if (user_id <= 0 || profileID <= 0) {
        if (callback) {
            callback();
        }
        return;
    }

    const auto k_url = StringUtils::format("https://mj-stories-169807-6c045.firebaseio.com/users/%d/profiles/%d.json", user_id, profileID);

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(k_url);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {

        if (!response->isSucceed()) {
            if (callback) {
                callback();
            }
            return;
        }

        auto buffer = *response->getResponseData();

        auto buffer_string = std::string(buffer.begin(), buffer.end());

        if (buffer_string.empty()) {
            if (callback) {
                callback();
            }
            return;
        }

        CCLOG("buffer_string:%s", buffer_string.c_str());

        rapidjson::Document doc;

        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)buffer_string.c_str());

        if (doc.HasParseError()) {
            if (callback) {
                callback();
            }
            return;
        }

        if (!doc.IsObject()) {

            if (callback) {
                callback();
            }

            return;
        }

        loadProfileForUsersUseWindows(doc, profileID);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            
            mm::RealtimeDatabase::getInstance().updatevalueProfileKeyPushDataStories(user_id, profileID, nullptr);
            if (MJDEFAULT->getBoolForKey("key_push_event_firestore", false)) {
                MSFIRESTORE->getDataStories(user_id, profileID, callback);
            } else {
                if(callback)
                {
                    callback();
                }
            }

        });
    });

    cocos2d::network::HttpClient::getInstance()->send(request);

    request->release();
}

#include "MSLessonManager.h"

void RealtimeDatabase::updateTrackingWorksheetsSended(int user_id, int profile_id, const std::string& value, std::function<void()> callback){
    std::map<std::string, Variant> multiPath;
    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    auto key_ws_sent = StringUtils::format(key_mat_worksheet_sent_x_y, user_id, profile_id);
    
    multiPath.insert({ path_process + key_ws_sent, Variant(value) });
    
    firebase::database::DatabaseReference dbref = database->GetReference();
    
    dbref.UpdateChildren(multiPath).OnCompletion([=](const firebase::Future<void> &r)
    {
        if (callback)
        {
            callback();
        }
    });
}

void RealtimeDatabase::updateTracking(mj::model::LessonInfo lesson,mj::model::ActInfo info,bool update_last_activity, std::function<void()> callback)
{
    firebase::database::DatabaseReference dbref = database->GetReference();
    std::map<std::string, Variant> values;
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();
    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    if(info.state == mj::model::ActInfo::State::LEARNED)
    {
        // nếu là MS và Lesson:Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(info.lesson_id))) {
            values.insert({path_lesson + StringUtils::format("%d/", info.lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id)});
        } else {
            values.insert({path_lesson + StringUtils::format("%d/", info.lesson_id), Variant(lesson.category_id)});
        }
    }
    if(update_last_activity)
    {
        values.insert({path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id) ,Variant(std::to_string(info.activity_id))});
    }
    values.insert({path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id) ,Variant(std::to_string(info.lesson_id))});
    dbref.UpdateChildren(values).OnCompletion([=](const firebase::Future<void> &r) {

		if (callback)
		{
			callback();
		}

	});
}

void mm::RealtimeDatabase::updateTrackingAndLearnedLessons(mj::model::LessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId, std::function<void()> callback)
{
	std::map<std::string, Variant> multiPath;

	auto user_id		= ap::Database::getInstance().getUserInfo()._id;
	auto profile_id		= ap::ProfileManager::getInstance().getCurrentProfileId();
	
	//update tracking

	std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
	std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    
    // nếu là MS và Lesson:Comprehension
    if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
    {
        if (update_last_activity)
        {
			int last_act_id = ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id);

			auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);

			int value = info.activity_id;

			if (!activities.empty() && activities.back().activity_id == last_act_id) {

				value = 1;
			}

            multiPath.insert({ path_lesson + StringUtils::format("%d/", info.lesson_id), value });
        }
    }
    // code cũ
    else if (info.state == mj::model::ActInfo::State::LEARNED)
    {
        multiPath.insert({ path_lesson + StringUtils::format("%d/", info.lesson_id), Variant(lesson.category_id) });
    }

	if (update_last_activity)
	{
		multiPath.insert({ path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id) ,Variant(std::to_string(info.activity_id)) });
	}

	multiPath.insert({ path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id) ,Variant(std::to_string(info.lesson_id)) });

	//update learned lessons.

	const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);

	if (FileUtils::getInstance()->isFileExist(k_filePath))
	{
		std::string learnedLessonsPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);

		try
		{
            std::string query = lessonId != 0 ? StringUtils::format("select * from tbl_lesson_learned where id=%d", lessonId) : "SELECT * from tbl_lesson_learned";
			SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
			SQLite::Statement stmt_lesson(db, query);

			while (stmt_lesson.executeStep()) 
			{
				int lesson_id = stmt_lesson.getColumn(0).getInt();
				int category_id = stmt_lesson.getColumn(1).getInt();
                
                // nếu là MS và Lesson:Comprehension
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson)) {
                    multiPath.insert({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
                } else {
                    multiPath.insert({ learnedLessonsPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                }
			}
		}
		catch (const std::exception&)
		{

		}
	}

	firebase::database::DatabaseReference dbref = database->GetReference();

	dbref.UpdateChildren(multiPath).OnCompletion([=](const firebase::Future<void> &r) 
	{

		if (callback)
		{
			callback();
		}

	});
}
void mm::RealtimeDatabase::updateTrackingAndLearnedLessonsAISpeaking(mj::model::AISpeakingLessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId, std::function<void()> callback)
{
    std::map<std::string, Variant> multiPath;

    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();

    //update tracking

    std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);

    // nếu là MS và Lesson:Comprehension
    if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson))
    {
        if (update_last_activity)
        {
            int last_act_id = ms::LessonManager::getInstance().getLastActivityIdOfLesson(info.lesson_id);

            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);

            int value = info.activity_id;

            if (!activities.empty() && activities.back().activity_id == last_act_id) {

                value = 1;
            }

            multiPath.insert({ path_lesson + StringUtils::format("%d/", info.lesson_id), value });
        }
    }
    // code cũ
    else if (info.state == mj::model::ActInfo::State::LEARNED)
    {
        multiPath.insert({ path_lesson + StringUtils::format("%d/", info.lesson_id), Variant(lesson.category_id) });
    }

    if (update_last_activity)
    {
        multiPath.insert({ path_process + StringUtils::format("key_last_activity_%d/", lesson.category_id) ,Variant(std::to_string(info.activity_id)) });
    }

    multiPath.insert({ path_process + StringUtils::format("key_last_lesson_%d/", lesson.category_id) ,Variant(std::to_string(info.lesson_id)) });

    //update learned lessons.

    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);

    if (FileUtils::getInstance()->isFileExist(k_filePath))
    {
        std::string learnedLessonsAiSpeakingPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);

        try
        {
            std::string query = lessonId != 0 ? StringUtils::format("select * from tbl_lesson_ai_speaking_learned where id=%d", lessonId) : "SELECT * from tbl_lesson_ai_speaking_learned";
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
            SQLite::Statement stmt_lesson(db, query);

            while (stmt_lesson.executeStep())
            {
                int lesson_id = stmt_lesson.getColumn(0).getInt();
                int category_id = stmt_lesson.getColumn(1).getInt();

                // nếu là MS và Lesson:Comprehension
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(lesson)) {
                    multiPath.insert({ learnedLessonsAiSpeakingPath + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
                }
                else {
                    multiPath.insert({ learnedLessonsAiSpeakingPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                }
            }
        }
        catch (const std::exception&)
        {

        }
    }

    firebase::database::DatabaseReference dbref = database->GetReference();

    dbref.UpdateChildren(multiPath).OnCompletion([=](const firebase::Future<void>& r)
        {

            if (callback)
            {
                callback();
            }

        });
}
void RealtimeDatabase::updateUserSettings() {
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    updateUserSettings(user_id, nullptr);
}

void RealtimeDatabase::updateUserSettings(int user_id, std::function<void()> callback) {
    firebase::database::DatabaseReference dbref = database->GetReference();
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
    
    std::map<std::string, Variant> setting_values;
    if(MJDEFAULT->getBoolForKey("mj_onboarding_license")){
        setting_values.insert({settings_path + "mj_onboarding_license",Variant(MJDEFAULT->getBoolForKey("mj_onboarding_license"))});
    }
    
    
    for(auto key : key_string){
         setting_values.insert({settings_path + key,Variant(MJDEFAULT->getStringForKey(key,""))});
    }
    
    for(auto key : key_bool){
         setting_values.insert({settings_path + key,Variant(MJDEFAULT->getBoolForKey(key))});
    }
    
    for(auto key : key_integer){
        if(key == "key_lang_display"){
             setting_values.insert({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
        }
        else if(key == key_current_profile_id)
        {
            if(MJDEFAULT->getIntegerForKey(key)>0)
            {
                setting_values.insert({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
            }
        }
        else{
             setting_values.insert({settings_path + key,Variant(MJDEFAULT->getIntegerForKey(key))});
        }
       
    }

    dbref.UpdateChildren(setting_values).OnCompletion([=](const firebase::Future<void> &r) {

        if (callback)
        {
            callback();
        }

    });
}

void RealtimeDatabase::updateProfileInfo(ap::Profile profile, std::function<void()> callback) {
    auto user = ap::Database::getInstance().getUserInfo();
    auto path_profile = cocos2d::StringUtils::format("users/%d/profiles/%d/profile_info/",user._id, profile.profile_id);
    std::string temp = profile.avatar;
    if (!(profile.avatar.find("mjstory/avatar/") != std::string::npos)) {
        temp = "mjstory/avatar/" + profile.avatar;
    }
    std::map<std::string, Variant> data = {
        {path_profile + "id",Variant(profile.profile_id)},
        {path_profile + "name",Variant(profile.name)},
        
        {path_profile + "avatar",Variant(temp)},
        {path_profile + "gender",Variant(profile.gender == ap::Profile::MALE?1:2)},
        {path_profile + "age",Variant(profile.age)},
        
    };
    auto dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {

		if (callback)
		{
			callback();
		}

	});
}

#include "HSLib.h"
void RealtimeDatabase::updateProfileKeys(int user_id,int profile_id, std::function<void()> callback){
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
	
	const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);

	if (!FileUtils::getInstance()->isFileExist(k_filePath))
	{
		CCLOG("cannot excute updateLearnedLessons function ! Because the %s was not found !", k_filePath.c_str());
		return;
	}
    
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt(db, "SELECT * from tbl_profile");
    
    std::map<std::string,Variant> data;
    try {
        while (stmt.executeStep()) {
            std::string key = StringUtils::toString(stmt.getColumn(0).getText());
            
            key = replaceString(key, ".", "-");
            auto value = XHSLib->giaimaString( stmt.getColumn(1).getText());
            std::cout << "row (" << key << ", \"" << value << "\")\n";
            data.insert({path + "/" + key, Variant(value)});
        }
    }
    catch (std::exception & e) {
        std::cout << "db exception: " << e.what() << std::endl;
      
    }
    
    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {

		if (callback)
		{
			callback();
		}

	});
}

void RealtimeDatabase::updateProfileKeysByKeyAndValue(int profile_id, std::string key, int value, std::function<void()> i_callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user_id, profile_id);
    std::string valueSting = std::to_string(value);
	
    std::map<std::string, Variant> data;
    data.insert({ path + "/" + key, Variant(valueSting) });

    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

        if (i_callback)
        {
            i_callback();
        }
        });
}

std::string RealtimeDatabase::replaceString(std::string subject, const std::string search,
                                                const std::string replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void RealtimeDatabase::updateLearnedLessons(int user_id, int profile_id, std::function<void()> callback) {
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user_id, profile_id);
    
	const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);

	if (!FileUtils::getInstance()->isFileExist(k_filePath))
	{
		CCLOG("cannot excute updateLearnedLessons function ! Because the %s was not found !", k_filePath.c_str());
		return;
	}
	
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_learned");

    std::map<std::string,Variant> data;
    while (stmt_lesson.executeStep()) {
        int lesson_id = stmt_lesson.getColumn(0).getInt();
        int category_id = stmt_lesson.getColumn(1).getInt();
        
        // nếu là MS và Lesson:Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {
            data.insert({path + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id)});
        } else {
            data.insert({path + StringUtils::format("%d/", lesson_id), Variant(category_id)});
        }
    }
     firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {

		if (callback)
		{
			callback();
		}

	});
}

void mm::RealtimeDatabase::updateLearnedLessonsAISpeaking(int user_id, int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user_id, profile_id);

    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);

    if (!FileUtils::getInstance()->isFileExist(k_filePath))
    {
        CCLOG("cannot excute updateLearnedLessons function ! Because the %s was not found !", k_filePath.c_str());
        return;
    }

    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);
    SQLite::Statement stmt_lesson(db, "SELECT * from tbl_lesson_ai_speaking_learned");

    std::map<std::string, Variant> data;
    while (stmt_lesson.executeStep()) {
        int lesson_id = stmt_lesson.getColumn(0).getInt();
        int category_id = stmt_lesson.getColumn(1).getInt();

        // nếu là MS và Lesson:Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {
            data.insert({ path + StringUtils::format("%d/", lesson_id), ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id) });
        }
        else {
            data.insert({ path + StringUtils::format("%d/", lesson_id), Variant(category_id) });
        }
    }
    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

        if (callback)
        {
            callback();
        }

        });
}

void RealtimeDatabase::updateReadStories(int user_id,int profile_id, int story_id, std::function<void()> callback, bool isPushFireStore)
{
    std::function<void()> callbackWhenNotConectFireStore = [=] () {
        std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/stories/", user_id, profile_id);
           
           const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
           
           if (!FileUtils::getInstance()->isFileExist(k_filePath))
           {
               CCLOG("RealtimeDatabase::updateReadStories| cannot updateReadStories: %d ! Because the %s was not found !", profile_id, k_filePath.c_str());
               
               return;
           }
           
           SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

           std::string  query = story_id != 0 ? StringUtils::format("select * from tbl_story_read where id_story_lang=%d", story_id) : "SELECT * from tbl_story_read";
           
           SQLite::Statement stmt_lesson(db, query);
           
           std::map<std::string,Variant> data;
           //ValueVector listData;
            std::string dataStr = "";
           while (stmt_lesson.executeStep())
           {
               int story_id = stmt_lesson.getColumn(0).getInt();
               int time_read = stmt_lesson.getColumn(2).getInt();
              
               auto storyPath = path + StringUtils::format("%d/", story_id);
               
               CCLOG("RealtimeDatabase::updateReadStories|storyPath: %s ",storyPath.c_str());
               if (dataStr != "") {
                   dataStr+=",";
               }
               dataStr += std::to_string(story_id) + "/" + std::to_string(time_read);
//               ValueMap mapStory;
//               mapStory["story_id"] = Value(story_id);
//               mapStory["time"] = Value(time_read);
//               listData.push_back(Value(mapStory));
               data.insert({storyPath, Variant(time_read)});
           }
//           ValueMap map;
//           map["profile_id"] = Value(profile_id);
//           map["user_id"] = Value(user_id);
//           map["data"] = listData;
//           auto buffer_string = JsonDump::getInstance()->MapToString(map);

        /*if (isPushFireStore && dataStr!="") {
               ap::api::APPushFirestoreDataAPI::call(dataStr, profile_id, user_id, [=](bool isSucces, std::string msg) {

                 });
        }*/

           firebase::database::DatabaseReference dbref = database->GetReference();
           
           dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r)
           {
               if (callback)
               {
                   callback();
               }
           });
    };
    if(isPushFireStore)
    {
        if (MJDEFAULT->getBoolForKey("key_push_event_firestore", false)) {
            MSFIRESTORE->updateStories(user_id, profile_id, story_id, callbackWhenNotConectFireStore);
            callbackWhenNotConectFireStore();
        } else {
            callbackWhenNotConectFireStore();
        }
    } else {
        callbackWhenNotConectFireStore();
    }
}
void RealtimeDatabase::pushDataFirestore(int user_id,int profile_id, int story_id)
{
#if (ENABLE_API_FIRESTORE)
    const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id);
    SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

    std::string  query = story_id != 0 ? StringUtils::format("select * from tbl_story_read where id_story_lang=%d", story_id) : "SELECT * from tbl_story_read";

    SQLite::Statement stmt_lesson(db, query);

    std::string dataStr = "";
    auto listLove = PROFILE_MANAGER.getAllLoveStories(profile_id);
    auto listAct = PROFILE_MANAGER.getAllStoriesActivity(profile_id);

    while (stmt_lesson.executeStep())
    {
        int story_id = stmt_lesson.getColumn(0).getInt();
        int time_read = stmt_lesson.getColumn(2).getInt();
        if (dataStr != "") {
            dataStr += ",";
        }
        bool isLove = std::find(listLove.begin(), listLove.end(), story_id) != listLove.end();
        bool isAct = std::find(listAct.begin(), listAct.end(), story_id) != listAct.end();
        dataStr += std::to_string(story_id) + "/" + std::to_string(time_read) + "/" + (isLove ? "true" : "false") + "/" + (isAct ? "true" : "false");
    }
    CCLOG("firestore push %s", dataStr.c_str());
    ap::api::APPushFirestoreDataAPI::call(dataStr, profile_id, user_id, [=](bool isSucces, std::string msg) {
        if (!isSucces) {
            MJDEFAULT->setBoolForKey("key_called_push_firestore_" + std::to_string(profile_id), false);
            CCLOG("firestore push fail");
        }
        });
#endif
}


void RealtimeDatabase::updateReadAudios(int user_id,int profile_id, int audio_id, std::function<void()>callback){
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/audiobooks/", user_id, profile_id);
        
    SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);

    std::string query = audio_id != 0 ? StringUtils::format("select * from tbl_audio_read where audio_id=%d", audio_id) : "SELECT * from tbl_audio_read";

    SQLite::Statement stmt_lesson(db, query);
    
    std::map<std::string,Variant> data;
    while (stmt_lesson.executeStep()) {
        int audio_id = stmt_lesson.getColumn(0).getInt();
        int time_read = stmt_lesson.getColumn(2).getInt();
       
//        updateLearnedLesson(user._id, p.profile_id, lesson_id, category_id);
        data.insert({path + StringUtils::format("%d/", audio_id), Variant(time_read)});
    }
     firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {

		if (callback)
		{
			callback();
		}

	});
}


void RealtimeDatabase::updateLoveStories(int user_id,int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_stories/", user_id, profile_id);
    
    std::map<std::string,Variant> data;
    
    try
    {
        SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);
        
        SQLite::Statement stmt_lesson(db, "SELECT * from tbl_love_stories");
        
        while (stmt_lesson.executeStep())
        {
            int audio_id = stmt_lesson.getColumn(0).getInt();
            data.insert({path + StringUtils::format("%d/", audio_id), Variant(0)});
        }
    }
    catch (std::exception & e)
    {
        CCLOG("RealtimeDatabase::updateLoveStories|error| cannot excute this function");
    }
    
    firebase::database::DatabaseReference dbref = database->GetReference();

    if(!data.empty())
    {
        dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {
            
            if (callback)
            {
                callback();
            }
            
        });
    }
    else
    {
        dbref.Child(path).RemoveValue().OnCompletion([=](const firebase::Future<void> &r){
            
            if (callback)
            {
                callback();
            }
            
        });
    }
}

void RealtimeDatabase::updateStoriesActivity(int user_id, int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/stories_activity/", user_id, profile_id);

    std::map<std::string, Variant> data;

    try
    {
        SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);

        SQLite::Statement stmt_lesson(db, "SELECT * from tbl_stories_activity");

        while (stmt_lesson.executeStep())
        {
            int audio_id = stmt_lesson.getColumn(0).getInt();
            data.insert({ path + StringUtils::format("%d/", audio_id), Variant(0) });
        }
    }
    catch (std::exception& e)
    {
        CCLOG("RealtimeDatabase::updateStoriesActivity|error| cannot excute this function");
    }

    firebase::database::DatabaseReference dbref = database->GetReference();

    if (!data.empty())
    {
        dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

            if (callback)
            {
                callback();
            }

            });
    }
    else
    {
        dbref.Child(path).RemoveValue().OnCompletion([=](const firebase::Future<void>& r) {

            if (callback)
            {
                callback();
            }

            });
    }
}

void RealtimeDatabase::updateLoveAudios(int user_id,int profile_id, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_audiobooks/", user_id, profile_id);
    
    std::map<std::string,Variant> data;
     
     try
     {
         SQLite::Database db(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile_id), SQLite::OPEN_READONLY);
         
         SQLite::Statement stmt_lesson(db, "SELECT * from tbl_love_audiobooks");
         
         while (stmt_lesson.executeStep())
         {
             int audio_id = stmt_lesson.getColumn(0).getInt();
             data.insert({path + StringUtils::format("%d/", audio_id), Variant(0)});
         }
     }
     catch (std::exception & e)
     {
         CCLOG("RealtimeDatabase::updateLoveAudios|error| cannot excute this function");
     }
     
     firebase::database::DatabaseReference dbref = database->GetReference();

     if(!data.empty())
     {
         dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {
             
             if (callback)
             {
                 callback();
             }
             
         });
     }
     else
     {
         dbref.Child(path).RemoveValue().OnCompletion([=](const firebase::Future<void> &r){
             
             if (callback)
             {
                 callback();
             }
             
         });
     }
}

void RealtimeDatabase::deleteLoveStories(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback )
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_stories/%d", user_id, profile_id,i_storyID);
     
   firebase::database::DatabaseReference dbref = database->GetReference();

    dbref.Child(path).RemoveValue().OnCompletion([=](const firebase::Future<void> &r)
    {
        
        if (i_callback)
        {
            i_callback();
        }
    });

}

void RealtimeDatabase::dropDataUserFreeFromFireBase(int user_id)
{
    std::string path = cocos2d::StringUtils::format("users/%d", user_id);

    firebase::database::DatabaseReference dbref = database->GetReference();

    dbref.Child(path).RemoveValue();

}

void RealtimeDatabase::deleteLoveAudios(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback )
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/love_audiobooks/%d", user_id, profile_id,i_storyID);
    
    firebase::database::DatabaseReference dbref = database->GetReference();
    
    dbref.Child(path).RemoveValue().OnCompletion([=](const firebase::Future<void> &r)  {
        
        if (i_callback)
        {
            i_callback();
        }
        
    });
    
}

void RealtimeDatabase::updateProfilesData() {
    try {
        auto user = ap::Database::getInstance().getUserInfo();
        auto profiles = ap::Database::getInstance().getProfiles();
		updateSequenceProfileData(user, profiles, 0);
        /*for (const auto & p : profiles) {
			mm::RealtimeDatabase::getInstance().updateProfileInfo(p, [=]() {
				mm::RealtimeDatabase::getInstance().updateReadAudios(user._id, p.profile_id, [=]() {
					mm::RealtimeDatabase::getInstance().updateReadStories(user._id, p.profile_id, [=]() {
						mm::RealtimeDatabase::getInstance().updateProfileKeys(user._id, p.profile_id, [=]() {
							mm::RealtimeDatabase::getInstance().updateLearnedLessons(user._id, p.profile_id);
						});
					});
				});
			});
        }*/
        
    }
    catch (std::exception &) {
        CCLOG("update firebase realtime fail");
    }
}
void RealtimeDatabase::updateSequenceProfileData(ap::UserInfo user, std::vector<ap::Profile> profiles, const int id) {
	if (id >= profiles.size()) {
		mm::RealtimeDatabase::getInstance().addListener(ap::Database::getInstance().getUserInfo()._id);
		return;
	}
	auto p = profiles[id];
	mm::RealtimeDatabase::getInstance().updateProfileInfo(p, [=]() {
		mm::RealtimeDatabase::getInstance().updateReadAudios(user._id, p.profile_id, 0, [=]() {
			mm::RealtimeDatabase::getInstance().updateReadStories(user._id, p.profile_id, 0, [=]() {
				mm::RealtimeDatabase::getInstance().updateProfileKeys(user._id, p.profile_id, [=]() {
					mm::RealtimeDatabase::getInstance().updateLearnedLessons(user._id, p.profile_id, [=] (){
                        mm::RealtimeDatabase::getInstance().updateLearnedLessonsAISpeaking(user._id, p.profile_id, [=]() {
                            mm::RealtimeDatabase::getInstance().updateSequenceProfileData(user, profiles, id + 1);
                            });
					});
				});
			});
		});
	});
}
#pragma Retrieve Data

void RealtimeDatabase::retrieveData(std::function<void(bool success)> callback) {
    auto user = ap::Database::getInstance().getUserInfo();
    if (user._id == 0) {
        if (callback){
            callback(false);
        }
        return;
    }
    CCLOG("REALTIME DATABASE retrieveData");
    firebase::database::DatabaseReference dbref = database->GetReference();
    std::string path = cocos2d::StringUtils::format("users/%d/",user._id);
    auto result = dbref.Child(path).GetValue();
    
    result.OnCompletion([callback](const Future<firebase::database::DataSnapshot>& result_data){
        if (result_data.status() != firebase::kFutureStatusComplete) {
          CCLOG("ERROR: GetValue() returned an invalid result.");
            if (callback){
                callback(false);
            }
          // Handle the error...
        } else if (result_data.error() != firebase::database::kErrorNone) {
          CCLOG("ERROR: GetValue() returned error %d: %s", result_data.error(),
                     result_data.error_message());
            if (callback){
                callback(false);
            }
          // Handle the error...
        } else {
            auto snapshot = result_data.result();
            mm::RealtimeDatabase::getInstance().loadUserSettings(*snapshot);
            mm::RealtimeDatabase::getInstance().loadSkipCountNumber(*snapshot);
            auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id);
            if(snapshot->HasChild("profiles"))
            {
                for(auto profile : snapshot->Child("profiles").children())
                {
                    int profile_id = stoi(profile.key_string());
                    if (profile_id == 0) break;
                    PROFILE_MANAGER.getInstance().switchProfile(profile_id);
                    if (profile.HasChild("profile_info")){
                       //mm::RealtimeDatabase::getInstance().loadProfileInfo(profile.Child("profile_info"),profile_id);
                    }
                    if (profile.HasChild("process_keys")){
                    mm::RealtimeDatabase::getInstance().loadProfileKeys(profile.Child("process_keys"),profile_id);
                    }
                    if (profile.HasChild("lessons")){
                        mm::RealtimeDatabase::getInstance().loadLessonLearned(profile,profile_id);
                    }
                    if (profile.HasChild("stories")){
                        mm::RealtimeDatabase::getInstance().loadReadStories(profile.Child("stories"),profile_id);
                    }
                    
                    if (profile.HasChild("love_stories")){
                        mm::RealtimeDatabase::getInstance().loadLoveStories(profile.Child("love_stories"),profile_id);
                    }

                    if (profile.HasChild("stories_activity")) {
                        mm::RealtimeDatabase::getInstance().loadLoveStories(profile.Child("stories_activity"), profile_id);
                    }
                    
                    if (profile.HasChild("audiobooks")){
                    mm::RealtimeDatabase::getInstance().loadReadAudios(profile.Child("audiobooks"),profile_id);
                    }
                    
                    if (profile.HasChild("love_audiobooks")){
                    mm::RealtimeDatabase::getInstance().loadLoveAudios(profile.Child("love_audiobooks"),profile_id);
                    }

                    if (profile.HasChild("activityPhonic")){
                    mm::RealtimeDatabase::getInstance().loadActivityLearnOfLessonPhonic(profile.Child("activityPhonic"),profile_id);
                    }
                    
                }
            }
            if(current_profile_id>0)
            {
                PROFILE_MANAGER.getInstance().switchProfile(current_profile_id);
            }
            if (callback){
                callback(true);
            }
        }
    });
}

void RealtimeDatabase::loadLessonLearned(const firebase::database::DataSnapshot &snapshot, int profile_id) {
	CCLOG("REALTIME DATABASE load lessons ");
    if(snapshot.key_string().compare("lessons") == 0)
    {
        CCLOG("update lesson");
        auto data = snapshot;
        for(auto lesson : data.children())
        {
            CCLOG("key (%s %lld)",lesson.key_string().c_str(),lesson.value().AsInt64().int64_value());
            auto id = std::stoi(lesson.key_string());
            auto category_id = (int)lesson.value().AsInt64().int64_value();
           // PROFILE_MANAGER.updateLearnedLesson(profile_id, id, category_id);
            
            // nếu là MS và Lesson:Comprehension: lưu last_activity_id vào key
            if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicCategory(category_id)) {
                int last_act_id = (int)lesson.value().int64_value();
                MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_activity_x_y, ap::ProfileManager::getInstance().getCurrentProfileId(), id), last_act_id);
            }
        }
    }
}

void RealtimeDatabase::loadActivityLearnOfLessonPhonic(const firebase::database::DataSnapshot& snapshot, int profile_id)
{
    MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 479));
    MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 480));
    MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 481));
    CCLOG("REALTIME DATABASE activity learn Phonic ");
    if (snapshot.key_string().compare("activityPhonic") == 0)
    {
        auto data = snapshot;
        for (auto lesson : data.children())
        {
            CCLOG("key (%s %lld)", lesson.key_string().c_str(), lesson.value().AsInt64().int64_value());
            auto id = std::stoi(lesson.key_string());
            auto category_id = (int)lesson.value().AsInt64().int64_value();
            MJDEFAULT->setStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, id), std::to_string(category_id));
        }
    }
}

void RealtimeDatabase::loadActivityLearnOfLessonInReadingComprehension(const firebase::database::DataSnapshot& snapshot, int profile_id)
{
    CCLOG("REALTIME DATABASE activity learn reading comprehension ");
    if (snapshot.key_string().compare("activity_comprehension") == 0)
    {
        auto data = snapshot;
        for (auto activityLearn : data.children())
        {
            CCLOG("key (%s %lld)", activityLearn.key_string().c_str(), activityLearn.value().AsInt64().int64_value());
            auto key = activityLearn.key_string();
            auto activityID = (int)activityLearn.value().AsInt64().int64_value();
            MJDEFAULT->setIntegerForKey(key, activityID);
        }
    }
}


void RealtimeDatabase::loadReadStories(const firebase::database::DataSnapshot &snapshot, int profile_id) {
	CCLOG("REALTIME DATABASE load stories ");
    if(snapshot.key_string().compare("stories") == 0)
    {
        CCLOG("update stories");
        auto data = snapshot;
        for(auto story : data.children())
        {
            CCLOG("key (%s %lld)",story.key_string().c_str(),story.value().AsInt64().int64_value());
            auto id = std::stoi(story.key_string());
            auto time_read = (long)story.value().int64_value();
            //PROFILE_MANAGER.updateReadStories(profile_id, id, time_read);
        }
        //PROFILE_MANAGER.saveReadStories(vector);
    }
}

void RealtimeDatabase::loadLoveStories(const firebase::database::DataSnapshot &snapshot, int profile_id) {
    
    CCLOG("RealtimeDatabase::loadLoveStories| REALTIME DATABASE load stories ");
    
    PROFILE_MANAGER.deleteLoveXXXBook(ap::ProfileManager::BOOK_TYPE::STORY,profile_id);
    
    if(snapshot.key_string().compare("love_stories") == 0)
    {
        CCLOG("update stories");
        
        auto data = snapshot;
        
        for(auto story : data.children())
        {
            CCLOG("key (%s %lld)",story.key_string().c_str(),story.value().AsInt64().int64_value());
           
            auto id = std::stoi(story.key_string());
            //auto time_read = (int)story.value().int64_value();
            PROFILE_MANAGER.updateLoveStories(profile_id, id);
        }
    }
}
void RealtimeDatabase::loadStoriesActivity(const firebase::database::DataSnapshot& snapshot, int profile_id) {

    CCLOG("RealtimeDatabase::loadStoriesActivity| REALTIME DATABASE load stories ");

    //PROFILE_MANAGER.deleteLoveXXXBook(ap::ProfileManager::BOOK_TYPE::STORY, profile_id);

    if (snapshot.key_string().compare("stories_activity") == 0)
    {
        CCLOG("update stories");

        auto data = snapshot;

        for (auto story : data.children())
        {
            CCLOG("key (%s %lld)", story.key_string().c_str(), story.value().AsInt64().int64_value());

            auto id = std::stoi(story.key_string());
            //auto time_read = (int)story.value().int64_value();
            PROFILE_MANAGER.updateStorieActivity(profile_id, id);
        }
    }
}


void RealtimeDatabase::loadReadAudios(const firebase::database::DataSnapshot &snapshot, int profile_id) {
    
    CCLOG("RealtimeDatabase::loadReadAudios");
    
    if(snapshot.key_string().compare("audiobooks") == 0)
    {
        CCLOG("RealtimeDatabase::loadReadAudios|update audiobooks");
        auto data = snapshot;
        for(auto auBook : data.children())
        {
            CCLOG("RealtimeDatabase::loadReadAudios|key (%s %lld)",auBook.key_string().c_str(),auBook.value().AsInt64().int64_value());
            auto id = std::stoi(auBook.key_string());
            auto time_read = (int)auBook.value().int64_value();
           // PROFILE_MANAGER.updateReadAudios(profile_id, id, time_read);
        }
    }
}

void RealtimeDatabase::loadLoveAudios(const firebase::database::DataSnapshot &snapshot, int profile_id)
{
    CCLOG("RealtimeDatabase::loadLoveAudios");
    
    PROFILE_MANAGER.deleteLoveXXXBook(ap::ProfileManager::BOOK_TYPE::AUDIOBOOK,profile_id);
    
    if(snapshot.key_string().compare("love_audiobooks") == 0)
    {
        auto data = snapshot;
        
        for(auto audio : data.children())
        {
            CCLOG("RealtimeDatabase::loadLoveAudios|key (%s %lld)",audio.key_string().c_str(),audio.value().AsInt64().int64_value());
            
            auto id = std::stoi(audio.key_string());
            
            PROFILE_MANAGER.updateLoveAudios(profile_id, id);
        }
    }
}

void RealtimeDatabase::loadProfileKeys(const firebase::database::DataSnapshot &snapshot, int profile_id) {
	CCLOG("REALTIME DATABASE load profile process ");
    if(snapshot.key_string().compare("process_keys") == 0)
    {
        CCLOG("update process_keys");
        auto data = snapshot;
        for(auto p : data.children())
        {
            auto key = p.key_string();
            std::string value = p.value().string_value();
            CCLOG("key (%s %s)",key.c_str(),value.c_str());
            key = replaceString(key, "-", ".");
            PROFILE_MANAGER.setStringForKey(key, value);
            //PROFILE_MANAGER.updateProfileKeys(profile_id, key, XHSLib->mahoaString(value));
        }
    }
}

//void RealtimeDatabase::loadLastLearnedSkill(const firebase::database::DataSnapshot &snapshot) {
//    CCLOG("update last_learned_skill");
//    int profile_id = std::stoi(snapshot.key_string());
//    auto data = snapshot.Child("last_learned_skill");
//    for(auto grade : data.children())
//    {
//        int grade_id = std::stoi(grade.key_string());
//        int skill_id = (int)grade.value().int64_value();
////        PROFILE_MANAGER.updateLastLearnedSkill(profile_id, grade_id, skill_id);
//    }
//}


void RealtimeDatabase::loadUserSettings(const firebase::database::DataSnapshot &snapshot) {
	CCLOG("REALTIME DATABASE load user setting ");
    std::vector<std::string> key_string = {
         "mj_license_key"
    };
    
    std::vector<std::string> key_bool = {
        "mj_onboarding_part3", "mj_onboarding_part2", "mj_onboarding_license", "key_is_licence", key_first_time_created_profile, key_data_converted, key_onboarding_lessons
    };
    
    std::vector<std::string> key_integer = {
        "key_count_guide_story", key_current_profile_id, "key_lang_display"
    };
    
    if(snapshot.HasChild("user_settings"))
    {
        for(auto setting : snapshot.Child("user_settings").children())
        {
            CCLOG("%s",setting.key());
            for (auto key : key_string){
                if (setting.key_string().compare(key) == 0){
                    MJDEFAULT->setStringForKey(setting.key(), setting.value().string_value());
                }
            }
            
            for (auto key : key_bool){
                if (setting.key_string().compare(key) == 0){
                    MJDEFAULT->setBoolForKey(setting.key(), setting.value().bool_value());
                }
            }

            for (auto key : key_integer)
            {

                if (setting.key_string().compare(key) == 0)
                {
                    if (key == key_lang_display)
                    {
                        MJDEFAULT->setIntegerForKey(setting.key(), setting.value().AsInt64().int64_value());
                    }
                    if(key == key_lang_display && MJDEFAULT->getBoolForKey("is_the_first_selection_language",false))
                    {
                       MJDEFAULT->deleteValueForKey("is_the_first_selection_language");
                        
                       continue;
                    }

					if (key == key_current_profile_id && MJDEFAULT->getBoolForKey("is_link_account_processing", false))
					{
						MJDEFAULT->deleteValueForKey("is_link_account_processing");

						continue;
					}


                    MJDEFAULT->setIntegerForKey(setting.key(), setting.value().AsInt64().int64_value());
                }
            }
        }
    }
}


#include "APProfile.h"
void RealtimeDatabase::loadProfileInfo(const firebase::database::DataSnapshot &snapshot, int profile_id) {
    if(snapshot.key_string().compare("profile_info") == 0)
    {
            CCLOG("REALTIME DATABASE load profile %d",profile_id);
        try {
            auto profile = ap::Database::getInstance().getProfileById(profile_id);
            auto data = snapshot;//.Child("profile_info");
            if(data.HasChild("name"))
                profile.name = data.Child("name").value().string_value();
            if(data.HasChild("avatar"))
                profile.avatar = data.Child("avatar").value().string_value();
            if(data.HasChild("gender"))
                profile.gender = (data.Child("gender").value().int64_value() == 1)? ap::Profile::MALE : ap::Profile::FEMALE;
            if(data.HasChild("age"))
                profile.age = (int)data.Child("age").value().int64_value();
           
            ap::Database::getInstance().updateProfile(profile);
            if(MJDEFAULT->getIntegerForKey(key_current_profile_id) == profile.profile_id)
            {
                //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.change.current.profile");
            }
        
        }
        catch(std::exception e)
        {
            CCLOG("load user info fail");
        }
    }
}

#include "APDatabase.h"
void RealtimeDatabase::addListener(int user_id) {
    CCLOG("REALTIME DATABASE add listener");
    mm::RealtimeDatabase::getInstance().removeListener(user_id);
    firebase::database::DatabaseReference dbref = database->GetReference();
    //dbref.Child("users").Child(std::to_string(user_id)).Child("profiles").SetKeepSynchronized(true);
    dbref.Child("users").Child(std::to_string(user_id)).SetKeepSynchronized(true);
    auto profiles = ap::Database::getInstance().getProfiles();
    for(auto profile : profiles)
    {
        auto profile_id = profile.profile_id;
        dbref.Child(StringUtils::format("users/%d/profiles/%d/lessons/",user_id,profile_id)).AddValueListener(new ProfilesValueListener("lessons",profile_id));
        dbref.Child(StringUtils::format("users/%d/profiles/%d/process_keys/",user_id,profile_id)).AddValueListener(new ProfilesValueListener("process_keys",profile_id));
        dbref.Child(StringUtils::format("users/%d/profiles/%d/profile_info/",user_id,profile_id)).AddValueListener(new ProfilesValueListener("profile_info",profile_id));
        dbref.Child(StringUtils::format("users/%d/profiles/%d/stories/",user_id,profile_id)).AddValueListener(new ProfilesValueListener("stories",profile_id));
        dbref.Child(StringUtils::format("users/%d/profiles/%d/audiobooks/",user_id,profile_id)).AddValueListener(new ProfilesValueListener("audiobooks",profile_id));
    }
	
	dbref.Child("users").Child(std::to_string(user_id)).Child(USER_REALTIME_VARIABLE).AddChildListener(new ProfilesChildListener());
}

void RealtimeDatabase::removeListener(int user_id) {
    firebase::database::DatabaseReference dbref = database->GetReference();
    //dbref.Child("users").Child(std::to_string(user_id)).Child("profiles").RemoveAllChildListeners();
    auto profiles = ap::Database::getInstance().getProfiles();
    for(auto profile : profiles)
    {
        auto profile_id = profile.profile_id;
        dbref.Child(StringUtils::format("users/%d/profiles/%d/lessons/",user_id,profile_id)).RemoveAllValueListeners();
        dbref.Child(StringUtils::format("users/%d/profiles/%d/process_keys/",user_id,profile_id)).RemoveAllValueListeners();
        dbref.Child(StringUtils::format("users/%d/profiles/%d/profile_info/",user_id,profile_id)).RemoveAllValueListeners();
        dbref.Child(StringUtils::format("users/%d/profiles/%d/stories/",user_id,profile_id)).RemoveAllValueListeners();
        dbref.Child(StringUtils::format("users/%d/profiles/%d/audiobooks/",user_id,profile_id)).RemoveAllValueListeners();
    }
    
	dbref.Child("users").Child(std::to_string(user_id)).Child(USER_REALTIME_VARIABLE).RemoveAllValueListeners();
}

void mm::RealtimeDatabase::retrieveCustomData(std::function<void(bool success)> callback)
{
    auto user = ap::Database::getInstance().getUserInfo();

    firebase::database::DatabaseReference dbref = database->GetReference();
    
    std::string path = cocos2d::StringUtils::format("users/%d/%s/", user._id, USER_REALTIME_VARIABLE);
    
    auto result = dbref.Child(path).GetValue();

    result.OnCompletion([callback](const Future<firebase::database::DataSnapshot>& result_data) {
        if (result_data.status() != firebase::kFutureStatusComplete)
        {
            CCLOG("ERROR: GetValue() returned an invalid result.");
            // Handle the error...
            if (callback)
            {
                callback(false);
            }
        }
        else if (result_data.error() != firebase::database::kErrorNone)
        {
            CCLOG("ERROR: GetValue() returned error %d: %s", result_data.error(), result_data.error_message());
            // Handle the error...
            if (callback)
            {
                callback(false);
            }
        }
        else
        {
            auto snapshot = result_data.result();

            mm::RealtimeDatabase::getInstance().loadSkipCountNumber(*snapshot);

            if (callback)
            {
                callback(true);
            }
        }
    });
}

void mm::RealtimeDatabase::addRemindingUpdateInfoPopupSkipVariableListener()
{
    firebase::database::DatabaseReference dbref = database->GetReference();

    auto user_id = ap::Database::getInstance().getUserInfo()._id;

    dbref.Child("users").Child(std::to_string(user_id)).Child(USER_REALTIME_VARIABLE).AddChildListener(new ProfilesChildListener());
}

void mm::RealtimeDatabase::updateRemindingUpdateInfoPopupSkipNumber(const int i_num)
{
    auto user = ap::Database::getInstance().getUserInfo();
    auto path_profile = cocos2d::StringUtils::format("users/%d/%s/", user._id, USER_REALTIME_VARIABLE);
    
    std::map<std::string, Variant> data =
    {
        { path_profile + USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY,Variant(i_num) },
    };
    
    auto dbref = database->GetReference();
    
    dbref.UpdateChildren(data);

    CCLOG("xx path_profile %s", path_profile.c_str());
}

void mm::RealtimeDatabase::loadSkipCountNumber(const firebase::database::DataSnapshot & snapshot, int profile_id)
{
    if (snapshot.HasChild(USER_REALTIME_VARIABLE) && snapshot.Child(USER_REALTIME_VARIABLE).HasChild(USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY))
    {
        const int k_remindingUpdateInfoPopupCount = snapshot.Child(USER_REALTIME_VARIABLE).Child(USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY).value().int64_value();

        GettingUserInfoManager::GetInstance()->setRemindingUpdateInfoPopupCount(k_remindingUpdateInfoPopupCount);
    }

	GettingUserInfoManager::GetInstance()->setFirebaseDataLoaded(true);

}

void mm::RealtimeDatabase::multipathUpdates(std::function<void()> callback)
{
	auto user = ap::Database::getInstance().getUserInfo();
	
	std::map<std::string, firebase::Variant> multiPath;

	//1.update user information.
	
//	const std::string k_userInfoPath = cocos2d::StringUtils::format("users/%d/user_info/", user._id);
//
//	multiPath.insert({k_userInfoPath + "name",Variant(user._name)});
//	multiPath.insert({k_userInfoPath + "email",Variant(user._mail)});
//	multiPath.insert({k_userInfoPath + "phone",Variant(user._phone)});
//	multiPath.insert({k_userInfoPath + "address",Variant(user._address)});

	//2.update user settings.
	const std::string k_settingsPath = cocos2d::StringUtils::format("users/%d/user_settings/", user._id);

	std::vector<std::string> key_string = 
	{
		"mj_license_key"
	};

	std::vector<std::string> key_bool = 
	{
		"mj_onboarding_part3",
		"mj_onboarding_part2",
		"key_is_licence",
		key_first_time_created_profile,//key_first_time_created_profile
		key_data_converted,//key_data_converted
        key_onboarding_lessons,
	};

	std::vector<std::string> key_integer = 
	{
		"key_count_guide_story",
        key_current_profile_id,
		"key_lang_display"
	};

	if (MJDEFAULT->getBoolForKey("mj_onboarding_license")) 
	{
		multiPath.insert({ k_settingsPath + "mj_onboarding_license",Variant(MJDEFAULT->getBoolForKey("mj_onboarding_license")) });
	}

	for (auto key : key_string) 
	{
		multiPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getStringForKey(key,"")) });
	}

	for (auto key : key_bool) {
		multiPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getBoolForKey(key)) });
	}

	for (auto key : key_integer) 
	{
        if(key == key_current_profile_id)
        {
            if(MJDEFAULT->getIntegerForKey(key)>0)
            {
                multiPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getIntegerForKey(key)) });
            }
        }
        else
        {
            multiPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getIntegerForKey(key)) });
            
        }
	}

	//3.update profiles
    
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)

    for(auto p:multiPath)
    {
        CCLOG("multipathUpdates key:%s",p.first.c_str());
    }
    
    #endif
    
	firebase::database::DatabaseReference dbref = database->GetReference();
	
	dbref.UpdateChildren(multiPath).OnCompletion([=](const firebase::Future<void> &r) 
	{
		if (callback)
		{
			callback();
		}
	});
}

void mm::RealtimeDatabase::updateUserSettingsTest(std::function<void()> callback)
{
    auto user = ap::Database::getInstance().getUserInfo();
    std::map<std::string, firebase::Variant> settingsPath;


    //2.update user settings.
    const std::string k_settingsPath = cocos2d::StringUtils::format("users/%d/user_settings/", user._id);

    std::vector<std::string> key_string =
    {
        "mj_license_key"
    };

    std::vector<std::string> key_bool =
    {
        "mj_onboarding_part3",
        "mj_onboarding_part2",
        "key_is_licence",
        key_first_time_created_profile,//key_first_time_created_profile
        key_data_converted,//key_data_converted
        key_onboarding_lessons
    };

    std::vector<std::string> key_integer =
    {
        "key_count_guide_story",
        key_current_profile_id,
        "key_lang_display"
    };

    if (MJDEFAULT->getBoolForKey("mj_onboarding_license"))
    {
        settingsPath.insert({ k_settingsPath + "mj_onboarding_license",Variant(MJDEFAULT->getBoolForKey("mj_onboarding_license")) });
    }

    for (auto key : key_string)
    {
        settingsPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getStringForKey(key,"")) });
    }

    for (auto key : key_bool) {
        settingsPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getBoolForKey(key)) });
    }

    for (auto key : key_integer)
    {
        if(key == key_current_profile_id)
        {
            if(MJDEFAULT->getIntegerForKey(key)>0)
            {
                settingsPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getIntegerForKey(key)) });
            }
        }
        else
        {
            settingsPath.insert({ k_settingsPath + key,Variant(MJDEFAULT->getIntegerForKey(key)) });
        }
    }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)

    for (auto p : settingsPath)
    {
        CCLOG("settingsPath key:%s", p.first.c_str());
    }

#endif

    firebase::database::DatabaseReference dbref = database->GetReference();

    dbref.UpdateChildren(settingsPath).OnCompletion([=](const firebase::Future<void>& r)
        {
            if (callback)
            {
                callback();
            }
        });
}
void mm::RealtimeDatabase::updateProfilesTest(std::function<void()> callback) {
    auto user = ap::Database::getInstance().getUserInfo();
    std::map<std::string, firebase::Variant> profilesPath;

    auto profiles = ap::Database::getInstance().getProfiles();

    for (auto profile : profiles)
    {
        auto path_profile = cocos2d::StringUtils::format("users/%d/profiles/%d/profile_info/", user._id, profile.profile_id);

        profilesPath.insert({ path_profile + "id",Variant(profile.profile_id) });
        profilesPath.insert({ path_profile + "name",Variant(profile.name) });
        //profilesPath.insert({ path_profile + "avatar",Variant(profile.avatar) });
        std::string temp = profile.avatar;
        if (!(profile.avatar.find("mjstory/avatar/") != std::string::npos)) {
            temp = "mjstory/avatar/" + profile.avatar;
        }
        profilesPath.insert({ path_profile + "avatar",Variant(temp) });
        profilesPath.insert({ path_profile + "gender",Variant(profile.gender == ap::Profile::MALE ? 1 : 2) });
        profilesPath.insert({ path_profile + "age",Variant(profile.age) });

        CCLOG("multipathUpdates profile id:%d", profile.profile_id);
        CCLOG("multipathUpdates profile name:%d", profile.name.c_str());
        CCLOG("multipathUpdates profile avatar:%d", profile.avatar.c_str());
        CCLOG("multipathUpdates profile gender:%d", profile.gender == ap::Profile::MALE ? 1 : 2);
        CCLOG("multipathUpdates profile age:%d", profile.age);

        //std::string path_process = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/",  user._id, profile.profile_id);
        //std::string path_lesson = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/",  user._id, profile.profile_id);

        const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile.profile_id);

        if (!FileUtils::getInstance()->isFileExist(k_filePath))
        {
            CCLOG("multipathUpdates | cannot update profile: %d ! Because the %s was not found !", profile.profile_id, k_filePath.c_str());
            continue;;
        }

        try
        {

            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            //update read audiobook

            std::string audiobookPath = cocos2d::StringUtils::format("users/%d/profiles/%d/audiobooks/", user._id, profile.profile_id);

            SQLite::Statement stmt_lesson(db, "SELECT * from tbl_audio_read");

            while (stmt_lesson.executeStep())
            {
                int audio_id = stmt_lesson.getColumn(0).getInt();
                int time_read = stmt_lesson.getColumn(2).getInt();

                profilesPath.insert({ audiobookPath + StringUtils::format("%d/", audio_id), Variant(time_read) });
            }

        }
        catch (const std::exception&)
        {

        }

        try
        {
            //update read stories
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            std::string readStoriesPath = cocos2d::StringUtils::format("users/%d/profiles/%d/stories/", user._id, profile.profile_id);

            SQLite::Statement stmtReadStories(db, "SELECT * from tbl_story_read");

            while (stmtReadStories.executeStep())
            {
                int story_id = stmtReadStories.getColumn(0).getInt();
                int time_read = stmtReadStories.getColumn(2).getInt();

                //profilesPath.insert({ readStoriesPath + StringUtils::format("%d/", story_id), Variant(time_read) });
            }
        }
        catch (const std::exception&)
        {

        }

        try
        {
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            //update profile key
            std::string profileKeyPath = cocos2d::StringUtils::format("users/%d/profiles/%d/process_keys/", user._id, profile.profile_id);

            SQLite::Statement stmtProfileKey(db, "SELECT * from tbl_profile");

            while (stmtProfileKey.executeStep())
            {
                std::string key = StringUtils::toString(stmtProfileKey.getColumn(0).getText());

                key = replaceString(key, ".", "-");

                auto value = XHSLib->giaimaString(stmtProfileKey.getColumn(1).getText());

                profilesPath.insert({ profileKeyPath + "/" + key, Variant(value) });
            }

            //update learned lesson
            std::string learedLessonPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons/", user._id, profile.profile_id);
            std::string learnedLessonsAiSpeakingPath = cocos2d::StringUtils::format("users/%d/profiles/%d/lessons_ai_speaking/", user._id, profile.profile_id);

            SQLite::Statement stmtLearnedLesson(db, "SELECT * from tbl_lesson_learned");
            SQLite::Statement stmtLearnedAISpeakingLesson(db, "SELECT * from tbl_lesson_ai_speaking_learned");

            while (stmtLearnedLesson.executeStep())
            {
                int lesson_id = stmtLearnedLesson.getColumn(0).getInt();
                int category_id = stmtLearnedLesson.getColumn(1).getInt();
                
                // nếu là MS và Lesson:Comprehension
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {

					auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);

					int value = ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id);

					if (!activities.empty() ) {

                        if (activities.back().activity_id == value) {
                            value = 1;
                        }

                        profilesPath.insert({ learedLessonPath + StringUtils::format("%d/", lesson_id),value });
					}

                } else {
                    profilesPath.insert({ learedLessonPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                }
            }
            while (stmtLearnedAISpeakingLesson.executeStep())
            {
                int lesson_id = stmtLearnedAISpeakingLesson.getColumn(0).getInt();
                int category_id = stmtLearnedAISpeakingLesson.getColumn(1).getInt();

                // nếu là MS và Lesson:Comprehension
                if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(ms::LessonManager::getInstance().getLessonById(lesson_id))) {

                    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);

                    int value = ms::LessonManager::getInstance().getLastActivityIdOfLesson(lesson_id);

                    if (!activities.empty()) {

                        if (activities.back().activity_id == value) {
                            value = 1;
                        }

                        profilesPath.insert({ learedLessonPath + StringUtils::format("%d/", lesson_id),value });
                    }

                }
                else {
                    profilesPath.insert({ learedLessonPath + StringUtils::format("%d/", lesson_id), Variant(category_id) });
                }
            }
        }
        catch (const std::exception&)
        {

        }
    }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)

    for (auto p : profilesPath)
    {
        CCLOG("profilesPath key:%s", p.first.c_str());
    }

#endif
    firebase::database::DatabaseReference dbref = database->GetReference();

    dbref.UpdateChildren(profilesPath).OnCompletion([=](const firebase::Future<void>& r)
        {
            if (callback)
            {
                callback();
            }
        });
}
void mm::RealtimeDatabase::updateSequelize(std::vector<std::map<std::string, firebase::Variant>>  v, int index,int length, firebase::database::Database* database) {
    if (index == length) return;
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    CCLOG("%d/////////////////////", index);
#endif
    for (auto x : v[index]) {
        CCLOG("storiesPath: %s", x.first.c_str());
    }
    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(v[index]).OnCompletion([=](const firebase::Future<void>& r)
        {
            RealtimeDatabase::updateSequelize(v,index+1,length,database);
        });
}

void RealtimeDatabase::loadProfileForUsersUseWindows(rapidjson::Value& value, int profile_id)
{

    //Load audio book
    if (value.HasMember("audiobooks") && value["audiobooks"].IsObject() && !value["audiobooks"].IsNull())
    {
        auto audiobooks = value["audiobooks"].GetObject();
        std::vector<std::pair<std::string, int>> listIdAudiobookLearn = {};
        for (auto audiobook = audiobooks.begin(); audiobook != audiobooks.end(); audiobook++)
        {
            auto audiobookId = audiobook->name.GetString();
            auto audiobookValue = audiobook->value.GetInt();
            listIdAudiobookLearn.push_back({ audiobookId , audiobookValue });;
        }
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            PROFILE_MANAGER.updateReadAudios(listIdAudiobookLearn);
        });
    }

    //Load Process key
    if (value.HasMember("process_keys") && value["process_keys"].IsObject() && !value["process_keys"].IsNull())
    {
        std::vector<std::pair<std::string, std::string>> listKeyValue = {};
        auto processKeys = value["process_keys"].GetObject();
        for (auto processKey = processKeys.begin(); processKey != processKeys.end(); processKey++)
        {
            auto processKeyId = processKey->name.GetString();
            std::string ss = processKeyId;
            ss = replaceString(ss, "-", ".");
            auto processKeyValue = processKey->value.GetString();
            listKeyValue.push_back({ ss , processKeyValue });
        }
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            PROFILE_MANAGER.setStringForKey(listKeyValue);
        });
    }

    //Load Lessison
    if (value.HasMember("lessons") && value["lessons"].IsObject() && !value["lessons"].IsNull())
    {
        auto lessons = value["lessons"].GetObject();

        std::vector<std::pair<std::string, int>> listIdLessonLearn = {};
        std::vector<std::pair<std::string, int>> listPhonicIdLessonLearn = {};

        int idProfileId = ap::ProfileManager::getInstance().getCurrentProfileId();

        for (auto lesson = lessons.begin(); lesson != lessons.end(); lesson++)
        {
            auto lessonId = lesson->name.GetString();
            auto lessonValue = lesson->value.GetInt();

            listIdLessonLearn.push_back({ lessonId, lessonValue });

            if (!ms::LessonManager::getInstance().isPhonicCategory(lessonValue)) {

                listPhonicIdLessonLearn.push_back({ StringUtils::format(key_last_activity_x_y, idProfileId, std::stoi(lessonId)), lessonValue });
             
            }
        }

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            MJDEFAULT->setIntegerForKeyForListValue(listPhonicIdLessonLearn);
            });

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            PROFILE_MANAGER.updateLearnedLesson(listIdLessonLearn);
        });
    }

    //Load Lessison
    if (value.HasMember("lessons_ai_speaking") && value["lessons_ai_speaking"].IsObject() && !value["lessons_ai_speaking"].IsNull())
    {
        auto lessons = value["lessons_ai_speaking"].GetObject();

        std::vector<std::pair<std::string, int>> listIdLessonLearn = {};

        for (auto lesson = lessons.begin(); lesson != lessons.end(); lesson++)
        {
            auto lessonId = lesson->name.GetString();
            auto lessonValue = lesson->value.GetInt();

            listIdLessonLearn.push_back({ lessonId, lessonValue });

            if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicCategory(lessonValue)) {
                MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_activity_x_y, ap::ProfileManager::getInstance().getCurrentProfileId(), std::stoi(lessonId)), lessonValue);
            }
        }
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            PROFILE_MANAGER.updateLearnedAISpeakingLesson(listIdLessonLearn);
            });
    }
    
    bool isUpdateDataStories = true;
    
    //key_update_fire_store
    if(value.HasMember("key_update_fire_store") && value["key_update_fire_store"].IsBool() && !value["key_update_fire_store"].IsNull())
    {
        isUpdateDataStories = value["key_update_fire_store"].GetBool();
    }
    
    //Load stories
    if (value.HasMember("stories") && value["stories"].IsObject() && !value["stories"].IsNull())
    {
        auto stories = value["stories"].GetObject();
        std::vector<std::pair<std::string, int>> listStoryList = {};
        for (auto storie = stories.begin(); storie != stories.end(); storie++)
        {
            auto storieId = storie->name.GetString();
            auto storieValue = storie->value.GetInt();
            listStoryList.push_back({ storieId , storieValue });
        }
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            PROFILE_MANAGER.updateReadStories(listStoryList, isUpdateDataStories);
        });
    }


    //Load love stories
    if (value.HasMember("love_stories") && value["love_stories"].IsObject() && !value["love_stories"].IsNull())
    {
        auto loveStories = value["love_stories"].GetObject();
        for (auto loveStorie = loveStories.begin(); loveStorie != loveStories.end(); loveStorie++)
        {
            auto loveStorieId = loveStorie->name.GetString();
            PROFILE_MANAGER.updateLoveStories(profile_id, std::stoi(loveStorieId));
        }
    }

    //Load stories activity
    if (value.HasMember("stories_activity") && value["stories_activity"].IsObject() && !value["stories_activity"].IsNull())
    {
        auto loveStories = value["stories_activity"].GetObject();
        for (auto loveStorie = loveStories.begin(); loveStorie != loveStories.end(); loveStorie++)
        {
            auto loveStorieId = loveStorie->name.GetString();
            PROFILE_MANAGER.updateStorieActivity(profile_id, std::stoi(loveStorieId));
        }
    }

    //Load Love audio boook
    if (value.HasMember("love_audiobooks") && value["love_audiobooks"].IsObject() && !value["love_audiobooks"].IsNull())
    {
        auto loveAudiobooks = value["love_audiobooks"].GetObject();
        for (auto loveAudiobook = loveAudiobooks.begin(); loveAudiobook != loveAudiobooks.end(); loveAudiobook++)
        {
            auto loveAudiobookeId = loveAudiobook->name.GetString();
            PROFILE_MANAGER.updateLoveAudios(profile_id, std::stoi(loveAudiobookeId));
        }
    }

    if (value.HasMember("activityPhonic") && value["activityPhonic"].IsObject() && !value["activityPhonic"].IsNull())
    {
        MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 479));
        MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 480));
        MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, 481));
        auto activityPhonics = value["activityPhonic"].GetObject();
        for (auto activityPhonic = activityPhonics.begin(); activityPhonic != activityPhonics.end(); activityPhonic++)
        {
            auto activityPhonicID = activityPhonic->name.GetString();
            auto activityPhonicValue = activityPhonic->value.GetInt();
            CCLOG("key (%s %d)", activityPhonicID, activityPhonicValue);
            MJDEFAULT->setStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, std::stoi(activityPhonicID)), std::to_string(activityPhonicValue));
        }
    }

    if (value.HasMember("activity_comprehension") && value["activity_comprehension"].IsObject() && !value["activity_comprehension"].IsNull())
    {

        std::vector<std::pair<std::string, int>> listKeyValue = {};
        CCLOG("REALTIME DATABASE activity learn reading comprehension ");
        auto activityComprehensions = value["activity_comprehension"].GetObject();
        for (auto activityComprehension = activityComprehensions.begin(); activityComprehension != activityComprehensions.end(); activityComprehension++)
        {
            auto activityComprehensionID = activityComprehension->name.GetString();
            auto activityComprehensionValue = activityComprehension->value.GetInt();
            CCLOG("key (%s %d)", activityComprehensionID, activityComprehensionValue);

            listKeyValue.push_back({ activityComprehensionID, activityComprehensionValue });
        }

        MJDEFAULT->setIntegerForKeyForListValue(listKeyValue);
    }
    
    if(JSON_SAFE_CHECK(value, "app", Object))
    {
        auto doc = value["app"].GetObject();

        if (JSON_SAFE_CHECK(doc, "debugkey_auto_audiobook", Bool)) {
            auto debugkey_auto_audiobook = doc["debugkey_auto_audiobook"].GetBool();
            MJDEFAULT->setBoolForKey("debugkey_auto_audiobook", debugkey_auto_audiobook);
        }

        if (JSON_SAFE_CHECK(doc, "debugkey_skip_story", Bool)) {
            auto debugkey_skip_story = doc["debugkey_skip_story"].GetBool();
            MJDEFAULT->setBoolForKey("debugkey_skip_story", debugkey_skip_story);
        }

        if (JSON_SAFE_CHECK(doc, "key_is_auto_play_stroy", Bool)) {
            auto debugkey_skip_story = doc["key_is_auto_play_stroy"].GetBool();
            MJDEFAULT->setBoolForKey("key_is_auto_play_stroy", debugkey_skip_story);
        }

        if (JSON_SAFE_CHECK(doc, "link_download_story_error", String)) {
            auto link_download_story_erro = doc["link_download_story_error"].GetString();
            MJDEFAULT->setStringForKey("link_download_story_error", link_download_story_erro);
        }else{
            MJDEFAULT->deleteValueForKey("link_download_story_error");
        }

        
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        
        if(JSON_SAFE_CHECK(doc, "new_api_android", Bool))
        {
            auto newApiAndroid = doc["new_api_android"].GetBool();
            MJDEFAULT->setBoolForKey("new_lib_api", newApiAndroid);
        }
        
        if(JSON_SAFE_CHECK(doc, "new_lib_download_android", Bool))
        {
            auto newLibDownloadAndroid = doc["new_lib_download_android"].GetBool();
            MJDEFAULT->setBoolForKey(key_turn_on_new_download, newLibDownloadAndroid);
        }
        
#else
        if(JSON_SAFE_CHECK(doc, "new_api_ios", Bool))
        {
            auto newApiIos = doc["new_api_ios"].GetBool();
            MJDEFAULT->setBoolForKey("new_lib_api", newApiIos);
        }
        
        if(JSON_SAFE_CHECK(doc, "new_lib_download_ios", Bool))
        {
            auto newApiIos= doc["new_lib_download_ios"].GetBool();
            MJDEFAULT->setBoolForKey(key_turn_on_new_download, newApiIos);
        }
        
        if (JSON_SAFE_CHECK(doc, "push_error_firebase", Bool)) {
            auto push_error_firebase = doc["push_error_firebase"].GetBool();
            MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, push_error_firebase);
        }
        
#endif
    }
}

void RealtimeDatabase::loadInfomationForPfofile(rapidjson::Value& value, int profile_id)
{
    std::string profile = std::to_string(profile_id);
    if (JSON_SAFE_CHECK(value, "profiles", Object))
    {
        auto valueProfiles = value["profiles"].GetObject();
        if (JSON_SAFE_CHECK(valueProfiles, profile.c_str(), Object))
        {
            auto valueInfoByID = valueProfiles[profile.c_str()].GetObject();
            if (valueInfoByID.HasMember("profile_info") && valueInfoByID["profile_info"].IsObject() && !valueInfoByID["profile_info"].IsNull())
            {
                auto profileInfo = valueInfoByID["profile_info"].GetObject();
                auto profile = ap::Database::getInstance().getProfileById(profile_id);
                profile.profile_id = profile_id;

                if (profileInfo.HasMember("name") && profileInfo["name"].IsString() && !profileInfo["name"].IsNull())
                    profile.name = profileInfo["name"].GetString();

                if (profileInfo.HasMember("avatar") && profileInfo["avatar"].IsString() && !profileInfo["avatar"].IsNull())
                    profile.avatar = profileInfo["avatar"].GetString();

                if (profileInfo.HasMember("gender") && !profileInfo["gender"].IsNull())
                    profile.gender = profileInfo["gender"].GetInt() == 1 ? ap::Profile::MALE : ap::Profile::FEMALE;

                if (profileInfo.HasMember("age") && !profileInfo["age"].IsNull())
                    profile.age = profileInfo["age"].GetInt();

                ap::Database::getInstance().updateProfile(profile);
            }
        }
    }
}

void RealtimeDatabase::loadUserSettingsForUsersUseWindows(rapidjson::Value& value) {
    
    std::vector<std::string> key_string = {
         "mj_license_key"
    };

    std::vector<std::string> key_bool = {
        "mj_onboarding_part3", "mj_onboarding_part2", "mj_onboarding_license", "key_is_licence", key_first_time_created_profile, key_data_converted, key_onboarding_lessons
    };

    std::vector<std::string> key_integer = {
        "key_count_guide_story", key_current_profile_id, "key_lang_display"
    };
    
    if (value.HasMember("user_settings") && value["user_settings"].IsObject() && !value["user_settings"].IsNull()) {

        auto settings = value["user_settings"].GetObject();
        
        for (auto settingIt = settings.begin(); settingIt != settings.end(); settingIt++) {
            
            std::string settingKey = settingIt->name.GetString();
            
            if (settingKey == "")  {
                continue;
            }

            for (auto key : key_string) {
                if (settingKey.compare(key) == 0) {
                    MJDEFAULT->setStringForKey(settingKey, settingIt->value.GetString());
                }
            }

            for (auto key : key_bool) {
                if (settingKey.compare(key) == 0) {
                    MJDEFAULT->setBoolForKey(settingKey, settingIt->value.GetBool());
                }
            }

            for (auto key : key_integer)
            {
                if (settingKey.compare(key) == 0)
                {
                    if (key == key_lang_display && MJDEFAULT->getBoolForKey("is_the_first_selection_language", false))
                    {
                        MJDEFAULT->deleteValueForKey("is_the_first_selection_language");

                        continue;
                    }

                    if (key == key_current_profile_id && MJDEFAULT->getBoolForKey("is_link_account_processing", false))
                    {
                        MJDEFAULT->deleteValueForKey("is_link_account_processing");

                        continue;
                    }

                    MJDEFAULT->setIntegerForKey(settingKey, settingIt->value.GetInt());
                }
            }
        }
    }

}

void RealtimeDatabase::loadSkipCountNumberForUsersUseWindows(rapidjson::Value& value, int profile_id)
{
    if (value.HasMember(USER_REALTIME_VARIABLE) && value[USER_REALTIME_VARIABLE].IsObject() && !value[USER_REALTIME_VARIABLE].IsNull())
    {
        auto realtimeVariable = value[USER_REALTIME_VARIABLE].GetObject();
    
        const int k_remindingUpdateInfoPopupCount = JSON_SAFE_EXTRACT(realtimeVariable, USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY,Int,-111);
            
        if (k_remindingUpdateInfoPopupCount != -1111) {
            GettingUserInfoManager::GetInstance()->setRemindingUpdateInfoPopupCount(k_remindingUpdateInfoPopupCount);
        }
    }

    GettingUserInfoManager::GetInstance()->setFirebaseDataLoaded(true);
}

void RealtimeDatabase::isUserExistForUsersUseWindows(int user_id, std::function<void(bool, std::string)> callback, std::string errorMessage)
{
    const auto k_url = StringUtils::format("https://mj-stories-169807-6c045.firebaseio.com/users/%d.json", user_id);

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(k_url);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
       
        if (!response->isSucceed()) {
            if (callback) {
                callback(false, "<error> HttpRequest failed!" + errorMessage);
            }
            return;
        }
        
        auto buffer = *response->getResponseData();
        
        auto buffer_string = std::string(buffer.begin(), buffer.end());
        
        if (buffer_string.empty()) {
            if (callback) {
                callback(false, "<error> HttpRequest empty data!" + errorMessage);
            }
            return;
        }

        rapidjson::Document doc;
        
        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)buffer_string.c_str());

        if (doc.HasParseError()) {
            if (callback) {
                callback(false, "<error> HttpRequest HasParseError!" + errorMessage);
            }
            return;
        }

        if (!doc.IsObject()) {

            if (callback) {
                callback(false, "<error> doc.IsObject false!" + errorMessage);
            }

            return;
        }

        auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id);
        auto map_responce = doc.GetObject();

        if (doc.HasMember("profiles"))
        {
            if (doc["profiles"].IsObject() && !doc["profiles"].IsNull())
            {
                auto profiles = doc["profiles"].GetObject();
                for (auto profileProperties = profiles.begin(); profileProperties != profiles.end(); profileProperties++)
                {
                    if (!profileProperties->value.IsObject() || profileProperties->value.IsNull()) {
                        continue;
                    }

                    std::string profileId = profileProperties->name.GetString();

                    if (profileId.empty()) {
                        continue;
                    }

                    auto profileValue   = profileProperties->value.GetObject();


                    int profile_id = 0;

                    try
                    {
                        profile_id = stoi(profileId);
                    }
                    catch (const std::exception& e)
                    {
                        CCLOG("%s", e.what());
                    }

                    if (profile_id == 0) continue;
                    if (doc.HasMember("profiles") && doc["profiles"].IsObject() && !doc["profiles"].IsNull())
                    {
                        loadInfomationForPfofile(doc, profile_id);
                    }
                    try {
                        int prodileId_Int = 0;
                        prodileId_Int = std::stoi(profileId);
                        if(prodileId_Int>0)
                            PROFILE_MANAGER.getInstance().switchProfile(prodileId_Int);
                    }
                    catch (...) {
                    }
                
                }
            }
            if(current_profile_id>0)
            {
                PROFILE_MANAGER.getInstance().switchProfile(current_profile_id);
            }
        }

        if (doc.HasMember("user_settings") && doc["user_settings"].IsObject() && !doc["user_settings"].IsNull()) {
            loadUserSettingsForUsersUseWindows(doc);
        }

        if (doc.HasMember("realtime_variable") && doc["realtime_variable"].IsObject() && !doc["realtime_variable"].IsNull()) {
            loadSkipCountNumberForUsersUseWindows(doc);
        }

        if (callback) {
            callback(true, "");
        }

    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void RealtimeDatabase::updateActivityLearnOfLessonPhonic(int user_id, int profile_id,int grade, int numberActivity, std::function<void()> i_callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/activityPhonic/", user_id, profile_id);

    std::map<std::string, Variant> data;

    data.insert({ path + std::to_string(grade), Variant(numberActivity) });
    // data.p;

    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

        if (i_callback)
        {
            i_callback();
        }

        });
}

void RealtimeDatabase::updateActivityLearnOfLessonInReadingComprehension(int profileID, std::string keyActivityLearn, int idActivity, std::function<void()> i_callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/activity_comprehension/", user_id, profileID);
    std::map<std::string, Variant> data;
    data.insert({ path + keyActivityLearn, Variant(idActivity) });

    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

        if (i_callback)
        {
            i_callback();
        }
    });
}


void RealtimeDatabase::updateStories(std::function<void()> callback) {
    auto user = ap::Database::getInstance().getUserInfo();
    std::map<std::string, firebase::Variant> storiesPath;
    auto profiles = ap::Database::getInstance().getProfiles();
    for (auto profile : profiles) {
        const std::string k_filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile/" + StringUtils::format("profile_%d.sqlite", profile.profile_id);
        try
        {
            //update read stories
            SQLite::Database db(k_filePath, SQLite::OPEN_READONLY);

            std::string readStoriesPath = cocos2d::StringUtils::format("users/%d/profiles/%d/stories/", user._id, profile.profile_id);

            SQLite::Statement stmtReadStories(db, "SELECT * FROM 'tbl_story_read'");

            while (stmtReadStories.executeStep())
            {
                int story_id = stmtReadStories.getColumn(0).getInt();
                int time_read = stmtReadStories.getColumn(2).getInt();

                storiesPath.insert({ readStoriesPath + StringUtils::format("%d/", story_id), Variant(time_read) });
            }

        }
        catch (const std::exception&)
        {

        }
    }
    std::vector< std::map<std::string, firebase::Variant> > v;
    std::map<std::string, firebase::Variant> temp;
    int count = 1;
    for (auto it = storiesPath.begin(); it != storiesPath.end(); ++it) {
        temp.insert(*it);
        if (count % MAXSTORY == 0||std::next(it) == storiesPath.end()) {
            v.push_back(temp);
            temp.clear();
        }
        count++;
    }
    updateSequelize(v, 0, v.size(),database);
    if (callback) {
        callback();
    }


}

void RealtimeDatabase::updateInformationEventNetworkError(int user_id, std::string link, std::string screen, std::string error, std::string errorcode, std::string statusCode, std::string remoteAdress, std::function<void()> i_callback, bool isPhusEventF2p)
{
    std::string platform = "win";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    platform = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    platform = "ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    platform = "mac";

#endif

    auto deveiceID = MJ_PLATFORM_CONFIG.getDeviceId();
    auto time = int(utils::getTimeInMilliseconds() / 1000);
    std::string path = cocos2d::StringUtils::format("users/%d/lost_connection/%s/%s/%d/", user_id, platform.c_str(), deveiceID.c_str(), time);

    if(isPhusEventF2p)
    {
        path = cocos2d::StringUtils::format("users/%d/lost_connection/%s/%s/", user_id, platform.c_str(), deveiceID.c_str());
    }
    std::map<std::string, Variant> data;
    data.insert({ path + "link", Variant(link)});
    data.insert({ path + "error", Variant(error)});
    data.insert({ path + "screen", Variant(screen)});
    data.insert({ path + "error_code", Variant(errorcode)});
    data.insert({ path + "http_status_code", Variant(statusCode)});
    data.insert({ path + "remote_address", Variant(remoteAdress)});

    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {

        if (i_callback)
        {
            i_callback();
        }
        });
}

#pragma User Listener

void UserValueListener::OnCancelled(const firebase::database::Error &error_code, const char *error_message) { 
}


void UserValueListener::OnValueChanged(const firebase::database::DataSnapshot &snapshot) {
    auto user = ap::Database::getInstance().getUserInfo();
    if(snapshot.HasChild("name")) user._name = snapshot.Child("name").value().string_value();
    if(snapshot.HasChild("phone")) user._phone = snapshot.Child("phone").value().string_value();
    if(snapshot.HasChild("email")) user._mail = snapshot.Child("email").value().string_value();
    if(snapshot.HasChild("address")) user._address = snapshot.Child("address").value().string_value();
    
    ap::Database::getInstance().saveUserInfo(StringUtils::toString(user._id), user._name, user._mail, user._phone, user._address, user._userName);
}

#pragma Profile Listener

void ProfilesChildListener::OnCancelled(const firebase::database::Error &error_code, const char *error_message) { 
    CCLOG("OR: SessionCommentsChildListener canceled: %d: %s",
    error_code, error_message);
}

void ProfilesChildListener::OnChildMoved(const firebase::database::DataSnapshot &snapshot, const char *previous_sibling) { 
    // do nothing
}

void ProfilesChildListener::OnChildRemoved(const firebase::database::DataSnapshot &snapshot) { 
    // do nothing
}

void ProfilesChildListener::OnChildChanged(const firebase::database::DataSnapshot &snapshot, const char *previous_sibling) {
   
	CCLOG("firebase database : Snap shot key: %s", snapshot.key());
    
    for(auto data : snapshot.children())
    {
         CCLOG("firebase database : Data key: %s", data.key());
    }

    if(snapshot.key() == "0") return;

    if (std::string(snapshot.key()) == USER_REMINDING_UPDATE_INFO_POPUP_COUNT_KEY)
    {
        const int k_remindingUpdateInfoPopupCount = snapshot.value().int64_value();

        GettingUserInfoManager::GetInstance()->setRemindingUpdateInfoPopupCount(k_remindingUpdateInfoPopupCount);
    }
	else
	{
		auto profile_id = stoi(snapshot.key_string());
		mm::RealtimeDatabase::getInstance().loadLessonLearned(snapshot, profile_id);
		mm::RealtimeDatabase::getInstance().loadReadAudios(snapshot, profile_id);
		mm::RealtimeDatabase::getInstance().loadReadStories(snapshot, profile_id);
		mm::RealtimeDatabase::getInstance().loadProfileKeys(snapshot, profile_id);
		mm::RealtimeDatabase::getInstance().loadProfileInfo(snapshot, profile_id);
	}

}
void ProfilesChildListener::OnChildAdded(const firebase::database::DataSnapshot &snapshot, const char *previous_sibling) { 
    OnChildChanged(snapshot,previous_sibling);
}

ProfilesValueListener::ProfilesValueListener()
{
    
}

ProfilesValueListener::ProfilesValueListener(const std::string& value_ref, int profile_id)
: _value_ref(value_ref)
, _profile_id(profile_id)
{
    
}

void ProfilesValueListener::OnValueChanged(const firebase::database::DataSnapshot &snapshot)
{
    CCLOG("Snap shot %s %d",_value_ref.c_str(),_profile_id);
    if(_value_ref.compare("lessons") == 0)
    {
        mm::RealtimeDatabase::getInstance().loadLessonLearned(snapshot,_profile_id);
    }
    else if(_value_ref.compare("process_keys") == 0)
    {
        mm::RealtimeDatabase::getInstance().loadProfileKeys(snapshot,_profile_id);
    }
    else if(_value_ref.compare("profile_info") == 0)
    {
        mm::RealtimeDatabase::getInstance().loadProfileInfo(snapshot,_profile_id);
    }
    else if(_value_ref.compare("stories") == 0)
    {
        mm::RealtimeDatabase::getInstance().loadReadStories(snapshot,_profile_id);
    }
    else if(_value_ref.compare("audiobooks") == 0)
    {
        mm::RealtimeDatabase::getInstance().loadReadAudios(snapshot,_profile_id);
    }
}

void ProfilesValueListener::OnCancelled(const firebase::database::Error &error_code, const char *error_message)
{
    //
}

void RealtimeDatabase::loadkeySubmitAndroid()
{
    const auto k_url = StringUtils::format("https://mj-stories-169807-6c045.firebaseio.com/users/5345516/submit_app.json");

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(k_url);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
       
        if (!response->isSucceed()) {
            
            return;
        }
        
        auto buffer = *response->getResponseData();
        
        auto buffer_string = std::string(buffer.begin(), buffer.end());
        
        if (buffer_string.empty()) {
            
            return;
        }

        rapidjson::Document doc;
        
        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)buffer_string.c_str());

        if (doc.HasParseError()) {
           
            return;
        }

        if (!doc.IsObject()) {

            return;
        }

       if(JSON_SAFE_CHECK(doc, "is_submit", Bool))
       {
           bool kk = doc["is_submit"].GetBool();
           MJDEFAULT->setBoolForKey(key_is_submit, kk);
       }

    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void RealtimeDatabase::updatevalueProfileKeyPushDataStories(int userID, int profileId, std::function<void()> callback)
{
    std::string path = cocos2d::StringUtils::format("users/%d/profiles/%d/key_update_fire_store", userID, profileId);
        
    std::map<std::string,Variant> data;
    data.insert({path , Variant(false)});
    
    auto dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void> &r) {
        if (callback)
        {
            callback();
        }

    });
}

void RealtimeDatabase::updatevalueUserLandingPage(std::string value, std::function<void()> callback)
{
    auto user_id = ap::Database::getInstance().getUserInfo()._id;

    std::string path = cocos2d::StringUtils::format("users/%d/key_is_landing/", user_id);
    std::map<std::string, Variant> data;
    data.insert({ path + "value", Variant(value)});
    firebase::database::DatabaseReference dbref = database->GetReference();
    dbref.UpdateChildren(data).OnCompletion([=](const firebase::Future<void>& r) {
        if (callback)
        {
            callback();
        }
    });
}

void RealtimeDatabase::loadValueUserLandingpage(int userId, std::function<void()> callbackFunction)
{
    if(userId == 0)
    {
        if(callbackFunction) callbackFunction();
        return;
    }

    const auto k_url = StringUtils::format("https://mj-stories-169807-6c045.firebaseio.com/users/%d/key_is_landing.json", userId);

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(k_url);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {

        if (!response->isSucceed()) {
            if(callbackFunction) callbackFunction();
            return;
        }

        auto buffer = *response->getResponseData();

        auto buffer_string = std::string(buffer.begin(), buffer.end());

        if (buffer_string.empty()) {
            if(callbackFunction) callbackFunction();
            return;
        }

        rapidjson::Document doc;

        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)buffer_string.c_str());

        if (doc.HasParseError()) {
            if(callbackFunction) callbackFunction();
            return;
        }

        if (!doc.IsObject()) {
            if(callbackFunction) callbackFunction();
            return;
        }

        if(JSON_SAFE_CHECK(doc, "value", String))
        {
            auto value_landingpage = doc["value"].GetString();
            MJDEFAULT->setStringForKey("key_is_landingPage", value_landingpage);
        }
        if(callbackFunction) callbackFunction();
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

