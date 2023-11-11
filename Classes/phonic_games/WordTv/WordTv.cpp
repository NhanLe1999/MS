//
//  WordTv.h
//


#include "WordTv.h"
#include "JsonDump.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"

#define SCALE_FACTOR_TEXT 2.0f

using namespace cocos2d;
WordTv * WordTv::createGame(std::string json_file) {
    
    WordTv * obj = new WordTv();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(json_file);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void WordTv::didLoadFromCSB(std::string json_file){
    auto mtBufferString = FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document myValueData;
    bool isConvertPlistJsonV2 = false;
    if (isConvertPlistJsonV2) {
        auto myValueMap = FileUtils::getInstance()->getValueMapFromFile(json_file);
        mtBufferString = JsonDump::getInstance()->MapToString(myValueMap);
    }
    
    myValueData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(mtBufferString.c_str());
    if (myValueData.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", mtBufferString.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer myRjBuffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> myWriter(myRjBuffer);
    myValueData.Accept(myWriter);
    CCLOG("JSON: \n%s", myRjBuffer.GetString());
#endif
    
    generateData(myValueData.GetObject());
    
}

void WordTv::generateData(rapidjson::Value value_data){
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    auto myData = value_data["data"].GetArray();
    for (int i=0; i< myData.Size(); i++){

        auto letter = myData[i]["text_full"].GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);

        WORD_INFO gi;
        auto listCharacters = pImage->getTextPhonic("name_p");
        auto listAudios = pImage->getAudioPhonic("name_p");
        for (int j=0; j< listCharacters.size(); j++){
            mj::PHONIC_WORD pw;
            pw.text = listCharacters[j];
            pw.audio_path = listAudios[j];
            gi.characters.push_back(pw);
        }
        for (int j=0; j< myData[i]["list_distracting"].Size(); j++){
            auto itemId = myData[i]["list_distracting"][j]["text"].GetString();
            auto itemImage = math::resource::ResHelper::getImage(images, itemId);
            
            mj::GEAR_ITEM g_item;
            g_item.isTrueAns = (j==0);
            g_item.audio_path = itemImage->getAudios("name_1").front();
            g_item.image_path = itemImage->path;
            gi.items.push_back(g_item);
        }
        game_info.word_infos.push_back(gi);
    }
    game_info.number_level = game_info.word_infos.size();
    mj::helper::shuffle(game_info.word_infos);
    for(int i =0; i< game_info.word_infos.size(); i++){
        mj::helper::shuffle(game_info.word_infos[i].items);
    }
}

void WordTv::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->loadFrames();
}

