//
//  JsonDump.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/8/16.
//
//

#include "WebService.h"
#include "NativeBridge.h"
#include "HSLib.h"
#include "MJPlatformConfig.h"
WebService* WebService::instance;
WebService* WebService::getInstance()
{
    if (instance == nullptr) {
        instance = new WebService();
        instance->buildDefaultVersion();
    }
    return instance;
}

void WebService::logUrl(string &url) {
    CCLOG("WebService URL: %s", url.c_str());
}

/*
 khoi tao cac gia tri mac dinh cho phan tuong tac voi server
 behocchu.com
 monkeyjunior.net
 54.187.133.151
 www.monkeyjunior.net
 www.behocchu.com
 */
void WebService::buildDefaultVersion() {
    serverNamespace.push_back("http://daybehoc.com/service/v1/story/");

    os_name = "ios";
    sub_version = mj::PlatformConfig::getInstance().getAppVersion();
    os_name = mj::PlatformConfig::getInstance().getOS();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    os_name = "android";
//    sub_version = "1.0.5";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    os_name = "win32";
//    sub_version =  "1.0.5";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//    os_name = "mac";
//    sub_version =  "1.0.5";
//#endif
    app_id = 40;
    version_id = 1;
    default_langid = 1;
    server_index = 0;
    server_namespace_index = MJDEFAULT->getIntegerForKey("server_namespace_index", 0);
    
}

void WebService::limitChange(int tag) {
    CCLOG("============ LIMIT CHANGE ============");
    CCLOG("Limit %i", tag);
    CCLOG("============ LIMIT CHANGE ============");
}

bool WebService::changeServerID() {
    server_index++;
    if(server_index >= serverID.size()) {
        server_index = 0;
        limitChange(1);
        return false;
    }
    return true;
}

bool WebService::changeServerNamespace() {
    server_namespace_index++;
    if(server_namespace_index >= serverNamespace.size()) {
        server_namespace_index = 0;
        MJDEFAULT->setIntegerForKey("server_namespace_index", 0);
        limitChange(2);
        return false;
    }
    MJDEFAULT->setIntegerForKey("server_namespace_index", server_namespace_index);
    return true;
}

bool WebService::checkLimitServerId() {
    if(server_index >= serverID.size()-1) {
        server_index = -1;
        limitChange(1);
        return true;
    }
    return false;
}
bool WebService::checkLimitServerNamespace(){
    if(server_namespace_index >= serverNamespace.size()-1) {
        server_namespace_index = -1;
        limitChange(2);
        return true;
    }
    return false;
}

int WebService::getServerID() {
    if (GlobalFunc::testUSDevice()) {
        return 1;
    }
    if(serverID.size() == 0) {
        string serverKey = MJDEFAULT->getStringForKey("server");
        rapidjson::Document document;
        document.Parse<0>(serverKey.c_str());
        rapidjson::Value &serverArray = document;
        for (rapidjson::SizeType j = 0; j < serverArray.Size(); j++) {
            log("Server = %d", serverArray[j].GetInt());
            serverID.push_back(serverArray[j].GetInt());
        }
    }
    if(server_index < 0) {
        server_index = 0;
    }
    return serverID.at(server_index);
}

void WebService::getServerNamespace() {
    if(server_namespace_index < 0) {
        server_namespace_index = 0;
    }
    base_url = serverNamespace.at(server_namespace_index);
    setParamsDefault();
}

void WebService::saveLastTimeUpdate() {
	int timex = (int)time(nullptr);
	MJDEFAULT->setIntegerForKey("lasttimeupdate", timex);
}

float WebService::getTimezoneOffset() {    
    return NativeBridge::getTimeZoneOffset();
}


void WebService::bindErrorData(string url, string error_content, int type) {
    ValueVector vector_error;
    string old_data = MJDEFAULT->getStringForKey("error_request", "");
    if(old_data.length() > 0) {
        vector_error = JsonDump::getInstance()->dumpValueVectorFromString(old_data);
    }
    ValueMap map_error;
    map_error["url"] = url;
    map_error["content"] = error_content;
    map_error["created"] = (int)time(nullptr);
    map_error["type"] = type;
    vector_error.push_back(Value(map_error));
    //parse thanh json luu vao user default
    string str = JsonDump::getInstance()->VectorToString(vector_error);
    MJDEFAULT->setStringForKey("error_request", str);
}

