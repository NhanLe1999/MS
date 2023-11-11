//
//  FireFighter.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/6/18.
//

#include "FireFighter.h"
#include "StoryConfigManager.h"
#define CSB_NAME "csb/game/FireFighter/Firefighter.csb"
#define update_time 10.0
#define font_name "fonts/SVN-Book Antiqua bold.ttf"
#define font_sz 150

USING_NS_CC;

INITIALIZE_READER (FireFighter);

std::once_flag FireFighter_Reader;

FireFighter* FireFighter::createGame(std::string json, bool is_Autoplay){
    
    std::call_once(FireFighter_Reader, []{
        REGISTER_CSB_READER(FireFighter);
    });
    
    auto p = static_cast<FireFighter*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json, is_Autoplay);
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
    
}

void FireFighter::onExit()
{
    
    AUDIO_ENGINE->stopAllAudio();
    this->unschedule(_learningReminderScheduleName);
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->stopAllActions();
    MJMainGBase::onExit();
}

void FireFighter::didLoadFromCSB(std::string json, bool is_Autoplay){
    _isStartGame = true;
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& jsonItem = document.FindMember("word")->value;
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    if(images.size() != 0){
         generateData(jsonItem, images);
    }else{
        _word.text = jsonItem.FindMember("text")->value.GetString();
        _word.audio_path = jsonItem.FindMember("audio_path")->value.GetString();
    }
    _screenSize = Director::getInstance()->getVisibleSize();
    this-> loadFrame();
}


void FireFighter::generateData(rapidjson::Value& json_data, std::vector<math::resource::Image*> images){
	for (auto image : images) {
		mj::GameObject obj;
		obj.text = image->getText("name_1");
		obj.audio_path = image->getAudios("name_1").front();
		obj.image_path = image->path;
		_gameDataSet.push_back(obj);
	}
	_gameIndex = 0;
    _word = _gameDataSet[_gameIndex];
}

