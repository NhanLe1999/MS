
//
//  BalloonTheClown.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/24/18.
//

#include "BalloonTheClown.h"
#include "AudioHelper.h"
USING_NS_CC;
INITIALIZE_READER(BalloonTheClown);

std::once_flag BalloonTheClown_reader;
#define CSB_NAME "csb/BalloonTheClown/BalloonTheClown.csb"
#define _game_path "json/balloontheclown/"


BalloonTheClown* BalloonTheClown::createGame(std::string json_file, bool isAutoPlay){
    std::call_once(BalloonTheClown_reader, []{
        REGISTER_CSB_READER(BalloonTheClown);
    });
    
    auto p = static_cast<BalloonTheClown*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void BalloonTheClown::didLoadFromCSB(std::string json_file, bool isAutoPlay){
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = "fonts/danhvan.ttf";
    }
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    rapidjson::Value& json_data = document.FindMember("data")->value;
    for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
         DataInfo game;
        auto letter = json_data[i].FindMember("text")->value.FindMember("text")->value.GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);
        
        game.text = pImage->getText("name_1");
        game.audio = pImage->getAudios("name_1").front();
        game.s_audio = pImage->getAudios("name_n").front();
        game.p_audio = pImage->getAudioPhonic("name_p");
        game.phonics = pImage->getTextPhonic("name_p");
        ////for position list
        auto pos_dau = pImage->getComparisionValue("number");
        if (pos_dau != -1){
            game.co_dau = true;
            game.pos_dau = pos_dau;
            game.text_dau = game.phonics.back();
            game.phonics.pop_back();
            game.audio_dau = game.p_audio.back();
            game.p_audio.pop_back();
            game.text_khong_dau = game.phonics.back();
            game.phonics.pop_back();
            game.audio_khong_dau = game.p_audio.back();
            game.p_audio.pop_back();
        }
       
        rapidjson::Value& json_list_answer = json_data[i].FindMember("answers")->value;
        auto right_ans = json_list_answer.FindMember("right_answer")->value.GetString();
        auto right_img = math::resource::ResHelper::getImage(images, right_ans);
        game.right_answer.text = right_img->getText("name_1");
        game.right_answer.audio_path = right_img->getAudios("name_1").front();
        game.right_sub_text = (right_img->getText("name_n") != "")? right_img->getText("name_n") : right_img->getText("name_1");
        game.answers.push_back(game.right_answer);
        
        rapidjson::Value& wrong_answer = json_list_answer.FindMember("wrong_answers")->value;
        for (rapidjson::SizeType j = 0; j < wrong_answer.Size(); ++j) {
            auto wrong_ans = wrong_answer[j].GetString();
            auto aImage = math::resource::ResHelper::getImage(images, wrong_ans);
            mj::GameObject go;
            go.text = aImage->getText("name_1");
            go.audio_path = aImage->getAudios("name_1").front();
            go.image_path = aImage->path;
            game.answers.push_back(go);
        }
    
		int found = game.text.find_first_of(game.right_sub_text.at(0));
		while(found != std::string::npos) {
			bool break_while = true;
			for (int i = 0; i < game.right_answer.text.size(); ++i) {
				if (game.right_answer.text[i] == '_' ) {
					//underline_index
					continue;
				}
				else if ( (found + i) < game.text.size() && game.text[found + i] == game.right_answer.text[i]) {
					game.position.push_back(found + i);
				}
				else {
					break_while = false;
					game.position.clear();
					break;
				}
			}
			if (break_while) break;
			found = game.text.find_first_of(game.right_sub_text.at(0), found + 1);
		};

        words.push_back(game);
    }
    
	count_game = 0;
    max_game = words.size();
    
    screen_size = Director::getInstance()->getVisibleSize();
    _isAutoPlay = isAutoPlay;
    loadFrame();
}

