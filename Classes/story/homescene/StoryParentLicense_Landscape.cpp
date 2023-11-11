//
//  StoryParentLicense_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/12/17.
//
//

#include "StoryParentLicense_Landscape.h"
#include "HSLib.h"
#include "StoryAlert.h"
#include "MJPlatformConfig.h"
#include "NativeBridge.h"
#include "MSIntroGrade.h"
#include "MJHelper.h"
#include "HSBaseScene.h"
#include "MSLicenseLimitPopup.h"
#include "MSLicenseOldNotice.h"
#include "MSForgotLicensePopup.h"
#include "MJAPI_RemoveLicense.h"
#include "MSLicenseOldNotice.h"
#include "MJListDeviceView.h"
#include "APPhoneOTP.h"
#include "APPhoneLogin.h"
#include "MSIntro.h"

INITIALIZE_READER_INSTANCE(StoryParentLicense_LandscapeReader);

StoryParentLicense_Landscape * StoryParentLicense_Landscape::createPage() {
	StoryParentLicense_Landscape *p;
	if (CONFIG_MANAGER->isHomeScreenLandscape()) {
		p = static_cast<StoryParentLicense_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentLicense_Landscape.csb"));
	}
	else p = static_cast<StoryParentLicense_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentLicense.csb"));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

#include "StoryDataModel.h"

void StoryParentLicense_Landscape::didLoadFromCSB() {
	StoryParentSubPage_Landscape::didLoadFromCSB();
	is_vn = LANGUAGE_MANAGER->getDisplayLangId();
	if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
		this->setContentSize(DIRECTOR->getVisibleSize());
		ui::Helper::doLayout(this);
	}

	auto p = StoryDataModel::getInstance()->getLicensePackages();

	auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

	utils::findChild<ui::Text *>(this, "parent.license.info")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.info"));
	utils::findChild<ui::Text *>(this, "parent.license.expire")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire"));
	utils::findChild<ui::Text *>(this, "parent.license.ask")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.ask"));
	utils::findChild<ui::Text *>(this, "parent.license.note")->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.note"));

	auto textfield_license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_licensekey"));
	textfield_license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_licensekey"));
	textfield_license->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("sidebar.des.license"));
	textfield_license->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	textfield_license->setTextVerticalAlignment(TextVAlignment::CENTER);
	textfield_license->setCursorEnabled(true);
	textfield_license->setMaxLength(11);
	textfield_license->setMaxLengthEnabled(true);
	textfield_license->addEventListener([=](Ref * sender, ui::TextField::EventType event) {
		auto bg = utils::findChild(this, "license_bg");
		switch (event) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		case ui::TextField::EventType::ATTACH_WITH_IME:
			bg->runAction(EaseQuadraticActionOut::create(MoveTo::create(0.25, Point(bg->getPositionX(), 500))));
			break;

		case ui::TextField::EventType::DETACH_WITH_IME:
			bg->runAction(EaseQuadraticActionOut::create(MoveTo::create(0.25, Point(bg->getPositionX(), 384))));
			break;
#endif
                
            case ui::TextField::EventType::INSERT_TEXT: {
                auto str = textfield_license->getString();
                auto utf8 = StringUtils::StringUTF8(str);
                if (utf8.length() == 6 || utf8.length() == 12) {
                    if (utf8.getString().at(utf8.length() - 1)._char != "-")
                        utf8.getString().insert(utf8.getString().begin() + utf8.length() - 1, {"-"});
                }
                textfield_license->setString(mj::helper::to_upper(utf8.getAsCharSequence()));
                break;
            }
                
            case ui::TextField::EventType::DELETE_BACKWARD: {
                auto str = textfield_license->getString();
                auto utf8 = StringUtils::StringUTF8(str);
                if (utf8.length() == 6 || utf8.length() == 12) {
                    utf8.getString().erase(utf8.getString().begin() + utf8.length() - 1);
                }
                textfield_license->setString(mj::helper::to_upper(utf8.getAsCharSequence()));
                break;
            }
                
            default:
                break;
        }
    });
    
    utils::findChild<ui::Text *>(this, "text_forgot_license")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("settings.forgot.license"));
    utils::findChild<ui::Text *>(this, "text_deactive_device")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("settings.deactivate.device"));
