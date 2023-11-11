//
//  MSIntroLicense.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/28/18.
//

#include "MSIntroLicense.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "StoryDataModel.h"
#include "MSHome.h"
#include "StoryConfigManager.h"
#include "StoryWelcomePopUp.h"
#include "MSIntroGrade.h"
#include "MSLicenseLimitPopup.h"
#include "MSLicenseOldNotice.h"
#include "LicenseInfoModel.h"
#include "MJPlatformConfig.h"
#include "MJAPI_RemoveLicense.h"
#include "MJListDeviceView.h"
#include "MSEventLogger.h"
#include "APPhoneOTP.h"
#include "APPhoneLogin.h"
#include "APActiveLicenseAPI.h"

USING_NS_CC;
INITIALIZE_READER(MSIntroLicense);

std::once_flag ms_intro_license_reader_flag;

MSIntroLicense* MSIntroLicense::showPopup() {
    std::call_once(ms_intro_license_reader_flag, [] {
        REGISTER_CSB_READER(MSIntroLicense);
    });
    auto view = static_cast<MSIntroLicense *>(CSLoader::createNodeWithVisibleSize("csb/StoryPopupInfo.csb"));
    if (view) {
        view->didLoadFromCSB();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

MSIntroLicense * MSIntroLicense::createView() {
    std::call_once(ms_intro_license_reader_flag, [] {
        REGISTER_CSB_READER(MSIntroLicense);
    });
    auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Intro_License.csb");
    auto view = reinterpret_cast<MSIntroLicense *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (view) {
        view->didLoadFromCSB();
    } else {
        CC_SAFE_DELETE(view);
    }
    
    return view;
}

void MSIntroLicense::didLoadFromCSB() {
    hideWarning();
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setMaxLengthEnabled(true);
    textfield->setMaxLength(11);
    textfield->setCursorChar('|');
    textfield->setCursorEnabled(true);
    textfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("sidebar.des.license"));
    textfield->setPlaceHolderColor(Color4B(0xff, 0xff, 0xff, 0xaa));
    textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
    textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
    textfield->addEventListener([=](Ref * sender, ui::TextField::EventType event) {
        hideWarning();
        switch (event) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            case ui::TextField::EventType::ATTACH_WITH_IME:
                this->runAction(EaseQuadraticActionOut::create(MoveTo::create(0.25, Point(this->getPositionX(), 200))));
                break;
                
            case ui::TextField::EventType::DETACH_WITH_IME:
                this->runAction(EaseQuadraticActionOut::create(MoveTo::create(0.25, Point(this->getPositionX(), 0))));
                break;
#endif
                
            case ui::TextField::EventType::INSERT_TEXT: {
                auto str = textfield->getString();
                auto utf8 = StringUtils::StringUTF8(str);
                if (utf8.length() == 6 || utf8.length() == 12) {
                    if (utf8.getString().at(utf8.length() - 1)._char != "-")
                        utf8.getString().insert(utf8.getString().begin() + utf8.length() - 1, {"-"});
                }
                textfield->setString(mj::helper::to_upper(utf8.getAsCharSequence()));
                break;
            }
                
            case ui::TextField::EventType::DELETE_BACKWARD: {
                auto str = textfield->getString();
                auto utf8 = StringUtils::StringUTF8(str);
                if (utf8.length() == 6 || utf8.length() == 12) {
                    utf8.getString().erase(utf8.getString().begin() + utf8.length() - 1);
                }
                textfield->setString(mj::helper::to_upper(utf8.getAsCharSequence()));
                break;
            }
                
            default:
                break;
        }
    });
    
    if(_timeline.empty()){
        utils::findChild<ui::Button*>(this, "btcancel")->setVisible(false);
        utils::findChild<ui::Text *>(this, "parent.license.ask")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.ask"));
        utils::findChild<ui::Text *>(this, "text_forgot_license")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("settings.forgot.license"));
        
        _timeline = "csb/StoryPopupInfo.csb";
        auto timeline = CSLoader::createTimeline(_timeline);
        this->runAction(timeline);
        timeline->play("show", false);
        isRunning = true;
        timeline->setLastFrameCallFunc([=] {
            isRunning = false;
        });
    }
    
}

void MSIntroLicense::showWarning(const std::string & message){
    utils::findChild(this, "img_warning")->setVisible(true);
    utils::findChild(this, "txt_warning")->setVisible(true);
    utils::findChild<ui::Text*>(this, "txt_warning")->setString(message);
}

