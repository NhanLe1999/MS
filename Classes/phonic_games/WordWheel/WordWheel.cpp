
#include "WordWheel.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include <math.h>
INITIALIZE_READER(WordWheel);
#define speed_opacity 15

std::once_flag WordWheel_reader;
#define CSB_NAME "csb/game/WordWheel/WordWheel.csb"

//sounds
#define correct "sounds/wordwheel/correct.mp3"
#define correct_alone "sounds/wordwheel/correct_alone.mp3"
#define wrong "sounds/wordwheel/wrong.mp3"
#define bg_music "sounds/wordwheel/music.mp3"
#define end_bg "sounds/wordwheel/win_end_game.mp3"
#define pick "sounds/wordwheel/pick.mp3"
#define show_up_images "sounds/wordwheel/show_up_images.mp3"
#define start_bg "sounds/wordwheel/start_game.mp3"
#define awesome "sounds/wordwheel/awesome.mp3"
#define intro "sounds/wordwheel/intro.mp3"

#define question_path "sounds/wordwheel/"

WordWheel * WordWheel::createGame(std::string json_file, bool isAutoPlay) {
	std::call_once(WordWheel_reader, [] {
        REGISTER_CSB_READER(WordWheel);
        REGISTER_CSB_READER(WordWheel_Item);
    });
    auto p = static_cast<WordWheel *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void WordWheel::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    rapidjson::Value& json_data=document.FindMember("data")->value;
    if (JSON_SAFE_CHECK(document, "number_game: S? lu?t choi trong game", String)){
        max_game = document.FindMember("number_game: S? lu?t choi trong game")->value.GetInt();
    }
    if (JSON_SAFE_CHECK(document, "number_bubble: S? Bubble trong game", String)){
        number_bubble = document.FindMember("number_bubble: S? Bubble trong game")->value.GetInt();
    }
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && document.HasMember("audio_intro_vi")){
        if(JSON_SAFE_CHECK(document, "audio_intro_vi", Array)) {
            auto audio_intro = document["audio_intro_vi"].GetArray();
            for (int i = 0; i < (int)audio_intro.Size(); i++) {
                auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
                question_vi = img_audio->getAudios("name_1").front();
            }
        }
    }
    
    
    this->generateData(json_data.GetArray(), images);
    
    if(max_game > data.size()){
        max_game = (int) data.size();
    }

    if(number_bubble > _list_phonic_info.size()){
        number_bubble = _list_phonic_info.size();
    }
    
    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;
    this->loadFrames();
    
}

WordWheel * WordWheel::createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay, std::string game_type) {
    std::call_once(WordWheel_reader, [] {
        REGISTER_CSB_READER(WordWheel);
        REGISTER_CSB_READER(WordWheel_Item);
    });
    auto p = static_cast<WordWheel *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(list_PHONIC_INFO, game_type, isAutoPlay);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void WordWheel::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
    _list_PHONIC_INFO = list_PHONIC_INFO;
    _list_phonic_info = _list_PHONIC_INFO.list_phonic;
    
    type_game = game_type;
    if(max_game > list_PHONIC_INFO.list_phonic.size()){
        max_game = (int) list_PHONIC_INFO.list_phonic.size();
    }

    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;
    this->loadFrames();
}

void WordWheel::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        rapidjson::Value& json_game = value_data[i].FindMember("game")->value;
        
        std::vector<mj::PHONIC_INFO> phonics;
		mj::PHONIC_INFO correct_phonic;
        for(rapidjson::SizeType j = 0; j < json_game.Size(); j++){
            rapidjson::Value& json_item = json_game[j].FindMember("item")->value;
			auto is_correct = false;
			if (json_item.HasMember("is_correct") && json_item.FindMember("is_correct")->value.GetBool() == true) is_correct = true;
            auto letter = json_item.FindMember("text")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
            
            mj::PHONIC_INFO phonic;
            phonic.text_full = pImage->getText("name_1");// json_game[j].FindMember("item")->value.FindMember("text")->value.GetString();
            phonic.audio_full = pImage->getAudios("name_1").front();//json_game[j].FindMember("item")->value.FindMember("audio")->value.GetString();
            phonic.text_phonic = pImage->getText("name_n");
            phonic.audio_phonic = pImage->getAudios("name_n").front();
            phonic.image_path = pImage->path;
            phonic.video_path = pImage->getAudios("position").front();
            phonic.highlights = {0};
            phonic.highlight_color = Color3B::RED;
			if (is_correct) {
				correct_phonic = phonic;
			}
			else {
				phonics.push_back(phonic);
			}
            
		
			
        }
        
        WWGAME game;
        game.type = value_data[i].FindMember("kind")->value.GetString();
        game.game = phonics;
		game.correct_word = correct_phonic;
        data.push_back(game);
  
     }
}