//    utils::findChild<ui::Text *>(this, "text_subscribe")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("settings.buy.license"));
    utils::findChild<ui::Text *>(this, "text_subscribe")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("settings.subscribe.license"));

}

void StoryParentLicense_Landscape::changeDisplayLanguage() {
	if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
		static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_portrait"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "license_title_portrait"));
	}
}

void StoryParentLicense_Landscape::onEnter() {
	StoryParentSubPage_Landscape::onEnter();
	changeDisplayLanguage();
	auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

	auto background = static_cast<ImageView *>(cocos2d::ui::Helper::seekWidgetByName(this, "license_bg"));

	cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(false);
	utils::findChild(this, "button_deactivate")->setVisible(false);
	background->setContentSize(cocos2d::Size(background->getContentSize().width, 275));
	cocos2d::ui::Helper::doLayout(this);

	//if (mj::Pay::getInstance().isActive())
	//{
	//	cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(true);
	//	background->setContentSize(cocos2d::Size(background->getContentSize().width, 425));
	//	auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "parent.license.expire"));
	//	auto expire_time = mj::Pay::getInstance().getTimeExpire();
	//	if (expire_time == -1) {
	//		text_details->setString(cocos2d::StringUtils::format("%s: %s",
	//			LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(),
	//			LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()
	//		));
	//	}
	//	else {

	//		auto expire_date = LTDate(expire_time);
	//		text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));
	//	}
	//}
	//else
	//{
	//	cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(false);
	//	utils::findChild(this, "button_deactivate")->setVisible(false);
	//	background->setContentSize(cocos2d::Size(background->getContentSize().width, 275));
	//	cocos2d::ui::Helper::doLayout(this);
	//}
	cocos2d::ui::Helper::doLayout(this);

	////    if(!MJDEFAULT->getBoolForKey(key_Actived, false)){
	//    if (mj::Pay::getInstance().getLicense().empty()) {
	////    if (MJDEFAULT->getStringForKey(k_MONKEY_JUNIOR_LICENSE).empty()) {
	//        cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(false);
	//        utils::findChild(this, "button_deactivate")->setVisible(false);
	//        background->setContentSize(cocos2d::Size(background->getContentSize().width, 275));
	//        cocos2d::ui::Helper::doLayout(this);
	//    } else {
	//        cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(true);
	//        background->setContentSize(cocos2d::Size(background->getContentSize().width, 425));
	//
	////        auto text_license = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "text_license"));
	////        text_license->setString(mj::Pay::getInstance().getLicense());
	//       
	//		auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "parent.license.expire"));
	//		auto expire_time = mj::Pay::getInstance().getTimeExpire();
	//        if (expire_time == -1) {
	//            text_details->setString(cocos2d::StringUtils::format("%s: %s",
	//                    LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(),
	//                    LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()
	//            ));
	//        } else {
	//            auto expire_date = LTDate(expire_time);
	//            text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));
	//        }
	//
	//        cocos2d::ui::Helper::doLayout(this);
	//    }
	//    pos_bottom = background->getPosition();
	//    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
	//        mj::PlatformConfig::getInstance().rotateKeyboard(true);
	//        pos_top = Vec2(pos_bottom.x - 50, pos_bottom.y);
	//    }else pos_top = Vec2(pos_bottom.x, pos_bottom.y + 150);
}

void StoryParentLicense_Landscape::onShow() {
	auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
	if (root_layout) {
		root_layout->setPositionY(768);
		root_layout->runAction(MoveBy::create(0.25f, Vec2(0, -768)));
	}
}

