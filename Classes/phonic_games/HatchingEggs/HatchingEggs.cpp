
#include "HatchingEggs.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
INITIALIZE_READER(HatchingEggs);
#define speed_opacity 15

std::once_flag HatchingEggs_reader;
#define CSB_NAME "csb/game/HatchingEggs/HatchingEggs.csb"

//sounds
#define chick_correct "sounds/hatchingeggs/Chicken_Correct.mp3"
#define egg_correct "sounds/hatchingeggs/Egg_Correct.mp3"
#define chick_wrong "sounds/hatchingeggs/Wrong.mp3"
#define bg_music "sounds/hatchingeggs/background.mp3"
#define end_bg "sounds/hatchingeggs/end_bg.mp3"
#define title "sounds/hatchingeggs/instruction_new.mp3"
#define chicken_ending "sounds/hatchingeggs/Ending_chicken.mp3"
#define chicken_transition "sounds/hatchingeggs/Chicken_transition.mp3"

#define number_chick  3

#define bg_game "games/hatchingeggs/eggs.png"
#define bg "games/hatchingeggs/bg.png"
#define eggs_music "sounds/hatchingeggs/hatching_eggs.mp3"

HatchingEggs * HatchingEggs::createGame(std::string json_file, bool isAutoPlay) {
	std::call_once(HatchingEggs_reader, [] {
        REGISTER_CSB_READER(HatchingEggs);
        REGISTER_CSB_READER(HatchingEggs_Item);
    });
    auto p = static_cast<HatchingEggs *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void HatchingEggs::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    number_egg = document.FindMember("number_egg")->value.GetInt();
    rapidjson::Value& data = document.FindMember("data")->value;
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && document.HasMember("audio_intro_vi")){
       auto audio_intro = document["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
    generateData(data.GetArray(), images);
    
    if(number_egg > _list_phonic_info.size()){
        number_egg = (int) _list_phonic_info.size();
    }

    if (JSON_SAFE_CHECK(document, "choi_theo_hinh_anh", Bool)) {
        _configImg = document.FindMember("choi_theo_hinh_anh")->value.GetBool();
    }

    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto board = utils::findChild<Layout*>(this, "board");
    board->setOpacity(0);

    if (FileUtils::getInstance()->isFileExist(bg_game)) {
        _backgoundGame = Sprite::create(bg_game);
        _backgoundGame->setPosition(Vec2(0, visibleSize.height / 2));
        _backgoundGame->setName("bg_game");
        this->addChild(_backgoundGame, INT_MAX - 1);

        _backgoundGame->runAction(MoveTo::create(1.0f, Vec2(visibleSize.width / 2, visibleSize.height / 2)));
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
            AUDIO_ENGINE->playSound(eggs_music, false, [=] {
                _backgoundGame->runAction(MoveTo::create(1.5f, Vec2(visibleSize.width + 525, visibleSize.height / 2)));
                });
            }), NULL));
    }
    
    this->runAction(Sequence::create(DelayTime::create(4.5f), CallFunc::create([=] {
        this->loadFrames();
        board->setOpacity(255);
    }), NULL));
}

HatchingEggs * HatchingEggs::createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay, std::string game_type) {
    std::call_once(HatchingEggs_reader, [] {
        REGISTER_CSB_READER(HatchingEggs);
        REGISTER_CSB_READER(HatchingEggs_Item);
    });
    auto p = static_cast<HatchingEggs *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(list_PHONIC_INFO, game_type, isAutoPlay);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void HatchingEggs::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
}

