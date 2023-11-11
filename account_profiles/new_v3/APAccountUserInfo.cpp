//
//  APAccountUserInfo.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#include "APAccountUserInfo.h"
#include "APDatabase.h"
#include "APAuth.h"
#include "MSEventLogger.h"
#include "manager/GettingUserInfoManager.h"
#include "manager/RatingManager.h"
#include "MJ_PageLoadResource.h"
#include "MSIntro.h"
#include "MJPay.h"
#include "StoryLanguageManager.h"
#include "LTDate.h"
#include "StoryParentProfileDetail_Landscape.h"
#include "APUpdateUserInfoAPI.h"
#include "APLoadUpdateAPI.h"
#include "APAccountAddPassword.h"
#include "APAccountUpdatePassword.h"
#include "APAccountConfirmPassword.h"
#include "APAccountProfileDetail.h"
#include "APPopup.h"
#include "NativeBridge.h"
#include "CleverTapManager.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountUserInfo);

#define MAX_AVATAR_SHOW 4

#define tag_network_not_available       1000

std::once_flag v3_ap_account_user_info_reader_flag;

#pragma mark - Initialize
APAccountUserInfo * APAccountUserInfo::createView()
{
    std::call_once(v3_ap_account_user_info_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountUserInfo);
    });
    
    auto p = static_cast<APAccountUserInfo *>(CSLoader::createNodeWithVisibleSize("csb/account/new_v3/APAccountUserInfo.csb"));

    if (p && p->didLoadFromCSB())
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountUserInfo::onExit()
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    APPopupBase::onExit();
}

