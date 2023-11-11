#include "CleverTapManager.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "MJDefault.h"
#include "rating/RatingPopup.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "MJHelper.h"
#include "Key_Default_Model.h"
#include "APDatabase.h"
#include "APProfile.h"
#include "StoryConfigManager.h"
#include "network/HttpClient.h"
#include "APProfileManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif
#include "GlobalFunc.h"
#include <free2paid/F2PManager.h>


#define CLEVERTAP_END_POINT "https://sg1.api.clevertap.com/1/upload"
#define CLEVERTAP_ACCOUTN_ID     "W46-6KK-685Z"
#define CLEVERTAP_PASSCODE  "IAC-SWB-OIKL"


USING_NS_CC;
#define PROFILE_TYPE_PARENT "parent"
#define PROFILE_TYPE_CHILD	"child"
#define EN_LANGUAGE_STRING	"english"
#define VI_LANGUAGE_STRING	"vietnamese"
#define TH_LANGUAGE_STRING  "thailand"
#define ID_LANGUAGE_STRING  "indonesia"

#define UPLOAD_PROFILE_BODY "{\"d\":[{\"identity\":\"%s\","\
							"\"objectId\":\"%s\","\
							"\"type\":\"profile\","\
							"\"profileData\":{\"Email\":\"%s\","\
							"\"Phone\":\"%s\","\
							"\"Name\":\"%s\","\
							"\"parent_id\":\"%s\","\
							"\"parent_name\":\"%s\","\
							"\"type\":\"%s\","\
							"\"recent_date\":\"%lld\","\
							"\"region\":\"%s\","\
							"\"city\":\"%s\","\
							"\"country\":\"%s\","\
							"\"latitude\":\"%f\","\
							"\"longitude\":\"%f\","\
							"\"deivice_id\":\"%s\","\
							"\"device_name\":\"%s\","\
							"\"total_profile\":\"%d\","\
							"\"profile_number\":\"%d\","\
							"\"last_learn_activity\":\"%s\","\
							"\"platform\":\"%s\","\
							"\"number_of_children\":\"%d\","\
							"\"Gender\":\"%s\","\
							"\"profile_age\":\"%d\","\
							"\"purchased\":\"%d\","\
							"\"lang_display_id\":\"%d\","\
                            "\"ms_display_language\":\"%s\","\
							"\"disabled\":\"%s\"}}]}"

#define UPLOAD_USER_LOGIN "{\"d\":[{"\
							"\"objectId\":\"%s\","\
							"\"type\":\"profile\","\
							"\"profileData\":{\"Name\":\"%s\","\
							"\"disabled\":\"%s\"}}]}"

#define UPLOAD_PROFILE_FROM_USER_LOGIN "{\"d\":[{"\
							"\"objectId\":\"%s\","\
							"\"type\":\"profile\","\
							"\"profileData\":{\"Email\":\"%s\","\
							"\"Phone\":\"%s\","\
							"\"Identity\":\"%s\","\
							"\"Name\":\"%s\","\
							"\"parent_id\":\"%s\","\
							"\"parent_name\":\"%s\","\
							"\"type\":\"%s\","\
							"\"recent_date\":\"%lld\","\
							"\"region\":\"%s\","\
							"\"city\":\"%s\","\
							"\"country\":\"%s\","\
							"\"latitude\":\"%f\","\
							"\"longitude\":\"%f\","\
							"\"deivice_id\":\"%s\","\
							"\"device_name\":\"%s\","\
							"\"total_profile\":\"%d\","\
							"\"profile_number\":\"%d\","\
							"\"last_learn_activity\":\"%s\","\
							"\"platform\":\"%s\","\
							"\"number_of_children\":\"%d\","\
							"\"Gender\":\"%s\","\
							"\"profile_age\":\"%d\","\
							"\"purchased\":\"%d\","\
							"\"lang_display_id\":\"%d\","\
                            "\"ms_display_language\":\"%s\","\
							"\"disabled\":\"%s\"}}]}"

#define PROFILE_JSON_STR "{\"Identity\":\"%s\","\
						"\"Email\":\"%s\","\
						"\"Phone\":\"%s\","\
						"\"Name\":\"%s\","\
						"\"parent_id\":\"%s\","\
						"\"parent_name\":\"%s\","\
						"\"type\":\"%s\","\
						"\"recent_date\":%lld,"\
						"\"region\":\"%s\","\
						"\"city\":\"%s\","\
						"\"country\":\"%s\","\
						"\"latitude\":%f,"\
						"\"longitude\":%f,"\
						"\"deivice_id\":%s,"\
						"\"device_name\":\"%s\","\
						"\"total_profile\":\"%s\","\
						"\"profile_number\":\"%s\","\
						"\"last_learn_activity\":\"%s\","\
						"\"platform\":\"%s\","\
						"\"number_of_children\":%d,"\
						"\"Gender\":\"%s\","\
						"\"profile_age\":%d,"\
						"\"purchased\":%d,"\
                        "\"lang_display_id\":%d,"\
                        "\"ms_display_language\":\"%s\","\
						"\"disabled\":\"%s\"}"\


