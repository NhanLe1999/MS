
#include "GameDiamondDig_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameDiamondDig_Item);

#define CSB_NAME "csb/game/diamond_dig/GameDiamondDig_Item.csb"

GameDiamondDig_Item * GameDiamondDig_Item::createItem(ItemInfo item_info) {
    auto p = static_cast<GameDiamondDig_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameDiamondDig_Item::didLoadFromCSB(ItemInfo item_info) {
	_item_info = item_info;
	this->setTouchEnabled(true);
	this->setContentSize(cocos2d::Size(882, 129));
	
	stone = GAFWrapper::createGAF("gaf/diamond_dig/dig_stone/dig_stone.gaf");
	stone->setPosition(this->getContentSize() / 2 + cocos2d::Size(0,35));
	this->addChild(stone);


	text = cocos2d::utils::findChild<Text*>(this, "text");
	text->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height*0.5116f));
	text->setString(_item_info.text);
	text->setFontName("fonts/monkeystories_ultra.ttf");
	text->setTextAreaSize(cocos2d::Size(700, 0));
	text->setFontSize(37);
	while (text->getContentSize().height>88){
		text->setFontSize(text->getFontSize()-1);
	}
	text->setLocalZOrder(10);
}


void GameDiamondDig_Item::setNewData(ItemInfo item_info){
	_item_info = item_info;
	stone->playSequence("stop");
	text->setString(_item_info.text);
	text->setOpacity(255);
}

void GameDiamondDig_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void GameDiamondDig_Item::onRead(bool isRead)
{
	if (isRead){
		text->setTextColor(Color4B::RED);
	}
	else
		text->setTextColor(Color4B::BLACK);
}

void GameDiamondDig_Item::onDig()
{
	stone->playSequence("correct");
	text->runAction(FadeOut::create(1.5f));
}