bool APAccountUserInfo::didLoadFromCSB()
{
    
    if (CONFIG_MANAGER->isVietnameseStories())
    {
         if(auto background = utils::findChild<ui::Layout*>(this, "layout"))
         {
             background->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
         }
        
        std::vector<ui::Text*> changingWhiteColorTexts = {
            
            utils::findChild<ui::Text*>(this, "expiry_date"),
            utils::findChild<ui::Text*>(utils::findChild<ui::Layout*>(this, "layout"), "title"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "title"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "name"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "email"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "email_value"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "phone"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_parents"), "phone_value"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_password"), "user_name"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_password"), "title"),
            utils::findChild<ui::Text*>(utils::findChild(this, "panel_students"), "title"),
        };
        
        for(auto text : changingWhiteColorTexts)
        {
            text->setTextColor(cocos2d::Color4B::WHITE);
        }
    }
    
//    this->setName(name_screen_apaccount_user_info);
    
    m_button_update_name = utils::findChild<ui::Button *>(this, "btn_update_name");
    m_textfield_name = utils::findChild<ui::TextField *>(this, "name_textfield");
    
    m_button_update_name->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_update_name->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.update"));
    
    // screen title
    auto screenTitle = utils::findChild<ui::Text *>(this, "title");
    screenTitle->setFontName(DEF_FONT_FILE_PATH);
    screenTitle->setString(DEF_GET_TEXT("key.accountv3.screen.account.title"));
    
    // button sign-out
    auto button_signout = utils::findChild<ui::Button *>(this, "btn_signout");
    button_signout->setTitleFontName(DEF_FONT_FILE_PATH);
    button_signout->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.signout"));
    
    // panel: PARENTS
    if (1)
    {
        auto panel_parents = utils::findChild<ui::Layout *>(this, "panel_parents");
        
        auto title = (ui::Text *)panel_parents->getChildByName("title");
        title->setFontName(DEF_FONT_FILE_PATH);
        title->setString(DEF_GET_TEXT("key.accountv3.screen.account.title.parents"));
        
        // name
        auto nameTitle = (ui::Text *)panel_parents->getChildByName("name");
        nameTitle->setFontName(DEF_FONT_FILE_PATH);
        nameTitle->setString(DEF_GET_TEXT("key.accountv3.screen.account.name"));
        
        m_textfield_name->setFontName(DEF_FONT_FILE_PATH);
        m_textfield_name->setPlaceHolder("");
        m_textfield_name->setTextColor(Color4B::BLACK);
        m_textfield_name->setCursorChar('|');
        m_textfield_name->setCursorEnabled(true);
        m_textfield_name->setMaxLengthEnabled(true);
        m_textfield_name->setMaxLength(DEF_NAME_MAX_LENGTH);
        
        m_textfield_name->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
        {
            switch (type)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                case TextField::EventType::DETACH_WITH_IME:
                {
                    this->trimSpace(m_textfield_name);
                }
                    break;
#endif

                default: break;
            }
        });
        
        // email
        auto emailTitle = (ui::Text *)panel_parents->getChildByName("email");
        emailTitle->setFontName(DEF_FONT_FILE_PATH);
        emailTitle->setString(DEF_GET_TEXT("key.popup.user.info.email") + " |");
        
        // phone
        auto phoneTitle = (ui::Text *)panel_parents->getChildByName("phone");
        phoneTitle->setFontName(DEF_FONT_FILE_PATH);
        phoneTitle->setString(DEF_GET_TEXT("key.accountv3.screen.account.phone") + " |");
    }
    
    // panel: PASSWORD
    if (1)
    {
        auto panel_password = utils::findChild<ui::Layout *>(this, "panel_password");
        
        auto title = (ui::Text *)panel_password->getChildByName("title");
        title->setFontName(DEF_FONT_FILE_PATH);
        title->setString(DEF_GET_TEXT("key.accountv3.screen.account.managepassword"));
    }
    
    // panel: PROFILE
    if (1)
    {
        auto panel_students = utils::findChild<ui::Layout *>(this, "panel_students");
        
        auto title = (ui::Text *)panel_students->getChildByName("title");
        title->setFontName(DEF_FONT_FILE_PATH);
        title->setString(DEF_GET_TEXT("key.accountv3.screen.account.manageprofiles"));
        
        auto button_detail = (ui::Button *)panel_students->getChildByName("btn_detail");
        button_detail->setTitleFontName(DEF_FONT_FILE_PATH);
        button_detail->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.details"));
    }
    
    if (MJDEFAULT->getBoolForKey("is_skip_login",false))
    {
        if(auto btnSignOutText = utils::findChild<ui::Button*>(this, "btn_signout"))
        {
            btnSignOutText->setTitleFontName( (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
            
            btnSignOutText->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"));
        }
    }

    if (auto userName = utils::findChild<ui::Text*>(this, "user_name"))
    {
        userName->setFontName(DEF_FONT_FILE_PATH);

        userName->setString(getDisplayTextByKey("key.v4.username")+":");
    }
    
    this->resetAccountInfoScreen();
    
    m_didLoadFromCSB = true;
    
    return true;
}

void APAccountUserInfo::resetAccountInfoScreen()
{
    this->loadUserInfo();
    this->loadLicenseInfo();
    this->loadProfileInfo();
}

void APAccountUserInfo::resetProfileInfo()
{
    this->loadProfileInfo();
}

void APAccountUserInfo::loadUserInfo()
{
    // Load Thông tin tài khoản
    auto user_info = ap::Database::getInstance().getUserInfo();
    
    auto txt_email = utils::findChild<ui::Text *>(this, "email_value");
    auto txt_phone = utils::findChild<ui::Text *>(this, "phone_value");
    txt_email->setFontName(DEF_FONT_FILE_PATH);
    txt_phone->setFontName(DEF_FONT_FILE_PATH);
    
    m_textfield_name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    m_textfield_name->setString(user_info._name);

	if (auto userNameTitle = utils::findChild<ui::TextField*>(this, "user_name_textfield"))
	{
		userNameTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
		userNameTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
		userNameTitle->setString(user_info._userName.empty()?"-----": user_info._userName);
		userNameTitle->setVisible(!user_info._userName.empty());
		userNameTitle->setEnabled(false);

		if (auto userName = utils::findChild<ui::Text*>(this, "user_name"))
		{
			userName->setVisible(!user_info._userName.empty());

			if (user_info._userName.empty())
			{
				if (auto buttonUpdatePassword = utils::findChild<ui::Button*>(this, "btn_update_password"))
				{
					buttonUpdatePassword->setIgnoreAnchorPointForPosition(false);
					buttonUpdatePassword->setAnchorPoint(userName->getAnchorPoint());
					buttonUpdatePassword->setPosition(userName->getPosition());
				}
			}
		}
		
	}

    // panels
    auto panel_parents = utils::findChild<ui::Layout *>(this, "panel_parents");
    auto panel_password = utils::findChild<ui::Layout *>(this, "panel_password");
    auto panel_students = utils::findChild<ui::Layout *>(this, "panel_students");
    
    // MARK: Hiển thị Thêm/Sửa email
    float padding_of_button = 7.f; // padding của button Add/Update đến email và phone
    
    auto button_updateEmail = utils::findChild<ui::Button *>(this, "btn_update_email");
    button_updateEmail->setTitleFontName(DEF_FONT_FILE_PATH);
    
    auto emailTitle = panel_parents->getChildByName("email");
    
    if (user_info._mail.empty())    /* chưa có email */
    {
        txt_email->setString(DEF_GET_TEXT("key.accountv3.screen.account.add.email.to.signin"));
        txt_email->setTextColor(Color4B(0, 0, 0, 112));
        
        button_updateEmail->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.add"));
        button_updateEmail->setPosition(Point(emailTitle->getPosition().x +
                                              emailTitle->getContentSize().width -
                                              (button_updateEmail->getContentSize().width - button_updateEmail->getTitleLabel()->getContentSize().width)/2 +
                                              padding_of_button,
                                              button_updateEmail->getPosition().y));
    }
    else                            /* đã có email */
    {
        txt_email->setString(user_info._mail);
        txt_email->setTextColor(Color4B::BLACK);
        
        button_updateEmail->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.update"));
        button_updateEmail->setPosition(Point(emailTitle->getPosition().x +
                                              emailTitle->getContentSize().width -
                                              (button_updateEmail->getContentSize().width - button_updateEmail->getTitleLabel()->getContentSize().width)/2 +
                                              padding_of_button,
                                              button_updateEmail->getPosition().y));
    }
    
    // MARK: Hiển thị Thêm/Sửa số điện thoại
    auto button_addPhone = utils::findChild<ui::Button *>(this, "btn_add_phone");
    button_addPhone->setTitleFontName(DEF_FONT_FILE_PATH);
    
    auto phoneTitle = panel_parents->getChildByName("phone");
    
    if (user_info._phone.empty())               /* chưa có số điện thoại */
    {
        txt_phone->setString(DEF_GET_TEXT("key.accountv3.screen.account.add.phone.to.signin"));
        txt_phone->setTextColor(Color4B(0, 0, 0, 112));
        
        button_addPhone->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.add"));
        button_addPhone->setPosition(Point(phoneTitle->getPosition().x +
                                           phoneTitle->getContentSize().width -
                                           (button_addPhone->getContentSize().width - button_addPhone->getTitleLabel()->getContentSize().width)/2 +
                                           padding_of_button,
                                           button_addPhone->getPosition().y));
    }
    else                                        /* đã có số điện thoại */
    {
        txt_phone->setString(user_info._phone);
        txt_phone->setTextColor(Color4B::BLACK);
        
        button_addPhone->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.update"));
        button_addPhone->setPosition(Point(phoneTitle->getPosition().x +
                                           phoneTitle->getContentSize().width -
                                           (button_addPhone->getContentSize().width - button_addPhone->getTitleLabel()->getContentSize().width)/2 +
                                           padding_of_button,
                                           button_addPhone->getPosition().y));
    }
    
    // MARK: Hiển thị Quản lý mật khẩu tài khoản
    auto layout = utils::findChild<ui::Layout *>(this, "layout");
    auto line1 = layout->getChildByName("line1");
    auto line2 = layout->getChildByName("line2");
    auto line3 = layout->getChildByName("line3");
    auto expiry_date = layout->getChildByName("expiry_date");
    
    // nếu cả email và phone đều trống thì không hiển thị
    if ((user_info._mail.empty() && user_info._phone.empty()))
    {
        if (m_bothEmailPhoneEmpty == false)
        {
            panel_password->setVisible(false);
            line2->setVisible(false);
            
            const Point p_line1 = line1->getPosition();
            const Point p_line3 = line3->getPosition();
            const Point p_expiry_date = expiry_date->getPosition();
            
            panel_parents->setPosition(Point(panel_parents->getPosition().x, (44.f/100.f)*layout->getContentSize().height));
            panel_students->setPosition(Point(panel_students->getPosition().x, (58.f/100.f)*layout->getContentSize().height));
            expiry_date->setPosition(Point(p_expiry_date.x, p_expiry_date.y - (6.f/100.f)*layout->getContentSize().height));
            line1->setPosition(Point(p_line1.x, p_line1.y - (6.f/100.f)*layout->getContentSize().height + 2.f));
            line3->setPosition(Point(p_line3.x, p_line3.y + (8.f/100.f)*layout->getContentSize().height));
        }
        
        m_bothEmailPhoneEmpty = true;
    }
    else
    {
        if (m_bothEmailPhoneEmpty) {
            panel_password->setVisible(true);
            line2->setVisible(true);
            
            panel_parents->setPosition(Point(panel_parents->getPosition().x, (50.f/100.f)*layout->getContentSize().height));
            panel_students->setPosition(Point(panel_students->getPosition().x, (50.f/100.f)*layout->getContentSize().height));
            expiry_date->setPosition(Point(expiry_date->getPosition().x, expiry_date->getPosition().y + (6.f/100.f)*layout->getContentSize().height));
            line1->setPosition(Point(line1->getPosition().x, line1->getPosition().y + (6.f/100.f)*layout->getContentSize().height - 2.f));
            line3->setPosition(Point(line3->getPosition().x, line3->getPosition().y - (8.f/100.f)*layout->getContentSize().height));
        }
        
        m_bothEmailPhoneEmpty = false;
        
        auto button_updatePassword = utils::findChild<ui::Button *>(this, "btn_update_password");
        auto button_updatePassword_title = (ui::Text *)button_updatePassword->getChildByName("title");
        button_updatePassword_title->setFontName(DEF_FONT_FILE_PATH);
        
        // chưa có mật khẩu tài khoản
        if (MJDEFAULT->getBoolForKey("need_to_fill_password", true))
        {
            button_updatePassword_title->setString(DEF_GET_TEXT("key.accountv3.button.title.addpassword"));
        }
        // đã có mật khẩu tài khoản
        else
        {
            button_updatePassword_title->setString(DEF_GET_TEXT("key.accountv3.button.title.updatepassword"));
        }
    }
    
    // MARK: Co giãn một chút với màn hình dài
#if 1
    float diffScaleW = Director::getInstance()->getWinSize().width / 1024.f;
    
    if (m_didLoadFromCSB == false && diffScaleW > 0) {
        float sss = 0.25f;
        float paddingX = sss * (diffScaleW - 1.f) * 1024.f / 2.f;
        
        line1->setContentSize(line1->getContentSize() + Size(paddingX * 2.f, 0));
        line2->setContentSize(line2->getContentSize() + Size(paddingX * 2.f, 0));
        line3->setContentSize(line3->getContentSize() + Size(paddingX * 2.f, 0));
        
        expiry_date->setPosition(expiry_date->getPosition() - Point(paddingX, 0));
        panel_parents->setPosition(panel_parents->getPosition() - Point(paddingX, 0));
        panel_password->setPosition(panel_password->getPosition() - Point(paddingX, 0));
        panel_students->setPosition(panel_students->getPosition() - Point(paddingX, 0));
        
        if (auto btn_detail = panel_students->getChildByName("btn_detail")) btn_detail->setPosition(btn_detail->getPosition() + Point(paddingX * 2.f, 0));
        
        // -- email --
        Point ppp = Point(1.25f*paddingX, 0);
        
        if (auto email_line = panel_parents->getChildByName("line_m1")) email_line->setPosition(email_line->getPosition() + Point(ppp.x - 0.25f*paddingX, ppp.y));
        emailTitle->setPosition(emailTitle->getPosition() + ppp);
        button_updateEmail->setPosition(button_updateEmail->getPosition() + ppp);
        txt_email->setPosition(txt_email->getPosition() + ppp);
        txt_email->setTextAreaSize(Size(txt_email->getContentSize().width + 0.7f*paddingX, txt_email->getContentSize().height));
        
        // -- phone --
        ppp = Point(2.f*paddingX, 0);
        
        if (auto phone_line = panel_parents->getChildByName("line_m2")) phone_line->setPosition(phone_line->getPosition() + Point(ppp.x - 0.25f*paddingX, ppp.y));
        phoneTitle->setPosition(phoneTitle->getPosition() + ppp);
        button_addPhone->setPosition(button_addPhone->getPosition() + ppp);
        txt_phone->setPosition(txt_phone->getPosition() + ppp);
    }
#endif
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        txt_email->setTextColor(Color4B(255, 255, 255, user_info._mail.empty() ? 128 : 255));
        txt_phone->setTextColor(Color4B(255, 255, 255, user_info._phone.empty() ? 128 : 255));
        
        button_updateEmail->setTitleColor(Color3B(41, 176, 223));
        button_addPhone->setTitleColor(Color3B(41, 176, 223));
    }
}