void MSIntroLicense::hideWarning(){
    utils::findChild(this, "img_warning")->setVisible(false);
    utils::findChild(this, "txt_warning")->setVisible(false);
}

void MSIntroLicense::onTrigger(cocos2d::Ref *sender) {
}

void MSIntroLicense::onSkip(cocos2d::Ref * sender) {
	MS_LOGGER.logEventFirstLicenseSkip();
//    CCLOG("button: %s", ((Button*)sender)->getName().c_str());
	closePopup();
	if (_license_callback)
	{
		_license_callback(false);
	}
}

#include "MSForgotLicensePopup.h"
void MSIntroLicense::onForgot(cocos2d::Ref *sender) {
    MS_LOGGER.logEventFirstLicenseForgot();
    auto popup = MSForgotLicensePopup::createView();
	auto license_callback = std::move(_license_callback);
	popup->setOnExitCallback([license_callback] {
		auto intro_license = MSIntroLicense::showPopup();
		intro_license->setOnLicenseCallback(license_callback);
		Director::getInstance()->getRunningScene()->addChild(intro_license, INT_MAX - 1);
	});
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX - 1);
	closePopup();
}

void MSIntroLicense::onCancel(cocos2d::Ref *sender){
	closePopup();
	if (_license_callback)
	{
		_license_callback(false);
	}
}

void MSIntroLicense::closePopup()
{
	auto timeline = CSLoader::createTimeline("csb/StoryPopupInfo.csb");
	this->runAction(timeline);
	timeline->play("hide", false);
	isRunning = true;
	timeline->setLastFrameCallFunc([=] {
		this->removeFromParent();
	});
}

void MSIntroLicense::onSend(Ref * sender) {
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    auto license = std::string(textfield->getString());
    _entered_license = license;
    if (license.empty()) {
        //StoryAlert::showAlert("Vui lòng nhập mã kích hoạt", "OK", "OK");
        showWarning(LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.empty"));
        return;
    }
	if (license.size() < 10)
	{
		showWarning(LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.note"));
		return;
	}
    auto utf = StringUtils::StringUTF8(license);
    
    MS_LOGGER.logEventFirstLicenseSubmit();
    mj::Pay::getInstance().setActiveLicenseDelegate(this);
    mj::Pay::getInstance().enterLicense(license);
//    ap::api::ActiveLicense::call(license, nullptr);
    
    
    auto load_action = Sequence::create(CallFuncN::create([](Node * n) {
        reinterpret_cast<ui::Text *>(n)->setString(".");
    }), DelayTime::create(0.25), CallFuncN::create([](Node * n) {
        reinterpret_cast<ui::Text *>(n)->setString(". .");
    }), DelayTime::create(0.25), CallFuncN::create([](Node * n) {
        reinterpret_cast<ui::Text *>(n)->setString(". . .");
    }), DelayTime::create(0.25), NULL);
    
    utils::findChild(this, "label_send")->runAction(RepeatForever::create(load_action));
    if (sender) reinterpret_cast<ui::Button *>(sender)->setTouchEnabled(false);
    CCLOG("touch disable");
}

#include "MJLicenseActiveSuccess.h"
#include "Key_Default_Model.h"
void MSIntroLicense::onActiveLicenseSuccess(const std::string & license, const int & time_expire, const std::string & message, rapidjson::Value data) {
    MS_LOGGER.logEventFirstLicenseSuccess();
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, time_expire, message] {
		closePopup();
        MJDEFAULT->setBoolForKey(key_onboarding_license, true);
		if (is_unverified_account) {
			auto view = MJLicenseActiveSuccess::createView(message, [] {
				MJDEFAULT->setBoolForKey(key_first_api_called, true);
				Director::getInstance()->replaceScene(MSIntroGrade::createScene());
			});
			Director::getInstance()->getRunningScene()->addChild(view, INT_MAX);
		}
		else {
			auto view = MJLicenseActiveSuccess::createView(message, [this] {
				if (_license_callback)
				{
					_license_callback(true);
				}
				});
			Director::getInstance()->getRunningScene()->addChild(view, INT_MAX);
		}
    }); 
}