#define PROFILE_JSON_STR_ACTIVITY "{\"last_learn_activity\":\"%s\","\
                                    "\"recent_date\":\"%lld\"}"\

#define PROFILE_JSON_STR_ACTIVITY_WIN_MAC "{\"d\":[{\"identity\":\"%s\","\
								"\"type\":\"profile\","\
								"\"profileData\":{\"last_learn_activity\":\"%s\","\
								"\"recent_date\":\"%lld\"}}]}"

#define STRING_DEFAULT_VALUE		"NONE"
#define NUMBER_DEFAULT_VALUE		-1

std::string replaceFirstOccurrence(
    std::string& s,
    const std::string& toReplace,
    const std::string& replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return s;
    return s.replace(pos, toReplace.length(), replaceWith);
}

std::function<void()> g_androidCleverTapCallback = nullptr;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

bool CleverTapManager::isClevertapLoginProcessing() {
	return false;//tmp for platform dont have isloginprocessing
}

#endif

void CleverTapManager::pushProfile(PUSH_TYPE i_type,std::function<void()> i_callback)
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
    auto profileList = ap::Database::getInstance().getProfiles();
    if(profileList.size()<=0)
        return;
    
	auto parentProfileJsonString = getProfileJson();

	CCLOG("pushProfile parentProfileJsonString %s", parentProfileJsonString.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI) && IS_CLEVER_TAP_ENABLE

    g_androidCleverTapCallback = nullptr;

    if(i_callback)
    {
        g_androidCleverTapCallback = i_callback;
    }

    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sPushProfile", parentProfileJsonString,i_type==PUSH_TYPE::UPDATE);

#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS)) && IS_CLEVER_TAP_ENABLE
	
    pushIOSProfile(parentProfileJsonString,i_type,i_callback);
    
#else
	// call http request ?
#endif
}

void CleverTapManager::disableProfileByID(long long profileID,PUSH_TYPE i_type,std::function<void()> i_callback, bool isPushLastLearnActivity)
{
    try
    {
        // string -> integer
        pushProfileByID(profileID, i_type, i_callback, isPushLastLearnActivity, true);
    }
    catch (const std::exception&)
    {

    }
    
}
void CleverTapManager::pushProfileByID(long long profileID,PUSH_TYPE i_type,std::function<void()> i_callback, bool isPushLastLearnActivity, bool disableProfile)
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
	GlobalFunc::getInstance()->setIsClevertapProfileInitialized(true);
    auto profileList = ap::Database::getInstance().getProfiles();
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    if(profileList.size()<=0)
        return;
//#endif
	std::string childProfileJsonString;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI) && IS_CLEVER_TAP_ENABLE
	childProfileJsonString = getProfileJson(profileID, isPushLastLearnActivity,JsonPurpose::PUSH_BY_SDK, disableProfile);
    if(i_callback)
    {
        g_androidCleverTapCallback = i_callback;
    }

	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sPushProfile", childProfileJsonString,i_type==PUSH_TYPE::UPDATE);

#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS)) && IS_CLEVER_TAP_ENABLE
    childProfileJsonString = getProfileJson(profileID, isPushLastLearnActivity, JsonPurpose::PUSH_BY_SDK, disableProfile);
	pushIOSProfile(childProfileJsonString,i_type,i_callback);

#else

	childProfileJsonString = getProfileJson(profileID, isPushLastLearnActivity, CleverTapManager::JsonPurpose::PUSH_BY_API, disableProfile);
	CleverTapApi(childProfileJsonString, i_callback);

	// call http request ?
#endif

	CCLOG("pushProfile childProfileJsonString %s", childProfileJsonString.c_str());

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
std::vector<std::string>CleverTapManager::getValueDateAppInbox()
{
    return getValueDataAppInboxIos();
}
#endif
void CleverTapManager::pushEvent(std::string i_eventName, std::vector<std::pair<std::string, cocos2d::Value>> i_properties)
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || CC_PLATFORM_HUAWEI
	// xu ly store event cho mac và win khi chua có userId và profileId, do win, mac khong có sdk để  dumy user nhu android và ios
	auto userInfo = ap::Database::getInstance().getUserInfo();
	auto profiles = ap::Database::getInstance().getProfiles();
	if ((userInfo._id <=0 || profiles.size()<0) && !F2P_MANAGER->isSkipUser())// user chua dang nhap hoac chua call login skip
	{
		storePendingEvent(i_eventName, i_properties);
	}