void WordWheel::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->setTouchEnabled(true);
    
    AUDIO_ENGINE->playSound(bg_music, true, nullptr, 0.5f);
	

    if(autoPlay){
        this->runAction(Sequence::create(DelayTime::create(5.0), CallFunc::create([=]{
//            this->hideTapGuideGame();
            this->onAutoPlay();
        }), NULL));
        
    }
}

void WordWheel::loadFrames(){
    
    root_panel = utils::findChild<Layout*>(this, "Panel_1");
    
    left_panel = utils::findChild<Layout*>(root_panel, "Panel_2");
    left_panel->setPosition(Vec2(-screen_size.width*0.5, 0));
    energyBar = utils::findChild(left_panel, "progressBar");
    auto energyWidth = utils::findChild(left_panel, "progressHold")->getContentSize().width *0.8;
    auto energyHeight = utils::findChild(left_panel, "progressHold")->getContentSize().height/max_game;
    energyBar->setContentSize(Size(energyWidth, energyHeight));
    energyBar->setScaleY(0.01);
    
    //create character
    character = GAFWrapper::createGAF("gaf/wordwheel/character/character.gaf");
    left_panel->addChild(character);
    character->setAnchorPoint(Point(0.6,0.5));
    character->setPosition(Vec2(left_panel->getContentSize().width / 2 - 35, left_panel->getContentSize().height*0.3));
    
    //text holder
    holder = GAFWrapper::createGAF("gaf/wordwheel/tv/tv.gaf");
    root_panel->addChild(holder);
    holder->setPosition(Vec2(root_panel->getContentSize().width*0.65, root_panel->getContentSize().height*0.5));
    holder->playSequence("stop",false);
    holder->setContentSize(Size(280,280));
    holder->setScale(0.01);
    holder->setRotation(-30);
    holder->setLocalZOrder(10);
    
    //bolt effect
    bolt = GAFWrapper::createGAF("gaf/wordwheel/set2/set2.gaf");
    holder->addChild(bolt, -11);
    bolt->setName("set2");
    bolt->setAnchorPoint(Point(0, 0));
    bolt->setPosition(Vec2(holder->getContentSize().width*0.52, holder->getContentSize().height*0.45));
    bolt->setScale(0.1);
    bolt->playSequence("action",true);
    
	// speaker
	speaker = cocos2d::ui::Button::create();
	speaker->loadTextures("mjstory/new_ui/speaker.png", "mjstory/new_ui/speaker.png");
	speaker->addClickEventListener(CC_CALLBACK_1(WordWheel::onSpeakerClicked, this));
	speaker->setPosition(Vec2(80, screen_size.height - 70));
	this->addChild(speaker, 10000);
    speaker->setTouchEnabled(false);


    //go in game
	this->scheduleOnce([=](float dt) {
		if (!CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->play(intro);
		this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=] {
			left_panel->runAction(MoveTo::create(0.5f, Vec2(0, 0)));
		}), DelayTime::create(1.0f), CallFunc::create([=] {
			character->playSequence("open", false);
			showGame();
		}), NULL));
	}, 0.f, "play intro");
    
    
}

