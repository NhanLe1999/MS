
#include "StoryAlert.h"
#include "HSLib.h"
#include "MJHelper.h"
#include "HelperManager.h"

INITIALIZE_READER_INSTANCE(StoryAlertReader);

USING_NS_CC;
using namespace ui;

StoryAlert * StoryAlert::showAlert(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt, int fontSize, StoryAlertDelegate * delegate, int tag,std::string alertName, std::string rickText, int fontSizeButton)
{
    CRASH_AUTO_LOG;
    mj::helper::deleteWaitView();
    std::string csb_name = "";
    
    csb_name = CONFIG_MANAGER->isHomeScreenLandscape()? mj::helper::getCSBName("csb/StoryAlert.csb") :"csb/StoryAlert_Portrait.csb";
   
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    
    if(!alert)
    {
        CCLOG("StoryAlert::showAlert| cannot parse .csb file.");
        
        return nullptr;
    }
    
    alert->_timeline = "";
    alert->_isTowButton = false;
    
    if(!alertName.empty())
    {
        alert->setName(alertName);
    }
    
    alert->setTag(tag);
    
    auto texts = mj::helper::split(message, '#');
    
    alert->chanegFontsBasedOnLanguage();


     if (texts.size() > 1 && rickText.length() == 0)
    {
        alert->setContent(texts[1], label_ok, label_cancel, isOneBt, fontSize, false, rickText);
        utils::findChild<ui::Text *>(alert, "label_title")->setString(texts[0]);
        utils::findChild<ui::Text *>(alert, "label_title")->setVisible(true);
        if (alert->label_message)
        {
            alert->label_message->setPosition(alert->label_message->getPosition() + Vec2(0, -20));
        }
    }
    else
    {
        utils::findChild<ui::Text *>(alert, "label_title")->setVisible(false);
        alert->setContent(message, label_ok, label_cancel, isOneBt, fontSize, false, rickText);
    }

    alert->show(Director::getInstance()->getRunningScene());
    alert->setDelegate(delegate);
    if (fontSizeButton != 0)
    {
        alert->label_yes->setFontSize(fontSizeButton);
        if (MJDEFAULT->getIntegerForKey(key_lang_display) != LANGUAGE_THAI_CODE)
        {
            alert->label_yes->setFontName("fonts/Montserrat-Bold.ttf");
        }
    }

    alert->setName("showAlert_popup");
    
    return alert;
}

StoryAlert * StoryAlert::showAlert2Button(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt, int fontSize, StoryAlertDelegate *delegate, int tag, std::string alertCSBPath, std::string rickText)
{
    CRASH_AUTO_LOG;
    mj::helper::deleteWaitView();
    auto csb_name = alertCSBPath.empty()? ( CONFIG_MANAGER->isHomeScreenLandscape()? "csb/StoryAlert_2button.csb":"csb/StoryAlert_2button_Portrait.csb") : alertCSBPath;

    auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
    
    alert->setTag(tag);
    alert->_isTowButton = true;
    
    alert->chanegFontsBasedOnLanguage();

    auto texts = mj::helper::split(message, '#');
    
    if (texts.size() > 1 && rickText.length() == 0)
    {
        alert->setContent(texts[1], label_ok, label_cancel, isOneBt, fontSize, true, rickText);
        utils::findChild<ui::Text *>(alert, "label_title")->setString(texts[0]);
        utils::findChild<ui::Text *>(alert, "label_title")->setVisible(true);
        if (alert->label_message)
        {
            alert->label_message->setPosition(alert->label_message->getPosition() + Vec2(0, -20));
        }
    }
    else
    {
        utils::findChild<ui::Text *>(alert, "label_title")->setVisible(false);
        alert->setContent(message, label_ok, label_cancel, isOneBt, fontSize, true, rickText);
    }
    alert->show(Director::getInstance()->getRunningScene());
    if (delegate)
    {
        alert->setDelegate(delegate);
    }

    return alert;
}

