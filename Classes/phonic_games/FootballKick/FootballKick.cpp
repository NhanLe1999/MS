//
//  FootballKick.h
//


#include "FootballKick.h"
#include "JsonDump.h"
#include "MJHelper.h"
#define num_egg 3

using namespace cocos2d;
FootballKick * FootballKick::createGame(std::string json_file) {
    
    FootballKick * obj = new FootballKick();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(json_file);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void FootballKick::didLoadFromCSB(std::string json_file){
    auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile(json_file);
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
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    generateData(value_data.GetObject(), images);
    
}

void FootballKick::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
    if (value_data.HasMember("game_mode") && value_data["game_mode"].IsInt()) {
        number_turnplay = value_data["game_mode"].GetInt();
    }

    if (value_data.HasMember("data") && value_data["data"].IsArray()) {
        auto list_item = value_data["data"].GetArray();
        for (int i = 0; i < (int) list_item.Size(); i++) {
              FOOTBALLKICK_INFO item_info;
             for (int j = 0; j < 3; j++) {
            
                auto letter = list_item[i][j].FindMember("full_text")->value.GetString();
                auto pImage = math::resource::ResHelper::getImage(images, letter);
				auto correct = false;
				if (list_item[i][j].HasMember("is_correct")) {
					correct = list_item[i][j].FindMember("is_correct")->value.GetBool();
				}
                if(correct){
                    item_info.text = pImage->getText("name_1");
                    item_info.audio_path_n = pImage->getAudios("name_1").front();
                    item_info.audio_path_p = pImage->getAudios("name_n").front();
                }else{
                    item_info.list_distracting.push_back({pImage->getText("name_1"),pImage->getAudios("name_1").front()});
                }
               
             }
            list_item_info.push_back(item_info);
        }
    }
}

void FootballKick::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->playBackgroundMusic("sounds/footballkick/football_bg.mp3", 0.8f);
    this->loadFrames();
    this->showIntroGame();
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(FootballKick::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(FootballKick::onTouchEnded, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(FootballKick::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
}

void FootballKick::onSpeakerClicked(cocos2d::Ref *sender){
    unscheduleInactiveMode();
    _audioTapOn = cocos2d::experimental::AudioEngine::play2d("sounds/footballkick/tap_on.mp3");
    cocos2d::experimental::AudioEngine::setFinishCallback(_audioTapOn, [=](int au_id, std::string path) {
        if (speaker_audio_path != "") {
            _audioInactiveMode1 = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_n);
            cocos2d::experimental::AudioEngine::setFinishCallback(_audioInactiveMode1, [=](int au_id, std::string path) {
                if (current_item.audio_path_p != "") {
                    _audioInactiveMode2 = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_p);
                    cocos2d::experimental::AudioEngine::setFinishCallback(_audioInactiveMode1, [=](int au_id, std::string path) {
                        inactiveMode();
                        });
                }
                });
        }
        });
}

void FootballKick::onExitTransitionDidStart(){
    MJMainGBase::onExitTransitionDidStart();
}

