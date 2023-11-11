#include "APProfileItem.h"
#include "StoryLanguageManager.h"

USING_NS_CC;

std::once_flag account_profile_item_reader_flag;

INITIALIZE_READER(APProfileItem);

APProfileItem * APProfileItem::createItem(const ap::Profile profile, bool selected)
{
	std::call_once(account_profile_item_reader_flag, [] {
		REGISTER_CSB_READER(APProfileItem);
	});
	auto card = static_cast<APProfileItem*>(CSLoader::createNode("account/AccountProfileItem.csb"));
	if (card)
	{
		card->didLoadFromCSB(profile, selected);
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}

void APProfileItem::setItemSelected(bool selected)
{
    if (selected == m_is_selected) return;
	
    m_is_selected = selected;

    utils::findChild<ui::Text*>(m_btn_select, "lbl_select")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(m_is_selected ? "key.selected":"key.selection.title"));

	m_btn_select->setBright(!selected);

}

void APProfileItem::setDelegate(APProfileItemDelegate* delegate)
{
	this->_delegate = delegate;
}

void APProfileItem::setProfile(ap::Profile profile)
{
	m_profile = profile;
	m_img_avatar->loadTexture(profile.avatar);
	m_lbl_name->setString(profile.name);
}

ap::Profile APProfileItem::getProfile()
{
	return m_profile;
}

void APProfileItem::didLoadFromCSB(const ap::Profile profile, bool selected)
{
	//this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_img_avatar = utils::findChild<ui::ImageView*>(this, "img_avatar");
	m_lbl_name = utils::findChild<ui::Text*>(this, "lbl_name");
	m_btn_select = utils::findChild<ui::Button*>(this, "btn_select");
    
    m_lbl_name->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    utils::findChild<ui::Text*>(m_btn_select, "lbl_select")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
    utils::findChild<ui::Text*>(m_btn_select, "lbl_select")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.selection.title"));
    
	setProfile(profile);
	setItemSelected(selected);
}

void APProfileItem::onSelect(cocos2d::Ref * sender)
{
	if (_delegate)
	{
		_delegate->onClickSelectItem(this);
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback APProfileItem::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
		CLICK_MAP(APProfileItem, onSelect)
	};

	if (function_map.find(name) != function_map.end())
    {
		return function_map.at(name);
	}
    
	return nullptr;
}
