//
//  Snail.h
//


#include "Snail.h"
#include "JsonDump.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
#include "AskPermissionPopup.h"
#define speed_walk 10.0f
#define speed_skate 3.0f
#define speed_jet 1.0f
#define opacity_hide 120
#define key_num_guided "key_num_guided_snail"
#define key_num_guided_record "key_num_guided_record"
#define max_num_guide 3
using namespace cocos2d;
INITIALIZE_READER(Snail);
USING_NS_CC;
std::once_flag snail_reader;
#define CSB_NAME "csb/game/snail/Snail.csb"

Snail * Snail::createGame(std::string json_file) {
    std::call_once(snail_reader, [] {
        REGISTER_CSB_READER(Snail);
        REGISTER_CSB_READER(Snail_Character);
    });
    auto p = static_cast<Snail *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void Snail::didLoadFromCSB(std::string json_file){
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

    generateData(value_data.GetObject());
//    list_item_info.erase(list_item_info.begin(), list_item_info.begin()+2);
}

void Snail::generateData(rapidjson::Value value_data){

     std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    if (value_data.HasMember("number_turnplay") && value_data["number_turnplay"].IsInt()) {
        number_turnplay = value_data["number_turnplay"].GetInt();
    }

    if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
        auto list_data = value_data["list_item"].GetArray();
        for (int i = 0; i < list_data.Size(); i++) {
            auto item_data = list_data[i].GetObject();
            SNAIL_INFO snail_info;

            auto letter = item_data["text"].GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
            snail_info.text = pImage->getText("name_1");
            snail_info.audio_path_n = pImage->getAudios("name_1").front();
            snail_info.audio_path_s = pImage->getAudios("name_n").front();

            //TODO: audio and text phonic
            auto phonics = pImage->getTextPhonic("name_p");
            auto phonic_audios = pImage->getAudioPhonic("name_p");

            for (int j = 0; j < phonics.size(); j++) {
                Snail_Character::ITEM_INFO character_info;
                character_info.character = phonics[j];
                character_info.audio_path = phonic_audios[j];

                snail_info.list_character.push_back(character_info);
            }
            list_item_info.push_back(snail_info);
        }
    }

//    if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
//        auto list_data = value_data["list_item"].GetArray();
//        for (int i = 0; i < list_data.Size(); i++) {
//            auto item_data = list_data[i].GetObject();
//            SNAIL_INFO snail_info;
//            snail_info.text = item_data["text"].GetString();
//            snail_info.audio_path_s = item_data["audio_path_slow"].GetString();
//            snail_info.audio_path_n = item_data["audio_path_normal"].GetString();
//
//            auto list_character_data = item_data["list_character"].GetArray();
//            for (int j = 0; j < list_character_data.Size(); j++) {
//                auto character_data = list_character_data[j].GetObject();
//                Snail_Character::ITEM_INFO character_info;
//                character_info.character = character_data["character"].GetString();
//                character_info.audio_path = character_data["audio_path"].GetString();
//
//                snail_info.list_character.push_back(character_info);
//            }
//            list_item_info.push_back(snail_info);
//        }
//    }

    mj::helper::shuffle(list_item_info);

}

void Snail::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->scheduleOnce([=](float){
        MJMainGBase::playBackgroundMusic("sounds/snail/bg.mp3", 0.4f);
//        audio_bg_id = AUDIO_ENGINE->play("sounds/snail/bg.mp3", true);
//        cocos2d::experimental::AudioEngine::setVolume(audio_bg_id, 0.4f);
    }, 0.5f, "playBackgroundMusic");

//    this->scheduleOnce([=](float){
        this->loadFrames();
        this->showIntroGame();
//    }, 2, "create game");
}


void Snail::showIntroGame(){
    setTouchCharacter_Item(false);
    _speaker->setVisible(false);
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/name_game_snail.mp3")) {
            // name image
            if (cocos2d::FileUtils::getInstance()->isFileExist("games/snail/ten_game.png")) {
                auto nameImage = ImageView::create("games/snail/ten_game.png");
                auto winSize = Director::getInstance()->getWinSize();
                nameImage->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
                this->addChild(nameImage);
                nameImage->setScale(2);
                nameImage->setZOrder(INT_MAX - 1);
                nameImage->setOpacity(0);
                nameImage->runAction(Sequence::create(FadeIn::create(0.25), DelayTime::create(1.5), FadeOut::create(0.25), nullptr));
            }
            // name sound
            AUDIO_ENGINE->playSound("sounds/snail/name_game_snail.mp3", false, ([=] {
                this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
                    this->createCharacter();
                    this->showSnail();
                    setTouchCharacter_Item(false);
                    if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/Tap the picture.mp3")) {
                        AUDIO_ENGINE->play("sounds/snail/Tap the picture.mp3", false, 1, ([=] {
                            scheduleGuidingByHandAndSound();
                            root_layout->setTouchEnabled(true);
                            bridge->setTouchEnabled(true);
                            setTouchCharacter_Item(true);
                            _speaker->setVisible(true);                           
                            }));
                    }
                    
                }), nullptr));               
            }));
        }
        else {
            scheduleGuidingByHandAndSound();
            root_layout->setTouchEnabled(true);
            bridge->setTouchEnabled(true);
            this->createCharacter();
            this->showSnail();
        }
    }), NULL));
}

void Snail::onFinishGame()
{
	//MJMainGBase::onFinishGame();

	//AUDIO_ENGINE->stopAllAudio();
    hideGuiding();
    character_layout->setVisible(false);
    this->unschedule("inActiveMode");
	AUDIO_ENGINE->play("sounds/snail/snail_ending.mp3");
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 1900000000);
	touch_layout->setBackGroundColor(Color3B::BLACK);
	touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	touch_layout->setOpacity(180);
	auto snail = GAFWrapper::createGAF("gaf/snail/snail_ending/snail_ending.gaf");
	this->addChild(snail, 1900000000);
	snail->setPosition(cocos2d::Size(screen_size.width / 2, screen_size.height/2));
	snail->play(false, [=](gaf::GAFObject *obj) {
        AUDIO_ENGINE->stopAllAudio();
		MJMainGBase::onCloseGame();
	});
}

void Snail::setTouchCharacter_Item(bool enable)
{
    character_layout->setTouchEnabled(enable);
    if (!list_character.empty()) {
        for (auto i : list_character) {
            i->setTouchEnabled(enable);
        }
    }
}

