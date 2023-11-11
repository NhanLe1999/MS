
#include "GamePlugInBulb2_Item.h"
#include "HSAudioEngine.h"
#include "GamePlugInBulb2.h"

INITIALIZE_READER(GamePlugInBulb2_Item);

#define CSB_NAME "csb/game/plug_in_bulb2/GamePlugInBulb2_Item.csb"

GamePlugInBulb2_Item * GamePlugInBulb2_Item::createItem(ItemInfo iten_info, GAME_PLUG_IN_BULB2_ITEM_TYPE type1, GAME_PLUG_IN_BULB2_ITEM_TYPE type2) {
    auto p = static_cast<GamePlugInBulb2_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(iten_info, type1, type2);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GamePlugInBulb2_Item::didLoadFromCSB(ItemInfo iten_info, GAME_PLUG_IN_BULB2_ITEM_TYPE type1, GAME_PLUG_IN_BULB2_ITEM_TYPE type2) {
	_item_info = iten_info;
	_type1 = type1;// left or righr
	_type2 = type2;// define text and image
	_rope = DrawNode::create(10);
	this->addChild(_rope);

	if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_OUT){
		cocos2d::utils::findChild(this, "plug_left")->setVisible(false);
		dot = cocos2d::utils::findChild<ImageView*>(this, "dot_right");
		plug = cocos2d::utils::findChild<ImageView*>(this, "plug_right");
		this->schedule(schedule_selector(GamePlugInBulb2_Item::updateRope));
		posPlug = plug->getPosition();
	}
	else if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IN){
		cocos2d::utils::findChild(this, "dot_right")->setVisible(false);
		cocos2d::utils::findChild(this, "plug_right")->setVisible(false);
		dot = nullptr;
		plug = cocos2d::utils::findChild<ImageView*>(this, "plug_left");
	}
	plug->setLocalZOrder(100);

    if(_item_info.text.length() < 5){
        font_sz = 90;
    }
    
    
	auto text = cocos2d::utils::findChild<Text*>(this, "text");
    text->setFontSize(font_sz);
    
    //TODO: fix text font size
    while (text->getVirtualRendererSize().width > getContentSize().width * 0.45) {
        auto font_size = text->getFontSize();
        text->setFontSize(font_size - 2);
        CCLOG("Scale down font: %f", font_size);
    }
    
	auto img = cocos2d::utils::findChild<ImageView*>(this, "img");
    
	if (type2 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IMAGE_AND_TEXT){
		text->setString(_item_info.text);
		img->loadTexture(_item_info.img_path);
		img->setContentSize(cocos2d::Size(190, 190));
	}
	else if (type2 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_TEXT_TO_IMAGE){
		if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_OUT){
			text->setString(_item_info.text);
			text->setPosition(text->getParent()->getContentSize() / 2);
		}
		else if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IN){
			text->setString("");
			img->loadTexture(_item_info.img_path);
			img->setPosition(text->getParent()->getContentSize() / 2);
			img->setContentSize(cocos2d::Size(240, 240));
		}
	}
	else if (type2 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IMAGE_TO_TEXT){
		if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_OUT){
			text->setString("");
			img->loadTexture(_item_info.img_path);
			img->setPosition(text->getParent()->getContentSize() / 2); 
			img->setContentSize(cocos2d::Size(240, 240));
		}
		else if (type1 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IN){
			text->setString(_item_info.text);
			text->setPosition(text->getParent()->getContentSize() / 2);
		}
	}
	else if (type2 == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_TEXT_TO_TEXT){
		text->setString(_item_info.text);
		text->setPosition(text->getParent()->getContentSize() / 2);
	}
	
	panelItem = cocos2d::utils::findChild<Layout*>(this, "panelItem");
	panelItem->addClickEventListener([=](Ref* sender){
			auto gamePlugItem2 = this->getParent()->getParent();
			auto item = dynamic_cast<GamePlugInBulb2*>(gamePlugItem2);
			item->stopAudioIntro();
			item->stopAudioSpeaker();
			AUDIO_ENGINE->stopAllAudio();
			AUDIO_ENGINE->playSound(_item_info.audio_path);
			if (item->_flag & item->_flagAnswer) {
				item->stopInActiveMode();
				item->inActiveMode(item->start, item->stop);
			}
			else
			{
				item->stopLearningReminder();
				item->learningReminder();
			}
	});
}

void GamePlugInBulb2_Item::updateRope(float dt) {
	_rope->setLocalZOrder(plug->getLocalZOrder() - 1);
	_rope->clear();
	_rope->drawLine(plug->getPosition() + Point(-30, 0), dot->getPosition(), Color4F::RED);
}