void HatchingEggs::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        auto json_game = value_data[i].FindMember("game")->value.GetArray();
        std::vector<mj::PHONIC_INFO> _sublist;
        for(rapidjson::SizeType j = 0; j < json_game.Size(); j++){
            if (json_game[j].IsObject()) {
                auto letter = json_game[j].FindMember("item")->value.FindMember("text")->value.GetString();
                auto pImage = math::resource::ResHelper::getImage(images, letter);
                if (json_game[j].FindMember("item")->value.HasMember("is_correct") && json_game[j].FindMember("item")->value.FindMember("is_correct")->value.GetBool()) {
                    LevelData level;
                    level.ansImage = pImage->path;
                    level.ansAudio = pImage->getAudio();
                    _totalLevels.push_back(level);
                }

                mj::PHONIC_INFO phonic;
                phonic.text_full = pImage->getText("name_1");
                phonic.text_phonic = pImage->getText("name_n");
                phonic.audio_full = pImage->getAudios("name_1").front();
                phonic.audio_phonic = pImage->getAudios("name_n").front();
                phonic.image_path = pImage->path;
                phonic.video_path = "";
                phonic.highlights = {};

                if (json_game[j].FindMember("item")->value.HasMember("is_correct") && json_game[j].FindMember("item")->value.FindMember("is_correct")->value.GetBool()) {
                    q_id.push_back(j);
                }

                _sublist.push_back(phonic);
            }
        }
         _list_phonic_info.push_back(_sublist);
    }
   
}

void HatchingEggs::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->setTouchEnabled(true);
    
    AUDIO_ENGINE->playSound(bg_music);
    if(autoPlay){
        this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=]{
            this->onAutoPlay();
        }), NULL));
        
    }
}

void HatchingEggs::loadFrames(){
    
	screen_size = Director::getInstance()->getWinSize();
	float space = 120;
	float scale = 1.0f;

	if (number_egg * space > screen_size.width) {
		scale = screen_size.width / (number_egg * space);
		space *= scale;
	}
    //create eggs
    auto chip_pos = Vec2((screen_size.width-((number_egg -1)*space))/2.0f, screen_size.height*0.1);
    for(int i = 0 ; i < number_egg; i++){
        GAFWrapper* egg = GAFWrapper::createGAF("gaf/hatchingeggs/Egg/Egg.gaf");
        egg->setAnchorPoint(Point(0.5,0.5));
        this->addChild(egg, 5);
        egg->setPosition(Vec2(chip_pos.x +space*i,-screen_size.height/2));
		egg->setScale(scale);
        egg->playSequence("eggidle");
        eggs.push_back(egg);
    }
    
    float delay_egg =0.1f;
    for(int i = 0; i < eggs.size(); i++){
        eggs[i]->runAction(MoveTo::create(delay_egg,(Vec2(chip_pos.x + space*i, chip_pos.y))));
        delay_egg+= 0.12;
    }
    
    showGame();
    
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    this->addChild(_speaker);
    _speaker->setTouchEnabled(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender)
        {
            _speaker->setTouchEnabled(false);
            if (!_configImg) {
                AUDIO_ENGINE->stopSound(right_phonic);
            }
            else
            {
                if (_countImg > 0 && _countImg - 1 < _totalLevels.size()) {
                    AUDIO_ENGINE->stopSound(_totalLevels[_countImg - 1].ansAudio);
                }
            }
            AUDIO_ENGINE->stopSound(title);
            _board->setTouchEnabled(true);
            stopAudioAnswer();
            stopInActiveMode();

            if (!_configImg) {
                _audio = AUDIO_ENGINE->playSound(title, false, [=] {
                    AUDIO_ENGINE->playSound(right_phonic, false, [=] {
                        _speaker->setTouchEnabled(true);
                        inActiveMode();
                        });
                    });
            }
            else
            {
                if (_countImg > 0 && _countImg - 1 < _totalLevels.size()) {
                    _audio = AUDIO_ENGINE->playSound(title, false, [=] {
                        AUDIO_ENGINE->playSound(_totalLevels[_countImg - 1].ansAudio, false, [=] {
                            _speaker->setTouchEnabled(true);
                            inActiveMode();
                            });
                        });
                }
            }
        });
}