void Snail::onSpeakerClicked(cocos2d::Ref *sender){
    if (speaker_audio_path != "") {

    }
}

void Snail::onExitTransitionDidStar(){
    MJMainGBase::onExitTransitionDidStart();
    if (_record && _is_recording){
        _record->stopRecord();
    }

}

void Snail::loadFrames(){
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    //
    //PHONIC_MANAGER->cacheGAF("gaf_tree", "gaf/snail/snail_trees/snail_trees.gaf");
    //auto tree1 = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_tree"]);
    //tree1->setPosition(Vec2(-100, screen_size.height/2 + 100));
    //this->addChild(tree1, 2);
    //tree1->play(true);
    //
    //auto tree2 = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_tree"]);
    //tree2->setPosition(Vec2(screen_size.width + 200, screen_size.height/2 + 100));
    //this->addChild(tree2, 2);
    //tree2->play(true);

    PHONIC_MANAGER->clearCacheGaf();

    root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "root_layout");
    root_layout->setTouchEnabled(false);
    root_layout->addClickEventListener([=](Ref*) {
        CCLOG("touched root layout");
        scheduleGuidingByHandAndSound();
        });
    root_layout->setLocalZOrder(1000000);

    bridge = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "bridge");
    bridge->setTouchEnabled(false);
    bridge->addClickEventListener([=](Ref*) {
        CCLOG("touched bridge");
        scheduleGuidingByHandAndSound();
        });
    character_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "character_layout");
    character_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
    bridge->setLocalZOrder(100);


    this->createCloud();
    snail_layout = cocos2d::ui::Layout::create();
    snail_layout->setContentSize(cocos2d::Size(100, 100));
    snail_layout->setPosition(Vec2(bridge->getContentSize().width/2, bridge->getContentSize().height/2));
    bridge->addChild(snail_layout, 1);
    snail_layout->setVisible(false);
    bridge->getChildByName("river")->setLocalZOrder(1000);
    character_layout->setLocalZOrder(1001);
    
    snail_layout->setBackGroundColor(Color3B::RED);

//    touch_snail = cocos2d::ui::Layout::create();
//    touch_snail->setContentSize(cocos2d::Size(170, 122));
//    touch_snail->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
//    touch_snail->setPosition(Vec2(10,-5));
//    touch_snail->setTouchEnabled(false);
//    touch_snail->addClickEventListener(CC_CALLBACK_1(Snail::onSnailClicked, this));
//    snail_layout->addChild(touch_snail);

    utils::findChild(this, "btnext")->setVisible(true);
    utils::findChild(this, "btprev")->setVisible(false);

    gaf_snail = GAFWrapper::createGAF("gaf/snail/snail_snail_2/snail_snail_2.gaf");
    snail_layout->addChild(gaf_snail, 1000);
    gaf_snail->setPosition(Vec2(450, 50));
    gaf_snail->playSequence("idle_walk", true);

    this->createButtonLevel();
    this->createButtonRecord();

    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    _speaker->setTouchEnabled(false);
    this->addChild(_speaker);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender)
        {
            hideGuiding();
            playSoundGuiding();

        }); 
}

void Snail::createCharacter(){

    character_layout->removeAllChildren();
    character_layout->setScale(1.0f);
    list_character.clear();
    index_character = 0;
    index_walk = 0;
    level_snail = LEVEL_WALK;
    index_level = LEVEL_WALK;
    is_rotate_snail = false;

    current_item = list_item_info[index_item];
    float width = 0;
    bool resize = (current_item.list_character.size() > 4)? true : false;
    for (int i = 0; i < current_item.list_character.size(); i++) {
        int type;
        if (i == 0) {
            type = Snail_Character::TYPE_BEGIN;
        }else if (i == (current_item.list_character.size() - 1)){
            type = Snail_Character::TYPE_END;
        }else
            type = Snail_Character::TYPE_MIDLE;

        auto character_item = Snail_Character::createItem(current_item.list_character[i], type, resize);
        character_layout->addChild(character_item);

        width = character_item->getContentSize().width;
        character_item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        character_item->setPosition(Vec2(width/2 + i*width, width/2));
        list_character.push_back(character_item);
        character_item->setDelegate(this);
        character_item->setScale(0);
        character_item->runAction(Sequence::create(DelayTime::create(0.05*i), ScaleTo::create(0.25f, 1.0f), NULL));
        character_item->setTouchEnabled(true);
        character_item->addClickEventListener([=](Ref* sender) {
            CCLOG("character item %d", i);
            setTouchCharacter_Item(false);
            btlevel1->setTouchEnabled(false);
            btlevel2->setTouchEnabled(false);
            btlevel3->setTouchEnabled(false);
            if (_enable_runTextHideLight) {
                runTextHighlight(i);
                this->runAction(Sequence::create(DelayTime::create(0.9), CallFunc::create([=] {
                    setTouchCharacter_Item(true);
                    btlevel1->setTouchEnabled(true);
                    btlevel2->setTouchEnabled(true);
                    btlevel3->setTouchEnabled(true);
                    }), nullptr));
            }
            scheduleGuidingByHandAndSound();
            });

		// đổi vị trí text để ko bị box đè lên
		//character_item->mText->removeFromParentAndCleanup(false);
		//character_layout->addChild(character_item->mText, 10);
		//character_item->mText->setPosition(character_item->getPosition());
		//character_item->mText->runAction(Sequence::create(DelayTime::create(0.05*i), ScaleTo::create(0.25f, 1.0f), NULL));

    }

    character_layout->setContentSize(cocos2d::Size(width*current_item.list_character.size(), list_character[0]->getContentSize().height));
    CCLOG("width = %f", width);
    character_layout->setPositionX(bridge->getContentSize().width/2 -character_layout->getContentSize().width/2);
    character_layout->setGlobalZOrder(2);
}