void MSIntroLicense::onActiveLicenseFailed(const std::string & license, const int & error_code, const std::string & error, rapidjson::Value data) {
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, license, error, error_code, data] {
    MS_LOGGER.logEventFirstLicenseFailed();
    auto bt = utils::findChild<ui::Text *>(this, "label_send");
    bt->stopAllActions();
    bt->setString(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.action"));

    //auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    //textfield->setString("");
    
    utils::findChild<ui::Button *>(this, "button_started")->runAction(Sequence::create(DelayTime::create(0.25), CallFuncN::create([](Node * n) {
        reinterpret_cast<ui::Button *>(n)->setTouchEnabled(true);
    }), NULL));
	if (error_code == 302) {// trương hợp account chưa được verify
		auto phone_verify = APPhoneLogin::createView(true);
		phone_verify->setAnchorPoint(Vec2(0.5, 0.5));
		phone_verify->setPosition(this->getContentSize() / 2);
		phone_verify->setZOrder(100000);
		phone_verify->is_onboard = true;
		this->addChild(phone_verify);
		return;
	}
    if (error_code == 210) {
        //trường hợp kích hoạt 2 thiết bị

        if (data.HasMember("phone") && data["phone"].IsString()) {
            _phone_info = data["phone"].GetString();
        }
        
        if (data.HasMember("email") && data["email"].IsString()) {
            _mail_info = data["email"].GetString();
        }
        LICENSE_INFO_MODEL->setInfoVerify(_phone_info, _mail_info);
        
        if (data.HasMember("device_info") && data["device_info"].IsArray()) {
            auto list_device = data["device_info"].GetArray();
            _list_device_info.clear();
            for (int i = 0; i < (int) list_device.Size(); i++) {
                auto device_data = list_device[i].GetObject();
                license::LicenseDeviceInfo device_info;
                if (device_data.HasMember("id") && device_data["id"].IsString()) {
                    device_info.device_id = atoi(device_data["id"].GetString());
                }
                
                if (device_data.HasMember("time_active") && device_data["time_active"].IsString()) {
                    device_info.active_date = atoi(device_data["time_active"].GetString());
                }
                
                if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                    device_info.device_name = device_data["device_info"].GetString();
                }
                
                device_info.device_status = 1;
                _list_device_info.push_back(device_info);
            }
            
            //                    LICENSE_INFO_MODEL->updateLicenseDeviceInfo(list_device_info);
        }
        
        this->showGuideRemoveLicense();
        return;
    }

    if (error_code == 420){//đã kích hoạt đủ 2 thiết bị đồng thời đã xóa hết quyền được xóa.
        this->updateDeviceInfoLicense(data.GetObject());
        this->showOverLicenseView(error);
        return;
    }

    if (error_code == 411){//Nhập mã MS vào MJ => báo lỗi => hiện ra thông báo mở app MS hoặc Download app MS
        //
        if (data.HasMember("app_id") && data["app_id"].IsString()) {
            auto app_id = atoi(data["app_id"].GetString());
            std::string str_appname;
            if (app_id == 2) {
                str_appname = "Monkey Junior";
                auto is_stories_installed = mj::PlatformConfig::getInstance().canOpenLearnToRead();
                int tag;
                std::string str_btok;
                auto msg = error;
                if (CONFIG_MANAGER->isCountryVN()) {
                    if (is_stories_installed) {
                        msg += StringUtils::format("\nVui lòng mở ứng dụng %s lên để nhập mã kích hoạt.", str_appname.c_str());
                        tag = 1003;
                        str_btok = "Mở";
                    }else {
                        tag = 1004;
                        msg += StringUtils::format("\nVui lòng cài đặt ứng dụng %s để nhập mã kích hoạt.", str_appname.c_str());
                        str_btok = "Cài đặt";
                    }
                }else {
                    if (is_stories_installed) {
                        msg += StringUtils::format("\nPlease open %s and enter the key there.", str_appname.c_str());
                        str_btok = "Open";
                        tag = 1003;
                    }else {
                        tag = 1004;
                        msg += StringUtils::format("nPlease install %s and enter the key there.", str_appname.c_str());
                        str_btok = "Install";
                    }
                }
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, str_btok, this, tag] {
                    StoryAlert::showAlert(msg, str_btok, "", false, 30, this, tag);
                });
            }
        }
    }
    
    if (error_code == 430) {
        auto split = mj::helper::split(error, '|');
        std::string mail = "", phone = "";
        if (!split.empty()) {
            mail = split[0];
            if (split.size() > 1) {
                phone = split[1];
            }
        }
        
        if (mail.empty() && phone.empty()) {
            
        } else {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([license, mail, phone] {
                auto popup = MSLicenseOldNotice::createView(mail, phone, license);
                Director::getInstance()->getRunningScene()->addChild(popup, std::numeric_limits<int>::max() - 1);
            });
            return;
        }
    }
    
    if (error == "") {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([license] {
            StoryAlert::showAlert(StringUtils::format("%s\n[%s]", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str(), license.c_str()), "OK", "");
        });
    } else {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([error] {
            StoryAlert::showAlert(error, "OK", "");
        });
    }