void StoryAlert::onEnter()
{
    CRASH_AUTO_LOG;
    Layout::onEnter();
    didLoadFromCSB();
    if (_timeline.empty()) {
        if (isNetworkDownAlert) {
            MJDEFAULT->setStringForKey(key_type_error, "No Internet");
            _timeline = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        } else
            _timeline = "csb/StoryAlert.csb";
    }
    auto timeline = CSLoader::createTimeline(_timeline);
    this->runAction(timeline);
    timeline->play("show", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] {
        isRunning = false;
    });
}

#pragma mark - CSB Reader callbacks
void StoryAlert::didLoadFromCSB()
{
    CRASH_AUTO_LOG;
    this->setName("storyalert_popup");
    
    delegate = nullptr;
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
}

void StoryAlert::show(Scene *scene, int z_order)
{
    CRASH_AUTO_LOG;
    if (!scene)
    {
        if(auto alert = utils::findChild(DIRECTOR->getRunningScene(), "StoryAlert"))
            return;
        DIRECTOR->getRunningScene()->addChild(this, z_order);
        this->setName("StoryAlert");
    }
    else
    {
        if(auto alert = utils::findChild(scene, "StoryAlert"))
            return;
        scene->addChild(this, z_order);
        this->setName("StoryAlert");
    }
    
}

void StoryAlert::setContent(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt, int fontSize, bool isTowButton, std::string rickText)
{
    CRASH_AUTO_LOG;
    _isTowButton = isTowButton;
    label_yes = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_yes"));
    label_no = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_no"));


    //label_yes->se
    auto parentYes = label_yes->getParent();
    auto parentNo = label_no->getParent();

    label_yes->setString(label_ok);
    label_no->setString(label_cancel);
    if(_isTowButton)
    {
        while (label_yes->getBoundingBox().size.width > parentYes->getBoundingBox().size.width - 5)
        {
            label_yes->setFontSize(label_yes->getFontSize() - 1);
        }

        _fontSize = label_yes->getFontSize();

        while (label_no->getBoundingBox().size.width > parentNo->getBoundingBox().size.width - 5)
        {
            label_no->setFontSize(_fontSize);
            _fontSize = _fontSize - 1;
        }

        _fontSize = _fontSize + 1;

        label_yes->setFontSize(_fontSize);
        label_no->setFontSize(_fontSize);

    }
    auto contentlb = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_message"));
    
    contentlb->setVisible(false);
    auto alert_bg = ui::Helper::seekWidgetByName(this, "alert_bg");
    
    bool isTh = LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE;

    if (rickText.length() != 0)
    {
        auto alert_layout = ui::Helper::seekWidgetByName(this, "alert_layout");
        auto notiText = cocos2d::ui::RichText::createWithXML(rickText);
        notiText->ignoreContentAdaptWithSize(false);
        notiText->setContentSize(Size(alert_bg->getContentSize().width * 0.8, isTh ? 36 : 100));
        notiText->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
        notiText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
        auto positionYTextNoti = isTh ? 275 - notiText->getBoundingBox().size.height : 270;
        notiText->setPosition(Vec2(alert_layout->getContentSize().width / 2, positionYTextNoti));
        alert_layout ->addChild(notiText);
    }
    else {
        label_message = Label::createWithTTF(message, isTh ? "fonts/leelawad.ttf" : "fonts/Montserrat-Medium.ttf", fontSize);
        label_message->setOpacity(180);
        label_message->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        
        alert_bg->addChild(label_message);

        if (CONFIG_MANAGER->isVietnameseStories())
        {
            label_message->setPosition(contentlb->getPosition() - Point(0, 20));
            label_message->setTextColor(Color4B(66, 46, 135, 255));
            label_message->setWidth(alert_bg->getContentSize().width * 0.9);
        }
        else
        {
            label_message->setPosition(contentlb->getPosition());
            label_message->setTextColor(Color4B(40, 127, 157, 255));
            label_message->setWidth(alert_bg->getContentSize().width * 0.8);
        }
    }
    
    if (isOneBt)
    {
        setStoryAlertOneButton();
    }
}