void FireFighter::loadFrame(){
    if (_rootLayout = utils::findChild<Layout*>(this, "root_panel")) {
        //truck
        if (_truck = GAFWrapper::createGAF("gaf/firefighter/ff_truck/ff_truck.gaf")) {
            _truck->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            _rootLayout->addChild(_truck, 100);
            _truck->setPosition(Vec2(_screenSize.width, 0));
            _truck->setName("truck");
        }
        

        //create fighter
        if (FileUtils::getInstance()->isFileExist("gaf/firefighter/ff_fighter/ff_fighter.gaf")) {
            if (_fighter = GAFWrapper::createGAF("gaf/firefighter/ff_fighter/ff_fighter.gaf")) {
                _fighter->setAnchorPoint(Point::ANCHOR_MIDDLE);
                _fighter->setPosition(Vec2(-0.12 * _rootLayout->getContentSize().width, _rootLayout->getContentSize().height * 0.2));
                _rootLayout->addChild(_fighter);
                _fighter->setName("fighter");
            }
        }
        
        

        //_fireFighter parts
        if (_fireFighter = utils::findChild<Layout*>(_rootLayout, "fighter_09")) {
            _fireFighter->setBackGroundColorType(BackGroundColorType::NONE);
            _fireFighter->setLocalZOrder(99);
            _fireFighter->setOpacity(0);


            if (Node* armF = utils::findChild(_fireFighter, "armF")) {
                armF->setRotation(-20);
                ////create tia nuoc
                if (_water = utils::findChild<Layout*>(_fireFighter, "water_panel")) {
                    _water->setBackGroundColorType(BackGroundColorType::NONE);

                    _waterSpray = GAFWrapper::createGAF("gaf/firefighter/ff_water/ff_water.gaf");
                    _waterSpray->setAnchorPoint(Point(0, 0.65));
                    armF->addChild(_waterSpray);
                    _waterSpray->setPosition(Vec2(armF->getContentSize().width, 0));
                    _waterSpray->setScaleX(0);
                    _waterSpray->setScaleY(0.6);
                    _waterSpray->playSequence("idle", true);

                    ////create chum nuoc
                    if (_ChumNuoc = utils::findChild<Layout*>(_water, "splash_panel")) {
                        _ChumNuoc->setBackGroundColorType(BackGroundColorType::NONE);
                        _ChumNuoc->setLocalZOrder(9);

                        if (GAFWrapper* splash_2 = GAFWrapper::createGAF("gaf/firefighter/ff_watersplash2/ff_watersplash2.gaf")) {
                            splash_2->setAnchorPoint(Point(1, 0.4));
                            _ChumNuoc->addChild(splash_2);
                            splash_2->setPosition(_ChumNuoc->getContentSize() / 2);
                            splash_2->setName("splash_2");
                            splash_2->setScale(0);
                            splash_2->play(true);
                        }


                        if (GAFWrapper* splash_1 = GAFWrapper::createGAF("gaf/firefighter/ff_watersplash/ff_watersplash.gaf")) {
                            _ChumNuoc->addChild(splash_1);
                            splash_1->setPosition(_ChumNuoc->getContentSize() / 2);
                            splash_1->setName("splash_1");
                            splash_1->play(true);
                            splash_1->setScale(0);
                        }

                    }
                }

            }
            if (Node* armB = utils::findChild(_fireFighter, "armB")) {
                armB->setRotation(-39);
            }
            if (Node* legF = utils::findChild(_fireFighter, "legF")) {
                legF->setRotation(21.4);

            }
            if (Node* head = utils::findChild(_fireFighter, "head")) {
                head->setRotation(-10);

            }
            if (Node* tube = utils::findChild(_fireFighter, "tube")) {
                tube->setSkewY(16);

            }

            
        }
        

        //create fire
        if (_fireHouse = utils::findChild<ui::ImageView*>(_rootLayout, "house")) {
            if (Layout* myLayout = utils::findChild<Layout*>(_fireHouse, "Panel_4")) {
                _edgeX = myLayout->getPositionX();
            }
            for (int i = 0; i < _maxCount; i++) {
                std::string myLayoutName = "Panel_" + cocos2d::StringUtils::toString(i);
                if (Layout* fireLayout = utils::findChild<Layout*>(_fireHouse, myLayoutName)) {
                    fireLayout->setBackGroundColorType(BackGroundColorType::NONE);

                    int fi = (i % 2 == 0) ? 1 : 2;
                    int zOrder = (i % 2 == 0) ? 2 : 1;
                    std::string gafName = "gaf/firefighter/ff_fire" + cocos2d::StringUtils::toString(fi) + "/ff_fire" + cocos2d::StringUtils::toString(fi) + ".gaf";
                    
                    if (GAFWrapper* fire = GAFWrapper::createGAF(gafName)) {
                        fire->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
                        fire->setPosition(Vec2(fireLayout->getContentSize().width / 2, 0));
                        fire->setScale(0.1 * random(4, 8));
                        fire->play(true);
                        fire->setName("fire");
                        fire->setTag(99);
                        fire->setLocalZOrder(zOrder);
                        fireLayout->addChild(fire);

                        _fireList.push_back(fire);
                    }
                }
            }

            _fireHouse->setOpacity(0);

            if (Layout* myLayout = utils::findChild<Layout*>(_fireHouse, "Panel_8")) {
                myLayout->setLocalZOrder(-1);
            }

            if (Layout* myLayout = utils::findChild<Layout*>(_fireHouse, "Panel_4")) {
                myLayout->setLocalZOrder(-1);
            }
        }
       
        if (ImageView* houseImage = utils::findChild<ui::ImageView*>(_rootLayout, "Image_2")) {
            houseImage->setOpacity(0);
        }
        
    }
    
    if (_touchLayout = utils::findChild<Layout*>(this, "touch_panel")) {
        _touchLayout->setTouchEnabled(false);
        _touchLayout->addTouchEventListener(CC_CALLBACK_2(FireFighter::onTouched, this));
    }
    
    // speaker
    _touchable = false;
    if (Button* speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png")) {
        speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
        speaker->addClickEventListener([=](cocos2d::Ref* sender) {
            if (_touchable) {
                AUDIO_ENGINE->stopSound(_introSoundPath);
                _introSpeakerSoundId = AUDIO_ENGINE->playSound(_introSoundPath, false, [=] {
                    this->unschedule(_learningReminderScheduleName);
                    learningReminder();
                    });
            }
            });
        this->addChild(speaker);
    }
    onGameStart();
    
}