//    });
    
}

ui::Widget::ccWidgetClickCallback MSIntroLicense::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSIntroLicense, onTrigger),
        CLICK_MAP(MSIntroLicense, onSkip),
        CLICK_MAP(MSIntroLicense, onSend),
        CLICK_MAP(MSIntroLicense, onCancel),
        CLICK_MAP(MSIntroLicense, onForgot)
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void MSIntroLicense::onAlertOK(cocos2d::Ref *sender) {
    auto tag = reinterpret_cast<Node *>(sender)->getTag();
    if (tag == 1001) {
        StoryAlert::showAlert2Button(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.question.2"),
                                     LANGUAGE_MANAGER->getDisplayTextByKey("deactive.yes"),
                                     LANGUAGE_MANAGER->getDisplayTextByKey("deactive.no"), false, 30, this, 1002);
        return;
    }
    
    if (tag == 1002) {
        auto layout = ui::Layout::create();
        layout->setContentSize(Director::getInstance()->getVisibleSize());
        layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B::BLACK);
        layout->setBackGroundColorOpacity(0xdd);
        layout->setName("pleasewait");
        Director::getInstance()->getRunningScene()->addChild(layout, 10);
        
        auto text = ui::Text::create(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.wait"), "Montserrat-Bold.ttf", 40);
        text->setTextColor(Color4B::WHITE);
        text->setPosition(Director::getInstance()->getVisibleSize() / 2);
        layout->addChild(text);
        
        mj::api::RemoveLicense::call([](bool success, std::string message) {
            if (success) {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {
                    MJDEFAULT->setBoolForKey(key_first_api_called, true);
                    Director::getInstance()->replaceScene(MSIntroGrade::createScene());
                });
            } else {
                if (message.empty()) {
                    message = LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error");
                }
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {
                    Director::getInstance()->getRunningScene()->removeChildByName("pleasewait");
                    StoryAlert::showAlert(message, "OK", "", false, 30);
                });
                //                alert->setDelegate(this);
            }
        });
        return;
    }
    
    if (tag == 1003) {
        mj::PlatformConfig::getInstance().openLearnToRead();
        return;
    }
    
    if (tag == 1004) {
        mj::PlatformConfig::getInstance().showLearnToReadStorePage();
        return;
    }
    Director::getInstance()->replaceScene(MSIntroGrade::createScene());
}

void MSIntroLicense::onAlertCancel(cocos2d::Ref *sender) {
}

