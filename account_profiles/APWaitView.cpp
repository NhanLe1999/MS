#include "APWaitView.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "APPopup.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJDefinitions.h"
USING_NS_CC;
INITIALIZE_READER(APWaitView);

std::once_flag ap_wait_view_reader_flag;

APWaitView * APWaitView::createView(const std::string & message, bool have_btn, bool have_layout) {
	std::call_once(ap_wait_view_reader_flag, [] {
		REGISTER_CSB_READER(APWaitView);
		});

	auto p = reinterpret_cast<APWaitView*>(CSLoader::createNodeWithVisibleSize("csb/account/Waiting_Login.csb"));
	if (p) {
		p->didLoadFromCSB(message, have_btn, have_layout);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void APWaitView::didLoadFromCSB(const std::string & message, bool have_btn, bool have_layout) {
   
    if(have_layout){
        utils::findChild<Layout*>(this, "Panel_1")->setVisible(false);
        utils::findChild<Layout*>(this, "Panel_2")->setVisible(true);
        utils::findChild(this, "message")->setVisible(false);
        
        auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        this->addChild(indicator);
        indicator->setPosition(this->getContentSize()/2);
        indicator->setVisible(true);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
        
    }else{
        utils::findChild<Layout*>(this, "Panel_1")->setVisible(true);
        utils::findChild<Layout*>(this, "Panel_2")->setVisible(false);
        
        if(CONFIG_MANAGER->isVietnameseStories()){
            utils::findChild<Layout*>(this, "Panel_1")->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
        }
        
        utils::findChild<ui::Text*>(this, "message")->setString(message);
        if(have_btn){
            utils::findChild<Layout*>(this, "panel_btn")->setVisible(true);
            utils::findChild<ui::Text*>(this, "message")->setPositionY(this->getContentSize().height*0.6);
        }
    }
}

void APWaitView::onEnter() {
	ui::Layout::onEnter();
    changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
}

void APWaitView::onExit() {
	ui::Layout::onExit();
}

#include "MSEventLogger.h"

void APWaitView::changeDisplayLanguage(int lang_id){
	if (CONFIG_MANAGER->isVietnameseStories())
	{
		utils::findChild<ui::Text*>(this, "message")->setTextColor(Color4B::WHITE);
		utils::findChild<ui::Button*>(this, "btn_confirm")->setTitleColor(Color3B(28, 34, 96));
		utils::findChild<ui::Button*>(this, "btn_cancel")->setTitleColor(Color3B(28, 34, 96));
	}
	else
	{
		utils::findChild<ui::Text*>(this, "message")->setTextColor(Color4B::BLACK);
		utils::findChild<ui::Button*>(this, "btn_confirm")->setTitleColor(Color3B(34,187,231));
		utils::findChild<ui::Button*>(this, "btn_cancel")->setTitleColor(Color3B(34, 187, 231));
	}
    utils::findChild<ui::Text*>(this, "message")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.account.license"));
    utils::findChild<ui::Button*>(this, "btn_confirm")->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.account.license.yesbtn"));
     utils::findChild<ui::Button*>(this, "btn_cancel")->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.account.license.nobtn"));
}

void APWaitView::onSkip(Ref * sender) {

	static_cast<ui::Button*>(sender)->setTouchEnabled(false);
	if (_confirm_callback)
	{
		_confirm_callback(false);
	}
}

void APWaitView::onConfirm(Ref * sender) {

	if (MJDEFAULT->getBoolForKey("is_skip_login", false)) {
		auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
		AccountPopup::show({ is_vn ? "Ba mẹ vui lòng đăng ký tài khoản để sử dụng tính năng này" : "Please register an account to access this function",is_vn ? "Quay lại" : "Back",is_vn ? "Đăng ký" : "Register" }, NULL, [this]() {
			auto waiting_layout = Layout::create();
			waiting_layout->setContentSize(Director::getInstance()->getVisibleSize());
			waiting_layout->setBackGroundColor(Color3B::BLACK);
			waiting_layout->setBackGroundColorType(BackGroundColorType::SOLID);
			waiting_layout->setBackGroundColorOpacity(200);
			waiting_layout->setTouchEnabled(true);
			waiting_layout->setName("waiting_logout");
			Director::getInstance()->getRunningScene()->addChild(waiting_layout);
			auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
			waiting_layout->addChild(indicator);
			indicator->setPosition(waiting_layout->getContentSize() / 2);
			indicator->setVisible(true);
			indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
			//reload data
			auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event)
				{
					MJDEFAULT->setBoolForKey(key_onboarding_license, false);
					MJDEFAULT->setBoolForKey(key_logged_in, false);
					//        waiting_layout->removeFromParent();
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
						auto scene = MSIntro::createScene();
						Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
						});
				});
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
			auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom * event) {
				cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
				waiting_layout->removeFromParent();
				});
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);
			ap::Auth::signOut();
            
			});
	}
	else {
		static_cast<ui::Button*>(sender)->setTouchEnabled(false);
		if (_confirm_callback)
		{
			_confirm_callback(true);
		}
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback APWaitView::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APWaitView, onConfirm),
        CLICK_MAP(APWaitView, onSkip),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }
	return nullptr;
}

#include "MJPlatformConfig.h"
void APWaitView::addDeviceId()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
		this->removeChildByName("text_device_id");
		auto id = MJ_PLATFORM_CONFIG.getDeviceId();
		auto text = ui::Text::create(StringUtils::format("Device ID: %s", id.c_str()), "fonts/Montserrat-SemiBold.ttf", 35);
		text->setName("text_device_id");
		text->setFontSize(20);
		text->setTextColor(Color4B::BLACK);
		text->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
		text->setPosition(Point(Director::getInstance()->getVisibleSize().width - 20, Director::getInstance()->getVisibleSize().height- 20));
		text->setOpacity(0x99);
		this->addChild(text, INT_MAX - 1);
	});
}