#endif
	if (i_properties.empty()) 
	{
		pushEvent(i_eventName);

		return;
	}
	
	for (auto ev : i_properties)
	{
		auto type = ev.second.getType();

		switch (type)
		{
		case cocos2d::Value::Type::STRING:
			CCLOG("event| %s : %s", ev.first.c_str(), ev.second.asString().c_str());
			break;

		case cocos2d::Value::Type::INTEGER:
			CCLOG("event| %s : %d", ev.first.c_str(), ev.second.asInt());
			break;

		case cocos2d::Value::Type::DOUBLE:
			CCLOG("event| %s : %f", ev.first.c_str(), ev.second.asFloat());
			break;

		case cocos2d::Value::Type::FLOAT:
			CCLOG("event| %s : %f", ev.first.c_str(), ev.second.asFloat());
			break;

		case cocos2d::Value::Type::UNSIGNED:
			CCLOG("event| %s : %d", ev.first.c_str(), ev.second.asInt());
			break;
		case cocos2d::Value::Type::BOOLEAN:
			CCLOG("event| %s : %s", ev.first.c_str(), ev.second.asBool()?"true":"false");
			break;
		default:
//			assert(false && "type is not supported by analytics");
			break;
		}
	}

	std::string propertiesJson = "{";

	std::string value;              // Biến này để lưu các property có kiểu dữ liệu string.
	for (auto &p : i_properties) 
	{
		auto type = p.second.getType();

		switch (type) 
		{
		case cocos2d::Value::Type::STRING:
		    /*
		     * Với các string có chứa dấu ("), khi thêm vào file json sẽ làm hỏng cấu trúc của file json, làm crash app
		     * Ta sẽ bỏ các dấu (") ra khỏi các property là string.
		     */
		    value = p.second.asString();
		    value.erase(remove(value.begin(), value.end(), '"'), value.end());
			propertiesJson += StringUtils::format("\"%s\":\"%s\",", p.first.c_str(), value.c_str());
			break;
		case cocos2d::Value::Type::BOOLEAN:
			propertiesJson += StringUtils::format("\"%s\":%s,", p.first.c_str(), p.second.asBool() ? "true":"false");
			break;
		case cocos2d::Value::Type::INTEGER:
			propertiesJson += StringUtils::format("\"%s\":%d,", p.first.c_str(), p.second.asInt());
			break;
		case cocos2d::Value::Type::DOUBLE:
			propertiesJson += StringUtils::format("\"%s\":%.02f,", p.first.c_str(), p.second.asFloat());
			break;
		case cocos2d::Value::Type::FLOAT:
			propertiesJson += StringUtils::format("\"%s\":%.02f,", p.first.c_str(), p.second.asFloat());
			break;
		default:
		    // Tương tự như trường hợp property là string.
            value = p.second.asString();
            value.erase(remove(value.begin(), value.end(), '"'), value.end());
            propertiesJson += StringUtils::format("\"%s\":\"%s\",", p.first.c_str(), value.c_str());
		}
	}
	
	propertiesJson.pop_back(); //remove the comma at the end of propertiesJson ","

	propertiesJson += "}";

	CCLOG("pushEvent|Event Name: %s Event Properties: %s", i_eventName.c_str(), propertiesJson.c_str());

	if (!isJsonVaild(propertiesJson))
	{
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
		assert(false && "pushEvent|build json error. cannot push events");
#endif
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sPushEvent", i_eventName, propertiesJson); // true mean properties enable.
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && IS_CLEVER_TAP_ENABLE
	pushIOSEvent(i_eventName, propertiesJson);// true mean properties enable.
#else 
	logEventByApi(i_eventName, propertiesJson);

#endif
}