void StoryParentLicense_Landscape::onBack(Ref *sender) {
	if (isBack) return;
	//    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
	//        mj::PlatformConfig::getInstance().rotateKeyboard(false);
	//    }
	isBack = true;
	auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
	if (root_layout) {
		root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=] {
			this->removeFromParent();
		}), NULL));
	}
}

void StoryParentLicense_Landscape::onDeactivate(cocos2d::Ref *sender) {
	CCLOG("Deactivating...");

	StoryAlert::showAlert2Button(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.question"),
		LANGUAGE_MANAGER->getDisplayTextByKey("deactive.yes"),
		LANGUAGE_MANAGER->getDisplayTextByKey("deactive.no"), false, 30, this, 1001);
}


void StoryParentLicense_Landscape::onForgotLicense(cocos2d::Ref * sender) {
	auto popup = MSForgotLicensePopup::createView();
	Director::getInstance()->getRunningScene()->addChild(popup, 10);
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentLicense_Landscape::onLocateClickCallback(const std::string &callback_name) {

	std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(StoryParentLicense_Landscape, onBack),
		CLICK_MAP(StoryParentLicense_Landscape, onOK),
		CLICK_MAP(StoryParentLicense_Landscape, onDeactivate),
		CLICK_MAP(StoryParentLicense_Landscape, onForgotLicense),
		CLICK_MAP(StoryParentLicense_Landscape, onSubScribe)
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

void StoryParentLicense_Landscape::onSubScribe(cocos2d::Ref * sender) {
	LICENSE_INFO_MODEL->onSubscribe();
}

void StoryParentLicense_Landscape::onOK(cocos2d::Ref *sender) {
    Button* btn = (Button*)sender;
    if (btn!=NULL){
        btn->setTouchEnabled(false);
        this->scheduleOnce([=](float)
        {
            btn->setTouchEnabled(true);
        }, 1.f, "reset button license ");
    }
    auto bg = utils::findChild(this, "license_bg");
    bg->setPositionY(384);
    MJ_PLATFORM_CONFIG.forceDismissKeyboard();
	mj::helper::playButtonFX();
	if (!NativeBridge::networkAvaiable()) {
		auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
		auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
		alert->setNetworkDownContent(false, true);
		alert->show();

		return;
	}

	auto textfield_license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_licensekey"));
	auto license = std::string(textfield_license->getString());
	//    auto license = textfield_license->getString();
	if (license == "") {
		StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.empty"),
			"OK",
			"", false);
		return;
	}

	mj::Pay::getInstance().setActiveLicenseDelegate(this);
	mj::Pay::getInstance().enterLicense(license);

	startLoading();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return;
#endif
	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(false);
}

void StoryParentLicense_Landscape::startLoading() {
	cocos2d::utils::findChild(this, "button")->setVisible(false);
	auto timeline = CSLoader::createTimeline("csb/StoryParentLicense_Landscape.csb");
	timeline->play("preloader", true);
	this->runAction(timeline);
}

void StoryParentLicense_Landscape::endLoading() {
	cocos2d::utils::findChild(this, "button")->setVisible(true);
	this->stopAllActions();
}

#include "MJLicenseActiveSuccess.h"
void StoryParentLicense_Landscape::onActiveLicenseSuccess(const std::string & license, const int & time_expire, const std::string & message, rapidjson::Value data) {
	str_licence = license;
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

		auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

		endLoading();
		auto background = cocos2d::ui::Helper::seekWidgetByName(this, "license_bg");

//        cocos2d::ui::Helper::seekWidgetByName(this, "panel_has_license")->setVisible(true);
//        background->setContentSize(cocos2d::Size(background->getContentSize().width, 350));
//
//        auto text_license = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "text_license"));
//        text_license->setString(mj::Pay::getInstance().getLicense());
//        auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "parent.license.expire"));
//
//        if (time_expire == -1) {
//            text_details->setString(cocos2d::StringUtils::format("%s: %s",
//                LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(),
//                LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()
//            ));
//        } else {
//            auto expire_date = LTDate(time_expire);
//            text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));
//        }


		//    auto expire_date = LTDate(mj::Pay::getInstance().getTimeExpire());
		//    text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));

		cocos2d::ui::Helper::doLayout(this);

		//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
		auto msg = message;
		//        msg += "\n";
		//        msg += LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.expire");
		//
		//        if (time_expire != -1) {
		//            auto exp_date = LTDate(time_expire);
		//            msg += exp_date.toPrettyString();
		//        } else {
		//            msg += LANGUAGE_MANAGER->getDisplayTextByKey("parent.license.lifetime");
		//        }

		auto view = MJLicenseActiveSuccess::createView(msg, [] {
			MJDEFAULT->setBoolForKey(key_first_api_called, true);
			Director::getInstance()->replaceScene(MSIntroGrade::createScene());
		});
		Director::getInstance()->getRunningScene()->addChild(view, INT_MAX);
		//        StoryAlert::showAlert(msg, "OK", "", false, 24, this);
	});
}