void FireFighter::onTouched(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    AUDIO_ENGINE->stopSound(_introSoundPath);
    this->unschedule(_learningReminderScheduleName);
    float goc, scale;
    string gafName = "splash_1" ;
    _touchable = false;

    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            //tinh goc va scale
            goc = calculateAngle(_water->convertToNodeSpace(((Layout*)sender)->getTouchBeganPosition()), _fireFighter->getAnchorPoint());
            scale = (_water->convertToNodeSpace(((Layout*)sender)->getTouchBeganPosition()).getDistance(_water->getAnchorPoint()))/_waterSpray->getContentSize().width;
            CCLOG("scale: %f", scale);
            CCLOG("goc: %f", goc);
            
            if((goc > 350 || goc < 60) && (scale > 0.53 && scale < 1.5 )){
                //water effect
                _ChumNuoc->setPosition(_water->convertToNodeSpace(((Layout*)sender)->getTouchBeganPosition()));
//            playWater(((Layout*)sender)->getTouchMovePosition());
                _waterActive = true;
                _waterSpray->runAction(ScaleTo::create(0.1, scale, 0.6));
    
                _waterId = AUDIO_ENGINE->play("sounds/firefighter/ff_water_loop_new.mp3", true);
//                _ChumNuoc->setScale(1);
                _ChumNuoc->stopActionByTag(99999);
                _ChumNuoc->runAction(ScaleTo::create(0.1, 1));
//                _ChumNuoc->setVisible(true);
                _waterSpray->playSequence("idle", true);

                //play fighter
                playFighter(goc);
            }else{
                _waterActive = false;
            }
            
               break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            if(_waterActive){
                //tinh goc va scale
                goc = calculateAngle(_water->convertToNodeSpace(((Layout*)sender)->getTouchMovePosition()), _fireFighter->getAnchorPoint());
                scale = (_water->convertToNodeSpace(((Layout*)sender)->getTouchMovePosition()).getDistance(_water->getAnchorPoint()))/_waterSpray->getContentSize().width;
                CCLOG("scale: %f", scale);
                CCLOG("goc: %f", goc);
                
                if((goc > 350 || goc < 60) && (scale > 0.53 && scale < 1.5 )){
                    //water effect
                    _ChumNuoc->setPosition(_water->convertToNodeSpace(((Layout*)sender)->getTouchMovePosition()));
                    _waterSpray->setScale(scale, _waterSpray->getScaleY());
                    //playWater(((Layout*)sender)->getTouchMovePosition());
                    //play fighter
                    playFighter(goc);
                }
                fireOff();
            }
    
            if(_count == _maxCount){
                _touchLayout->setTouchEnabled(false);
                onTouched(_touchLayout, TouchEventType::ENDED);
            }
            
            break;
            
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            if(_waterActive){
                waterOff();
            }
            _touchable = true;
            learningReminder();
            break;
            
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        {
            if (_waterActive) {
                waterOff();
            }
            break;
        }
          
            
        default:
            if(_waterActive){
                fireOff();
            }
            
            break;
    }
}

void FireFighter::learningReminder()
{
    this->schedule([=](float dt) {
        _introLearningReminderSoundId = AUDIO_ENGINE->playSound(_introSoundPath, false, [=] {
            this->unschedule(_learningReminderScheduleName);
            learningReminder();
            });
        }, 10, _learningReminderScheduleName);
}