void Snail::showSnail(){
    _enable_runTextHideLight = true;
    scheduleGuidingByHandAndSound();
    auto pos_x = character_layout->getPositionX() - 30;
    snail_layout->setRotation(0);
    snail_layout->setPosition(Vec2(pos_x, this->getPosYGafSnail(pos_x)));
    snail_layout->setVisible(true);
    AUDIO_ENGINE->playEffect("sounds/snail/fade_in_walk.mp3");
    gaf_snail->playSequence("fade_in_walk", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
//        touch_snail->setTouchEnabled(true);

		if (index_level == LEVEL_WALK) {
			gaf_snail->playSequence("idle_walk", true);
			//setBtnSnailLevel(LEVEL_WALK);
		}
        else if (index_level == LEVEL_SKATE) {
			gaf_snail->playSequence("idle_skate", true);
			//setBtnSnailLevel(LEVEL_SKATE);
            //gaf_level2->playSequence("idle", true);
        }else if (index_level == LEVEL_JET){
			AUDIO_ENGINE->playEffect("sounds/snail/fade_in_jet.mp3");
			gaf_snail->playSequence("fade_in_jet", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				gaf_snail->playSequence("idle_jet", true);
				audio_action = AUDIO_ENGINE->play("sounds/snail/idle_jet.mp3", true);
			});
            //gaf_level3->playSequence("idle", true);
			//setBtnSnailLevel(LEVEL_JET);
        }
		setBtnSnailLevel((SNAIL_LEVEL)level_snail);

        /*for (int i = LEVEL_WALK; i <= index_level; i++) {
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([=]{
                cocos2d::utils::findChild<Layout *>(this, StringUtils::format("btlevel%d", i))->setTouchEnabled(true);
            })));
        }*/
        is_btlevel_clicked = false;

//        auto num_guided = MJDEFAULT->getIntegerForKey(key_num_guided, 0);
//        if (num_guided <= max_num_guide && index_level == LEVEL_WALK){
//            num_guided++;
//            MJDEFAULT->setIntegerForKey(key_num_guided, num_guided);
//            this->showTapGuideGame(root_layout->convertToWorldSpace(cocos2d::utils::findChild(this, StringUtils::format("btlevel%d", index_level))->getPosition()), 0.5f);
//        }

        //inActiveMode();


//        enableAllBtns();
    });
}

void Snail::onSnailClicked(cocos2d::Ref *sender){
    if (!is_btlevel_clicked) return;
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);
	this->hideTapGuideGame();
	this->unschedule("inActiveMode");
    scheduleGuidingByHandAndSound();
//    touch_snail->setTouchEnabled(false);
    if (level_snail == LEVEL_WALK) {
        gaf_snail->playSequence("go_walk", true);
        audio_action = AUDIO_ENGINE->play("sounds/snail/go_walk.mp3", true);
        index_walk = 0;
        index_character = 0;
        this->onSnailWalk();
    }else if (level_snail == LEVEL_SKATE) {
        this->onSnailSkate();
    }else {
        this->onSnailJet();
    }
}

void Snail::setBtnSnailLevel(SNAIL_LEVEL level)
{
	//level_snail = level;
	switch (level)
	{
	case Snail::LEVEL_NONE:
		btlevel1->setTouchEnabled(false);
		btlevel2->setTouchEnabled(false);
		btlevel3->setTouchEnabled(false);
		gaf_level1->playSequence("greyout");
		gaf_level2->playSequence("greyout");
		gaf_level3->playSequence("greyout");
		break;
	case Snail::LEVEL_WALK:
		btlevel1->setVisible(true);
		btlevel1->setTouchEnabled(true);
        _speaker->setTouchEnabled(true);
        root_layout->setTouchEnabled(true);
        bridge->setTouchEnabled(true);
        setTouchCharacter_Item(false);
        if (!_enableShowHandinAllLevel) {
            setTouchCharacter_Item(true);
        }
        scheduleGuidingByHandAndSound();
		//gaf_level1->setVisible(true);
		gaf_level1->playSequence("idle",true);
		break;
	case Snail::LEVEL_SKATE:
		btlevel2->setVisible(true);
		btlevel2->setTouchEnabled(true);
        _speaker->setTouchEnabled(true);
        root_layout->setTouchEnabled(true);
        setTouchCharacter_Item(true);
        bridge->setTouchEnabled(true);
        scheduleGuidingByHandAndSound();
		gaf_level2->playSequence("idle",true);
		break;
	case Snail::LEVEL_JET:
		btlevel3->setVisible(true);
		btlevel3->setTouchEnabled(true);
        _speaker->setTouchEnabled(true);
        root_layout->setTouchEnabled(true);
        setTouchCharacter_Item(true);
        bridge->setTouchEnabled(true);
        scheduleGuidingByHandAndSound();
		//gaf_level3->setVisible(true);
		gaf_level3->playSequence("idle",true);
		break;
	default:
		break;
	}
}

void Snail::onLevelClicked(cocos2d::Ref *sender){

//	if (SNAIL_LEVEL::LEVEL_NONE == level_snail) return;
//    count_touch++;
	setBtnSnailLevel(SNAIL_LEVEL::LEVEL_NONE);
    AUDIO_ENGINE->stopAllSound();
    this->hideGuiding();
    _speaker->setTouchEnabled(false); 
    root_layout->setTouchEnabled(false);
    bridge->setTouchEnabled(false);

	if (is_btlevel_clicked) return;
    is_btlevel_clicked = true;
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);

    if(btrecord->isVisible()){
        btrecord->setTouchEnabled(false);
        btrecord->runAction(FadeTo::create(0.2f, opacity_hide));
    }
    if(btplay->isVisible()){
        btplay->setTouchEnabled(false);
        btplay->runAction(FadeTo::create(0.2f, opacity_hide));
    }

    this->hideTapGuideGame();
    this->unschedule("inActiveMode");

    auto bt = static_cast<Layout *>(sender);

    auto name = bt->getName();
    if (name == std::string("btlevel1")) {
        _sound_type_guiding = SoundTypeGuiding::BEFORE_RECORD;
        CCLOG("clicked level WALK");
        level_snail = LEVEL_WALK;
        gaf_snail->playSequence("go_walk", true);
        audio_action = AUDIO_ENGINE->play("sounds/snail/go_walk.mp3", true);
        index_walk = 0;
        index_character = 0;
        this->onSnailWalk();
    }else if (name == std::string("btlevel2")){
        CCLOG("clicked level SKATE");
        level_snail = LEVEL_SKATE;
        AUDIO_ENGINE->play("sounds/snail/fade_in_skate.mp3");
        gaf_snail->playSequence("fade_in_skate", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            //gaf_snail->playSequence("idle_skate", true);
             this->onSnailSkate();
        });
    }else {
        CCLOG("clicked level JET");
        level_snail = LEVEL_JET;
        //AUDIO_ENGINE->play("sounds/snail/fade_in_jet.mp3");
        //gaf_snail->playSequence("fade_in_jet", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
//            gaf_snail->playSequence("idle_jet", true);
//            audio_action = AUDIO_ENGINE->play("sounds/snail/idle_jet.mp3");
            this->onSnailJet();
        //});
    }

