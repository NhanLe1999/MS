#include "ChoBoPhanTimTongThe_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(ChoBoPhanTimTongThe_Item);

#define CSB_NAME "csb/game/ChoBoPhanTimTongThe/ChoBoPhanTimTongThe_Item.csb"
//#define img_path "games/ChoBoPhanTimTongThe/"

ChoBoPhanTimTongThe_Item * ChoBoPhanTimTongThe_Item::createItem(std::string text, std::string audio,bool is_true) {

	auto p = static_cast<ChoBoPhanTimTongThe_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(text, audio,is_true);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void ChoBoPhanTimTongThe_Item::didLoadFromCSB(std::string text, std::string audio, bool is_true)
{
	_audio = audio;
	this->is_true = is_true;
	item_layout = utils::findChild<Layout*>(this, "it_layout");
    item_layout->setBackGroundColorOpacity(0);
	auto image_answer = utils::findChild<ImageView*>(this, "image_item");
	image_answer->loadTexture(text);
	item_layout->setTouchEnabled(true);
	item_layout->addClickEventListener([=](cocos2d::Ref * sender) {
		if (_delegate) {
			_delegate->onClickedBox(this);
		}
	});
	

}

void ChoBoPhanTimTongThe_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();

}

void ChoBoPhanTimTongThe_Item::showItem() {
	this->setScale(0);
	this->runAction(EaseIn::create(ScaleTo::create(0.5, 0.75f), 1.5));
	//this->playAudio();
}



void ChoBoPhanTimTongThe_Item::setTouchEnable(bool enable) {
	item_layout->setTouchEnabled(enable);
}

void ChoBoPhanTimTongThe_Item::setDelegate(ChoBoPhanTimTongThe_ItemDelegate *delegate) {
	_delegate = delegate;
}



void ChoBoPhanTimTongThe_Item::setAudio(std::string audio) {
	_audio = audio;
}


void ChoBoPhanTimTongThe_Item::playAudio() {
	AUDIO_ENGINE->playEffect(_audio);
}

std::string ChoBoPhanTimTongThe_Item::getAudio() {
	return _audio;
}
