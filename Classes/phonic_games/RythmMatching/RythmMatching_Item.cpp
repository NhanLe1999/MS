#include "RythmMatching_Item.h"

#define RM_ITEM_CSB "csb/game/rythmmatching/RythmMatching_Item.csb"

INITIALIZE_READER(RythmMatching_Item);

RythmMatching_Item::RythmMatching_Item()
	: m_bg_type(RMI_BG_NONE)
	, m_touch(RMI_DENY)
	, _clickable(false)
	, _dragable(false)
	, _touch_layout(nullptr)
	, _delegate (nullptr)
{

}


RythmMatching_Item * RythmMatching_Item::createItem(RM_ITEM_INFO info, RM_ITEM_BG_TYPE bg_type, RM_ITEM_TOUCHPERMISSION touch)
{
	auto item = static_cast<RythmMatching_Item *>(cocos2d::CSLoader::createNode(RM_ITEM_CSB));
	if (item) {
		item->didLoadFromCSB(info,bg_type,touch);
	}
	else {
		CC_SAFE_DELETE(item);
		item = nullptr;
	}
	return item;
}

void RythmMatching_Item::didLoadFromCSB(RM_ITEM_INFO info, RM_ITEM_BG_TYPE bg_type, RM_ITEM_TOUCHPERMISSION touch)
{
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	this->setCascadeOpacityEnabled(true);
	_touch_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "touch_layout");
	_touch_layout->setTouchEnabled(true);
	setInfo(info);
	setBackgroundItemType(bg_type);
	setTouchPermission(touch);
}


cocos2d::ui::Widget::ccWidgetTouchCallback RythmMatching_Item::onLocateTouchCallback(const std::string & callback_name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetTouchCallback> function_map = {
		{ "onTouchedItem",CC_CALLBACK_2(RythmMatching_Item::onTouchedItem,this) }
	};

	if (function_map.find(callback_name) != function_map.end()) {
		return function_map.at(callback_name);
	}

	return nullptr;
}

void RythmMatching_Item::onTouchedItem(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto item = static_cast<RythmMatching_Item *> (sender);
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		onTouchBegan(this);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		onTouchMoved(this);
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		onTouchEnded(this);
		break;
	default:
		break;
	}
}

void RythmMatching_Item::onTouchBegan(cocos2d::Ref * sender)
{
	auto item = static_cast<RythmMatching_Item *> (sender);
	_start_pos = item->getPosition();
	if (_clickable && _delegate) {
		_delegate->onTouchItemBegan(sender);
	}
	
}

void RythmMatching_Item::onTouchMoved(cocos2d::Ref * sender)
{
	auto item = static_cast<RythmMatching_Item *> (sender);
	if (_dragable) {
		item->setPosition(_touch_layout->getTouchMovePosition());
		if (_delegate) _delegate->onTouchItemMoved(sender);
	}
}

void RythmMatching_Item::onTouchEnded(cocos2d::Ref * sender)
{
	if (_dragable && _delegate) {
		_delegate->onTouchItemEnded(sender);
	}
}

void RythmMatching_Item::setInfo(RM_ITEM_INFO info)
{
	m_info = info;

	auto image = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "image");
	image->loadTexture(info.image);
	//image->setVisible((m_bg_type != RMI_BG_DASH));

	auto lbl_name = cocos2d::utils::findChild<cocos2d::ui::Text*>(this, "lbl_name");
	lbl_name->setString(info.text);
	//lbl_name->setVisible((m_bg_type != RMI_BG_DASH));
}

void RythmMatching_Item::setBackgroundItemType(RM_ITEM_BG_TYPE type)
{
	m_bg_type = type;
	cocos2d::utils::findChild(this, "image")->setVisible(m_bg_type != RMI_BG_DASH);
	cocos2d::utils::findChild(this, "lbl_name")->setVisible(m_bg_type != RMI_BG_DASH);
	cocos2d::utils::findChild(this, "dash_layout")->setVisible(m_bg_type == RMI_BG_DASH);
	cocos2d::utils::findChild(this, "box_y_layout")->setVisible(m_bg_type == RMI_BG_YELLOW);
	cocos2d::utils::findChild(this, "box_w_layout")->setVisible(m_bg_type == RMI_BG_WHITE);
}

void RythmMatching_Item::setTouchPermission(RM_ITEM_TOUCHPERMISSION permission)
{
	m_touch = permission;
	_clickable = (permission != RMI_DENY);
	_dragable = (permission == RMI_TOUCH);
}


RythmMatching_Item::RM_ITEM_INFO RythmMatching_Item::getInfo()
{
	return m_info;
}

void RythmMatching_Item::backToStart(float duration)
{
	_touch_layout->setTouchEnabled(false);
	this->runAction(MoveTo::create(duration, _start_pos));
	_touch_layout->scheduleOnce([=](float dt) {
		_touch_layout->setTouchEnabled(true);
	},duration,"back to start");
}


void RythmMatching_Item::onItemCorrect()
{
	switch (m_bg_type)
	{
	case RythmMatching_Item::RMI_BG_NONE:
		break;
	case RythmMatching_Item::RMI_BG_DASH:
		setBackgroundItemType(RMI_BG_WHITE);
		break;
	case RythmMatching_Item::RMI_BG_YELLOW:
		break;
	case RythmMatching_Item::RMI_BG_WHITE:
		setBackgroundItemType(RMI_BG_YELLOW);
		break;
	default:
		break;
	}
}

void RythmMatching_Item::onItemWrong()
{
	switch (m_bg_type)
	{
	case RythmMatching_Item::RMI_BG_NONE:
		break;
	case RythmMatching_Item::RMI_BG_DASH:
	{
		auto dash_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "dash_layout");
		auto img_wrong = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(dash_layout, "box_gr");
		img_wrong->stopAllActions();
		img_wrong->setOpacity(0);
		img_wrong->setVisible(true);
		img_wrong->runAction(Sequence::create(FadeIn::create(0.125f), Hide::create(), DelayTime::create(0.125f),
			Show::create(), DelayTime::create(0.25f), Hide::create(), NULL));
	}
		break;
	case RythmMatching_Item::RMI_BG_YELLOW:
	case RythmMatching_Item::RMI_BG_WHITE:
	{
		auto box_r = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "box_r");
		box_r->stopAllActions();
		box_r->setOpacity(0);
		box_r->setVisible(true);
		box_r->runAction(Sequence::create(
			FadeIn::create(0.125f), Hide::create(), DelayTime::create(0.125f),
			Show::create(), DelayTime::create(0.25f), Hide::create(),
			NULL));
	}
		break;
	default:
		break;
	}
	
}

void RythmMatching_Item::setDelegate(RythmMatching_ItemDelegate * delegate)
{
	_delegate = delegate;
}

void RythmMatching_Item::onShow(float delay)
{
	auto start_pos = this->getPosition();
	this->setPosition(start_pos + Vec2(0, -500));
	this->runAction(Sequence::create(DelayTime::create(delay), EaseBackOut::create(MoveTo::create(0.5f, start_pos)), CallFunc::create([=] {
	}), NULL));
}
