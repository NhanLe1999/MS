
#include "Robot.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "APProfileManager.h"
INITIALIZE_READER(Robot);
#define speed_opacity 15

std::once_flag Robot_reader;
#define CSB_NAME "csb/game/Robot/Robot.csb"
#define _game_path "json/robot/"

//sounds
#define bg_music "sounds/robot/ms_bg.mp3"
#define question_sound "sounds/robot/Find the sound.mp3"
#define question_word "sounds/robot/Find the word.mp3"
#define question_vi_am "sounds/robot/question_vi_am.mp3"
#define question_vi_van "sounds/robot/question_vi_van.mp3"
#define complete "sounds/robot/win.mp3"
#define right "sounds/robot/correct.mp3"
#define wrong "sounds/robot/wrong.mp3"

#define intro "sounds/robot/intro.mp3"
#define intro_word "sounds/robot/intro_word.mp3"
#define intro_vi "sounds/robot/intro_vi.mp3"
#define open "sounds/robot/opening.mp3"

#define huzz "sounds/robot/huzz.mp3"
#define drag "sounds/robot/drag.mp3"
#define wheel_start "sounds/robot/wheel_start.mp3"
#define wheel_move "sounds/robot/wheel_move.mp3"
#define robot_sound "sounds/robot/robot_sound.mp3"
#define open_robot "sounds/robot/open_robot.mp3"


//#define audio_path "sample_game/robot/"
#define img_prefix "games/robot/"

Robot * Robot::createGame(std::string json_file, bool isAutoPlay) {
    std::call_once(Robot_reader, [] {
        REGISTER_CSB_READER(Robot);
        REGISTER_CSB_READER(Robot_Item);
    });
    auto p = static_cast<Robot *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file,isAutoPlay);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void Robot::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    if (document.HasMember("game_intro")) {
        _intro = document.FindMember("game_intro")->value.GetInt();
    }
    else
    {
        _intro = 0;
    }

    game_mode = document.FindMember("game_mode")->value.GetInt();
    if (document.HasMember("option_vm_phonic")){
        hoc_am_hoc_van = document.FindMember("option_vm_phonic")->value.GetString();
    }
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	if (game_mode == 0) game_mode = 2; // sua lai game_mode = 0 thanh choi nhung tu co is_correct = true chu ko phai chi choi 1 tu
    this->generateData(json_data.GetArray(), images);

    if (max_game > data.size()) {
        max_game = (int) data.size();
    }
    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;
//    getPhonicList();
//    wordToPhonic("book");
    this->loadFrames();
}

void Robot::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
	is_word = false;
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        rapidjson::Value& json_game = value_data[i].FindMember("game")->value;
        std::vector<Robot_Item::Phonic> phonics;

        for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {

            rapidjson::Value& json_item = json_game[j].FindMember("item")->value;
            auto letter = json_item.FindMember("text")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
			if (pImage->getComparisionValue("role") == 1) is_word = true;
            Robot_Item::Phonic phonic;
            phonic.text = pImage->getText("name_1");// json_game[j].FindMember("item")->value.FindMember("text")->value.GetString();
            phonic.audio = pImage->getAudios("name_1").front();//json_game[j].FindMember("item")->value.FindMember("audio")->value.GetString();
            phonic.img_path = pImage->path;
            if(pImage->isExist("name_n")){
                phonic.audio = pImage->getAudios("name_n").front();
            }
			phonic.pair = pImage->getComparisionValue("pair");
			if (json_game[j].FindMember("item")->value.HasMember("is_right")) {
				phonic.is_right = json_game[j].FindMember("item")->value.FindMember("is_right")->value.GetBool();
			}
			else {
				phonic.is_right = false;
			}

            phonics.push_back(phonic);
        }
        data.push_back(phonics);
    }
}

void Robot::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);

    if(autoPlay){
        this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=]{
            this->onAutoPlay();
        }), NULL));
    }
}