void WordWheel::showGame(){
    this->unscheduleAllCallbacks();
    angle =0;
    count_wrong = 0;
    bolt->stopAllActions();
    
    _list_phonic_info = data[count_game].game;
    mj::helper::shuffle(_list_phonic_info);
    
	phonic = data[count_game].correct_word;

	_list_phonic_info.insert(_list_phonic_info.begin(), phonic);
    //create text
    txt_phonic = phonic.text_phonic;
    txt_full = phonic.text_full;
    Text* text = Text::create(txt_phonic, font_name, 100);
    text->setName("box_text");
    holder->addChild(text,30);
    text->setPosition(Vec2(holder->getContentSize().width*0.52, holder->getContentSize().height*0.45));
    text->setTextColor(Color4B::BLACK);
    
    //create bubbles panel
    item_panel = Layout::create();
    item_panel->setContentSize(Size(500,500));
    root_panel->addChild(item_panel,10000);
    item_panel->setAnchorPoint(Point(0.5, 0.5));
    item_panel->setPosition(holder->getPosition());
    item_panel->setName("item_layout");
    item_panel->addTouchEventListener (CC_CALLBACK_2(WordWheel::onTvTouched, this));
    item_panel->setTouchEnabled(false);
    auto textSize = text->getBoundingBox().size.width;
    auto itemSize = item_panel->getBoundingBox().size.width / 2;
    auto textFontSize = text->getFontSize();
    float ratioText = textSize / itemSize;
    if (ratioText >= 1)
    {
        text->setFontSize(textFontSize / (ratioText + 0.7));
    }
    //text->setFontSize(textFontSize);
    //create bubbles
    number_bubble = random(4, 7);
    if(number_bubble > _list_phonic_info.size()){
        number_bubble = (int)_list_phonic_info.size();
    }
    
    //holder_pos converted
    holder_pos =  item_panel->convertToNodeSpace(holder->getPosition());;
    
    items.clear();
    for (int i =0 ; i < number_bubble; i++) {
        WordWheel_Item* item = WordWheel_Item::createItem(_list_phonic_info[i]);
        item->setOpacity(0);
        item_panel->addChild(item,1000);
        item->setDelegate(this);
        item->setPosition(holder_pos);
        item->setTouchEnabled(false);
        items.push_back(item);
        
        CCLOG("item name: %s", utils::findChild<Text*>(item, "bubble_textF")->getString().c_str());
        
        if(_list_phonic_info[i].text_phonic == txt_phonic){
            right_item = item;
        }
    }
    
    //position list
    pos_list = calculate_pos(number_bubble, bankinh);

    enabledAllItems(false);

    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
        if(count_game == 0){
          
            auto seq1 = Sequence::create(Repeat::create(Sequence::create(RotateTo::create(0.2f, 10), RotateTo::create(0.2f, -10), NULL),2), RotateTo::create(0.1f, 0), NULL);
            
            auto seq2 = Sequence::create(Repeat::create(Sequence::create( ScaleTo::create(0.1f, 0.6, 1.2),ScaleTo::create(0.1f, 0.7, 1.1),NULL),2),ScaleTo::create(0.1f, 1), NULL);
            
            holder->runAction(Sequence::create(Spawn::create(RotateTo::create(0.5f, 20.0f), ScaleTo::create(0.5f, 1), NULL), seq1, CallFunc::create([=]{
                       this->runAction(Sequence::create(CallFunc::create([=]{
                       }),NULL));

                }),seq2, NULL));
        }

        character->playSequence("idle",true);
        
        holder->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
            holder->playSequence("action", false);
        }),DelayTime::create(0.8f), CallFunc::create([=]{
            int i = 0;
            for(WordWheel_Item* bubble: items){
                bubble->runAction(FadeIn::create(0.2f));
                utils::findChild(bubble, "image")->runAction(FadeIn::create(0.2f));
                bubble->runAction(EaseBackOut::create(MoveTo::create(0.4f, pos_list[i])));
                i++;
            }
            
            AUDIO_ENGINE->play(show_up_images);
        }), DelayTime::create(0.6f), CallFunc::create([=]{
           
            text->runAction(Sequence::create(ScaleTo::create(0.2f, 1, 1.2),ScaleTo::create(0.15f, 1, 1) , NULL));
            item_panel->runAction(RepeatForever::create(RotateBy::create(0.1f, 1.0f)));

            for(WordWheel_Item* bubble: items){
                utils::findChild(bubble, "image")->runAction(RepeatForever::create(RotateBy::create(0.1f, -1.0f)));
            }
            holder->playSequence("idle", true);
        }),CallFunc::create([=]{
            //say question
//            int index = random(0, (int)question_list.size()-1);
            string question = question_path + data[count_game].type + ".mp3"; //question_path + _list_phonic_info[0].type;
            if (CONFIG_MANAGER->isVietnameseStories()){
                question = question_vi;
            }
            AUDIO_ENGINE->play(question, false, 1.0, [=]{
                AUDIO_ENGINE->playSound(phonic.audio_phonic, false, [=] {
                    speaker->setTouchEnabled(true);
                    item_panel->setTouchEnabled(true);
                    enabledAllItems(true);
                    if (flag) {
                        this->unschedule("animated_guide");
                        AUDIO_ENGINE->stopEffect(_audioGuideGame);
                        this->inActiveMode(10.0f);
                    }
                    else
                    {
                        stopLearningReminder();
                        learningReminder(10.0f);
                    }
                    });
            });
        }),NULL));
    })  , NULL));
}

