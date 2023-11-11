//
//  StoryFeedback.cpp
//  MJQuiz
//
//  Created by MonkeyStories.
//
//

#include "HSLib.h"
#include "ui/UIHelper.h"

#include "StoryFeedback.h"
#include "StoryDataModel.h"
#include "NativeBridge.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "StoryFreeManager.h"
#include "MJPay.h"
#include "MJHelper.h"
#include "MJAPISendFeedback.h"
#include "StoryRatePopUp.h"
#include "MJAPIRateApp.h"
#include "MJPlatformConfig.h"
#include "HSBaseScene.h"

INITIALIZE_READER_INSTANCE(StoryFeedbackReader);

USING_NS_CC;
using namespace std;
using namespace ui;

StoryFeedback * StoryFeedback::createPage() {
    StoryFeedback *p;
    p = static_cast<StoryFeedback *>(CSLoader::createNodeWithVisibleSize("csb/StoryFeedback.csb"));
    if (p) {
        p->initPage();
    }
    return p;
}

void StoryFeedback::initPage()
{
    didLoadFromCSB();
    
}

#pragma mark - onEnter
void StoryFeedback::onEnter()
{
    Layout::onEnter();
}

void StoryFeedback::onEnterTransitionDidFinish()
{
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryFeedback::onExitTransitionDidStart()
{
    Layout::onExitTransitionDidStart();
}

void StoryFeedback::didLoadFromCSB()
{
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    
    auto lang_display = LANGUAGE_MANAGER->getDisplayLangId();
    
    std::string bg_name;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        bg_name = "bg_ngang";
    }else bg_name = "bg_ngang";
    
    auto background = this->getChildByName<ImageView *>(bg_name);
    background->setVisible(true);
    
    if (CONFIG_MANAGER->isAppActive()) {
        background->getChildByName<Text *>("feedback.title")->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.title.active"));
    }else {
        background->getChildByName<Text *>("feedback.title")->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.title"));
        //background->getChildByName<Text *>("feedback.content")->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.content"));
        
    }
    static_cast<Text *>(ui::Helper::seekWidgetByName(background, "feedback.btsend"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.btsend"));
    
    textfield = static_cast<TextField *>(ui::Helper::seekWidgetByName(background, "textfield"));
    textfield->setPlaceHolder(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.textfield.placeholder"));
    
    textfield->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    content_label = static_cast<Text *>(ui::Helper::seekWidgetByName(background, "content_label"));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    content_label->setOpacity(0);
#else
    textfield->setOpacity(0);
    content_label->setString(textfield->getPlaceHolder());
    content_label->setTextColor(Color4B(97, 199, 237, 255));
#endif
//    textfield->setMaxLength(28);

}

void StoryFeedback::onClose(cocos2d::Ref *sender)
{
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    this->removeFromParent();
}

#include "StoryRateAction.h"
void StoryFeedback::onSend(cocos2d::Ref *sender)
{
    
    std::string content_text = textfield->getString();
    
    vector<string> arr = explode(content_text, ' ');
    if(arr.size() < 15) {
        auto lang_display = LANGUAGE_MANAGER->getDisplayLangId();
//        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.alert.content"), LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.alert.button"), "");

        auto current_scene = dynamic_cast<HSBaseScene *>(Director::getInstance()->getRunningScene());
        if (current_scene) {
            current_scene->showAlertWithContent(LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.alert.content"), LANGUAGE_MANAGER->getTextByKey(lang_display, "feedback.alert.button"), "");
        }

        return;
    }
    urlEncode(content_text);
    mj::MJAPISendFeedback::call(content_text);
    
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
   Director::getInstance()->getOpenGLView()->setIMEKeyboardState(false);
#endif
    
    
    //kTag_API_rated
    mj::RateAppAPI::call();
    //display popup bonus rated.
    MJDEFAULT->setBoolForKey(key_rate_app, true);
    MJDEFAULT->setBoolForKey(key_rate_app_won, true);
//    MJDEFAULT->setIntegerForKey(key_numstories_rate_bonus, 4);
    
    if (CONFIG_MANAGER->isAppActive()) {
        auto rate_bonus = StoryRatePopUp::createPage(StoryRatePopUp::RATE_THANKS);
        Director::getInstance()->getRunningScene()->addChild(rate_bonus, 19999999);
        rate_bonus->onShowHidePopUp(true);
    }else {
        auto rate_bonus = StoryRatePopUp::createPage(StoryRatePopUp::RATE_BONUS);
        Director::getInstance()->getRunningScene()->addChild(rate_bonus, 19999999);
        rate_bonus->onShowHidePopUp(true);
        MJDEFAULT->setIntegerForKey(key_rate_action, mjrate::StoryRateAction::FEEDBACK_ACTION);
        FREE_STORY_MANAGER->setRateTimeExpire(time_bonus_rate);
    }
    this->removeFromParent();
    
}

vector<string> StoryFeedback::explode(const string& str, const char& ch) {
    string next;
    vector<string> result;
    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
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


#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryFeedback::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryFeedback, onClose),
        CLICK_MAP(StoryFeedback, onSend),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}

Widget::ccWidgetEventCallback StoryFeedback::onLocateEventCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetEventCallback> function_map = {
        {"onTextfieldChanged", CC_CALLBACK_2(StoryFeedback::onTextfieldChanged, this)}
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

#pragma mark - TEXT FIELD
void StoryFeedback::onTextfieldChanged(cocos2d::Ref *pSender, int event){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    auto textfield_event = (TextField::EventType) event;
    switch (textfield_event)
    {
        case TextField::EventType::ATTACH_WITH_IME:
        {
            content_label->setString(textfield->getString());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            utils::findChild(this, "bg_ngang")->runAction(MoveBy::create(0.25f, Vec2(0, 200)));
#endif
        }
            break;
            
        case TextField::EventType::DETACH_WITH_IME:
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            utils::findChild(this, "bg_ngang")->runAction(MoveBy::create(0.25f, Vec2(0, -200)));
#endif
            break;
            
        case TextField::EventType::INSERT_TEXT:
        case TextField::EventType::DELETE_BACKWARD:{
            //CCLOG("Delete");
            content_label->setString(textfield->getString());
        }
            break;
            
        default:
            break;
    }
}
