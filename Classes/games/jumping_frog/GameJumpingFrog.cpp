
#include "GameJumpingFrog.h"
#include "MJ_AnimationCache.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#define posItem1 0.2
#define posItem2 0.5
#define posItem3 0.8
INITIALIZE_READER(MJGameJumpingFrog);

USING_NS_CC;

void MJGameJumpingFrog::initReader() {
    
}

std::once_flag jumping_frog_reader;

mj::GameDataSet<mj::JumpingFrogDataSet> MJGameJumpingFrog::generateData(rapidjson::Value value_data, std::string resource){
	auto images = math::resource::ResHelper::getImages("icons.json");
	
	mj::GameDataSet<mj::JumpingFrogDataSet> data;
    mj::JumpingFrogDataSet set;
    
    auto arrObjects = value_data["text_info"].GetArray();
    for (int i = 0; i < (int)arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			resource + img_nature->path,
			resource + img_nature->getAudio()
		};
        set.objects.push_back(game_object);
    }
    
    data.sets.push(set);
    
    return data;
}

mj::GameDataSet<mj::JumpingFrogDataSet> MJGameJumpingFrog::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::JumpingFrogDataSet> data;
    mj::JumpingFrogDataSet set;

    auto arrObjects = value_map["text_info"].asValueVector();
    for (auto it : arrObjects) {
        auto obj = it.asValueMap();
        mj::GameObject game_object = {
            obj["text"].asString(),
            resource + obj["img_path"].asString(),
            resource + obj["audio_path"].asString()
        };
        set.objects.push_back(game_object);
    }

    data.sets.push(set);

    return data;
}

mj::GameDataSet<mj::JumpingFrogDataSet> MJGameJumpingFrog::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::JumpingFrogDataSet> data;
	mj::JumpingFrogDataSet set;

	auto arrObjects = value_data["text_info"].GetArray();
	for (int i = 0; i < (int)arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			img_nature->path,
			img_nature->getAudio()
		};
		set.objects.push_back(game_object);
	}
    if (value_data.HasMember("option_vm_phonic")){
        set.hoc_am_hoc_van = value_data.FindMember("option_vm_phonic")->value.GetString();
    }
	data.sets.push(set);

	return data;
}

MJGameJumpingFrog * MJGameJumpingFrog::createGame(mj::GameDataSet<mj::JumpingFrogDataSet> data) {
    std::call_once(jumping_frog_reader, [] {
        REGISTER_CSB_READER(MJGameJumpingFrog);
        REGISTER_CSB_READER(MJGameJumpingFrog_Item);
    });
    auto p = static_cast<MJGameJumpingFrog *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/game/jumpingfrog/GameJumpingFrog.csb"));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingFrog::didLoadFromCSB(mj::GameDataSet<mj::JumpingFrogDataSet> data) {
    win_size = Director::getInstance()->getWinSize();
    this->setContentSize(win_size);
    ui::Helper::doLayout(this);
    
    _data = data;
    _content_layout = this->getChildByName<Layout *>("content");
    _content_layout->setLocalZOrder(100);
    auto bg = this->getChildByName("bg");
    if (bg->getContentSize().width < win_size.width) {
        bg->setScale((win_size.width+30)/bg->getContentSize().width);
    }
    
    mj::AnimationCache::startSession();
    this->getChildByName("btrefresh")->setLocalZOrder(101);
    this->getChildByName("text")->setLocalZOrder(101);
}

void MJGameJumpingFrog::onEnter(){
    _current_set = _data.sets.front();
    for(auto word : _current_set.objects){
        if(word.text.length() > 4){
            font_sz = 60;
        }
    }
    
    MJGameBase::onEnter();
    setInputConfigGame();
    this->runAction(Sequence::create(DelayTime::create(_time_delay-0.25f), CallFunc::create([=]{
        if(MJDEFAULT->getBoolForKey(key_background_music)){
            AUDIO_ENGINE->playBackgroundMusic("sounds/jumpingfrog/jumpingfrog_bg.mp3");
        }
        AUDIO_ENGINE->playEffect("sounds/jumpingfrog/jumpingfrog_dekeu.mp3", true);
        loadFrames();
    }), NULL));
    
}

