#include "WS_Playarea.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "HSAudioEngine.h"

using namespace cocos2d::experimental;

WS_Playarea* WS_Playarea::createGame(std::vector<math::resource::Image*> images)
{
    WS_Playarea* obj = new WS_Playarea();
    if(obj->init())
    {
        obj->autorelease();
        obj->initOptions(images);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void WS_Playarea::initOptions(std::vector<math::resource::Image*> images)
{
    this->_callbackCloseActivity = [=] () {
        listener->setEnabled(false);
    };
    
    origin_wheel = screenSize.width*0.5 + 225;
	listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(WS_Playarea::onTouchesBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(WS_Playarea::onTouchesMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(WS_Playarea::onTouchesEnded,this);
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener,-1);
    
    loadData(images);
    
    //background...
    background=Sprite::create("games/wheel_spin/PlayAreaBG.png");
    background->setPosition(Vec2(screenSize.width/2,screenSize.height/2));
    this->addChild(background);
	if (background->getContentSize().width < Director::getInstance()->getVisibleSize().width)
		background->setScale(Director::getInstance()->getVisibleSize().width / background->getContentSize().width);
    
    if ((int)list_object.size() >= 5) {
        NoOfPart = 5;
    }else NoOfPart = 4;
    
    
    //Create Start Screen
    this->setupStartScreen();
    
    //Scene 1 Create
    this->setupWheel();
    
    //Scene 2 Create
    this->setupScene2();
    
    auto layout_title = cocos2d::ui::Layout::create();
    layout_title->setBackGroundColor(Color3B::BLACK);
    layout_title->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_title->setContentSize(screenSize);
    layout_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layout_title->setPosition(layout_title->getContentSize()/2);
    this->addChild(layout_title, 10000000);
    layout_title->setOpacity(200);
    
	AUDIO_ENGINE->playEffect("sounds/wheel_spin/gameName.mp3");
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playEffect("sounds/wheel_spin/music_bg.mp3");
    }

    auto imgGameName = cocos2d::ui::ImageView::create("games/wheel_spin/title.png");
	//imgGameName->setScale(3);
    layout_title->addChild(imgGameName, 10000);
    imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
    layout_title->runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(0.5f)
                                            , CallFunc::create([=](){
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::startGame,this),0.2,"startGame");
        layout_title->removeFromParent();
    }), nullptr));
    
}
#include "JsonDump.h"
void WS_Playarea::loadData(std::vector<math::resource::Image*> images){
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("json/ObjectDetails.plist");
        buffer_string = JsonDump::getInstance()->MapToString(value_map);
    }
    
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    value_data.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    auto content_info = value_data["data"].GetArray();
    for (int i = 0; i < (int) content_info.Size(); i++) {
        WheelSpinSet wheelspin_set;
		auto info = math::resource::ResHelper::getImage(images, content_info[i].GetString());
		wheelspin_set.audio_path = info->getAudio();
		wheelspin_set.img_path = info->path;
		wheelspin_set.text = info->getText();
        Object_Name_str[i+1] = wheelspin_set.text;
        list_object.push_back(wheelspin_set);
    }
}

//For Setup Start Screen
void WS_Playarea::setupStartScreen()
{
    _sp_No = Sprite::create(this->getImage("wheel_spin/Start_1.png"));
    _sp_No->setScale(SCALE_XY*0.5);
    _sp_No->setPosition(Vec2(screenSize.width/2 - 278, screenSize.height/2));
    _sp_No->setScale(0);
    this->addChild(_sp_No);
    
    /*_sp_start = Sprite::create(this->getImage("wheel_spin/Start_4.png"));
    _sp_start->setScale(SCALE_XY*0.5);
    _sp_start->setPosition(Vec2(origin_wheel-225,522 + add_y));
    _sp_start->setVisible(false);
    this->addChild(_sp_start);
    
    _sp_start->runAction(Repeat::create(Sequence::create(MoveTo::create(0.1, Vec2(origin_wheel-225,522 + add_y)),MoveTo::create(0.1, Vec2(origin_wheel-215,522 + add_y)), NULL),100));*/
    
}

void WS_Playarea::onEnter(){
    ms::BaseActivity::onEnter();
}

//For Start Game
void WS_Playarea::startGame()
{
    touch_tool = true;
	CCLOG("Start game");
    currentScene = 1;
    AUDIO_ENGINE->playEffect("sounds/wheel_spin/mc1.mp3");
    
    _sp_No->runAction(Sequence::create(
		DelayTime::create(3.0f)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/Ping.mp3"))
		,EaseBackInOut::create(ScaleTo::create(0.3, SCALE_XY*0.5))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/1.mp3"))
		,DelayTime::create(0.9f),ScaleTo::create(0.35, 0)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/Ping.mp3"))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::changeImage, this,_sp_No,"wheel_spin/Start_2.png"))
		,EaseBackInOut::create(ScaleTo::create(0.3, SCALE_XY*0.5))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/2.mp3"))
		,DelayTime::create(0.9f),ScaleTo::create(0.35, 0)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/Ping.mp3"))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::changeImage, this,_sp_No,"wheel_spin/Start_3.png"))
		,EaseBackInOut::create(ScaleTo::create(0.3, SCALE_XY*0.5))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/3.mp3"))
		,DelayTime::create(1.2),MoveBy::create(0.25, Vec2(-500*POSX, 0*POSY))
		,Hide::create(), NULL));
    
    //_sp_start->runAction(Sequence::create(DelayTime::create(7.0f),Show::create(), NULL));
    
    this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::enableSpine,this),7.0f,"enableSpine");

}