void BalloonTheClown::loadFrame(){
	this->setContentSize(screen_size);
    root_layout = utils::findChild<Layout*>(this, "root_panel");
    _bg = utils::findChild<ImageView*>(root_layout, "bg_panel");
    
    if(_bg->getContentSize().width < screen_size.width)
    {
        _bg->setScaleX(screen_size.width / _bg->getContentSize().width);
    }
    
    goal = GAFWrapper::createGAF("gaf/balloontheclown/target/target.gaf");
    root_layout->addChild(goal,1);
    goal->setPosition(Vec2(screen_size.width*0.65, screen_size.height*0.4));
    goal->playSequence("idle", true);
    goal->setScale(0.8);
    
    cat = GAFWrapper::createGAF("gaf/balloontheclown/cat/cat.gaf");
    root_layout->addChild(cat, 10);
    cat->setPosition(Vec2(screen_size.width*0.4, -screen_size.height* 0.15));
    cat->setScale(0.8);

    //create speaker
    _touchAbleSpeaker = false;
    if (Button* speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png")) {
        speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
        speaker->addClickEventListener([=](cocos2d::Ref* sender) {
            if (_touchAbleSpeaker) {
                stopSequenceAudio(_seq);
                this->unschedule(_learningReminderScheduleName);
                w_id = audio_helper->play(word.audio);
                AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
                    _testSequenceForAudioButton = Sequence::create(
                        DelayTime::create(1.0),
                        CallFunc::create([=] {
                            if (!word.p_audio.empty()) {
                                audio_helper->play_sequence(word.p_audio, [=](string path) {}, "play phonic");
                            }
                            learningReminder();
                            }), NULL);
                    this->runAction(_testSequenceForAudioButton);
                    });
            }
            });
        this->addChild(speaker);
    }
    showIntroGame();
}

void BalloonTheClown::gameConfig(){
}

