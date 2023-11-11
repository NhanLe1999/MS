#include "CompareAndContrast_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(CompareAndContrast_Item);

#define CSB_NAME "csb/game/CompareAndContrast/CompareAndContrast_Item.csb"
//#define img_path "games/CompareAndContrast/"

CompareAndContrast_Item * CompareAndContrast_Item::createItem(std::string text , int id, std::string audio) {

	auto p = static_cast<CompareAndContrast_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(text, id, audio);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void CompareAndContrast_Item::didLoadFromCSB(std::string text, int id, std::string audio)
{
	item_layout = utils::findChild<Layout*>(this, "it_layout");
	item_layout->setTouchEnabled(true);
	this->_text = text;
	this->id = id;
	this->setAnchorPoint(Vec2(0.5, 0.5));
	auto ui_text = utils::findChild<Text*>(this, "Text");
	ui_text->setString(text);
	this->_audio = audio;

}

void CompareAndContrast_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();

}

void CompareAndContrast_Item::showItem() {
	this->setScale(0);
	this->runAction(EaseIn::create(ScaleTo::create(0.5, 1.f), 1.5));
}

void CompareAndContrast_Item::backToStart()
{
	item_layout->setTouchEnabled(false);
	//    this->runAction(Sequence::create(cocos2d::EaseQuadraticActionIn::create(cocos2d::MoveTo::create(0.25f, _start_pos)), CallFunc::create([=] {
	//        item_layout->setTouchEnabled(true);
	//    }), NULL));

	this->runAction(Sequence::create(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.25f, _start_pos)), CallFunc::create([=] {
		item_layout->setTouchEnabled(true);
	}), NULL));
}


void CompareAndContrast_Item::setTouchHandleType(CompareAndContrastItemTouchType type)
{
	switch (type)
	{
	case CompareAndContrastItemTouchType::NONE:
		break;
	case CompareAndContrastItemTouchType::CLICK:
		item_layout->setTouchEnabled(true);
		item_layout->addClickEventListener([=](cocos2d::Ref * sender) {
			if (_delegate) {
				_delegate->onClickedBox(this);
			}
		});
		break;
	case CompareAndContrastItemTouchType::TOUCH:
		item_layout->setTouchEnabled(true);
		item_layout->addTouchEventListener([=](cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type) {
			cocos2d::Vec2 move_pos;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				CCLOG("onTouchBegan");
				m_is_moved = false;
				_start_pos = this->getPosition();
				if (_delegate) {
					_delegate->onMoveBeginBox(this);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				m_is_moved = true;
				if (_delegate) {
					//item_layout->setVisible(false);
					move_pos = item_layout->getTouchMovePosition();
					this->setPosition(move_pos);
					_delegate->onMovingBox(this);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				if (_delegate) {
					_delegate->onMoveEndedBox(this);
				}
				break;
			default:
				break;
			}
		});
		break;
	default:
		break;
	}
}

void CompareAndContrast_Item::setTouchEnable(bool enable) {
	item_layout->setTouchEnabled(enable);
}

void CompareAndContrast_Item::setDelegate(CompareAndContrast_ItemDelegate *delegate) {
	_delegate = delegate;
}



void CompareAndContrast_Item::setAudio(std::string audio) {
	_audio = audio;
}


void CompareAndContrast_Item::playAudio() {
	AUDIO_ENGINE->playEffect(_audio);
}

std::string CompareAndContrast_Item::getAudio() {
	return _audio;
}

void CompareAndContrast_Item::playEffect(bool correct) {

	if (correct) {
		//right

	}
	else {
		//wrong

	}
}