//For End Game
void WS_Playarea::endGame()
{
    _sp_stand->stopAllActions();
    _sp_wheel->stopAllActions();
    _sp_pin->stopAllActions();
    
//    _sp_stand->runAction(Sequence::create(ScaleTo::create(0, SCALE_X,SCALE_Y),MoveBy::create(0.5, Vec2(-800*POSX, 0*POSY)), NULL));
//    _sp_wheel->runAction(Sequence::create(ScaleTo::create(0, SCALE_X,SCALE_Y),MoveBy::create(0.5, Vec2(-800*POSX, 0*POSY)), NULL));
//    _sp_pin->runAction(Sequence::create(ScaleTo::create(0, SCALE_X,SCALE_Y),MoveBy::create(0.5, Vec2(-800*POSX, 0*POSY)), NULL));
    
    for (int i=1; i<=NoOfPart; i++)
    {
        _sp_star[i]->stopAllActions();
        _sp_star[i]->runAction(Sequence::create(DelayTime::create(0.0),MoveBy::create(0.3, Vec2(0*POSX, -260)), CallFunc::create([=]{
			if (i==NoOfPart){
				this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
					//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
                    this->_callbackCloseActivity = nullptr;
					commonEndGame(ms::BaseActivity::EGT_COMMON);
				}), NULL));
			}
        }), NULL));
    }
}

//void WS_Playarea::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(screenSize);
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