void APAccountUserInfo::loadLicenseInfo()
{
    auto lang_id = LANGUAGE_MANAGER->getDisplayLangId();
    
    auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "expiry_date"));
    
    text_details->setFontName(DEF_FONT_FILE_PATH);
    
    auto expire_time = mj::Pay::getInstance().getTimeExpire();
    
    if (mj::Pay::getInstance().isActive())
    {
        if (expire_time == -1)
        {
            text_details->setString(cocos2d::StringUtils::format("%s: %s",
                LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(),
                LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()
            ));
        }
        else
        {
            auto expire_date = LTDate(expire_time);
            text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));
        }
    }
    else
    {
        text_details->setString(cocos2d::StringUtils::format("%s: %s",
                                                             LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), LANGUAGE_MANAGER->getDisplayTextByKey("key.trial").c_str()));
    }
}

void APAccountUserInfo::loadProfileInfo()
{
    auto lv_avatar = utils::findChild<ui::ListView*>(this, "lv_avatar");
    lv_avatar->setScrollBarEnabled(false);

    auto list_profile = ap::Database::getInstance().getProfiles();
    lv_avatar->removeAllItems();
    int num_avatar = std::min(MAX_AVATAR_SHOW, (int)list_profile.size());
    float size_width = 0.0f;
    for (int i = 0; i < num_avatar; ++i)
    {
        auto path_avatar = list_profile[i].avatar;
        auto image = ui::ImageView::create(path_avatar);
        image->ignoreContentAdaptWithSize(false);
        image->setContentSize(Size(80, 80));
        lv_avatar->pushBackCustomItem(image);
        size_width += image->getContentSize().width + lv_avatar->getItemsMargin();
    }
    lv_avatar->setContentSize(Size(size_width, lv_avatar->getContentSize().height));
}