void Robot::loadFrames(){

    //root layout
    root_layout = utils::findChild<Layout*>(this, "root_panel");

    //drop layout
    box = Layout::create();
    box->setContentSize(screen_size*0.3);
    box->setAnchorPoint(Point(0.5, 0.5));
    root_layout->addChild(box);
    box->setPosition(Vec2(screen_size.width/2, screen_size.height*0.6));

    //create speaker
    loa = Button::create("games/robot/loa.png");
    root_layout->addChild(loa);
    loa->setPosition(Vec2(40, screen_size.height - 40));
    loa->addClickEventListener(CC_CALLBACK_1(Robot::onSpeakerClicked, this));

    //create robot
    robot = GAFWrapper::createGAF("gaf/robot/robot_idle_wrong/robot_idle_wrong.gaf");
    root_layout->addChild(robot, 2);
    robot->playSequence("idle", true);
    robot->setName("robot");

    item_layout = utils::findChild<Layout*>(root_layout, "item_layout");
    item_layout->setLocalZOrder(10);

    //bg music
    this->playBackgroundMusic(bg_music, 0.65);

    showGame();
    itemOverLay();
}

void Robot::showGame(){

    //reset
    phonics.clear();
    items.clear();
    right_ans = 0;
    right_count =0;
    wrong_count =0;
    item_layout->removeAllChildren();


    robot->setVisible(true);
    robot->setPosition(Vec2 (-screen_size.width ,root_layout->getContentSize().height*0.65));


    //game data
    phonics = data[count_game];
    mj::helper::shuffle(phonics);
	q_id = 0;
	if (game_mode == 2) {
		for (int i = 0; i < phonics.size(); i++) {
			if (phonics[i].is_right) {
				q_id = i;
				break;
			}
		}
	}

    auto item_font_size = getItemFontSize();

    //create items
    for (int i = 0; i < phonics.size(); i++) {
//        std::string img_path = img_prefix + cocos2d::StringUtils::toString(i) + ".png";
        Robot_Item* item = Robot_Item::createItem(phonics[i], phonics[i].img_path);
        item->setDelegate(this);
        item->setAnchorPoint(Point(0.5,0.5));
        item->setHint(phonics[i].is_right);
        item->setAudio(phonics[i].audio);
        if(count_game == 0){
            //item->setEnabled(false);
        }
        item->setTag(0);
        if(phonics[i].is_right && game_mode == 0){
            right_ans++;
            right_audio = phonics[i].audio;
			right_pair = phonics[i].pair;
            item->setTag(1);
        }

		if (item->getPhonic().text.size() >= 5) {
			item->setFontSize(item_font_size*0.8);
		}
		else {
			item->setFontSize(item_font_size);
		}
        items.push_back(item);
    }

    auto pos_x = (screen_size.width / (phonics.size()+1));
    mj::helper::shuffle(items);
    for (int i = 0; i < items.size(); i++) {
        auto item = items[i];
        item_layout->addChild(item, 2);
        item->setPosition(Vec2(pos_x * (i+1),  item_layout->getContentSize().height*0.5));
    }
    CCLOG("item layout pos: %f, %f", item_layout->getPosition().x, item_layout->getPosition().y);

    float delay_time = 1.0f;
    float delay_time1 = 1.0f;
    if(count_game == 0){
        delay_time = 4.0f;
        delay_time1 = 6.5f;
    }

    this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]{
        //audio intro + opening
        if(count_game == 0){
            if (CONFIG_MANAGER->isGameVMonkey()){
                AUDIO_ENGINE->play(intro_vi);
            }else{
                if (_intro == 0) {
                    AUDIO_ENGINE->play(intro);
                }
                else
                {
                    AUDIO_ENGINE->play(intro_word);
                }
            }
            
        }

        AUDIO_ENGINE->play(wheel_start);
        AUDIO_ENGINE->play(robot_sound);

        //move robot
        robot->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.8f, Vec2(screen_size.width/2, screen_size.height*0.65))), NULL));

    }), DelayTime::create(delay_time), CallFunc::create([=]{
        /*if(count_game == 0){
            AUDIO_ENGINE->play(open);
        }*/
        robot->runAction(Repeat::create(Sequence::create(MoveTo::create(0.5f, Vec2(screen_size.width/2 -50, screen_size.height*0.65)), MoveTo::create(0.8f,Vec2(screen_size.width/2, screen_size.height*0.65)),MoveTo::create(0.5f,Vec2(screen_size.width/2 +50, screen_size.height*0.65)),MoveTo::create(0.8f,Vec2(screen_size.width/2, screen_size.height*0.65)), NULL) , 100000));

    }), DelayTime::create(delay_time1), CallFunc::create([=]{
        //move items

		//this->scheduleOnce([=](float dt) {
			item_layout->runAction(EaseBackOut::create(MoveTo::create(0.3f, Vec2(item_layout->getPositionX(), 0))));
		//}, 1.5f, cocos2d::StringUtils::format("show item"));
        AUDIO_ENGINE->play(huzz);
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (hoc_am_hoc_van == "hoc_am")
                AUDIO_ENGINE->play(question_vi_am);
            else
                AUDIO_ENGINE->play(question_vi_van);
        }else{
            this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                if (_intro == 0) {
                    AUDIO_ENGINE->play(question_sound);
                }
                else
                {
                    AUDIO_ENGINE->play(question_word);
                }
                }), NULL));
        }
		

    }),DelayTime::create(3.0f), CallFunc::create([=]{
        if(game_mode != 0){
			if (q_id < phonics.size()) {
				right_audio = phonics[q_id].audio;
				right_pair = phonics[q_id].pair;
			}
        }
        q_audio_id = AUDIO_ENGINE->play(right_audio, false, 1.0, [=]{
            loa->setTouchEnabled(true);
            this->inActiveMode(10.0f);
            removeItemOverLay();
        });

    }), NULL));
}

