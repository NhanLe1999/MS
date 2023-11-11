
//
//  BalloonTheClown_VM.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/24/18.
//

#include "BalloonTheClown_VM.h"
#include "AudioHelper.h"
USING_NS_CC;
INITIALIZE_READER(BalloonTheClown_VM);

std::once_flag BalloonTheClown_VM_reader;
#define CSB_NAME "csb/BalloonTheClown_VM/BalloonTheClown_VM.csb"
#define _game_path "json/BalloonTheClown_VM/"

BalloonTheClown_VM* BalloonTheClown_VM::createGame(std::string json_file, bool isAutoPlay){
    std::call_once(BalloonTheClown_VM_reader, []{
        REGISTER_CSB_READER(BalloonTheClown_VM);
    });
    
    auto p = static_cast<BalloonTheClown_VM*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void BalloonTheClown_VM::didLoadFromCSB(std::string json_file, bool isAutoPlay){
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = "fonts/danhvan.ttf";
    }
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && document.HasMember("audio_intro_vi")){
       auto audio_intro = document["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
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
        auto text_utf8 = StringUtils::StringUTF8(game.text);
        auto text_right_utf8 = StringUtils::StringUTF8(game.right_sub_text);
        for (auto i = 0; i < text_utf8.length(); i++){
            auto str = text_utf8.getAsCharSequence(i,1);
            if (str == text_right_utf8.getAsCharSequence(0,1)){
                auto kt = true;
                for (int j = 0; j < text_right_utf8.length(); j++){
                    if (text_right_utf8.getAsCharSequence(j,1) != text_utf8.getAsCharSequence(i+j,1)){
                        kt = false;
                        break;
                    }
                }
                if (kt){
                    for (int j = 0; j < text_right_utf8.length(); j++){
                        game.position.push_back(i+j);
                    }
                }
            }
        }
//		int found = game.text.find_first_of(game.right_sub_text.at(0));
//		while(found != std::string::npos) {
//			bool break_while = true;
//			for (int i = 0; i < game.right_answer.text.size(); ++i) {
//				if (game.right_answer.text[i] == '_' ) {
//					//underline_index
//					continue;
//				}
//				else if ( (found + i) < game.text.size() && game.text[found + i] == game.right_answer.text[i]) {
//					game.position.push_back(found + i);
//				}
//				else {
//					break_while = false;
//					game.position.clear();
//					break;
//				}
//			}
//			if (break_while) break;
//			found = game.text.find_first_of(game.right_sub_text.at(0), found + 1);
//		};

        words.push_back(game);
    }
    
	count_game = 0;
    max_game = words.size();
    
    screen_size = Director::getInstance()->getVisibleSize();
    _isAutoPlay = isAutoPlay;
    loadFrame();
}

void BalloonTheClown_VM::loadFrame(){
	this->setContentSize(screen_size);
    root_layout = utils::findChild<Layout*>(this, "root_panel");
    _bg = utils::findChild<ImageView*>(root_layout, "bg_panel");
    
    if(_bg->getContentSize().width < screen_size.width)
    {
        _bg->setScaleX(screen_size.width / _bg->getContentSize().width);
    }
    
    goal = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/target/target.gaf");
    root_layout->addChild(goal,1);
    goal->setPosition(Vec2(screen_size.width*0.65, screen_size.height*0.4));
    goal->playSequence("idle", true);
    goal->setScale(0.8);
    
    cat = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/cat/cat.gaf");
    root_layout->addChild(cat, 10);
    cat->setPosition(Vec2(screen_size.width*0.4, -screen_size.height* 0.15));
    cat->setScale(0.8);
    
//    gameConfig();
    showIntroGame();
}

//game config
void BalloonTheClown_VM::gameConfig(){
}