void APAccountUserInfo::callAPILoadUpdateAndResetScreen()
{
    ap::api::LoadUpdate::call(false, CC_CALLBACK_2(APAccountUserInfo::onLoadUpdateAPICalled, this));
}

void APAccountUserInfo::onLoadUpdateAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        this->resetAccountInfoScreen();
    }
    else
    {
        CCLOG("APAccountUserInfo::onLoadUpdateAPICalled - LoadUpdate responses ERROR");
    }
}

void APAccountUserInfo::onAlertOK(cocos2d::Ref * sender)
{
}

void APAccountUserInfo::onAlertClose(cocos2d::Ref * sender)
{
    
}

void APAccountUserInfo::onAlertCancel(cocos2d::Ref * sender)
{
    if (auto alert = dynamic_cast<StoryAlert *>(sender))
    {
        switch (alert->getTag())
        {
            case tag_network_not_available:
                this->onSignOut(nullptr);
                break;
                
            default:
                break;
        }
    }
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountUserInfo::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountUserInfo, onSignOut),
        CLICK_MAP(APAccountUserInfo, onClose),
        CLICK_MAP(APAccountUserInfo, onUpdateNameClicked),
        CLICK_MAP(APAccountUserInfo, onUpdateEmailClicked),
        CLICK_MAP(APAccountUserInfo, onAddPhoneClicked),
        CLICK_MAP(APAccountUserInfo, onUpdatePasswordClicked),
        CLICK_MAP(APAccountUserInfo, onViewProfilesClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountUserInfo::onSignOut(cocos2d::Ref * sender)
{
    if (!NativeBridge::networkAvaiable())
    {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        alert->setTag(tag_network_not_available);
        alert->setDelegate(this);
        
        return;
    }
    
    ms::EventLogger::getInstance().logEvent("click_signout", {});
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

    ap::Auth::signOut(this);
    //reload data

    auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event)
    {
    
       ms::EventLogger::getInstance().logEvent("event_signout_done", {}); cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_successfully");
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
         ms::EventLogger::getInstance().logEvent("event_signout_fail", {});
        cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
        waiting_layout->removeFromParent();

    });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);

    //delete unused data key.
    GettingUserInfoManager::GetInstance()->deleteData();
    RatingManager::GetInstance()->deleteData();
    
    mj::PageLoadResource::getInstance().deleteCacheAllStory();
    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
    FileUtils::getInstance()->purgeCachedEntries();
}