void FireFighter::onGameStart(){
    
    _touchable = false;
    if (_isStartGame)
    {
        if (auto gameNameImageView = ImageView::create("games/firefighter/game_name_text.png")) {
            gameNameImageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            gameNameImageView->setName("gameNameImageView");
            gameNameImageView->setPosition(Vec2(_rootLayout->getContentSize().width * 0.5, _rootLayout->getContentSize().height * 0.85));
            _rootLayout->addChild(gameNameImageView, INT_MAX - 1);
        }
    }
    
    this->runAction(Sequence::create(
        CallFunc::create([=] {
            auto truck_fadein = AUDIO_ENGINE->play("sounds/firefighter/ff_truck_fadein.mp3", false);
            _truck->runAction(Sequence::create(
                CallFunc::create([=] {
                    _truck->runAction(EaseBackOut::create(MoveTo::create(2.5f, Vec2(-_truck->getContentSize().width * 0.9, 0))));
                    _truck->play(false);
                    }),
                DelayTime::create(1),
                CallFunc::create([=] {
                        if (_isStartGame)
                        {
                            _isStartGame = false;
                            AUDIO_ENGINE->playSound("sounds/firefighter/firefighter.mp3", false, [=] {
                                if (auto gameNameImageView = utils::findChild<ImageView*>(_rootLayout, "gameNameImageView"))
                                {
                                    gameNameImageView->runAction(FadeOut::create(1));
                                }
                                }, 1);
                        }
                    }), NULL));
            }), 
        DelayTime::create(3), 
        CallFunc::create([=] {
                _fireHouse->runAction(FadeIn::create(1));
                if (auto houseImage = utils::findChild<ui::ImageView*>(_rootLayout, "Image_2")) {
                    houseImage->runAction(FadeIn::create(1));
                }
            auto fighterId = AUDIO_ENGINE->play("sounds/firefighter/ff_fighter_fadein.mp3", false);
            cocos2d::experimental::AudioEngine::setFinishCallback(fighterId, [=](int id, string path) {
                AUDIO_ENGINE->play("sounds/firefighter/ff_water_fadein.mp3", false);
                AUDIO_ENGINE->playSound(_introSoundPath, false, [=] {
                    learningReminder();
                    _touchable = true;
                    });
                });
            _fighter->setVisible(true);
            _fighter->playSequence("fade_in", false, [=](gaf::GAFObject* obj, string path) {
                _fighter->playSequence("waterstart", false, [=](gaf::GAFObject* obj, string path) {
                    _waterSpray->runAction(ScaleTo::create(0.5, 1, 0.6));
                    auto d = _waterSpray->getContentSize().width;
                    auto posX = d * sin(20);
                    auto posY = d * cos(20) - 60;

                    _ChumNuoc->runAction(MoveTo::create(0.5, Vec2(posX, posY)));
                    utils::findChild(_ChumNuoc, "splash_1")->runAction(ScaleTo::create(0.5, 1));
                    _fireFighter->setOpacity(255);
                    _fighter->setVisible(false);
                    _touchLayout->setTouchEnabled(true);
                    });
                });
            }), 
        CallFunc::create([=] {
                _ChumNuoc->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2, 1.3), ScaleTo::create(1, 1), NULL)));
                
            }), NULL));
    
    updateFire(update_time);
}


void FireFighter::onEnter(){
//    onFinishGame();
    MJMainGBase::onEnter();
    MJMainGBase::playBackgroundMusic("sounds/firefighter/ff_bgm.mp3");
    _fireId = AUDIO_ENGINE->play("sounds/firefighter/ff_fire_new.mp3", true, 0.3);
    this->setContentSize(_screenSize);
    this->unscheduleUpdate();
}

void FireFighter::onFinishGame(){
    _touchable = false;
    AUDIO_ENGINE->stopSound(_introSoundPath);
    this->unschedule(_learningReminderScheduleName);
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
	++_gameIndex;
	if (_gameIndex == _gameDataSet.size() ){
        //AUDIO_ENGINE->stopAllEffects();
		MJMainGBase::onFinishGame();
	}
	else {
		
		onTransitionGame();
	}
   
}

void FireFighter::onExitTransitionDidStart(){
    /*this->unschedule(_learningReminderScheduleName);
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();*/
	MJMainGBase::onExitTransitionDidStart();
}

