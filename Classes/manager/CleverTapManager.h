//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once

#include "BaseSingleton.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "APDatabase.h"
#include "APProfile.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#define CLEVER_TAP_OBJECTID_ID						"objectId"
#define CLEVER_TAP_IS_NEW_OBJECTID_ID						"is_creat_new_object_id"
#define CLEVER_TAP_USER_PROPERTIES_ID						"id"
#define CLEVER_TAP_USER_PROPERTIES_NAME						"name"
#define CLEVER_TAP_USER_PROPERTIES_PARENT_ID				"parent_id"
#define CLEVER_TAP_USER_PROPERTIES_PARENT_NAME				"parent_name"
#define CLEVER_TAP_USER_PROPERTIES_TYPE						"type"
#define CLEVER_TAP_USER_PROPERTIES_APP_USING				"app_using"
#define CLEVER_TAP_USER_PROPERTIES_PURCHASED				"purchased"
#define CLEVER_TAP_USER_PROPERTIES_TIME_PURCHASED			"time_purchased"
#define CLEVER_TAP_USER_PROPERTIES_TIME_EXPIRED				"time_expired"
#define CLEVER_TAP_USER_PROPERTIES_RECENT_DATE				"recent_date"
#define CLEVER_TAP_USER_PROPERTIES_LOCATION					"location"
#define CLEVER_TAP_USER_PROPERTIES_CITY						"city"
#define CLEVER_TAP_USER_PROPERTIES_STATE					"state"
#define CLEVER_TAP_USER_PROPERTIES_COUNTRY					"country"
#define CLEVER_TAP_USER_PROPERTIES_LATITUDE					"latitude"
#define CLEVER_TAP_USER_PROPERTIES_LONGITUDE				"longitude"
#define CLEVER_TAP_USER_PROPERTIES_EMAIL					"email"
#define CLEVER_TAP_USER_PROPERTIES_PHONE					"phone"
#define CLEVER_TAP_USER_PROPERTIES_DEIVICE_ID				"deivice_id"
#define CLEVER_TAP_USER_PROPERTIES_DEVICE_NAME				"device_name"
#define CLEVER_TAP_USER_PROPERTIES_PLATFORM					"platform"
#define CLEVER_TAP_USER_PROPERTIES_NUMBER_OF_CHILDREN		"number_of_children"
#define CLEVER_TAP_USER_PROPERTIES_GENDER 					"gender"
#define CLEVER_TAP_USER_PROPERTIES_AGE 						"age"
#define CLEVER_TAP_USER_PROPERTIES_MJ_DISPLAY_LANGUAGE				"mj_display_language"
#define CLEVER_TAP_USER_PROPERTIES_MJ_AUTO_PLAY						"mj_auto_play"
#define CLEVER_TAP_USER_PROPERTIES_MJ_SKIP_PHONICS					"mj_skip_phonics"
#define CLEVER_TAP_USER_PROPERTIES_MJ_FLASH_INTERVAL				"mj_flash_interval"
#define CLEVER_TAP_USER_PROPERTIES_MJ_DISPLAY_POINTER				"mj_display_pointer"
#define CLEVER_TAP_USER_PROPERTIES_MJ_MAX_LESSONS_SESSION			"mj_max_lessons_session"
#define CLEVER_TAP_USER_PROPERTIES_MJ_MIN_INTERVAL_SESSION			"mj_min_interval_session"
#define CLEVER_TAP_USER_PROPERTIES_MS_DISPLAY_LANGUAGE				"ms_display_language"
#define CLEVER_TAP_USER_PROPERTIES_MM_DISPLAY_LANGUAGE				"mm_display_language"
#define CLEVER_TAP_USER_PROPERTIES_VM_DISPLAY_LANGUAGE				"vm_display_language"
#define DUMMY_PROFILE "{\"Name\":\"Mr. Robot\"}"


#if (defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)) // we should disable the clever tap if we use the debug mode.
    #define IS_CLEVER_TAP_ENABLE    1
#else
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        #define IS_CLEVER_TAP_ENABLE    1
    #else
        #define IS_CLEVER_TAP_ENABLE    1
    #endif
#endif