void MJGameJumpingFrog::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
    mj::AnimationCache::endSession();
}

void MJGameJumpingFrog::setInputConfigGame(){
    _gamelevel = _data.info.level;
    _numkeywords = 0; _numturnplay = 0; _count_turnplay_done = 0; _count_keywords_done = 0;
    isRunAni = true;
//    if (_gamelevel >= 4) {
//        _numkeywords = 7;
//        _numturnplay = 2;
//    }else if (_gamelevel >= 2){
//        _numkeywords = 5;
//        _numturnplay = 3;
//    }else{
//        _numkeywords = 3;
//        _numturnplay = 5;
//    }
    if (0/*CONFIG_MANAGER->isTestGame()*/) {
        _numkeywords = (int) _current_set.objects.size();
        _numturnplay = 3;
    }else {
        _numkeywords = 3;
        _numturnplay = 5;
    }

    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_current_set.objects.begin(), _current_set.objects.end(), g);
    
    auto winsize = Director::getInstance()->getWinSize();
    list_pos = {Vec2(winsize.width*posItem1, winsize.height*0.65),Vec2(winsize.width*posItem2, winsize.height*0.65),Vec2(winsize.width*posItem3, winsize.height*0.65)};
    list_id_items = {1, 2, 3};

}

void MJGameJumpingFrog::loadFrames() {
    //load item.
    addObjectPlayer();
    loadItemTurnPlay();
    runAnimStartGame();
}

void MJGameJumpingFrog::runAnimStartGame(){
    _content_layout->setPositionY(2*851);
    for (int i = 0; i < 2; i++) {
        auto img_bg = ImageView::create("games/jumpingfrog/jumpingfrog_bg.png");
        _content_layout->addChild(img_bg);
        img_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        img_bg->setPosition(Vec2(win_size.width/2, -2 -i*850));
        img_bg->setName(StringUtils::format("bgrun%d", i+1));
    }
    
    auto img_bg = ImageView::create("games/jumpingfrog/jumpingfrog_bg.png");
    _content_layout->addChild(img_bg);
    img_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    img_bg->setPosition(Vec2(win_size.width/2, -4));
    img_bg->setName(StringUtils::format("bgrun%d", 3));
    
    
   
    if (CONFIG_MANAGER->isGameVMonkey())
    {
        _content_layout->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(2.5f, Vec2(0, 0))),DelayTime::create(3.f), CallFunc::create([=]{
            _content_layout->getChildByName("bgrun1")->removeFromParent();
            _content_layout->getChildByName("bgrun2")->removeFromParent();
            _content_layout->getChildByName("bgrun3")->removeFromParent();
            this->loadGuideAnswer();
        }),NULL));
        this->runAction(Sequence::create(DelayTime::create(0.f), CallFunc::create([=]{
            auto duration_delay = 0.f;
            std::string audio_path = "";
            if (_data.info.instructions.size() == 0){
                audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
                if(CONFIG_MANAGER->isGameVMonkey())
                {
                    audio_path = item_data.hoc_am_hoc_van == "hoc_am"?"sounds/jumpingfrog/jumpingfrog_vi_am.mp3":"sounds/jumpingfrog/jumpingfrog_vi_van.mp3";
                }
            }
            else{
                audio_path = _data.info.instructions[0].audio_path;
            }
            MJGameInstructionOverlay::playInstructionAfterDelay(duration_delay+1.0f, mj::GameType(_data.info.game_id), audio_path, "");
            guide_answer = true;
        }), NULL));
    }else{
        auto title = ImageView::create("games/jumpingfrog/jumpingfrog_title.png");
           title->setPosition(win_size/2);
           this->addChild(title, 9000000);
           title->setScale(0);
        title->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)), DelayTime::create(1.0f), FadeOut::create(0.5f), CallFunc::create([=]{
            _content_layout->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(2.5f, Vec2(0, 0))), CallFunc::create([=]{
                _content_layout->getChildByName("bgrun1")->removeFromParent();
                _content_layout->getChildByName("bgrun2")->removeFromParent();
                _content_layout->getChildByName("bgrun3")->removeFromParent();
                this->loadGuideAnswer();
            }),NULL));
            title->removeFromParent();
        }), NULL));
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
            auto duration_delay = 0.5f;
            std::string audio_path = "";
            if (_data.info.instructions.size() == 0){
                audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
                if(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
                {
                    audio_path = item_data.hoc_am_hoc_van == "hoc_am"?"sounds/jumpingfrog/jumpingfrog_vi_am.mp3":"sounds/jumpingfrog/jumpingfrog_vi_van.mp3";
                    //duration_delay = 3.5f;
                }
            }
            else{
                audio_path = _data.info.instructions[0].audio_path;
            }
            MJGameInstructionOverlay::playInstructionAfterDelay(duration_delay+1.0f, mj::GameType(_data.info.game_id), audio_path, "");
            guide_answer = true;
        }), NULL));
    }
    
    
    
    
}