void CleverTapManager::pushEvent(std::string i_eventName)
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
	if (i_eventName.empty())
	{
		CCLOG("pushEvent| cannot push event : %s", i_eventName.c_str());
	}

	CCLOG("pushEvent|Event name: %s", i_eventName.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sPushEvent", i_eventName, "");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && IS_CLEVER_TAP_ENABLE
	pushIOSEvent(i_eventName, "");
#else
	logEventByApi(i_eventName, "{}");
#endif
}
std::string CleverTapManager::getCleverTapIdentify()
{
	std::string cleverTabId = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
	cleverTabId = cocos2d::JniHelper::callStaticStringMethod("org.cocos2dx.cpp.AppActivity", "sGetCleverTabID");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && IS_CLEVER_TAP_ENABLE
    cleverTabId = getCleverTabIdentityIOS();
#else
    cleverTabId = std::to_string(ap::Database::getInstance().getUserInfo()._id) + std::to_string(PROFILE_MANAGER.getInstance().getCurrentProfileId());
#endif
	return cleverTabId;
}

void CleverTapManager::updateUserInfoForAllAccounts()
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
    
#if SOLUTION_02_FOR_ANDROID

	std::vector<std::string> profileJsonDataList;

	//1. get parent's profile json.
	profileJsonDataList.push_back(getProfileJson());


	//2. get profile json of children.
	int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

	auto profiles = ap::Database::getInstance().getProfiles();

	if(!profiles.empty())
	{
		//remove current id.
		profiles.erase(std::remove_if(profiles.begin(), profiles.end(), [k_currentProfileID](ap::Profile profile){
			return profile.profile_id == k_currentProfileID;
		}),profiles.end());
	}

	profiles.push_back(ap::Database::getInstance().getProfileById(k_currentProfileID));

	for (auto p : profiles)
	{
		profileJsonDataList.push_back(getProfileJson(p.profile_id));
	}

	//end 2.

	//3. update sequen profile data on android native codes.

	const std::string className = "org.cocos2dx.cpp.AppActivity";
	const std::string methodName = "sUpdateSequenceProfileData";

	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), "([Ljava/lang/String;)V"))
	{
		jclass classId = t.env->FindClass("java/lang/String");

		jint size = profileJsonDataList.size();

		jobjectArray objectArray = t.env->NewObjectArray(size, classId, 0);

		for (int i = 0; i < size; i++)
		{
			jstring jstr = t.env->NewStringUTF(profileJsonDataList[i].c_str());
			t.env->SetObjectArrayElement(objectArray, i, jstr);
		}

		t.env->DeleteLocalRef(classId);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, objectArray);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(objectArray);
	}

	return;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)||(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)

    //parent.
     pushProfile(PUSH_TYPE::LOGIN,[=](){
        
      int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

      auto profileList = ap::Database::getInstance().getProfiles();
             
     if(!profileList.empty())
     {
         //remove current id.
         profileList.erase(std::remove_if(profileList.begin(), profileList.end(), [k_currentProfileID](ap::Profile profile){
             return profile.profile_id == k_currentProfileID;
         }),profileList.end());
     }
         
      profileList.push_back(ap::Database::getInstance().getProfileById(k_currentProfileID));
          
      updateSequenceProfileData(profileList,0);
        
    });
#endif
        
}

void CleverTapManager::updateSequenceProfileData(std::vector<ap::Profile> profiles, int i_id)
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
    
    if(i_id >= profiles.size())
    {
        return;
    }

	auto profile = profiles.at(i_id);

    pushProfileByID(profile.profile_id,CleverTapManager::PUSH_TYPE::LOGIN,[=](){
        
        updateSequenceProfileData(profiles,i_id+1);
        
    });
}

void CleverTapManager::updateParentInfoAndSignInWithCurrentProfileID()
{
#if !(IS_CLEVER_TAP_ENABLE)
    return;
#endif
    
#if SOLUTION_02_FOR_ANDROID

	std::vector<std::string> profileJsonDataList;

	int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

	profileJsonDataList.push_back(getProfileJson(k_currentProfileID));
	profileJsonDataList.push_back(getProfileJson());
    profileJsonDataList.push_back(getProfileJson(k_currentProfileID));

	//update sequen profile data on android native codes.

	const std::string className = "org.cocos2dx.cpp.AppActivity";
	const std::string methodName = "sUpdateSequenceProfileData";

	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), "([Ljava/lang/String;)V"))
	{
		jclass classId = t.env->FindClass("java/lang/String");

		jint size = profileJsonDataList.size();

		jobjectArray objectArray = t.env->NewObjectArray(size, classId, 0);

		for (int i = 0; i < size; i++)
		{
			jstring jstr = t.env->NewStringUTF(profileJsonDataList[i].c_str());
			t.env->SetObjectArrayElement(objectArray, i, jstr);
		}

		t.env->DeleteLocalRef(classId);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, objectArray);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(objectArray);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
    const int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

    CleverTapManager::GetInstance()->pushProfileByID(k_currentProfileID,CleverTapManager::PUSH_TYPE::LOGIN,[=](){
        
        CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN,[=](){
            
            const int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

            CleverTapManager::GetInstance()->pushProfileByID(k_currentProfileID,CleverTapManager::PUSH_TYPE::LOGIN,nullptr);
            
        });
        
    });