//    if (index_item > LEVEL_JET) {
//        index_item = LEVEL_JET;
//    }
}

void Snail::onSnailWalk(){
    _enable_runTextHideLight = false;
    if (index_walk < (int)list_character.size()) {
        auto pos_goal = bridge->convertToNodeSpace(character_layout->convertToWorldSpace(list_character[index_walk]->getPosition()));
        pos_goal.y = this->getPosYGafSnail(pos_goal.x);
        auto duration = speed_walk*1.0f/1024*pos_goal.distance(snail_layout->getPosition());
        snail_layout->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
            this->runTextHighlight();
            index_walk++;
            this->onSnailWalk();
        }), NULL));
        if (!is_rotate_snail){
            if (pos_goal.x > 1050) {
                is_rotate_snail = true;
                snail_layout->runAction(RotateTo::create(duration, 7));
//                count_walk++;
            }
        }
    }else{
//        btrecord->setTouchEnabled(true);

        index_walk -= 1;
        auto pos_goal = bridge->convertToNodeSpace(character_layout->convertToWorldSpace(list_character[index_walk]->getPosition()));
        pos_goal.x += list_character[index_walk]->getContentSize().width;
        pos_goal.y = this->getPosYGafSnail(pos_goal.x);
        auto duration = speed_walk*1.0f/1024*pos_goal.distance(snail_layout->getPosition());
        snail_layout->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
            this->onSnailHide();
        }), NULL));

        if (!is_rotate_snail){
            if (pos_goal.x > 1050) {
                is_rotate_snail = true;
                snail_layout->runAction(RotateTo::create(duration, 7));
            }
        }
    }

}

void Snail::onSnailHide(){
    if (level_snail == LEVEL_WALK) {
        AUDIO_ENGINE->stopEffect(audio_action);
        gaf_snail->playSequence("rolling");
        auto pos_goal = Vec2(2000, 198);
        auto duration = 1.5f/1024*pos_goal.distance(snail_layout->getPosition());
        snail_layout->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
            level_snail = LEVEL_SKATE;
            //gaf_level1->playSequence("normal");
            _enable_runTextHideLight = true;
            this->showSnail();
            if(count_skate == 0){
                if (index_level <= LEVEL_SKATE) {
                    index_level = LEVEL_SKATE;
                }

            }
//            btlevel1->setTouchEnabled(true);
            btrecord->runAction(FadeIn::create(0.1));
            if(count_walk != 0){
                enableAllBtns();
            }
            count_walk++;

        }), NULL));
    }
}

void Snail::onSnailSkate(){

    disableAllBtns();
    _enable_runTextHideLight = false;
    gaf_snail->playSequence("jump_skate", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        gaf_snail->playSequence("go skate1", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            gaf_snail->playSequence("go skate2", true);
            audio_action = AUDIO_ENGINE->play("sounds/snail/go_skate.mp3");

            auto pos0 = snail_layout->getPosition() + Vec2(100, 0);
            pos0.y = this->getPosYGafSnail(pos0.x) + 5;
            auto pos1 = Vec2(bridge->getContentSize().width/2, 325);
            auto pos2 = Vec2(1100, 315);
            auto pos3 = Vec2(2000, 185);

            auto duration0 = speed_skate*1.0f/1024*pos0.distance(snail_layout->getPosition());
            auto duration1 = speed_skate*1.0f/1024*pos1.distance(pos0);
            auto duration2 = speed_skate*1.0f/1024*pos2.distance(pos1);
            auto duration3 = speed_skate*0.5f/1024*pos3.distance(pos2);

            auto ac0 = MoveTo::create(duration0, pos0);
            auto ac1 = MoveTo::create(duration1, pos1);
            auto ac2 = Spawn::create(MoveTo::create(duration2, pos2), RotateTo::create(duration3*0.5, 10), NULL);
            auto ac3 = MoveTo::create(duration3, pos3);
            snail_layout->runAction(Sequence::create(ac0, CallFunc::create([=]{
                //đọc ở đây.

            }), ac1, ac2, ac3, CallFunc::create([=]{
                AUDIO_ENGINE->stopEffect(audio_action);
                level_snail = LEVEL_JET;
                if(count_jet == 0){
                    index_level = LEVEL_JET;

                }

               /* gaf_level2->playSequence("normal", false ,[=](gaf::GAFObject *obj, string path){

                });*/
                _enable_runTextHideLight = true;
                this->showSnail();
//                btlevel1->setTouchEnabled(true);
//                btlevel2->setTouchEnabled(true);

                btrecord->setTouchEnabled(true);
                btrecord->runAction(FadeIn::create(0.1));

            }), NULL));

            auto audio_id = AUDIO_ENGINE->play(current_item.audio_path_s);
            this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=]{
                auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
                for (int i = 0; i < (int) list_character.size(); i++) {
                    list_character[i]->playText(duration);
                }
            }), DelayTime::create(0.05f), CallFunc::create([=]{
                count_skate++;
                if(count_skate != 0){
//                    enableAllBtns();
//                    btlevel1->setTouchEnabled(true);
//                    btlevel2->setTouchEnabled(true);
//                    btlevel3->setTouchEnabled(true);
                }
            }), NULL));


        });
    });
}
void Snail::onSnailJet(){
    _enable_runTextHideLight = false;
    AUDIO_ENGINE->stopEffect(audio_action);
    gaf_snail->playSequence("go_jet");
    audio_action = AUDIO_ENGINE->play("sounds/snail/go_jet.mp3");
    auto pos0 = snail_layout->getPosition() + Vec2(50, 0);
    pos0.y = this->getPosYGafSnail(pos0.x) + 5;
    auto pos1 = Vec2(bridge->getContentSize().width/2, 325);
    auto pos2 = Vec2(1100, 315);
    auto pos3 = Vec2(2000, 185);

    auto duration0 = speed_jet*1.0f/1024*pos0.distance(snail_layout->getPosition());
    auto duration1 = speed_jet*1.0f/1024*pos1.distance(pos0);
    auto duration2 = speed_jet*1.0f/1024*pos2.distance(pos1);
    auto duration3 = speed_jet*0.5f/1024*pos3.distance(pos2);

    auto ac0 = MoveTo::create(duration0, pos0);
    auto ac1 = MoveTo::create(duration1, pos1);
    auto ac2 = Spawn::create(MoveTo::create(duration2, pos2), RotateTo::create(duration3*0.5, 10), NULL);
    auto ac3 = MoveTo::create(duration3, pos3);
    snail_layout->runAction(Sequence::create(ac0, CallFunc::create([=]{
        //đọc ở đây.
        auto audio_id = AUDIO_ENGINE->play(current_item.audio_path_n);
        this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=]{
            auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
            for (int i = 0; i < (int) list_character.size(); i++) {
                list_character[i]->playText(duration);
            }
        }), NULL));
    }), ac1, ac2, ac3, CallFunc::create([=]{
        //gaf_level3->playSequence("normal");
        AUDIO_ENGINE->stopEffect(audio_action);

        level_snail = LEVEL_NONE;
        index_level = LEVEL_WALK;

        //gaf_level2->playSequence("normal");
        //this->showSnail();

        if(count_jet == 0){
            //btlevel1->runAction(MoveBy::create(0.3f, Vec2(-180, 0)));
            //btlevel2->runAction(MoveBy::create(0.3f, Vec2(-180, 0)));
            //btlevel3->runAction(MoveBy::create(0.3f, Vec2(-180, 0)));
			btlevel1->setVisible(false);
			btlevel2->setVisible(false);
			btlevel3->setVisible(false);
//            count_jet++;

            btrecord->setVisible(true);
            btrecord->setTouchEnabled(false);
            btnext->setVisible(true);
            btprev->setVisible(false);
            btplay->setVisible(true);
            btstop->setVisible(true);

            // guiding for record
            _sound_type_guiding = SoundTypeGuiding::ON_RECORD;
            _speaker->setTouchEnabled(true);
            setTouchCharacter_Item(false);
            _speaker->setTouchEnabled(false);
            if (_enableShowHandinAllLevel) {
                this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([=] {
                    btrecord->setTouchEnabled(false);
                    AUDIO_ENGINE->stopAllSound();
                    if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/Tap the button.mp3")) {
                        AUDIO_ENGINE->playSound("sounds/snail/Tap the button.mp3", false, ([=] {
                            btrecord->setTouchEnabled(true);
                            scheduleGuidingByHandAndSound();
                            _enable_runTextHideLight = true;
                            setTouchCharacter_Item(true);
                            _speaker->setTouchEnabled(true);
                            }));
                    }
                    }), nullptr));
            }
            else {
                btrecord->setTouchEnabled(true);
                scheduleGuidingByHandAndSound();
                _enable_runTextHideLight = true;
                setTouchCharacter_Item(true);
                _speaker->setTouchEnabled(true);
            }
                       
        }
        count_jet++;
        btrecord->runAction(FadeIn::create(0.5f));
