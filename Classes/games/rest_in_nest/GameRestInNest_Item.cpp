
#include "GameRestInNest_Item.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"

INITIALIZE_READER(GameRestInNest_Item);

#define CSB_NAME "csb/game/rest_in_nest/GameRestInNest_Item.csb"

GameRestInNest_Item * GameRestInNest_Item::createItem() {
    auto p = static_cast<GameRestInNest_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameRestInNest_Item::reloadInfo(ItemInfo item_info)
{
	_item_info = item_info;
	mj::helper::shuffle(_item_info.texts);
	mj::helper::shuffle(numImg);
  
    /*for(auto text : _item_info.texts){
        if(text.length() > 3){
            text_sz = 60;
        }
    }*/

	for (int i = 0; i < (int)_item_info.texts.size(); i++){
		Text* text = imgs.at(i)->getChildByName<Text*>("text");
		text->setString(_item_info.texts.at(i));
        text->setOpacity(0xff);
        text->setFontSize(text_sz);
        if (CONFIG_MANAGER->isGameVMonkey()){
            text->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
        }
		std::string img_name = StringUtils::format("games/rest_in_nest/rest_in_nest_%d.png", numImg.at(i));
		imgs.at(i)->loadTexture(img_name);
	}
}

bool GameRestInNest_Item::onClick(int index)
{
	if (_item_info.texts.at(index).compare(_item_info.main_text)==0){
		//CCLOG("text: %d -  %s - %s -%s --- %s", index, _item_info.texts.at(0).c_str(), _item_info.texts.at(1).c_str(), _item_info.texts.at(2).c_str(), _item_info.main_text.c_str());
		return true;
	}
	else{
		AUDIO_ENGINE->playEffect("sounds/rest_in_nest/wrong.mp3");
		auto spaw1 = Spawn::createWithTwoActions(ScaleTo::create(0.2f, 1.1f, 0.8f), RotateTo::create(0.2f, 10));
		auto spaw2 = Spawn::createWithTwoActions(ScaleTo::create(0.2f, 1.0f, 1.0f), RotateTo::create(0.2f, 0));
		auto spaw3 = Spawn::createWithTwoActions(ScaleTo::create(0.2f, 0.9f, 1.05f), RotateTo::create(0.2f, -10));
		auto spaw4 = Spawn::createWithTwoActions(ScaleTo::create(0.2f, 1.0f, 1.0f), RotateTo::create(0.2f, 0));
		imgs.at(index)->setTouchEnabled(false);
		imgs.at(index)->runAction(Sequence::create(spaw1, spaw2, spaw3, spaw4, CallFunc::create([=](){imgs.at(index)->setTouchEnabled(true); }), nullptr));
		return false;
	}
}

bool GameRestInNest_Item::isRight(int index)
{
    
    if (_item_info.texts.at(index).compare(_item_info.main_text)==0){
        return true;
    }
    else{
        return false;
    }
}

ImageView* GameRestInNest_Item::getRightImage(int index){
    if (_item_info.texts.at(index).compare(_item_info.main_text)==0){
        return imgs.at(index);
    }
    else{
        return nullptr;
    }
}

void GameRestInNest_Item::setForFinish()
{
	for (int i = 0; i < imgs.size(); i++){
		imgs.at(i)->setVisible(false);
	}

	auto img = ImageView::create("games/rest_in_nest/rest_in_nest_dich.png");
	img->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	img->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
	img->setPositionY(-50);
	this->addChild(img);

}

void GameRestInNest_Item::didLoadFromCSB() {
	//this->setContentSize(cocos2d::Size(Director::getInstance()->getVisibleSize().width, 256));

	for (int i = 1; i <= 3; i++){
		std::string name = StringUtils::format("%s%d", "img", i);
		auto img1 = cocos2d::utils::findChild<ImageView*>(this, name);
		img1->setTouchEnabled(true);
		imgs.pushBack(img1);

		Text* text = img1->getChildByName<Text*>("text");
		text->enableOutline(Color4B::WHITE, 2);
	}

	numImg.push_back(1);
	numImg.push_back(2);
	numImg.push_back(3);
	numImg.push_back(4);
	numImg.push_back(5);
	numImg.push_back(6);
	numImg.push_back(7);
}