void Robot::onExitTransitionDidStart(){
//    AUDIO_ENGINE->stopAllAudio();
    cocos2d::ui::Layout::onExitTransitionDidStart();
}

void Robot::onExit()
{
    this->unschedule("in_active_mode");
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onExit();
}

void Robot::onSpeakerClicked(cocos2d::Ref *sender){
    stopInActiveMode();
    loa->setTouchEnabled(false);
    this->scheduleOnce([=](float dt) {
        if (_intro == 0) {
            AUDIO_ENGINE->playSound(question_sound, false, [=] {
                AUDIO_ENGINE->playSound(right_audio, false, [=] {
                    loa->setTouchEnabled(true);
                    inActiveMode(10.5f);
                    });
                right_audio = phonics[q_id].audio;
                right_pair = phonics[q_id].pair;
                });
        }
        else
        {
            AUDIO_ENGINE->playSound(question_word, false, [=] {
                AUDIO_ENGINE->playSound(right_audio, false, [=] {
                    loa->setTouchEnabled(true);
                    inActiveMode(10.5f);
                    });
                right_audio = phonics[q_id].audio;
                right_pair = phonics[q_id].pair;
                });
        }
        }, 0.f, "continue");

}

void Robot::stopAudioSpeaker() {
    if (_intro == 0) {
        AUDIO_ENGINE->stopSound(question_sound);
        AUDIO_ENGINE->stopSound(right_audio);
    }
    else
    {
        AUDIO_ENGINE->stopSound(question_word);
        AUDIO_ENGINE->stopSound(right_audio);
    }
}