void FootballKick::loadFrames(){
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    auto root_layout = cocos2d::ui::Layout::create();
    root_layout->setContentSize(screen_size);
    this->addChild(root_layout);
    root_layout->setBackGroundColor(Color3B::YELLOW);
    root_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    
    ball_layout = cocos2d::ui::Layout::create();
    this->addChild(ball_layout, 100);
    ball_layout->setPositionX(screen_size.width/2);
    
    bg_gaf = GAFWrapper::createGAF("gaf/footballkick/football_bg/football_bg.gaf");
    this->addChild(bg_gaf, 1);
    bg_gaf->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2 - 10));
    
    goal_gaf = GAFWrapper::createGAF("gaf/footballkick/football_goal/football_goal.gaf");
    this->addChild(goal_gaf, 2);
    goal_gaf->setPosition(this->getContentSize()/2);
    
    fox_gaf = GAFWrapper::createGAF("gaf/footballkick/football_fox/football_fox.gaf");
    this->addChild(fox_gaf, 5);
    fox_gaf->setPosition(this->getContentSize()/2);
    fox_gaf->playSequence("idle", true);
    
    std::vector<Vec2> list_pos_ballscore = {Vec2(718, 424), Vec2(305, 424), Vec2(276, 424)};
    for (int i = 0; i < 3; i++) {
        auto ball_score = cocos2d::ui::ImageView::create("games/footballkick/ball.png");
        fox_gaf->addChild(ball_score);
        ball_score->setScale(0.3695f);
        ball_score->setPosition(fox_gaf->convertToNodeSpace(list_pos_ballscore[i]));
        ball_score->setName(StringUtils::format("ball_score%d", i+1));
        ball_score->setVisible(false);
    }
    gamename_gaf = GAFWrapper::createGAF("gaf/footballkick/football_gamename/football_gamename.gaf");
    this->addChild(gamename_gaf, 100000);
    gamename_gaf->setPosition(this->getContentSize()/2);
    gamename_gaf->setVisible(false);

    
    number_turnplay = min(number_turnplay, (int) list_item_info.size());
    auto min_x_score = 150;
    auto y_score = screen_size.height - 30;
    for (int i = 0; i < number_turnplay; i++) {
        auto score = cocos2d::ui::ImageView::create("games/footballkick/ball_score.png");
        this->addChild(score, 10000);
        score->setPosition(Vec2(min_x_score + i*(90), y_score));
        list_score.push_back(score);
        
        auto score_gaf = GAFWrapper::createGAF("gaf/footballkick/football_score/football_score.gaf");
        this->addChild(score_gaf, 2);
        score_gaf->setVisible(false);
        score_gaf->setPosition(score->getPosition());
        list_score_gaf.push_back(score_gaf);
    }
    
    speaker = cocos2d::ui::Button::create("games/footballkick/ball_speaker.png");
    this->addChild(speaker, 10000000);
    speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    speaker->setPosition(Vec2(5, screen_size.height-5));
    speaker->setScale(1.2f);
    speaker->addClickEventListener(CC_CALLBACK_1(FootballKick::onSpeakerClicked, this));
    
    shoot_effect = cocos2d::ui::ImageView::create("games/footballkick/foot_shooteffect.png");
    this->addChild(shoot_effect, 12);
    shoot_effect->setVisible(false);
}

void FootballKick::showIntroGame(){
    if (!CONFIG_MANAGER->isGameVMonkey()) gamename_gaf->setVisible(true);
    gamename_gaf->play(false, [=] (gaf::GAFObject *obj){
        gamename_gaf->setVisible(false);
    });
//    this->runFox();
    if (!CONFIG_MANAGER->isGameVMonkey()){
        auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/footballkick/football_gamename.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/footballkick/football_mc.mp3");
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        //            fox_gaf->stopAllActions();
                    this->onNextTurn();
                });
            });
    }else{
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(question_vi);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
    //            fox_gaf->stopAllActions();
                this->onNextTurn();
            });
    }
}

void FootballKick::addBall(){
    ball_layout->removeAllChildrenWithCleanup(true);
    list_ball.clear();
    current_item = list_item_info[index_item];
    auto list_text_ball = current_item.list_distracting;
    list_text_ball.push_back({current_item.text,current_item.audio_path_n});
    mj::helper::shuffle(list_text_ball);
    std::vector<Vec2> list_pos = {{-365, 186}, {0, 155}, {365, 186}};
    for (int i = 0; i < 3; i++) {
        auto ball = cocos2d::ui::ImageView::create("games/footballkick/ball.png");
        ball_layout->addChild(ball);
        ball->setVisible(false);
        ball->setPosition(list_pos[i]);
        ball->setTouchEnabled(false);
        ball->addClickEventListener(CC_CALLBACK_1(FootballKick::onBall, this));
        ball->setName(list_text_ball[i].second);
        ball->setTag(i);
        list_ball.push_back(ball);
        
        auto font_size = 80;
        auto text_lb = cocos2d::ui::Text::create(list_text_ball[i].first, PHONIC_MANAGER->getGameFontName(), font_size);
        text_lb->setPosition(ball->getContentSize()/2);
        text_lb->setColor(Color3B::BLUE);
        text_lb->setName("text_lb");
        ball->addChild(text_lb);
        text_lb->setOpacity(0);
        if (current_item.text == list_text_ball[i].first) {
            index_correct = i;
        }
        
        if (text_lb->getContentSize().width > ball->getContentSize().width) {
            text_lb->setFontSize(font_size * (ball->getContentSize().width*1.0/text_lb->getContentSize().width));
        }
    }
}
void FootballKick::showBall(){
    for (int i = 0; i < (int)list_ball.size(); i++) {
        list_ball[i]->setVisible(true);
        list_ball[i]->getChildByName("text_lb")->runAction(Sequence::create(DelayTime::create(i*0.15f), FadeIn::create(0.35f), DelayTime::create(0.2f), CallFunc::create([=]{
            if (i == 2) {
                speaker->setTouchEnabled(false);
                speaker_audio_path = current_item.audio_path_n;
                audio_id_p = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_n);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_p, [=](int au_id, std::string path){
                        speaker->setTouchEnabled(true);
                    this->enableTouchBall();
                });
            }
        }), NULL));
    }
}

