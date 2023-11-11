//
//  MSLicenseOldNotice.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/31/18.
//

#include "MSLicenseOldNotice.h"
#include "StoryLanguageManager.h"
#include "MJAPI_ChangeLicense.h"
#include "MSEventLogger.h"

USING_NS_CC;
INITIALIZE_READER(MSLicenseOldNotice);

std::once_flag ms_license_old_notice_reader_flag;

MSLicenseOldNotice * MSLicenseOldNotice::createView(std::string mail, std::string phone, std::string license) {
    std::call_once(ms_license_old_notice_reader_flag, [] {
        REGISTER_CSB_READER(MSLicenseOldNotice);
    });
    
    auto csb = "csb/new_ui/LicenseOldNotice.csb";
    if (LANGUAGE_MANAGER->getDisplayLangId() == 1) {
        csb = "csb/new_ui/LicenseOldNotice_en.csb";
    }
    
    auto view = reinterpret_cast<MSLicenseOldNotice *>(CSLoader::createNodeWithVisibleSize(csb));
    if (view) {
        view->didLoadFromCSB(mail, phone, license);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}

#include "MJHelper.h"
void MSLicenseOldNotice::didLoadFromCSB(std::string mail, std::string phone, std::string license) {
    _api = nullptr;
    _license = license;
    auto text = utils::findChild<ui::Text *>(this, "text");
    auto str = text->getString();
    str = mj::helper::replace_string(str, "xxxxx", license);
    text->setString(str);
    
    auto via_mail = utils::findChild<ui::Text *>(this, "via_mail");
    auto via_phone = utils::findChild<ui::Text *>(this, "via_phone");
    
    via_mail->setString(via_mail->getString() + " " + mail);
    via_phone->setString(via_phone->getString() + " " + phone);
    
    if (mail.empty()) {
        via_mail->getParent()->removeFromParent();
        via_phone->getParent()->setPosition(via_phone->getParent()->getPositionX(), 80);
    } else if (phone.empty()) {
        via_phone->getParent()->removeFromParent();
        via_mail->getParent()->setPosition(via_mail->getParent()->getPositionX(), 80);
    }
}

void MSLicenseOldNotice::onEnter() {
    ui::Layout::onEnter();
}

void MSLicenseOldNotice::onClose(cocos2d::Ref *sender) {
    if (_api) {
        _api->setCallback(nullptr);
    }
    this->removeFromParent();
}

#include "StoryAlert.h"
void MSLicenseOldNotice::onMail(cocos2d::Ref *sender) {
    showWait();
    
    ms::EventLogger::getInstance().logEvent("request_new_license_via_mail", {});
    
    _api = mj::api::ChangeLicense::call(mj::api::ChangeLicense::Type::MAIL, _license, [=](bool success, std::string msg) {
        _api = nullptr;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg, success] {
            this->hideWait();
            StoryAlert::showAlert(msg, "OK", "");
            if (success) {
                auto via_phone = utils::findChild(this, "via_phone");
                if (via_phone) {
                    auto bt = reinterpret_cast<ui::Button *>(via_phone->getParent());
                    if (bt) {
                        bt->setBright(false);
                        bt->setEnabled(false);
                    }
                }
                
                auto via_mail = utils::findChild(this, "via_mail");
                if (via_mail) {
                    auto bt = reinterpret_cast<ui::Button *>(via_mail->getParent());
                    if (bt) {
                        bt->setBright(false);
                        bt->setEnabled(false);
                    }
                }
            }
        });
    });
}

void MSLicenseOldNotice::onSMS(cocos2d::Ref * sender) {
    showWait();
    
    ms::EventLogger::getInstance().logEvent("request_new_license_via_phone", {});

    _api = mj::api::ChangeLicense::call(mj::api::ChangeLicense::Type::PHONE, _license, [=](bool success, std::string msg) {
        _api = nullptr;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg, success] {
            this->hideWait();
            StoryAlert::showAlert(msg, "OK", "");
            if (success) {
                auto via_phone = utils::findChild(this, "via_phone");
                if (via_phone) {
                    auto bt = reinterpret_cast<ui::Button *>(via_phone->getParent());
                    if (bt) {
                        bt->setBright(false);
                        bt->setEnabled(false);
                    }
                }
                
                auto via_mail = utils::findChild(this, "via_mail");
                if (via_mail) {
                    auto bt = reinterpret_cast<ui::Button *>(via_mail->getParent());
                    if (bt) {
                        bt->setBright(false);
                        bt->setEnabled(false);
                    }
                }
            }
        });
    });
}

#include "MJHelper.h"
void MSLicenseOldNotice::showWait() {
    auto bg = utils::findChild(this, "bg");
    auto children = bg->getChildren();
    for (auto c : children) {
        if (c->getName() != "button_close")
            c->runAction(FadeOut::create(0.5));
    }
    
    auto lb = ui::Text::create(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.wait"), "Montserrat-Bold.ttf", 35);
    lb->setTextColor(Color4B(mj::helper::colorFromHex(0x34C5ED)));
    lb->setPosition(bg->getContentSize() / 2);
    lb->setName("wait");
    bg->addChild(lb);
}

void MSLicenseOldNotice::hideWait() {
    auto bg = utils::findChild(this, "bg");
    bg->removeChildByName("wait");
    auto children = bg->getChildren();
    for (auto c : children) {
        if (c->getName() != "button_close")
            c->runAction(FadeIn::create(0.5));
    }
}

ui::Widget::ccWidgetClickCallback MSLicenseOldNotice::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSLicenseOldNotice, onClose),
        CLICK_MAP(MSLicenseOldNotice, onMail),
        CLICK_MAP(MSLicenseOldNotice, onSMS)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