void Robot::onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type, int item_pair,std::string audio_path){

    this->unscheduleAllCallbacks();
    stopInActiveMode();

    Robot_Item* item = static_cast<Robot_Item*>(sender);
    item->getParent()->stopAllActions();
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
//            CCLOG("began");
            //play gaf chon
            robot_right = GAFWrapper::createGAF("gaf/robot/robot_Drag_drop/robot_Drag_drop.gaf");
            root_layout->addChild(robot_right, 11);
            robot_right->setPosition(robot->getPosition());
            robot->pause() ;
            robot->pauseAnimation();
            robot->setOpacity(0);
            item->setGlobalZOrder(100);
            old_pos = item->getParent()->getPosition();

            for(auto other: items){
                if(other != item){
                    other->setEnabled(false);
                }
            }

            //music drag
            AUDIO_ENGINE->play(drag);
            robot_right->setFps(40);
            robot_right->playSequence("drag", false, [=](gaf::GAFObject * obj, const std::string & callback_name){
                robot_right->setFps(30);
                robot_right->playSequence("idle_open", true);
            });

            break;

        case cocos2d::ui::Widget::TouchEventType::MOVED:
//            CCLOG("moved");
            item->getParent()->setPosition(item_layout->convertToNodeSpace(((Layout*) sender)->getTouchMovePosition()));

            break;

        default:
//            CCLOG("ended or canceled");
            AUDIO_ENGINE->stopEffect(q_audio_id);
            item->setGlobalZOrder(10);

            if(box->getBoundingBox().containsPoint(item->getParent()->getPosition()) && ((game_mode !=0 &&  (utils::findChild<Text*>(item, "text_audio")->getString() == right_audio || (item_pair == right_pair && right_pair != -1))) || (game_mode == 0 && item->getParent()->getTag() == 1))){
                CCLOG("hint: %i", item->getHint());
                itemOverLay();
                right_count++;
                _flag2 = false;
                robot_right->playSequence("drop", false, [=](gaf::GAFObject * obj, const std::string & callback_name){
                    robot_right->removeFromParent();
                    robot->resume();
                    robot->setOpacity(255);
                    robot->playSequence("idle", true);
                    for(auto other: items){
                        if(other != item){
                            other->setEnabled(true);
                        }
                    }
                });

                //right effect
                right_drag(item);

                hideHint();
                wrong_count = 0;
                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
                    this->onNextStage();
                    _flag2 = true;
                }), NULL));
            }else{

                if (ap::ProfileManager::getInstance().getInformationOfProfile().age < _age_check) {
                    if (_flag2 && (game_mode != 0 && (utils::findChild<Text*>(item, "text_audio")->getString() == right_audio || (item_pair == right_pair && right_pair != -1))) || (game_mode == 0 && item->getParent()->getTag() == 1)) {
                        CCLOG("ok");
                        itemOverLay();
                        for (auto other : items) {
                            if (other != item) {
                                other->setEnabled(false);
                            }
                        }
                        loa->setTouchEnabled(false);
                        right_count++;
                        item->runAction(Sequence::create(MoveTo::create(1.0f, item->convertToNodeSpace(Vec2(screen_size/2) + Vec2(10,50))), CallFunc::create([=] {
                            }), CallFunc::create([=] {
                                if (robot_right) {
                                    robot_right->playSequence("drop", false, [=](gaf::GAFObject* obj, const std::string& callback_name) {
                                        robot_right->removeFromParent();
                                        robot->resume();
                                        robot->setOpacity(255);
                                        robot->playSequence("idle", true);
                                        });
                                }

                                right_drag(item);

                                hideHint();
                                wrong_count = 0;
                                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                                    this->onNextStage();
                                    _flag2 = true;
                                    }), NULL));
                                }), NULL));
                    }
                    else
                    {
                        inActiveMode(10.5f);
                        loa->setTouchEnabled(true);
                        robot->resume();
                        AUDIO_ENGINE->stopSound(audio_path);
                        AUDIO_ENGINE->playSound(audio_path, false);
                        if (box->getBoundingBox().containsPoint(item->getParent()->getPosition())) {
                            //wrong effect
                            wrong_drag(item, old_pos, true);

                        }
                        else {
                            wrong_drag(item, old_pos, false);

                        }

                        wrong_count++;
                        if (wrong_count >= 4 && items.size() >= 5) {
                            showHint();
                        }
                    }
                }
                else
                {
                    inActiveMode(10.5f);
                    removeItemOverLay();
                    loa->setTouchEnabled(true);
                    robot->resume();
                    AUDIO_ENGINE->stopSound(audio_path);
                    AUDIO_ENGINE->playSound(audio_path, false);
                    if (box->getBoundingBox().containsPoint(item->getParent()->getPosition())) {
                        //wrong effect
                        wrong_drag(item, old_pos, true);

                    }
                    else {
                        wrong_drag(item, old_pos, false);

                    }

                    wrong_count++;
                    if (wrong_count >= 4 && items.size() >= 5) {
                        showHint();
                    }
                }
            }

            break;
    }
}

void Robot::right_drag(Robot_Item* item){
    //keo dung
    _flag = false;
    Text* text = utils::findChild<Text*>(item, "text_");
    text->retain();
    text->removeFromParent();

    text->setFontSize(font_size*2);
    text->setScale(0.6);
    root_layout->addChild(text, 10);
    text->setGlobalZOrder(100);
    text->release();
    //item disappear
    item->removeFromParent();
    text->setPosition(screen_size/2);

    float scale = std::min(3.5f,0.8f * screen_size.width / text->getVirtualRendererSize().width);

    text->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5f, scale), CallFunc::create([=]{
        //phao hoa
        GAFWrapper* star = GAFWrapper::createGAF("gaf/effect/effec.gaf");
        root_layout->addChild(star,99);
        star->setPosition(text->getPosition());
        star->play(false, [=](gaf::GAFObject* obj){
            star->removeFromParent();
        });

        //music right
        AUDIO_ENGINE->play(right);

    }), NULL), FadeOut::create(0.1f), CallFunc::create([=]{
        AUDIO_ENGINE->play(right_audio);
        text->removeFromParent();
    }) , NULL));
    CCLOG("cham");
}