void BalloonTheClown::onStartGame() {
	//reset game
	mj::helper::shuffle(words);
	word = words.front();
	words.erase(words.begin());
	answers.clear();
	list_index.clear();

    stopSequenceAudio(_seq);

    hideTapGuideGame();

	clown = GAFWrapper::createGAF("gaf/balloontheclown/clown/clown.gaf");
	clown->setAnchorPoint(Point(0.35, 0.45));
	root_layout->addChild(clown);
	clown->setPosition(Vec2(screen_size.width*0.75, screen_size.height*0.45));
	clown->playSequence("idle0", true);
	clown->setScale(0.8);

    _touchAbleSpeaker = false;

	if (count_game == 0) {
		q_id = audio_helper->play("sounds/balloontheclown/question.mp3");
        AudioEngine::setFinishCallback(q_id, [=](int id, string path) {
			w_id = audio_helper->play(word.audio);

			AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
                learningReminder();
                _touchAbleSpeaker = true;
			});
		});
	}
	else {

			w_id = audio_helper->play(word.audio);
			AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
                _touchAbleSpeaker = true;
                learningReminder();
			});
		
	}

    
    //create word
    fullText = Layout::create();
    fullText->setAnchorPoint(Point::ANCHOR_MIDDLE);
    fullText->setContentSize(Size(300, 100));
    
    root_layout->addChild(fullText);
    fullText->setPositionX(screen_size.width*0.3);
    fullText->setPositionY(screen_size.height*0.88);
    fullText->setOpacity(0);
    
	std::vector <char> thinner = { 'f', 'i', 'j', 'l', 't', 'r', '.', ',','\'' };
	std::vector <char> thicker = { 'H', 'W', 'w', 'M', 'm', 'G', 'K', 'Q', 'O' };
    

	float pos_x = 0;
	float offset;
    for (auto i = 0; i < word.text.length(); ++i) {
        if (checkRomaji(word.text[i])){
            auto c = Label::createWithTTF(StringUtils::toString(word.text[i]), font_name, font_sz);
            c->setTextColor(Color4B::BLACK);
            c->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
            offset = c->getContentSize().width;
            pos_x += offset / 2;
            c->setPosition(Vec2(pos_x, -10));
            pos_x += offset / 2;
            c->setTag(i);
            fullText->addChild(c);
        }else{
            for (int j = i+1; j < word.text.length(); j++ ){
                if (checkRomaji(word.text[j])){
                    auto str = word.text.substr(i,j-i);
                    i = j;
                    auto c = Label::createWithTTF(StringUtils::toString(str), font_name, font_sz);
                    c->setTextColor(Color4B::BLACK);
                    c->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
                    offset = c->getContentSize().width;
                    pos_x += offset / 2;
                    c->setPosition(Vec2(pos_x, -10));
                    pos_x += offset / 2;
                    c->setTag(i);
                    fullText->addChild(c);
                    break;
                }else{
                    if (j == word.text.length() - 1){
                        auto str = word.text.substr(i,j-i);
                        CCLOG("str %s",str.c_str());
                        i = j;
                        auto c = Label::createWithTTF(StringUtils::toString(str), font_name, font_sz);
                        c->setTextColor(Color4B::BLACK);
                        c->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
                        offset = c->getContentSize().width;
                        pos_x += offset / 2;
                        c->setPosition(Vec2(pos_x, -10));
                        pos_x += offset / 2;
                        c->setTag(i);
                        fullText->addChild(c);
                    }
                }
            }
        }
        
    }
   
    for(int i = 0; i < word.text.size(); i++ ){
        for(int j = 0; j < word.position.size(); j++){
            if(i == word.position[j]){
                auto c = fullText->getChildByTag(i);
                c->setScale(0);
                Layout* underline = Layout::create();
                underline->setBackGroundColor(Color3B::BLACK);
                underline->setBackGroundColorType(LAYOUT_COLOR_SOLID);
                underline->setAnchorPoint(Point::ANCHOR_MIDDLE);
                underline->setContentSize(Size(c->getContentSize().width, 10));
                fullText->addChild(underline);
                underline->setPosition(Vec2(c->getPositionX(), 15));
                underline->setName("underline");
            }
        }
    }
    
    AUDIO_ENGINE->play("sounds/balloontheclown/text_show.mp3");
    fullText->runAction(Sequence::create(/*DelayTime::create(1),*/ FadeIn::create(1.5), NULL));

    float MaxSizeWidthOfText = 120;
    std::vector<ui::Text*> listAnswersText;
    for (int i = 0; i < word.answers.size(); i++) {
        Text* textAnswer = Text::create(word.answers[i].text, font_name, 68);
        textAnswer->setTextColor(Color4B::WHITE);
        textAnswer->enableOutline(Color4B::BLACK);
        textAnswer->setTextVerticalAlignment(TextVAlignment::CENTER);

        if (textAnswer->getContentSize().width > MaxSizeWidthOfText) {
            MaxSizeWidthOfText = textAnswer->getContentSize().width;
        }

        listAnswersText.push_back(textAnswer);
    }

    for(int i = 0 ; i < word.answers.size(); i++){
        ImageView* answer = ImageView::create();
        answer->loadTexture("games/balloontheclown/2.png");
        root_layout->addChild(answer);
        answer->setPosition(Vec2(-screen_size.width*0.2, screen_size.height*0.72));
       
        answer->ignoreContentAdaptWithSize(false);
        answer->setContentSize(Size(MaxSizeWidthOfText + 40, answer->getContentSize().height));
        answer->addChild(listAnswersText[i],100);

        listAnswersText[i]->setPosition(Vec2(answer->getContentSize()/2)+ Vec2(0,6));
        listAnswersText[i]->setName("text");
        
        answer->setCascadeOpacityEnabled(true);
        
        answer->setTag(0);
        if(i == 0){
            answer->setTag(1);
        }
		answer->setUserData(&word.answers[i].audio_path);
        answer->addClickEventListener(CC_CALLBACK_1(BalloonTheClown::onClickedAnswer, this));
        answers.push_back(answer);
    }
    
    mj::helper::shuffle(answers);
    for(int i = 0; i < answers.size(); i++){
        Vec2 anserSize = answers[i]->getContentSize();
        float startPlace = screen_size.width * 0.05;
        answers[i]->runAction(Sequence::create(EaseCircleActionIn::create(
            MoveTo::create(
                (5-i)*0.25, 
                Vec2(startPlace + (anserSize.x + 20) * i + anserSize.x / 2 , screen_size.height*0.72 ))),
            CallFunc::create([=]{
                answers[i]->setTouchEnabled(true);
        }) , NULL));
    }
}

void BalloonTheClown::showIntroGame(){
    AUDIO_ENGINE->play("sounds/balloontheclown/smile.mp3");
    this->setScale(1.8);
    cat->playSequence("look", false, [=](gaf::GAFObject* obj, string path){
        this->runAction(Spawn::create(ScaleTo::create(0.5f, 1), CallFunc::create([=]{

            goal->runAction(EaseOut::create(MoveTo::create(.6f, Vec2(screen_size.width/2, screen_size.height*0.4)), 1));
        }), NULL));
       AUDIO_ENGINE->play("sounds/balloontheclown/turn.mp3");
        cat->playSequence("turn", false, [=](gaf::GAFObject* obj, string path){
            cat->playSequence("idle", true);
            
        });
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5), CallFunc::create([=]{
            onStartGame();
    })));
}