void HatchingEggs::showGame(){
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_name = PHONIC_MANAGER->getGameFontName();
    }
    //list phonic dung trong game
    _sublist_phonic_info = _list_phonic_info[count_game];
    
    //create board text
    auto font_sz = 200;
    _board = utils::findChild<Layout*>(this, "board");


    if (!_configImg) {
        board_text = ui::Text::create("", font_name, font_sz);
        _text = _sublist_phonic_info[q_id[count_game]].text_phonic;
        board_text->setString(_text);
        while (board_text->getVirtualRendererSize().width > _board->getContentSize().width * 0.8) {
            font_sz = board_text->getFontSize();
            board_text->setFontSize(font_sz - 10);
            CCLOG("Scale down font: %d", font_sz);
        }


        board_text->setTextVerticalAlignment(TextVAlignment::CENTER);
        board_text->setTextHorizontalAlignment(TextHAlignment::CENTER);
        board_text->setTextColor(Color4B::BLACK);
        board_text->setOpacity(0);

        _board->addChild(board_text, 3);
        board_text->setPosition(_board->getContentSize() / 2);
    }
    else
    {
        _text = _sublist_phonic_info[q_id[count_game]].text_phonic;
        std::string path = _totalLevels[_countImg].ansImage;
        _countImg ++;
        _img = ImageView::create(path);
        auto imgSize = _img->getBoundingBox().size.width;
        auto boardSize = _board->getBoundingBox().size.width;
        _img->setScale(boardSize / imgSize * 0.75);
        _img->setPosition(_board->getContentSize() / 2);
        _board->addChild(_img);
        _img->setOpacity(0);
    }


    _board->setTouchEnabled(false);
    _board->addClickEventListener(CC_CALLBACK_1(HatchingEggs::onBoardClicked, this));
    right_phonic = _sublist_phonic_info[q_id[count_game]].audio_phonic;
    //create chickens
    chickens.clear();
    mj::helper::shuffle(_sublist_phonic_info);
    chicken_root = utils::findChild<ui::Layout*>(this, "chicken_layout");
     start_pos = Vec2(screen_size.width*(0.65), screen_size.height*0.25);
    
    for(int i = 0; i < number_chick; i++){
        HatchingEggs_Item* chicken = HatchingEggs_Item::createItem(_sublist_phonic_info[i]);
        chicken_root->addChild(chicken,3);
        chicken->setPosition(chicken_root->convertToNodeSpace(Vec2(-screen_size.width/2, start_pos.y)));
        chicken->setDelegate(this);
        if(_sublist_phonic_info[i].text_phonic == _text){
            chicken->setName("right");
			chicken->setUserData(&_sublist_phonic_info[i].audio_full);
            right_audio = _sublist_phonic_info[i].audio_full;
        }
        chickens.push_back(chicken);
        
        // disable touch chicken
        utils::findChild<ui::Layout*>(chicken, "chicken_layout")->setTouchEnabled(false);
    }
    
    float delay = 0.3f;
    for(int i = 0; i < chickens.size(); i++){
        chickens[i]->runAction(MoveTo::create(delay, chicken_root->convertToNodeSpace (Vec2(start_pos.x - chickens[i]->getContentSize().width*i - 20, start_pos.y))));
        delay+= 0.2;
    }
   
    this->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=]{
        if (!_configImg) {
            board_text->runAction(FadeIn::create(0.2f));
        }
        else
        {
            _img->runAction(FadeIn::create(0.2f));
        }

        for (HatchingEggs_Item* chicken : chickens) {
            auto text = utils::findChild(utils::findChild<ui::Layout*>(chicken, "chicken_layout"), "chick_text");
            text->runAction(FadeIn::create(0.2f));
            // enable touch chicken
            utils::findChild<ui::Layout*>(chicken, "chicken_layout")->setTouchEnabled(true);
        }
        if(count_game == 0 ) {
            if (!_configImg) {
                AUDIO_ENGINE->playSound(title, false, [=] {
                    AUDIO_ENGINE->playSound(right_phonic, false, [=] {
                        if (_board) {
                            _board->setTouchEnabled(true);
                        }
                        _speaker->setTouchEnabled(true);
                        _flag = false;
                        inActiveMode();
                        });
                    });
            }
            else
            {
                if (_countImg > 0 && _countImg - 1 < _totalLevels.size()) {
                    AUDIO_ENGINE->playSound(title, false, [=] {
                        AUDIO_ENGINE->playSound(_totalLevels[_countImg - 1].ansAudio, false, [=] {
                            if (_board) {
                                _board->setTouchEnabled(true);
                            }
                            _speaker->setTouchEnabled(true);
                            _flag = false;
                            inActiveMode();
                            });
                        });
                }
            }
        }
        else
        {
            if (_board) {
                _board->setTouchEnabled(true);
            }
            _speaker->setTouchEnabled(true);
            stopInActiveMode();
            inActiveMode();
        }
    }), NULL));
}