std::vector<Vec2> WordWheel::calculate_pos(int number_point, int r){
    std::vector<Vec2> pos_list;
    
    auto angle = 360/number_point;
    int rad = 0;
    
    for (int i =0; i < number_point; i++){
        float x = r * cosf(CC_DEGREES_TO_RADIANS(rad));
        float y = r * sinf(CC_DEGREES_TO_RADIANS(rad));
        
        Vec2 pos (x + holder_pos.x, y+ holder_pos.y);
        angles.push_back(rad);
        rad += angle;
        pos_list.push_back(pos);
    }
    return pos_list;
}

void WordWheel::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllAudio();
}

void WordWheel::onTvTouched(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if(holder->getBoundingBox().containsPoint( root_panel->convertToNodeSpace(((Layout*) sender)->getTouchBeganPosition()))){
                onSpeakerClicked(this);
            }
            break;
        default:
            break;
    }
}

void WordWheel::onTouchedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    
    this->unscheduleAllCallbacks();
    WordWheel_Item* item = (WordWheel_Item*) sender;
    item->stopAllActions();
    
    mj::PHONIC_INFO phonic;
    for(int i = 0; i < _list_phonic_info.size(); i++){
        std::string item_text = utils::findChild<Text*>(item, "bubble_textF")->getString().c_str();
        if (_list_phonic_info[i].text_full == item_text){
            phonic = _list_phonic_info[i];
        }
    }
    
    //disable touch
    enabledAllItems(false);
    
    float goc_set, scale;
    
        switch(type){
            case ui::Widget::TouchEventType::BEGAN :
                stopLearningReminder();
                this->unschedule("animated_guide");
                AUDIO_ENGINE->stopEffect(_audioGuideGame);
                itemOverLay();
                CCLOG("touched");
                AUDIO_ENGINE->play(pick);
                AUDIO_ENGINE->playSound(phonic.audio_full, false, [=] {
                    removeItemOverLay();
                    });
                //clone item
                item_cloned = WordWheel_Item::createItem(phonic);
                item_cloned->setAnchorPoint(Point(0.5, 0.5));

                start_pos = item_panel->convertToWorldSpace(item->getParent()->getPosition());
                CCLOG("start_pos: %f, %f", start_pos.x, start_pos.y);
                utils::findChild(item_cloned, "image")->setOpacity(255);
                root_panel->addChild(item_cloned,90000);
                item_cloned->setPosition(start_pos);
                item_cloned->setName("item_cloned");

                item->setOpacity(0);

                //bolt effect
                bolt->stopAllActions();
                goc_set = calculateAngle(start_pos, holder->getPosition());
                bolt->setRotation(-goc_set);
                
                bolt->setScale(1.0f);
                break;
                
            case ui::Widget::TouchEventType::MOVED :
                CCLOG("moved");
                item_cloned->setPosition(((Layout*)sender)->getTouchMovePosition());
                //bolt effect
                goc_set = calculateAngle(((Layout*)sender)->getTouchMovePosition(), holder->getPosition());
                scale = (item_cloned->getPosition().getDistance(holder->getPosition()) - item_cloned->getContentSize().width*0.3)/bankinh;

                bolt->setRotation(-goc_set);
                bolt->setScale(scale, 1);
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                stopAudioSpeaker();
                if (flag) {
                    this->unschedule("animated_guide");
                    AUDIO_ENGINE->stopEffect(_audioGuideGame);
                    this->inActiveMode(10.0f);
                }
                else
                {
                    stopLearningReminder();
                    learningReminder(10.0f);
                }
                CCLOG("ended");
                item_cloned->setOpacity(0);
                item_cloned->removeFromParent();
                bolt->setScale(0.1);
                item->setOpacity(255);

                //enable touch
                enabledAllItems(true);
                break;

            default:
                CCLOG("canceled");
                bolt->setScale(0.1);
                if(holder->getBoundingBox().containsPoint(item_cloned->getPosition())){
                    if(checkDrop(item_cloned)){
                        stopAudioSpeaker();
                        flag = false;
                        this->unschedule("animated_guide");
                        //neu keo dung
                        this->runAction(Sequence::create(CallFunc::create([=]{
                            //AUDIO_ENGINE->play(awesome);
                        }), DelayTime::create(0.5f), CallFunc::create([=]{
                            AUDIO_ENGINE->play(correct_alone);
                        }), NULL));
                        
                        this->runAction(Sequence::create(CallFunc::create([=]{
                            character->playSequence("win", false, [=](gaf::GAFObject *obj, string str){
                                character->playSequence("idle", true);
                            });
                        }), DelayTime::create(0.1f), CallFunc::create([=]{
                            character->playSequence("win", false);
                        }), NULL));
                       
                        Vec2 item_pos (holder->getPosition().x+2, holder->getPosition().y-12);
                        item_cloned->runAction(Spawn::create(MoveTo::create(0.2f, item_pos), ScaleTo::create(0.2f, 0.8), NULL));
                        onNextStage();
                    }else{
                        //neu keo sai
                        stopAudioSpeaker();
                        if (flag) {
                            this->unschedule("animated_guide");
                            AUDIO_ENGINE->stopEffect(_audioGuideGame);
                            this->inActiveMode(10.0f);
                        }
                        else
                        {
                            stopLearningReminder();
                            learningReminder(10.0f);
                        }
                        AUDIO_ENGINE->play(wrong);
                        character->playSequence("lose",false, [=](gaf::GAFObject *obj, string str){
                            character->playSequence("idle", true);
                        });
                        this->runAction(Sequence::create(CallFunc::create([=]{
                            item_cloned->runAction(EaseBackOut::create( MoveTo::create(0.15f, item_panel->convertToWorldSpace(item->getParent()->getPosition()))));
                        }), DelayTime::create(0.15f), CallFunc::create([=]{
                            item->setOpacity(255);
                            item_cloned->setOpacity(0);
                            item_cloned->removeFromParent();
                            //enable touch
                            enabledAllItems(true);
                        }), NULL));
                        
                        count_wrong++;
                        //sai hon 3 lan
                        if(count_wrong > 2){
                            showHint();
                        }
                    }
                }else{
                    //keo ra ngoai
                    float time = moveTime(item, item_cloned);
                    this->runAction(Sequence::create(CallFunc::create([=]{
                        item_cloned->runAction(EaseBackOut::create( MoveTo::create(time, item_panel->convertToWorldSpace(item->getParent()->getPosition()))));
                    }), DelayTime::create(time), CallFunc::create([=]{
                        item->setOpacity(255);
                        item_cloned->setOpacity(0);
                        item_cloned->removeFromParent();
                        //enable touch
                        enabledAllItems(true);
                    }), NULL));
                }
                stopAudioSpeaker();
                if (flag) {
                    this->unschedule("animated_guide");
                    AUDIO_ENGINE->stopEffect(_audioGuideGame);
                    this->inActiveMode(10.0f);
                }
                else
                {
                    stopLearningReminder();
                    learningReminder(10.0f);
                }
                break;
        }
}