void APAccountUserInfo::onClose(cocos2d::Ref * sender)
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this]
    {
        this->removeFromParent();
    }), NULL));
}

void APAccountUserInfo::onUpdateNameClicked(cocos2d::Ref * sender)
{
    if(isShowUpgradePopup())
    {
        return;
    }
    
    this->hideVirtualKeyboard(m_textfield_name);
    
    auto name = mj::helper::str_trim(m_textfield_name->getString());
    
    if (!name.empty())
    {
        auto user_info = ap::Database::getInstance().getUserInfo();
        
        if (name.compare(user_info._name) != 0)
        {
            m_button_update_name->setEnabled(false);
            m_textfield_name->setEnabled(false);
            
            // TODO: call API: UpdateUserInfo
            m_name = name;
            ap::api::UpdateUserInfo::call(m_name, "", true, CC_CALLBACK_2(APAccountUserInfo::onUpdateUserInfoAPICalled, this));
        }
        else
        {
            // nếu vẫn là tên cũ thì không làm gì cả!
        }
    }
    else
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("key.accountv3.screen.account.popup.noname"));
    }
}

void APAccountUserInfo::onUpdateUserInfoAPICalled(bool is_success, std::string msg)
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    m_button_update_name->setEnabled(true);
    m_textfield_name->setEnabled(true);
    
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            // TODO: Save name to database
            auto user_info = ap::Database::getInstance().getUserInfo();
            ap::Database::getInstance().saveUserInfo(std::to_string(user_info._id), m_name, user_info._mail, user_info._phone, user_info._address, user_info._userName);
            
            // TODO: CleverTap-pushEvent: "update_user_info"
            try
            {
                CleverTapManager::GetInstance()->pushEvent("update_user_info",
                {
                    {"click_type", cocos2d::Value("name")},
                    {"update_type", cocos2d::Value(user_info._name.length() ? "update" : "add")},
                    {"email_verified", cocos2d::Value("none")},
                    {"update_info_successfull", cocos2d::Value("yes")},
                    {"source_update", cocos2d::Value("organic")},
                });
            }
            catch (const std::exception&)
            {
                
            }
            
            this->resetAccountInfoScreen();
        });
    }
    else
    {
        // TODO: CleverTap-pushEvent: "update_user_info"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("update_user_info",
            {
                {"click_type", cocos2d::Value("name")},
                {"update_type", cocos2d::Value(ap::Database::getInstance().getUserInfo()._name.length() ? "update" : "add")},
                {"email_verified", cocos2d::Value("none")},
                {"update_info_successfull", cocos2d::Value("no")},
                {"source_update", cocos2d::Value("organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            APPopupBase::showErrorAlert(msg);
        });
    }
}