void WordTv::loadFrames(){
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    
    auto rootLayout = cocos2d::ui::Layout::create();
    rootLayout->setContentSize(screen_size);
    rootLayout->setTouchEnabled(true);
    this->addChild(rootLayout);
    
    touchLayout = cocos2d::ui::Layout::create();
    touchLayout->setContentSize(screen_size);
    touchLayout->setTouchEnabled(true);
    this->addChild(touchLayout,1000);
    
    bg1 = cocos2d::Sprite::create("games/WordTv/bg1.png");
    bg1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg1->setPosition(screen_size/2);
    if(bg1->getContentSize().width<screen_size.width){
        bg1->setScale(screen_size.width/bg1->getContentSize().width);
    }
    this->addChild(bg1);
    
    bg2 = cocos2d::Sprite::create("games/WordTv/bg2.png");
    bg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg2->setPosition(screen_size/2);
    if(bg2->getContentSize().width<screen_size.width){
        bg2->setScale(screen_size.width/bg2->getContentSize().width);
    }
    this->addChild(bg2);
    bg2->setVisible(false);
    
    auto floor =cocos2d::Sprite::create("games/WordTv/floor.png");
    floor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    floor->setPosition(screen_size/2);
    if(floor->getContentSize().width<screen_size.width){
        floor->setScale(screen_size.width/bg2->getContentSize().width);
    }
    this->addChild(floor);
    
    bg3 = cocos2d::Sprite::create("games/WordTv/photobg2.png");
    bg3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg3->setPosition(screen_size/2);
    if(bg3->getContentSize().width<screen_size.width){
        bg3->setScale(2.0f*screen_size.width/bg3->getContentSize().width);
    }
    this->addChild(bg3, 100);
    bg3->setOpacity(0);
    
    tvLayout = cocos2d::ui::Layout::create();
    //tvLayout->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);
    tvLayout->setTouchEnabled(true);
    tvLayout->setPositionX(screen_size.width/2);
    tvLayout->setPositionY(screen_size.height - 250);
    tvLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(tvLayout,90);
    
    textLayout = cocos2d::ui::Layout::create();
    textLayout->setTouchEnabled(true);
    textLayout->setPositionX(screen_size.width/2);
    textLayout->setPositionY(screen_size.height - 150);
    textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(textLayout,101);
    
    posTextLayout = cocos2d::ui::Layout::create();
    posTextLayout->setTouchEnabled(true);
    posTextLayout->setPositionX(screen_size.width/2);
    posTextLayout->setPositionY(screen_size.height - 150);
    posTextLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(posTextLayout,101);
    posTextLayout->setVisible(false);
    
    itemLayout = cocos2d::ui::Layout::create();
    itemLayout->setTouchEnabled(true);
    itemLayout->setPositionX(screen_size.width/2);
    itemLayout->setPositionY(screen_size.height/2 - 160);
    itemLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(itemLayout,101);
    
    
    led = WordTv_Led::createItem();
    this->addChild(led,10);
    led->setPosition(Vec2(0, 50));
    
    _touchable = false;
    auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
    speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        if (_touchable) {
            _touchable = false;
            stopIntroAudioWhenPlayGame();
            inActiveMode();
            this->runAction(Sequence::create(
                CallFunc::create([=] {
                _introSpeakerSoundId = AUDIO_ENGINE->playSound(current_intro);
                }), 
                CallFunc::create([=] {
                    
                    _touchable = true;
                    }), NULL));
        }
            
        });
    this->addChild(speaker);
    
    startGame();
}

void WordTv::showIntroGame(){
    
}

void WordTv::onExitTransitionDidStar(){
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->unschedule(_audioGuidePlaybackScheduleName);

    MJMainGBase::onExitTransitionDidStart();
}

void WordTv::startGame(){
    touchLayout->setTouchEnabled(false);
    cur_level=0;
    curZ=0;
    _isStartGame = true;
    AUDIO_ENGINE->playBackgroundMusic("sounds/WordTv/music_bg.mp3", 0.75f);
    startLevel();
}

