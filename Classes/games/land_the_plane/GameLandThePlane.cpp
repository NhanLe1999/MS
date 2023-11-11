
#include "GameLandThePlane.h"
#include "GameLandThePlane_Cloud.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#define posItem1 0.2
#define posItem2 0.5
#define posItem3 0.8


INITIALIZE_READER(MJGameLandThePlane);

USING_NS_CC;

void MJGameLandThePlane::initReader() {
}

mj::GameDataSet<mj::LandThePlaneDataSet> MJGameLandThePlane::generateData(rapidjson::Value value_data, std::string resource){
	auto images = math::resource::ResHelper::getImages("icons.json");
	
	mj::GameDataSet<mj::LandThePlaneDataSet> data;
    mj::LandThePlaneDataSet set;
    
    auto arrObjects = value_data["text_info"].GetArray();
    for (int i = 0; i < (int) arrObjects.Size(); i++) {
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

mj::GameDataSet<mj::LandThePlaneDataSet> MJGameLandThePlane::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::LandThePlaneDataSet> data;
    mj::LandThePlaneDataSet set;

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

mj::GameDataSet<mj::LandThePlaneDataSet> MJGameLandThePlane::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::LandThePlaneDataSet> data;
	mj::LandThePlaneDataSet set;

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

std::once_flag land_the_plane_reader;

MJGameLandThePlane * MJGameLandThePlane::createGame(mj::GameDataSet<mj::LandThePlaneDataSet> data) {
    std::call_once(land_the_plane_reader, [] {
        REGISTER_CSB_READER(MJGameLandThePlane);
        REGISTER_CSB_READER(MJGameLandThePlane_Item);
        REGISTER_CSB_READER(MJGameLandThePlane_Cloud);
    });
    
    auto p = static_cast<MJGameLandThePlane *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/game/landtheplane/GameLandThePlane.csb"));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameLandThePlane::didLoadFromCSB(mj::GameDataSet<mj::LandThePlaneDataSet> data) {
    win_size = Director::getInstance()->getWinSize();
    this->setContentSize(win_size);
    ui::Helper::doLayout(this);
    
    _data = data;
    _current_set = _data.sets.front();
    _content_layout = this->getChildByName<Layout *>("content");

}

void MJGameLandThePlane::onEnter(){
    MJGameBase::onEnter();
    setInputConfigGame();
    _content_layout->setPositionY(2000);
    this->runAction(Sequence::create(DelayTime::create(_time_delay-0.25f), CallFunc::create([=]{
        if(MJDEFAULT->getBoolForKey(key_background_music)){
            AUDIO_ENGINE->playBackgroundMusic("sounds/landtheplane/landtheplane_bg.mp3", 0.35f);
        }
        loadFrames();
    }), NULL));
}

void MJGameLandThePlane::onExitTransitionDidStart(){
    this->unschedule("inActive_Mode");
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    Layout::onExitTransitionDidStart();
}

void MJGameLandThePlane::setInputConfigGame(){
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
    list_pos = {Vec2(winsize.width*posItem1, winsize.height*0.7),Vec2(winsize.width*posItem2, winsize.height*0.7),Vec2(winsize.width*posItem3, winsize.height*0.7)};
    list_id_items = {1, 2, 3, 4, 5};
    
    for(auto word : _current_set.objects){
        if(word.text.length() > 4){
            font_sz = 55;
        }
    }

}

void MJGameLandThePlane::loadFrames() {
    //load cloud

    auto cloud = MJGameLandThePlane_Cloud::createFrame();
    cloud->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    cloud->setPosition(_content_layout->convertToNodeSpace(Vec2(win_size.width/2, 0)));
    _content_layout->addChild(cloud, 100000);
    cloud->runAction(Sequence::create(MoveBy::create(20, Vec2(0, -1700)), CallFunc::create([=]{
        cloud->removeFromParent();
    }), NULL));
    
    loadCloud();
    
    _speaker = mj::helper::addSpeaker(this);
    _speaker->addClickEventListener(CC_CALLBACK_1(MJGameLandThePlane::onSpeakerClicked, this));
    _speaker->setVisible(false);

    runAnimStartGame();
}

void MJGameLandThePlane::loadCloud(){
    
    auto cloud = MJGameLandThePlane_Cloud::createFrame();
    cloud->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    cloud->setPosition(_content_layout->convertToNodeSpace(Vec2(win_size.width/2, 1700)));
    _content_layout->addChild(cloud, 10000);
    cloud->runAction(Sequence::create(MoveBy::create(20, Vec2(0, -1700)), CallFunc::create([=]{
        loadCloud();
    }), MoveBy::create(20, Vec2(0, -1700)), CallFunc::create([=]{
        cloud->removeFromParent();
    }), NULL));
}

void MJGameLandThePlane::runAnimStartGame(){
    if (CONFIG_MANAGER->isGameVMonkey()){
        this->addObjectPlayer();
    }else{
        auto title = ImageView::create("games/landtheplane/landtheplane_title.png");
        title->setPosition(win_size/2);
        this->addChild(title, 9000000);
        title->setScale(0);
        
        title->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)), DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([=]{
            this->addObjectPlayer();
            title->removeFromParent();
        }), NULL));
    }
    
}