void APAccountUserInfo::onUpdateEmailClicked(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    if(isShowUpgradePopup())
    {
        return;
    }
    
    // chưa có password
    if (MJDEFAULT->getBoolForKey("need_to_fill_password", true))
    {
        auto screen_addPassword = APAccountAddPassword::createView(DEF_APACCOUNT_FLOW::UPDATE_EMAIL);
        screen_addPassword->setAccountBackendLayer(this);
        Director::getInstance()->getRunningScene()->addChild(screen_addPassword);
    }
    // đã có password
    else
    {
        auto screen_confirmPassword = APAccountConfirmPassword::createView(DEF_APACCOUNT_FLOW::UPDATE_EMAIL);
        screen_confirmPassword->setAccountBackendLayer(this);
        Director::getInstance()->getRunningScene()->addChild(screen_confirmPassword, INT_MAX-1);
    }
}

void APAccountUserInfo::onAddPhoneClicked(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    if(isShowUpgradePopup())
    {
        return;
    }
    
    HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

    // chưa có password
    if (MJDEFAULT->getBoolForKey("need_to_fill_password", true))
    {
        auto screen_addPassword = APAccountAddPassword::createView(DEF_APACCOUNT_FLOW::UPDATE_PHONE);
        screen_addPassword->setAccountBackendLayer(this);
        Director::getInstance()->getRunningScene()->addChild(screen_addPassword);
    }
    // đã có password
    else
    {
        auto screen_confirmPassword = APAccountConfirmPassword::createView(DEF_APACCOUNT_FLOW::UPDATE_PHONE);
        screen_confirmPassword->setAccountBackendLayer(this);
        Director::getInstance()->getRunningScene()->addChild(screen_confirmPassword, INT_MAX-1);
    }
}