void FireFighter::updateFire(float time){
    this->schedule([=](float){
        for(auto fire: _fireList){
            if(fire->getScale() <= 0.3 && fire->getTag() == 99){
                fire->runAction(ScaleTo::create(1, random(0.4, 0.8)));
            }
        }
    }, time , 10000, 5.0f, "fire_fire");
}

void FireFighter::fireOff(){
    float fire_scale;
    for(auto fire_hold: _fireHouse->getChildren()){
        auto fire = utils::findChild(fire_hold, "fire");
        fire_scale = fire->getScale();
        if(fire_hold->getBoundingBox().containsPoint( (_fireHouse->convertToNodeSpace(_water->convertToWorldSpace(_ChumNuoc->getPosition()))))){
            //                    CCLOG("cham");
            if(fire->getScale() > 0.3){
                fire->runAction(ScaleTo::create(0.4, fire_scale - 0.3));
            }else{
                if(fire->getTag() == 99){
                   
                    AUDIO_ENGINE->play("sounds/firefighter/ff_fireoff.mp3", false);
                    _count++;
                    showText(fire_hold);
                    fire->setTag(98);
                }
                fire->setVisible(false);
            }
            break;
        }
    }
}

void FireFighter::onTransitionGame()
{
	this->unschedule("fire_fire");
    _word = _gameDataSet[_gameIndex];
	_count = 0;
	_truck->play(false);
	AUDIO_ENGINE->play("sounds/firefighter/ff_truck_fadein.mp3", false);
	_truck->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(-_truck->getContentSize().width * 2, 0)), CallFunc::create([=] {
		_truck->setPosition(Vec2(_screenSize.width, 0));
	}), NULL));
	_fireHouse->runAction(Sequence::create(
		FadeOut::create(0.5f),
		DelayTime::create(1.0f),
        CallFunc::create([=] {
            onGameStart();
            }),
        DelayTime::create(3.0f),
		FadeIn::create(0.5f), 
		CallFunc::create([=] {
		for (auto fire : _fireList) {
			if (fire) {
				fire->runAction(ScaleTo::create(0.5f, random(0.4, 0.8)));
				fire->play(true);
				fire->setVisible(true);
				fire->setTag(99);
			}
		}
	}),NULL));

	//_fireFighter parts
	_fireFighter = utils::findChild<Layout*>(_rootLayout, "fighter_09");
	_fireFighter->setBackGroundColorType(BackGroundColorType::NONE);
	_fireFighter->setLocalZOrder(99);
	_fireFighter->setOpacity(0);

	auto armF = utils::findChild(_fireFighter, "armF");
	auto armB = utils::findChild(_fireFighter, "armB");
	auto legF = utils::findChild(_fireFighter, "legF");
	auto head = utils::findChild(_fireFighter, "head");
	auto tube = utils::findChild(_fireFighter, "tube");

	armF->setRotation(-20);
	armB->setRotation(-39);
	legF->setRotation(21.4);
	head->setRotation(-10);
	tube->setSkewY(16);

	_waterSpray->setPosition(Vec2(armF->getContentSize().width, 0));
	_waterSpray->setScaleX(0);
	_waterSpray->setScaleY(0.6);
	_waterSpray->playSequence("idle", true);

	_ChumNuoc->setPosition(Vec2(70, 140));
}

void FireFighter::waterOff(){
    _ChumNuoc->stopAllActions();
    _waterActive = false;
    auto action = Spawn::create(ScaleTo::create(0.5, 0), MoveBy::create(0.5, Vec2(0, - 100)), NULL) ;
    action->setTag(99999);
    _ChumNuoc->runAction(action);
    AUDIO_ENGINE->stopEffect(_waterId);
    AUDIO_ENGINE->play("sounds/firefighter/ff_water_fadeout.mp3", false);
    _waterSpray->playSequence("fade_out", false, [=](gaf::GAFObject* obj, string path){
        _waterSpray->setScaleX(0);
        if(_count == _maxCount){
            _touchable = false;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([=]{
                this->onFinishGame();
            })));
        }
    });
}