void MJGameLandThePlane::loadItemTurnPlay(){
    _speaker->setVisible(true);
    list_items.clear();
    item_data.objects.clear();
    item_data.objects = _current_set.objects;
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(item_data.objects.begin()+1, item_data.objects.end(), g);
    std::shuffle(list_id_items.begin(), list_id_items.end(), g);
    std::shuffle(list_pos.begin(), list_pos.end(), g);
    
    word_answer = item_data.objects[0].text;
    auto max_length = std::max(StringUtils::StringUTF8(item_data.objects[0].text).length(),StringUtils::StringUTF8(item_data.objects[1].text).length());
    max_length = std::max(max_length,StringUtils::StringUTF8(item_data.objects[2].text).length());
    for (int i = 0; i < 3; i++) {
        auto font_size = font_sz;
        if (max_length <= 4 && CONFIG_MANAGER->isGameVMonkey()){
            font_size = 70;
        }
        auto item_land = MJGameLandThePlane_Item::createFrame(item_data.objects[i], list_id_items[i], font_size);
        item_land->setAnchorPoint(Vec2(0.5, 0.5));
        item_land->setPosition(_content_layout->convertToNodeSpace(list_pos[i]));
        item_land->setDelegate(this);
        item_land->setTag(tag_item);
        tag_item++;
        
        _content_layout->addChild(item_land);
        list_items.push_back(item_land);
        
        item_land->setScale(0);
        item_land->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.75f, 1.0)), CallFunc::create([=]{
            isRunAni = false;
        }), NULL));
    }
    
    this->getChildByName<Text *>("text")->setString(word_answer);
    inActiveMode();
}

void MJGameLandThePlane::addObjectPlayer(){
    _objectplayer = GAFWrapper::createGAF("gaf/airplane/airplane.gaf");
    auto pos_objectplayer = Vec2(win_size.width/2, -200);
    _objectplayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _objectplayer->setPosition(_content_layout->convertToNodeSpace(pos_objectplayer));
    _content_layout->addChild(_objectplayer, 10000);
    _objectplayer->play(true);
    
    auto pos_to = _content_layout->convertToNodeSpace(Vec2(win_size.width/2, win_size.height*0.6));
    auto move_to1 = MoveTo::create(1.0f, pos_to);
    
    au_id_startplane = cocos2d::experimental::AudioEngine::play2d("sounds/landtheplane/landtheplane_planestart.mp3");
    if (CONFIG_MANAGER->isGameVMonkey()){
        std::string audio_path = "sounds/landtheplane/instruction_vi.mp3";
        this->scheduleOnce([=](float) {
            AUDIO_ENGINE->playSound(_data.sets.front().hoc_am_hoc_van == "hoc_van"?"sounds/landtheplane/tap_on_vi_van.mp3":"sounds/landtheplane/tap_on_vi_am.mp3");
        }, 5.0f, "tap");
        MJGameInstructionOverlay::playInstructionAfterDelay(1.0f, mj::GameType(_data.info.game_id), audio_path, "");
        guide_answer = true;
    }else{
        this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=]{
            auto duration_delay = 0.5f;
            std::string audio_path = "";
            if (_data.info.instructions.size() == 0){
                audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
            }
            else{
                audio_path = _data.info.instructions[0].audio_path;
            }
            MJGameInstructionOverlay::playInstructionAfterDelay(duration_delay+1.0f, mj::GameType(_data.info.game_id), audio_path, "");
            guide_answer = true;
        }), NULL));
    }
    
    
    _objectplayer->runAction(Sequence::create(move_to1, CallFunc::create([=]{
        _content_layout->runAction(Sequence::create(MoveTo::create(5.0f, Vec2(0, 300)), MoveTo::create(1.0f, Vec2(0, 0)), CallFunc::create([=]{
            
            this->loadItemTurnPlay();
            
            auto cloud = MJGameLandThePlane_Cloud::createFrame();
            cloud->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            cloud->setPosition(_content_layout->convertToNodeSpace(Vec2(win_size.width/2, win_size.height)));
            _content_layout->addChild(cloud, 100000);
            cloud->runAction(Sequence::create(MoveBy::create(25, Vec2(0, -2500)), CallFunc::create([=]{
                cloud->removeFromParent();
            }), NULL));
            
            

        }), DelayTime::create(0.74f), CallFunc::create([=]{
            loadGuideAnswer();
        }), NULL));
        auto pos_to3 = _content_layout->getChildByName("land_start")->getPosition();
        auto pos_to2 = pos_to3;
        pos_to2.y -= 300;
        pos_to3.y += 80;
        auto pos_to1 = pos_to2;
        pos_to1.y -= 500;
        auto move_to1 = MoveTo::create(2.5f, pos_to1);
        auto move_to2 = MoveTo::create(1.5f, pos_to2);
        auto move_to3 = MoveTo::create(0.65f, pos_to3);
        
        auto scale_11 = ScaleTo::create(0.5f, 1.3f);
        auto scale_12 = ScaleTo::create(0.65f, 1.0f);
        _objectplayer->runAction(Sequence::create(move_to1, scale_11, move_to2, Spawn::create(move_to3, Sequence::create(scale_12, NULL), NULL), CallFunc::create([=]{
            cocos2d::experimental::AudioEngine::stop(au_id_startplane);
        }),NULL));
    }), NULL));
    
}