void BalloonTheClown::onEnter(){
    MJMainGBase::onEnter();
    bg_id = playBackgroundMusic("sounds/balloontheclown/music.mp3", 0.5f);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void BalloonTheClown::onExit()
{

    if (_seq) {

        delete _seq;
        _seq = nullptr;
    }


    AUDIO_ENGINE->stopAllAudio();
    this->unschedule(_learningReminderScheduleName);
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->stopAllActions();

    MJMainGBase::onExit();
}

void BalloonTheClown::onClickedAnswer(cocos2d::Ref *sender){
    this->hideTapGuideGame();
    AUDIO_ENGINE->stopEffect(w_id);
    AUDIO_ENGINE->stopEffect(p_id);

    stopSequenceAudio(_seq);
    this->unschedule(_learningReminderScheduleName);
    _touchAbleSpeaker = false;

    auto answer = (ImageView*)sender;
    bool correct = answer->getTag();
    
    for(auto a: answers){
        a->setEnabled(false);
    }
    
    
	std::string * audio_p = (std::string *)answer->getUserData();
	std::string audio = *(audio_p);
	
	int ans_id = AUDIO_ENGINE->play(audio);
    throwBall(correct);
    
    if(correct){
        //right
        AudioEngine::stop(q_id);
        AudioEngine::setFinishCallback(ans_id, [=](int id, string path){
             AUDIO_ENGINE->playSound("sounds/balloontheclown/correct.mp3", false, [=] {}, 0.7f);
        });

        GAFWrapper* star = GAFWrapper::createGAF("gaf/balloontheclown/Star/cloudcatcher_effectunder.gaf");
        answer->addChild(star);
        star->setAnchorPoint(Point(0.4, 0.45));
        star->setPosition(answer->getContentSize()/2);
        star->playSequence("hit", false, [=](gaf::GAFObject* obj, string path){
            star->removeFromParent();
            answer->runAction(EaseBounceIn::create(ScaleTo::create(0.2f, 0)));
        });
    
        for(auto a: answers) {
            a->runAction(Sequence::create(
                DelayTime::create(0.4), 
                FadeOut::create(0.5), 
                CallFunc::create([=]{ 
                    a->removeFromParent();
            }),NULL));
        }
        
        for(auto c: fullText->getChildren()){
            if(c->getName() == "underline")
                c->setVisible(false);
        }
        
        for(int i = 0; i < word.text.length(); i++){
            fullText->getChildByTag(i)->runAction(EaseBounceOut::create(ScaleTo::create(0.2, 1)));
        }
        
        fullText->runAction(Sequence::create(
            DelayTime::create(2.5), 
            EaseBackIn::create(ScaleTo::create(0.3, 0)),
            CallFunc::create([=]{}),
            NULL));
        
    }else{
        //wrong
        AudioEngine::setFinishCallback(ans_id, [=](int id, string path){
            AUDIO_ENGINE->playSound("sounds/balloontheclown/wrong.mp3", false, [=] {
                this->runAction(Sequence::create(
                    DelayTime::create(0.5),
                    CallFunc::create([=] {
                        _touchAbleSpeaker = true;
                        })
                    , NULL));
                }, 0.7f);
        });
        
        ImageView* red = ImageView::create("games/balloontheclown/3.png");
        red->setOpacity(180);
        answer->addChild(red, 10);
        red->setPosition(answer->getContentSize()/2);
        red->ignoreContentAdaptWithSize(false);
        red->setContentSize(answer->getContentSize());
        
        answer->runAction(Sequence::create(
            EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), 
            EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), 
            EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), 
            CallFunc::create([=]{
            red->removeFromParent();
        }) , NULL));
       
        learningReminder();

    }
    
}