class CleverTapManager : public BaseSingleton<CleverTapManager>
{

public:
    
    enum class PUSH_TYPE
    {
        LOGIN,
        UPDATE
    };
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    bool isClevertapLoginProcessing();

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void autoIntegrate();
#endif
    void pushProfile(PUSH_TYPE i_type,std::function<void()> i_callback);
    void disableProfileByID(long long identity,PUSH_TYPE i_type,std::function<void()> i_callback, bool isPushLastLearnActivity = false);// identity = "userId" + "profileId"
    
	void pushProfileByID(long long profileID,PUSH_TYPE i_type,std::function<void()> i_callback, bool isPushLastLearnActivity = false, bool disableProfile = false);

	// event with properties
	void pushEvent(std::string i_name, std::vector<std::pair<std::string, cocos2d::Value>> i_properties);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::vector<std::string> getValueDateAppInbox();
#endif
	// event without properties
	void pushEvent(std::string i_name);

    std::string getCleverTapIdentify();

    void updateUserInfoForAllAccounts();
    
    void updateSequenceProfileData(std::vector<ap::Profile> profiles, int i_id);
    
    void updateParentInfoAndSignInWithCurrentProfileID();
    
    void initAppInbox();
    
    void showAppInbox();
    
    void storePendingEvent(std::string i_eventName, std::vector<std::pair<std::string, cocos2d::Value>> i_properties);
    
    void pushPendingEvents();
    
    std::string getDisplayLanguage();
    void pushInboxNotificationViewedEvent(std::string messageId, int type);

    void CleverTapApi(std::string data, std::function<void()> callback = nullptr, int timeEventPush = -1);
    void CleverTapApi(std::string data, std::function<void(int responseCode)> callback);
    void logEventByApi(std::string i_eventName, std::string propertiesJson);

    void initProductConfig();

    bool getProductConfigBoolValue(std::string key);

    void fechProductConfig();

	void activeClevertap();

    bool activeClevertap(std::string key);

    void initFeatureFlags();
    
    bool getFeatureFlagsBoolValue(std::string key,bool defaultValue);
    void loginDummyProfile();
    void mergeProfile(std::string profile);
    std::string getCTIdentity();
#if ((CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS) || CC_PLATFORM_HUAWEI) && IS_CLEVER_TAP_ENABLE
    void loginDummyProfileForWinAndMac();

    void updateProfileFromloginForWinAndMac();

    void loginClevertap();

#endif
    enum JsonPurpose {
        PUSH_BY_SDK,
        PUSH_BY_API,
        PUSH_BY_API_FOR_LOGIN_USER,
        PUSH_BY_API_UPDATE_PROFILE_FORM_LOGIN_USER,
    };
    void setUserInfoForDelete(ap::UserInfo userInfo){
        _userInfo = userInfo;
    }
    ap::UserInfo getUserInfoForDelete(){
        return _userInfo;
    }
    
    void setProfileInfoForDelete(ap::Profile profileForDelete){
        _profileForDelete = profileForDelete;
    }
    ap::Profile getProfileInfoForDelete(){
        return _profileForDelete;
    }
    bool isJsonVaild(std::string i_json);

private:

    
	std::string getProfileJson(const long long i_profileID = -1, bool isPushLastLearnActivity = false, JsonPurpose purpose = PUSH_BY_SDK, bool disableProfile = false); //= -1 mean this is the parent.

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && IS_CLEVER_TAP_ENABLE
    
    void pushIOSProfile(std::string i_json,PUSH_TYPE i_type,std::function<void()> i_callback);
    
    void pushIOSEvent(std::string i_eventNam,std::string i_eventProperties);
     
    void initAppInboxIOS();
    
    std::vector<std::string> getValueDataAppInboxIos();
    
    void showAppInboxIOS();
    std::string getCleverTabIdentityIOS();
public:
    
    void registerDelegates();
    
 #endif
    ap::UserInfo _userInfo = ap::UserInfo();
    ap::Profile _profileForDelete = ap::Profile();
    std::vector<std::pair<std::string,std::vector<std::pair<std::string, cocos2d::Value>>>> m_eventStacks;

};