//For Setup Scene 2
void WS_Playarea::setupScene2()
{
    //Generate Random Object
    Object_counter_scene2 = 1;
    for(int i=1;i<=NoOfPart;i++)
    {
        Object_No_scene2[i] = random(1, NoOfPart);
        int j = 1;
        while (j<i)
        {
            if(Object_No_scene2[j] == Object_No_scene2[i])
            {
                Object_No_scene2[i] = random(1, NoOfPart);
                j = 1;
            }
            else
                j++;
        }
    }
    
    int X = 568 - ((100 * (NoOfPart-1)) / 2);
    
    X = screenSize.width/2 - (NoOfPart-1)/2.0f*100;
    CCLOG("tru = %f", (NoOfPart-1)/2.0f*100);
    for(int i=1;i<=NoOfPart;i++)
    {
        _sp_star[i]=Sprite::create(this->getImage("wheel_spin/Star_2.png"));
        _sp_star[i]->setPosition(Vec2(X + (i-1)*100,-200));
        this->addChild(_sp_star[i]);
        
        Sprite *_sp_temp = Sprite::create(this->getImage("wheel_spin/Star_1.png"));
        _sp_temp->setTag(1);
        _sp_temp->setScale(0);
        _sp_temp->setPosition(Vec2(_sp_star[i]->getContentSize().width/2, _sp_star[i]->getContentSize().height/2));
        _sp_star[i]->addChild(_sp_temp);
        
//        X += 100;
    }
}
void WS_Playarea::startObjectButton(int SelectedObject)
{
    touch_tool=true;

    
    if(Object_No[SelectedObject] == Object_No_scene2[Object_counter_scene2])
    {
        AUDIO_ENGINE->stopAllSound();
        this->unschedule("suggetion_game");
        AUDIO_ENGINE->playEffect("sounds/wheel_spin/Positive_sound_2.mp3");
        _sp_object_button[SelectedObject]->stopAllActions();

		_sp_object_button[SelectedObject]->runAction(Sequence::create(DelayTime::create(0.3),MoveTo::create(0.5f,screenSize/2), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path)), ScaleTo::create(0.2, SCALE_X * 2.25, SCALE_Y * 2.25), DelayTime::create(2.5), EaseBackInOut::create(ScaleTo::create(0.2, SCALE_X * 0, SCALE_Y * 0)), NULL));
        

        Animation *anim_button = Animation::create();
        anim_button->setDelayPerUnit(0.1);
        anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_1.png"));
        anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_2.png"));
        anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_3.png"));
        anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_4.png"));
        anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_5.png"));
        
        ((Sprite *)_sp_object_button[SelectedObject]->getChildByTag(1))->stopAllActions();
        ((Sprite *)_sp_object_button[SelectedObject]->getChildByTag(1))->setTexture(this->getImage("wheel_spin/Buttons_2.png"));
        _sp_object_button[SelectedObject]->getChildByTag(1)->runAction(Sequence::create(DelayTime::create(0.2),Repeat::create(Animate::create(anim_button),3),CallFunc::create(CC_CALLBACK_0(WS_Playarea::changeImage, this,(Sprite *)_sp_object_button[SelectedObject]->getChildByTag(1),"wheel_spin/Buttons_2.png")),NULL));
        
        _sp_star[Object_counter_scene2]->getChildByTag(1)->runAction(
			Sequence::create(
			DelayTime::create(0.5)
			,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/Positive_sound_1.mp3"))
			,EaseBackInOut::create(ScaleTo::create(0.5, 1.0)), NULL));
        
        for(int i=1;i<=NoOfPart;i++)
        {
            
            if(i != SelectedObject)
                _sp_object_button[i]->runAction(Sequence::create(DelayTime::create(0.0),ScaleTo::create(0.2, SCALE_X*0,SCALE_Y*0), NULL));
        }       
        Object_counter_scene2++;
        if (Object_counter_scene2 < 5) {
            
            answerRemind(8.0f);
        }
        else {
            _speaker->setTouchEnabled(false);
        }
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::enableObjectButton,this),(2.0),"enableObjectButton");
        
        
    }
    else
    {
        //click wrong answer, audio remind answer correct
        _isSuggetGame = false;
        NoOfWrongAnswer++;
        //AUDIO_ENGINE->stopAllSound();
        AUDIO_ENGINE->stopAllEffects();
        this->unschedule("suggetion_game");
        AUDIO_ENGINE->playEffect("sounds/wheel_spin/Ping.mp3");
        _sp_object_button[SelectedObject]->runAction(Sequence::create(EaseBackInOut::create(ScaleTo::create(0.5, SCALE_X*0,SCALE_Y*0)), NULL));
        _speaker->setTouchEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                AUDIO_ENGINE->play("sounds/wheel_spin/tap_on1.mp3", false,1.0f, [=]() {
                    AUDIO_ENGINE->play(list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path, false, 1.0, [=]() {
                        _speaker->setTouchEnabled(true);
                        answerRemind(0.0f);
                        touch_true();
                        }); });
            
             }), NULL));
        
        if(NoOfWrongAnswer == 2)
        {
            Animation *anim_button = Animation::create();
            anim_button->setDelayPerUnit(0.1);
            anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_4.png"));
            anim_button->addSpriteFrameWithFile(this->getImage("wheel_spin/Buttons_5.png"));
            
            for(int i=1;i<=NoOfPart;i++)
            {
                if(Object_No[i] == Object_No_scene2[Object_counter_scene2])
                {
                    _sp_object_button[i]->getChildByTag(1)->runAction(Sequence::create(DelayTime::create(0.2),Repeat::create(Animate::create(anim_button),-1),NULL));
                    
                    break;
                }
            }
        }
    }
}
void WS_Playarea::enableObjectButton()
{
    if(Object_counter_scene2 <= NoOfPart)
    {
        NoOfWrongAnswer = 0;
        if(Object_counter_scene2>1)
        {
            _speaker->setTouchEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.5),
				CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/huzz_pop.mp3")),
				CallFunc::create([=](){
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(StringUtils::format("sounds/wheel_spin/tap_on%d.mp3", random(1, 4)));
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){

                    AUDIO_ENGINE->play(list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path, false, 1.0f, [=]() {
                        _speaker->setTouchEnabled(true);
                        });

				});
			})
				, nullptr));
            
            Object_No_scene2_temp[1] = Object_No_scene2_temp[2] = Object_No_scene2_temp[3] = Object_No_scene2_temp[4] = Object_No_scene2_temp[5] = 0;
            
            for(int i=1;i<=NoOfPart;i++)
            {               
                Object_No_scene2_temp[i] = random(1, NoOfPart);
                int j = 1;
                while (j<i)
                {
                    if(Object_No_scene2_temp[j] == Object_No_scene2_temp[i])
                    {
                        Object_No_scene2_temp[i] = random(1, NoOfPart);
                        j = 1;
                    }
                    else
                        j++;
                }
            }

           
            //sort answer
            if (NoOfPart < 5) {
                for (int i = 1; i <= NoOfPart; i++)
                {
                    _sp_object_button[i]->setPosition(pos_object_button[Object_No_scene2_temp[i]]);
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    _sp_object_button[i]->stopAllActions();
                    if (i < 3) {
                        _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                        auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), (visibleSize.height * 0.65)) + Vec2((i - 1) * visibleSize.width / 2, 0));
                        auto scale = ScaleTo::create(0.5, 1.5);
                        _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                    }
                    else {
                        _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                        auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), visibleSize.height * 0.35) + Vec2((i - 3) * visibleSize.width / 2, 0));
                        auto scale = ScaleTo::create(0.5, 1.5);
                        _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                    }
                    //_sp_object_button[i]->runAction(Sequence::create(EaseBackInOut::create(ScaleTo::create(0.5, SCALE_X,SCALE_Y)), NULL));
                }
            }
            else {
                for (int i = 1; i <= NoOfPart; i++)
                {
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    _sp_object_button[i]->stopAllActions();

                    if (i < 3) {
                        _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                        auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), (visibleSize.height * 0.75)) + Vec2((i - 1) * visibleSize.width / 2, 0));
                        auto scale = ScaleTo::create(0.5, 1.5);
                        _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                    }
                    else if (i == 3) {
                        _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                        auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.5), (visibleSize.height * 0.5)));
                        auto scale = ScaleTo::create(0.5, 1.5);
                        _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                    }
                    else {
                        _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                        auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), visibleSize.height * 0.25) + Vec2((i - 4) * visibleSize.width / 2, 0));
                        auto scale = ScaleTo::create(0.5, 1.5);
                        _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                    }
                }
            }
            
        }
        else
        {
            _speaker->setTouchEnabled(false);
			auto audio_id = cocos2d::experimental::AudioEngine::play2d(StringUtils::format("sounds/wheel_spin/tap_on%d.mp3", random(1, 4)));
			cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){             
                AUDIO_ENGINE->play(list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path, false, 1.0f, [=]() {
                    _speaker->setTouchEnabled(true);
                    });               
			});
        }
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::touch_true,this),(2.0),"touch_true");
    }
    else
    {
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::touch_true,this),(1.0),"touch_true");
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::endGame,this),(1.0),"endGame");
    }
}
//For Setup Wheel
void WS_Playarea::setupWheel()
{
    rotationValue = 360 / NoOfPart;
    
    //Generate Random Object
    Object_counter = 1;
    for(int i=1;i<=NoOfPart;i++)
    {
        Object_No[i] = random(1, NoOfPart);
        int j = 1;
        while (j<i)
        {
            if(Object_No[j] == Object_No[i])
            {
                Object_No[i] = random(1, NoOfPart);
                j = 1;
            }
            else
                j++;
        }
    }
    //Wheel Create
    _sp_spine=Sprite::create(this->getImage("wheel_spin/Spine_1.png"));
    _sp_spine->setScale(SCALE_X,SCALE_Y);
    _sp_spine->setPosition(Vec2(origin_wheel-4,247+add_y));
    this->addChild(_sp_spine);
    
    _sp_spine1=Sprite::create(this->getImage("wheel_spin/Buttons_1.png"));
    _sp_spine1->setScale(0,0);
    _sp_spine1->setOpacity(0);
    _sp_spine1->setPosition(Vec2(origin_wheel-5,581+add_y));
    this->addChild(_sp_spine1);
    
    _sp_stand=Sprite::create(this->getImage("wheel_spin/Stand.png"));
    _sp_stand->setScale(SCALE_X,SCALE_Y);
    _sp_stand->setPosition(Vec2(origin_wheel-3,150+add_y));
    this->addChild(_sp_stand);
    
    _sp_wheel=Sprite::create(this->getImage(StringUtils::format("wheel_spin/Wheel_%d_1.png",NoOfPart)));
    _sp_wheel->setScale(SCALE_X,SCALE_Y);
    _sp_wheel->setPosition(Vec2(origin_wheel,343+add_y));
    this->addChild(_sp_wheel);
    
  
    
    int Y = ((90 * (NoOfPart-1)) / 2) + 320;
    Y = screenSize.height/2 + (NoOfPart-1)/2.0f*90;
    for(int i=1;i<=NoOfPart;i++)
    {
        _sp_object[i]=ImageView::create(list_object[i-1].img_path);
        _sp_object[i]->setScale(0.0);
        _sp_wheel->addChild(_sp_object[i],0);
        
        Object_Name[i] = Label::createWithTTF(Object_Name_str[i].getCString(), "monkeystories_ultra.ttf", 140);
        Object_Name[i]->setPosition(Vec2(screenSize.width/2,90));
        Object_Name[i]->setScale(SCALE_XY*0);
        //Object_Name[i]->setGlobalZOrder(10);
        this->addChild(Object_Name[i],10);
        
		_sp_object_button[i] = cocos2d::ui::Scale9Sprite::create(this->getImage("wheel_spin/Buttons_0.png"));
        _sp_object_button[i]->setScale(SCALE_X*0,SCALE_Y*0);
		//_sp_object_button[i]->setContentSize(cocos2d::Size(200,70));
        _sp_object_button[i]->setPosition(Vec2(screenSize.width/2-238,Y));//185
        this->addChild(_sp_object_button[i]);
        
        pos_object_button[i] = Vec2(screenSize.width/2,Y);
        
        Y -= 90;
        
        Sprite *_sp_temp = Sprite::create(this->getImage("wheel_spin/Buttons_2.png"));
        _sp_temp->setTag(1);
        _sp_temp->setPosition(Vec2(_sp_object_button[i]->getContentSize().width/2, _sp_object_button[i]->getContentSize().height/2));
        _sp_object_button[i]->addChild(_sp_temp);
        
        Object_Name_Button[i] = Label::createWithTTF(Object_Name_str[Object_No[i]].getCString(), "monkeystories_ultra.ttf", 30);
        Object_Name_Button[i]->setPosition(Vec2(_sp_object_button[i]->getContentSize().width/2, _sp_object_button[i]->getContentSize().height/2));
        _sp_object_button[i]->addChild(Object_Name_Button[i]);
    }
    if(NoOfPart == 5)
    {
        pos_object[1] = Vec2(34*POSX,199*POSY);
        _sp_object[1]->setScale(0.1f);
        _sp_object[1]->setPosition(Vec2(213*POSX,86*POSY));
        
        pos_object[2] = Vec2(89*POSX,42*POSY);
        _sp_object[2]->setScale(0.1f);
        _sp_object[2]->setPosition(Vec2(285*POSX,180*POSY));
        
        pos_object[3] = Vec2(296*POSX,61*POSY);
		_sp_object[3]->setScale(0.1f);
        _sp_object[3]->setPosition(Vec2(215*POSX,282*POSY));
        
        pos_object[4] = Vec2(370*POSX,232*POSY);
		_sp_object[4]->setScale(0.1f);
        _sp_object[4]->setPosition(Vec2(101*POSX,251*POSY));
        
        pos_object[5] = Vec2(192*POSX,388*POSY);
		_sp_object[5]->setScale(0.1f);
        _sp_object[5]->setPosition(Vec2(89*POSX,121*POSY));

		for (int i = 1; i <= 5; i++){
			float maxScaleY = 90 / _sp_object[i]->getContentSize().height;
			float maxScaleX = 90 / _sp_object[i]->getContentSize().width;
			float maxScale = maxScaleX > maxScaleY ? maxScaleY : maxScaleX;
			_sp_object[i]->setScale(maxScale);
		}
    }
    else if(NoOfPart == 4)
    {
        pos_object[1] = Vec2(34*POSX,199*POSY);
		_sp_object[1]->setScale(0.1f);
        _sp_object[1]->setPosition(Vec2(228*POSX,93*POSY));
        
        pos_object[2] = Vec2(167*POSX,17*POSY);
		_sp_object[2]->setScale(0.1f);
        _sp_object[2]->setPosition(Vec2(277*POSX,229*POSY));
        
        pos_object[3] = Vec2(340*POSX,181*POSY);
		_sp_object[3]->setScale(0.1f);
        _sp_object[3]->setPosition(Vec2(140*POSX,271*POSY));
        
        pos_object[4] = Vec2(213*POSX,331*POSY);
		_sp_object[4]->setScale(0.1f);
        _sp_object[4]->setPosition(Vec2(97*POSX,139*POSY));
		for (int i = 1; i <= 4; i++){
			float maxScaleY = 100 / _sp_object[i]->getContentSize().height;
			float maxScaleX = 100 / _sp_object[i]->getContentSize().width;
			float maxScale = maxScaleX > maxScaleY ? maxScaleY : maxScaleX;
			_sp_object[i]->setScale(maxScale);
		}
    }
    
    _sp_pin=Sprite::create(this->getImage("wheel_spin/Pin.png"));
    _sp_pin->setScale(SCALE_X,SCALE_Y);
    _sp_pin->setPosition(Vec2(origin_wheel-3,128+add_y));
    _sp_pin->setAnchorPoint(Vec2(0.5,0));
    this->addChild(_sp_pin,1);

	lyl_wheel = LayerColor::create(Color4B(0, 0, 0, 0), screenSize.width, screenSize.height);
	this->addChild(lyl_wheel, 9);
	//lyl_wheel->setGlobalZOrder(9);
    
}
void WS_Playarea::startWheel()
{
    touch_tool=true;
    
	// if(Object_counter>1)
    //    _sp_object[Object_No[Object_counter-1]]->setGlobalZOrder(0);
    
    Animation *anim_wheel = Animation::create();
    anim_wheel->addSpriteFrameWithFile(this->getImage(StringUtils::format("wheel_spin/Wheel_%d_3.png",NoOfPart)));
    anim_wheel->addSpriteFrameWithFile(this->getImage(StringUtils::format("wheel_spin/Wheel_%d_4.png",NoOfPart)));
    anim_wheel->setDelayPerUnit(0.1);
    
    int rand_no = 60;
    
    AUDIO_ENGINE->playEffect("sounds/wheel_spin/slide.mp3");
    AUDIO_ENGINE->playEffect("sounds/wheel_spin/spining.mp3");
    
    _sp_wheel->runAction(Sequence::create(
		Repeat::create(
		Animate::create(anim_wheel),(rand_no/4))
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::changeImage, this,_sp_wheel,StringUtils::format("wheel_spin/Wheel_%d_1.png",NoOfPart))), NULL));
    
    _sp_wheel->setRotation(currentRotation);
    int TotalRotation = 2880;
    TotalRotation -= currentRotation;
    int rotateCircleValue = (rotationValue*(Object_No[Object_counter]-1));
    currentRotation = rotateCircleValue;
    TotalRotation += rotateCircleValue;
    
    _sp_wheel->runAction((Sequence::create(
		EaseSineIn::create(RotateBy::create((1.0), TotalRotation/4))
		,RotateBy::create((1.0), (TotalRotation/2))
		,EaseSineOut::create(RotateBy::create((1.0), (TotalRotation/4))),NULL)));
    
    auto object_spin = _sp_object[Object_No[Object_counter]];
	auto object_clone = object_spin->clone();
	object_clone->setVisible(false);
	this->addChild(object_clone, 10);
    //object_spin->setGlobalZOrder(10);
   
	float minScaleY = 100 / object_spin->getContentSize().height;
	float minScaleX = 100 / object_spin->getContentSize().width;
	float minScale = minScaleX > minScaleY ? minScaleY : minScaleX;

	float maxHeght = Director::getInstance()->getVisibleSize().height - 200;
	float maxScaleY = maxHeght / object_spin->getContentSize().height;
	float maxWidth = Director::getInstance()->getVisibleSize().width - 200;
	float maxScaleX = maxWidth / object_spin->getContentSize().width;
	float maxScale = maxScaleX > maxScaleY ? maxScaleY : maxScaleX;
    object_clone->runAction(Sequence::create(
		DelayTime::create((0.05*rand_no)+0.3)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/huzz_pop.mp3"))
		, CallFunc::create([=]{
		object_spin->setVisible(false);
		object_clone->setPosition(_sp_wheel->convertToWorldSpace(object_spin->getPosition()));
		object_clone->setRotation(_sp_wheel->getRotation());
		object_clone->setVisible(true);
		auto old_pos_object = object_clone->getPosition();
		auto pos_goal = Vec2(screenSize.width / 2, screenSize.height / 2 + 20);
        CCLOG("pin pin pin = %f, %f, %f", pos_goal.x, pos_goal.y, _sp_wheel->getRotation());
		object_clone->runAction(Sequence::create(
			Spawn::create(
			RotateTo::create(0.5, 0)
			,ScaleTo::create(0.5, maxScale*0.9),MoveTo::create(0.5, pos_goal), NULL)
			, DelayTime::create(2.0)
			,Spawn::create(
			RotateTo::create(0.3, (int)_sp_wheel->getRotation() % 360)
			, ScaleTo::create(0.3, minScale)
			,MoveTo::create(0.3, old_pos_object), NULL), CallFunc::create([=]{
			object_spin->setVisible(true);
			object_clone->setVisible(false);
            if (Object_counter >= 3 && !game_play_enable) {
                this->pauseGame();
            }
        }), NULL));
    }), NULL));
    
    Object_Name[Object_No[Object_counter]]->runAction(Sequence::create(\
		DelayTime::create((0.05*rand_no)+0.8)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/slide.mp3"))
		,EaseBackOut::create(ScaleTo::create(0.1, SCALE_XY))
		,DelayTime::create(0.2)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, list_object[Object_No[Object_counter]-1].audio_path))
		,DelayTime::create(1.8)
		,ScaleTo::create(0.1, SCALE_XY*0), NULL));
    
    lyl_wheel->runAction(Sequence::create(DelayTime::create((0.05*rand_no)+0.3),FadeTo::create(0.5, 215),DelayTime::create(2.0),FadeOut::create(0.1), NULL));
    
    _sp_object_button[Object_counter]->runAction(Sequence::create(
		DelayTime::create((0.05*rand_no)+0.3)
		,DelayTime::create(2.6)
		,EaseBackInOut::create(ScaleTo::create(0.5, SCALE_X,SCALE_Y)), NULL));
    _sp_object_button[Object_counter]->runAction(Sequence::create(
		DelayTime::create((0.05*rand_no)+0.3)
		,DelayTime::create(2.8)
		,CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this,"sounds/wheel_spin/Positive_sound_1.mp3")), NULL));
    
    Object_counter++;
    
    _sp_pin->runAction(Sequence::create(Repeat::create(Sequence::create(RotateBy::create(0.1, -30),RotateBy::create(0.1, 30), NULL), (rand_no/4)),RotateTo::create(0.0, 0),NULL));

    if(Object_counter == NoOfPart+1)
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::enableSpine,this),((0.05*rand_no)+4.1),"enableSpine");
    else
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::enableSpine,this),((0.05*rand_no)+3.1),"enableSpine");
}
void WS_Playarea::enableSpine()
{
    
    if(Object_counter <= NoOfPart)
    {
        unschedule("letspin");
        _sp_spine->stopAllActions();
        _sp_spine->setTexture(this->getImage("wheel_spin/Spine_3"));
        Animation *anim_spine = Animation::create();
        anim_spine->setDelayPerUnit(0.2);
        anim_spine->addSpriteFrameWithFile(this->getImage("wheel_spin/Spine_2.png"));
        anim_spine->addSpriteFrameWithFile(this->getImage("wheel_spin/Spine_1.png"));
        
        

        _sp_spine->runAction(Sequence::create(DelayTime::create(0.0),EaseBackInOut::create(MoveTo::create(0.5, Vec2(origin_wheel-4,367+add_y))),Repeat::create(Animate::create(anim_spine),-1), NULL));
        if (Object_counter == 1) {
            _sp_spine1->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, "sounds/wheel_spin/spin_up.mp3")), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, "sounds/wheel_spin/letspin.mp3")), DelayTime::create(0.2), ScaleTo::create(0, SCALE_X, SCALE_Y), NULL));
            spinRemind();
        }       
        else if (Object_counter == 2) {
            
            _sp_spine1->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, "sounds/wheel_spin/spin_up.mp3")), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, "sounds/wheel_spin/Spin the wheel again .mp3")), DelayTime::create(0.2), ScaleTo::create(0, SCALE_X, SCALE_Y), NULL));
            spinRemind();
        }       
        else {
            _sp_spine1->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(WS_Playarea::playSound, this, "sounds/wheel_spin/spin_up.mp3")), DelayTime::create(0.2), ScaleTo::create(0, SCALE_X, SCALE_Y), NULL));
            spinRemind();
        }               
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::touch_true,this),(0.7),"touch_true");
    }
    else
    {
        currentScene = 2;
        _sp_spine->stopAllActions();
        _sp_spine1->stopAllActions();
        _sp_stand->stopAllActions();
        _sp_wheel->stopAllActions();
        _sp_pin->stopAllActions();

        _sp_spine->runAction(Sequence::create(MoveBy::create(0.5, Vec2(800*POSX, 0*POSY)),ScaleTo::create(0, 0), NULL));
        _sp_spine1->runAction(Sequence::create(MoveBy::create(0.5, Vec2(800*POSX, 0*POSY)),ScaleTo::create(0, 0), NULL));
        _sp_stand->runAction(Sequence::create(MoveBy::create(0.5, Vec2(800*POSX, 0*POSY)),ScaleTo::create(0, 0), NULL));
        _sp_wheel->runAction(Sequence::create(MoveBy::create(0.5, Vec2(800*POSX, 0*POSY)),ScaleTo::create(0, 0), NULL));
        _sp_pin->runAction(Sequence::create(MoveBy::create(0.5, Vec2(800*POSX, 0*POSY)),ScaleTo::create(0, 0),DelayTime::create(0.6),CallFunc::create(CC_CALLBACK_0(WS_Playarea::enableObjectButton, this)), NULL));
      
        

        //sort answer & star
        if (NoOfPart < 5) {
            for (int i = 1; i <= NoOfPart; i++)
            {
                auto visibleSize = Director::getInstance()->getVisibleSize();
                _sp_object_button[i]->stopAllActions();


                if (i < 3) {
                    _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                    auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), (visibleSize.height * 0.65)) + Vec2((i - 1) * visibleSize.width / 2, 0));
                    auto scale = ScaleTo::create(0.5, 1.5);
                    _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                }
                else {
                    _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                    auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), visibleSize.height * 0.35) + Vec2((i - 3) * visibleSize.width / 2, 0));
                    auto scale = ScaleTo::create(0.5, 1.5);
                    _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                }
                _sp_star[i]->stopAllActions();
                _sp_star[i]->runAction(Sequence::create(DelayTime::create(0.4), MoveBy::create(0.5, Vec2(0, 260)), NULL));
            }
        }
        else {
            for (int i = 1; i <= NoOfPart; i++)
            {
                auto visibleSize = Director::getInstance()->getVisibleSize();
                _sp_object_button[i]->stopAllActions();

                if (i < 3) {
                    _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                    auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), (visibleSize.height * 0.75)) + Vec2((i - 1) * visibleSize.width / 2, 0));
                    auto scale = ScaleTo::create(0.5, 1.5);
                    _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                }
                else if (i == 3) {
                    _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                    auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.5), (visibleSize.height * 0.5)));
                    auto scale = ScaleTo::create(0.5, 1.5);
                    _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                }
                else {
                    _sp_object_button[i]->setAnchorPoint(Vec2(0.5, 0.5));
                    auto moveTo = MoveTo::create(0.5, Vec2((visibleSize.width * 0.25), visibleSize.height * 0.25) + Vec2((i - 4) * visibleSize.width / 2, 0));
                    auto scale = ScaleTo::create(0.5, 1.5);
                    _sp_object_button[i]->runAction(Spawn::create(moveTo, scale, NULL));
                }
                _sp_star[i]->stopAllActions();
                _sp_star[i]->runAction(Sequence::create(DelayTime::create(0.4), MoveBy::create(0.5, Vec2(0, 260)), NULL));
            }
        }
             
        radio();
    }
}

