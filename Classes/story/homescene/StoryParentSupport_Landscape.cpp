//
//  StoryParentSupport_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/12/17.
//
//

#include "StoryParentSupport_Landscape.h"
#include "MJPlatformConfig.h"
#include "HSLib.h"
#include "StoryAlert.h"
#include "manager/CleverTapManager.h"
#include "MJUpdateManager.h"
#include "MJUpdatePopup.h"

INITIALIZE_READER_INSTANCE(StoryParentSupport_LandscapeReader);

StoryParentSupport_Landscape::~StoryParentSupport_Landscape()
{
    //write your code here if you want
}

StoryParentSupport_Landscape * StoryParentSupport_Landscape::createPage()
{
    StoryParentSupport_Landscape *p = static_cast<StoryParentSupport_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentSupport_Landscape.csb"));
    
    if (p)
    {
        p->didLoadFromCSB();
       // Màn hình design là màn hình 700x628, tuỳ từng màn hình, sẽ scale cái content cho đẹp.
        return p;
    }

    CC_SAFE_DELETE(p);
    
    return nullptr;
}

bool StoryParentSupport_Landscape::isNeedToUpdate(const std::string& curr_version, const std::string& newest_version) {
    
    if (curr_version.empty() || newest_version.empty() || newest_version.size() < 5 || curr_version.size() < 5) {
        return true;
    }
    
    if ((curr_version.at(0) < newest_version.at(0)) || (curr_version.at(2) < newest_version.at(2)) || (curr_version.at(4) < newest_version.at(4))) {
        return true;
    }
    
    return false;
}

void StoryParentSupport_Landscape::didLoadFromCSB()
{
    if (!CONFIG_MANAGER->isHomeScreenLandscape())
    {
        this->setContentSize(DIRECTOR->getVisibleSize());
        ui::Helper::doLayout(this);
    }
    
    LANGUAGE_MANAGER->setFontByLang({
        ui::Helper::seekWidgetByName(this, "email"),
        ui::Helper::seekWidgetByName(this, "numberphone"),
        ui::Helper::seekWidgetByName(this, "facebook_group")
    }, StoryLanguageManager::TextType::BOLD, true);
    
	LANGUAGE_MANAGER->setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"),"key.support",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "call"),"key.support.call",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "join"),"support_action3",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "contact_mail"),"key.support.send.mail",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "update_bt"),"key.support.update.now",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(cocos2d::utils::findChild(this, "app_info"), "info"),"key.support.app.info",StoryLanguageManager::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(cocos2d::utils::findChild(this, "app_info"), "newest_version"),"key.support.app.info.new.version",StoryLanguageManager::TextType::REGULAR},
		{cocos2d::utils::findChild<ui::Text*>(cocos2d::utils::findChild(this, "app_info"), "curr_version"),"key.support.app.info.curr.version",StoryLanguageManager::TextType::REGULAR},
	}, true);
	
    changeSuppportInformation();

    std::string currVerion = MJ_PLATFORM_CONFIG.getAppVersion();
    std::string newestVersion = MJDEFAULT->getStringForKey(key_ver_newest_version, "0");
    
	if (auto currentVersion = utils::findChild<Text*>(this, "curr"))
	{
		currentVersion->setString(StringUtils::format("  %s", currVerion.c_str()));
	}
    
    if(auto newest  = utils::findChild<ui::Text *>(this, "newest")) {
        newest->setString("  " + newestVersion);
    }
    
    auto updateButton = utils::findChild<ui::Button *>(this, "update_bt");
    bool needUpdate = isNeedToUpdate(currVerion, newestVersion);
    updateButton->setVisible(needUpdate);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto bt_send = utils::findChild(this, "contact_mail");
    bt_send->getParent()->setContentSize(Size(bt_send->getParent()->getContentSize().width, 150));
    //bt_send->setVisible(false);
    
    auto bt_call = utils::findChild(this, "call");
    bt_call->getParent()->setContentSize(Size(bt_call->getParent()->getContentSize().width, 150));
    bt_call->setVisible(false);
    
    // hieunt: không hiểu tại sao bản win32 thì background của email lại bị scale chiều cao, đành hard fix ở đây =.=
    if (auto mail_bg = utils::findChild(this, "mail_background")) {
        mail_bg->setContentSize(Size(mail_bg->getContentSize().width, 68));
    }
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	bt_call->setVisible(true);
	bt_send->setVisible(true);
#endif
#endif
 
    
}


void StoryParentSupport_Landscape::onEnter()
{
    StoryParentSubPage_Landscape::onEnter();
    // anh quan da bo phan nay
/*
    if (!CONFIG_MANAGER->isCountryVN()&& !CONFIG_MANAGER->isCountryThailand())
    {
        auto title1 = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title1"));
        utils::findChild(this, "panel_call")->setVisible(false);
        utils::findChild(this, "panel_facebook")->setVisible(false);
        utils::findChild(this, "bulletin")->setVisible(false);
        title1->setAnchorPoint(Point::ANCHOR_MIDDLE);
        title1->setPosition(Point(utils::findChild(this, "panel_mail")->getContentSize().width / 2, title1->getPositionY()));

        utils::findChild(this, "panel_mail")->setPosition(utils::findChild(this, "panel_mail")->getParent()->getContentSize() / 2);
    }
    
    if(MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        utils::findChild(this, "fanpage")->setVisible(false);
    }
 */
}