void StoryAlert::setStoryAlertOneButton()
{
    CRASH_AUTO_LOG;
    static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btok"))->setVisible(false);
    auto buttonClose = static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btcancel"));
    buttonClose->setPositionY(85);
}


void StoryAlert::setAlertConfirmRate(std::string message, std::string label_ok, std::string label_cancel, std::string label_guide)
{
    CRASH_AUTO_LOG;
    label_yes = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_yes"));
    label_no = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_no"));
    
    label_yes->setString(label_ok);
    label_no->setString(label_cancel);

    static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_guide"))->setString(label_guide);
    
    auto contentlb = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_message"));
    contentlb->setVisible(false);
    label_message = Label::createWithTTF(message,LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf", 40);
    label_message->setOpacity(180);
    label_message->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    auto alert_layout = ui::Helper::seekWidgetByName(this, "alert_bg");
    alert_layout->addChild(label_message);
    
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        label_message->setPosition(contentlb->getPosition() - Point(0, 20));
        label_message->setTextColor(Color4B(66, 46, 135, 255));
        label_message->setWidth(alert_layout->getContentSize().width*0.9);
    }
    else
    {
        label_message->setPosition(contentlb->getPosition());
        label_message->setTextColor(Color4B(40, 127, 157, 255));
        label_message->setWidth(alert_layout->getContentSize().width*0.8);
    }
}

void StoryAlert::setUpdatePopupContent(bool is_required){
    CRASH_AUTO_LOG;
    label_yes = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_yes"));
    label_no = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_no"));
    
    label_yes->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "update_btok"));
    label_no->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "update_btcancel"));
    
    
    auto map_content = JsonDump::getInstance()->dumpValueMapFromString(MJDEFAULT->getStringForKey(key_update_content));
    
    auto contentlb = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_message"));
    contentlb->setVisible(false);
    label_message = Label::createWithTTF(map_content["message"].asString(),LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf", 28);
    label_message->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    auto alert_layout = ui::Helper::seekWidgetByName(this, "alert_layout");
    alert_layout->addChild(label_message);
    
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        label_message->setPosition(contentlb->getPosition() - Point(0, 20));
        label_message->setTextColor(Color4B(66, 46, 135, 255));
        label_message->setWidth(alert_layout->getContentSize().width*0.9);
        label_message->setSystemFontSize(28*(CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f));
    }
    else
    {
        label_message->setPosition(contentlb->getPosition());
        label_message->setTextColor(Color4B(40, 127, 157, 255));
        label_message->setWidth(alert_layout->getContentSize().width*0.8);
    }
    
    if (map_content["status"].asString() == "require" || is_required) {
        static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btcancel"))->setVisible(false);
        auto btok = static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btok"));
        btok->setPositionY(72);
        label_message->setPositionY(186);
    }
    if (label_message->getStringNumLines() < 2) {
        label_message->setTTFConfig(TTFConfig(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf" : "fonts/Montserrat-Medium.ttf", 32));
    }
    
    auto title = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "label_title"));
    title->setString(replaceString(map_content["title"].asString(), "*", "\n"));
}


