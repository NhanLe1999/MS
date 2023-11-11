//
//  APAccountUpdatePhone.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/25/20.
//

#include "APAccountUpdatePhone.h"
#include "StoryLanguageManager.h"
#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "APAccountUserInfo.h"
#include "HelperManager.h"
#include "APUpdatePasswordAPI.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountUpdatePhone);

std::once_flag v3_ap_account_update_phone_reader_flag;

#pragma mark - Initialize
APAccountUpdatePhone * APAccountUpdatePhone::createView()
{
    std::call_once(v3_ap_account_update_phone_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountUpdatePhone);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountUpdatePhone.csb");
    auto p = static_cast<APAccountUpdatePhone *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB())
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountUpdatePhone::onExit()
{
    hideVirtualKeyboard(m_textfield_phone);
    
    APPopupBase::onExit();
}

bool APAccountUpdatePhone::didLoadFromCSB()
{
    this->setName(name_screen_apaccount_update_phone);
    
    m_state = APAccountUpdatePhone::State::NONE;
    
    auto user_info = ap::Database::getInstance().getUserInfo();
    m_phone_old = user_info._phone;
    
    // phone country code và country id
    std::string phone_code  = MJDEFAULT->getStringForKey("phonecode_code", "+84");
    std::string phone_id    = MJDEFAULT->getStringForKey("phonecode_id", "VN");
    
    m_text_numbercode = utils::findChild<ui::Text *>(this, "numbercode_text");
    m_text_countrycode = utils::findChild<ui::Text *>(this, "countrycode_text");
    m_button_country = utils::findChild<ui::Button *>(this, "btn_country");
    m_button_update = utils::findChild<ui::Button *>(this, "btn_update");
    m_textfield_phone = utils::findChild<ui::TextField *>(this, "phone_textfield");
    
    // country & number codes
    m_text_countrycode->setFontName(DEF_FONT_FILE_PATH);
    m_text_countrycode->setString(phone_id);
    
    m_text_numbercode->setFontName(DEF_FONT_FILE_PATH);
    m_text_numbercode->setString(phone_code);
    
    // button update
    m_button_update->setEnabled(false);
    m_button_update->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_update->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.save"));
    m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // title & desc
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setFontName(DEF_FONT_FILE_PATH);
    title->setString(DEF_GET_TEXT("key.accountv3.screen.updatephone.title"));
    
    auto desc = utils::findChild<ui::Text *>(this, "desc");
    desc->setFontName(DEF_FONT_FILE_PATH);
    desc->setString(DEF_GET_TEXT("key.accountv3.screen.updatephone.desc") + "\n" + DEF_GET_TEXT("key.accountv3.text.have.new.way.to.signin"));
    
    // textfield phone
    m_textfield_phone->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_phone->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatephone.placeholder"));
    m_textfield_phone->setCursorChar('|');
    m_textfield_phone->setCursorEnabled(true);
    m_textfield_phone->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_phone->setTextColor(Color4B::BLACK);
    m_textfield_phone->setMaxLengthEnabled(true);
    m_textfield_phone->setMaxLength(DEF_PHONE_MAX_LENGTH);
    
    m_textfield_phone->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_phone->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                this->moveUpForm();
            }
                break;
                
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_phone->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_phone);
                this->moveDownForm();
            }
                break;
#endif
            case TextField::EventType::INSERT_TEXT:
            {
                validPhoneInput(m_textfield_phone);
                this->enableWidgetNodeBasedOnTextLength(m_textfield_phone->getString(), DEF_PHONE_MIN_LENGTH, m_button_update);
            }
                break;
                
            case TextField::EventType::DELETE_BACKWARD:
            {
                validPhoneInput(m_textfield_phone);
                this->enableWidgetNodeBasedOnTextLength(m_textfield_phone->getString(), DEF_PHONE_MIN_LENGTH, m_button_update);
            }
                break;
                
            default:
                break;
        }
    });
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        title->setTextColor(Color4B::WHITE);
        desc->setTextColor(Color4B::WHITE);
        
        if (auto background = utils::findChild<ui::Layout *>(this, "background"))
        {
            background->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
        }
        
        if (auto button_back = cocos2d::utils::findChild<ui::Button *>(this, "btn_back"))
        {
            button_back->loadTextureNormal("account/new_v3_vn/button_back_left.png");
            button_back->loadTexturePressed("account/new_v3_vn/button_back_left.png");
            button_back->resetNormalRender();
            button_back->resetPressedRender();
            button_back->resetDisabledRender();
            button_back->getRendererNormal()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererClicked()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererDisabled()->setPosition(button_back->getContentSize()/2);
        }
        
        m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->effectShowing();
    
    return true;
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountUpdatePhone::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountUpdatePhone, onUpdate),
        CLICK_MAP(APAccountUpdatePhone, onBack),
        CLICK_MAP(APAccountUpdatePhone, onCountryCodeClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountUpdatePhone::onUpdate(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_phone);
    
    auto phoneNumber = m_textfield_phone->getString();
    
    if (phoneNumber.empty())
    {
        CCLOG("ERROR: m_textfield_phone is empty!");
    }
    else if (!mj::helper::isDigitString(phoneNumber))
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("key.invalid.phone.number"));
    }
    else if (phoneNumber.compare(m_phone_old) == 0)
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("key.invalid.phone.number"));
    }
    else
    {
        if (std::string(phoneNumber.begin(), phoneNumber.begin() + 1).compare("0") == 0) {
            phoneNumber = std::string(phoneNumber.begin() + 1, phoneNumber.end());
        }
        
        m_state = APAccountUpdatePhone::State::NONE;
        m_phone = m_text_numbercode->getString() + phoneNumber;
        
        m_button_update->setEnabled(false);
        
        // TODO: call API: UpdateUserInfo
        ap::api::UpdateUserInfo::call("", m_phone, true, CC_CALLBACK_2(APAccountUpdatePhone::onUpdateUserInfoAPICalled, this));
    }
}

