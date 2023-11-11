//
//  JsonDump.h
//  JsonDump
//
//  Created by Ha Anh Man on 3/8/16.
//
//

#ifndef __WebService__H__
#define __WebService__H__


#include "base_include.h"
class WebService {
public:
    static WebService* getInstance();
    void buildDefaultVersion();
    string base_url = "";
    string os_name = "";
    string sub_version = "";
    string params_default = "";
    int app_id = 0;
    int version_id = 0;
    int default_langid = 0;
    int getServerID();
    float getTimezoneOffset();
    
    
    bool changeServerID();
    void getServerNamespace();
    bool changeServerNamespace();
    bool checkLimitServerId();
    bool checkLimitServerNamespace();
    /*
     tag: 1 => limit server id\n
     tag: 2 => limit server namespace
     */
    void limitChange(int tag);
    void saveLastTimeUpdate();
    void logUrl(string &url);
    
    int server_index = 0;
    vector<int> serverID  ;//= {};
    int server_namespace_index = 0;
    vector<string> serverNamespace ;//= {};
    void urlEncode(string &s);
    void bindErrorData(string url, string error_content, int type);
    void syncErrorData();
    
    void setParamsDefault();
    
    //build_url_GET
    std::string build_url_homepage();
    std::string build_url_explorepage(int lang_id, int level_id, int page_id);
    std::string build_url_detailcharacter(int lang_id, int character_id);
    std::string build_url_detailtopic(int lang_id, int topic_id);
    std::string build_url_getLinkDownloadStory(int lang_id, int story_id, int isHdr);
    std::string build_url_search(int lang_id, string text);
    std::string build_url_ratestory(ValueVector list_request);
    std::string build_url_freestories();
    std::string build_url_getmessage();
    std::string build_url_getReadinglevel();
    std::string build_url_getRatedApp();
    
    //build_url_POST
    ValueMap build_urlpos_storydownloadsentdata(int story_lang_id, int type);

private:
    static WebService* instance;
};

#endif /* defined(__WebService__H__) */
