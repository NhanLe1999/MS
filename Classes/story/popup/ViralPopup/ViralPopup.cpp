//
//  ViralPopup.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#include "ViralPopup.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "MJLicenseAlert.h"
#include "LicenseInfoModel.h"
#include "MJWaitingHUD.h"
#include "MJAPISendMail.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"
#include "StoryAlert.h"

INITIALIZE_READER(ViralPopup);
//std::once_flag viral_popup_reader;

ViralPopup* ViralPopup::createPopup(bool from_popup ){
//    std::call_once(viral_popup_reader, [] {
//        REGISTER_CSB_READER(ViralPopup);
//        REGISTER_CSB_READER(ViralPopup_Item);
//    });
    
    auto p = static_cast<ViralPopup *>(CSLoader::createNodeWithVisibleSize("csb/ViralPopup.csb"));
    if (p) {
        p->didLoadFromCSB(from_popup);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void ViralPopup::show(bool from_popup ) {
    auto popup = ViralPopup::createPopup(from_popup);
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX - 1);
}

void ViralPopup::didLoadFromCSB(bool from_popup ){
    
    textField_name = utils::findChild<ui::TextField*>(this, "name");
    textField_name->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44 ));
    textField_name->setMaxLength(20);
    textField_name->addEventListener([=](Ref* sender, TextField::EventType type){
        string text = textField_name->getString();
        string new_text = "";
        switch (type) {
            case cocos2d::ui::TextField::EventType::INSERT_TEXT:
            case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
               
                for(int i = 0; i < text.length(); i++){
                    auto letter = text[i];
                    if(i == 0){
                        letter = toupper(letter);
                    }
                    if(text[i-1] == 32){
                        letter = toupper(letter);
                    }
                    new_text += letter ;
                }
                textField_name->setString(new_text);
                break;
            default:
                break;
        }
    });

    
    if(from_popup){
        auto item = ViralPopup_Item::createPopup();
        item->setName("popup");
        item->setDelegate(this);
        item->setAnchorPoint(Point::ANCHOR_MIDDLE);
        addChild(item);
        item->setPosition(Vec2(this->getContentSize()/2));
    }else{
        auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralPopup.csb"));
        runani->setTag(1011);
        this->runAction(runani);
        runani->play("show", false);
    }
    
}

void ViralPopup::onEnter(){
    Layout::onEnter();
    if(LANGUAGE_MANAGER->getDisplayLangId() == 4){
        is_vn = true;
    }
    
    utils::findChild<ui::Text*>(this, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.sub.popup.title").c_str());
    utils::findChild<ui::Text*>(this, "title_sub")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.sub.popup.content").c_str());
//    utils::findChild<ui::Button*>(this, "btn")->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("viral.sub.popup.button").c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    utils::findChild<ui::Button*>(this, "btn")->setTitleText(is_vn?"SAO CHÉP LINK":"COPY LINK");
    utils::findChild<ui::Button*>(this, "btn")->setTitleFontSize(25);
#endif
    if(MJDEFAULT->getStringForKey("user.name") != ""){
        textField_name->setString(MJDEFAULT->getStringForKey("user.name"));
    }else{
        textField_name->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("viral.sub.popup.name").c_str());
    }
}

void ViralPopup::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void ViralPopup::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

void ViralPopup::onClose(Ref* sender){
    if(!touch){
        return;
    }
    touch = false;
    dynamic_cast<UICCTextField*>(textField_name->getVirtualRenderer())->closeIME();
    auto ani_name = "hide";
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralPopup.csb"));
    runani->setTag(1010);
    this->runAction(runani);
    runani->play(ani_name, false);
    runani->setLastFrameCallFunc([=]{
        this->removeFromParent();
    });
}

void ViralPopup::onBtnShare(Ref* sender){
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralPopup.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    runani->play("show", false);
}

#include "MJRequest.h"
#include "MJPlatformConfig.h"
void ViralPopup::onSend(Ref* sender){
    CCLOG("send now");
    mj::helper::playButtonFX();
    
    if(textField_name->getString().empty()){
//        MJLicenseAlert::showAlert(0, is_vn?"Hãy nhập tên của bạn":"Please enter your name", "", is_vn?"Đóng":"Close", nullptr);
//        return;
    }else{
        MJDEFAULT->setStringForKey("user.name", textField_name->getString());
    }
  
    //TODO
    std::string url = mj::Request::getViralSharingURL(MJDEFAULT->getStringForKey("user.name"));
    MJ_PLATFORM_CONFIG.shareViral(url);
    onClose(this);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    StoryAlert::showAlert(is_vn? "Liên kết chia sẻ đã được sao chép. Hãy gửi cho bạn bè của mình!":"Shared link is copied to the clipboard. Let's share it with your friends!", "OK", "");
#endif
    MS_LOGGER.logEventGetSharedLink();
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback ViralPopup::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(ViralPopup, onClose),
        CLICK_MAP(ViralPopup, onSend),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