void WordTv::startLevel(){
    stopIntroAudioWhenPlayGame();
    itemLayout->removeAllChildren();
    textLayout->removeAllChildren();
    tvLayout->removeAllChildren();

    _isAudioIntroGameStart = true;
    _touchable = false;

    Tvs.clear();
    posTextLayout->removeAllChildren();
    texts.clear();
    
    float delayTime =0;
    if (cur_level>0){
        delayTime = 0.5f;
    }
    
    if (_isStartGame)
    {
        _isStartGame = false;
        this->runAction(Sequence::create(
            CallFunc::create([=] {
                if (auto gameLogo = cocos2d::Sprite::create("games/WordTv/Ten_game.png")) {
                    gameLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    gameLogo->setPosition(screen_size / 2);
                    gameLogo->setContentSize(gameLogo->getContentSize() * 1.5);
                    this->addChild(gameLogo, INT_MAX - 1);
                    gameLogo->runAction(Sequence::create(
                        DelayTime::create(1),
                        CallFunc::create([=] {
                            AUDIO_ENGINE->playSound("sounds/WordTv/name_game.mp3");
                            }),
                        DelayTime::create(1),
                                FadeOut::create(1),
                                DelayTime::create(0.5),
                                CallFunc::create([=] {
                                this->removeChild(gameLogo);
                                loadTVIcon();
                                this->scheduleOnce([=](float dt) {
                                    current_intro = "sounds/WordTv/intro1.mp3";
                                    AUDIO_ENGINE->playSound("sounds/WordTv/intro1.mp3", false, [=] {
                                        touchLayout->setTouchEnabled(false);
                                        _isAudioIntroGameStart = false;
                                        _touchable = true;

                                        this->unschedule(_audioGuidePlaybackScheduleName);
                                        stopIntroAudioWhenPlayGame();
                                        inActiveMode();
                                        });
                                    }, 0, "readIntro");
                                    }), NULL));
                }
                else {
                    loadTVIcon();
                    this->scheduleOnce([=](float dt) {
                        current_intro = "sounds/WordTv/intro1.mp3";
                        AUDIO_ENGINE->playSound("sounds/WordTv/intro1.mp3", false, [=] {
                            touchLayout->setTouchEnabled(false);
                            _isAudioIntroGameStart = false;
                            _touchable = true;

                            this->unschedule(_audioGuidePlaybackScheduleName);
                            stopIntroAudioWhenPlayGame();
                            inActiveMode();
                            });
                        }, 0, "readIntro");
                }
                }), NULL));
    }
    else {
        loadTVIcon();
        this->scheduleOnce([=](float dt) {
            current_intro = "sounds/WordTv/intro1.mp3";
            AUDIO_ENGINE->playSound("sounds/WordTv/intro1.mp3", false, [=] {
                touchLayout->setTouchEnabled(false);
                _isAudioIntroGameStart = false;
                _touchable = true;

                this->unschedule(_audioGuidePlaybackScheduleName);
                stopIntroAudioWhenPlayGame();
                inActiveMode();
                });
            }, 0, "readIntro");
    }
}

void WordTv::inActiveMode(float delay_time) {
	this->schedule([=](float) {
        AUDIO_ENGINE->playSound(current_intro, false, [=] {
            this->unschedule(_audioGuidePlaybackScheduleName);
            inActiveMode();
            });
	}, _delaytimeAudioGuidePlayback, _audioGuidePlaybackScheduleName);
}
void WordTv::stopIntroAudioWhenPlayGame()
{
    AUDIO_ENGINE->stopSound(current_intro);
    this->unschedule(_audioGuidePlaybackScheduleName);
}
void WordTv::loadTVIcon()
{
    float sizeW = 0;

    int curLevel = -1;
    if (cur_level >= game_info.word_infos.size() || cur_level < 0) {
        curLevel = game_info.word_infos.size() - 1;
    }
    else if (cur_level < 0) {
        curLevel = 0;
    }
    else {
        curLevel = cur_level;
    }

    for (int i = 0; i < game_info.word_infos.at(curLevel).characters.size(); i++) {
        auto myTV = WordTv_Tv::createItem(game_info.word_infos.at(curLevel).characters.at(i));
        myTV->color = i;
        myTV->playTv(WordTv_Tv::WORD_TV_STATE::LOCK);
        tvLayout->addChild(myTV);
        float scale = 1.0f;
        if (game_info.word_infos.at(curLevel).characters.size() * myTV->getContentSize().width > screen_size.width) {
            scale = screen_size.width / (game_info.word_infos.at(curLevel).characters.size() * myTV->getContentSize().width - 50);
        }
        myTV->setScale(scale);
        myTV->setPositionX(((float)i - (float)game_info.word_infos.at(curLevel).characters.size() / 2 + 0.5f) * myTV->getContentSize().width * scale);
        myTV->addClickEventListener([=](Ref* sender) {
            if (!_isAudioIntroGameStart)
            {
                stopIntroAudioWhenPlayGame();
                inActiveMode();
                AUDIO_ENGINE->stopAllSound();
                _touchable = false;
                if (myTV->_state == WordTv_Tv::WORD_TV_STATE::ACTIVE) {
                    _isAudioIntroGameStart = true;
                    led->playTv(WordTv_Led::LED_STATE::ACTION);
                    myTV->playTv(WordTv_Tv::WORD_TV_STATE::ACTION, [=]() {
                        if (i + 1 < Tvs.size()) {
                            _isAudioIntroGameStart = false;
                            _touchable = true;
                            Tvs.at(i + 1)->playTv(WordTv_Tv::WORD_TV_STATE::ACTIVE);
                        }
                        else {
                            this->showItems();
                        }
                        });
                }
                else if (myTV->_state == WordTv_Tv::WORD_TV_STATE::IDLE) {
                    myTV->playSoundVoice(&_touchable);
                }
            }
            });
        Tvs.push_back(myTV);
        if (i == 0) {
            myTV->playTv(WordTv_Tv::WORD_TV_STATE::ACTIVE);
        }


        auto myText = Text::create("", PhonicConfig::getInstance()->getGameFontName(), 100);
        myText->setString(myTV->_data.text);
        myText->setTextColor(cocos2d::Color4B::BLACK);
        myText->setScale(SCALE_FACTOR_TEXT);
        texts.push_back(myText);
        posTextLayout->addChild(myText, 100);
        if (i == 0) {
            myText->setPositionX(myText->getContentSize().width * myText->getScale() / 2);
        }
        else {
            myText->setPositionX(texts.at(i - 1)->getPositionX() + texts.at(i - 1)->getContentSize().width * myText->getScale() / 2 + myText->getContentSize().width * myText->getScale() / 2);
        }
        sizeW += myText->getContentSize().width * myText->getScale();
        widths.push_back(myText->getContentSize().width * myText->getScale());
    }
    widths.push_back(sizeW);
    posTextLayout->setContentSize(cocos2d::Size(sizeW, 0));
}