#include "MJWarningActiveLicense.h"
void StoryParentLicense_Landscape::onActiveLicenseFailed(const std::string & license, const int & error_code, const std::string & error, rapidjson::Value response_data) {
	str_licence = license;
	endLoading();
    if (error_code == 302){// trương hợp account chưa được verify
        auto phone_verify = APPhoneLogin::createView(true);
        phone_verify->setAnchorPoint(Vec2(0.5,0.5));
        phone_verify->setPosition(this->getContentSize()/2);
        phone_verify->setScale(0.88);
        utils::findChild<ui::Layout*>(phone_verify, "Panel_black")->setOpacity(0);
        this->addChild(phone_verify);
        return;
    }
	if (error_code == 210) {//đã kích hoạt đủ trên 2 thiết bị và vẫn còn quyền xóa thiết bị.
		license::Package package;

		package.license = str_licence;
		if (JSON_SAFE_CHECK(response_data, "phone", String)) {
			package.phone = response_data["phone"].GetString();
		}

		if (JSON_SAFE_CHECK(response_data, "email", String)) {
			package.mail = response_data["email"].GetString();
		}

		if (JSON_SAFE_CHECK(response_data, "product_name", String)) {
			package.name = response_data["product_name"].GetString();
		}

		if (JSON_SAFE_CHECK(response_data, "product_id", String)) {
			package.product_id = response_data["product_id"].GetString();
		}

		if (JSON_SAFE_CHECK(response_data, "time_expire", Int)) {
			package.time_expiry = response_data["time_expire"].GetInt();
		}

		if (JSON_SAFE_CHECK(response_data, "total_active", String)) {
			std::string ss = response_data["total_active"].GetString();
			auto split = mj::helper::split(ss, '/');
			if (split.size() == 2) {
				package.total_active = std::atoi(split[1].c_str());
				package.current_active = std::atoi(split[0].c_str());
			}
			else {
				package.total_active = 0;
				package.current_active = 0;
			}
		}

		auto data = response_data.GetObject();
		if (data.HasMember("phone") && data["phone"].IsString()) {
			phone_info = data["phone"].GetString();
		}

		if (data.HasMember("email") && data["email"].IsString()) {
			mail_info = data["email"].GetString();
		}
		LICENSE_INFO_MODEL->setInfoVerify(phone_info, mail_info);

		if (data.HasMember("device_info") && data["device_info"].IsArray()) {
			auto list_device = data["device_info"].GetArray();
			_list_device_info.clear();
			for (int i = 0; i < (int)list_device.Size(); i++) {
				auto device_data = list_device[i].GetObject();
				license::LicenseDeviceInfo device_info;
				if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
					device_info.device_id = atoi(device_data["device_id"].GetString());
				}

				if (device_data.HasMember("device_id") && device_data["device_id"].IsInt()) {
					device_info.device_id = device_data["device_id"].GetInt();
				}

				if (device_data.HasMember("time_active") && device_data["time_active"].IsString()) {
					device_info.active_date = atoi(device_data["time_active"].GetString());
				}

				if (device_data.HasMember("time_active") && device_data["time_active"].IsInt()) {
					device_info.active_date = device_data["time_active"].GetInt();
				}

				if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
					device_info.device_name = device_data["device_info"].GetString();
				}

				device_info.device_status = 1;
				_list_device_info.push_back(device_info);
			}

			//                    LICENSE_INFO_MODEL->updateLicenseDeviceInfo(list_device_info);
		}

		this->showGuideRemoveLicense(package);
		return;
	}

	if (error_code == 420) {//đã kích hoạt đủ 2 thiết bị đồng thời đã xóa hết quyền được xóa.
		this->updateDeviceInfoLicense(response_data.GetObject());
		this->showOverLicenseView(error);
		return;
	}

	if (error_code == CODE_ACTIVE_LICENSE_DENY) {
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
			auto warning = MJWarningActiveLicense::createView("Không hợp lệ", error, false);
			auto button_cancel = utils::findChild(warning, "btcancel");
			button_cancel->setVisible(false);
			auto button_ok = utils::findChild(warning, "btclose");
			button_ok->setPosition(Size(button_ok->getParent()->getContentSize().width / 2, button_ok->getPosition().y));
			Director::getInstance()->getRunningScene()->addChild(warning, INT_MAX - 1);
		});
		return;
	}

	if (error_code == CODE_ACTIVE_LICENSE_WARNING) {
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
			auto warning = MJWarningActiveLicense::createView("Cảnh báo", error, false, [this, license] {
				startLoading();
				mj::Pay::getInstance().enterLicense(license, true);
			});
			Director::getInstance()->getRunningScene()->addChild(warning, INT_MAX - 1);
		});
		return;
	}

	if (error_code == 411) {//Nhập mã MS vào MJ => báo lỗi => hiện ra thông báo mở app MS hoặc Download app MS
		//
		if (response_data.HasMember("app_id") && response_data["app_id"].IsString()) {
			auto app_id = atoi(response_data["app_id"].GetString());
			std::string str_appname;
			if (app_id == 40) {
				str_appname = "Monkey Stories";
				auto is_stories_installed = mj::PlatformConfig::getInstance().canOpenLearnToRead();
				int tag;
				std::string str_btok;
				auto msg = error;
				if (is_vn) {
					if (is_stories_installed) {
						msg += StringUtils::format("\nVui lòng mở ứng dụng %s lên để nhập mã kích hoạt.", str_appname.c_str());
						tag = 1003;
						str_btok = "Mở";
					}
					else {
						tag = 1004;
						msg += StringUtils::format("\nVui lòng cài đặt ứng dụng %s để nhập mã kích hoạt.", str_appname.c_str());
						str_btok = "Cài đặt";
					}
				}
				else {
					if (is_stories_installed) {
						msg += StringUtils::format("\nPlease open %s and enter the key there.", str_appname.c_str());
						str_btok = "Open";
						tag = 1003;
					}
					else {
						tag = 1004;
						msg += StringUtils::format("nPlease install %s and enter the key there.", str_appname.c_str());
						str_btok = "Install";
					}
				}
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, str_btok, tag, this] {
					StoryAlert::showAlert(msg, str_btok, "", false, 30, this, tag);
				});
			}
		}

	}

	if (error_code == 430) {//nhập license cũ => show màn hình lấy license mới qua điện thoại/mail.
		auto split = mj::helper::split(error, '|');
		std::string mail = "", phone = "";
		if (!split.empty()) {
			mail = split[0];
			if (split.size() > 1) {
				phone = split[1];
			}
		}

		if (mail.empty() && phone.empty()) {

		}
		else {
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
	}
	else {
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([error] {
			StoryAlert::showAlert(error, "OK", "");
		});
	}
}


