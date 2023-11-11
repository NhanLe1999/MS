//
//  MSMaterialEnterEmail.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 11/17/20.
//

#include "MSMaterialEnterEmail.h"
#include "MJHelper.h"
#include "APUpdateEmailMaterialAPI.h"
#include "APSendEmailMaterialAPI.h"
#include "APDatabase.h"
#include "MSTabMaterial.h"
#include "MSSidebar.h"
#include "CleverTapManager.h"

#define MATERIAL_ENTER_EMAIL_CSB_PATH           "csb/new_ui/material/MaterialEnterEmail.csb"
#define MATERIAL_ENTER_EMAIL_FADE_DURATION      0.25f

INITIALIZE_READER(MSMaterialEnterEmail);

std::once_flag mat_material_enter_email_reader_flag;

#pragma mark - Initialize
MSMaterialEnterEmail * MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type type, MSMaterialEnterEmailDelegate * delegate)
{
    std::call_once(mat_material_enter_email_reader_flag, []
    {
        REGISTER_CSB_READER(MSMaterialEnterEmail);
    });
    
    auto p = static_cast<MSMaterialEnterEmail *>(CSLoader::createNodeWithVisibleSize(MATERIAL_ENTER_EMAIL_CSB_PATH));
    
    if (p && p->didLoadFromCSB(type, delegate))
    {
        return p;
    }
    
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

std::string MSMaterialEnterEmail::getCurrentMaterialEmail() {
    auto user_info = ap::Database::getInstance().getUserInfo();
    auto email_material = MJDEFAULT->getStringForKey(StringUtils::format(key_email_material_x, user_info._id), "");
    std::string email_address = email_material;
    
    if (email_address.empty()) {
        email_address = user_info._mail;
    }
    
    return email_address;
}

void MSMaterialEnterEmail::onExit()
{
    this->hideVirtualKeyboard(m_email_textfield);
    
    APPopupBase::onExit();
}

void MSMaterialEnterEmail::showPopup()
{
    if (m_isRunning)
    {
        return;
    }
    
    m_isRunning = true;
    
    auto csb_name = mj::helper::getCSBName(MATERIAL_ENTER_EMAIL_CSB_PATH);
    auto runani = CSLoader::createTimeline(csb_name);
    this->runAction(runani);
    
    runani->play("show", false);
    
    runani->setLastFrameCallFunc([=]{
        m_isRunning = false;
    });
}

void MSMaterialEnterEmail::hidePopup()
{
    if (m_isRunning)
    {
        return;
    }
    
    m_isRunning = true;
    
    auto csb_name = mj::helper::getCSBName(MATERIAL_ENTER_EMAIL_CSB_PATH);
    auto runani = CSLoader::createTimeline(csb_name);
    this->runAction(runani);
    
    runani->play("hide", false);
    
    runani->setLastFrameCallFunc([=]{
        m_isRunning = false;
        this->removeFromParent();
    });
}

cocos2d::ui::Widget::ccWidgetClickCallback MSMaterialEnterEmail::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSMaterialEnterEmail, onOK),
        CLICK_MAP(MSMaterialEnterEmail, onCancel),
        CLICK_MAP(MSMaterialEnterEmail, onEditEmail),
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

