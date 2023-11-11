#include "APUpdateUserInfoAPI.h"
#include "APStoreErrorAPI.h"
#include "APDatabase.h"

#include "json/stringbuffer.h"
#include "json/writer.h"

#define MAX_RETRY 3

// AUTHOR: Hieu Nguyen
void ap::api::UpdateUserInfo::call(const std::string& name, const std::string& phone, bool is_retry, const std::function<void(bool is_success, std::string msg)> & callback)
{
    auto api_call = new UpdateUserInfo(name, phone, is_retry, callback);
    api_call->send();
}
// -- end --

void ap::api::UpdateUserInfo::call(const UserInfo & user_info, bool is_retry, const std::function<void(bool is_success, std::string msg)>& callback)
{
	auto api_call = new UpdateUserInfo(user_info, is_retry, callback);
	api_call->send();
}

void ap::api::UpdateUserInfo::send() const
{
	if (_request) {
		_request->send();
	}
}

// AUTHOR: Hieu Nguyen
ap::api::UpdateUserInfo::UpdateUserInfo(const std::string& name, const std::string& phone, bool is_retry, const std::function<void(bool is_success, std::string msg)> & callback) : _is_retry(is_retry), _callback(callback)
{
    _request = new Base(api::PlatformType::NEWV2, "account/update-info", {
        {"user_name", name},
        {"phone", phone},
        {"sync_clevertap", "1"}
    }
    , api::RequestType::POST);
    _request->setDelegate(this);
    _request->enableLog();
}
// -- end --

ap::api::UpdateUserInfo::UpdateUserInfo(const UserInfo & user_info, const bool is_retry, const std::function<void(bool is_success, std::string msg)>& callback) : _callback(callback), _info(user_info)
{
    _is_retry = is_retry;
    CCLOG("profile_item %s:", buildConfigItem().c_str());
	_request = new Base(api::PlatformType::NEW, "account/update-info", {
		{"phone",user_info._phone},
		{"email",user_info._mail},
		{"user_name",user_info._name},
        {"address", user_info._address},
        {"is_new", "1"},
        //{"config_item", buildConfigItem()},
        {"sync_clevertap", "1"}
    }
	, api::RequestType::POST);
	_request->setDelegate(this);
	_request->enableLog();
}

void ap::api::UpdateUserInfo::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data)
{
	// Cập nhập thông tin người dùng vào database
	ap::Database::getInstance().saveUserInfo(std::to_string(_info._id), _info._name, _info._mail, _info._phone, _info._address, _info._userName);
	if (_callback) {
		_callback(true, "");
	}
	delete this;
}

void ap::api::UpdateUserInfo::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data)
{
    StoreError::call(_request->getURL(), message, "account/update-info", _request->getParam());
    if (_callback) {
        _callback(false, message);
    }
    delete this;
    
}