void Robot::wrong_drag(Robot_Item* item, Vec2 old_pos, bool inbox){
    //keo sai
    robot_right->removeFromParent();
    robot->setOpacity(255);
   
    //play wrong robot
    if(inbox){
        AUDIO_ENGINE->play(wrong);

        robot->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string & callback_name){
            robot->playSequence("idle", true);
        });
    }else{
        robot->playSequence("idle", true);
    }

    //return item
    item->getParent()->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.2f, old_pos)), CallFunc::create([=]{
        for(auto other: items){
            other->setEnabled(true);
        }
    }), NULL)) ;
}

void Robot::showHint(){
    for(auto item: items){
        if(item->getAudio() == right_audio){
            item->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.2f, 1.2), ScaleTo::create(0.2f, 0.8), ScaleTo::create(0.2f, 1), NULL), 10000));
        }
    }
}

void Robot::hideHint(){
    for(auto item: items){
        item->stopAllActions();
        item->setScale(1);
    }
}

void Robot::onNextStage(){
    this->unscheduleAllCallbacks();

    if(game_mode == 0){
        //mode 0
        //next answer
        if(right_count == right_ans){
            item_layout->runAction(EaseBackIn::create(MoveTo::create(0.3f, Vec2(item_layout->getPositionX(), -screen_size.height*1.2))));
            robot->setVisible(false);
            nextGame();
        }
    }else{
        //mode 1
        //next question
		if (game_mode == 2) {
			while (1) {
				q_id++;
				if (q_id >= phonics.size()) break;
				if (phonics[q_id].is_right) break;
			}
		}
		else {
			if (game_mode == 1) {
				q_id++;
			}
		}
        if(q_id == (int) phonics.size()){
            item_layout->runAction(EaseBackIn::create(MoveTo::create(0.3f, Vec2(item_layout->getPositionX(), -screen_size.height*1.2))));
            robot->setVisible(false);
            nextGame();
        }else{
			this->scheduleOnce([=](float dt) {
                auto audio_id = -1;
                if (_intro == 0) {
                    audio_id = cocos2d::experimental::AudioEngine::play2d(question_sound);
                }
                else
                {
                    audio_id = cocos2d::experimental::AudioEngine::play2d(question_word);
                }
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                    q_audio_id = AUDIO_ENGINE->playSound(right_audio, false, [=] {
                        removeItemOverLay();
                        loa->setTouchEnabled(true);
                        for (auto other : items) {
                            other->setEnabled(true);
                        }
                        });
				});
			}, 1.f,"continue");
            right_audio = phonics[q_id].audio;
			right_pair = phonics[q_id].pair;
            inActiveMode(10.5f);
        }
    }
}

void Robot::onFinishGame(){
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGame();

}

void Robot::nextGame(){

    count_game++;
    robot_win = GAFWrapper::createGAF("gaf/robot/robot_win/robot_win.gaf");

    //transition
    this->runAction(Sequence::create(CallFunc::create([=]{
        root_layout->runAction(ScaleTo::create(0.4, 1.8));
        //play win gaf
        root_layout->addChild(robot_win, 12);
        robot_win->setPosition(robot->getPosition());
        robot_win->playSequence("win", true);

        auto pos = root_layout->getPosition();
        root_layout->runAction(Sequence::create(DelayTime::create(0.6), Repeat::create(Sequence::create(MoveTo::create(0.1f, Vec2(pos.x-15, pos.y)), MoveTo::create(0.1f, Vec2(pos.x+15, pos.y)), MoveTo::create(0.1f, Vec2(pos.x-10, pos.y-8)), MoveTo::create(0.1f, Vec2(pos.x +10, pos.y+8)), MoveTo::create(0.1f, Vec2(pos.x, pos.y)),NULL), 4), NULL));
        AUDIO_ENGINE->play(complete);

    }), DelayTime::create(1.8), CallFunc::create([=]{
        //transition
        root_layout->runAction(ScaleTo::create(0.4, 1));
        robot_win->runAction(EaseBackIn::create(MoveTo::create(1.6f, Vec2(screen_size.width * 1.5, screen_size.height*0.65))));

    }), DelayTime::create(1.5f), CallFunc::create([=]{
        if(count_game == max_game){
            this->onFinishGame();
        }else {
            showGame();
            robot->stopAllActions();
            if(!autoPlay){
            }
        }
    }), NULL));

}