bool MSMaterialEnterEmail::didLoadFromCSB(MSMaterialEnterEmail::Type type, MSMaterialEnterEmailDelegate * delegate)
{
    m_type = type;
    m_delegate = delegate;
    m_hasEmail = MSMaterialEnterEmail::getCurrentMaterialEmail().empty() ? false : true;
    
    if (auto alert_bg = cocos2d::utils::findChild(this, "alert_bg"))
    {
        m_email_background = cocos2d::utils::findChild<ui::ImageView *>(alert_bg, "email_bg");
        m_email_textfield = cocos2d::utils::findChild<ui::TextField *>(alert_bg, "email_textfield");
        m_warning_icon = cocos2d::utils::findChild<ui::ImageView *>(alert_bg, "warning_icon");
        m_warning_text = cocos2d::utils::findChild<ui::Text *>(alert_bg, "warning_text");
        m_desc = cocos2d::utils::findChild<ui::Text *>(alert_bg, "desc_1");
        m_button_ok = cocos2d::utils::findChild<ui::Button *>(alert_bg, "btok");
        m_button_edit = cocos2d::utils::findChild<ui::Button *>(alert_bg, "btedit");
        m_button_cancel = cocos2d::utils::findChild<ui::Button *>(this, "btcancel");
        
        // desc
        m_desc->setFontName(MSMAT_FONT_FILE_PATH);
        
        // button OK
        m_button_ok->setTitleFontName(MSMAT_FONT_FILE_PATH);
        m_button_ok->setTitleText(MSMAT_GET_TEXT("key.save"));
        m_button_ok->getRendererDisabled()->setOpacity(160);
        
        // warning
        m_warning_icon->setVisible(false);
        m_warning_text->setVisible(false);
        m_warning_text->setFontName(MSMAT_FONT_FILE_PATH);
        
        // email textfield
        m_email_textfield->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        m_email_textfield->setPlaceHolder(MSMAT_GET_TEXT("key.enter.email"));
        m_email_textfield->setPlaceHolderColor(Color4B(0, 0, 0, 64));
        m_email_textfield->setTextColor(Color4B(41, 177, 222, 255));
        m_email_textfield->setCursorChar('|');
        m_email_textfield->setCursorEnabled(true);
        m_email_textfield->setMaxLengthEnabled(true);
        m_email_textfield->setMaxLength(100);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        registerPressCtrlEventForTextField(m_email_textfield, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

        scrollView = cocos2d::utils::findChild<ui::ScrollView*>(this, "ScrollView_2");

        m_email_textfield->addEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
            {
                bool isIOS = (CC_TARGET_PLATFORM == CC_PLATFORM_IOS);
                switch (type)
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                case TextField::EventType::ATTACH_WITH_IME:
                {
                    m_email_textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                    utils::findChild<ui::Layout*>(this, "alert_layout")->runAction(MoveBy::create(0.25, Vec2(0, isIOS ? 225 : 300)));
                }
                break;

                case TextField::EventType::DETACH_WITH_IME:
                {
                    m_email_textfield->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                    this->removeSpace(m_email_textfield);
                    utils::findChild<ui::Layout*>(this, "alert_layout")->runAction(MoveBy::create(0.25, Vec2(0, isIOS ? -225 : -300)));

                    if (m_hasEmail && m_email_textfield->getString().empty())
                    {
                        m_email_textfield->setString(MSMaterialEnterEmail::getCurrentMaterialEmail());
                        m_email_textfield->setEnabled(false);
                        m_button_edit->setVisible(true);
    }
}
                break;
#endif
                    }
                textFieldUnlimited(m_email_textfield, type, scrollView, "Montserrat-Bold.ttf", 22, nullptr);
                });

        if (m_type == MSMaterialEnterEmail::Type::ENTER_EMAIL)
        {
            m_desc->setString(MSMAT_GET_TEXT("key.material.popup.enter.email"));
            m_button_ok->setTitleText(MSMAT_GET_TEXT("key.material.button.register"));
        }
        else if (m_type == MSMaterialEnterEmail::Type::SEND_MATERIAL)
        {
            std::string email_address = MSMaterialEnterEmail::getCurrentMaterialEmail();
            
            if (email_address.length())    /* đã có email học liệu */
            {
                m_emailType = MSMaterialEnterEmail::EmailType::NONE;
                m_button_edit->setVisible(true);
                m_button_ok->setTitleText(MSMAT_GET_TEXT("key.material.button.confirm"));
                m_desc->setString(MSMAT_GET_TEXT("key.material.confirm.email.to.send"));
                m_email_textfield->setEnabled(false);
                m_email_textfield->setString(email_address);
            }
            else                            /* chưa có email học liệu */
            {
                m_emailType = MSMaterialEnterEmail::EmailType::NEW;
                m_button_edit->setVisible(false);
                m_button_ok->setTitleText(MSMAT_GET_TEXT("key.material.button.confirm"));
                m_desc->setString(MSMAT_GET_TEXT("key.material.popup.enter.email"));
                m_email_textfield->setEnabled(true);
                m_email_textfield->setString("");
            }
        }
    }
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    ui::Helper::doLayout(this);
    
    return true;
}