void StoryParentSupport_Landscape::onShow()
{
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->setPositionY(768);
        root_layout->runAction(MoveBy::create(0.25f, Vec2(0, -768)));
        static_cast<Text *>(ui::Helper::seekWidgetByName(root_layout, "title"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "support_title"));
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentSupport_Landscape::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentSupport_Landscape, onCall),
        CLICK_MAP(StoryParentSupport_Landscape, onMail),
        CLICK_MAP(StoryParentSupport_Landscape, onBack),
        CLICK_MAP(StoryParentSupport_Landscape, onJoin),
        CLICK_MAP(StoryParentSupport_Landscape, onUpdate)
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

void StoryParentSupport_Landscape::onBack(Ref *){
    if (isBack) return;
    isBack = true;
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }
}

void StoryParentSupport_Landscape::onCall(cocos2d::Ref *sender) 
{
	m_supportProperty = "phone";

	pushCleverTapTrackingData();

    if (MJ_PLATFORM_CONFIG.phoneSupport("1900636052"))
	{
		///????
    } 
	else 
	{
        auto str = LANGUAGE_MANAGER->getTextByKeyContryCode("support.action.error");
        StoryAlert::showAlert(str, "OK", "");
    }
}

void StoryParentSupport_Landscape::onMail(cocos2d::Ref *) {
    bool isZalo = MJDEFAULT->getStringForKey(key_country_code, "US") == "VN";
	m_supportProperty = isZalo ? "zalo" : "email";

	pushCleverTapTrackingData();
    
    if (isZalo) {
        Application::getInstance()->openURL("https://zalo.me/396675150817766153");
    }
    else {
        if (MJ_PLATFORM_CONFIG.mailSupport(LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.email"))) {
        }
        else {
            auto str = LANGUAGE_MANAGER->getTextByKeyContryCode("support.action.error");
            StoryAlert::showAlert(str, "OK", "");
        }
    }
}

void StoryParentSupport_Landscape::onJoin(cocos2d::Ref * sender)
{
	m_supportProperty = "facebook";

	pushCleverTapTrackingData();

    if(CONFIG_MANAGER->isAppVNStories())
    {
        if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE){
            Application::getInstance()->openURL("https://www.facebook.com/groups/learntoreadwithmonkey");
        }else{
            Application::getInstance()->openURL(GROUP_VM_URL);
        }
    }
    else
    {
        Application::getInstance()->openURL(LANGUAGE_MANAGER->getTextByKeyContryCode("key.support.fbgroup.link"));
    }
}

void StoryParentSupport_Landscape::onUpdate(cocos2d::Ref *sender) {

    std::string url_app = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    url_app = CONFIG_MANAGER->isVietnameseStories() ? key_appstore_app_vn_url :key_appstore_app_url;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    url_app = CONFIG_MANAGER->isVietnameseStories()?key_app_store_app_vn_android :key_app_store_app_android;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    url_app = CONFIG_MANAGER->isVietnameseStories()?key_app_store_app_vn_mac :key_app_store_app_mac;
#else
    url_app = CONFIG_MANAGER->isVietnameseStories()?key_app_store_app_vn_win32 :key_app_store_app_win32;
#endif
    Application::getInstance()->openURL(url_app);
}

void StoryParentSupport_Landscape::changeSuppportInformation()
{
	std::string email			= LANGUAGE_MANAGER->getTextByKeyContryCode("key.support.info.email");
	std::string phoneNumber		= LANGUAGE_MANAGER->getTextByKeyContryCode("key.support.info.phonenumber");
	std::string fbGroup			= LANGUAGE_MANAGER->getTextByKeyContryCode("key.support.fbgroup.name");

    if(auto emailNode =  static_cast<Text*>(ui::Helper::seekWidgetByName(this, "mail_address")))
    {
		emailNode->setString(email);
    }
    
    if(auto numberphone =  static_cast<Text *>(ui::Helper::seekWidgetByName(this, "phone_number")))
    {
        numberphone->setString(phoneNumber);
    }
    
    if(auto facebookGroup =  static_cast<Text *>(ui::Helper::seekWidgetByName(this, "facebook_group")))
    {
		facebookGroup->setString(fbGroup);
    }

    if (MJDEFAULT->getStringForKey(key_country_code, "US") == "VN") {
        if (auto emailNode = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "mail_address")))
        {
            emailNode->setString("Monkey Việt Nam");
        }
        if (auto btn = static_cast<Button*>(ui::Helper::seekWidgetByName(this, "contact_mail")))
        {
            btn->setTitleText("Chat Zalo");
        }
        if (auto icon = static_cast<ImageView*>(ui::Helper::seekWidgetByName(this, "mail_icon")))
        {
            icon->loadTexture("zalo.png");
            icon->setContentSize(Size(25, 25));
        }
    }
}

void StoryParentSupport_Landscape::pushCleverTapTrackingData()
{
    try
    {
        CleverTapManager::GetInstance()->pushEvent("click_support_button",
        {
            {"view_screen",cocos2d::Value(true)},
            {"support_properties",cocos2d::Value(m_supportProperty)},
        });
    }
    catch (const std::exception& e)
    {

    }
}