WS_Playarea::~WS_Playarea()
{
	this->getEventDispatcher()->removeEventListener(listener);
}

void WS_Playarea::onExit()
{
    this->_callbackCloseActivity = nullptr;
}

bool WS_Playarea::onTouchesBegan(Touch* touch, Event* event)
{
    Point location=touch->getLocationInView();
    location=Director::getInstance()->convertToGL(location);
    location=this->convertToNodeSpace(location);

    if(touch_tool == true)
        return false;
    
    if((_sp_spine1->getBoundingBox().containsPoint(location)
        || _sp_wheel->getBoundingBox().containsPoint(location))
        && _sp_wheel->getNumberOfRunningActions()
        == 0 && Object_counter <= NoOfPart && lyl_wheel->getNumberOfRunningActions()
        == 0 && currentScene == 1)
    {

        

        unschedule("letspin");
        //_sp_start->setVisible(false);
        _sp_spine->stopAllActions();
        _sp_spine->setTexture(this->getImage("wheel_spin/Spine_3.png"));
        
        Animation *anim_spine = Animation::create();
        anim_spine->setDelayPerUnit(0.2);
        anim_spine->addSpriteFrameWithFile(this->getImage("wheel_spin/Spine_4.png"));
        anim_spine->addSpriteFrameWithFile(this->getImage("wheel_spin/Spine_3.png"));
        
        AUDIO_ENGINE->playEffect("sounds/wheel_spin/switch_only.mp3");
        
        _sp_spine->runAction(Sequence::create(DelayTime::create(0),EaseBackInOut::create(MoveTo::create(0.5, Vec2(origin_wheel-4,247+add_y))),Repeat::create(Animate::create(anim_spine), -1), NULL));
        _sp_spine1->runAction(Sequence::create(DelayTime::create(0),ScaleTo::create(0,0), NULL));
        this->scheduleOnce(CC_CALLBACK_0(WS_Playarea::startWheel,this),0.1,"startWheel");
    }
    else if(currentScene == 2)
    {
        unschedule("letspin");
        
        for(int i=1;i<=NoOfPart;i++)
        {
            if(_sp_object_button[i]->getBoundingBox().containsPoint(location) && (_sp_object_button[i]->getNumberOfRunningActions() == 0) && Object_counter_scene2 <= NoOfPart)
            {
                
                this->startObjectButton(i);
                break;
            }
        }
    }
	return true;
}
void WS_Playarea::onTouchesMoved(Touch* touch, Event* event)
{
  
    
}
void WS_Playarea::onTouchesEnded(Touch* touch, Event* event)
{
   
}
void WS_Playarea::radio()
{
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    this->addChild(_speaker);
    answerRemind(10);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        _isSuggetGame = false;
        this->unschedule("suggetion_game");
        _speaker->setTouchEnabled(false);
        if (list_object.empty()) {
            answerRemind(0.0);
            return;
        }
        auto str = list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path;
        if (str == "") {
            return;
        }
        std::vector<std::string> value = {};
        value.push_back(StringUtils::format("sounds/wheel_spin/tap_on%d.mp3", random(1, 4)));
        
        value.push_back(str);

        AUDIO_ENGINE->playChain(value, [=]() {
            answerRemind(0.0);
            _speaker->setTouchEnabled(true); 
            });

        });
}
void WS_Playarea::first_effect()
{
}
void WS_Playarea::touch_true()
{
    touch_tool=false;
}
void WS_Playarea::home_Effect()
{
//     Director::getInstance()->replaceScene(TransitionFade::create(1.0,LevelScreen::createScene()));
}
void WS_Playarea::playSound(std::string str)
{
    AUDIO_ENGINE->playEffect(str);
}
void WS_Playarea::changeImage(Sprite *sp,std::string str)
{
    sp->setTexture(this->getImage(str));
}
void WS_Playarea::spinRemind()
{
    this->schedule([=](float) {
        AUDIO_ENGINE->playSound(AUDIO_LETS_SPIN);
        touch_true();
        }, 10.0f, CC_REPEAT_FOREVER,10.0f, "letspin");
}

void WS_Playarea::answerRemind(float delay)
{
    this->scheduleOnce([=](float) {

        if (_isSuggetGame)
        {
            runActionSuggetGame();
        }
        else {
            _isSuggetGame = true;
        }
        answerRemind(10.0);
     }, delay, "suggetion_game");
}

void WS_Playarea::runActionSuggetGame()
{
    _speaker->setTouchEnabled(false);
    AUDIO_ENGINE->play("sounds/wheel_spin/tap_on1.mp3", false, 1.0f, [=]() {
        AUDIO_ENGINE->play(list_object[Object_No_scene2[Object_counter_scene2] - 1].audio_path, false, 1.0f, [=]() {
            _speaker->setTouchEnabled(true);
            });
        });
    touch_true();
}

std::string WS_Playarea::getImage(std::string str)
{
    return StringUtils::format("games/%s",str.c_str());
}