void MSMaterialEnterEmail::showWarning(const std::string& text)
{
    m_warning_text->setString(text.empty() ? MSMAT_GET_TEXT("parent.common.error") : text);
    m_warning_text->setVisible(true);
    m_warning_text->stopAllActions();
    m_warning_text->runAction(FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION));
    
    m_warning_icon->setVisible(true);
    m_warning_icon->stopAllActions();
    m_warning_icon->runAction(FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION));
}

void MSMaterialEnterEmail::hideWarning()
{
    if (m_warning_text->isVisible())
    {
        m_warning_text->stopAllActions();
        m_warning_text->runAction(Sequence::create(FadeOut::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), CallFunc::create([=](){m_warning_text->setVisible(false);}), NULL));
        m_warning_icon->stopAllActions();
        m_warning_icon->runAction(Sequence::create(FadeOut::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), CallFunc::create([=](){m_warning_icon->setVisible(false);}), NULL));
    }
}

void MSMaterialEnterEmail::showPopupNote(ui::Widget * contentNode)
{
    m_type = MSMaterialEnterEmail::Type::POPUP_NOTE;
    
    m_button_ok->setEnabled(true);
    m_button_ok->setTitleText(MSMAT_GET_TEXT("viral.gift.button"));
    m_button_cancel->setVisible(false);
    
    m_email_background->setVisible(false);
    m_email_textfield->setVisible(false);
    m_desc->setVisible(false);
    
    if (auto popup_note = utils::findChild(this, "popup_note")) {
        bool is_phonics = m_delegate ? (m_delegate->fromMaterialScreen() == mj::model::MaterialInfo::Type::PHONICS ? true : false) : false;
        popup_note->setVisible(true);
        
        if (auto text_title = utils::findChild<ui::Text *>(popup_note, "title")) {
            text_title->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
            text_title->setString(MSMAT_GET_TEXT("key.material.note.title"));
        }
        
        // truyện/bài học đã học
        if (auto text_downloaded_story = utils::findChild<ui::Text *>(popup_note, "txt_down_story")) {
            text_downloaded_story->setFontName(MSMAT_FONT_FILE_PATH);
            text_downloaded_story->setString(is_phonics ? MSMAT_GET_TEXT("key.material.lesson.learned") : MSMAT_GET_TEXT("key.material.story.readed"));
            
            if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                text_downloaded_story->setFontSize(17);
            }
            
            // không dùng text và icon này nữa
            text_downloaded_story->getParent()->setVisible(false);
        }
        
        // học liệu đã tải
        if (auto text_downloaded_ws = utils::findChild<ui::Text *>(popup_note, "txt_down_ws")) {
            text_downloaded_ws->setFontName(MSMAT_FONT_FILE_PATH);
            text_downloaded_ws->setString(MSMAT_GET_TEXT("key.material.worksheet.downloaded"));
            
            if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                text_downloaded_ws->setFontSize(17);
            }
            
            // icon
            if (auto icon = utils::findChild<ui::ImageView *>(popup_note, "Image_3")) {
                icon->setPosition(Point(-icon->getContentSize().width/2 - 1.f, text_downloaded_ws->getContentSize().height/2));
            }
            
            // không dùng text và icon này nữa
            text_downloaded_ws->setVisible(false);
        }
        
        if (contentNode) {
            contentNode->setScale(1.22f);
            contentNode->setPosition(Point(-contentNode->getContentSize().width/2 - 32.f + MJ_PLATFORM_CONFIG.getNotchHeight()/4, -13) + popup_note->getContentSize()/2);
            popup_note->addChild(contentNode);
        }
        
        if (auto text_desc = utils::findChild<ui::Text *>(popup_note, "txt_desc")) {
            text_desc->setFontName(MSMAT_FONT_FILE_PATH);
            
            if (m_delegate && m_delegate->fromMaterialScreen() == mj::model::MaterialInfo::Type::PHONICS) {
                text_desc->setString(MSMAT_GET_TEXT("key.material.note.desc.2"));
            } else {
                text_desc->setString(MSMAT_GET_TEXT("key.material.note.desc"));
            }
            
            if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                text_desc->setFontSize(18);
            }
        }
    }
}