#endif
}

void CleverTapManager::initAppInbox()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "sInitAppInbox");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //initAppInboxIOS();
#endif
}

void CleverTapManager::showAppInbox()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //showAppInboxIOS();
#endif
}

void CleverTapManager::storePendingEvent(std::string i_eventName, std::vector<std::pair<std::string, cocos2d::Value>> i_properties)
{
    m_eventStacks.push_back({i_eventName,i_properties});
}

void CleverTapManager::pushPendingEvents()
{
    for(auto ev : m_eventStacks)
    {
        CleverTapManager::GetInstance()->pushEvent(ev.first, ev.second);
    }
    
    m_eventStacks.clear();
}

std::string CleverTapManager::getDisplayLanguage()
{
    if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) {
        return TH_LANGUAGE_STRING;
    } else if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE) {
        return VI_LANGUAGE_STRING;
    } else if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
        return ID_LANGUAGE_STRING;
    } else {
        return EN_LANGUAGE_STRING;
    }
}

bool CleverTapManager::isJsonVaild(std::string i_json)
{
	rapidjson::Document doc;

	doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(i_json.c_str());

	if (doc.HasParseError())
	{
		return false;
	}

	return true;
}

std::string CleverTapManager::getProfileJson(const long long i_profileID, bool isPushLastLearnActivity , JsonPurpose purpose, bool disableProfile) //= -1 mean this is the parent.
{
	auto profileList = ap::Database::getInstance().getProfiles();

	auto userInfo = ap::Database::getInstance().getUserInfo();
    if(disableProfile)
    {
        userInfo = getUserInfoForDelete();
    }
	if (!userInfo._phone.empty() && userInfo._phone.front() == '0' && MJDEFAULT->getStringForKey(key_country_code, "NONE") == "VN")
	{
		userInfo._phone.erase(userInfo._phone.begin());
		userInfo._phone = "+84" + userInfo._phone;
	}

	std::string			identity		= std::to_string(userInfo._id);
	std::string			objectId		= MJDEFAULT->getStringForKey(CLEVER_TAP_OBJECTID_ID, "");
	std::string			email			= userInfo._mail;
	std::string			phone			= userInfo._phone;
	std::string			name			= userInfo._name;
    std::string            parentID        = "";
    std::string            parentName      = "";
	std::string			type			= PROFILE_TYPE_PARENT;
	long long			recentDate		= mj::helper::getTime();
	std::string			region			= MJDEFAULT->getStringForKey(REGION_NAME_KEY, STRING_DEFAULT_VALUE);
	std::string			city			= MJDEFAULT->getStringForKey(CITY_NAME_KEY, STRING_DEFAULT_VALUE);
	std::string			country			= MJDEFAULT->getStringForKey(COUNTRY_NAME_KEY, STRING_DEFAULT_VALUE);
	float				latitude		= MJDEFAULT->getFloatForKey(LATITUDE_NAME_KEY,  NUMBER_DEFAULT_VALUE);
	float				longitude		= MJDEFAULT->getFloatForKey(LONGITUDE_NAME_KEY, NUMBER_DEFAULT_VALUE);
	std::string			deviceID		= mj::PlatformConfig::getInstance().getDeviceId();
	std::string			deviceName		=  mj::PlatformConfig::getInstance().getDeviceInfoShort();
	std::string			platform		= STRING_DEFAULT_VALUE;
	int					numOfChildren	= profileList.size();
	std::string			gender			= STRING_DEFAULT_VALUE;
	int					age				= NUMBER_DEFAULT_VALUE;
    std::string			msDisplayLang	= getDisplayLanguage();
	bool 				isActive 		= MJDEFAULT->getBoolForKey(key_Actived, false);
	int					numberProfile	= MJDEFAULT->getIntegerForKey(key_number_profile, 0);
	std::string			lastLearnActivity = MJDEFAULT->getStringForKey(key_name_last_learn_activity);
	bool is_active = MJDEFAULT->getBoolForKey(key_Actived, false);
	bool is_free = MJDEFAULT->getBoolForKey(key_isFreeUse, false);
	bool is_purchased = MJDEFAULT->getBoolForKey(key_isPurchased, false);
    
    std::string disableProfileStr = disableProfile? "True":"False";
	int purchased = is_active ? 2 : is_free ? 0 : is_purchased ? 3 : 1;
	if (lastLearnActivity.length() == 0)
	{
		lastLearnActivity = "none";
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	platform = "android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	platform = "ios";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	platform = "mac";
#else
	platform = "windows";
#endif

	CCLOG("pushProfile PROFILE_JSON_STR %s", PROFILE_JSON_STR);

	auto getProfileFunction = [&]()->std::string {

		return isPushLastLearnActivity ?
		StringUtils::format(PROFILE_JSON_STR_ACTIVITY, lastLearnActivity.c_str(), recentDate) :
		StringUtils::format(PROFILE_JSON_STR,
                                identity.c_str(),
                                email.c_str(),
                                phone.c_str(),
                                name.c_str(),
                                parentID.c_str(),
                                parentName.c_str(),
                                type.c_str(),
                                recentDate,
                                region.c_str(),
                                city.c_str(),
                                country.c_str(),
                                latitude,
                                longitude,
                                deviceID.c_str(),
                                deviceName.c_str(),
                                std::to_string(numOfChildren).c_str(),
                                std::to_string(numberProfile).c_str(),
                                lastLearnActivity.c_str(),
                                platform.c_str(),
                                numOfChildren,
                                gender.c_str(),
                                age,
                                purchased,
                                LANGUAGE_MANAGER->getDisplayLangId(),
                                msDisplayLang.c_str(),
                                disableProfileStr.c_str()
        ) ;

	};

	auto getProfileForApi = [&]()->std::string {

		return isPushLastLearnActivity ?
			StringUtils::format(PROFILE_JSON_STR_ACTIVITY_WIN_MAC, identity.c_str(), lastLearnActivity.c_str(), recentDate) :
			StringUtils::format(UPLOAD_PROFILE_BODY,
			identity.c_str(),
			objectId.c_str(),
			email.c_str(),
			phone.c_str(),
			name.c_str(),
			parentID.c_str(),
			parentName.c_str(),
			type.c_str(),
			recentDate,
			region.c_str(),
			city.c_str(),
			country.c_str(),
			latitude,
			longitude,
			deviceID.c_str(),
			deviceName.c_str(),
			numOfChildren,
			numberProfile,
			lastLearnActivity.c_str(),
			platform.c_str(),
			numOfChildren,
			gender.c_str(),
			age,
			purchased,
			LANGUAGE_MANAGER->getDisplayLangId(),
			msDisplayLang.c_str(),
            disableProfileStr.c_str()
		); };
	auto getProfileForApILogin = [&]()->std::string {

		return StringUtils::format(UPLOAD_USER_LOGIN,
				objectId.c_str(),
				"Mr. Robot",
				disableProfileStr.c_str()
			); };

	auto getProfileFromApILogin = [&]()->std::string {

		return StringUtils::format(UPLOAD_PROFILE_FROM_USER_LOGIN,
			objectId.c_str(),
			email.c_str(),
			phone.c_str(),
			identity.c_str(),
			"Mr. Robot",
			parentID.c_str(),
			parentName.c_str(),
			type.c_str(),
			recentDate,
			region.c_str(),
			city.c_str(),
			country.c_str(),
			latitude,
			longitude,
			deviceID.c_str(),
			deviceName.c_str(),
			numOfChildren,
			numberProfile,
			lastLearnActivity.c_str(),
			platform.c_str(),
			numOfChildren,
			gender.c_str(),
			age,
			purchased,
			LANGUAGE_MANAGER->getDisplayLangId(),
			msDisplayLang.c_str(),
			disableProfileStr.c_str()
		); };

	CCLOG("pushProfile PROFILE_JSON_STR %s", UPLOAD_PROFILE_BODY);

	if(i_profileID == -1) //this is a parent
	{

		switch (purpose)
		{
			switch (purpose)
			{
			case CleverTapManager::PUSH_BY_SDK:
				return getProfileFunction();
				break;
			case CleverTapManager::PUSH_BY_API:
				return getProfileForApi();
				break;
			case CleverTapManager::PUSH_BY_API_FOR_LOGIN_USER:
				return getProfileForApILogin();
				break;
			case CleverTapManager::PUSH_BY_API_UPDATE_PROFILE_FORM_LOGIN_USER:
				return getProfileFromApILogin();
				break;
			default:
				break;
			}
		}
	}

	auto profile = ap::Database::getInstance().getProfileById(i_profileID);
    if(disableProfile)
    {
       profile = getProfileInfoForDelete();
    }
    
	if (profile.profile_id > 0) {
		identity = std::to_string(userInfo._id) + std::to_string(profile.profile_id);
	}
    
    if(F2P_MANAGER->isSkipUser())
    {
        MJDEFAULT->setIntegerForKey("key_profile_skip_id_merge_clevertap", profile.profile_id);
        MJDEFAULT->setIntegerForKey("key_user_skip_id_merge_clevertap", userInfo._id);
        identity = "";
    }
    
    name = profile.name;
    parentID = std::to_string(userInfo._id);
    parentName = userInfo._name;
    type = PROFILE_TYPE_CHILD;
    gender = (profile.gender == ap::Profile::Gender::MALE ? "M" : "F");
    age = profile.age;

	switch (purpose)
	{
	case CleverTapManager::PUSH_BY_SDK:
		return getProfileFunction();
		break;
	case CleverTapManager::PUSH_BY_API:
		return getProfileForApi();
		break;
	case CleverTapManager::PUSH_BY_API_FOR_LOGIN_USER:
		return getProfileForApILogin();
		break;

	case CleverTapManager::PUSH_BY_API_UPDATE_PROFILE_FORM_LOGIN_USER:
		return getProfileFromApILogin();
		break;
	default:
		return getProfileFunction();
		break;
	}

}

void CleverTapManager::CleverTapApi(std::string data, std::function<void()> callback, int timeEventPush) {
	auto request = new cocos2d::network::HttpRequest();
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setUrl(CLEVERTAP_END_POINT);
	request->setHeaders({
		StringUtils::format("X-CleverTap-Account-Id: %s",CLEVERTAP_ACCOUTN_ID),
		StringUtils::format("X-CleverTap-Passcode: %s",CLEVERTAP_PASSCODE),
		"Content-Type: application/json; charset=utf-8"
		});
	request->setRequestData(data.data(), data.size());
	CCLOG("push event clevertap data: %s", data.c_str());
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		auto buffer = *response->getResponseData();
		auto buffer_string = std::string(buffer.begin(), buffer.end());
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI
		if (buffer_string.empty())
		{
			PROFILE_MANAGER.getInstance().setDataEvent(data, timeEventPush);
		}
#endif
		MK_RETURN_IF(buffer_string.empty());
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI
		if (timeEventPush != -1)
		{
			PROFILE_MANAGER.getInstance().deleteDataEvent(timeEventPush);
		}
#endif
		rapidjson::Document doc;
		doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

		rapidjson::StringBuffer rj_buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
		doc.Accept(writer);
		CCLOG("cleverTapurl: %s", request->getUrl());
		CCLOG("response for: %s", data.c_str());
		CCLOG("clevertap response:\n%s", rj_buffer.GetString());
		if (callback) {
			callback();
		}
	});
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
void CleverTapManager::CleverTapApi(std::string data, std::function<void(int responseCode)> callback) {
	auto request = new cocos2d::network::HttpRequest();
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setUrl(CLEVERTAP_END_POINT);
	request->setHeaders({
		StringUtils::format("X-CleverTap-Account-Id: %s",CLEVERTAP_ACCOUTN_ID),
		StringUtils::format("X-CleverTap-Passcode: %s",CLEVERTAP_PASSCODE),
		"Content-Type: application/json; charset=utf-8"
		});
	request->setRequestData(data.data(), data.size());
	CCLOG("push clevertap data: %s", data.c_str());
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		auto buffer = *response->getResponseData();
		auto buffer_string = std::string(buffer.begin(), buffer.end());
		if (callback) {
			callback(response->getResponseCode());
		}
		MK_RETURN_IF(buffer_string.empty());
		rapidjson::Document doc;
		doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

		rapidjson::StringBuffer rj_buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
		doc.Accept(writer);
		CCLOG("response for: %s", data.c_str());
		CCLOG("clevertap response:\n%s", rj_buffer.GetString());
		});
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void CleverTapManager::logEventByApi(std::string i_eventName, std::string i_eventProperties) {
	std::string identity = std::to_string(ap::Database::getInstance().getUserInfo()._id) + std::to_string(PROFILE_MANAGER.getInstance().getCurrentProfileId());

	std::string eventForm = "{\"d\":[{"
		"\"type\": \"event\","
		"\"identity\": \"%s\","
		"\"evtName\": \"%s\","
		"\"evtData\": %s"
		"}]}";
	if (PROFILE_MANAGER.getInstance().getCurrentProfileId() <= 0)
	{
		identity = MJDEFAULT->getStringForKey(CLEVER_TAP_OBJECTID_ID);
		eventForm = "{\"d\":[{"
			"\"type\": \"event\","
			"\"objectId\": \"%s\","
			"\"evtName\": \"%s\","
			"\"evtData\": %s"
			"}]}";
	}
	std::string finalJson = cocos2d::StringUtils::format(eventForm.c_str(), identity.c_str(), i_eventName.c_str(), i_eventProperties.c_str());
	CCLOG("xxxxxxxxxxxx%s", finalJson.c_str());

	int timeEvent = -1;

	if (i_eventName == "lost_connections")
	{
		timeEvent = 0;
	}

	CleverTapApi(finalJson, nullptr, timeEvent);
}
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS || CC_PLATFORM_HUAWEI) && IS_CLEVER_TAP_ENABLE
void CleverTapManager::loginDummyProfileForWinAndMac()
{
	std::string childProfileJsonString;
	std::string	cleverTapObjectId = StringUtils::format("%s_%s", MJ_PLATFORM_CONFIG.getDeviceId().c_str(), std::to_string(mj::helper::getTime()).c_str());
	if(MJDEFAULT->getStringForKey(CLEVER_TAP_OBJECTID_ID, "").empty())
		MJDEFAULT->setStringForKey(CLEVER_TAP_OBJECTID_ID, cleverTapObjectId);
	childProfileJsonString = getProfileJson(-1, false, CleverTapManager::JsonPurpose::PUSH_BY_API_FOR_LOGIN_USER);
	auto userInfo = ap::Database::getInstance().getUserInfo();
	auto profiles = ap::Database::getInstance().getProfiles();
	int profileId = PROFILE_MANAGER.getCurrentProfileId();
	if (profileId <= 0)
	{
		MJDEFAULT->setBoolForKey(CLEVER_TAP_IS_NEW_OBJECTID_ID, true);// doi voi thiet bi chua cai app/ hoac da go app va cai lai, se tao cleverTap id moi
		CleverTapApi(childProfileJsonString, nullptr, 0);
	}
}
void CleverTapManager::updateProfileFromloginForWinAndMac()
{
	std::string childProfileJsonString;
	childProfileJsonString = getProfileJson(PROFILE_MANAGER.getCurrentProfileId(), false, CleverTapManager::JsonPurpose::PUSH_BY_API_UPDATE_PROFILE_FORM_LOGIN_USER);
	if (MJDEFAULT->getBoolForKey(CLEVER_TAP_IS_NEW_OBJECTID_ID, false))
	{
		CleverTapApi(childProfileJsonString, nullptr, 0);
	}
	else
	{
		CleverTapManager::GetInstance()->pushProfileByID(PROFILE_MANAGER.getCurrentProfileId(), CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
	}
}

void CleverTapManager::loginClevertap() {

	
}
#endif



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)