//        btplay->setTouchEnabled(true);
        btplay->runAction(FadeTo::create(0.5, opacity_hide));

        if (!is_record_guided && MJDEFAULT->getIntegerForKey(key_num_guided_record) < max_num_guide ){
			AUDIO_ENGINE->playSound("sounds/snail/Tap the button.mp3");
            auto record_layout = cocos2d::utils::findChild(this, "record_layout");
            record_layout->setVisible(true);
            record_layout->runAction(Sequence::create(FadeTo::create(0.25f, 0.9*255), NULL));
            cocos2d::utils::findChild(this, "title_record_guide")->setPosition(record_layout->convertToNodeSpace(root_layout->convertToWorldSpace(btrecord->getPosition() + Vec2(0, -100))));
            this->showTapGuideGame(root_layout->convertToWorldSpace(btrecord->getPosition()), 0.1f);
        }else{
			//this->inActiveMode();
        }

        //btlevel1->setTouchEnabled(true);
        //btlevel2->setTouchEnabled(true);
        //btlevel3->setTouchEnabled(true);

    }), NULL));
}

void Snail::disableAllBtns(){
    btrecord->setTouchEnabled(false);
    btplay->setTouchEnabled(false);
//    btlevel1->setTouchEnabled(false);
//    btlevel2->setTouchEnabled(false);
//    btlevel3->setTouchEnabled(false);
}

void Snail::enableAllBtns(){
    btrecord->setTouchEnabled(true);

    //btlevel1->setTouchEnabled(true);
    //btlevel2->setTouchEnabled(true);
    //btlevel3->setTouchEnabled(true);
}

void Snail::runTextHighlight(){
    list_character[index_character]->playCharacter();
}
void Snail::runTextHighlight(int idx)
{
    if (idx >= list_character.size()) return;
    list_character[idx]->playCharacter();
}
void Snail::runTextSlow(){

}
void Snail::runTextNormal(){

}

void Snail::onCharacterPlayDone(){
    index_character++;
//    if (index_character < (int)list_character.size()) {
//        this->onSnailAction();
//    }else{
//        this->onSnailHide();
//    }
}

void Snail::createButtonLevel(){
    gaf_level1 = GAFWrapper::createGAF("gaf/snail/btn_walk/btn_walk.gaf");
    btlevel1 = cocos2d::utils::findChild<Layout *>(this, "btlevel1");
    btlevel1->addChild(gaf_level1);
    gaf_level1->setPosition(btlevel1->getContentSize()/2);

    gaf_level2 = GAFWrapper::createGAF("gaf/snail/btn_skate/btn_skate.gaf");
    btlevel2 = cocos2d::utils::findChild<Layout *>(this, "btlevel2");
    btlevel2->addChild(gaf_level2);
    gaf_level2->setPosition(btlevel2->getContentSize()/2);

    gaf_level3 = GAFWrapper::createGAF("gaf/snail/btn_jet/btn_jet.gaf");
    btlevel3 = cocos2d::utils::findChild<Layout *>(this, "btlevel3");
    btlevel3->addChild(gaf_level3);
    gaf_level3->setPosition(btlevel3->getContentSize()/2);

    //gaf_level1->playSequence("idle", true);
    //gaf_level2->playSequence("greyout");
    //gaf_level3->playSequence("greyout");
    //
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);
	setBtnSnailLevel(SNAIL_LEVEL::LEVEL_NONE);

	btlevel1->setVisible(false);
	btlevel2->setVisible(false);
	btlevel3->setVisible(false);
}

