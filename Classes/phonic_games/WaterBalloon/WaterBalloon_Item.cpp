
#include "WaterBalloon_Item.h"
#include "GAFWrapper.h"

INITIALIZE_READER(WaterBalloon_Item);

#define CSB_NAME "csb/game/WaterBalloon/WaterBalloon_Item.csb"
//#define img_path "games/WaterBalloon/"

WaterBalloon_Item * WaterBalloon_Item::createItem(WaterBalloon_Item::Word word, int id) {
	
	auto p = static_cast<WaterBalloon_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(word,id);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void WaterBalloon_Item::didLoadFromCSB(WaterBalloon_Item::Word word, int id)
{
    
	item_layout = utils::findChild<Layout*>(this, "it_layout");
	item_layout->setTouchEnabled(true);
	item_layout->addTouchEventListener(CC_CALLBACK_2(WaterBalloon_Item::onClickItem, this));
	this->word = word;
	this->id = id;
	text = utils::findChild<Text*>(this, "Text");
	text->setText(word.text);
    if (CONFIG_MANAGER->isGameVMonkey()){
        
        //text->enableOutline(Color4B::RED);
        text->enableOutline(Color4B::BLUE,4);
        //text->setTextColor(Color4B::BLACK);
        text->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
        if (word.text.length() <= 3){
           text->setFontSize(250);
        }
    }
    _audio = word.audio_1;
}

void WaterBalloon_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();
}

void WaterBalloon_Item::onClickItem(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
	if (_delegate) {
		_delegate->onClickedDelegate(sender, this->id);
	}
}

void WaterBalloon_Item::setDelegate(WaterBalloon_ItemDelegate *delegate) {
	_delegate = delegate;
}


void WaterBalloon_Item::lockBalloon() {
	item_layout->setTouchEnabled(false);
}

void WaterBalloon_Item::unLockBalloon() {
	item_layout->setTouchEnabled(true);
}

void WaterBalloon_Item::setAudio(std::string audio) {
	_audio = audio;
}

std::string WaterBalloon_Item::getAudio() {
	return _audio;
}

void WaterBalloon_Item::playEffect(bool correct) {

	if (correct) {
		//right

	}
	else {
		//wrong

	}
}

void WaterBalloon_Item::setFontSize(int size){
    utils::findChild<Text*>(this, "Text")->setFontSize(size);
}
