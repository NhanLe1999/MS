//
//  StoryParentCoupon_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#include "StoryParentCoupon_Landscape.h"
#include "MJDefault.h"
#include "HSLib.h"
#include "MJPay.h"
#include "MJDefinitions.h"
#include "StoryAlert.h"
#include "MJPlatformConfig.h"
#include "HSBaseScene.h"
#include "NativeBridge.h"

INITIALIZE_READER_INSTANCE(StoryParentCoupon_LandscapeReader);

StoryParentCoupon_Landscape * StoryParentCoupon_Landscape::createPage() {
    StoryParentCoupon_Landscape *p;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        p = static_cast<StoryParentCoupon_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentCoupon_Landscape.csb"));
    }else p = static_cast<StoryParentCoupon_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentCoupon.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}


void StoryParentCoupon_Landscape::onEnter() {
    cocos2d::ui::Layout::onEnter();

    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
    background = static_cast<ImageView *>(cocos2d::ui::Helper::seekWidgetByName(this, "coupon_bg"));

    if (mj::Pay::getInstance().getCoupon().empty() || mj::Pay::getInstance().getCoupon() == "40OFF") {
        cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_coupon")->setVisible(false);
        background->setContentSize(cocos2d::Size(background->getContentSize().width, 200));
        cocos2d::ui::Helper::doLayout(this);
    } else {
        cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_coupon")->setVisible(true);
        background->setContentSize(cocos2d::Size(background->getContentSize().width, 350));
        cocos2d::ui::Helper::doLayout(this);

        auto text_license = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "text_coupon"));
        text_license->setString(mj::Pay::getInstance().getCoupon());
        auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "parent.coupon.discount"));
        text_details->setString(cocos2d::StringUtils::format("%s: %d%%", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.coupon.discount").c_str(), mj::Pay::getInstance().getDiscountPercent()));
    }
    pos_bottom = background->getPosition();
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        mj::PlatformConfig::getInstance().rotateKeyboard(true);
        pos_top = Vec2(pos_bottom.x - 100, pos_bottom.y);
    }else pos_top = Vec2(pos_bottom.x, pos_bottom.y + 200);

    
}

void StoryParentCoupon_Landscape::didLoadFromCSB(){
    StoryParentSubPage_Landscape::didLoadFromCSB();

    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    utils::findChild<ui::Text *>(this, "parent.coupon.info")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.coupon.info"));
    utils::findChild<ui::Text *>(this, "parent.coupon.discount")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.coupon.discount"));
    utils::findChild<ui::Text *>(this, "parent.coupon.ask")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.coupon.ask"));

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        this->setContentSize(DIRECTOR->getVisibleSize());
        ui::Helper::doLayout(this);
    }
    changeDisplayLanguage();
    textfield_license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_coupon"));
    textfield_license->setPlaceHolder("");
    textfield_license->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textfield_license->setCursorEnabled(true);
    
}


void StoryParentCoupon_Landscape::changeDisplayLanguage(){
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_portrait"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "coupon_title_portrait"));
    }

}

void StoryParentCoupon_Landscape::onShow(){
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->setPositionY(768);
        root_layout->runAction(MoveBy::create(0.25f, Vec2(0, -768)));
    }
}

void StoryParentCoupon_Landscape::onOK(Ref * sender) {
    if (!NativeBridge::networkAvaiable()) {
        auto scene = dynamic_cast<HSBaseScene *>(Director::getInstance()->getRunningScene());
        if (scene) {
            scene->showAlertNetworkDown(100, !CONFIG_MANAGER->isHomeScreenLandscape(), true);
        }
        return;
    }

    if (textfield_license->getString() == "") return;
    startLoading();
    mj::Pay::getInstance().setValidateCouponDelegate(this);
    mj::Pay::getInstance().setCoupon(textfield_license->getString());
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(false);

}

void StoryParentCoupon_Landscape::onBack(Ref *sender){
    if (isBack) return;
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        mj::PlatformConfig::getInstance().rotateKeyboard(false);
    }
    isBack = true;
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentCoupon_Landscape::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentCoupon_Landscape, onBack),
        CLICK_MAP(StoryParentCoupon_Landscape, onOK)
    };
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (callback_name == "onBack") {
        
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onBack(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    }
#endif
    
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


void StoryParentCoupon_Landscape::startLoading() {
    cocos2d::utils::findChild(this, "button")->setVisible(false);
    auto timeline = CSLoader::createTimeline("csb/StoryParentCoupon_Landscape.csb");
    timeline->play("preloader", true);
    this->runAction(timeline);
}

void StoryParentCoupon_Landscape::endLoading() {
    cocos2d::utils::findChild(this, "button")->setVisible(true);
    this->stopAllActions();
}


void StoryParentCoupon_Landscape::onValidateCouponSuccess(const std::string & coupon, const float & discount_percent) {
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    auto background = cocos2d::ui::Helper::seekWidgetByName(this, "coupon_bg");
    cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_coupon")->setVisible(true);
    background->setContentSize(cocos2d::Size(background->getContentSize().width, 350));

    auto text_license = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "text_coupon"));
    text_license->setString(mj::Pay::getInstance().getCoupon());
    auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "parent.coupon.discount"));
    text_details->setString(cocos2d::StringUtils::format("%s: %d%%", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.coupon.discount").c_str(), mj::Pay::getInstance().getDiscountPercent()));
    
    cocos2d::ui::Helper::doLayout(this);

    textfield_license->setString("");
    endLoading();

    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_coupon_change);

    StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.coupon.success"), "OK", "");
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    textfield_license->setAttachWithIME(false);
    textfield_license->setDetachWithIME(true);
    
    background->stopAllActions();
    background->runAction(MoveTo::create(0.25f, pos_bottom));
}

void StoryParentCoupon_Landscape::onValidateCouponFailed(const std::string & coupon, const std::string & error) {
    endLoading();
    if (error == "") {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error"), "OK", "");
    } else {
        StoryAlert::showAlert(error, "OK", "");
    }
}

void StoryParentCoupon_Landscape::onTextfieldChanged(cocos2d::Ref *pSender, int event){
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS
    return;
#endif
    auto textfield_event = (TextField::EventType) event;
    switch (textfield_event)
    {
        case TextField::EventType::ATTACH_WITH_IME:
        {
            CCLOG("Lên");
            background->stopAllActions();
            background->runAction(MoveTo::create(0.25f, pos_top));
            
        }
            break;
            
        case TextField::EventType::DETACH_WITH_IME:
        {
            CCLOG("Xuống");
            background->stopAllActions();
            background->runAction(MoveTo::create(0.25f, pos_bottom));
        }
            break;
            
        case TextField::EventType::INSERT_TEXT:
            break;
        case TextField::EventType::DELETE_BACKWARD:{
        }
            break;

    }
}

Widget::ccWidgetEventCallback StoryParentCoupon_Landscape::onLocateEventCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetEventCallback> function_map = {
        {"onTextfieldChanged", CC_CALLBACK_2(StoryParentCoupon_Landscape::onTextfieldChanged, this)}
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
