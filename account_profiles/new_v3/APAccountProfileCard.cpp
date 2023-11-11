//
//  APAccountProfileCard.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 8/10/20.
//

#include "APAccountProfileCard.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
#include "MJDefinitions.h"
#include "MJDefault.h"

USING_NS_CC;

std::once_flag v3_ap_account_profile_card_reader_flag;

INITIALIZE_READER(APAccountProfileCard);

APAccountProfileCard * APAccountProfileCard::createCard(const ap::Profile profile, APCARD_MODE card_mode)
{
    std::call_once(v3_ap_account_profile_card_reader_flag, [] {
        REGISTER_CSB_READER(APAccountProfileCard);
    });
    auto card = static_cast<APAccountProfileCard*>(CSLoader::createNode("account/new_v3/APAccountProfileCard.csb"));
    if (card)
    {
        card->didLoadFromCSB(profile, card_mode);
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

APAccountProfileCard::APCARD_MODE APAccountProfileCard::getCardMode()
{
    return m_card_mode;
}

void APAccountProfileCard::setCardMode(APCARD_MODE card_mode)
{
    m_card_mode = card_mode;
    switch (card_mode)
    {
    case APAccountProfileCard::EDITABLE:
        m_img_mode->setVisible(true);
        //TODO: load edit's texture if need
        break;
    case APAccountProfileCard::NORMAL:
    default:
        m_img_mode->setVisible(false);
        break;
    }
}

void APAccountProfileCard::setDelegate(APAccountProfileCardDelegate* delegate)
{
    this->_delegate = delegate;
}

void APAccountProfileCard::setProfile(ap::Profile profile)
{
    m_profile = profile;
    m_img_avatar->loadTexture(profile.avatar);
    m_img_avatar->setScale(0.75f);
    
    m_lbl_name->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    m_lbl_name->setString(profile.name);
    if (profile.name.size() >= 10) m_lbl_name->setScale(0.9);

    m_lbl_age->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    m_lbl_age->setString(StringUtils::format("%s %d",LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.edit.age").c_str(), profile.age));
    
    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        m_lbl_age->setVisible(false);
    }
}

ap::Profile APAccountProfileCard::getProfile()
{
    return m_profile;
}

void APAccountProfileCard::didLoadFromCSB(const ap::Profile profile, APCARD_MODE card_mode)
{
    m_img_mode = utils::findChild<ui::ImageView*>(this, "img_mode");
    m_img_avatar = utils::findChild<ui::ImageView*>(this, "img_avatar");
    m_lbl_name = utils::findChild<ui::Text*>(this, "lbl_name");
    m_lbl_age = utils::findChild<ui::Text*>(this, "lbl_age");
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        m_img_mode->loadTexture("account/new_v3_vn/button_profile_edit.png");
    }

    m_profile = profile;
    setCardMode(card_mode);
    setProfile(profile);
}

void APAccountProfileCard::onClickCard(cocos2d::Ref * sender)
{
    if (_delegate)
    {
        _delegate->onClickProfileCard(this);
    }
}

void APAccountProfileCard::onClickIcon(cocos2d::Ref * sender)
{
    if (_delegate)
    {
        _delegate->onClickProfileIcon(this);
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback APAccountProfileCard::onLocateClickCallback(const std::string & name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APAccountProfileCard, onClickCard),
        CLICK_MAP(APAccountProfileCard, onClickIcon)
    };

    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    return nullptr;
}