void MJGameJumpingFrog::loadGuideAnswer(){
    auto duration_delay = 0.5f;
    auto duration_hide = 5.0f;

    if (!guide_answer) {
		std::string audio_path = "";
		if (_data.info.instructions.size() == 0){
			audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
            if(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
            {
                audio_path = item_data.hoc_am_hoc_van == "hoc_am"?"sounds/jumpingfrog/jumpingfrog_vi_am.mp3":"sounds/jumpingfrog/jumpingfrog_vi_van.mp3";
                //duration_delay = 3.5f;
            }
		}
		else{
			audio_path = _data.info.instructions[0].audio_path;
		}
        MJGameInstructionOverlay::playInstructionAfterDelay(duration_delay+1.0f, mj::GameType(_data.info.game_id), audio_path, item_data.objects[0].audio_path);
        guide_answer = true;
    }else {
        duration_hide = 2.0;
        MJGameInstructionOverlay::playInstructionAfterDelay(duration_delay+1.0f, mj::GameType(_data.info.game_id), item_data.objects[0].audio_path);
    }
    auto guide_layout = Layout::create();
    this->addChild(guide_layout, 9000000);
    guide_layout->setContentSize(win_size);
    guide_layout->setBackGroundColor(Color3B::WHITE);
    guide_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    guide_layout->setOpacity(0);
    guide_layout->setTouchEnabled(true);
    
    auto item_land = MJGameJumpingFrog_Item::createFrame(item_data.objects[0], list_id_items[random(0, (int)list_id_items.size()-1)], font_sz);
    item_land->setAnchorPoint(Vec2(0.5, 0.5));
    item_land->setPosition(Vec2(win_size.width/2, win_size.height*0.7));
    item_land->isTouchEnable = false;
    guide_layout->addChild(item_land);
    item_land->setScale(0);
    
    guide_layout->runAction(Sequence::create(DelayTime::create(duration_delay), FadeTo::create(0.5f, 0.95*255), NULL));
    item_land->runAction(Sequence::create(DelayTime::create(duration_delay),EaseBackOut::create(ScaleTo::create(0.5f, 1.8f)), DelayTime::create(duration_hide), CallFunc::create([=]{
        guide_layout->runAction(FadeOut::create(0.3f));
    }), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=]{
        guide_layout->removeFromParent();
    }), NULL));
}