void GamePlugInBulb2_Item::movePlugBack() {
	//original point: 92.5, 50
	AUDIO_ENGINE->playEffect ("sounds/plug_in_bulb2/bulb_wrong.mp3");
	plug->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(0.75, dot->getPosition() + Vec2(27,0)), 0.6), CallFunc::create([=] {
	}), nullptr));
}


void GamePlugInBulb2_Item::connectToRight(GamePlugInBulb2_Item * right, const std::function<void()> afunc /*= nullptr*/) {
	flag = false;
	auto gamePlugItem2 = this->getParent()->getParent();
	auto item = dynamic_cast<GamePlugInBulb2*>(gamePlugItem2);
	auto screen_size = Director::getInstance()->getVisibleSize();
	item->answerCorrect++;
	overlay = Layout::create(); // đè layout k cho tương tác khi kết thúc game
	if (item->answerCorrect == item->answer) {
		overlay->setContentSize(Size(screen_size.width, screen_size.height));
		overlay->setAnchorPoint(Vec2(0.5, 0.5));
		overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height/2));
		this->addChild(overlay, INT_MAX-1);
		overlay->setTouchEnabled(true);
		overlay->addClickEventListener([=](cocos2d::Ref* sender) {
			CCLOG("ok");
			});
		item->_speaker->setTouchEnabled(false);
		item->answerCorrect = 0;
	}
	item->_flagAnswer = flag;
	item->stopInActiveMode();
	item->learningReminder();
	plug->setTouchEnabled(false);
    cocos2d::experimental::AudioEngine::play2d(right->_item_info.audio_path);
	auto destination = right->convertToWorldSpace(right->plug->getPosition() + Vec2(-30,0));
	destination = this->convertToNodeSpace(destination);

	end1 = Node::create();
	end1->setPosition(dot->getPosition());
	this->addChild(end1);

	end2 = Node::create();
	end2->setPosition(plug->getPosition() + Point(-30, 0));
	this->addChild(end2);

	control_point = Node::create();
	control_point->setPosition((end1->getPosition() + end2->getPosition()) / 2);
	this->addChild(control_point);

	end2->runAction(EaseCircleActionIn::create(MoveTo::create(0.5, destination + Point(-27, 0))));

	this->schedule([=](float dt) {
		_rope->clear();
		_rope->drawQuadBezier(end1->getPosition(), control_point->getPosition(), end2->getPosition(), 10, Color4F::RED);
	}, "trampoline");

	plug->runAction(ScaleTo::create(0.5, 1, 1.3));
	plug->runAction(Sequence::create(
		EaseCircleActionIn::create(MoveTo::create(0.5, destination)),
		CallFunc::create([=] {
		
		auto voice_id1 = cocos2d::experimental::AudioEngine::play2d("sounds/plug_in_bulb2/bulb_right.mp3");
		cocos2d::experimental::AudioEngine::setFinishCallback(voice_id1, [=](int au_id, std::string path){
				if (afunc){
					this->scheduleOnce([=](float dt){
						afunc();
					}, 0.5f, StringUtils::format("finish_noi_%s", _item_info.audio_path.c_str()));
					
				}
		});

		control_point->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(1, (end1->getPosition() + destination + Point(-30, 0)) / 2), 0.15), CallFunc::create([=] {
			this->unschedule("trampoline");

		}), nullptr));

		this->scheduleOnce([=](float) {
		}, 1.5f, "delay_text_audio");

		plug->loadTexture("games/plug_in_bulb2/9.png");
		this->turnBulbOn();
		right->turnBulbOn();
		auto electric = GAFWrapper::createGAF("gaf/electric/electric.gaf");
		electric->setPosition(destination);
		electric->play(false, [=](gaf::GAFObject * object) {
			electric->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([=]{
				electric->removeFromParent();
			}), NULL));
		});
		this->addChild(electric);
	}),
		EaseElasticOut::create(ScaleTo::create(1, 1), 0.5),
		CallFunc::create([=] {
	}),
		nullptr)
		);
}

void GamePlugInBulb2_Item::turnBulbOn() {
	auto bg = this->getChildByName("bg");
	auto bg1 = this->getChildByName("bg1");
	this->runAction(Sequence::create(
		CallFunc::create([=](){
		bg->setVisible(false);
		bg1->setVisible(true);
	}), DelayTime::create(0.2f)
		,CallFunc::create([=](){
		bg->setVisible(true);
		bg1->setVisible(false);
	}), DelayTime::create(0.2f)
		, CallFunc::create([=](){
		bg->setVisible(false);
		bg1->setVisible(true);
	}), DelayTime::create(0.2f)
		, CallFunc::create([=](){
		bg->setVisible(true);
		bg1->setVisible(false);
	}), DelayTime::create(0.2f)
		, CallFunc::create([=](){
		bg->setVisible(false);
		bg1->setVisible(true);
	}), nullptr));
	
}