void MSMaterialEnterEmail::showPopupFreeLimited(const std::string& mess)
{
    m_type = MSMaterialEnterEmail::Type::POPUP_FREE_LIMITED;
    
    m_button_ok->setTitleText(MSMAT_GET_TEXT("sub.popup.btsub"));
    m_button_ok->setTitleFontSize(m_button_ok->getTitleFontSize() * 0.88f);
    m_button_ok->setEnabled(true);
    m_button_ok->setOpacity(0);
    m_button_ok->runAction(FadeIn::create(0.3f));
    
    m_button_cancel->setVisible(true);
    m_button_edit->setVisible(false);
    
    m_email_background->setVisible(false);
    m_email_textfield->setVisible(false);
    m_desc->setVisible(false);
    
    if (auto popup = utils::findChild(this, "popup_free_limit"))
    {
        popup->setVisible(true);
        popup->setOpacity(0);
        popup->runAction(FadeIn::create(0.3f));
        
        if (auto desc = utils::findChild<ui::Text *>(popup, "desc"))
        {
            desc->setFontName(MSMAT_FONT_FILE_PATH);
            desc->setString(mess);
        }
    }
}

void MSMaterialEnterEmail::showNotice(const std::string& mess)
{
    m_type = MSMaterialEnterEmail::Type::NOTICE;
    m_button_ok->setTitleText(MSMAT_GET_TEXT("viral.gift.button"));
    m_button_cancel->setVisible(false);
    
    m_email_background->setVisible(false);
    m_email_textfield->setVisible(false);
    m_desc->setString(mess);
    m_desc->setPosition(Point(m_desc->getPosition().x, 268));
}

void MSMaterialEnterEmail::showSuccessMessage(const std::string& text_top, const std::string& text_email, const std::string& text_bottom)
{
    m_isSuccess = true;
    
    // SHOW SUCCESSFUL MESSAGE
    // button
    m_button_ok->setEnabled(true);
    m_button_ok->setTitleText(MSMAT_GET_TEXT("viral.gift.button"));
    m_button_edit->setVisible(false);
    m_button_cancel->setVisible(false);
    
    // email, desc, warning
    m_desc->runAction(Sequence::create(FadeOut::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), CallFunc::create([=](){m_desc->setVisible(false);}), NULL));
    m_email_background->runAction(Sequence::create(FadeOut::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), CallFunc::create([=](){m_email_background->setVisible(false);}), NULL));
    m_email_textfield->runAction(Sequence::create(FadeOut::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), CallFunc::create([=](){m_email_textfield->setVisible(false);}), NULL));
    this->hideWarning();
    
    // texts
    if (text_top.length() && text_email.length() && text_bottom.length())
    {
        auto desc_1 = cocos2d::utils::findChild<ui::Text *>(this, "desc_2_1");
        desc_1->setFontName(MSMAT_FONT_FILE_PATH);
        desc_1->setString(text_top);
        desc_1->setOpacity(0);
        desc_1->setVisible(true);
        desc_1->runAction(Sequence::create(DelayTime::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), NULL));
        
        auto desc_2 = cocos2d::utils::findChild<ui::Text *>(this, "desc_2_2");
        desc_2->setFontName(MSMAT_FONT_FILE_PATH);
        desc_2->setString(text_email);
        desc_2->setOpacity(0);
        desc_2->setVisible(true);
        desc_2->runAction(Sequence::create(DelayTime::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), NULL));
        
        auto desc_3 = cocos2d::utils::findChild<ui::Text *>(this, "desc_2_3");
        desc_3->setFontName(MSMAT_FONT_FILE_PATH);
        desc_3->setString(text_bottom);
        desc_3->setOpacity(0);
        desc_3->setVisible(true);
        desc_3->runAction(Sequence::create(DelayTime::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), NULL));
        
        desc_2->setPosition(Point(desc_2->getParent()->getContentSize().width/2, 268));
        desc_1->setPosition(desc_2->getPosition() + Point(0, desc_2->getContentSize().height/2));
        desc_3->setPosition(desc_2->getPosition() - Point(0, desc_2->getContentSize().height/2));
    }
    else
    {
        auto desc_2 = cocos2d::utils::findChild<ui::Text *>(this, "desc_2_2");
        desc_2->setFontName(MSMAT_FONT_FILE_PATH);
        desc_2->setTextAreaSize(Size(480, 120));
        desc_2->setString(text_top);
        desc_2->setPosition(Point(desc_2->getParent()->getContentSize().width/2, 268));
        desc_2->setVisible(true);
        desc_2->setOpacity(0);
        desc_2->runAction(Sequence::create(DelayTime::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), FadeIn::create(MATERIAL_ENTER_EMAIL_FADE_DURATION), NULL));
    }
}