float WordWheel::calculateAngle(cocos2d::Vec2 pos, Vec2 center){
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
    CCLOG("%f", angle);
    return angle;
}

void WordWheel::onSpeakerClicked(cocos2d::Ref * sender)
{
    if (flag) {
        this->unschedule("animated_guide");
        AUDIO_ENGINE->stopEffect(_audioGuideGame);
        this->inActiveMode(10.0f);
    }
    else
    {
        stopLearningReminder();
        learningReminder(10.0f);
    }
	string question = question_path + data[count_game].type + ".mp3"; //question_path + _list_phonic_info[0].type;
    if (CONFIG_MANAGER->isVietnameseStories()){
        question = question_vi;
    }
    AUDIO_ENGINE->stopSound(question);
	_audioSpeaker = AUDIO_ENGINE->playSound(question, false,[=] {
		AUDIO_ENGINE->play(phonic.audio_phonic);
	});
}

void WordWheel::stopAudioSpeaker() {
    AUDIO_ENGINE->stopEffect(_audioSpeaker);
}

void WordWheel::showHint(){
    right_item->runAction(RepeatForever::create(Spawn::create(Sequence::create(ScaleTo::create(0.3f, 1.1f), ScaleTo::create(0.3f, 0.9f), NULL), NULL)));
}