void BalloonTheClown::onTransition(){
    AUDIO_ENGINE->stopEffect(w_id);
    AUDIO_ENGINE->stopEffect(p_id);
    
    clown->removeFromParent();
    
    Layout* transition = Layout::create();
    transition->setContentSize(screen_size);
    transition->setBackGroundColor(Color3B::BLACK);
    transition->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    transition->setOpacity(0);
    
    this->addChild(transition, 999999);
    transition->setPosition(Vec2(0,0));
    transition->runAction(FadeIn::create(0.5));
    
    Text* text = Text::create(word.text, font_name, font_sz*2);
    transition->addChild(text);
    text->setPosition(Vec2(screen_size.width*0.5, screen_size.height*0.5));
    text->setTextColor(Color4B::WHITE);
    text->setScale(0);
    text->runAction(EaseBounceOut::create(Spawn::create(ScaleTo::create(0.4f, 1), JumpTo::create(0.2, screen_size/2, 100, 1), NULL)));
    
    this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=]{
        AudioEngine::setVolume(bg_id, 0.6);
        playPhonicText(text);
    }), DelayTime::create(word.phonics.size()+0.5), CallFunc::create([=]{
        playSlowText(text);
    }), DelayTime::create(2.5), CallFunc::create([=]{
        playNormalText(text);
    }), DelayTime::create(1.5), CallFunc::create([=]{
        text->removeFromParent();
        transition->runAction(Sequence::createWithTwoActions(FadeOut::create(0.5), CallFunc::create([=]{
        transition->removeFromParent();
        })));
        AudioEngine::setVolume(bg_id, 1);
        onNextGame();
        
    }), NULL));
}

void BalloonTheClown::onNextStage(){
    this->runAction(Sequence::create(CallFunc::create([=]{
        GAFWrapper* clown_end = GAFWrapper::createGAF("gaf/balloontheclown/clown_end/clown_end.gaf");
        clown_end->setScale(0.8);
        root_layout->addChild(clown_end);
        clown_end->setPosition(Vec2(clown->getPositionX()+20, clown->getPositionY()+ 100));
        clown_end->playSequence("fall", false);
        AUDIO_ENGINE->play("sounds/balloontheclown/clown_end.mp3");
        clown->playSequence("end", false);
        
        root_layout->setAnchorPoint(Point(0.7, 0.55));
        root_layout->runAction(Sequence::create(ScaleTo::create(0.8, 1.2), CallFunc::create([=]{
            
        }), CallFunc::create([=]{
            clown_end->pauseAnimation();
            clown_end->playSequence("fall2", true, [=](gaf::GAFObject* obj, string path){
                Vec2 pos = root_layout->getAnchorPoint();
                root_layout->runAction(Repeat::create(Sequence::create(MoveTo::create(0.05f, Vec2(pos.x-8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x+8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x-5, pos.y-4)), MoveTo::create(0.05f, Vec2(pos.x +5, pos.y+4)), MoveTo::create(0.05f, Vec2(pos.x, pos.y)),NULL), 2));
                
                GAFWrapper* water_splash = GAFWrapper::createGAF("gaf/balloontheclown/splash_big/splash_big.gaf");
                water_splash->setPosition(Vec2(screen_size.width*0.75, screen_size.height*0.3));
                water_splash->setScale(1.5);
                root_layout->addChild(water_splash, 999);
                water_splash->play(false,[=](gaf::GAFObject* obj){
                    water_splash->removeFromParent();
                });
                
                GAFWrapper* phaohoa = GAFWrapper::createGAF("gaf/balloontheclown/phaohoa/phaohoa.gaf");
                root_layout->addChild(phaohoa, 999);
                phaohoa->setPosition(Vec2(screen_size.width*0.2, screen_size.height*0.4));
                phaohoa->play(false, [=](gaf::GAFObject* obj){
                    phaohoa->removeFromParent();
                });
                
                GAFWrapper* phaohoa_1 = GAFWrapper::createGAF("gaf/balloontheclown/phaohoa/phaohoa.gaf");
                root_layout->addChild(phaohoa_1, 999);
                phaohoa_1->setFlippedX(true);
                phaohoa_1->setPosition(Vec2(screen_size.width*0.8, screen_size.height*0.4));
                phaohoa_1->play(false, [=](gaf::GAFObject* obj){
                    phaohoa_1->removeFromParent();
                });
            });
            clown_end->runAction(Sequence::create( EaseBackIn::create(MoveTo::create(0.3, Vec2(clown_end->getPositionX(), screen_size.height*0.3))), CallFunc::create([=]{
                clown_end->setVisible(false);
            }), NULL));
        }), ScaleTo::create(0.3, 1), DelayTime::create(1), CallFunc::create([=]{
            clown_end->removeFromParent();
            onTransition();
            AUDIO_ENGINE->stopAllAudio();
            this->unscheduleAllCallbacks();
            this->unscheduleAllSelectors();
            _learingReminderCountDown = 0;
            this->unschedule(_learningReminderScheduleName);
        }), NULL));
    }), NULL));
}

