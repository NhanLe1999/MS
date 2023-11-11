
#include "GameRabbitClassify_Light.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameRabbitClassify_Light);

USING_NS_CC;

GameRabbitClassify_Light * GameRabbitClassify_Light::createItem() {
	auto p = new GameRabbitClassify_Light();
	if (p) {
		p->init();
		p->autorelease();
		return p;
	}

    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GameRabbitClassify_Light::init()
{
	auto imgItem = cocos2d::ui::ImageView::create("games/rabbit_classify/rabbitclassify_light_off.png");
	imgItem->setPosition(imgItem->getContentSize() / 2);
	this->addChild(imgItem, 0, "imgItem");
	this->setContentSize(imgItem->getContentSize());
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	/*auto imgItemOn = cocos2d::ui::ImageView::create("games/rabbit_classify/rabbitclassify_light_on.png");
	imgItemOn->setPosition(this->getContentSize() / 2);
	this->addChild(imgItemOn, 0, "imgItemOn");
	imgItemOn->setVisible(false);*/
	return true;
}

void GameRabbitClassify_Light::onLightOn(){
	isOn = true;
	auto imgItem = this->getChildByName<cocos2d::ui::ImageView*>("imgItem");
	imgItem->loadTexture("games/rabbit_classify/rabbitclassify_light_on.png");
	//imgItem->removeFromParent();

	/*auto imgItemOn = this->getChildByName<cocos2d::ui::ImageView*>("imgItemOn");
	imgItemOn->setVisible(true);*/
}