void Snail::resetButtonLevel(){

	setBtnSnailLevel(LEVEL_NONE);
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);
    //
    //gaf_level1->playSequence("idle", true);
    //gaf_level2->playSequence("greyout");
    //gaf_level3->playSequence("greyout");
}

void Snail::createButtonRecord(){
    btrecord = cocos2d::utils::findChild<Layout *>(this, "btrecord");
    btstop = cocos2d::utils::findChild<Layout *>(this, "btstop");
    btplay = cocos2d::utils::findChild<Layout *>(this, "btplay");
    btnext = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "btnext");
    btprev = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "btprev");

	btplay->setCascadeOpacityEnabled(true);
	btrecord->setCascadeOpacityEnabled(true);

    btrecord->setOpacity(0);
    btstop->setOpacity(0);
    btplay->setOpacity(0);
    btnext->setOpacity(0);
     btprev->setOpacity(0);

    btrecord->setVisible(false);
    btstop->setVisible(false);
    btplay->setVisible(false);
    btnext->setVisible(false);
     btprev->setVisible(false);

    btrecord->setTouchEnabled(false);
    btplay->setTouchEnabled(false);
    btstop->setTouchEnabled(false);
    btnext->setTouchEnabled(false);
    btprev->setTouchEnabled(false);

    btrecord->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
    btstop->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
    btplay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);

    PHONIC_MANAGER->cacheGAF("btrecord", "gaf/snail/pbbuttons/pbbuttons.gaf");
    gaf_record = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["btrecord"]);
    gaf_record->setPosition(btrecord->getContentSize()/2);
    btrecord->addChild(gaf_record);

    gaf_stop = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["btrecord"]);
    gaf_stop->setPosition(btstop->getContentSize()/2);
    btstop->addChild(gaf_stop);

    gaf_play = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["btrecord"]);
    gaf_play->setPosition(btplay->getContentSize()/2);
    btplay->addChild(gaf_play);

    PHONIC_MANAGER->clearCacheGaf();

    gaf_record->playSequence("rec_idle");
    gaf_stop->playSequence("stop_idle");
    gaf_play->playSequence("play_idle");

}

void Snail::createCloud(){
    auto cloud_path = std::string("games/snail/snail_cloud.png");

    auto cloud_1 = cocos2d::ui::ImageView::create(cloud_path);
    this->addChild(cloud_1);
    cloud_1->setPosition(Vec2(-cloud_1->getContentSize().width/2 - 100, 450));
    auto move_width = screen_size.width + cloud_1->getContentSize().width + 100 - cloud_1->getPositionX();
    this->moveCloud(cloud_1, move_width);

    auto cloud_2 = cocos2d::ui::ImageView::create(cloud_path);
    this->addChild(cloud_2);
    cloud_2->setPosition(Vec2(-cloud_2->getContentSize().width/2 - 100, 700));
    move_width = screen_size.width + cloud_2->getContentSize().width + 200 - cloud_2->getPositionX();
    this->moveCloud(cloud_2, move_width);
    cloud_2->setScale(0.6);

    auto cloud_3 = cocos2d::ui::ImageView::create(cloud_path);
    this->addChild(cloud_3);
    cloud_3->setPosition(Vec2(screen_size.width + cloud_3->getContentSize().width + 100, 550));
    move_width = cloud_3->getPositionX() + cloud_3->getContentSize().width;
    this->moveCloud(cloud_3, -move_width);
}

void Snail::moveCloud(cocos2d::ui::ImageView *cloud, float width){
    auto speed = abs(width);
    auto max = 20;
    auto min = 15;
    auto speed1 = random(min, max)*speed*1.0f/1024;
    auto speed2 = random(min, max)*speed*1.0f/1024;
    cloud->runAction(Sequence::create(MoveBy::create(speed1, Vec2(width, 0)), MoveBy::create(speed2, Vec2(-width, 0)), CallFunc::create([=]{
        this->moveCloud(cloud, width);
    }), NULL));
}

float Snail::getPosYGafSnail(float pos_x){
    if (pos_x < 180.0f) {
        return 268.0f;
    }else if (pos_x < 250.0f) {
        return 285.0f;
    }else if (pos_x < 380.0f) {
        return 300.0f;
    }else if (pos_x < 530.0f) {
        return 310.0f;
    }else if (pos_x < 680.0f) {
        return 320.0f;
    }else if (pos_x < 1085.0f) {
        return 325.0f;
    }else if (pos_x < 1185.0f) {
        return 310.0f;
    }else if (pos_x < 1265.0f) {
        return 300.0f;
    }else if (pos_x < 1405.0f) {
        return 290.0f;
    }else {
        return 280.0f;
    }
}

void Snail::onHideGuideRecord(cocos2d::Ref *sender){
	if (!is_record_guided) {
		is_record_guided = true;
		int num_guide = MJDEFAULT->getIntegerForKey(key_num_guided_record);
		MJDEFAULT->setIntegerForKey(key_num_guided_record, num_guide + 1);
	}

    auto record_layout = cocos2d::utils::findChild(this, "record_layout");
	AUDIO_ENGINE->stopSound("sounds/snail/Tap the button.mp3");
    record_layout->runAction(Sequence::create(FadeOut::create(0.15f), CallFunc::create([=]{
        record_layout->setVisible(false);
    }), NULL));
}

void Snail::scheduleGuidingByHandAndSound()
{
    CCLOG("scheduleGuiding");
    this->hideGuiding();
    this->schedule([=](float)
        {       
            if (_enableShowHandinAllLevel) {
                this->removeChildByName("guide_layout");
                if (btnext->getOpacity() > 180 && btnext->isVisible()) {
                    _sound_type_guiding = SoundTypeGuiding::ON_CONTINUE;
                    showGuidingByHand(root_layout->convertToWorldSpace(btnext->getPosition()));
                }
                if (btplay->getOpacity() > 180 && btplay->isVisible()) {
                    if (_enableShowHandGuidingPlay) {
                        showGuidingByHand(root_layout->convertToWorldSpace(btplay->getPosition()));
                    }
                }
                if (btrecord->getOpacity() > 180 && btrecord->isVisible()) {
                    if (_enableShowHandGuidingRecord) {
                        showGuidingByHand(root_layout->convertToWorldSpace(btrecord->getPosition()));
                    }
                }
                auto btlevel = cocos2d::utils::findChild(this, StringUtils::format("btlevel%d", index_level));
                if (btlevel && btlevel->isVisible()) {
                    showGuidingByHand(root_layout->convertToWorldSpace(btlevel->getPosition()));
                }
            }
            
            playSoundGuiding();
        }, 10, 100, 10, "scheduleGuiding");
}

