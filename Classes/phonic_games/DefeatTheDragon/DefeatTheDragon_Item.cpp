
#include "DefeatTheDragon_Item.h"
#include "GAFWrapper.h"

INITIALIZE_READER(DefeatTheDragon_Item);

#define CSB_NAME "csb/game/DefeatTheDragon/DefeatTheDragon_Item.csb"
//#define img_path "games/DefeatTheDragon/"

DefeatTheDragon_Item * DefeatTheDragon_Item::createItem(Text* text, bool is_true, int id) {
	
	auto p = static_cast<DefeatTheDragon_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(text, is_true, id);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void DefeatTheDragon_Item::didLoadFromCSB(Text* text, bool is_true, int id)
{
	item_layout = utils::findChild<Layout*>(this, "it_layout");

	item_layout->setTouchEnabled(true);
	item_layout->addTouchEventListener(CC_CALLBACK_2(DefeatTheDragon_Item::onClickItem, this));
	this->is_true = is_true;
	this->id = id;

	//fire
	fire_small = GAFWrapper::createGAF("gaf/defeatthedragon/DtD_firesmall/DtD_firesmall.gaf");
	//fire_small->setScale(0.9f);
	item_layout->addChild(fire_small, 2);
	fire_small->setPosition(item_layout->getContentSize() / 2);
	fire_small->setVisible(false);

	fire_break = GAFWrapper::createGAF("gaf/defeatthedragon/DtD_firebreak/DtD_firebreak.gaf");
	item_layout->addChild(fire_break, 2);
	fire_break->setPosition(item_layout->getContentSize() / 2);
	fire_break->setVisible(false);

	fire_star = GAFWrapper::createGAF("gaf/defeatthedragon/DtD_stars/DtD_stars.gaf");
	item_layout->addChild(fire_star, 2);
	fire_star->setPosition(item_layout->getContentSize() / 2);
	fire_star->setVisible(false);

	//create text
	this->text = text;
	text->setPosition(item_layout->getContentSize() / 2);
	item_layout->addChild(text, 2);

}

void DefeatTheDragon_Item::showFire() {
	fire_small->play(true);
	fire_small->setVisible(true);
}

void DefeatTheDragon_Item::breakFire() {
	fire_break->setVisible(true);
	std::string s = "break" + cocos2d::StringUtils::toString(random(1, 4));
	fire_break->playSequence(s, false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		fire_break->setVisible(false);
	});
}

void DefeatTheDragon_Item::showStar() {

	fire_small->runAction(EaseIn::create(Sequence::create(ScaleTo::create(0.2, 1.4), DelayTime::create(0.15f), ScaleTo::create(0.3, 0), NULL),1.5f));
	text->runAction(EaseIn::create(Sequence::create(ScaleTo::create(0.2, 1.4), DelayTime::create(0.15f), ScaleTo::create(0.2, 1), NULL),1.5f));
	this->scheduleOnce([=](float dt) {
		fire_star->setVisible(true);
		fire_star->play(false, [=](gaf::GAFObject *obj) {
			fire_star->setVisible(false);
		});
	}, 0.3f, cocos2d::StringUtils::format("hien star"));
}

GAFWrapper* DefeatTheDragon_Item::getFire() {
	return this->fire_small;
}

void DefeatTheDragon_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();
}

void DefeatTheDragon_Item::onClickItem(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
	if (_delegate) {
		_delegate->onClickedDelegate(sender, this->text, this->id);
	}
}

void DefeatTheDragon_Item::setDelegate(DefeatTheDragon_ItemDelegate *delegate) {
	_delegate = delegate;
}

Text* DefeatTheDragon_Item::getText() {
	return this->text;
}

void DefeatTheDragon_Item::lockTouch() {
	item_layout->setTouchEnabled(false);
}

void DefeatTheDragon_Item::unLockTouch() {
	item_layout->setTouchEnabled(true);
}

void DefeatTheDragon_Item::setAudio(std::string audio) {
	_audio = audio;
}

std::string DefeatTheDragon_Item::getAudio() {
	return _audio;
}

void DefeatTheDragon_Item::playEffect(bool correct) {

	if (correct) {
		//right

	}
	else {
		//wrong

	}
}

