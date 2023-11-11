//
//  MSForgotLicensePopup.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#include "MSForgotLicensePopup.h"
#include "StoryAlert.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(MSForgotLicensePopup);

std::once_flag mm_forgot_license_popup;

MSForgotLicensePopup * MSForgotLicensePopup::createView() {
    std::call_once(mm_forgot_license_popup, [] {
        REGISTER_CSB_READER(MSForgotLicensePopup);
    });
    
    auto view = reinterpret_cast<MSForgotLicensePopup *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/ForgotLicense.csb"));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

void MSForgotLicensePopup::didLoadFromCSB() {
	auto panel_warning = utils::findChild(this, "panel_warning");
	panel_warning->setVisible(false);
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    textfield->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textfield->setCursorChar('|');
    textfield->setCursorEnabled(true);
    textfield->setMaxLength(50);
    textfield->setMaxLengthEnabled(true);
    
    utils::findChild<ui::Text *>(this, "forgot.license.ask")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.ask"));
    utils::findChild<ui::Text *>(this, "forgot.license.action")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.action"));
    

    textfield->addEventListener([=](Ref * sender, ui::TextField::EventType event) {
		utils::findChild(this, "panel_warning")->setVisible(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        auto bg = utils::findChild(this, "bg");
        switch (event) {
            case ui::TextField::EventType::ATTACH_WITH_IME:
                bg->setPosition(Point(Director::getInstance()->getVisibleSize() / 2) + Point(0, 100));
                break;
                
            case ui::TextField::EventType::DETACH_WITH_IME:
                bg->setPosition(Director::getInstance()->getVisibleSize() / 2);
                break;
                
            default:
                break;
        }
#endif
    });

}

void MSForgotLicensePopup::onEnter() {
    ui::Layout::onEnter();
}

#include "MJAPI_ForgotLicense.h"
void MSForgotLicensePopup::onOK(cocos2d::Ref *sender) {
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    auto bg = utils::findChild(this, "bg");
    bg->setPosition(Director::getInstance()->getVisibleSize() / 2);

	auto panel_warning = utils::findChild(this, "panel_warning");
	panel_warning->setVisible(false);

    auto text = utils::findChild<ui::TextField *>(this, "textfield")->getString();
    if (text.empty()) {
		panel_warning->setVisible(true);
		utils::findChild<ui::Text*>(panel_warning, "txt_warning")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"));
        //StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"), LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"), "", false, 24);
        return;
    }
    
    if (!mj::helper::isValidEmail(text) && !mj::helper::isValidPhoneNumber(text)) {
		panel_warning->setVisible(true);
		utils::findChild<ui::Text*>(panel_warning, "txt_warning")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"));
        //StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"), LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"), "", false, 24);
        return;
    }
    
    startLoading();
    
    _request = mj::api::ForgotLicense::call(text, [=](bool success, std::string message) {
        _request = nullptr;
        this->stopLoading();
        if (success) {
            if (message.empty()) {
                message = LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error");
            }
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, message] {
                Director::getInstance()->getRunningScene()->removeChildByName("pleasewait");
                StoryAlert::showAlert(message, "OK", "", false, 30);
                this->onClose(nullptr);
            });
        } else {
            if (message.empty()) {
                message = LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error");
            }
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {
                Director::getInstance()->getRunningScene()->removeChildByName("pleasewait");
                StoryAlert::showAlert(message, "OK", "", false, 30);
            });
        }
    });
}

void MSForgotLicensePopup::startLoading() {
    cocos2d::utils::findChild(this, "button")->setVisible(false);
    auto timeline = CSLoader::createTimeline("csb/new_ui/ForgotLicense.csb");
    timeline->play("preloader", true);
    this->runAction(timeline);
}

void MSForgotLicensePopup::stopLoading() {
    cocos2d::utils::findChild(this, "button")->setVisible(true);
    this->stopAllActions();
}

void MSForgotLicensePopup::onClose(cocos2d::Ref *sender) {
    if (_request) {
        _request->setCallback(nullptr);
    }
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MSForgotLicensePopup::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSForgotLicensePopup, onClose),
        CLICK_MAP(MSForgotLicensePopup, onOK),
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