void Snail::playSoundGuiding()
{
    if (_sound_type_guiding == SoundTypeGuiding::ON_RECORD) {
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/guide_red.mp3"))
            AUDIO_ENGINE->playSound("sounds/snail/guide_red.mp3", false, ([=] {
            scheduleGuidingByHandAndSound();
                }));
    }
    else if (_sound_type_guiding == SoundTypeGuiding::AFTER_RECORD) {
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/guide_green.mp3"))
            AUDIO_ENGINE->playSound("sounds/snail/guide_green.mp3", false, ([=] {
            _speaker->setTouchEnabled(true);
            scheduleGuidingByHandAndSound();
                }));
    }
    else if (_sound_type_guiding == SoundTypeGuiding::ON_CONTINUE) {
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/Tap to continue playing!.mp3")) {
            AUDIO_ENGINE->playSound("sounds/snail/Tap to continue playing!.mp3", false, ([=] {
                _speaker->setTouchEnabled(true);
                scheduleGuidingByHandAndSound();
                }));
        }
    }
    else {
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/snail/Tap the picture.mp3"))
            AUDIO_ENGINE->playSound("sounds/snail/Tap the picture.mp3", false, ([=] {
            scheduleGuidingByHandAndSound();
                }));
    }
}

void Snail::showGuidingByHand(Vec2 pos)
{
    auto guide_layout_check = this->getChildByName("guide_layout");
    if (guide_layout_check) return;
    auto guide_layout = cocos2d::ui::Layout::create();
    guide_layout->setContentSize(screen_size);
    this->addChild(guide_layout, INT_MAX - 2);
    guide_layout->setName("guide_layout");

    if (cocos2d::FileUtils::getInstance()->isFileExist("gaf/guidegame/hand2/hand2.gaf")) {
        auto hand = GAFWrapper::createGAF("gaf/guidegame/hand2/hand2.gaf");
        guide_layout->addChild(hand, 1900000000);
        hand->setScale(0.5);
        hand->setPosition(pos);
        hand->play(false, [=](gaf::GAFObject* obj) {
            hand->removeFromParent();
            });
    } 
}

void Snail::hideGuiding()
{
    this->unschedule("scheduleGuiding");
    this->removeChildByName("guide_layout");
    AUDIO_ENGINE->stopAllSound();
}

ui::Widget::ccWidgetClickCallback Snail::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onLevelClicked") {
        return CC_CALLBACK_1(Snail::onLevelClicked, this);
    }
    if (callback_name == "onRecord") {
        return CC_CALLBACK_1(Snail::onRecord, this);
    }
    if (callback_name == "onStop") {
        return CC_CALLBACK_1(Snail::onStop, this);
    }
    if (callback_name == "onPlay") {
        return CC_CALLBACK_1(Snail::onPlay, this);
    }

    if (callback_name == "onNext") {
        return CC_CALLBACK_1(Snail::onNext, this);
    }

    if (callback_name == "onHideGuideRecord") {
        return CC_CALLBACK_1(Snail::onHideGuideRecord, this);
    }

    return nullptr;
}
#pragma mark - RECORD
void Snail::onRecord(cocos2d::Ref *sender){
//    disableLBtns();
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);
	setBtnSnailLevel(LEVEL_NONE);
    _enableShowHandGuidingRecord = false;
    AUDIO_ENGINE->stopAllSound();
    _speaker->setTouchEnabled(false);
    setTouchCharacter_Item(false);
    hideGuiding();
	this->unschedule("inActiveMode");

    AUDIO_ENGINE->pauseEffects(audio_bg_id);
    _is_recording = true;
    startRecord();

	this->unschedule("inActiveMode");
    this->hideTapGuideGame();
    this->onHideGuideRecord(nullptr);
}
void Snail::onStop(cocos2d::Ref *sender){

	this->hideTapGuideGame();
	this->unschedule("inActiveMode");
    scheduleGuidingByHandAndSound();

    _is_recording = false;
    _record->stopRecord();

    gaf_stop->playSequence("stop_idle");
    btstop->runAction(FadeTo::create(0.2f, opacity_hide));
}
void Snail::onPlay(cocos2d::Ref *sender){
//    disableAllBtns();
    //btlevel1->setTouchEnabled(false);
    //btlevel2->setTouchEnabled(false);
    //btlevel3->setTouchEnabled(false);
    AUDIO_ENGINE->stopAllSound();
    _record->stopPlay();
    _speaker->setTouchEnabled(false);
    _enableShowHandGuidingPlay = false;
    setTouchCharacter_Item(false);
    scheduleGuidingByHandAndSound();

	this->hideTapGuideGame();
	this->unschedule("inActiveMode");

	setBtnSnailLevel(LEVEL_NONE);
    btrecord->setTouchEnabled(false);
	this->unschedule("inActiveMode");
	this->hideTapGuideGame();
    btrecord->runAction(FadeTo::create(0.2f, opacity_hide));
    btplay->setTouchEnabled(false);
    btstop->setVisible(false);

    gaf_play->playSequence("play_action", true);


    _is_recording = false;
    _record->startPlay(false);

    btnext->setTouchEnabled(false);
    btnext->stopAllActions();
    btnext->runAction( FadeOut::create(0.2f));
    btnext->setScale(1);

    //btprev->setTouchEnabled(false);
    //btprev->stopAllActions();
    //btprev->runAction( FadeOut::create(0.2f));
    //btprev->setScale(1);

}

