//
//  HSBaseScene.cpp
//  MonkeyJunior
//
//  Created by developer on 3/9/16.
//
//
#include "HSBaseScene.h"
#include "NativeBridge.h"
#include "HSLib.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <regex.h>
#endif


#define ktag_indicator 200

#include "MJHelper.h"
void HSBaseScene::showAlertWithContent(std::string content,std::string btOk, std::string btCancel, int tag, bool isOneButton, int fontSize){
    auto csb_name = mj::helper::getCSBName("csb/StoryAlert.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setContent(content, btOk, btCancel, isOneButton, fontSize);
    alert->setTag(tag);
    alert->show();
    if (tag != 0) {
        alert->setDelegate(this);
    }
}

void HSBaseScene::showAlertHaiLongApp(std::string content, std::string btOk, std::string btCancel, std::string btguiderate, int tag){
    auto csb_name = mj::helper::getCSBName("csb/StoryRateConfirm.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setAlertConfirmRate(content, btOk, btCancel, btguiderate);
    alert->setTag(tag);
    alert->isRateConfirm = true;
    alert->show();
    if (tag != 0) {
        alert->setDelegate(this);
    }
}

void HSBaseScene::showAlertNetworkDown(int tag, bool isPortrait, bool isOnbt){
    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setNetworkDownContent(isPortrait, isOnbt);
    alert->setTag(tag);
    alert->show();
    if (tag != 0) {
        alert->setDelegate(this);
    }
}

void HSBaseScene::showUpdateAppPopup(int tag, bool is_required){
    //TODO: vn_update_popup csb?
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode("csb/StoryUpdate.csb"));
    alert->setUpdatePopupContent(is_required);
    alert->setTag(tag);
    alert->show(this);
    if (tag != 0) {
        alert->setDelegate(this);
    }
}

void HSBaseScene::onAlertOK(cocos2d::Ref * sender){}

void HSBaseScene::onAlertCancel(cocos2d::Ref * sender){}

void HSBaseScene::requestUrl(std::string url, int tagRequest){
    CCLOG("url_get: %s\n tag_request = %d", url.c_str(), tagRequest);
    if (request) {
        request->setResponseCallback(nullptr);
        request->release();
        request = nullptr;
    }
    request = new cocos2d::network::HttpRequest();
    request->setUrl(url.c_str());
    request->setTag(cocos2d::StringUtils::toString(tagRequest).c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient * p_client, cocos2d::network::HttpResponse * p_response) {
        int request_id = atoi(p_response->getHttpRequest()->getTag());
        int code = (int)p_response->getResponseCode();
        if (!p_response) {
            return;
        }
        if (!p_response->isSucceed()) {
            return;
        }
        if (p_response->isSucceed() && code == 200) {
            std::vector<char>* buffer = p_response->getResponseData();
            buffer->push_back('\0');
            rapidjson::Document serverData;
            if (!serverData.HasParseError()) {
                serverData.Parse<0>(std::string(&(*buffer)[0]).c_str());
                rapidjson::Value &vl = serverData;
                ValueMap map_response = JsonDump::getInstance()->dumpValueMap(vl);
                callbackRequestUrl(map_response, request_id);
            }
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
}


void HSBaseScene::requestPostDataWithUrl(std::string url, std::string data, int tagRequest){
    CCLOG("url_post: %s, data = %s", url.c_str(), data.c_str());
    request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setUrl(url.c_str());
    request->setRequestData(data.c_str(), data.length());
    request->setTag(cocos2d::StringUtils::toString(tagRequest).c_str());
    
    request->setResponseCallback([=](cocos2d::network::HttpClient * p_client, cocos2d::network::HttpResponse * p_response) {
        int request_id = atoi(p_response->getHttpRequest()->getTag());
        if (!p_response) {
            return;
        }
        if (!p_response->isSucceed()) {
            return;
        }
        int code = (int)p_response->getResponseCode();
        if (p_response->isSucceed() && code == 200) {
            std::vector<char>* buffer = p_response->getResponseData();
            buffer->push_back('\0');
            rapidjson::Document serverData;
            if (!serverData.HasParseError()) {
                serverData.Parse<0>(std::string(&(*buffer)[0]).c_str());
                
                rapidjson::Value &vl = serverData;
                
                ValueMap map_response = JsonDump::getInstance()->dumpValueMap(vl);
                callbackRequestUrl(map_response, request_id);
            }
            
//            auto raw_data = std::string(p_response->getResponseData()->data(), p_response->getResponseData()->size());
//            CCLOG("data = %s", raw_data.c_str());
//            ValueMap map_responseX = JsonDump::getInstance()->dumpValueMapFromString(raw_data);
//            callbackRequestUrl(map_responseX, request_id);
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    //cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(15);
}


void HSBaseScene::callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest){}
int HSBaseScene::matchRegex(const char *string)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[A-Za-z0-9.-]+.[A-Za-z]{2,4}$";
    int    status;
    regex_t    re;
    
    
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* Report error. */
    }
    return(1);
#endif
    return(1);
    
}
void HSBaseScene::trim(std::string& str, string character)
{
    str.erase(0, str.find_first_not_of(character));       //prefixing spaces
    str.erase(str.find_last_not_of(character)+1);         //surfixing spaces    
}
void HSBaseScene::urlEncode(string &s) {
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

void HSBaseScene::showIndicator(bool isShow){
    auto checklayout = this->getChildByName("panelindicator");
    if (isShow) {
        if (!checklayout) {
            auto indicator = cocos2d::ui::Layout::create();
            indicator->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            indicator->setBackGroundColor(cocos2d::Color3B::BLACK);
            indicator->setOpacity(100);
            indicator->setContentSize(XHSLib->getSizeScreen());
            indicator->setTouchEnabled(true);
            indicator->setName("panelindicator");
            this->addChild(indicator, 1000000);
            
            auto img = ui::ImageView::create("img/indicator.png");
            img->setPosition(Vec2(indicator->getContentSize().width/2, indicator->getContentSize().height/2));
            indicator->addChild(img);
            img->runAction(RepeatForever::create(RotateBy::create(0.5f, 180)));
        }
    }else{
        auto checklayout = this->getChildByName("panelindicator");
        if (checklayout) {
            checklayout->removeFromParent();
        }
    }
}

void HSBaseScene::showPleaseWait(bool isShow, bool isPortrait){
    auto checklayout = this->getChildByName("panelindicator");
    if (isShow) {
        if (!checklayout) {
            auto indicator = cocos2d::ui::Layout::create();
            indicator->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            indicator->setBackGroundColor(cocos2d::Color3B::BLACK);
            indicator->setOpacity(180);
            indicator->setContentSize(this->getContentSize());
            indicator->setTouchEnabled(true);
            indicator->setName("panelindicator");
            this->addChild(indicator, 1000000);
            
            auto text = Text::create("", "fonts/monkeystories_medium.ttf", 60);
            text->setString("Please wait...");
            if(CONFIG_MANAGER->isVietnameseStories()){
                text->setString("Vui lòng đợi...");
            }
            text->setPosition(Vec2(indicator->getContentSize().width/2, indicator->getContentSize().height*0.3));
            indicator->addChild(text);
            text->setName("text");
            
            if (isPortrait) {
                text->setRotation(-90);
                text->setPosition(Vec2(indicator->getContentSize().width/2, indicator->getContentSize().height*0.5));
            }
        }
    }else{
        if (checklayout) {
            checklayout->removeFromParent();
        }
    }
}


void HSBaseScene::releaseRequest(){
    if (request) {
        request->setResponseCallback(nullptr);
        request->release();
        request = nullptr;
    }
    showIndicator(false);
}

void HSBaseScene::onExitTransitionDidStart()
{
    Scene::onExitTransitionDidStart();
    if (XHSLib->pop_scene_index > 0) return;
}

void HSBaseScene::onExit(){
    Scene::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}