#pragma mark - CleverTap events
void MSMaterialEnterEmail::pushCleverTapEvent_emailSetting(bool is_success)
{
    try
    {
        CleverTapManager::GetInstance()->pushEvent("worksheet_email_setting",
        {
            {"is_success", cocos2d::Value(is_success ? "yes" : "no")},
        });
    }
    catch (const std::exception&)
    {
        // nothing here
    }
}

void MSMaterialEnterEmail::pushCleverTapEvent_confirmEmail(const std::string& status, const std::string& type, bool is_success, bool close_popup, const std::string& failReason)
{
    std::vector<std::pair<std::string, cocos2d::Value>> eventProperties;
    if (is_success) {
        eventProperties = {
            {"status", cocos2d::Value(status)},
            {"type", cocos2d::Value(type)},
            {"is_success", cocos2d::Value(is_success ? "yes" : "no")},
            {"close_popup", cocos2d::Value(close_popup ? "yes" : "no")}            
        };
    }
    else {
        eventProperties = {
            {"status", cocos2d::Value(status)},
            {"type", cocos2d::Value(type)},
            {"is_success", cocos2d::Value(is_success ? "yes" : "no")},
            {"close_popup", cocos2d::Value(close_popup ? "yes" : "no")},
            {"failed_reason",cocos2d::Value(is_success ? "" : failReason)}
        };
    }

    try
    {
        CleverTapManager::GetInstance()->pushEvent("worksheet_confirm_email",eventProperties);
    }
    catch (const std::exception& ex)
    {
        // nothing here
    }
}

#pragma mark - Handle with Buttons
void MSMaterialEnterEmail::onEditEmail(cocos2d::Ref *ref)
{
    m_email_textfield->setString("");
    m_email_textfield->setEnabled(true);
    m_email_textfield->attachWithIME();
    m_button_edit->setVisible(false);
}
std::string getStatus(MSMaterialEnterEmail::EmailType type) {
    switch (type)
    {
    case MSMaterialEnterEmail::EmailType::NONE:
        return "none";
        break;
    case MSMaterialEnterEmail::EmailType::NEW:
        return "new";
        break;
    case MSMaterialEnterEmail::EmailType::UPDATE:
        return "update";
        break;
    default:
        return "none";
        break;
    }
}
void MSMaterialEnterEmail::onCancel(cocos2d::Ref *ref)
{
    mj::helper::playButtonFXClose();
    switch (m_type) {
        case MSMaterialEnterEmail::Type::ENTER_EMAIL: {
            this->pushCleverTapEvent_emailSetting(false);
        }
            break;
            
        case MSMaterialEnterEmail::Type::SEND_MATERIAL: {
            std::string status = "none";
            if (m_emailType == MSMaterialEnterEmail::EmailType::UPDATE) {
                status = "update";
            } else if (m_emailType == MSMaterialEnterEmail::EmailType::NEW) {
                status = "new";
            }
            this->pushCleverTapEvent_confirmEmail(status, MSMAT_MATERIAL_TYPE_STR(m_delegate->fromMaterialScreen()), false, true, m_failReason);
        }
            break;
        case MSMaterialEnterEmail::Type::POPUP_FREE_LIMITED:
            this->pushCleverTapEvent_confirmEmail(getStatus(m_emailType), MSMAT_MATERIAL_TYPE_STR(m_delegate->fromMaterialScreen()), false, true, m_failReason);
            break;

        default:
            break;
    }
    
    this->hidePopup();
}

