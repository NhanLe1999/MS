
#include "HotAirBalloon_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"
#include "PhonicConfig.h"

HotAirBalloon_Item * HotAirBalloon_Item::createItem(int number, int id) {

	auto p = new HotAirBalloon_Item();
	if (p->init()) {
        p->autorelease();
		p->didLoadFromCSB(number, id);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void HotAirBalloon_Item::didLoadFromCSB(int number, int id)
{
	item_size = Size(240, 160);
	this->setContentSize(item_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	this->_number = number;
	this->id = id;
	gaf::GAFAsset * cloud_asset;
	try {
		cloud_asset = PhonicConfig::getInstance()->gaf_cache.at("hab_cloud");
	}
	catch(const std::out_of_range& oor) {
		PhonicConfig::getInstance()->cacheGAF("hab_cloud","gaf/hotairballoon/hab_cloud/hab_cloud.gaf");
		cloud_asset = PhonicConfig::getInstance()->gaf_cache.at("hab_cloud");
	}
	gaf_cloud = GAFWrapper::createGAF(cloud_asset);
	gaf_cloud->setPosition(Vec2(item_size.width / 2, item_size.height / 2));
	gaf_cloud->setVisible(false);
	addChild(gaf_cloud);

	text = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%d", number), font_name, font_sz);
	text->setPosition(Vec2(item_size / 2));
	text->setTextColor(cocos2d::Color4B::BLACK);
	text->enableOutline(cocos2d::Color4B(248,235,232,255), 8);
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
	item_layout->addClickEventListener(CC_CALLBACK_1(HotAirBalloon_Item::onClickItem, this));

	is_pressed = true;
	is_used = false;
}

void HotAirBalloon_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();
}

void HotAirBalloon_Item::onClickItem(cocos2d::Ref *sender) {
	if (!is_pressed && _delegate) {
		is_pressed = true;
		_delegate->onClickedDelegate(this);
	}
}

void HotAirBalloon_Item::setDelegate(HotAirBalloon_ItemDelegate *delegate) {
	_delegate = delegate;
}

void HotAirBalloon_Item::setAudio(std::string audio) {
	_audio = audio;
}

void HotAirBalloon_Item::setTouchOnOff(bool isTrue) {
	item_layout->setTouchEnabled(isTrue);
}

void HotAirBalloon_Item::onShow(float dt)
{
	gaf_cloud->setVisible(true);
	gaf_cloud->playSequence("idle", true, [=](gaf::GAFObject * obj, const std::string &sqN) {
		is_pressed = false;
	});
	this->scheduleOnce([=](float delay) {
		text->setVisible(true);
		text->setScaleY(0);
		text->runAction(Sequence::create(ScaleTo::create(0.25f, 1.0f), NULL));
	}, dt, "item show");

}

void HotAirBalloon_Item::onHide(float dt)
{
	this->removeFromParent();
}

std::string HotAirBalloon_Item::getAudio() {
	return _audio;
}

void HotAirBalloon_Item::playEffect(bool correct, bool main_item) {
	if (is_used) return;
	is_used = true;
	if (correct) {
		//right
		gaf_cloud->playSequence("correct", false, [=](gaf::GAFObject * obj, const std::string &sqN) {

		});
        
        auto actions = Sequence::create(ScaleTo::create(0.25f, 2.0f), DelayTime::create(0.75f), FadeOut::create(0.25f), NULL);
        if(!main_item){
            actions = Sequence::create(FadeOut::create(0.25f), NULL);
        }
		text->runAction(Sequence::create(actions,CallFunc::create([=] {
			gaf::GAFAsset * rainbow_asset;
			try {
				rainbow_asset = PhonicConfig::getInstance()->gaf_cache.at("hab_rainbow");
			}
			catch (const std::out_of_range& oor) {
				PhonicConfig::getInstance()->cacheGAF("hab_rainbow","gaf/hotairballoon/hab_rainbow/hab_rainbow.gaf");
				rainbow_asset = PhonicConfig::getInstance()->gaf_cache.at("hab_rainbow");
			}
			auto gaf_rainbow = GAFWrapper::createGAF(rainbow_asset);
			gaf_rainbow->setPosition(Vec2(item_size.width / 2, item_size.height / 2));
			addChild(gaf_rainbow, -1);
			gaf_rainbow->play(false);
		}), NULL));

	}
	else {
		//wrong
		gaf_cloud->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			obj->playSequence("wrong_idle", true);

			int lightning = cocos2d::random(1, 3);
			gaf::GAFAsset * lightning_asset;
			try {
				lightning_asset = PhonicConfig::getInstance()->gaf_cache.at(StringUtils::format("hab_lightning_r%d", lightning));
			}
			catch (const std::out_of_range& oor) {
				PhonicConfig::getInstance()->cacheGAF(StringUtils::format("hab_lightning_r%d", lightning),StringUtils::format("gaf/hotairballoon/hab_lightning_r%d/hab_lightning_r%d.gaf", lightning, lightning));
				lightning_asset = PhonicConfig::getInstance()->gaf_cache.at(StringUtils::format("hab_lightning_r%d", lightning));
			}
			auto gaf_lightning = GAFWrapper::createGAF(lightning_asset);
			gaf_lightning->setPosition(Vec2(item_size.width / 2, item_size.height / 2));
			addChild(gaf_lightning,-1);
			gaf_lightning->play(true);

		});

		text->runAction(Sequence::create(ScaleTo::create(0.25f, 1.0f, 0.0f), NULL));
	}
}