void Robot::showTapGuideGame(float delay_time,std::string gaf_guide_path){
    if (_flag && ap::ProfileManager::getInstance().getInformationOfProfile().age >= _age_check) {
        this->scheduleOnce([=](float) {
            Robot_Item* it = nullptr;

            for (auto item : items) {
                if (item && utils::findChild<Text*>(item, "text_audio")->getString() == right_audio) {
                    it = item;
                    break;
                }
            }
            if (it) {
                auto hand = GAFWrapper::createGAF(gaf_guide_path);
                auto it_pos = it->getPosition();
                it->addChild(hand, 19000, "guide_gaf");
                hand->setGlobalZOrder(100);
                hand->setScale(0.5f);
                hand->setPosition(Vec2(it->getContentSize().width * 0.3, it->getContentSize().height * 0.3));

                it->runAction(Sequence::create(MoveTo::create(0.7f, item_layout->convertToNodeSpace(box->getPosition())), CallFunc::create([=] {
                    hand->removeFromParent();
                    for (auto item : items) {
                        item->setEnabled(true);
                    }
                    }), EaseBackOut::create(MoveTo::create(0.3f, it_pos)), NULL));
            }
            }, delay_time, "guide_gaf");
    }

    this->scheduleOnce([=](float dt) {
        if (_intro == 0) {
            AUDIO_ENGINE->playSound(question_sound, false, [=] {
                AUDIO_ENGINE->playSound(right_audio);
                right_audio = phonics[q_id].audio;
                right_pair = phonics[q_id].pair;
                });
        }
        else
        {
            AUDIO_ENGINE->playSound(question_word, false, [=] {
                AUDIO_ENGINE->playSound(right_audio);
                right_audio = phonics[q_id].audio;
                right_pair = phonics[q_id].pair;
                });
        }
        }, 1.f, "continue");
    

}

void Robot::hideTapGuideGame(){
	this->unschedule("guide_gaf");
}

void Robot::inActiveMode(float delay_time){
    this->schedule([=](float){
        this->showTapGuideGame();
    }, 12.5, 100, delay_time, "animated_guide");
}

void Robot::stopInActiveMode() {
    this->unschedule("animated_guide");
    if (_intro == 0) {
        AUDIO_ENGINE->stopSound(question_sound);
        AUDIO_ENGINE->stopSound(right_audio);
    }
    else
    {
        AUDIO_ENGINE->stopSound(question_word);
        AUDIO_ENGINE->stopSound(right_audio);
    }
}

#define auto_testing_delay 1.5f
void Robot::onAutoPlay()
{
    this->scheduleOnce([=](float dt) {
        this->autoPlayRun(1.5f);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Robot::autoPlayRun(float dt)
{
    this->scheduleOnce([=](float dt) {
        this->onAutoPlay();
    }, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}

int Robot::getItemFontSize(){
    string longest = "";
    for(auto p: phonics){
        if(p.text.length() > longest.length()){
            longest = p.text;
        }
    }
    auto size = font_size;
    Text* example = Text::create(longest, font_name, size);
    while (example->getVirtualRendererSize().width > 200 * 0.9) {
        size = size-2;
        example->setFontSize(size);
    }
    return size;
}

void Robot::itemOverLay() {
    auto screen_size = Director::getInstance()->getVisibleSize();
    Layout* overlay = Layout::create();
    overlay->setContentSize(Size(screen_size.width, screen_size.height));
    overlay->setAnchorPoint(Vec2(0.5, 0.5));
    overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
    overlay->setName("overlay");
    overlay->setGlobalZOrder(INT_MAX - 1);
    this->addChild(overlay, INT_MAX - 1);
    overlay->setTouchEnabled(true);
    overlay->addClickEventListener([=](cocos2d::Ref* sender) {
        CCLOG("ok");
        });
}

void Robot::removeItemOverLay() {
    while (cocos2d::utils::findChild(this, "overlay"))
    {
        this->removeChildByName("overlay");
    }
}