void MJGameLandThePlane::loadGuideAnswer(){
    auto duration_delay = 0.5f;
    auto duration_hide = 5.0f;
    if (!guide_answer) {
		std::string audio_path = "";
		if (_data.info.instructions.size() == 0){
			audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
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
    auto font_size = (StringUtils::StringUTF8(item_data.objects[0].text).length()<=4 && CONFIG_MANAGER->isGameVMonkey()) ? 70: font_sz;
    auto item_land = MJGameLandThePlane_Item::createFrame(item_data.objects[0], list_id_items[random(0, (int)list_id_items.size()-1)], font_size);
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
        this->inActiveMode();
    }), NULL));
    
}


void MJGameLandThePlane::onClickLand(Ref *sender){
    hideHand();
    if (isRunAni) return;
    isRunAni = true;
    auto item = static_cast<MJGameLandThePlane_Item *>(sender);
    CCLOG("item = %s  | answer = %s", item->getObjectName().c_str(), word_answer.c_str());
    if (item->getObjectName() == word_answer) {//chọn đúng!
        for (int i = 0; i < 3; i++) {
            list_items[i]->isTouchEnable = false;
        }
        _guideShowed = true;
        moveObjectPlayer(item->getPosition());
    }else {//chọn sai
        item->addCloudWhenWrongAnswer();
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
            isRunAni = false;
        }),NULL));
    }
    inActiveMode();
}


void MJGameLandThePlane::moveObjectPlayer(cocos2d::Vec2 pos_to){
    std::vector<std::string> correctWordSound;

    _audioFly = AUDIO_ENGINE->playEffect("sounds/landtheplane/landtheplane_planestart.mp3",true,0.5f);
    correctWordSound.push_back("sounds/landtheplane/landtheplane_correct.mp3");
    correctWordSound.push_back(item_data.objects[0].audio_path);

    AUDIO_ENGINE->playChain(correctWordSound);
    auto ww = cocos2d::Director::getInstance()->getVisibleSize().height;
    float duration_move = 0.75 /ww;
    auto pos_from = _objectplayer->getPosition();
    duration_move = duration_move*pos_from.distance(pos_to);
    CCLOG("duration_move = %f", duration_move);
    auto move_to = MoveTo::create(duration_move, pos_to);
    
//    auto jump_to = JumpTo::create(duration_move, pos_to, 300, 1);
    
    auto air_scale = Sequence::create(DelayTime::create(duration_move*0.05), ScaleTo::create(duration_move*0.25, 1.7f), DelayTime::create(duration_move*0.3), ScaleTo::create(duration_move*0.35, 1.0f), NULL);
    
    auto air_rotation = Sequence::create(DelayTime::create(duration_move * 0.25), RotateTo::create(duration_move * 0.15, !isDoneGame ? 0 : -10 + random(0, 2) * 10), CallFunc::create([=] {cocos2d::experimental::AudioEngine::stop(au_id_plane); }), NULL);

    _objectplayer->runAction(Sequence::create(
        DelayTime::create(duration_move),
        RotateTo::create(0.35f, getAngelObjectPlayer(pos_from, pos_to)),
        Spawn::create(move_to, air_scale, NULL),
        air_rotation,
        CallFunc::create([=]{
            if (_audioFly != -1) {
                AUDIO_ENGINE->stopEffect(_audioFly);
            }
        if (!isDoneGame) {
            for (int i = 0; i < 3; i++) {
                if (list_items[i]->getObjectName() != word_answer) {
                    list_items[i]->text->runAction(FadeOut::create(0.5f));
                }
            }
            this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=]{
                _count_turnplay_done++;
                this->checkNextKeyWord();
            }), NULL));
            
        }else{//Alert WIN
			AUDIO_ENGINE->stopBackgroundMusic();
			AUDIO_ENGINE->stopAllEffects();
            cocos2d::experimental::AudioEngine::play2d("sounds/landtheplane/landtheplane_win.mp3");
            this->onFinishGame();
        }
        
    }), NULL));
    
    auto content_move = MoveBy::create(duration_move, Vec2(0, -win_size.height*0.5));
    _content_layout->runAction(Sequence::create(DelayTime::create(duration_move*2.5), content_move, NULL));
   
}