void StoryParentLicense_Landscape::onAlertOK(cocos2d::Ref *sender) {
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
			}
			else {
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

	MJDEFAULT->setBoolForKey(key_first_api_called, true);
	Director::getInstance()->replaceScene(MSIntroGrade::createScene());
}

void StoryParentLicense_Landscape::onAlertCancel(cocos2d::Ref *sender) {
}

void StoryParentLicense_Landscape::updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success) {
	auto data = response_data.GetObject();
	if (data.HasMember("phone") && data["phone"].IsString()) {
		phone_info = data["phone"].GetString();
	}

	if (data.HasMember("email") && data["email"].IsString()) {
		mail_info = data["email"].GetString();
	}
	if (data.HasMember("total_active") && data["total_active"].IsString()) {
		total_active = data["total_active"].GetString();
	}

	LICENSE_INFO_MODEL->setInfoVerify(phone_info, mail_info);

	if (data.HasMember("device_in_licence") && data["device_in_licence"].IsObject()) {
		auto device_in_licence = data["device_in_licence"].GetObject();
		_list_device_info.clear();
		if (device_in_licence.HasMember("list_active") && device_in_licence["list_active"].IsArray()) {
			auto list_active = device_in_licence["list_active"].GetArray();
			for (int i = 0; i < (int)list_active.Size(); i++) {
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
			for (int i = 0; i < (int)list_remove.Size(); i++) {
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

		if (is_active_success) {
			LICENSE_INFO_MODEL->updateLicenseDeviceInfo(_list_device_info);
			MJDEFAULT->setStringForKey("phone_info", phone_info);
			MJDEFAULT->setStringForKey("mail_info", mail_info);
			MJDEFAULT->setStringForKey("total_active", total_active);
		}
	}
}

void StoryParentLicense_Landscape::showGuideRemoveLicense(license::Package license) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
		auto full_license_view = MJFullLicenseView::createView(license);
		Director::getInstance()->getRunningScene()->addChild(full_license_view, INT_MAX - 1);
		full_license_view->setDelegate(this);
	});
}