void MSMaterialEnterEmail::onOK(cocos2d::Ref *ref)
{
    mj::helper::playButtonFX();
    switch (m_type) {
        case MSMaterialEnterEmail::Type::NOTICE:
            //this->onCancel(ref);
            this->hidePopup();
            break;
        case MSMaterialEnterEmail::Type::POPUP_NOTE: {
            if (m_delegate) m_delegate->pushEventNotation();
            //this->onCancel(ref);
            this->hidePopup();
        }
            break;
            
        case MSMaterialEnterEmail::Type::POPUP_FREE_LIMITED: {
            if (auto sidebar = utils::findChild<MSSidebar *>(Director::getInstance()->getRunningScene(), "msSideBar") ) {
                sidebar->onSubscriptionsForce();
                //this->onCancel(ref);
                this->hidePopup();
            }
        }
            break;
            
        case MSMaterialEnterEmail::Type::ENTER_EMAIL: {
            if (m_isSuccess) {
                //this->onCancel(ref);
                this->hidePopup();
            } else {
                this->handleWithUpdateEmailMaterial(ref);
            }
        }
            break;
            
        case MSMaterialEnterEmail::Type::SEND_MATERIAL: {
            if (m_isSuccess) {
                //this->onCancel(ref);
                this->hidePopup();
                m_delegate ? m_delegate->onMaterialEmailSent() : (void) nullptr;
            } else {
                this->handleWithSendEmailMaterial(ref);
            }
        }
            break;
            
        default:
            break;
    }
}

#pragma mark - Update Material Email
void MSMaterialEnterEmail::handleWithUpdateEmailMaterial(cocos2d::Ref *ref)
{
    this->hideVirtualKeyboard(m_email_textfield);
    
    const std::string& email_address = m_email_textfield->getString();
    
    if (email_address.empty())
    {
        this->showWarning(MSMAT_GET_TEXT("key.material.warning.no.email"));
    }
    else if (!mj::helper::isEmailAddressValid(email_address))
    {
        this->showWarning(MSMAT_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
    }
    else
    {
        m_button_ok->setEnabled(false);
        m_email_textfield->setEnabled(false);
        
        this->hideWarning();
        
        // TODO: call API: UpdateEmail
        m_email = email_address;
        
        auto user_info = ap::Database::getInstance().getUserInfo();
        ap::api::UpdateEmailMaterial::call(m_email, std::to_string(user_info._id), true, CC_CALLBACK_3(MSMaterialEnterEmail::onUpdateEmailMaterialAPICalled, this));
    }
}

void MSMaterialEnterEmail::onUpdateEmailMaterialAPICalled(bool is_success, std::string msg, int code)
{
    m_failReason = mapMessageFromAPIToEventName(msg,code);
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg, code]
    {
        if (is_success)
        {
            // save email
            auto user_info = ap::Database::getInstance().getUserInfo();
            MJDEFAULT->setStringForKey(StringUtils::format(key_email_material_x, user_info._id), m_email);
            
            if (m_delegate)
            {
                m_delegate->onMaterialEmailUpdated();
            }
            
            if (m_type == MSMaterialEnterEmail::Type::ENTER_EMAIL)
            {
                // show message: register successfully
                this->showSuccessMessage(MSMAT_GET_TEXT("key.material.notice.success.register.email"));
                this->pushCleverTapEvent_emailSetting(true);
            }
            else if (m_type == MSMaterialEnterEmail::Type::SEND_MATERIAL)
            {
                m_emailType = m_hasEmail ? MSMaterialEnterEmail::EmailType::UPDATE : MSMaterialEnterEmail::EmailType::NEW;
                
                auto email_address = MSMaterialEnterEmail::getCurrentMaterialEmail();
                
                // TODO: call API SendEmailMaterial
                this->sendEmailMaterial(email_address);
            }
        }
        else
        {
            if (m_type == MSMaterialEnterEmail::Type::ENTER_EMAIL)
            {
                m_button_ok->setEnabled(true);
                m_email_textfield->setEnabled(true);
                this->showWarning(msg.length() ? msg : MSMAT_GET_TEXT("parent.common.error"));
            }
            else if (m_type == MSMaterialEnterEmail::Type::SEND_MATERIAL)
            {
                if (code == 306)    /* trùng email với người khác */
                {
                    m_email_duplicated = m_email;
                    m_button_ok->setTitleText(MSMAT_GET_TEXT("key.material.button.confirm"));
                    m_button_ok->setEnabled(true);
                    m_email_textfield->setEnabled(true);
                    this->showWarning(msg.length() ? msg : MSMAT_GET_TEXT("parent.common.error"));
                }
                else                /* các lỗi còn lại */
                {
                    m_button_ok->setEnabled(true);
                    m_email_textfield->setEnabled(true);
                    this->showWarning(msg.length() ? msg : MSMAT_GET_TEXT("parent.common.error"));
                }
            }
        }
    });
}