void BalloonTheClown::onNextGame(){
    AUDIO_ENGINE->stopAllAudio();
    count_game++;
    if(count_game < max_game){
            onStartGame();
    }else{
        root_layout->setAnchorPoint(Point(0,0));
        root_layout->runAction(Sequence::createWithTwoActions(ScaleTo::create(1, 1.8), CallFunc::create([=]{
            cat->playSequence("fade_out", false, [=](gaf::GAFObject* obj, string path){
                this->onFinishGame();
            });
            AUDIO_ENGINE->play("sounds/balloontheclown/smile.mp3");
           
        })));
    }

}

void BalloonTheClown::onFinishGame(){
    MJMainGBase::onFinishGame();
}

void BalloonTheClown::clownEffect(bool correct){
    if(correct){
        onNextStage();
    }else{
        std::string clown_laugh = (random(0, 1))? "sounds/balloontheclown/clown_laugh1.mp3" : "sounds/balloontheclown/clown_laugh2.mp3" ;
        AUDIO_ENGINE->play(clown_laugh);
        clown->playSequence("win" + cocos2d::StringUtils::toString(0), false, [=](gaf::GAFObject* obj, string path){
            clown->playSequence("idle" + cocos2d::StringUtils::toString(0));
            for(auto a: answers){
                a->setEnabled(true);
            }
        });
    }
}

void BalloonTheClown::throwBall(bool correct, bool clown_effect){
    
    audio_helper->stop(w_id);
    audio_helper->stop(p_id);
    
    int throw_id = audio_helper->play("sounds/balloontheclown/throw.mp3");
    AudioEngine::setFinishCallback(throw_id, [=](int id, string path){
        audio_helper->play("sounds/balloontheclown/balloon_water.mp3");
        if(clown_effect){
             clownEffect(correct);
        }
        
    });
    cat->playSequence("throw", false, [=](gaf::GAFObject* obj, string path){
        cat->playSequence("idle", true);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.45f), CallFunc::create([=] {
        float scale_time = 0.1,
        scale_in_root_time = 0.3,
        scale_out_root_time = 0.3,
        move_up_time = 0.2,
        move_down_time = 0.2,
        pos_x_bf = 150,
        pos_y_bf = 220,
        pos_x_af = goal->getPositionX() - screen_size.width*0.3 - pos_x_bf,
        pos_y_af = -200;
        
        if(!correct){
            scale_time = 0.2f;
            scale_in_root_time = 0.5;
            scale_out_root_time = 0.8;
            move_down_time = 0.35;
            if(random(0, 1)){
                pos_x_bf = 200;
                pos_x_af = 220;
                pos_y_af = -300;
            }else{
                pos_x_bf = 100;
                pos_x_af = 100;
                pos_y_af = -300;
            }
        }
        
        auto action = Sequence::create(Spawn::create(
            ScaleTo::create(scale_time, 1.2), 
            MoveBy::create(move_up_time, Vec2(0, pos_y_bf)), 
            EaseIn::create(MoveBy::create(move_up_time, Vec2(pos_x_bf, 0)), 1), NULL), 
            Spawn::create(
                MoveBy::create(move_down_time, Vec2(pos_x_af, 0)), 
                EaseIn::create(MoveBy::create(move_down_time, Vec2(0, pos_y_af)), 2), 
                ScaleTo::create(move_down_time, 0.2), NULL), 
            NULL);
        
        auto root_action = Sequence::create(
            ScaleTo::create(scale_in_root_time, 1.2), 
            ScaleTo::create(scale_out_root_time, 1), 
            NULL);
        
        root_layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
        root_layout->setIgnoreAnchorPointForPosition(true);
        root_layout->runAction(root_action);
        
        ImageView* ball = ImageView::create("games/balloontheclown/1.png");
        ball->setScaleX(0.9);
        root_layout->addChild(ball, 1);
        ball->setPosition(Vec2(screen_size.width*0.3, screen_size.height*0.4));
        ball->runAction(Sequence::create(action, CallFunc::create([=]{
            GAFWrapper* splash = GAFWrapper::createGAF("gaf/balloontheclown/splash/splash.gaf");
            int zorder = 0;
            if(correct){
                splash = GAFWrapper::createGAF("gaf/balloontheclown/hit/hit.gaf");
                goal->playSequence("hit", false);
                auto pos = root_layout->getAnchorPoint();
                root_layout->runAction(Repeat::create(Sequence::create(MoveTo::create(0.05f, Vec2(pos.x-8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x+8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x-5, pos.y-4)), MoveTo::create(0.05f, Vec2(pos.x +5, pos.y+4)), MoveTo::create(0.05f, Vec2(pos.x, pos.y)),NULL), 2));
                AUDIO_ENGINE->play("sounds/balloontheclown/hit_target.mp3");
                zorder = 2;
            }
            root_layout->addChild(splash, zorder);
            splash->setScale(2.5);
            splash->setPosition(Vec2(ball->getPositionX(), ball->getPositionY() - 10));
            ball->removeFromParent();
            splash->play(false, [=](gaf::GAFObject *obj){
                splash->removeFromParent();
            });
        })  ,NULL));
    }), NULL));
}