float WordWheel::moveTime(WordWheel_Item *item, WordWheel_Item* item_clone){
    float time = 0.1f;
    auto item_pos = item_panel->convertToWorldSpace(item->getParent()->getPosition());
    float distance = item_clone->getPosition().getDistance(item_pos);
    time *= distance/bankinh ;
    return time;
}

bool WordWheel::checkDrop(WordWheel_Item* item){
    if(utils::findChild(item, "bubble_textP") && utils::findChild<Text*>(item, "bubble_textP")->getString() == txt_phonic){
            return true;
    }else
        return false;
}

void WordWheel::onNextStage(){
    speaker->setTouchEnabled(false);
    count_game++;
    ////transition game
    this->unscheduleUpdate();
    //energy increase
    utils::findChild(left_panel, "pipe")->runAction(Sequence::create(ScaleTo::create(0.1f, 1.1, 1), ScaleTo::create(0.15f, 0.8, 1), ScaleTo::create(0.15f, 1.3, 1), ScaleTo::create(0.1f, 1.0, 1), NULL));
    auto character_pos = character->getPosition();
    character->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(character_pos.x+10, character_pos.y)), MoveTo::create(0.15f, Vec2(character_pos.x-10, character_pos.y)), MoveTo::create(0.15f, Vec2(character_pos.x+10, character_pos.y)), MoveTo::create(0.1f, Vec2(character_pos.x, character_pos.y)), NULL));
    energyBar->runAction(ScaleTo::create(0.5f, 1,count_game));

    GAFWrapper* smoke1 = GAFWrapper::createGAF("gaf/wordwheel/smoke1/smoke1.gaf");
    utils::findChild(left_panel, "pipe")->addChild(smoke1, -1);
    smoke1->setPosition(utils::findChild(left_panel, "pipe")->getContentSize()/2);
    smoke1->play(false);
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        smoke1->setVisible(false);
        smoke1->removeFromParent();
    }), NULL));
    
    //hide all items
    for(WordWheel_Item* item: items){
        item->setOpacity(0);
    }
    
    //remove items panel
    item_panel->stopAllActions();
    item_panel->removeFromParent();
    
    //hide text
    utils::findChild(holder, "box_text")->removeFromParent();
    
    //gaf smoke
    GAFWrapper* smoke2 = GAFWrapper::createGAF("gaf/wordwheel/smoke2/smoke2.gaf");
    holder->addChild(smoke2);
    smoke2->setPosition(Vec2(holder->getContentSize()/2));
    smoke2->play(false);
    
    holder->runAction(Sequence::create(ScaleTo::create(0.1f, 1.2f, 0.8f), ScaleTo::create(0.1f, 0.8f,1.2f), ScaleTo::create(0.1f, 1.2f,0.8f), ScaleTo::create(0.1f, 1.0f, 1.0f), NULL));
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
//        hide cloned item
        utils::findChild(root_panel, "item_cloned")->runAction(Sequence::create(FadeOut::create(1.0f), DelayTime::create(1.0f), CallFunc::create([=]{
            utils::findChild(root_panel, "item_cloned")->removeFromParent();
        }), NULL));
    }), DelayTime::create(1.0f), CallFunc::create([=]{
        nextGame();
    }), NULL));
    
}

