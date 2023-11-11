//
//  APEnterEmailPopup.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/18/20.
//

#include "APEnterEmailPopup.h"
#include "StoryLanguageManager.h"
#include "GettingUserInfoManager.h"
#include "MJHelper.h"
#include "APUpdateEmailAPI.h"
#include "APAccountAddPassword.h"
#include "APAccountConfirmPassword.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APEnterEmailPopup);

std::once_flag v3_ap_enter_email_popup_reader_flag;

#pragma mark - Initialize
APEnterEmailPopup * APEnterEmailPopup::createPopup()
{
    std::call_once(v3_ap_enter_email_popup_reader_flag, []
    {
        REGISTER_CSB_READER(APEnterEmailPopup);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APEnterEmailPopup.csb");
    auto p = static_cast<APEnterEmailPopup *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB())
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APEnterEmailPopup::onExit()
{
    this->hideVirtualKeyboard(m_textfield_email);
    
    APPopupBase::onExit();
}

bool APEnterEmailPopup::didLoadFromCSB()
{
    this->setName(name_screen_enter_email_popup);
    
    // button ok
    m_button_ok = utils::findChild<ui::Button *>(this, "bt_ok");
    m_button_ok->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_ok->setTitleText(DEF_GET_TEXT("key.save"));
    m_button_ok->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    this->setEnabledButtonOk(false);
    
    // label notice
    m_text_warning = utils::findChild<ui::Text *>(this, "label_notice");
    m_text_warning->setFontName(DEF_FONT_FILE_PATH);
    
    // email title & message
    auto email = utils::findChild<ui::Text *>(this, "label_email");
    email->setFontName(DEF_FONT_FILE_PATH);
    email->setString(DEF_GET_TEXT("key.popup.user.info.email") + " (*)");
    
    auto message = utils::findChild<ui::Text *>(this, "label_message");
    message->setFontName(DEF_FONT_FILE_PATH);
    message->setString(DEF_GET_TEXT("key.accountv3.popup.enteremail.desc"));
    
    // textfield email
    m_textfield_email = utils::findChild<ui::TextField *>(this, "textfield_email_text");
    m_textfield_email->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_email->setPlaceHolder(DEF_GET_TEXT("key.enter.email"));
    m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_email->setTextColor(Color4B::BLACK);
    m_textfield_email->setCursorChar('|');
    m_textfield_email->setCursorEnabled(true);
    m_textfield_email->setMaxLengthEnabled(true);
    m_textfield_email->setMaxLength(100);
    
    m_textfield_email->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                utils::findChild<ui::Layout *>(this, "alert_layout")->runAction(MoveBy::create(0.25, Vec2(0, 225)));
            }
                break;
                
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_email);
                utils::findChild<ui::Layout *>(this, "alert_layout")->runAction(MoveBy::create(0.25, Vec2(0, -225)));
            }
                break;
#endif
            case TextField::EventType::INSERT_TEXT:
            case TextField::EventType::DELETE_BACKWARD:
            {
                this->trimSpace(m_textfield_email);
                m_textfield_email->getString().empty() ? this->setEnabledButtonOk(false) : this->setEnabledButtonOk(true);
            }
                break;
                
            default:
                break;
        }
    });
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        message->setString(DEF_GET_TEXT("key.accountv3.popup.enteremail.desc.vm"));
        
        m_button_ok->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    // ẩn warning
    this->hideWarning();
    
    return true;
}

void APEnterEmailPopup::setEnabledButtonOk(bool enabled)
{
    m_button_ok->setEnabled(enabled);
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        if (auto mask = cocos2d::utils::findChild<ui::ImageView *>(m_button_ok, "bt_ok_mask"))
        {
            if (enabled)
            {
                mask->setVisible(false);
            }
            else
            {
                mask->setOpacity(128);
                mask->setVisible(true);
            }
        }
    }
}

void APEnterEmailPopup::setDelegate(APEnterEmailPopupDelegate * delegate)
{
    m_delegate = delegate;
}

void APEnterEmailPopup::showPopup()
{
    if (m_isRunning)
    {
        return;
    }
    
    m_isRunning = true;
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APEnterEmailPopup.csb");
    auto runani = CSLoader::createTimeline(csb_name);
    this->runAction(runani);
    
    runani->play("show", false);
    
    runani->setLastFrameCallFunc([=]{
        m_isRunning = false;
    });
}

