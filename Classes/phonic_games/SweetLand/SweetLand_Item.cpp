
#include "SweetLand_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"


SweetLand_Item * SweetLand_Item::createItem(int number, int id) {
	
	auto p = new SweetLand_Item();
	if (p->init()) {
        p->autorelease();
		p->didLoadFromCSB(number,id);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void SweetLand_Item::didLoadFromCSB(int number, int id)
{
	cocos2d::Size item_size = Size(180, 150);
	this->setContentSize(item_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	this->_number = number;
	this->id = id;

	gaf_button = GAFWrapper::createGAF("gaf/sweetland/sl_button/sl_button.gaf");
	gaf_button->setPosition(Vec2(item_size.width - 5,300));
	gaf_button->setVisible(false);
	addChild(gaf_button);

	text = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%d",number),font_name,font_sz);
	text->setPosition(Vec2(item_size.width / 2, 100));
	text->setTextColor(cocos2d::Color4B::BLACK);
	text->enableOutline(cocos2d::Color4B::WHITE, 8);
	text->setVisible(false);
	addChild(text, 1);

	item_layout = cocos2d::ui::Layout::create();
	item_layout->setContentSize(this->getContentSize());
	//// 4 debug
	//item_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	//item_layout->setBackGroundColorOpacity(100);
	//item_layout->setBackGroundColor(Color3B::BLUE);

	addChild(item_layout, 100);
	item_layout->setTouchEnabled(true);
	item_layout->addClickEventListener(CC_CALLBACK_1(SweetLand_Item::onClickItem, this));

	is_pressed = true;
}

void SweetLand_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();
}

void SweetLand_Item::onClickItem(cocos2d::Ref *sender) {
	if (!is_pressed && _delegate) {
		is_pressed = true;
		text->runAction(MoveBy::create(0.25f, Vec2(0, 100.0f)));
		_delegate->onClickedDelegate(this);
	}
}

void SweetLand_Item::setDelegate(SweetLand_ItemDelegate *delegate) {
	_delegate = delegate;
}

void SweetLand_Item::setAudio(std::string audio) {
	_audio = audio;
}

void SweetLand_Item::setTouchOnOff(bool isTrue) {
	item_layout->setTouchEnabled(isTrue);
}

void SweetLand_Item::onShow(float dt)
{
	this->scheduleOnce([=](float delay) {
		gaf_button->setVisible(true);
		gaf_button->playSequence("fadein", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			is_pressed = false;
		});
		text->setVisible(true);
		text->setScaleY(0);
		text->runAction(Sequence::create(DelayTime::create(0.3f),ScaleTo::create(0.3f,1.0f),NULL));
	}, dt, "item show");

}

void SweetLand_Item::onHide(float dt)
{
	this->scheduleOnce([=](float delay) {
		auto sequence_hide = is_pressed ? "fadeout1" : "fadeout2";
		gaf_button->playSequence(sequence_hide, false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			this->removeFromParent();
		});
	}, dt, "on hide");

}

std::string SweetLand_Item::getAudio() {
	return _audio;
}

void SweetLand_Item::playEffect(bool correct) {
	gaf_button->playSequence("press", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		if (correct) {
			//right
			gaf_button->playSequence("correct");
			text->runAction(Sequence::create(Spawn::create(MoveBy::create(0.25f, Vec2(0, 125)), ScaleTo::create(0.25f, 2.0f), NULL), DelayTime::create(0.85f), FadeOut::create(0.25f), NULL));
		}
		else {
			//wrong
			gaf_button->playSequence("wrong");
			text->runAction(Sequence::create(FadeOut::create(0.25f), NULL));
		}
	});

}

