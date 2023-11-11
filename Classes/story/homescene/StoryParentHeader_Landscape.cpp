//
//  StoryParentHeader_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#include "StoryParentHeader_Landscape.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"

INITIALIZE_READER_INSTANCE(StoryParentHeader_LandscapeReader);

StoryParentHeader_Landscape * StoryParentHeader_Landscape::createHeader(const std::string &header) {
    
    auto csb_name = mj::helper::getCSBName("csb/StoryParentHeader_Landscape.csb");
    auto p = static_cast<StoryParentHeader_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    
    if (p)
    {
        auto title = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(p, "title"));
    
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        
        title->setString(header);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

StoryParentHeader_Landscape *StoryParentHeader_Landscape::createHeader(const std::string & str_userID, const string &str_deviceID) {
    auto csb_name = mj::helper::getCSBName("csb/StoryParentHeader_Landscape_userID.csb");
    
    auto p = static_cast<StoryParentHeader_Landscape *> (cocos2d::CSLoader::createNode(csb_name));

    if(p) {
        auto userID =  utils::findChild<cocos2d::ui::Text *>(p, "user_id");
        if(userID) {
            userID->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
            userID->setFontSize(15);
            userID->setString(str_userID);
        }
        auto deviceID = utils::findChild<cocos2d::ui::Text *>(p, "device_id");
        if(deviceID) {
            deviceID->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
            deviceID->setFontSize(15);
            deviceID->setString(str_deviceID);
        }
        
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void StoryParentHeader_Landscape::adjustWidth(float width) {
    this->setContentSize(cocos2d::Size(width, this->getContentSize().height));
    cocos2d::ui::Helper::doLayout(this);
    
    auto title = cocos2d::utils::findChild(this, "title");
    if(title) {
        title->setPosition(title->getPosition() + cocos2d::Vec2(MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
    }
    
}
