//
//  ViralPopup_Item.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#include "ViralPopup_Item.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "ViralPopup.h"
#include "MSEventLogger.h"

USING_NS_CC;
INITIALIZE_READER(ViralPopup_Item);

ViralPopup_Item* ViralPopup_Item::createPopup(){
    std::string csb_name = "csb/ViralPopup_Item.csb";
    auto p = static_cast<ViralPopup_Item *>(CSLoader::createNode(csb_name));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void ViralPopup_Item::didLoadFromCSB(){
    onPlayEffect(true);
}

void ViralPopup_Item::onPlayEffect(bool is_shown, std::function<void()> callback){
    std::string csb_name = "csb/ViralPopup_Item.csb";
    
    auto runani = CSLoader::createTimeline(csb_name);
    runani->setTag(999);
    this->runAction(runani);
    if (is_shown) {
        runani->play("show", false);
    }else runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        if(!is_shown){
            this->removeFromParent();
        }
        if(callback){
            callback();
        }
    });
}

void ViralPopup_Item::onEnter(){
    Layout::onEnter();
    utils::findChild<ui::Text*>(this, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.popup.title").c_str());
    utils::findChild<ui::Text*>(this, "content")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.popup.content").c_str());
    utils::findChild<ui::Text*>(this, "titlebtn")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.popup.button").c_str());
    utils::findChild<ui::Text*>(this, "btreceive")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.receive.text").c_str());
    

}

void ViralPopup_Item::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void ViralPopup_Item::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
    
}

void ViralPopup_Item::onShare(Ref* sender){
    CCLOG("share your code");
    mj::helper::playButtonFX();
    if(_delegate ){
        MS_LOGGER.logEventShareButton("from_freetab");
        onPlayEffect(false, [=](){
             _delegate->onBtnShare(sender);
        });
    }else{
         MS_LOGGER.logEventShareButton("from_notification");
        ViralPopup::show(false);
    }
}

void ViralPopup_Item::onClose(Ref* sender){
    auto ani_name = "hide";
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralPopup_Item.csb"));
    runani->setTag(1010);
    this->runAction(runani);
    runani->play(ani_name, false);
    runani->setLastFrameCallFunc([=]{
        if(_delegate){
            this->getParent()->removeFromParent();
        }else{
            this->removeFromParent();
        }
    });
}

#include "ViralReceiveGift.h"
void ViralPopup_Item::onReceiveGift(Ref *sender){
    onClose(sender);
    ViralReceiveGift::show();
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback ViralPopup_Item::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(ViralPopup_Item, onShare),
        CLICK_MAP(ViralPopup_Item, onClose),
        CLICK_MAP(ViralPopup_Item, onReceiveGift),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