void StoryAlert::setNetworkDownContent(bool isPortrait, bool isOneBt, bool isCloseBt)
{
    CRASH_AUTO_LOG;
    auto classes_name = "popup";
    
    isNetworkDownAlert = true;
    
    if(auto imgAlertBg = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "alert_bg"))
    {
        imgAlertBg->loadTexture("mjstor/popup/networkdown_ngang_bg_new.png");
    }
    
    Layout *alert_layout = isPortrait?static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "alert_layout_doc")):static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "alert_layout_ngang"));
  
    alert_layout->setVisible(true);
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(alert_layout, "label_yes"),
            utils::findChild<ui::Text *>(alert_layout, "label_no"),
            utils::findChild<ui::Text *>(alert_layout, "label_message"),
            utils::findChild<ui::Text *>(alert_layout, "titlelb"),
            utils::findChild<ui::Text *>(alert_layout, "label_one"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/monkeystories_medium.ttf");
        }
    }
        
    label_yes = static_cast<Text *>(ui::Helper::seekWidgetByName(alert_layout, "label_yes"));
    label_no = static_cast<Text *>(ui::Helper::seekWidgetByName(alert_layout, "label_no"));
    
    string message, label_ok, label_cancel, titlestr;
    
    label_ok        = string(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "network_ok"));
    label_cancel    = string(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "network_cancel"));
    titlestr        = string(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "network_title"));
    
    message = string(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, (isPortrait) ? "network_cancel_msgdoc": "network_cancel_msgngang", true));
    
    label_yes->setString(label_ok);
    label_no->setString(label_cancel);
    
    static_cast<Text *>(ui::Helper::seekWidgetByName(alert_layout, "label_message"))->setString(message);
    static_cast<Text *>(ui::Helper::seekWidgetByName(alert_layout, "titlelb"))->setString(titlestr);
    
    if (isOneBt)
    {
        if (isPortrait)
        {
            auto btok = (ui::Helper::seekWidgetByName(alert_layout, "btok"));
            btok->setVisible(false);
//            btok->setTouchEnabled(false);
//            btok->setOpacity(100);
            (ui::Helper::seekWidgetByName(alert_layout, "btcancel"))->setContentSize(cocos2d::Size(524, 180));
            label_no->setFontSize(35);
            
            if (auto btn_x = ui::Helper::seekWidgetByName(alert_layout, "btcancel_x")) btn_x->setVisible(isCloseBt);
        }
        else
        {
            (ui::Helper::seekWidgetByName(alert_layout, "btcancel"))->setVisible(false);
            (ui::Helper::seekWidgetByName(alert_layout, "btok"))->setVisible(false);
            (ui::Helper::seekWidgetByName(alert_layout, "btone"))->setVisible(true);
            static_cast<Text *>(ui::Helper::seekWidgetByName(alert_layout, "label_one"))->setString(label_cancel);
            
            if (auto btn_x = ui::Helper::seekWidgetByName(alert_layout, "btcancel_x")) btn_x->setVisible(isCloseBt);
        }
    }
}

void StoryAlert::setDelegate(StoryAlertDelegate * delegate)
{
    this->delegate = delegate;
}

Widget::ccWidgetClickCallback StoryAlert::onLocateClickCallback(const std::string &callback_name) {
    CRASH_AUTO_LOG;
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryAlert, onOK),
        CLICK_MAP(StoryAlert, onCancel),
        CLICK_MAP(StoryAlert, onGuideRate),
        CLICK_MAP(StoryAlert, onClose)
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

#pragma mark - callback functions

void StoryAlert::onOK(cocos2d::Ref *sender) {
    CRASH_AUTO_LOG;
    if (isRunning) return;
    string name_timeline;
    if (isNetworkDownAlert) {
        MJDEFAULT->setStringForKey(key_type_error, "No Internet");
        name_timeline = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    }
    else 
        name_timeline =  mj::helper::getCSBName("csb/StoryAlert.csb");
    auto timeline = CSLoader::createTimeline(name_timeline);
    this->runAction(timeline);
    timeline->play("hide", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] 
	{
		if (m_okButtonCallback)
		{
			m_okButtonCallback();
		}

        if (this->delegate) {
            this->delegate->onAlertOK(this);
        }
        this->removeFromParent();
    });
}