void HatchingEggs::onBoardClicked(Ref* sender){
    AUDIO_ENGINE->stopEffect(_audio);
    _speaker->setTouchEnabled(true);
    stopAudioAnswer();
    stopInActiveMode();

    _board->setTouchEnabled(false);

    if (!_configImg) {
        AUDIO_ENGINE->playSound(right_phonic, false, [=] {
            _board->setTouchEnabled(true);
            inActiveMode();
            });
    }
    else
    {
        if (_countImg > 0 && _countImg - 1 < _totalLevels.size()) {
            AUDIO_ENGINE->stopSound(_totalLevels[_countImg - 1].ansAudio);
            _audioImg = AUDIO_ENGINE->playSound(_totalLevels[_countImg - 1].ansAudio, false, [=] {
                _board->setTouchEnabled(true);
                inActiveMode();
                });
        }
    }
}

void HatchingEggs::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
}

void HatchingEggs::onClickedDelegate(cocos2d::Ref *sender){
    if (_flag) 
        return;
    this->unscheduleAllCallbacks();
    this->stopAllActions();
    stopInActiveMode();
	
    AUDIO_ENGINE->stopEffect(_audio);
    AUDIO_ENGINE->stopSound(right_phonic);
    AUDIO_ENGINE->stopEffect(_audioImg);

    _speaker->setTouchEnabled(true);
    _board->setTouchEnabled(true);

    HatchingEggs_Item* chicken = (HatchingEggs_Item*) sender;

    if(chicken->getName()=="right"){
        //disable touch
        _speaker->setTouchEnabled(false);
        _board->setTouchEnabled(false);
        AUDIO_ENGINE->stopSound(chick_wrong);

        for(HatchingEggs_Item* chicken: chickens){
            utils::findChild<ui::Layout*>(chicken, "chicken_layout")->setTouchEnabled(false);
        }
        //animation
        Sequence* seq = Sequence::create(CallFunc::create([=]{
            chicken->playEffect(true);
            //play sound
            AUDIO_ENGINE->playSound(chick_correct);
			auto sound_chicken = (std::string*)chicken->getUserData();
            AUDIO_ENGINE->play(*sound_chicken);
        }),DelayTime::create(1.0f), CallFunc::create([=]{
            //play egg
            this->runAction(Sequence::create(CallFunc::create([=]{
                eggs[count_game]->playSequence("true");
            }), DelayTime::create(1.2f), CallFunc::create([=]{
                eggs[count_game]->playSequence("checkidle");
            }) , NULL));
            AUDIO_ENGINE->play(egg_correct);
        }) , DelayTime::create(1.5f), CallFunc::create([=]{
            //next stage
            onNextStage();
        }), NULL);
        
        this->runAction(seq);
        
    }else{
        inActiveMode();
        //chicken animation
        chicken->playEffect(false);
        //play sound
        if (CONFIG_MANAGER->isGameVMonkey()) AUDIO_ENGINE->play(chicken->_audio);
        AUDIO_ENGINE->playSound(chick_wrong);
    }
}

void HatchingEggs::stopAudioAnswer() {
    AUDIO_ENGINE->stopSound(chick_wrong);
    AUDIO_ENGINE->stopSound(chick_correct);
}