void MJGameLandThePlane::checkNextKeyWord(){
    CCLOG("_count_turnplay_done = %d", _count_turnplay_done);

	if (!game_play_enable && _count_turnplay_done == 3){
		pauseGame();
		return;
	}

    if (_count_turnplay_done == _numturnplay) {//Done key_word
        _count_keywords_done++;
        if (_count_keywords_done == _numkeywords) {//WIN
            isDoneGame = true;
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                this->moveObjectPlayer(_content_layout->convertToNodeSpace(Vec2(win_size.width/2-50 + random(0, 3)*30, win_size.height*1.37)));
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
        auto land_finish = ImageView::create("games/landtheplane/landtheplane_finish.png");
        
        land_finish->setPosition(_content_layout->convertToNodeSpace(Vec2(win_size.width/2, win_size.height*1.57)));//1.57 is magic number => don't change.
        _content_layout->addChild(land_finish);
    }
    
    removeItemLand();
}


float MJGameLandThePlane::getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to){
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

void MJGameLandThePlane::removeItemLand(){
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

void MJGameLandThePlane::onFinishGame(){
    this->unschedule("inActive_Mode");
    auto text = Text::create("", "fonts/monkeystories_medium.ttf", 80);
    text->setPosition(this->getContentSize()/2);
    this->addChild(text, 9000000);
    text->setScale(0);
    
    this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([=]{
        AUDIO_ENGINE->stopAllEffects();
        AUDIO_ENGINE->stopBackgroundMusic();
        this->onGameOver();
        this->unscheduleAllCallbacks();
        this->unscheduleUpdate();
        this->stopAllActions();
    }), NULL));
}

mj::GameDataSet<mj::LandThePlaneDataSet> MJGameLandThePlane::loadData(){
    mj::GameDataSet<mj::LandThePlaneDataSet> data;
    mj::LandThePlaneDataSet set;
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
ui::Widget::ccWidgetClickCallback MJGameLandThePlane::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameLandThePlane::onRefresh, this);
    }
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJGameLandThePlane::onClose, this);
    }
    return nullptr;
}

void MJGameLandThePlane::onRefresh(cocos2d::Ref *sender) {
    auto data = this->loadData();
    this->removeFromParent();
    Director::getInstance()->getRunningScene()->addChild(MJGameBase::createGame(data));

}

void MJGameLandThePlane::onClose(cocos2d::Ref * sender){
    this->removeFromParent();
}

void MJGameLandThePlane::inActiveMode() {
    hideHand();
    this->scheduleOnce([=](float) {
        if (_audioId != -1) {
            cocos2d::experimental::AudioEngine::stop(_audioId);
            }
        _audioId = cocos2d::experimental::AudioEngine::play2d(item_data.objects[0].audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_audioId, [=](int, std::string) {
            inActiveMode();
            });
        /*if (!_guideShowed) {
            showHand();
        }*/
    }, 10, "inActive_Mode");

}
void MJGameLandThePlane::onSpeakerClicked(cocos2d::Ref* sender) {
    hideHand();
    if (!item_data.objects.empty()  && item_data.objects[0].audio_path != "") {
        if (_audioId != -1) {
            cocos2d::experimental::AudioEngine::stop(_audioId);
        }
        _audioId = cocos2d::experimental::AudioEngine::play2d(item_data.objects[0].audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_audioId, [=](int, std::string) {
            inActiveMode();
        });

    }
}

void MJGameLandThePlane::showHand() {
    for (auto item : list_items) {
        if (item->getObjectName() == word_answer) {
            auto hand = GAFWrapper::createGAF("gaf/guidegame/hand2/hand2.gaf");
            item->addChild(hand, INT_MAX - 1);
            hand->setScale(0.75f);
            hand->setPosition(item->getBoundingBox().size / 2);
            hand->play(false, [=](gaf::GAFObject* obj) {
                hand->removeFromParent();
                });
            hand->setName("hand_guide");

            break;
        }
    }
}

void MJGameLandThePlane::hideHand() {
    this->unschedule("inActive_Mode");
    if (_audioId != -1) {
        cocos2d::experimental::AudioEngine::stop(_audioId);
    }
    if (auto hand=cocos2d::utils::findChild(this, "hand_guide")) {
        hand->removeFromParent();
    }
}