void FireFighter::playWater(Vec2 pos){
    
    auto pos_x = _fireHouse->convertToNodeSpace(pos).x;
    auto pos_y = _fireHouse->convertToNodeSpace(pos).y;
    
    auto splash_1 = utils::findChild<GAFWrapper*>(_ChumNuoc, "splash_1");
    auto splash_2 = utils::findChild<GAFWrapper*>(_ChumNuoc, "splash_2");
    
    float scale = 0.5;
    
    if(  pos_x >= _fireHouse->getContentSize().width*0.065 && pos_x <= _fireHouse->getContentSize().width*0.075){
        if(pos_y > _fireHouse->getContentSize().height*0.7){
            scale  = 0.75;
        }else if(pos_y < _fireHouse->getContentSize().height*0.4){
            scale = 0.35;
        }
        _waterSpray->setScale(scale, 0.6);
        splash_1->setScale(0);
        splash_2->setScale(splash_2 ->getScale() +0.1);
    }else if(pos_x > _fireHouse->getContentSize().width*0.075 && pos_x <= _fireHouse->getContentSize().width*0.17){
        splash_1->setScale(pos_x/(_fireHouse->getContentSize().width*0.15));
        splash_2->setScale(_fireHouse->getContentSize().width*0.05/pos_x);
    }else{
        splash_1->setScale(1);
        splash_2->setScale(0);
    }
}

void FireFighter::playFighter(float goc){
    auto armF = utils::findChild(_fireFighter, "armF");
    auto armB = utils::findChild(_fireFighter, "armB");
    auto body = utils::findChild(_fireFighter, "body");
    auto head = utils::findChild(_fireFighter, "head");
    auto tube = utils::findChild(_fireFighter, "tube");
    
    if(goc < 60){
        armF->setRotation(-  goc);
        armB->setRotation(-goc*90/50);
        body->setRotation(- (4*goc/50));
        head->setRotation(- (18.3*goc/50));
        tube->setRotation(-(5*goc/50));
        tube->setSkewY((20*goc/50));
    }else{
        armF->setRotation(- goc);
        armB->setRotation((goc*22/355));
        body->setRotation((2.2*goc/300));
        head->setRotation((2.5*goc/300));
        tube->setSkewY( -(2.5*goc/280));
    }
}

void FireFighter::showText(Node* fire_hold){
    std::string font = "fonts/KGWHATTHETEACHERWANTS.TTF";
    Text* text = Text::create(_word.text, font, font_sz);
    text->setTextColor(Color4B(240, 157, 69, 255));
    text->enableOutline(Color4B::WHITE, 4);
    text->setScale(0);
    text->setOpacity(0);
    _rootLayout->addChild(text, 1000);
    text->setPosition(_rootLayout->convertToNodeSpace(fire_hold->getParent()->convertToWorldSpace(fire_hold->getPosition())));
    
    //TODO: fix text font size
    while (text->getVirtualRendererSize().width > getContentSize().width * 0.4) {
        auto font_size = text->getFontSize();
        text->setFontSize(font_size - 2);
        CCLOG("Scale down font: %f", font_size);
    }
    
    text->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1), FadeIn::create(0.3), MoveTo::create(0.5, Vec2(_fireHouse->getContentSize().width*0.3, _fireHouse->getContentSize().height*0.9)), CallFunc::create([=]{
        AUDIO_ENGINE->play(_word.audio_path);
    }),NULL), DelayTime::create(0.5),  FadeOut::create(0.3),CallFunc::create([=]{
        text->removeFromParent();
        
    }), NULL));
}

float FireFighter::calculateAngle(cocos2d::Vec2 pos, Vec2 center){
    float angle;
    float goc_set = abs((pos.y-center.y)/pos.getDistance(center));
    
    if(pos.x > center.x){
        if(pos.y > center.y){
            angle = CC_RADIANS_TO_DEGREES( asin(goc_set));
            
        }else{
            angle = 360-CC_RADIANS_TO_DEGREES(asin(goc_set));
        }
    }else{
        if(pos.y > center.y){
            angle = 180 - CC_RADIANS_TO_DEGREES(asin(goc_set));
        }else{
            angle = 180 + CC_RADIANS_TO_DEGREES(asin(goc_set));
        }
    }
    return angle;
}