void HatchingEggs::onNextStage(){
    this->unscheduleAllCallbacks();
    float delay = 0.3f;
    for(int i = 0; i < chickens.size(); i++){
        chickens[i]->runAction(MoveTo::create(delay, chicken_root->convertToNodeSpace (Vec2(screen_size.width*1.5, start_pos.y))));
        delay+= 0.2f;
    }
    //hide board text
    if (!_configImg) {
        board_text->runAction(FadeOut::create(0.2f));
    }
    else
    {
        _img->runAction(FadeOut::create(0.2f));
    }
    
    //next game
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        for (int i = 0; i < (int) chickens.size(); i++) {
            chickens[i]->removeFromParent();
        }
        nextGame();
    }), NULL));
    
}

void HatchingEggs::onFinishGame(){
//    MJMainGBase::onFinishGame();
    AUDIO_ENGINE->stopAllAudio();
    
    auto touch_layout = cocos2d::ui::Layout::create();
    touch_layout->setContentSize(screen_size);
    touch_layout->setTouchEnabled(true);
    this->addChild(touch_layout, 190);
    touch_layout->setBackGroundColor(Color3B::BLACK);
    touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    touch_layout->setOpacity(150);
    
    if (CONFIG_MANAGER->isVietnameseStories()){
        AUDIO_ENGINE->play(end_bg);
        AUDIO_ENGINE->play(chicken_ending);
        MJMainGBase::onCloseGame();
    }else{
        GAFWrapper* ending = GAFWrapper::createGAF("gaf/hatchingeggs/ending_win/ending_win.gaf");
        this->addChild(ending, 227000);
        ending->setPosition(Vec2(screen_size.width*1.5 ,screen_size.height/2));
        ending->runAction(MoveTo::create(0.3f, screen_size/2));
        
        AUDIO_ENGINE->play(end_bg);
        AUDIO_ENGINE->play(chicken_ending);
        
        ending->play(false, [=](gaf::GAFObject *obj){
            MJMainGBase::onCloseGame();
        });
    }
    
}


void HatchingEggs::nextGame(){
    count_game++;
    if(count_game == number_egg){
        this->runAction(Sequence::create( CallFunc::create([=]{
            if (!_configImg) {
                board_text->getParent()->runAction(EaseInOut::create(MoveTo::create(0.2f, Vec2(board_text->getParent()->getPosition().x, screen_size.height * 1.5)), 1));
            }
            else
            {
                _img->getParent()->runAction(EaseInOut::create(MoveTo::create(0.2f, Vec2(_img->getParent()->getPosition().x, screen_size.height * 1.5)), 1));
            }

            float delay_egg =0.1f;
            for(GAFWrapper* egg: eggs){
                egg->runAction(MoveTo::create(delay_egg,(Vec2(egg->getPosition().x, -egg->getPosition().y))));
                delay_egg+= 0.12;
            }
        }), DelayTime::create(0.2f), CallFunc::create([=]{
                this->onFinishGame();
        }) , NULL));
        
    }else {
        showGame();
        if(!autoPlay){
//            inActiveMode();
        }
        
    }
}

void HatchingEggs::inActiveMode(){
    this->schedule([=](float) {
        if (!_configImg) {
            AUDIO_ENGINE->playSound(title, false, [=] {
                AUDIO_ENGINE->playSound(right_phonic);
                });
        }
        else
        {
            if (_countImg > 0 && _countImg - 1 < _totalLevels.size()) {
                AUDIO_ENGINE->playSound(title, false, [=] {
                    AUDIO_ENGINE->playSound(_totalLevels[_countImg - 1].ansAudio);
                    });
            }
        }
    }, 13, 100, 10, "in_activeMode");
}

void HatchingEggs::stopInActiveMode() {
    this->unschedule("in_activeMode");
    AUDIO_ENGINE->stopSound(title);
    AUDIO_ENGINE->stopSound(right_phonic);
}

#define auto_testing_delay 1.5f
void HatchingEggs::onAutoPlay()
{
    this->scheduleOnce([=](float dt) {
        this->autoPlayRun(1.5f);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
   
}

void HatchingEggs::autoPlayRun(float dt)
{
    int index =random(0, (int)chickens.size()-1);
    auto chicken = chickens[index];
    if(chicken){
       this->onClickedDelegate(chicken);
    }
    this->scheduleOnce([=](float dt) {
        this->onAutoPlay();
    }, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}