#pragma mark - Send Material Email
void MSMaterialEnterEmail::handleWithSendEmailMaterial(cocos2d::Ref *ref)
{
    const std::string& email_address = m_email_textfield->getString();
    
    if (email_address.empty())
    {
        this->showWarning(MSMAT_GET_TEXT("key.material.warning.no.email"));
        m_failReason = "email_no_insert";
    }
    else if (!mj::helper::isEmailAddressValid(email_address))
    {
        m_failReason = "wrong_format";
        this->showWarning(MSMAT_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
    }
    else
    {
        this->hideVirtualKeyboard(m_email_textfield);
        
        if (m_delegate)
        {
            m_button_ok->setEnabled(false);
            m_email_textfield->setEnabled(false);
            
            this->hideWarning();
            
            // trường hợp confirm gửi học liệu đến email đã được người khác đăng ký
            if (email_address.compare(m_email_duplicated) == 0)
            {
                m_failReason = "email_registered";
                m_emailType = MSMaterialEnterEmail::EmailType::NONE;
                
                // TODO: call API SendEmailMaterial
                this->sendEmailMaterial(email_address);
            }
            // trường hợp gửi học liệu về email đã được đăng ký
            else if (email_address.compare(MSMaterialEnterEmail::getCurrentMaterialEmail()) == 0)
            {
                m_emailType = MSMaterialEnterEmail::EmailType::NONE;
                m_email_duplicated = "";
                
                // TODO: call API SendEmailMaterial
                this->sendEmailMaterial(email_address);
            }
            // trường hợp đăng ký/cập nhật email nhận học liệu mới trước khi gửi học liệu
            else
            {
                // TODO: call API: UpdateEmail
                m_email = email_address;
                m_email_duplicated = "";
                
                auto user_info = ap::Database::getInstance().getUserInfo();
                ap::api::UpdateEmailMaterial::call(m_email, std::to_string(user_info._id), false, CC_CALLBACK_3(MSMaterialEnterEmail::onUpdateEmailMaterialAPICalled, this));
            }
        }
    }
}
std::string MSMaterialEnterEmail::mapMessageFromAPIToEventName(const std::string&  msg,int code) {
    switch (code)
    {
    case 200://good
        return " ";
    case 306://duplicate email
        return "email_registered";
    case 307: 
        return "limit_free_user";
    default:
        return "time_out";
        break;
    }
    return msg;
}

void MSMaterialEnterEmail::onSendEmailMaterialAPICalled(bool is_success, std::string msg, int code)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg, code]
    {
        if (is_success)
        {
            // event
            std::string status = "none";
            if (m_emailType == MSMaterialEnterEmail::EmailType::UPDATE) {
                status = "update";
            } else if (m_emailType == MSMaterialEnterEmail::EmailType::NEW) {
                status = "new";
            }
            this->pushCleverTapEvent_confirmEmail(status, MSMAT_MATERIAL_TYPE_STR(m_delegate->fromMaterialScreen()), true, false);
            
            // show message
            this->showSuccessMessage(MSMAT_GET_TEXT("key.material.notice.success.send.worksheet"));
        }
        else
        {
            if (code == 307)
            {
                this->showPopupFreeLimited(msg);
                m_failReason = mapMessageFromAPIToEventName(msg,code);
            }
            else
            {
                m_button_ok->setEnabled(true);
                m_email_textfield->setEnabled(true);
                this->showWarning(msg);
                if (code != 200) {
                    m_failReason = mapMessageFromAPIToEventName(msg,code);
                }
                else {
                    m_failReason = "server_error";
                }
                
            }
        }
    });
}

void MSMaterialEnterEmail::sendEmailMaterial(const std::string& email_address)
{
    auto string_ids = m_delegate->getStringMaterialIds();
    auto profile_name = MSMAT_GET_TEXT("Unknow");
    auto profiles = ap::Database::getInstance().getProfiles();
    auto cur_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id);
    
    for (auto profile : profiles) {
        if (profile.profile_id == cur_profile_id) {
            profile_name = profile.name;
            break;
        }
    }
    
    // TODO: call API SendEmailMaterial
    ap::api::SendEmailMaterial::call(email_address, profile_name, string_ids, CC_CALLBACK_3(MSMaterialEnterEmail::onSendEmailMaterialAPICalled, this));
}