void APAccountUserInfo::onUpdatePasswordClicked(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    // chưa có mật khẩu tài khoản
    if (MJDEFAULT->getBoolForKey("need_to_fill_password", true))
    {
        auto screen_addPassword = APAccountAddPassword::createView();
        screen_addPassword->setAccountBackendLayer(this);
        Director::getInstance()->getRunningScene()->addChild(screen_addPassword);
    }
    // đã có mật khẩu tài khoản
    else
    {
        auto screen_updatePassword = APAccountUpdatePassword::createView();
        Director::getInstance()->getRunningScene()->addChild(screen_updatePassword);
    }
}

void APAccountUserInfo::onViewProfilesClicked(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_name);
    
    auto profile_detail = APAccountProfileDetail::createPage();
    profile_detail->setParentDelegate(this);
    profile_detail->adjustWidth(this->getContentSize().width);
    this->addChild(profile_detail, INT_MAX - 1);
}

bool APAccountUserInfo::isShowUpgradePopup()
{
    if (MJDEFAULT->getBoolForKey("is_skip_login", false))
    {
        AccountPopup::show({
            LANGUAGE_MANAGER->getDisplayTextByKey("key.upgrade.account.popup.msg"),
            LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.back"),
            LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title") },NULL, [this]() {
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
                auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event) {
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
        
        return true;
    }
    
    return false;
}




