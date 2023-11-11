#include "APProfileCard.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

USING_NS_CC;

std::once_flag account_profile_card_reader_flag;

INITIALIZE_READER(APProfileCard);

APProfileCard * APProfileCard::createCard(const ap::Profile profile, APCARD_MODE card_mode)
{
	std::call_once(account_profile_card_reader_flag, [] {
		REGISTER_CSB_READER(APProfileCard);
	});
	auto card = static_cast<APProfileCard*>(CSLoader::createNode("account/AccountProfileCard.csb"));
	if (card)
	{
		card->didLoadFromCSB(profile, card_mode);
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}

APProfileCard::APCARD_MODE APProfileCard::getCardMode()
{
	return m_card_mode;
}

void APProfileCard::setCardMode(APCARD_MODE card_mode)
{
	m_card_mode = card_mode;
	switch (card_mode)
	{
	case APProfileCard::EDITABLE:
		m_img_mode->setVisible(true);
		//TODO: load edit's texture if need
		break;
	case APProfileCard::NORMAL:
	default:
		m_img_mode->setVisible(false);
		break;
	}
}

void APProfileCard::setDelegate(APProfileCardDelegate* delegate)
{
	this->_delegate = delegate;
}

void APProfileCard::setProfile(ap::Profile profile)
{
	m_profile = profile;
	m_img_avatar->loadTexture(profile.avatar);
    
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

ap::Profile APProfileCard::getProfile()
{
	return m_profile;
}

void APProfileCard::didLoadFromCSB(const ap::Profile profile, APCARD_MODE card_mode)
{
	//this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_img_mode = utils::findChild<ui::ImageView*>(this, "img_mode");
	m_img_avatar = utils::findChild<ui::ImageView*>(this, "img_avatar");
	m_lbl_name = utils::findChild<ui::Text*>(this, "lbl_name");
	m_lbl_age = utils::findChild<ui::Text*>(this, "lbl_age");

	m_profile = profile;
	setCardMode(card_mode);
    setProfile(profile);

//    m_img_avatar->loadTexture(profile.avatar);
//    m_lbl_name->setString(profile.name);
//    m_lbl_age->setString(StringUtils::format("Age %d",profile.age));
}

void APProfileCard::onClickCard(cocos2d::Ref * sender)
{
	if (_delegate)
	{
		_delegate->onClickProfileCard(this);
	}
}

void APProfileCard::onClickIcon(cocos2d::Ref * sender)
{
	if (_delegate)
	{
		_delegate->onClickProfileIcon(this);
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback APProfileCard::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APProfileCard, onClickCard),
		CLICK_MAP(APProfileCard, onClickIcon)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}
	return nullptr;
}
