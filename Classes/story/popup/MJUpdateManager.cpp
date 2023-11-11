//
//  MJUpdateManager.hpp
//
//  Created by HoiSung on 03/2/18.
//

#include "MJUpdateManager.h"
#include "MJDefault.h"
#include "JsonDump.h"
#include "MJUpdatePopup.h"
#include "MJDefinitions.h"
MJUpdateManager* MJUpdateManager::createUpdate(std::string current_ver_app)
{
    MJUpdateManager* obj = new MJUpdateManager();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadData(current_ver_app);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void MJUpdateManager::didLoadData(std::string current_ver_app) {
    version_app = current_ver_app;
}

void MJUpdateManager::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->setTouchEnabled(false);
    auto event_listener_process_refresh = EventListenerCustom::create(key_event_process_refresh_api_done, [=](EventCustom *) {
        this->onShowUpdatePopup();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener_process_refresh, this);
}

bool MJUpdateManager::onShowUpdatePopup(){
    if (MJDEFAULT->getStringForKey(key_update_content, "") == std::string("")) return false;
    auto map_content = JsonDump::getInstance()->dumpValueMapFromString(MJDEFAULT->getStringForKey(key_update_content));
    std::string string_yes = map_content["label_button"].asString();
    std::string string_no = "";
    if (checkVersionLess(version_app, MJDEFAULT->getStringForKey(key_version_required))) {
        showUpdateAppPopup(true,string_yes,string_no);
        MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
        return true;
    }else {
//        if (checkVersionLess(version_app, MJDEFAULT->getStringForKey(key_ver_update_app))) {
            auto time_skip = MJDEFAULT->getIntegerForKey(key_time_update_skip);
            if (time_skip <= 0) {
                showUpdateAppPopup(false,string_yes,string_no);
                MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                return true;
            }else {
                
                if (map_content["status"].asString() == "require") {
                    showUpdateAppPopup(false,string_yes,string_no);
                    MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                    return true;
                }else {
                    auto day_skip = LTDate(time_skip);
                    auto time_curent = MJDEFAULT->getIntegerForKey(key_time_curent);
                    auto day_current = LTDate(time_curent);
                    CCLOG("bao nhieu = %d", day_current - day_skip);
                    auto num_date_check_show_again = map_content["time_remind"].asInt();
                    
                    if (num_date_check_show_again == -1) {
                        //đổi OK, xử lý url
                        //check db xem đã add message chưa
                        //server push noti
                        showUpdateAppPopup(false,string_yes,string_no);
                        return true;
                    }
                    if (num_date_check_show_again == 0) {
                        num_date_check_show_again = time_show_update_again;
                    }
                    if ((day_current - day_skip) >= num_date_check_show_again*24*3600) {//Sau XXX ngày XXX hiện lại.
                        showUpdateAppPopup(false,string_yes,string_no);
                        MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                        return true;
                    }
                }
            }
//        }
    }
    return false;
}

void MJUpdateManager::showUpdateAppPopup(bool is_required,std::string str_yes,std::string str_no)
{
    if(auto scene = cocos2d::Director::getInstance()->getRunningScene())
    {
        auto check_node = scene->getChildByName("update_popup");
        
        if (check_node)
        {
            return;
        }
        
        auto update_popup = MJUpdatePopup::createUpdatePopup(is_required,str_yes,str_no);
        update_popup->setName("update_popup");
        cocos2d::Director::getInstance()->getRunningScene()->addChild(update_popup, 2000000999);
    }
}

#pragma mark - FUNTION LIB
bool MJUpdateManager::checkVersionLess(std::string ver1, std::string ver2){
    auto split_ver1 = splitVersion(ver1);
    auto split_ver2 = splitVersion(ver2);
    return checkVersionSplitLess(split_ver1, split_ver2);
}

bool MJUpdateManager::checkVersionSplitLess(std::vector<std::string> ver1, std::vector<std::string> ver2){
    auto value_ver1 = atoi(ver1[0].c_str())*10000+atoi(ver1[1].c_str())*100+atoi(ver1[2].c_str());
    auto value_ver2 = atoi(ver2[0].c_str())*10000+atoi(ver2[1].c_str())*100+atoi(ver2[2].c_str());
    if (value_ver1 < value_ver2) {
        return true;
    }else return false;
}

std::vector<std::string> MJUpdateManager::splitVersion(std::string version_str){
    auto arr_version_tach = this->explode(version_str, '.');
    if ((int)arr_version_tach.size() < 3) {
        for (int i = (int)arr_version_tach.size(); i < 3; i++) {
            arr_version_tach.push_back(std::string("0"));
        }
    }
    return arr_version_tach;
}


std::vector<std::string> MJUpdateManager::explode(const std::string& str, const char& ch) {
    std::string next;
    std::vector<std::string> result;
    // For each character in the string
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}