void WordTv::showItems(){
    _touchable = false;
    led->playTv(WordTv_Led::LED_STATE::ACTION);
    touchLayout->setTouchEnabled(true);
    itemLayout->removeAllChildren();
    items.clear();

    auto myTimeDone =0.5f + 0.3f*game_info.word_infos.at(cur_level).items.size();
    this->scheduleOnce([=](float dt){
        touchLayout->setTouchEnabled(false);
    }, myTimeDone,"done_show_item");
    
    for (int i =0; i<game_info.word_infos.at(cur_level).items.size(); i++) {
        auto item = WordTv_Item::createItem(game_info.word_infos.at(cur_level).items.at(i));
        itemLayout->addChild(item);
        items.push_back(item);
        
        item->setPositionX(-screen_size.width);
        item->runAction(Sequence::create(
            DelayTime::create((i + 1) * 0.1f),
            MoveTo::create(0.5f, Vec2(((float)(game_info.word_infos.at(cur_level).items.size() - i - 1) - (float)game_info.word_infos.at(cur_level).items.size() / 2 + 0.5f) * (item->getContentSize().width + 50), 0)),
            CallFunc::create([=]() {
                if (i == 0) {
                    auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/WordTv/wtv_photo.mp3");
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                        AUDIO_ENGINE->playSound("sounds/WordTv/intro2.mp3", false, [=] {
                            touchLayout->setTouchEnabled(false);
                            stopIntroAudioWhenPlayGame();
                            inActiveMode();
                            _touchable = true;
                            _isAudioIntroGameStart = false;
                            });
                        current_intro = "sounds/WordTv/intro2.mp3";
                        });
                }
                })
            , NULL));
        
        //click item tv
        item->addClickEventListener([=](Ref* sender){
            stopIntroAudioWhenPlayGame();

            if (item->_item_info.isTrueAns){
                for (int j=0; j< Tvs.size(); j++){
                    Tvs.at(j)->playTv(WordTv_Tv::WORD_TV_STATE::CORRECT);
                }
                item->setCorrect();
                touchLayout->setTouchEnabled(true);
                led->playTv(WordTv_Led::LED_STATE::BLINK);
                
                for (int j =0; j< 3; j++){
                    auto phao_hoa = GAFWrapper::createGAF("gaf/WordTv/effec/effec.gaf");
                    phao_hoa->setVisible(false);
                    tvLayout->addChild(phao_hoa,10000);
                    if (j==0){
                        phao_hoa->setPosition(Vec2(-screen_size.width*0.3f,screen_size.height*0.2f));
                    }
                    else if (j==1){
                         phao_hoa->setPosition(Vec2(50,-screen_size.height*0.1f));
                    }
                    else if (j==2){
                         phao_hoa->setPosition(Vec2(screen_size.width*0.32f,screen_size.height*0.2f));
                    }
                    
                    phao_hoa->runAction(Sequence::create(DelayTime::create(j*0.2f),CallFunc::create([=](){
                        phao_hoa->setVisible(true);
                        phao_hoa->play(false);
                    }), NULL));
                }
            }
            else{
                inActiveMode();
                _touchable = true;
                for (int j=0; j< Tvs.size(); j++){
                    Tvs.at(j)->playTv(WordTv_Tv::WORD_TV_STATE::WRONG);
                }
                item->setWrong();
                bg2->setVisible(true);
                led->playTv(WordTv_Led::LED_STATE::WRONG);
            }
            CCLOG("click vao item %d", i);
        });
        
        item->setCallbackW([=](){
            bg2->setVisible(false);
        });
        
        item->setCallbackC([=](){
            for(int n=0; n< Tvs.size(); n++){
                auto text = Text::create("",PhonicConfig::getInstance()->getGameFontName(), 100);
                //text->setFontName(PhonicConfig::getInstance()->getGameFontName());
                text->setString(Tvs.at(n)->_data.text);
                text->setTextColor(cocos2d::Color4B::BLACK);
                textLayout->addChild(text,100);
                
                auto globalPos = Tvs.at(n)->convertToWorldSpace(Tvs.at(n)->text->getPosition());
                text->setPosition(textLayout->convertToNodeSpace(globalPos));
                Tvs.at(n)->text->setVisible(false);
                
                
                text->runAction(Sequence::create(
                    DelayTime::create(0.1f*n), 
                    MoveTo::create(0.2f, Vec2(text->getPositionX(), texts.at(n)->getPositionY()))
                    , NULL));
                
                 auto pos1 = posTextLayout->convertToWorldSpace(texts.at(n)->getPosition());
                text->scheduleOnce([=](float dt){
                    text->stopAllActions();
                    text->runAction(Spawn::createWithTwoActions(
                        MoveTo::create(0.15f, textLayout->convertToNodeSpaceAR(pos1)), 
                        ScaleTo::create(0.2f, SCALE_FACTOR_TEXT)));
                }, 0.4f,"abc");
            }
            
            item->runAction(Sequence::create(
                Spawn::createWithTwoActions(JumpTo::create(0.8f, Vec2(0,50), 100, 1), ScaleTo::create(0.5f, 2.0f)), 
                DelayTime::create(0.5f), 
                CallFunc::create([=]() {
                    AUDIO_ENGINE->play(item->_item_info.audio_path);
                    }),
                DelayTime::create(1.5f),
                CallFunc::create([=]() {
                        if (cur_level == game_info.number_level - 1) {
                            this->unschedule("instrucsion replay");
                            AUDIO_ENGINE->stopAllAudio();
                            this->onFinishGame();
                        }
                        else {
                            cur_level++;
                            bg3->runAction(FadeOut::create(0.5f));
                            this->startLevel();
                        }
                }), nullptr));
            
            bg3->runAction(FadeIn::create(0.5f));
            
            for(int n=0; n< items.size(); n++){
                if (items.at(n)!= item){
                    items.at(n)->runAction(FadeOut::create(0.5f));
                }
            }
        });
    }
    
}

void WordTv::onExit()
{
    AUDIO_ENGINE->stopAllAudio();
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->unschedule(_audioGuidePlaybackScheduleName);
    MJMainGBase::onExit();
}