void MJGameJumpingFrog::loadItemTurnPlay(){
    list_items.clear();
    item_data.objects.clear();
    item_data.objects = _current_set.objects;
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(item_data.objects.begin()+1, item_data.objects.end(), g);
    std::shuffle(list_id_items.begin(), list_id_items.end(), g);
    std::shuffle(list_pos.begin(), list_pos.end(), g);
    
    word_answer = item_data.objects[0].text;
    
    for (int i = 0; i < 3; i++) {
        auto item_land = MJGameJumpingFrog_Item::createFrame(item_data.objects[i], list_id_items[i], font_sz);
        item_land->setAnchorPoint(Vec2(0.5, 0.5));
        item_land->setPosition(_content_layout->convertToNodeSpace(list_pos[i]));
        item_land->setDelegate(this);
        item_land->setTag(tag_item);
        tag_item++;
        
        _content_layout->addChild(item_land, 12);
        list_items.push_back(item_land);
        
        item_land->setScale(0);
        item_land->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.75f, 1.0)), CallFunc::create([=]{
            isRunAni = false;
        }), NULL));
    }
    
    this->getChildByName<Text *>("text")->setString(word_answer);
    this->scheduleOnce([=](float){
        inActiveMode();
    }, 10.0f, "start_inActiveMode");

}

void MJGameJumpingFrog::addObjectPlayer(){
    _objectplayer = GAFWrapper::createGAF("gaf/frog/frog.gaf");
    auto pos_objectplayer = Vec2(win_size.width/2, 100);
    _objectplayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _objectplayer->setPosition(pos_objectplayer);
    _content_layout->addChild(_objectplayer, 10000);
    _objectplayer->playSequence("idle", true);
    
    auto beo_start = GAFWrapper::createGAF("gaf/beo_start/beo_start.gaf");
    beo_start->setPosition(pos_objectplayer);
    _content_layout->addChild(beo_start, 1000);
    beo_start->play(true);
    
    addObjectTrangTri();
}


void MJGameJumpingFrog::addObjectTrangTri(){
    auto object_trangtri = GAFWrapper::createGAF("gaf/bubble/bubble.gaf");
    object_trangtri->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object_trangtri->setPosition(Vec2(win_size.width*0.3, win_size.height*0.75));
    
    auto object_trangtri2 = GAFWrapper::createGAF("gaf/bubble/bubble.gaf");
    object_trangtri2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object_trangtri2->setPosition(Vec2(win_size.width*0.3, win_size.height*0.4));
    
    
    auto object_trangtri3 = GAFWrapper::createGAF("gaf/bubble/bubble.gaf");
    object_trangtri3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object_trangtri3->setPosition(Vec2(win_size.width*0.7, win_size.height*0.5));
    
    this->addChild(object_trangtri, 10);
    this->addChild(object_trangtri2, 10);
    this->addChild(object_trangtri3, 10);
    
    object_trangtri->play(true);
    object_trangtri2->play(true);
    object_trangtri3->play(true);
}

void MJGameJumpingFrog::onClickItem(Ref *sender){
    if (isRunAni) return;
    isRunAni = true;
    auto item = static_cast<MJGameJumpingFrog_Item *>(sender);
    tag_item_clicked = (int) item->getTag();

    CCLOG("item = %s  | answer = %s", item->getObjectName().c_str(), word_answer.c_str());
    if (item->getObjectName() == word_answer) {//chọn đúng!
        for (int i = 0; i < 3; i++) {
            list_items[i]->isTouchEnable = false;
        }
        
        this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([=]{
            //object_player -> BAY.
            moveObjectPlayer(item->getPosition());
        }),NULL));
        
        
    }else {//chọn sai
        cocos2d::experimental::AudioEngine::play2d("sounds/jumpingfrog/jumpingfrog_wrong.mp3");
        item->whenWrongAnswer();
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
            isRunAni = false;
        }),NULL));
    }
}