void WordWheel::onFinishGame(){
    
    if(autoPlay){
        this->unscheduleAllSelectors();
//        this->unschedule("autoTesting");
    }
    
    this->unscheduleAllCallbacks();
   
    
    GAFWrapper* smoke1 = GAFWrapper::createGAF("gaf/wordwheel/smoke1/smoke1.gaf");
    utils::findChild(left_panel, "pipe")->addChild(smoke1);
    smoke1->setPosition(Vec2(utils::findChild(left_panel, "pipe")->getContentSize()/2));
    smoke1->setVisible(false);
    this->runAction(Sequence::create(CallFunc::create([=]{
        character->playSequence("close_win");
    }) ,DelayTime::create(0.5f),CallFunc::create([=]{
        root_panel->runAction(MoveTo::create(0.5f, Vec2(root_panel->getContentSize().width/3,0)));
        holder->runAction(FadeOut::create(0.5f));
        AUDIO_ENGINE->stopAllAudio();
    }),DelayTime::create(1.0f), CallFunc::create([=]{
        AUDIO_ENGINE->play(end_bg);
        energyBar->runAction(ScaleTo::create(1.5f, 1, 0));
        
        utils::findChild(left_panel, "pipe")->runAction(Sequence::create(ScaleTo::create(0.1f, 1.1, 1), ScaleTo::create(0.15f, 0.8, 1), ScaleTo::create(0.15f, 1.3, 1), ScaleTo::create(0.15f, 0.8, 1), ScaleTo::create(0.15f, 1.3, 1), ScaleTo::create(0.1f, 1.0, 1), NULL));
        auto character_pos = character->getPosition();
        character->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(character_pos.x+10, character_pos.y)), MoveTo::create(0.15f, Vec2(character_pos.x-10, character_pos.y)), MoveTo::create(0.15f, Vec2(character_pos.x+10, character_pos.y)), MoveTo::create(0.15f, Vec2(character_pos.x-10, character_pos.y)),MoveTo::create(0.15f, Vec2(character_pos.x+10, character_pos.y)),MoveTo::create(0.1f, Vec2(character_pos.x, character_pos.y)), NULL));
        
        smoke1->setVisible(true);
        smoke1->play(true);
        smoke1->runAction(MoveTo::create(1.0f, Vec2(smoke1->getPosition().x, energyBar->getPosition().y)));
    }), DelayTime::create(0.5f), CallFunc::create([=]{
        smoke1->play(false);
        smoke1->setVisible(false);
        smoke1->removeFromParent();
        
    }) ,DelayTime::create(0.5f), CallFunc::create([=]{
                character->playSequence("final");
    }) , DelayTime::create(4.0f), CallFunc::create([=]{
//        this->unscheduleAllCallbacks();
//        this->unscheduleUpdate();
//        this->stopAllActions();
//        this->removeFromParent();
        MJMainGBase::onCloseGame();
    }) , NULL));
}

void WordWheel::nextGame(){
    if(count_game == max_game){
        this->runAction(Sequence::create( CallFunc::create([=]{
        }), DelayTime::create(1.2f), CallFunc::create([=]{
                this->onFinishGame();
        }) , NULL));
        
    }else {
        showGame();
        if(!autoPlay){
//            showIntroGame();
        }
    }
}

void WordWheel::showTapGuideGame(float delay_time, std::string gaf_guide_path){
        auto hand = GAFWrapper::createGAF(gaf_guide_path);
        auto item = right_item;
        Vec2 it_pos = item->getPosition();
        utils::findChild<ImageView*>(item, "image")->addChild(hand, 1900000000,"guide_gaf");
        hand->setPosition(Vec2(utils::findChild<ImageView*>(item, "image")->getContentSize().width*0.25, utils::findChild<ImageView*>(item, "image")->getContentSize().height*0.3));
        hand->setScale(0.5);
        item->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(MoveTo::create(0.5f, holder_pos), CallFunc::create([=]{
        }), NULL), CallFunc::create([=]{
            hand->removeFromParent();
        }), MoveTo::create(0.3f, it_pos) , NULL));
        string question = question_path + data[count_game].type + ".mp3";
        AUDIO_ENGINE->stopSound(question);
        _audioGuideGame = AUDIO_ENGINE->playSound(question, false, [=] {
            AUDIO_ENGINE->play(phonic.audio_phonic);
        });
}

void WordWheel::inActiveMode(float delay_time){
    this->schedule([=](float){
        this->showTapGuideGame();
    }, 13, 100, delay_time, "animated_guide");
}