void FootballKick::hideBall(){
    for (int i = 0; i < (int)list_ball.size(); i++) {
        list_ball[i]->setVisible(false);
    }
}

void FootballKick::disableTouchBall(){
    for (int i = 0; i < (int)list_ball.size(); i++) {
        list_ball[i]->setTouchEnabled(false);
    }
}
void FootballKick::enableTouchBall(){
    for (int i = 0; i < (int)list_ball.size(); i++) {
        list_ball[i]->setTouchEnabled(true);
    }
}

void FootballKick::onBall(cocos2d::Ref *sender){
    unscheduleInactiveMode();
    inactiveMode();
    speaker->setTouchEnabled(true);
    AUDIO_ENGINE->stopEffect(audio_id_p);
    auto ball = static_cast<cocos2d::ui::ImageView *>(sender);
    
    shoot_effect->setVisible(true);
    shoot_effect->setPosition(ball_layout->convertToWorldSpace(ball->getPosition()));
    shoot_effect->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
        shoot_effect->setVisible(false);
    }), NULL));
    
    auto ball_index = ball->getTag() + 1;//atoi(ball->getName().c_str()) - 100 + 1;
    auto text = ball->getChildByName<cocos2d::ui::Text *>("text_lb")->getString();
    CCLOG("onBall = %s", text.c_str());
    if (text == current_item.text) {//right
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_n);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        });
        speaker_audio_path = "";
        AUDIO_ENGINE->play(CONFIG_MANAGER->isVietnameseStories()?"sounds/footballkick/football_correct_vi.mp3":"sounds/footballkick/football_correct.mp3");
        this->hideBall();
        fox_gaf->stopAllActions();
        auto action_name = StringUtils::format("correct%d", ball_index);
        fox_gaf->playSequence(action_name, false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            fox_gaf->playSequence("idle", true);
            index_item++;
            if (index_item <= number_turnplay - 1) {
                this->onNextTurn();
            }else {
                AUDIO_ENGINE->stopAllAudio();
                this->onFinishGame();
            }
        });
        this->runAction(Sequence::create(DelayTime::create(2.25f), CallFunc::create([=]{
            this->runScore(ball_index);
        }), NULL));
        bg_gaf->playSequence(action_name, false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            bg_gaf->playSequence("idle", true);
            
        });
        goal_gaf->playSequence(action_name, false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            goal_gaf->playSequence("idle", true);
        });

    }else {//wrong
        if (CONFIG_MANAGER->isVietnameseStories()) cocos2d::experimental::AudioEngine::play2d(ball->getName());
        AUDIO_ENGINE->play(StringUtils::format("sounds/footballkick/football_wrong%d.mp3", ball_index));
        this->disableTouchBall();
        fox_gaf->stopAllActions();
        list_ball[ball_index-1]->setVisible(false);
        auto action_name = StringUtils::format("wrong%d", ball_index);
        fox_gaf->playSequence(action_name, false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            list_ball[ball_index-1]->setVisible(true);
            this->runFox();
            this->enableTouchBall();
        });
    }
}

void FootballKick::runFox(){
    auto khoangcach = 45;
    auto pos_right = screen_size.width/2 + khoangcach;
    auto pos_left = screen_size.width/2 - khoangcach;
    auto pos_fox = fox_gaf->getPositionX();
    auto y_fox = fox_gaf->getPositionY();
    auto max_duration = 35.0f;
    auto duration = khoangcach*max_duration/1024;
    
    if (pos_fox >= screen_size.width/2) {
        auto duration_1 = (pos_right-pos_fox)*max_duration/1024;
        fox_gaf->playSequence("move", true);
        fox_gaf->runAction(Sequence::create(MoveTo::create(duration_1, Vec2(pos_right, y_fox)), CallFunc::create([=]{
            fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                this->runFoxRepeat(duration, 1, pos_right, pos_left);
            });
        }), NULL));
    }else {
        auto duration_1 = (pos_fox - pos_left)*max_duration/1024;
        fox_gaf->playSequence("move", true);
        fox_gaf->runAction(Sequence::create(MoveTo::create(duration_1, Vec2(pos_left, y_fox)), CallFunc::create([=]{
            fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                this->runFoxRepeat(duration, 2, pos_right, pos_left);
            });
        }), NULL));
    }
}