void APAccountUpdatePhone::onUpdateUserInfoAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        m_state = APAccountUpdatePhone::State::PHONE_UPDATE_SUCCESS;
        
        // TODO: Save phone number to database
        auto user_info = ap::Database::getInstance().getUserInfo();
        ap::Database::getInstance().saveUserInfo(std::to_string(user_info._id), user_info._name, user_info._mail, m_phone, user_info._address, user_info._userName);
        
        // TODO: CleverTap-pushEvent: "update_user_info"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("update_user_info",
            {
                {"click_type", cocos2d::Value("phone")},
                {"update_type", cocos2d::Value(user_info._phone.length() ? "update" : "add")},
                {"email_verified", cocos2d::Value("none")},
                {"update_info_successfull", cocos2d::Value("yes")},
                {"source_update", cocos2d::Value("organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, user_info]
        {
            if (m_account_backend)
            {
//                m_account_backend->callAPILoadUpdateAndResetScreen();
                m_account_backend->resetAccountInfoScreen();
            }
            
            if (MJDEFAULT->getBoolForKey("need_to_fill_password"))
            {
                auto password = MJDEFAULT->getStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD);
                
                if (password.empty()) {
                    CCLOG("screen APAccountUpdatePhone: WTF???");
                    this->removeFromParentWithEffect();
                    return;
                }
                
                // TODO: call API: UpdatePassword
                ap::api::UpdatePassword::call("", password, CC_CALLBACK_2(APAccountUpdatePhone::onUpdatePasswordAPICalled, this));
                
                this->runAction(FadeOut::create(0.25f));
            }
            else
            {
                // Cập nhật thành công thì cho tắt màn hình luôn
                this->removeFromParentWithEffect();
            }
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            m_button_update->setEnabled(true);
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}

void APAccountUpdatePhone::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([is_success, msg, this]
    {
        if (is_success)
        {
            // TODO: Save value for key "need_to_fill_password"
            MJDEFAULT->setBoolForKey("need_to_fill_password", false);
            MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, "");
            
            if (m_account_backend)
            {
                m_account_backend->resetAccountInfoScreen();
            }
        }
        else
        {
            APPopupBase::showErrorAlert(msg);
        }
        
        this->removeFromParent();
    });
}

void APAccountUpdatePhone::removeFromParentWithEffect()
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this]
    {
        this->removeFromParent();
    }), NULL));
}

void APAccountUpdatePhone::onBack(cocos2d::Ref * sender)
{
    if (m_state != APAccountUpdatePhone::State::PHONE_UPDATE_SUCCESS)
    {
        // TODO: CleverTap-pushEvent: "update_user_info"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("update_user_info",
            {
                {"click_type", cocos2d::Value("phone")},
                {"update_type", cocos2d::Value(ap::Database::getInstance().getUserInfo()._phone.length() ? "update" : "add")},
                {"email_verified", cocos2d::Value("none")},
                {"update_info_successfull", cocos2d::Value("no")},
                {"source_update", cocos2d::Value("organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
    
    this->removeFromParentWithEffect();
}

void APAccountUpdatePhone::onCountryCodeClicked(cocos2d::Ref * sender)
{
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));
    
    auto popup = APPhoneCodePopup::createView();
    popup->setDelegate(this);
    this->addChild(popup, POPUP_ZORDER);
}

void APAccountUpdatePhone::onPhoneCodeItemSelected(PhoneCodeInfo i_info)
{
    m_text_countrycode->setString(i_info.id);
    m_text_numbercode->setString(i_info.code);
}