void MJGameJumpingFrog::moveObjectPlayer(cocos2d::Vec2 pos_to){
    float duration_move = 1.0f;
    float h_jump = 250;
    if (isDoneGame) {
        h_jump = 300;
    }

    _objectplayer->playSequence("jump", false);
    
    auto pos_from = _objectplayer->getPosition();
    auto jump_to = JumpTo::create(duration_move, pos_to, 250, 1);
    
    auto objectplayer_scale = Sequence::create(DelayTime::create(duration_move*0.05), ScaleTo::create(duration_move*0.25, 1.5f), DelayTime::create(duration_move*0.3), ScaleTo::create(duration_move*0.35, 1.0f), NULL);
    auto objectplayer_rotation = Sequence::create(DelayTime::create(duration_move*0.85), RotateTo::create(duration_move*0.15, !isDoneGame?0:-10+random(0, 2)*10), NULL);

    
    this->runAction(Sequence::create(DelayTime::create(0.15f+duration_move), CallFunc::create([=]{
        if (!isDoneGame) {
            _content_layout->getChildByTag<MJGameJumpingFrog_Item *>(tag_item_clicked)->whentrueAnswer();
        }
        _objectplayer->runAction(Sequence::create(ScaleTo::create(0.25f, 0.9f), DelayTime::create(0.3f), ScaleTo::create(0.265f, 1.0f), CallFunc::create([=]{
            _objectplayer->playSequence("idle", true);
        }), NULL));
    }), NULL));
    
    
    _objectplayer->runAction(Sequence::create(RotateTo::create(0.3f, getAngelObjectPlayer(pos_from, pos_to)), CallFunc::create([=]{
        if (tag_item_recent == 0) {
            tag_item_recent = tag_item_clicked;
        }else {
            auto item_recent = _content_layout->getChildByTag<MJGameJumpingFrog_Item *>(tag_item_recent);
            if (item_recent) {
                item_recent->whentrueAnswer();
            }
            
            tag_item_recent = tag_item_clicked;
        
        }
        
        cocos2d::experimental::AudioEngine::play2d("sounds/jumpingfrog/jumpingfrog_frog.mp3");
    }), Spawn::create(jump_to, objectplayer_scale, objectplayer_rotation, NULL), CallFunc::create([=]{
        if (!isDoneGame) {
//            cocos2d::experimental::AudioEngine->playEffect("sounds/jumpingfrog/jumpingfrog_frog2.mp3");
            for (int i = 0; i < 3; i++) {
                if (list_items[i]->getObjectName() != word_answer) {
                    list_items[i]->text->runAction(FadeOut::create(0.5f));
                }
            }
            AUDIO_ENGINE->playEffect(item_data.objects[0].audio_path);
            _count_turnplay_done++;
            this->checkNextKeyWord();
            
        }else{//Alert WIN
            this->onFinishGame();
        }
        
    }), NULL));
    
    
    if (!isDoneGame) {
        auto content_move = MoveBy::create(duration_move, Vec2(0, -win_size.height*0.5));
        _content_layout->runAction(Sequence::create(DelayTime::create(0.25f), content_move, NULL));
    }
   
}

void MJGameJumpingFrog::checkNextKeyWord(){
    CCLOG("_count_turnplay_done = %d", _count_turnplay_done);
	if (!game_play_enable && _count_turnplay_done==3){
		pauseGame();
		return;
	}
    
    this->unschedule("inActive_Mode");
    if (_count_turnplay_done == _numturnplay) {//Done key_word
        _count_keywords_done++;
        if (_count_keywords_done == _numkeywords) {//WIN
            isDoneGame = true;
            this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
                auto pos_land_finish = _content_layout->getChildByName("land_finish")->getPosition();
                this->moveObjectPlayer(Vec2(pos_land_finish.x-50 + random(0, 3)*30, pos_land_finish.y));
            }), NULL));
        }else{//Change key_word
            _count_turnplay_done = 0;
            _current_set.objects.push_back(_current_set.objects[0]);
            _current_set.objects.erase(_current_set.objects.begin());
            this->loadItemTurnPlay();
            loadGuideAnswer();
        }
        
    }else{
        this->loadItemTurnPlay();
    }
    
    if (_count_keywords_done == _numkeywords-1 && _count_turnplay_done == _numturnplay-1) {
        auto land_finish = ImageView::create("games/jumpingfrog/jumpingfrog_finish.png");
        auto posy = win_size.height*1.15 + win_size.height*0.35 - land_finish->getContentSize().height/2;
        land_finish->setPosition(_content_layout->convertToNodeSpace(Vec2(win_size.width/2, posy)));
        _content_layout->addChild(land_finish);
        land_finish->setName("land_finish");
    }
    
    removeItemLand();
}