void BalloonTheClown_VM::onStartGame() {

	//reset game
	mj::helper::shuffle(words);
	word = words.front();
	words.erase(words.begin());
	answers.clear();
	list_index.clear();
    
    hideTapGuideGame();

	clown = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/clown/clown.gaf");
	clown->setAnchorPoint(Point(0.35, 0.45));
	root_layout->addChild(clown);
	clown->setPosition(Vec2(screen_size.width*0.75, screen_size.height*0.45));// (screen_size.width, screen_size.height*0.45));
	clown->playSequence("idle0", true);
	clown->setScale(0.8);
	//    clown->runAction(EaseOut::create(MoveTo::create(.3f, Vec2(screen_size.width*0.75, screen_size.height*0.45)), 1));

		//create speaker
	ImageView* speaker = ImageView::create("games/BalloonTheClown_VM/loa.png");
	root_layout->addChild(speaker);
	speaker->setPosition(Vec2(screen_size.width * 0.08, screen_size.height* 0.88));
	speaker->setTouchEnabled(false);
	speaker->addClickEventListener(CC_CALLBACK_1(BalloonTheClown_VM::onClickedSpeaker, this));


	if (count_game == 0) {
		q_id = audio_helper->play(question_vi);
        AudioEngine::setFinishCallback(q_id, [=](int id, string path) {
			w_id = audio_helper->play(word.audio);
			AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
				//AUDIO_ENGINE->play_sequence(word.p_audio, [=](string path) {
				//	speaker->setTouchEnabled(true);
				//}, "play_phonic");
				speaker->setTouchEnabled(true);
                showGuideGame();
			});
		});
	}
	else {

			w_id = audio_helper->play(word.audio);
			AudioEngine::setFinishCallback(w_id, [=](int id, string path) {
				speaker->setTouchEnabled(true);
//                showGuideGame();
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
    word.texts.clear();
    int index = 0;
    auto text_utf8 = StringUtils::StringUTF8(word.text);
    for (auto i = 0; i < text_utf8.length(); i++){
        auto str = text_utf8.getAsCharSequence(i,1);
        word.texts.push_back(str);
        auto c = Label::createWithTTF(str, font_name, font_sz);
        c->setTextColor(Color4B::BLACK);
        c->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        offset = c->getContentSize().width;
        pos_x += offset / 2;
        c->setPosition(Vec2(pos_x, -10));
        pos_x += offset / 2;
        c->setTag(i);
        fullText->addChild(c);
    }
   
    for(int i = 0; i < word.texts.size(); i++ ){
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
    
    AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/text_show.mp3");
    fullText->runAction(Sequence::create(/*DelayTime::create(1),*/ FadeIn::create(1.5), NULL));

    //create answer
    for(int i = 0 ; i < word.answers.size(); i++){
        ImageView* answer = ImageView::create();
        answer->loadTexture("games/BalloonTheClown_VM/2.png");
        root_layout->addChild(answer);
        answer->setPosition(Vec2(-screen_size.width*0.2, screen_size.height*0.72));
       
        Text* text = Text::create(word.answers[i].text, font_name, 68);
        text->setTextColor(Color4B::WHITE);
        text->enableOutline(Color4B::BLACK);
		text->setTextVerticalAlignment(TextVAlignment::CENTER);
        answer->addChild(text,100);
        text->setPosition(Vec2(answer->getContentSize()/2)+ Vec2(0,0));
        text->setName("text");
        
        answer->setCascadeOpacityEnabled(true);
        
        // 0 = false, 1 = true;
        answer->setTag(0);
        if(i == 0 /*word.right_index*/){
            answer->setTag(1);
        }
		answer->setUserData(&word.answers[i].audio_path);
        answer->addClickEventListener(CC_CALLBACK_1(BalloonTheClown_VM::onClickedAnswer, this));
        answers.push_back(answer);
    }
    
    mj::helper::shuffle(answers);
    for(int i = 0; i < answers.size(); i++){
        auto a = answers[i];
        a->runAction(Sequence::create(EaseCircleActionIn::create(MoveTo::create((5-i)*0.25, Vec2(screen_size.width*0.12 + 220*i, screen_size.height*0.72 ))), CallFunc::create([=]{
            a->setTouchEnabled(true);
            //showGuideGame();
        }) , NULL));
    }
    
}

void BalloonTheClown_VM::showIntroGame(){
    //zoom cat
    AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/smile.mp3");
    this->setScale(1.8);
    cat->playSequence("look", false, [=](gaf::GAFObject* obj, string path){
        this->runAction(Spawn::create(ScaleTo::create(0.5f, 1), CallFunc::create([=]{

            goal->runAction(EaseOut::create(MoveTo::create(.6f, Vec2(screen_size.width/2, screen_size.height*0.4)), 1));
        }), NULL));
       AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/turn.mp3");
        cat->playSequence("turn", false, [=](gaf::GAFObject* obj, string path){
            cat->playSequence("idle", true);
            
        });
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5), CallFunc::create([=]{
            onStartGame();
    })));
}

void BalloonTheClown_VM::onEnter(){
    MJMainGBase::onEnter();
    bg_id = playBackgroundMusic("sounds/BalloonTheClown_VM/music.mp3", 0.5f);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void BalloonTheClown_VM::onClickedAnswer(cocos2d::Ref *sender){
    this->unschedule("show_guide");
    this->hideTapGuideGame();
    AUDIO_ENGINE->stopEffect(w_id);
    AUDIO_ENGINE->stopEffect(p_id);
    
    auto answer = (ImageView*)sender;
    bool correct = answer->getTag();
    
    for(auto a: answers){
        a->setEnabled(false);
    }
    
    //audio
    //auto index = 0;
    //for(auto a: answers){
    //    if(utils::findChild<Text*>(answer, "text")->getString() == utils::findChild<Text*>(a, "text")->getString()){
    //        break;
    //    }
    //    index++;
    //}
	
	std::string * audio_p = (std::string *)answer->getUserData();
	std::string audio = *(audio_p);
	
	int ans_id = AUDIO_ENGINE->play(audio);
    throwBall(correct);
    
    if(correct){
        //right
        AudioEngine::stop(q_id);
        AudioEngine::setFinishCallback(ans_id, [=](int id, string path){
             AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/correct.mp3", false, 0.7);
        });
        
        GAFWrapper* star = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/Star/cloudcatcher_effectunder.gaf");
        answer->addChild(star);
        star->setAnchorPoint(Point(0.4, 0.45));
        star->setPosition(answer->getContentSize()/2);
        star->playSequence("hit", false, [=](gaf::GAFObject* obj, string path){
            star->removeFromParent();
            answer->runAction(EaseBounceIn::create(ScaleTo::create(0.2f, 0)));
        });
    
        for(auto a: answers) {
            a->runAction(Sequence::create(DelayTime::create(0.4), FadeOut::create(0.5), CallFunc::create([=]{
                a->removeFromParent();
            }),NULL));
        }
        
        for(auto c: fullText->getChildren()){
            if(c->getName() == "underline")
                c->setVisible(false);
        }
        
        for(int i = 0; i < word.texts.size(); i++){
            fullText->getChildByTag(i)->runAction(EaseBounceOut::create(ScaleTo::create(0.2, 1)));
        }
        
        fullText->runAction(Sequence::create(DelayTime::create(2.5), EaseBackIn::create(ScaleTo::create(0.3, 0)),CallFunc::create([=]{
//            onNextStage();
//            onTransition();
        }),NULL));
        
    }else{
        //wrong
        AudioEngine::setFinishCallback(ans_id, [=](int id, string path){
          AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/wrong.mp3", false, 0.7);
        });
        
        ImageView* red = ImageView::create("games/BalloonTheClown_VM/3.png");
        red->setOpacity(180);
        answer->addChild(red, 10);
        red->setPosition(answer->getContentSize()/2);
        
        answer->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), CallFunc::create([=]{
            red->removeFromParent();
            this->showGuideGame();
        }) , NULL));
    }


}