void APEnterEmailPopup::hidePopup()
{
    if (m_isRunning)
    {
        return;
    }
    
    m_isRunning = true;
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APEnterEmailPopup.csb");
    auto runani = CSLoader::createTimeline(csb_name);
    this->runAction(runani);
    
    runani->play("hide", false);
    
    runani->setLastFrameCallFunc([=]{
        m_isRunning = false;
        this->removeFromParent();
    });
}

#pragma mark - Zzz
void APEnterEmailPopup::showWarning(const std::string& warningText)
{
    utils::findChild(this, "img_notice")->setVisible(true);
    
    m_text_warning->setVisible(true);
    m_text_warning->setString(warningText.length() ? warningText : " ");
}

void APEnterEmailPopup::hideWarning()
{
    utils::findChild(this, "img_notice")->setVisible(false);
    m_text_warning->setVisible(false);
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APEnterEmailPopup::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APEnterEmailPopup, onOk),
        CLICK_MAP(APEnterEmailPopup, onCancel),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APEnterEmailPopup::onCancel(cocos2d::Ref * sender)
{
    GettingUserInfoManager::GetInstance()->increaseRemindingUpdateInfoPopupCount();
    
    // TODO: CleverTap-pushEvent: "get_user_info_click_popup"
    try
    {
        CleverTapManager::GetInstance()->pushEvent("get_user_info_click_popup",
        {
            {"click_type", cocos2d::Value("exit")},
            {"has_password", cocos2d::Value(MJDEFAULT->getBoolForKey("need_to_fill_password", true) ? "no" : "yes")},
        });
    }
    catch (const std::exception&)
    {
        
    }
    
    this->hidePopup();
}

void APEnterEmailPopup::onOk(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_email);
    
    std::string email_address = m_textfield_email->getString();
    
    if (email_address.empty())
    {
        this->showWarning(DEF_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
    }
    else if (!mj::helper::isEmailAddressValid(email_address))
    {
        this->showWarning(DEF_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
    }
    else
    {
        this->setEnabledButtonOk(false);
        m_textfield_email->setEnabled(false);
        
        this->hideWarning();
        
        // MARK: call API: UpdateEmail
        ap::api::UpdateEmail::call(email_address, CC_CALLBACK_2(APEnterEmailPopup::onUpdateEmailAPICalled, this));
    }
}

void APEnterEmailPopup::onUpdateEmailAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            MJDEFAULT->setStringForKey(DEF_TEMP_EMAIL_FOR_ENTER_INFO_THREAD, m_textfield_email->getString());
            this->setVisible(false);
            
            auto alert = StoryAlert::showAlert(DEF_GET_TEXT("key.accountv3.popup.enteremail.thankyou"), "OK", " ");
            alert->setDelegate(this);
        });
        
        // TODO: CleverTap-pushEvent: "get_user_info_click_popup"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("get_user_info_click_popup",
            {
                {"click_type", cocos2d::Value("save")},
                {"has_password", cocos2d::Value(MJDEFAULT->getBoolForKey("need_to_fill_password", true) ? "no" : "yes")},
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            this->setEnabledButtonOk(true);
            m_textfield_email->setEnabled(true);
            
            this->showWarning(msg.length() ? msg : DEF_GET_TEXT("parent.common.error"));
        });
    }
}

#pragma mark - StoryAlertDelegate
void APEnterEmailPopup::onAlertOK(cocos2d::Ref * sender)
{
    // chưa có password
    if (MJDEFAULT->getBoolForKey("need_to_fill_password", true))
    {
        auto addPasswordScreen = APAccountAddPassword::createView(DEF_APACCOUNT_FLOW::ENTER_INFO);
        Director::getInstance()->getRunningScene()->addChild(addPasswordScreen, INT_MAX-1);
    }
    // đã có password
    else
    {
        auto confirmPasswordScreen = APAccountConfirmPassword::createView(DEF_APACCOUNT_FLOW::ENTER_INFO);
        Director::getInstance()->getRunningScene()->addChild(confirmPasswordScreen, INT_MAX-1);
    }
    
    this->removeFromParent();
}

void APEnterEmailPopup::onAlertCancel(cocos2d::Ref * sender)
{
    this->removeFromParent();
}