void CleverTapManager::initProductConfig() {

}

void CleverTapManager::fechProductConfig() {
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "fetchProductConfig");
}

bool  CleverTapManager::getProductConfigBoolValue(std::string key) {
	fechProductConfig();
	activeClevertap();
	return cocos2d::JniHelper::callStaticBooleanMethod("org.cocos2dx.cpp.AppActivity", "getProductConfigBoolValue", key);
}

void  CleverTapManager::activeClevertap() {
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "activeClevertap");
}

void CleverTapManager::loginDummyProfile(){
    JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "loginDummyProfileCT", DUMMY_PROFILE);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string CleverTapManager::getCTIdentity(){
	return JniHelper::callStaticStringMethod("org.cocos2dx.cpp.AppActivity", "getClevertapIdCpp");
}
#endif
void CleverTapManager::initFeatureFlags() {
    

}

bool CleverTapManager::getFeatureFlagsBoolValue(std::string key,bool defaultValue) {
    
    return false;
}


extern "C"
{
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onCleverTapCallback(JNIEnv * env, jobject thiz)
{
    CCLOG("clever tap| called onCleverTapCallback native function");

    if (g_androidCleverTapCallback)
    {
        CCLOG("clever tap| g_androidCleverTapCallback != nullptr ");

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
            {
              //  cocos2d::Director::getInstance()->getRunningScene()->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f),CallFunc::create([=]()
                //{
                    g_androidCleverTapCallback();

               // })));
            });
    }
}
}


#elif (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)


void CleverTapManager::initProductConfig() {

}

void CleverTapManager::fechProductConfig() {
}

bool  CleverTapManager::getProductConfigBoolValue(std::string key) {
	return false;
}

void  CleverTapManager::activeClevertap() {
}

void CleverTapManager::initFeatureFlags() {

}

bool CleverTapManager::getFeatureFlagsBoolValue(std::string key, bool defaultValue) {

	return false;
}
#endif