void BalloonTheClown_VM::onTransition(){
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
    
    Text* text = Text::create(word.co_dau?word.text_khong_dau:word.text, font_name, font_sz*2);
    transition->addChild(text);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(Vec2(screen_size.width*0.5, screen_size.height*0.5));
    text->setTextColor(Color4B::WHITE);
    
    if (word.co_dau){
        Text* text_dau = Text::create(word.text_dau, font_name, font_sz*2);
        text->addChild(text_dau);
        text_dau->setName("dau");
        text_dau->setAnchorPoint(Vec2(0.5,0.5));
        auto lb = text->getLetter(word.pos_dau);
        auto pos_dau = Vec2(lb->getPosition().x,lb->getPosition().y+lb->getContentSize().height/2);
        if (word.text_dau == "."){
            pos_dau = Vec2(lb->getPosition().x,lb->getPosition().y-lb->getContentSize().height/2);
        }
        if (word.text_dau == "/" || word.text_dau == "´"){
            auto text = StringUtils::StringUTF8(word.text_dau).getAsCharSequence(word.pos_dau);
            pos_dau = Vec2(lb->getPosition().x+10,lb->getPosition().y+lb->getContentSize().height/2-27.5);
        }
        if (word.text_dau == "~"){
            auto utf8 = StringUtils::StringUTF8(word.text_khong_dau);
            auto ee = utf8.getAsCharSequence(word.pos_dau,1);
            pos_dau = Vec2(lb->getPosition().x,lb->getPosition().y+lb->getContentSize().height/2-11);
            if (ee == "ư"){
               pos_dau = Vec2(lb->getPosition().x-10,lb->getPosition().y+lb->getContentSize().height/2-11);
            }
        }
        text_dau->setPosition(pos_dau);
        text_dau->setTextColor(Color4B::WHITE);
        text->setScale(0);
        text->runAction(EaseBounceOut::create(Spawn::create(ScaleTo::create(0.4f, 1), JumpTo::create(0.2, screen_size/2, 100, 1), NULL)));
        this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=]{
            AudioEngine::setVolume(bg_id, 0.6);
            playPhonicText(text);
        }), DelayTime::create(2.5), CallFunc::create([=]{
            playTextDau(text);
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
    }else{
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
    
}

void BalloonTheClown_VM::onNextStage(){
    
//    throwBall(true, false);
    
    this->runAction(Sequence::create(/*DelayTime::create(1.3),*/ CallFunc::create([=]{
        GAFWrapper* clown_end = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/clown_end/clown_end.gaf");
        clown_end->setScale(0.8);
        root_layout->addChild(clown_end);
        clown_end->setPosition(Vec2(clown->getPositionX()+20, clown->getPositionY()+ 100));
        clown_end->playSequence("fall", false);
        AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/clown_end.mp3");
        clown->playSequence("end", false);
        
        root_layout->setAnchorPoint(Point(0.7, 0.55));
        root_layout->runAction(Sequence::create(ScaleTo::create(0.8, 1.2), CallFunc::create([=]{
            
        }), CallFunc::create([=]{
            clown_end->pauseAnimation();
            clown_end->playSequence("fall2", true, [=](gaf::GAFObject* obj, string path){
                Vec2 pos = root_layout->getAnchorPoint();
                root_layout->runAction(Repeat::create(Sequence::create(MoveTo::create(0.05f, Vec2(pos.x-8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x+8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x-5, pos.y-4)), MoveTo::create(0.05f, Vec2(pos.x +5, pos.y+4)), MoveTo::create(0.05f, Vec2(pos.x, pos.y)),NULL), 2));
                
                GAFWrapper* water_splash = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/splash_big/splash_big.gaf");
                water_splash->setPosition(Vec2(screen_size.width*0.75, screen_size.height*0.3));
                water_splash->setScale(1.5);
                root_layout->addChild(water_splash, 999);
                water_splash->play(false,[=](gaf::GAFObject* obj){
                    water_splash->removeFromParent();
                });
                
                GAFWrapper* phaohoa = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/phaohoa/phaohoa.gaf");
                root_layout->addChild(phaohoa, 999);
                phaohoa->setPosition(Vec2(screen_size.width*0.2, screen_size.height*0.4));
                phaohoa->play(false, [=](gaf::GAFObject* obj){
                    phaohoa->removeFromParent();
                });
                
                GAFWrapper* phaohoa_1 = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/phaohoa/phaohoa.gaf");
                root_layout->addChild(phaohoa_1, 999);
                phaohoa_1->setFlippedX(true);
                phaohoa_1->setPosition(Vec2(screen_size.width*0.8, screen_size.height*0.4));
                phaohoa_1->play(false, [=](gaf::GAFObject* obj){
                    phaohoa_1->removeFromParent();
                });
            });
            clown_end->runAction(Sequence::create( EaseBackIn::create(MoveTo::create(0.3, Vec2(clown_end->getPositionX(), screen_size.height*0.3))), CallFunc::create([=]{
                clown_end->setVisible(false);
//                clown_end->removeFromParent();
            }), NULL));
        }), ScaleTo::create(0.3, 1), DelayTime::create(1), CallFunc::create([=]{
//            onFinishGame();
            clown_end->removeFromParent();
            onTransition();
        
        }), NULL));
    }), NULL));
}

void BalloonTheClown_VM::onNextGame(){
    
    count_game++;
    if(count_game < max_game){
            onStartGame();
    }else{
//        onNextStage();
        //finish game
//        this->setScale(1.8);
        root_layout->setAnchorPoint(Point(0,0));
        root_layout->runAction(Sequence::createWithTwoActions(ScaleTo::create(1, 1.8), CallFunc::create([=]{
            cat->playSequence("fade_out", false, [=](gaf::GAFObject* obj, string path){
                this->onFinishGame();
            });
            AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/smile.mp3");
           
        })));
    }
}

void BalloonTheClown_VM::onFinishGame(){
//    MJMainGBase::onFinishGame(CHARACTER::NAT);
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGame();
}

void BalloonTheClown_VM::showGuideGame(std::string gaf_guide_path){
 
//    this->scheduleOnce([=](float){
        auto guide_pos = Vec2(0, 0);
        for (auto a : answers) {
            if(a && a->getTag()) {
                guide_pos = root_layout->convertToWorldSpace(a->getPosition());
                this->showTapGuideGame(guide_pos, 8);
                break;
            }
        }
//    }, 0, "show_guide");

}

/*
 game effect
*/
void BalloonTheClown_VM::clownEffect(bool correct){
    if(correct){
        onNextStage();
        
    }else{
        std::string clown_laugh = (random(0, 1))? "sounds/BalloonTheClown_VM/clown_laugh1.mp3" : "sounds/BalloonTheClown_VM/clown_laugh2.mp3" ;
        AUDIO_ENGINE->play(clown_laugh);
        clown->playSequence("win" + cocos2d::StringUtils::toString(0), false, [=](gaf::GAFObject* obj, string path){
            clown->playSequence("idle" + cocos2d::StringUtils::toString(0));
            for(auto a: answers){
                a->setEnabled(true);
            }
        });
    }
}

void BalloonTheClown_VM::throwBall(bool correct, bool clown_effect){
    
    audio_helper->stop(w_id);
    audio_helper->stop(p_id);
    
    int throw_id = audio_helper->play("sounds/BalloonTheClown_VM/throw.mp3");
    AudioEngine::setFinishCallback(throw_id, [=](int id, string path){
        audio_helper->play("sounds/BalloonTheClown_VM/balloon_water.mp3");
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
        
        auto action = Sequence::create(Spawn::create(ScaleTo::create(scale_time, 1.2), MoveBy::create(move_up_time, Vec2(0, pos_y_bf)), EaseIn::create(MoveBy::create(move_up_time, Vec2(pos_x_bf, 0)), 1), NULL), Spawn::create(MoveBy::create(move_down_time, Vec2(pos_x_af, 0)), EaseIn::create(MoveBy::create(move_down_time, Vec2(0, pos_y_af)), 2), ScaleTo::create(move_down_time, 0.2), NULL), NULL);
        
        auto root_action = Sequence::create(ScaleTo::create(scale_in_root_time, 1.2), ScaleTo::create(scale_out_root_time, 1), NULL);
        
        root_layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
        root_layout->setIgnoreAnchorPointForPosition(true);
        root_layout->runAction(root_action);
        
        ImageView* ball = ImageView::create("games/BalloonTheClown_VM/1.png");
        ball->setScaleX(0.9);
        root_layout->addChild(ball, 1);
        ball->setPosition(Vec2(screen_size.width*0.3, screen_size.height*0.4));
        ball->runAction(Sequence::create(action, CallFunc::create([=]{
            GAFWrapper* splash = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/splash/splash.gaf");
            int zorder = 0;
            if(correct){
                splash = GAFWrapper::createGAF("gaf/BalloonTheClown_VM/hit/hit.gaf");
                // splash->setScale(2.5);
                goal->playSequence("hit", false);
                auto pos = root_layout->getAnchorPoint();
                root_layout->runAction(Repeat::create(Sequence::create(MoveTo::create(0.05f, Vec2(pos.x-8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x+8, pos.y)), MoveTo::create(0.05f, Vec2(pos.x-5, pos.y-4)), MoveTo::create(0.05f, Vec2(pos.x +5, pos.y+4)), MoveTo::create(0.05f, Vec2(pos.x, pos.y)),NULL), 2));
                AUDIO_ENGINE->play("sounds/BalloonTheClown_VM/hit_target.mp3");
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

/*
 onClick callback
*/
void BalloonTheClown_VM::onClickedSpeaker(cocos2d::Ref *sender){
    ImageView* btn = (ImageView*) sender;
    btn->setTouchEnabled(false);
    
    w_id = audio_helper->play(word.audio);
    AudioEngine::setFinishCallback(w_id, [=](int id, string path){
        btn->setTouchEnabled(true);
    });
}

/*
 Play audio text
*/
void BalloonTheClown_VM::playPhonicText(Text* text){

	int start_index = 0;
	std::queue<int> underline_index;
	
    for (int i = 0; i < word.phonics.size(); ++i) {
        auto phonic_data = word.phonics[i];
        std::vector<int> indexes;
        //check chữ
        int index = 0;
        for (int j = 0; j < phonic_data.size(); ++j) {
            CCLOG("test %c",phonic_data[j]);
            if (!checkRomaji(phonic_data[j])){
                if (index == 0){
                    indexes.push_back(start_index + index);
                    index++;
                }else{
                    if(checkRomaji(phonic_data[j-1])){
                        indexes.push_back(start_index + index);
                        index++;
                    }
                }
            }else{
                indexes.push_back(start_index +  index);
                index++;
            }
        }
        list_index.push_back(indexes);
        start_index += (index);
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
void BalloonTheClown_VM::playTextDau(Text* text){
    AUDIO_ENGINE->play(word.audio_khong_dau);
    text->runAction(EaseBounceOut::create(Sequence::create(CallFunc::create([=]{
        text->setTextColor(Color4B(250, 54, 208, 255));
    }), ScaleTo::create(0.3f, 1, 1.f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=]{
        text->setTextColor(Color4B::WHITE);
    }),DelayTime::create(0.5f),CallFunc::create([=]{
        AUDIO_ENGINE->play(word.audio_dau);
        auto text_dau = text->getChildByName<Text*>("dau");
        //text_dau->setTextColor(Color4B(250, 54, 208, 255));
        text_dau->runAction(Sequence::create( CallFunc::create([=]{
            text_dau->setColor(Color3B(250, 54, 208));
        }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=]{
            text_dau->setColor(Color3B(255, 255, 255));
        }), NULL));
    }), NULL)));
}
void BalloonTheClown_VM::playSlowText(Text* text){
    AUDIO_ENGINE->play(word.s_audio);
    text->runAction(EaseBounceOut::create(Sequence::create(CallFunc::create([=]{
        text->setTextColor(Color4B(250, 54, 208, 255));
        if (word.co_dau) text->getChildByName<Text*>("dau")->setTextColor(Color4B(250, 54, 208, 255));
    }), ScaleTo::create(1, 1.2), ScaleTo::create(1, 1), CallFunc::create([=]{
        text->setTextColor(Color4B::WHITE);
        if (word.co_dau) text->getChildByName<Text*>("dau")->setTextColor(Color4B::WHITE);
    }), NULL)));
}

void BalloonTheClown_VM::playNormalText(Text* text){
    AUDIO_ENGINE->play(word.audio);
    text->runAction(EaseBounceOut::create(Sequence::create(CallFunc::create([=]{
        text->setTextColor(Color4B(250, 54, 208, 255));
        if (word.co_dau) text->getChildByName<Text*>("dau")->setTextColor(Color4B(250, 54, 208, 255));
    }), ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1), CallFunc::create([=]{
        text->setTextColor(Color4B::WHITE);
        if (word.co_dau) text->getChildByName<Text*>("dau")->setTextColor(Color4B::WHITE);
    }), NULL)));
}
bool BalloonTheClown_VM::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    return false;
}
