
#include "GameUFO_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameUFO_Item);

#define CSB_NAME "csb/game/ufo/GameUFO_Item.csb"

GameUFO_Item * GameUFO_Item::createItem(std::string str) {
    auto p = static_cast<GameUFO_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(str);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

int GameUFO_Item::getFontSize()
{
	auto text = cocos2d::utils::findChild<Text*>(this, "text");
	return text->getFontSize();
}

void GameUFO_Item::setFontSize(int sz)
{
	auto text = cocos2d::utils::findChild<Text*>(this, "text");
	text->setFontSize(sz);
}

void GameUFO_Item::didLoadFromCSB(std::string str) {
	//_item_info = item_info;
	text = str;
	this->setTouchEnabled(true);
	

	auto imgBg = cocos2d::utils::findChild<ImageView*>(this, "imgBg");
	this->setContentSize(imgBg->getContentSize());
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	this->setCascadeOpacityEnabled(true);
	this->setCascadeColorEnabled(true);

	auto text = imgBg->getChildByName<Text*>("text");

	text->setString(str);
	while (text->getContentSize().width>385){
		text->setFontSize(text->getFontSize() - 1);
	}
}