float MJGameJumpingFrog::getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to){
//    CCLOG("pos_objectplayer %f x %f", pos_objectplayer.x, pos_objectplayer.y);
//    CCLOG("pos_to %f x %f", pos_to.x, pos_to.y);

    float origin_line;
    if (fabs(pos_objectplayer.x - win_size.width*posItem1) < 20) {
        origin_line = win_size.width*posItem1;
    }else if (fabs(pos_objectplayer.x - win_size.width*posItem3) < 20){
        origin_line = win_size.width*posItem3;
    }else origin_line = win_size.width*posItem2;
    
    float tan_alpha = (pos_to.x - origin_line)/(pos_to.y - pos_objectplayer.y);
    float  Radian_Gocquay = atanf(tan_alpha);
    float cocosAngle = Radian_Gocquay * 180/M_PI;
    CCLOG("cocosAngle = %f", cocosAngle);
    return cocosAngle;
}

void MJGameJumpingFrog::removeItemLand(){
    if ((tag_item-tag_item_remove) > 7) {
        for (int i = tag_item - 7; i >= tag_item - 9; i--) {
            auto item_land_remove = _content_layout->getChildByTag(i);
            if (item_land_remove) {
                item_land_remove->removeFromParent();
            }
        }
        tag_item_remove += 3;
    }else{
        auto land_start = _content_layout->getChildByName("land_start");
        if (land_start) {
            land_start->removeFromParent();
        }
    }
}

void MJGameJumpingFrog::onFinishGame(){
    cocos2d::experimental::AudioEngine::play2d("sounds/jumpingfrog/jumpingfrog_win.mp3");
    auto text = Text::create("", "fonts/monkeystories_medium.ttf", 80);
    text->setPosition(this->getContentSize()/2);
    this->addChild(text, 9000000);
    text->setScale(0);
    text->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.5f, 1.0f), DelayTime::create(0.5f), CallFunc::create([=]{
        AUDIO_ENGINE->stopAllEffects();
        AUDIO_ENGINE->stopBackgroundMusic();
        this->onGameOver();
    }), NULL));
}

mj::GameDataSet<mj::JumpingFrogDataSet> MJGameJumpingFrog::loadData(){
    mj::GameDataSet<mj::JumpingFrogDataSet> data;
    mj::JumpingFrogDataSet set;
    set.objects = {
        {"Ship", "games/sample/ship.png"},
        {"Ball", "games/sample/ball.png"},
        {"Truck", "games/sample/truck.png"},
        {"Block Shape", "games/sample/cube.png"},
        {"Turtle Car", "games/sample/car.png"},
        {"Cog", "games/sample/cog.png"}
    };
    
    data.sets.push(set);
    return data;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameJumpingFrog::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameJumpingFrog::onRefresh, this);
    }
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJGameJumpingFrog::onClose, this);
    }
    return nullptr;
}

void MJGameJumpingFrog::onRefresh(cocos2d::Ref *sender) {
    auto data = this->loadData();
    this->removeFromParent();
    Director::getInstance()->getRunningScene()->addChild(MJGameBase::createGame(data));

}

void MJGameJumpingFrog::onClose(cocos2d::Ref * sender){

    this->removeFromParent();
}

void MJGameJumpingFrog::inActiveMode(){
    this->schedule([=](float){
        AUDIO_ENGINE->playEffect(item_data.objects[0].audio_path);
    }, 10.0f ,"inActive_Mode");
}