void WebService::syncErrorData() {
    string json_error = MJDEFAULT->getStringForKey("error_request", "");
    if(json_error.length() == 0) {
        return;
    }
    
    string urlBuild = StringUtils::format("error/index?appid=%i&device_id=%i", app_id, GlobalFunc::getDeviceID());
    getServerNamespace();
    string url = base_url + urlBuild;
    logUrl(url);
    //EduHttpRequest::getInstance()->postUrl(url, json_error);
}

void WebService::setParamsDefault(){
    auto device_id = MJ_PLATFORM_CONFIG.getDeviceId();
    urlEncode(device_id);
    params_default = StringUtils::format("&os=%s&app_id=%d&device_id=%s&app_ver=%s&device_type=%d&lang=%s", os_name.c_str(), app_id, device_id.c_str(),sub_version.c_str(), StoryData::getInstance()->isHdr?4:2, string("en-US").c_str());

}

#pragma mark - BUILD URL
std::string WebService::build_url_homepage(){
    string url = StringUtils::format("home?ver=%d", MJDEFAULT->getIntegerForKey(key_ver_api_home, 0));
    url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_explorepage(int lang_id, int level_id, int page_id){
    auto ver = MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d",key_ver_api_level, lang_id), 0);
    string url = StringUtils::format("level?lang_id=%d&ver=%d", lang_id, ver);
    url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
//    if (level_id == 0) {
//        url += StringUtils::format("&page=%d", page_id);
//    }else {
//        url += StringUtils::format("&level=%d", level_id);
//    }

    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_detailcharacter(int lang_id, int character_id){
    string url = StringUtils::format("character?id=%d&lang_id=%d&ver=%d", character_id,lang_id, 0);
    url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_detailtopic(int lang_id, int topic_id){
    string url = StringUtils::format("category?id=%d&lang_id=%d&ver=%d", topic_id,lang_id, 0);
    url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_getLinkDownloadStory(int lang_id, int story_id, int isHdr){
    string url = StringUtils::format("getstory?lang_id=%d&sid=%d&is_hdr=%d", lang_id, story_id, isHdr);
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_search(int lang_id, string text){
    urlEncode(text);
    string url = StringUtils::format("search?text=%s&lang_id=%d", text.c_str(), lang_id);
    url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_ratestory(ValueVector list_request){
    auto str_request = JsonDump::getInstance()->VectorToString(list_request);
    CCLOG("list_rate = %s", str_request.c_str());
    urlEncode(str_request);
    string url = StringUtils::format("ratestory?ratecontent=%s", str_request.c_str());
    //url += StringUtils::format("&isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
    
}

std::string WebService::build_url_freestories(){
    string url = StringUtils::format("freestory");
    url += StringUtils::format("?isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_getmessage(){
    string url = StringUtils::format("listmessage");
    url += StringUtils::format("?isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_getReadinglevel(){
    string url = StringUtils::format("getreadinglevel?");
    url += StringUtils::format("isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}

std::string WebService::build_url_getRatedApp(){
    string url = StringUtils::format("rateapp?");
    url += StringUtils::format("isbeta=%d", (int)CONFIG_MANAGER->isBetaVersion());
    getServerNamespace();
    url = base_url + url + params_default;
    logUrl(url);
    return url;
}


ValueMap WebService::build_urlpos_storydownloadsentdata(int story_lang_id, int type){
    ValueMap map_url;
    string url = StringUtils::format("senddata");
    getServerNamespace();
    url = base_url + url;
    string data = StringUtils::format("story_lang_id=%d&type=%d", story_lang_id, type);
    data += params_default;
    map_url["url"] = url;
    map_url["data"] = data;
    return map_url;
}

void WebService::urlEncode(string &s) {
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(int i=0, ix = (int)s.length(); i < ix; i++) {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
            (65 <= c && c <= 90) ||//abc...xyz
            (97 <= c && c <= 122) || //ABC...XYZ
            (c=='-' || c=='_' || c=='.' || c=='~')
            ) {
            e << c;
        } else {
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    s = e.str();
}