void MSIntroLicense::updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success){
    auto data = response_data.GetObject();
    if (data.HasMember("phone") && data["phone"].IsString()) {
        _phone_info = data["phone"].GetString();
    }
    
    if (data.HasMember("email") && data["email"].IsString()) {
        _mail_info = data["email"].GetString();
    }
    if (data.HasMember("total_active") && data["total_active"].IsString()) {
        _total_active = data["total_active"].GetString();
    }
    
    LICENSE_INFO_MODEL->setInfoVerify(_phone_info, _mail_info);
    
    if (data.HasMember("device_in_licence") && data["device_in_licence"].IsObject()) {
        auto device_in_licence = data["device_in_licence"].GetObject();
        _list_device_info.clear();
        if (device_in_licence.HasMember("list_active") && device_in_licence["list_active"].IsArray()) {
            auto list_active = device_in_licence["list_active"].GetArray();
            for (int i = 0; i < (int) list_active.Size(); i++) {
                auto device_data = list_active[i].GetObject();
                license::LicenseDeviceInfo device_info;
                if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                    device_info.device_id = atoi(device_data["device_id"].GetString());
                }
                if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                    device_info.active_date = atoi(device_data["active_date"].GetString());
                }
                if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                    device_info.device_name = device_data["device_info"].GetString();
                }
                device_info.device_status = 1;
                _list_device_info.push_back(device_info);
            }
        }
        
        if (device_in_licence.HasMember("list_remove") && device_in_licence["list_remove"].IsArray()) {
            auto list_remove = device_in_licence["list_remove"].GetArray();
            for (int i = 0; i < (int) list_remove.Size(); i++) {
                auto device_data = list_remove[i].GetObject();
                license::LicenseDeviceInfo device_info;
                if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                    device_info.device_id = atoi(device_data["device_id"].GetString());
                }
                
                if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                    device_info.active_date = atoi(device_data["active_date"].GetString());
                }
                
                if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                    device_info.device_name = device_data["device_info"].GetString();
                }
                
                if (device_data.HasMember("time_remove") && device_data["time_remove"].IsString()) {
                    device_info.deactive_date = atoi(device_data["time_remove"].GetString());
                }
                device_info.device_status = 0;
                _list_device_info.push_back(device_info);
            }
        }
        
        if (is_active_success){
            LICENSE_INFO_MODEL->updateLicenseDeviceInfo(_list_device_info);
            MJDEFAULT->setStringForKey("phone_info", _phone_info);
            MJDEFAULT->setStringForKey("mail_info", _mail_info);
            MJDEFAULT->setStringForKey("total_active", _total_active);
        }
    }
}

void MSIntroLicense::showGuideRemoveLicense(){
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        license::Package lic;
        lic.license = _entered_license;
        auto full_license_view = MJFullLicenseView::createView(lic);
        Director::getInstance()->getRunningScene()->addChild(full_license_view, INT_MAX - 1);
        full_license_view->setDelegate(this);
    });
}

void MSIntroLicense::onFullLicenseViewFreeUse(){
    LICENSE_INFO_MODEL->onFreeUse();
    onSkip(nullptr);
    
//    Node * n = this;
//    while (!dynamic_cast<StoryParentPage_Landscape *>(n->getParent())) {
//        n = n->getParent();
//    }
//
//    reinterpret_cast<StoryParentPage_Landscape *>(n->getParent())->onBack(nullptr);
}

void MSIntroLicense::onFullLicenseonSubscribe() {
    //    Director::getInstance()->getScheduler()->schedule([=](float) {
    //        LICENSE_INFO_MODEL->onSubscribe();
    //    }, this, 0, 0, 1, false, "delay_pop_subscribe_over_license");
    MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, true);
    onSkip(nullptr);
}

void MSIntroLicense::onFullLicenseonRemoveDevice(license::Package lic){
    auto current_device = LICENSE_INFO_MODEL->getDeviceID();
    auto total_device = MJDEFAULT->getStringForKey("total_device");
    auto list_device_view = MJListDeviceView::createView(_list_device_info, current_device, lic, total_device, true);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
    list_device_view->onShow();
//    this->removeFromParent();
}

void MSIntroLicense::showOverLicenseView(std::string message){
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, message] {
        auto over_license_view = MJOverLicenseView::createView(message);
        Director::getInstance()->getRunningScene()->addChild(over_license_view, INT_MAX - 1);
        over_license_view->setDelegate(this);
    });
}

void MSIntroLicense::onOverLicenseViewFreeUse() {
    LICENSE_INFO_MODEL->onFreeUse();
    onSkip(nullptr);
    
//    Node * n = this;
//    while (!dynamic_cast<StoryParentPage_Landscape *>(n->getParent())) {
//        n = n->getParent();
//    }
//
//    reinterpret_cast<StoryParentPage_Landscape *>(n->getParent())->onBack(nullptr);
}

void MSIntroLicense::onOverLicenseViewSubscribe() {
//    Director::getInstance()->getScheduler()->schedule([=](float) {
//        LICENSE_INFO_MODEL->onSubscribe();
//    }, this, 0, 0, 1, false, "delay_pop_subscribe_over_license");
    MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, true);
    onSkip(nullptr);
}

void MSIntroLicense::onOverLicenseViewListDevice() {
    auto current_device = LICENSE_INFO_MODEL->getDeviceID();
    auto total_device = MJDEFAULT->getStringForKey("total_device");
    license::Package lic;
    lic.license = _entered_license;
    auto list_device_view = MJListDeviceView::createView(_list_device_info, current_device, lic, total_device, true, false);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
    list_device_view->onShow();
//    this->removeFromParent();
}