void FootballKick::runFoxRepeat(float duration, int type, float pos_right, float pos_left){
    auto y_fox = fox_gaf->getPositionY();
    fox_gaf->playSequence("move", true);
    if (type == 1) {//move_to_left
        fox_gaf->runAction(Sequence::create(MoveTo::create(duration*2, Vec2(pos_left, y_fox)), CallFunc::create([=]{
            fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                fox_gaf->playSequence("move", true);
                fox_gaf->runAction(Sequence::create(MoveTo::create(duration*2, Vec2(pos_right, y_fox)), CallFunc::create([=]{
                    fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                        this->runFoxRepeat(duration, type, pos_right, pos_left);
                    });
                }), NULL));
            });
        }), NULL));
    }else {//move_to_right
        fox_gaf->runAction(Sequence::create(MoveTo::create(duration*2, Vec2(pos_right, y_fox)), CallFunc::create([=]{
            fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                fox_gaf->playSequence("move", true);
                fox_gaf->runAction(Sequence::create(MoveTo::create(duration*2, Vec2(pos_left, y_fox)), CallFunc::create([=]{
                    fox_gaf->playSequence("idle", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                        this->runFoxRepeat(duration, type, pos_right, pos_left);
                    });
                }), NULL));
            });
        }), NULL));
    }
}
void FootballKick::runScore(int type_goal){
    AUDIO_ENGINE->play("sounds/footballkick/football_score.mp3");
    auto ball_score = fox_gaf->getChildByName(StringUtils::format("ball_score%d", type_goal));
    ball_score->setVisible(true);
    
    Vec2 old_pos = ball_score->getPosition();
    Vec2 goal_pos = fox_gaf->convertToNodeSpace(list_score[index_item]->getPosition());
    ball_score->runAction(Sequence::create(Spawn::create(MoveTo::create(0.3f, goal_pos), ScaleTo::create(0.3f, 0.295f), NULL), CallFunc::create([=]{
        list_score_gaf[index_item]->setVisible(true);
        list_score_gaf[index_item]->play(false);
        list_score[index_item]->setVisible(false);
    }), DelayTime::create(0.1f), CallFunc::create([=]{
        ball_score->setVisible(false);
        ball_score->setPosition(old_pos);
    }), NULL));
}

void FootballKick::onNextTurn(){
    this->addBall();
    fox_gaf->playSequence("fade_in", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        this->showBall();
        fox_gaf->playSequence("idle", true);
        this->runFox();
        inactiveMode();
    });
}

void FootballKick::showImage(){
    
}

bool FootballKick::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    unscheduleInactiveMode();
    return true;
}

void FootballKick::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    inactiveMode();
    auto touch_end = this->convertToNodeSpace(touch->getLocation());
    CCLOG("touch = %f x %f", touch_end.x, touch_end.y);
    
}
void FootballKick::inactiveMode() {
    this->scheduleOnce([=](float) {
        _audioTapOn = cocos2d::experimental::AudioEngine::play2d("sounds/footballkick/tap_on.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_audioTapOn, [=](int au_id, std::string path) {
            if (speaker_audio_path != "") {
                _audioInactiveMode1 = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_n);
                cocos2d::experimental::AudioEngine::setFinishCallback(_audioInactiveMode1, [=](int au_id, std::string path) {
                    inactiveMode();
                    if (current_item.audio_path_p != "") {
                        _audioInactiveMode2 = cocos2d::experimental::AudioEngine::play2d(current_item.audio_path_p);
                        cocos2d::experimental::AudioEngine::setFinishCallback(_audioInactiveMode1, [=](int au_id, std::string path) {
                            inactiveMode();
                            });
                    }
                    });
            }
        });
    }, 10, "football_reminder");
}

void FootballKick::unscheduleInactiveMode()
{
    this->unschedule("football_reminder");
    if (_audioInactiveMode2 != -1) {
        cocos2d::experimental::AudioEngine::stop(_audioInactiveMode1);
    }
    if (_audioInactiveMode2 != -1) {
        cocos2d::experimental::AudioEngine::stop(_audioInactiveMode2);
    }
    if (_audioTapOn != -1) {
        cocos2d::experimental::AudioEngine::stop(_audioTapOn);
    }
}