#define auto_testing_delay 4.5f
void WordWheel::onAutoPlay()
{
    this->schedule([=](float dt) {
        this->autoPlayRun(4.5f);
    },auto_testing_delay,1000000,0, "autoTesting");
}

void WordWheel::autoPlayRun(float dt)
{
    int index = random(0, (int)items.size()-1);
    auto item = items[index];

    mj::PHONIC_INFO phonic;
    for(int i = 0; i < _list_phonic_info.size(); i++){
        std::string item_text = utils::findChild<Text*>(item, "bubble_textF")->getString().c_str();
        if(count_wrong < 3 ){
            if (_list_phonic_info[i].text_full == item_text){
                phonic = _list_phonic_info[i];
            }
        }else{
            if(_list_phonic_info[i].text_full == txt_full){
                phonic = _list_phonic_info[i];
                item = right_item;
            }
        }
    }
    
    item_cloned = WordWheel_Item::createItem(phonic);
    item_cloned->setAnchorPoint(Point(0.5,0.5));
    utils::findChild(item_cloned, "image")->setOpacity(255);
    root_panel->addChild(item_cloned,1000);
    item_cloned->setName("item_cloned");
    item_cloned->setPosition(item_panel->convertToWorldSpace(item->getPosition()));

    item->setOpacity(0);
    this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=]{
        item_cloned->runAction(MoveTo::create(0.5f, holder->getPosition()));
        
    }), DelayTime::create(0.5f), CallFunc::create([=]{
        
        if(checkDrop(item_cloned)){
            //neu keo dung
            AUDIO_ENGINE->play(correct_alone);
            character->playSequence("win", false, [=](gaf::GAFObject *obj, string str){
                character->playSequence("idle", true);
            });
            Vec2 item_pos (holder->getPosition().x+2, holder->getPosition().y-12);
            item_cloned->runAction(MoveTo::create(0.2f, item_pos));
            onNextStage();
        }else{
            //neu keo sai
            AUDIO_ENGINE->play(wrong);
            character->playSequence("lose",false, [=](gaf::GAFObject *obj, string str){
                character->playSequence("idle", true);
            });
            this->runAction(Sequence::create(CallFunc::create([=]{
                item_cloned->runAction(EaseBackOut::create( MoveTo::create(0.15f, item_panel->convertToWorldSpace(item->getParent()->getPosition()))));
            }), DelayTime::create(0.15f), CallFunc::create([=]{
                item->setOpacity(255);
                item_cloned->setOpacity(0);
                item_cloned->removeFromParent();
            }), NULL));
            count_wrong++;
            
            //sai hon 3 lan
            if(count_wrong > 2){
                showHint();
            }
        }
    }), NULL));
    
    this->scheduleOnce([=](float dt) {
        this->onAutoPlay();
    }, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}

void WordWheel::enabledAllItems(bool _lock){
    for(int i = 0; i < items.size(); i++){
        items[i]->setEnabled(_lock);
    }
}

void WordWheel::learningReminder(float delay_time) {
    this->schedule([=](float) {
        string question = question_path + data[count_game].type + ".mp3"; //question_path + _list_phonic_info[0].type;
        AUDIO_ENGINE->stopSound(question);
        _audio = AUDIO_ENGINE->playSound(question, false, [=] {
            AUDIO_ENGINE->play(phonic.audio_phonic);
            });
        }, 10, 100, delay_time, "learning_reminder");
}

void WordWheel::stopLearningReminder() {
    this->unschedule("learning_reminder");
    AUDIO_ENGINE->stopEffect(_audio);
}
void WordWheel::itemOverLay() {
    auto screen_size = Director::getInstance()->getVisibleSize();
    Layout* overlay = Layout::create();
    overlay->setContentSize(Size(screen_size.width, screen_size.height));
    overlay->setAnchorPoint(Vec2(0.5, 0.5));
    overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
    overlay->setName("overlay");
    this->addChild(overlay, INT_MAX - 1);
    overlay->setTouchEnabled(true);
    overlay->addClickEventListener([=](cocos2d::Ref* sender) {
        CCLOG("ok");
        });
}

void WordWheel::removeItemOverLay() {
    this->removeChildByName("overlay");
}