//#include "DatabaseHelper.h"
#include "JsonDump.h"
#include "APProfileManager.h"
std::string ap::api::UpdateUserInfo::buildConfigItem()
{
    std::vector<std::string> key_string = {
         "mj_license_key"
    };
    
    std::vector<std::string> key_bool = {
        "mj_onboarding_part3", "mj_onboarding_part2", "key_is_licence",key_first_time_created_profile, key_data_converted
    };
    
    std::vector<std::string> key_integer = {
        "key_count_guide_story", "key_current_profile_id", "key_lang_display"
    };
    
    cocos2d::ValueMap user_default_list;
    if(MJDEFAULT->getBoolForKey("mj_onboarding_license")){
        user_default_list["mj_onboarding_license"] = MJDEFAULT->getBoolForKey("mj_onboarding_license");
    }
    for(auto key : key_string){
        user_default_list[key] = MJDEFAULT->getStringForKey(key);
    }
    
    for(auto key : key_bool){
        user_default_list[key] = MJDEFAULT->getBoolForKey(key);
    }
    
    for(auto key : key_integer){
        if(key == "key_lang_display"){
             user_default_list[key] = MJDEFAULT->getIntegerForKey(key);
        }else{
             user_default_list[key] = MJDEFAULT->getIntegerForKey(key);
        }
       
    }
    
//    auto config_item = MJ_PLATFORM_CONFIG.getUserDefaults();
    auto config_item = JsonDump::getInstance()->MapToString(user_default_list);
    CCLOG("config_item: %s", config_item.c_str());
    
    rapidjson::Document doc;
    // define document as object
    doc.SetObject();
    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    rapidjson::Value user_default(rapidjson::kObjectType);
    user_default.SetString(config_item.c_str(), allocator);
    doc.AddMember("user_default", user_default, allocator);
    
    auto p_ver = MJDEFAULT->getIntegerForKey(profile_version_control, 0);
    doc.AddMember("version_control", p_ver++, allocator);
    
    auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id);
    
    rapidjson::Value profile_list(rapidjson::kArrayType);
    for(auto profile : ap::Database::getInstance().getProfiles()){
        
        rapidjson::Value o_profile(rapidjson::kObjectType);
        o_profile.SetObject();
        
//        rapidjson::Value avatar_path;
//        avatar_path.SetString(profile.avatar.c_str(), allocator);
//        o_profile.AddMember("avatar_path", avatar_path, allocator);
        o_profile.AddMember("profile_id", profile.profile_id, allocator);
        
        PROFILE_MANAGER.switchProfile(profile.profile_id);
        auto map = PROFILE_MANAGER.getAllDatabase();
        rapidjson::Value d_profile(rapidjson::kObjectType);
        d_profile.SetObject();
        if(map.size() > 0){
            for(auto p: map ){
                rapidjson::Value key (rapidjson::kStringType);
                rapidjson::Value value;
                key.SetString(p.first.c_str(), allocator);
                value.SetString(p.second.c_str(), allocator );
                d_profile.AddMember(key, value, allocator);
            }
        }
        o_profile.AddMember("profile_detail", d_profile, allocator);
        
        auto stories = PROFILE_MANAGER.getAllTable("tbl_story_read");
        rapidjson::Value d_stories(rapidjson::kObjectType);
        d_stories.SetObject();
        if(stories.size() > 0){
            for(auto p: stories ){
                rapidjson::Value key (rapidjson::kStringType);
                rapidjson::Value value;
                key.SetString(p.first.c_str(), allocator);
                value.SetString(p.second.c_str(), allocator );
                d_stories.AddMember(key, value, allocator);
            }
        }
        o_profile.AddMember("stories_read", d_stories, allocator);
        
        auto audios = PROFILE_MANAGER.getAllTable("tbl_audio_read");
        rapidjson::Value d_audios(rapidjson::kObjectType);
        d_audios.SetObject();
        if(audios.size() > 0){
            for(auto p: audios ){
                rapidjson::Value key (rapidjson::kStringType);
                rapidjson::Value value;
                key.SetString(p.first.c_str(), allocator);
                value.SetString(p.second.c_str(), allocator );
                d_audios.AddMember(key, value, allocator);
            }
        }
        o_profile.AddMember("audios_read", d_audios, allocator);
        
        auto lessons = PROFILE_MANAGER.getAllTable("tbl_lesson_learned");
        rapidjson::Value d_lessons(rapidjson::kObjectType);
        d_lessons.SetObject();
        if(lessons.size() > 0){
            for(auto p: lessons ){
                rapidjson::Value key (rapidjson::kStringType);
                rapidjson::Value value;
                key.SetString(p.first.c_str(), allocator);
                value.SetString(p.second.c_str(), allocator );
                d_lessons.AddMember(key, value, allocator);
            }
        }
        o_profile.AddMember("lesson_learned", d_lessons, allocator);
        
        profile_list.PushBack(o_profile, allocator);
    }

    //switch lại profile
    PROFILE_MANAGER.switchProfile(current_profile_id);
    
    doc.AddMember("profile_list", profile_list, allocator);
    
    // convert json to string
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);
    return strbuf.GetString();
}