void Snail::onNext(cocos2d::Ref *sender){
    _sound_type_guiding = SoundTypeGuiding::BEFORE_RECORD;
    _speaker->setTouchEnabled(true);
    scheduleGuidingByHandAndSound();
    disableAllBtns();

	this->hideTapGuideGame();
	this->unschedule("inActiveMode");

    AUDIO_ENGINE->resumEffects(audio_bg_id);
    is_show_next = false;

    btnext->setTouchEnabled(false);
    btnext->runAction(FadeOut::create(0.2f));

    //btprev->setTouchEnabled(false);
    //btprev->runAction(FadeOut::create(0.2f));

    if(((Layout*)sender)->getName() == "btnext"){
        index_item++;
        _enableShowHandinAllLevel = false;
    }else{
        index_item--;
    }

    if (index_item >= (int) list_item_info.size()) {
        this->onFinishGame();

    //}else if (index_item < 0){
    //    index_item = 0;
    //    enableAllBtns();
    //    btplay->setTouchEnabled(true);
    }else {
        if (index_item < 0) {
            index_item = 0;
            _enableShowHandinAllLevel = false;
        }
        count_walk = 0;
        count_skate = 0;
        count_jet = 0;

        btplay->setTouchEnabled(false);
        btplay->runAction(FadeOut::create(0.2f));

        btrecord->setTouchEnabled(false);
        btrecord->runAction(FadeOut::create(0.2f));

        //btlevel1->runAction(MoveBy::create(0.3, Vec2(180, 0)));
        //btlevel2->runAction(MoveBy::create(0.3, Vec2(180, 0)));
        //btlevel3->runAction(MoveBy::create(0.3, Vec2(180, 0)));

        character_layout->runAction(Sequence::create(ScaleTo::create(0.25f, 1, 0), CallFunc::create([=]{
            this->createCharacter();
            this->resetButtonLevel();

        }), DelayTime::create(0.5f), CallFunc::create([=]{
			level_snail = LEVEL_WALK;
			index_level = LEVEL_WALK;
            this->showSnail();
            //btlevel1->setTouchEnabled(true);
            //btlevel2->setTouchEnabled(true);
            //btlevel3->setTouchEnabled(true);

            btrecord->setVisible(false);
            btplay->setVisible(false);
        }), NULL));
    }
}

void Snail::startRecordForPlastForm()
{
    if(!_record) {
        return;
    }
    
    this->addChild(_record);
    _record->setDelegate(this);
    _record->setName("_record");

//    btlevel1->setTouchEnabled(false);

    btrecord->setTouchEnabled(false);
    btrecord->runAction(FadeOut::create(0.2f));
    btstop->setVisible(true);
    btstop->setOpacity(0);
    btstop->runAction(FadeTo::create(0.2f, opacity_hide));
    btstop->setTouchEnabled(false);
    gaf_stop->playSequence("stop_action", true);
    btstop->runAction(Sequence::create(DelayTime::create(2.0f), FadeIn::create(0.2f), CallFunc::create([=]{
        btstop->setTouchEnabled(true);
    }), NULL));

    btplay->setTouchEnabled(false);
    btplay->runAction(FadeOut::create(0.2f));

    btnext->setTouchEnabled(false);
    btnext->stopAllActions();
    btnext->runAction(FadeOut::create(0.2f));
    btnext->setScale(1);

    //btprev->setTouchEnabled(false);
    //btprev->stopAllActions();
    //btprev->runAction( FadeOut::create(0.2f));
    //btprev->setScale(1);
}

// audio guide : guide_red , guide_green
void Snail::startRecord(){
    if (_record) {
        this->removeChildByName("_record");
        _record = nullptr;
    }
    _record = MSRecord::createRecord(5.0f,"record_output",true);//1.5*_object_info.audio_duration
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    if(MJDEFAULT->getStringForKey("key_turn_record") == "YES")
    {
        startRecordForPlastForm();
    }else{
        mj::helper::hideTap();
        auto screen_size = Director::getInstance()->getVisibleSize();
        micLayout = Layout::create(); // k cho nhan vao mic khi hien popup
        micLayout->setContentSize(Size(400, 400));
        micLayout->setAnchorPoint(Vec2(0.5, 0.5));
        micLayout->setPosition(Vec2(screen_size.width / 2, screen_size.height));
        micLayout->setName("mic_layout");
        this->addChild(micLayout, INT_MAX - 1);
        micLayout->setTouchEnabled(true);
        character_layout->setVisible(false);
        AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::RECORD, nullptr, nullptr, this, true, nullptr, [=] {
            if (micLayout) {
                micLayout->removeFromParent();
            }
            character_layout->setVisible(true);
        });
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    startRecordForPlastForm();
#endif

}

void Snail::onFinishRecord(int finish_type,std::string url) {
    setTouchCharacter_Item(true);
    scheduleGuidingByHandAndSound();
    if (finish_type == RECORD_DONE){
        _sound_type_guiding = SoundTypeGuiding::AFTER_RECORD;
        _speaker->setTouchEnabled(true);
        btplay->setOpacity(0);
        btplay->setVisible(true);
        btplay->setTouchEnabled(true);
        btplay->runAction(FadeIn::create(0.2f));
        btstop->setTouchEnabled(false);
        btstop->runAction(FadeOut::create(0.2f));
        btrecord->setVisible(true);
        btrecord->setOpacity(0);
        btrecord->runAction(FadeIn::create(0.2f));
        btrecord->setTouchEnabled(true);

        if (is_show_next) {

            btnext->setTouchEnabled(true);
            btnext->runAction(FadeIn::create(0.2f));
            btnext->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5, 1.1), ScaleTo::create(0.5, 0.9))));

            //btprev->setTouchEnabled(true);
            //btprev->runAction(FadeIn::create(0.2f));
            //btprev->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5, 1.1), ScaleTo::create(0.5, 0.9))));
        }

    }else {//PlayDone
        _sound_type_guiding = SoundTypeGuiding::ON_CONTINUE;
        _speaker->setTouchEnabled(true);
        setTouchCharacter_Item(true);
        is_show_next = true;
        btrecord->setTouchEnabled(true);
        btrecord->runAction(FadeIn::create(0.2f));
		// hide play button
        btplay->setTouchEnabled(true);
		btplay->setVisible(FadeOut::create(0.2f));
        gaf_play->playSequence("play_idle");

        btnext->setTouchEnabled(true);
        btnext->runAction(FadeIn::create(0.2f));
        btnext->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5, 1.1), ScaleTo::create(0.5, 0.9))));

        //btprev->setTouchEnabled(true);
        //btprev->runAction(FadeIn::create(0.2f));
        //btprev->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5, 1.1), ScaleTo::create(0.5, 0.9))));
    }
    enableAllBtns();
	//inActiveMode();
}

void Snail::runLayoutRecord(cocos2d::ui::Layout *layout, bool isShow){
    layout->stopAllActions();
    if (isShow) {
        layout->setTouchEnabled(true);
        layout->runAction(FadeIn::create(0.2));
    }else{
        layout->setTouchEnabled(false);
        layout->runAction(FadeTo::create(0.2f, opacity_hide));
    }
}
