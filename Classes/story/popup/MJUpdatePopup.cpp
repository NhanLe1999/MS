
#include "MJUpdatePopup.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "JsonDump.h"
#include "MJDefinitions.h"
INITIALIZE_READER(MJUpdatePopup);
USING_NS_CC;
std::once_flag MJUpdatePopup_reader;
#define CSB_NAME "csb/MJUpdatePopup.csb"

MJUpdatePopup * MJUpdatePopup::createUpdatePopup(bool is_required,std::string yes,std::string no) {
    std::call_once(MJUpdatePopup_reader, [] {
        REGISTER_CSB_READER(MJUpdatePopup);
    });
    auto p = static_cast<MJUpdatePopup *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(is_required,yes,no);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJUpdatePopup::didLoadFromCSB(bool is_required,std::string str_yes,std::string str_no) {
    scrollview = cocos2d::utils::findChild<cocos2d::ui::ScrollView *>(this, "scrollview");
    scrollview->setBackGroundColorOpacity(0);
    auto label_yes = cocos2d::utils::findChild<Text *>(this, "label_yes");
    auto label_no = cocos2d::utils::findChild<Button *>(this, "label_no");
 //   label_yes->setFontName("fonts/Lato-Black.ttf");
//    label_no->setFontName("fonts/Lato-Black.ttf");
    if (str_yes == ""){
        if (LANGUAGE_MANAGER->getDisplayLangId() == 4) {
            label_yes->setString("Cập nhật ngay!");
        }else {
            label_yes->setString("Update Now!");
        }
    }else{
        auto map_yes = JsonDump::getInstance()->dumpValueMapFromString(str_yes);
        if (LANGUAGE_MANAGER->getDisplayLangId() == 4) {
            if (!map_yes["vi"].isNull()){
                label_yes->setString(map_yes["vi"].asString());
            }else{
                label_yes->setString("Cập nhật ngay!");
            }
            
        }else {
            if (!map_yes["en-us"].isNull()){
                label_yes->setString(map_yes["en-us"].asString());
            }else{
                label_yes->setString("Update Now!");
            }
        }
    }
    if (str_no == ""){
        if (LANGUAGE_MANAGER->getDisplayLangId() == 4) {
            label_no->setTitleText("Bỏ qua");
        }else {
            label_no->setTitleText("Skip");
        }
    }else{
        label_no->setTitleText(str_no);
    }
    /*
    auto gach_ngang = cocos2d::ui::Layout::create();
    gach_ngang->setContentSize(cocos2d::Size(label_no->getContentSize().width, 2));
    gach_ngang->setAnchorPoint(Vec2(0, 1));
    gach_ngang->setPosition(Vec2(0, -1));
    gach_ngang->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    gach_ngang->setBackGroundColor(Color3B(label_no->getTextColor()));
    label_no->addChild(gach_ngang);
    */
    
    auto layout_content = cocos2d::ui::Layout::create();
    layout_content->setContentSize(scrollview->getContentSize());
    layout_content->setAnchorPoint(Vec2(0, 0));
    scrollview->addChild(layout_content);
    
    auto map_content = JsonDump::getInstance()->dumpValueMapFromString(MJDEFAULT->getStringForKey(key_update_content));
    
    auto label_message = cocos2d::Label::createWithTTF("", "fonts/monkeystories_medium.ttf", 30);
    if (!map_content["message"].isNull()){
        label_message->setString(map_content["message"].asString());
    }
    if (!map_content["title"].isNull()){
        auto title = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_title"));
        title->setFontName("fonts/monkeystories_medium.ttf");
        auto str_title = map_content["title"].asString();
        title->setString(str_title);
        if (str_title.size() >= 90) title->setFontSize(25);
    }
    
    label_message->setTextColor(Color4B::BLACK);
    label_message->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    label_message->setWidth(scrollview->getContentSize().width*0.9);
    layout_content->addChild(label_message);
    
    if (label_message->getContentSize().height > scrollview->getContentSize().height) {
        auto min_y = 20;
        label_message->setAnchorPoint(Vec2(0.5, 0));
        label_message->setPosition(Vec2(layout_content->getContentSize().width/2, min_y));
        layout_content->setContentSize(cocos2d::Size(layout_content->getContentSize().width, label_message->getContentSize().height + min_y));
        scrollview->setScrollBarEnabled(true);
    }else {
        label_message->setAnchorPoint(Vec2(0.5, 0.5));
        label_message->setPosition(layout_content->getContentSize()/2);
        scrollview->setTouchEnabled(false);
        scrollview->setScrollBarEnabled(false);
        
    }
    
    scrollview->setInnerContainerSize(layout_content->getContentSize());
    
    CCLOG("showupdate - status = %s", map_content["status"].asString().c_str());
    if (map_content["status"].asString() == "require" || is_required) {
        label_no->setVisible(false);
    }
    
    auto name_timeline = "csb/MJUpdatePopup.csb";
    auto timeline = CSLoader::createTimeline(name_timeline);
    this->runAction(timeline);
    timeline->play("show", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] {
        isRunning = false;
    });
}

void MJUpdatePopup::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJUpdatePopup::setDelegate(MJUpdatePopupDelegate *delegate){
    _delegate = delegate;
}

#include "StoryConfigManager.h"
void MJUpdatePopup::onOk(cocos2d::Ref *sender){
    if (isRunning) return;
    isRunning = true;
    auto name_timeline = "csb/MJUpdatePopup.csb";
    auto timeline = CSLoader::createTimeline(name_timeline);
    this->runAction(timeline);
    timeline->play("hide", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] {
        auto update_content = JsonDump::getInstance()->dumpValueMapFromString(MJDEFAULT->getStringForKey(key_update_content));
        std::string url_app = update_content["url"].asString();
        
        if (update_content["url"].asString().empty()) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if(CONFIG_MANAGER->isVietnameseStories()){
                url_app = key_appstore_app_vn_url;
            }else{
                url_app = key_appstore_app_url;
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(CONFIG_MANAGER->isVietnameseStories()){
                url_app = key_app_store_app_vn_android;
            }else{
                url_app = key_app_store_app_android;
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            if(CONFIG_MANAGER->isVietnameseStories()){
                url_app = key_app_store_app_vn_mac;
            }else{
                url_app = key_app_store_app_mac;
            }
#else
            if(CONFIG_MANAGER->isVietnameseStories()){
				url_app = key_app_store_app_vn_win32;
            }else{
                url_app = key_app_store_app_win32;
            }
#endif
        }
        Application::getInstance()->openURL(url_app);
        this->removeFromParent();

    });
}
void MJUpdatePopup::onCancel(cocos2d::Ref *sender){
    if (isRunning) return;
    isRunning = true;
    auto name_timeline = "csb/MJUpdatePopup.csb";
    auto timeline = CSLoader::createTimeline(name_timeline);
    this->runAction(timeline);
    timeline->play("hide", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] {
        this->removeFromParent();
    });
    MJDEFAULT->setBoolForKey(key_update_show, false);
    MJDEFAULT->setIntegerForKey(key_time_update_skip, MJDEFAULT->getIntegerForKey(key_time_curent));
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJUpdatePopup::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onOk") {
        return CC_CALLBACK_1(MJUpdatePopup::onOk, this);
    }
    
    if (callback_name == "onCancel") {
        return CC_CALLBACK_1(MJUpdatePopup::onCancel, this);
    }
    return nullptr;
}

