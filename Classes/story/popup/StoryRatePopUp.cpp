//
//  StoryRatePopUp.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryRatePopUp.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryAlert.h"
#include "MJHelper.h"
INITIALIZE_READER_INSTANCE(StoryRatePopUpReader);

USING_NS_CC;
using namespace ui;

StoryRatePopUp * StoryRatePopUp::createPage(int type) {
    StoryRatePopUp *p;
    string csb_name;
    switch (type) {
        case RATE_FREE:
            csb_name = mj::helper::getCSBName("csb/StoryRatePopUp.csb");
            p = static_cast<StoryRatePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
            break;
        case RATE_BONUS:
            csb_name = mj::helper::getCSBName("csb/StoryRateBonus.csb");
            p = static_cast<StoryRatePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
            break;
        case RATE_ACTIVE:
            csb_name = mj::helper::getCSBName("csb/StoryRatePopUp_Buy.csb");
            p = static_cast<StoryRatePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
            break;
        case RATE_THANKS:
            csb_name = mj::helper::getCSBName("csb/StoryRateThanks.csb");
            p = static_cast<StoryRatePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
            break;
        default:
            csb_name = mj::helper::getCSBName("csb/StoryRatePopUp.csb");
            p = static_cast<StoryRatePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
            break;
    }

    if (p) {
        p->didLoadFromCSB(type);
    }
    return p;
}

void StoryRatePopUp::didLoadFromCSB(int type) {
    _type = type;
    auto rate_bg = this->getChildByName("rate_bg");
    title = rate_bg->getChildByName<Text *>("title");
    content = rate_bg->getChildByName<Text *>("content");
    num_storieslb = rate_bg->getChildByName<Text *>("num_storieslb");
//    auto num_bonus = 4;
//    lbbtrate =
    auto lang_display = LANGUAGE_MANAGER->getDisplayLangId();
    switch (type) {
        case RATE_FREE:
        {
            
            lbbtrate = rate_bg->getChildByName("btrate")->getChildByName<Text *>("title");
            title->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "rate.popup.title"));
            content->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "rate.popup.content"));
            lbbtrate->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "rate.popup.bt"));
            num_storieslb->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "rate.popup.num"));
        }
            break;
            
        case RATE_BONUS:
        {
            lbbtrate = rate_bg->getChildByName("btok")->getChildByName<Text *>("title");
//            content->setString("Stories");
            alertlb = rate_bg->getChildByName<Text *>("alertlb");
            
            title->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "bonus_title"));
//            content->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "bonus_content"));
            alertlb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "bonus_content2"));
//            num_storieslb->setString(StringUtils::toString(num_bonus));
            auto numgift = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "numgift");
            numgift->loadTexture(StringUtils::format("mjstory/popup/ratebonus_num_gift_%d.png", lang_display));
            numgift->ignoreContentAdaptWithSize(false);

        }
            break;
            
        case RATE_ACTIVE:
        {
            lbbtrate = rate_bg->getChildByName("btrate")->getChildByName<Text *>("title");
            lbbtrate->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "ratebuy_bt"));
            title->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "rate.active.title"));
        }
            break;
        case RATE_THANKS:
        {
            cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "rate.thanks.title"));
            cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "close_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "rate.thanks.close"));

        }
        default:
            break;
    }
    
}
#pragma mark - onEnter
void StoryRatePopUp::onEnter() {
    Layout::onEnter();
}

void StoryRatePopUp::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryRatePopUp::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();

}

void StoryRatePopUp::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    auto csb_name = std::string("csb/StoryRatePopUp.csb");
    if (_type == RATE_ACTIVE) {
        csb_name = std::string("csb/StoryRatePopUp_Buy.csb");
    }else if (_type == RATE_THANKS) {
        csb_name = std::string("csb/StoryRateThanks.csb");
    }else if (_type == RATE_BONUS) {
        csb_name = std::string("csb/StoryRateBonus.csb");
    }
    
    auto runani = CSLoader::createTimeline(csb_name);
    runani->setTag(1011);
    this->runAction(runani);
    if (isShow) {
        runani->play("show", false);
    }else runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        isRunning = false;
        if (!isShow) {
            this->removeFromParent();
        }
    });
}

void StoryRatePopUp::showNetwordDown(){
    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setNetworkDownContent(false, true);
    alert->show();
}

#pragma mark - BUTTON
void StoryRatePopUp::onRate(cocos2d::Ref *sender){
    if (isRunning) return;
    if (NativeBridge::networkAvaiable()) {
        MJDEFAULT->setBoolForKey(key_rate_app, true);
        this->removeFromParent();
        NativeBridge::rateAppMJStories();
    }else{
        showNetwordDown();
    }
}

void StoryRatePopUp::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryRatePopUp::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryRatePopUp, onRate),
        CLICK_MAP(StoryRatePopUp, onClose),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