void BalloonTheClown::learningReminder()
{
    _learingReminderCountDown = 0;

    if (_seq) {

        delete _seq;

        _seq = nullptr;
    }

    _seq = new math::SequenceAudio();

    this->schedule([=](float) {
        if (_learingReminderCountDown <= 10)
        {
            _learingReminderCountDown++;
            if (_learingReminderCountDown == 10)
            {
                w_id = audio_helper->play(word.audio);
                AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
                    _testSequence = Sequence::create(
                        DelayTime::create(1.0),
                        CallFunc::create([=] {
                            audio_helper->play_sequence_with_sequanceAudio(word.p_audio, [=](string path) {
                                }, "play phonic", _seq);
                            _learingReminderCountDown = 0;
                            }),
                        NULL);
                    this->runAction(_testSequence);
                    });
            }
        }
        }, 1, _learningReminderScheduleName);
}
void BalloonTheClown::stopSequenceAudio(math::SequenceAudio* param_seq)
{
    this->stopAction(_testSequence);
    this->stopAction(_testSequenceForAudioButton);
    AUDIO_ENGINE->stopAllAudio();
    audio_helper->stop(w_id);
    if (param_seq)
    {
        if (param_seq->current != -1) {
            audio_helper->stop(param_seq->current);
            audio_helper->stop(param_seq->index);
            audio_helper->stop_sequence(param_seq);
            param_seq->current = -1;
            param_seq->index = param_seq->audios.size() + 1;
        }
    }
}

void BalloonTheClown::playPhonicText(Text* text){
	int start_index = 0;
	std::queue<int> underline_index;
	
	for (int i = 0; i < word.phonics.size(); ++i) {
		std::vector<int> indexes;
		auto phonic_data = word.phonics[i];
		for (int j = 0; j < phonic_data.size(); ++j) {
			if (phonic_data[j] == '_') {
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					indexes.push_back(start_index + j);
				}
				else {
					indexes.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_data.size();
		list_index.push_back(indexes);
	}
    for(int i = 0 ; i < list_index.size(); i++){
        this->runAction(Sequence::create(DelayTime::create(1.0f *(i)), CallFunc::create([=]{
            audio_helper->play(word.p_audio[i]);
            for(int j = 0; j < list_index[i].size(); j++){
                int index = list_index[i][j];
                text->getLetter(index)->runAction(Sequence::create( CallFunc::create([=]{
                    text->getLetter(index)->setColor(Color3B(250, 54, 208));
                }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=]{
                    text->getLetter(index)->setColor(Color3B(255, 255, 255));
                }), NULL));
            }
        }), NULL));
    }
}

void BalloonTheClown::playSlowText(Text* text){
    AUDIO_ENGINE->play(word.s_audio);
    text->runAction(EaseBounceOut::create(Sequence::create(CallFunc::create([=]{
        text->setTextColor(Color4B(250, 54, 208, 255));
    }), ScaleTo::create(1, 1.2), ScaleTo::create(1, 1), CallFunc::create([=]{
        text->setTextColor(Color4B::WHITE);
    }), NULL)));
}

void BalloonTheClown::playNormalText(Text* text){
    AUDIO_ENGINE->play(word.audio);
    text->runAction(EaseBounceOut::create(Sequence::create(CallFunc::create([=]{
        text->setTextColor(Color4B(250, 54, 208, 255));
    }), ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1), CallFunc::create([=]{
        text->setTextColor(Color4B::WHITE);
    }), NULL)));
}
bool BalloonTheClown::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    return false;
}