void StoryAlert::onClose(cocos2d::Ref *sender) {
    CRASH_AUTO_LOG;
    MJDEFAULT->deleteValueForKey(key_count_retry);
    if (isRunning) return;
    string name_timeline;
    if (isNetworkDownAlert) {
        MJDEFAULT->setStringForKey(key_type_error, "No Internet");
        name_timeline = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    }
    else name_timeline =  mj::helper::getCSBName("csb/StoryAlert.csb");
    auto timeline = CSLoader::createTimeline(name_timeline);
    this->runAction(timeline);
    timeline->play("hide", false);
    isRunning = true;
    timeline->setLastFrameCallFunc([=] {
        if (this->delegate) {
            this->delegate->onAlertClose(this);
        }
        this->removeFromParent();
        if (m_CloseButtonCallback)
        {
            m_CloseButtonCallback();
        }
    });
}

void StoryAlert::onCancel(cocos2d::Ref *sender) {
    CRASH_AUTO_LOG;
    MJDEFAULT->setBoolForKey("key_update_3_day", false);
    if (isRunning) return;
    string name_timeline;
    if (isNetworkDownAlert) {
        MJDEFAULT->setStringForKey(key_type_error, "No Internet");
        name_timeline = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    }else if (isRateConfirm){
        name_timeline = "csb/StoryAlertLoveApp.csb";
        if(CONFIG_MANAGER->isVietnameseStories()){
            name_timeline = "csb/StoryRateConfirm_VN.csb";
        }
    }else name_timeline = mj::helper::getCSBName("csb/StoryAlert.csb");
    
    if (!isRateConfirm) {
        auto timeline = CSLoader::createTimeline(name_timeline);
        this->runAction(timeline);
        timeline->play("hide", false);
        isRunning = true;
        timeline->setLastFrameCallFunc([=] 
		{
			if (m_cancelButtonCallback)
			{
				m_cancelButtonCallback();
			}

            if (this->delegate) {
                this->delegate->onAlertCancel(this);
            }
            this->removeFromParent();
        });
    }
}

void StoryAlert::onGuideRate(cocos2d::Ref * sender){
    CRASH_AUTO_LOG;
    string guide_url = "http://www.earlystart.co/rate-ios.html";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    guide_url = "http://www.earlystart.co/rate-android.html";
#endif
    
    if(MJDEFAULT->getIntegerForKey(key_lang_display) == 4) {
        guide_url = "http://www.daybehoc.com/rate-ios.html";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        guide_url = "http://www.daybehoc.com/rate-android.html";
#endif
    }

    Application::getInstance()->openURL(guide_url);
}


void StoryAlert::onTapOutside(cocos2d::Ref *sender) {

}

std::string StoryAlert::replaceString(std::string subject, const std::string search,
                                                const std::string replace) {
    CRASH_AUTO_LOG;
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}


void StoryAlert::chanegFontsBasedOnLanguage()
{
    CRASH_AUTO_LOG;
    if(auto text = utils::findChild<ui::Text *>(this, "label_title"))
    {
		const std::string k_fontPath = HelperManager::GetInstance()->getFontPath(text, "fonts/monkeystories_medium.ttf");

        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName(k_fontPath);
    }
    
    if(auto text = utils::findChild<ui::Text *>(this, "label_yes"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName(CONFIG_MANAGER->isVietnameseStories() ? "fonts/monkeystories_ultra.ttf" : "fonts/monkeystories_medium.ttf");
    }
    
    if(auto text = utils::findChild<ui::Text *>(this, "label_no"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName(CONFIG_MANAGER->isVietnameseStories() ? "fonts/monkeystories_ultra.ttf" : "fonts/monkeystories_medium.ttf");
    }
    
    if(auto text = utils::findChild<ui::Text *>(this, "label_message"))
    {
		const std::string k_fontPath = HelperManager::GetInstance()->getFontPath(text, "fonts/monkeystories_medium.ttf");
	
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  
			text->setFontName("fonts/leelawad.ttf") : text->setFontName(k_fontPath);
    }
}