#include "StoryParentPage_Landscape.h"

void StoryParentLicense_Landscape::onFullLicenseViewFreeUse() {
	LICENSE_INFO_MODEL->onFreeUse();

	Node * n = this;
	while (!dynamic_cast<StoryParentPage_Landscape *>(n->getParent())) {
		n = n->getParent();
	}

	reinterpret_cast<StoryParentPage_Landscape *>(n->getParent())->onBack(nullptr);
}

void StoryParentLicense_Landscape::onFullLicenseonRemoveDevice(license::Package lic) {
	auto current_device = LICENSE_INFO_MODEL->getDeviceID();
	auto total_device = MJDEFAULT->getStringForKey("total_device");
	auto list_device_view = MJListDeviceView::createView(_list_device_info, current_device, lic, total_device, true);
	cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
	list_device_view->onShow();
	//    this->removeFromParent();
}

void StoryParentLicense_Landscape::showOverLicenseView(std::string message) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, message] {
		auto over_license_view = MJOverLicenseView::createView(message);
		Director::getInstance()->getRunningScene()->addChild(over_license_view, INT_MAX - 1);
		over_license_view->setDelegate(this);
	});
}

void StoryParentLicense_Landscape::onOverLicenseViewFreeUse() {
	LICENSE_INFO_MODEL->onFreeUse();

	Node * n = this;
	while (!dynamic_cast<StoryParentPage_Landscape *>(n->getParent())) {
		n = n->getParent();
	}

	reinterpret_cast<StoryParentPage_Landscape *>(n->getParent())->onBack(nullptr);
}
void StoryParentLicense_Landscape::onOverLicenseViewListDevice() {
	auto current_device = LICENSE_INFO_MODEL->getDeviceID();
	auto total_device = MJDEFAULT->getStringForKey("total_device");
	license::Package lic;
	lic.license = str_licence;
	auto list_device_view = MJListDeviceView::createView(_list_device_info, current_device, lic, total_device, true, false);
	cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
	list_device_view->onShow();
	//    this->removeFromParent();
}


