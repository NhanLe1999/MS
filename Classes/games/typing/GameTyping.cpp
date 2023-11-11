
#include "GameTyping.h"
#include "HSAudioEngine.h"
#include "GamePhonics_Monkey.h"
#include "GamePhonics_TextVideo.h"
#include "StoryConfigManager.h"
#include "JsonDump.h"
INITIALIZE_READER(MJGameTyping);

USING_NS_CC;
std::once_flag typing_reader;
#define CSB_NAME "csb/game/typing/GameTyping.csb"
#define interval_time 8
MJGameTyping * MJGameTyping::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(typing_reader, [] {
        REGISTER_CSB_READER(MJGameTyping);
        REGISTER_CSB_READER(MJGameTyping_ItemAnswer);
    });
    auto p = static_cast<MJGameTyping *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameTyping::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_typing.plist");
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
    
	//generateData(value_data.GetObject());
	readConfig(value_data.GetObject(),images);
    
    background_img = this->getChildByName<cocos2d::ui::ImageView *>("bg");
}

void MJGameTyping::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
    auto content_info = value_data["content_info"].GetObject();

    auto list_object = content_info["list_items"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameTyping_ItemAnswer::ItemAnswerInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        if (object_value.HasMember("gaf_path")) {
            object_info.gaf_path = object_value["gaf_path"].GetString();
        }
        
        if (object_value.HasMember("img_path")) {
            object_info.img_path = object_value["img_path"].GetString();
        }
        
        game_dataset.list_objects.push_back(object_info);
    }
    
    if (content_info.HasMember("list_distracting")) {
        auto list_distracting = content_info["list_distracting"].GetArray();
        for (int i = 0; i < (int) list_distracting.Size(); i++) {
            MJGameTyping_ItemAnswer::ItemDistracting item_distracting;
            auto distracting_info = list_distracting[i].GetObject();
            item_distracting.text = distracting_info["text"].GetString();
            game_dataset.list_distracting.push_back(item_distracting);
        }
    }
    if (content_info.HasMember("option_vm_phonic") && content_info.FindMember("option_vm_phonic")->value.GetString()){
        game_dataset.hoc_am_hoc_van = content_info.FindMember("option_vm_phonic")->value.GetString();
    }
}

void MJGameTyping::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();

	auto imgGameName = cocos2d::ui::ImageView::create("games/typing/logo.png");
	this->addChild(imgGameName, 10001);
	
	imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
    if (CONFIG_MANAGER->isGameVMonkey()){
        imgGameName->setOpacity(0);
        onStartGame();
    }else{
        auto blackLayout = LayerColor::create(Color4B(0, 0, 0, 200));
        this->addChild(blackLayout, 10000);
        auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/typing/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            imgGameName->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f)
                , CallFunc::create([=](){
                onStartGame();
            }), RemoveSelf::create(true), nullptr));
            blackLayout->runAction(Sequence::createWithTwoActions(DelayTime::create(0.55f), RemoveSelf::create(true)));
        });
    }

  //  onStartGame();
    //git git git
}
void MJGameTyping::loadFrames() {
    auto screensize = Director::getInstance()->getWinSize();
    
    gaf_monkey_typing = GAFWrapper::createGAF("gaf/typing/monkey_typing/monkey_typing.gaf");
    background_img->addChild(gaf_monkey_typing);
    gaf_monkey_typing->setPosition(Vec2(screensize.width/2 - 132, 280));
    playMonkeyTypingWithSequenceType(IDLE, true);
//    background_img->setPositionX(screensize.width - background_img->getContentSize().width);
//    background_img->runAction(Sequence::create(MoveTo::create(4.0f, Vec2(0, background_img->getPositionY())), DelayTime::create(0.5f), CallFunc::create([=]{
//        
//    }), NULL));
    
    std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/monkeystories_ultra.ttf";
    text_object = cocos2d::ui::Text::create("", "fonts/monkeystories_ultra.ttf", 100);
    background_img->addChild(text_object, 10000);
    text_object->setVisible(false);
    text_object->setTextColor(Color4B::BLACK);
    text_object->enableOutline(Color4B::WHITE,2);

    text_object->addClickEventListener(CC_CALLBACK_1(MJGameTyping::onClickText, this));
    
    gaf_bird = GAFWrapper::createGAF("gaf/typing/cucko_typing/cucko_typing.gaf");
    auto clock = cocos2d::utils::findChild(this, "clock");
    clock->addChild(gaf_bird);
    gaf_bird->setPosition(Vec2(8, 580));
    gaf_bird->playSequence("close");

    clock->setPositionX(screensize.width/2 +349);
//    gaf_bird->setFlippedX(true);

    _speaker = mj::helper::addSpeaker(this);
    _speaker->addClickEventListener(CC_CALLBACK_1(MJGameTyping::onSpeakerClicked, this));
    _speaker->setTouchEnabled(false);

}

void MJGameTyping::setInputConfigGame(){
    mj::helper::shuffle(game_dataset.list_objects);
    max_turn = (int)game_dataset.list_objects.size();
    max_item_typing = 3;
    max_read_word = 5;
    max_keyboard_word = 3;
    if (max_turn >= 4) {
        max_read_word = 1;
        max_read_word = 3;
    }else if (max_turn > 1){
        max_read_word = 2;
    }

    if (number_keyboard_server > 0){
        max_keyboard_word = number_keyboard_server;//số lượt hiện trên đầu khỉ khỉ
    }
}

void MJGameTyping::onStartGame(){
    list_pos = {Vec2(-250, 250), Vec2(0, 350), Vec2(250, 250)};
//    mj::helper::shuffle(list_pos);
    text_object->setString(game_dataset.list_objects[object_index].text);
    
    if (gaf_word_object) {
        gaf_word_object->removeFromParent();
        gaf_word_object = nullptr;
    }
    
    if (game_dataset.list_objects[object_index].gaf_path != "") {
        gaf_word_object = GAFWrapper::createGAF(game_dataset.list_objects[object_index].gaf_path);
        background_img->addChild(gaf_word_object, 10000);
        gaf_word_object->setVisible(false);
    }
    std::string audio_path;
    if (object_index == 0) {
        playMonkeyTypingWithSequenceType(TALK, true);
        audio_path = std::string("sounds/typing/typing_mc1.mp3");
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                audio_path = std::string("sounds/typing/typing_mc1_vi_am.mp3");
            }else{
                audio_path = std::string("sounds/typing/typing_mc1_vi_van.mp3");
            }
        }
        
        
    }else {
        playMonkeyTypingWithSequenceType(TALK, true);
        audio_path = std::string("sounds/typing/typing_mc4.mp3");
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                audio_path = std::string("sounds/typing/typing_mc4_vi_am.mp3");
            }else{
                audio_path = std::string("sounds/typing/typing_mc4_vi_van.mp3");
            }
        }
    }
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        playMonkeyTypingWithSequenceType(TYPING);
        displayObject();
    });
}

void MJGameTyping::displayObject(){
    _speaker->setTouchEnabled(true);
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
        text_object->setTextColor(Color4B::BLACK);
        text_object->setVisible(true);
        text_object->setScale(0);
        text_object->setPosition(gaf_monkey_typing->getPosition());
		

        auto rand = random(0, 2);
        while (rand == old_rand_pos) {
            rand = random(0, 2);
        }
        old_rand_pos = rand;
        auto pos_goal = list_pos[old_rand_pos];
        auto pos_check = background_img->convertToWorldSpace(text_object->getPosition() + pos_goal);
        
        if (old_rand_pos == 0 && pos_check.x - text_object->getContentSize().width/2 < 0) {
            CCLOG("hut = %f", pos_check.x - text_object->getContentSize().width/2);
            pos_goal.x = pos_goal.x - (pos_check.x - text_object->getContentSize().width/2) + 10;
            pos_goal.y += 60;
        }
        
        
        text_object->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5f, 1.0f), EaseBackOut::create(MoveBy::create(0.5f, pos_goal)), NULL), CallFunc::create([=]{
			//this->inActiveMode(text_object->getParent()->convertToWorldSpace(text_object->getPosition()));
            this->inActiveMode();
			if (count_readword > 0) {
                text_object->setTouchEnabled(true);
            }else{
                playMonkeyTypingWithSequenceType(TALK, true);
                auto audio_path = std::string("sounds/typing/typing_mc2.mp3");
                if (CONFIG_MANAGER->isGameVMonkey()){
                    if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                        audio_path = std::string("sounds/typing/typing_mc2_vi_am.mp3");
                    }else{
                        audio_path = std::string("sounds/typing/typing_mc2_vi_van.mp3");
                    }
                }
                auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    playMonkeyTypingWithSequenceType(IDLE, true);
                    auto audio_id2 = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id2, [=](int au_id, std::string path){
                        text_object->setTouchEnabled(true);
                        if (count_tap_guide <= 1) {
                            count_tap_guide++;
                            //mj::helper::showTap(text_object->getParent()->convertToWorldSpace(text_object->getPosition()));
							
                        }
                    });
                });
            }
        }),NULL));
    }), NULL));
}

//void MJGameTyping::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(this->getContentSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(this->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(this->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void MJGameTyping::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	auto list_object = value_data["list_item"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameTyping_ItemAnswer::ItemAnswerInfo object_info;

		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());


		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		//if (object_value.HasMember("gaf_path")) {
		object_info.gaf_path = "";
		//}

		//if (object_value.HasMember("img_path")) {
		object_info.img_path = "";// object_value["img_path"].GetString();
		//}

		game_dataset.list_objects.push_back(object_info);
	}

	if (value_data.HasMember("list_distracting")) {
		auto list_distracting = value_data["list_distracting"].GetArray();
		for (int i = 0; i < (int)list_distracting.Size(); i++) {
			MJGameTyping_ItemAnswer::ItemDistracting item_distracting;
			item_distracting.text = list_distracting[i].GetString();
			game_dataset.list_distracting.push_back(item_distracting);
		}
	}
    if (CONFIG_MANAGER->isVietnameseStories() && value_data.HasMember("list_distracting_vm")) {
        game_dataset.list_distracting.clear();
        auto list_distracting = value_data["list_distracting_vm"].GetArray();
        for (int i = 0; i < (int)list_distracting.Size(); i++) {
            auto img_distracing = math::resource::ResHelper::getImage(images, list_distracting[i].GetString());
            MJGameTyping_ItemAnswer::ItemDistracting item_distracting;
            item_distracting.text = img_distracing->getText("name_1");
            item_distracting.audio_path = img_distracing->getAudios("name_1").front();
            game_dataset.list_distracting.push_back(item_distracting);
        }
    }
    
    if (value_data.HasMember("number_read_word")) {
        number_keyboard_server = value_data["number_read_word"].GetInt();
    }
    if (value_data.HasMember("option_vm_phonic") && value_data.FindMember("option_vm_phonic")->value.GetString()){
        game_dataset.hoc_am_hoc_van = value_data.FindMember("option_vm_phonic")->value.GetString();
    }
    
}

void MJGameTyping::onFinishGame(){
   
	gaf_monkey_typing->setVisible(false);
		this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
			//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
            if (CONFIG_MANAGER->isVietnameseStories()) {
                this->onCloseGameVmonkey();
            }else{
                commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
            }
		}), NULL));
}


void MJGameTyping::playMonkeyTypingWithSequenceType(int type_sequence, bool isloop) {
	typing_sequence_type = type_sequence;
	if (typing_sequence_type == TYPING) {
		AUDIO_ENGINE->playEffect("sounds/typing/typing_typing.mp3");
	}
	gaf_monkey_typing->playSequence(getGafSequenceName(type_sequence), isloop, [=](gaf::GAFObject *obj, std::string name_sequence) {
		onFinishMokeyTypingSequenceType(typing_sequence_type);
	});
}
void MJGameTyping::onFinishMokeyTypingSequenceType(int type_sequence) {
	if (type_sequence == TYPING) {
		if (count_readword > 0) {
			playMonkeyTypingWithSequenceType(IDLE, true);
		}

		return;
	}
}

void MJGameTyping::onClickText(cocos2d::Ref *sender) {
	this->unschedule("inactive_mode");
    inActiveMode();
	mj::helper::hideTap();
	cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
	count_readword++;
	text_object->setTouchEnabled(false);
   // text_object->setColor(Color3B::RED);
    text_object->setTextColor(Color4B::RED);
	text_object->runAction(Sequence::create(ScaleTo::create(0.25f, 1.5f), ScaleTo::create(0.15f, 0), DelayTime::create(0.2f), CallFunc::create([=] {
		text_object->setVisible(false);
		if (gaf_word_object) {
			gaf_word_object->setPosition(text_object->getPosition());
			gaf_word_object->setVisible(true);
			gaf_word_object->play(false, [=](gaf::GAFObject *obj) {
				gaf_word_object->setVisible(false);
				playObject();
			});
		}
		else {
			playObject();
		}

	}), NULL));
}

void MJGameTyping::playObject() {
	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		if (!game_play_enable && object_index >= 1) {
			this->pauseGame();
			return;
		}
		if (count_readword >= max_keyboard_word) {
			CCLOG("Sang câu hỏi");
			displayAnswer();
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
				playMonkeyTypingWithSequenceType(TALK, true);
				auto audio_path = std::string("sounds/typing/typing_mc2.mp3");
                if (CONFIG_MANAGER->isGameVMonkey()){
                    if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                        audio_path = std::string("sounds/typing/typing_mc2_vi_am.mp3");
                    }else{
                        audio_path = std::string("sounds/typing/typing_mc2_vi_van.mp3");
                    }
                }
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
					playMonkeyTypingWithSequenceType(IDLE, true);
					auto audio_id2 = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id2, [=](int au_id, std::string path) {

					});
				});
			}), NULL));
		}
		else {
			playMonkeyTypingWithSequenceType(TYPING);
			displayObject();
		}
	}), NULL));
}

void MJGameTyping::displayAnswer() {
	std::vector<MJGameTyping_ItemAnswer::ItemDistracting> list_answer;
	auto list_distracting = game_dataset.list_distracting;
	for (int i = 0; i < (int)game_dataset.list_objects.size(); i++) {
		if (i == object_index) continue;
		MJGameTyping_ItemAnswer::ItemDistracting item_distracting_convert;
		item_distracting_convert.text = game_dataset.list_objects[i].text;
		list_distracting.push_back(item_distracting_convert);
	}
	mj::helper::shuffle(list_distracting);

	for (int i = 0; i < max_item_typing - 1; i++) {
		list_answer.push_back(list_distracting[i]);
	}
	MJGameTyping_ItemAnswer::ItemDistracting item_distracting_convert;
	item_distracting_convert.text = game_dataset.list_objects[object_index].text;
	list_answer.push_back(item_distracting_convert);
	mj::helper::shuffle(list_answer);

	for (int i = 0; i < list_answer.size(); i++) {
		CCLOG("i = %d | %s", i, list_answer[i].text.c_str());
	}

	for (int i = 0; i < 3; i++) {
		auto item_bg = cocos2d::utils::findChild(this, StringUtils::format("item%d", i + 1));
		auto item_answer = MJGameTyping_ItemAnswer::createGame(list_answer[i]);
		item_bg->addChild(item_answer);
		item_answer->setDelegate(this);
		if (item_answer->text->getString() == game_dataset.list_objects[object_index].text){
			//this->inActiveMode(item_answer->getParent()->convertToWorldSpace(item_answer->getPosition())+ item_answer->getContentSize()/2);
            inActiveMode();
		}
    } 
	
}



void MJGameTyping::onClickAnswer(cocos2d::Ref *sender){
    unschedule("inactive_mode");
    inActiveMode();
    auto item = static_cast<MJGameTyping_ItemAnswer *>(sender);
    if (item->text->getString() == game_dataset.list_objects[object_index].text) {
		this->unschedule("active_mode");
        item->onResultAnswer_True();
        playMonkeyTypingWithSequenceType(TALK, true);
        auto audio_path = CONFIG_MANAGER->isGameVMonkey()?StringUtils::format("sounds/typing/typing_mc3%d_vi.mp3", random(1, 4)):StringUtils::format("sounds/typing/typing_mc3%d.mp3", random(1, 4));
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        playMonkeyTypingWithSequenceType(IDLE, true);
        });
        
        AUDIO_ENGINE->playEffect("sounds/typing/typing_correct.mp3");
        gaf_bird->playSequence("open", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            gaf_bird->playSequence("close");
        });
    }else{
        item->onResultAnswer_False();
        AUDIO_ENGINE->playEffect("sounds/typing/typing_wrong.mp3");
        if (CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->playEffect(item->getInfo().audio_path);
    }
}

void MJGameTyping::onDoneAnswer(cocos2d::Ref *sender){
    playMonkeyTypingWithSequenceType(IDLE, true);
    count_answer++;
    this->runAction(Sequence::create(DelayTime::create(0.15f), CallFunc::create([=]{
        for (int i = 0; i < 3; i++) {
            auto item_bg = cocos2d::utils::findChild(this, StringUtils::format("item%d", i+1));
            item_bg->removeAllChildren();
        }
    }), DelayTime::create(0.5f), CallFunc::create([=]{
        if (count_answer >= max_read_word) {
            object_index++;
            if (object_index >= max_turn) {
                onFinishGame();
            }else {
                count_readword = 0;
                count_answer = 0;
                onStartGame();
            }
        }else {
            displayAnswer();
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
				playMonkeyTypingWithSequenceType(TALK, true);
				auto audio_path = std::string("sounds/typing/typing_mc2.mp3");
                if (CONFIG_MANAGER->isGameVMonkey()){
                    if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                        audio_path = std::string("sounds/typing/typing_mc2_vi_am.mp3");
                    }else{
                        audio_path = std::string("sounds/typing/typing_mc2_vi_van.mp3");
                    }
                }
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    playMonkeyTypingWithSequenceType(IDLE, true);
					auto audio_id2 = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id2, [=](int au_id, std::string path){
						
					});
				});
			}), NULL));
        }
    }), NULL));
}


std::string MJGameTyping::getGafSequenceName(int type_sequence){
    if (type_sequence == IDLE) {
        return "idle";
    }
    if (type_sequence == TYPING) {
        return "typing";
    }
    if (type_sequence == TALK) {
        return "talk";
    }
    return "idle";
}


#pragma callback
ui::Widget::ccWidgetClickCallback MJGameTyping::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameTyping::onRefresh, this);
    }
    return nullptr;
}

void MJGameTyping::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = MJGameTyping::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
    
}

void MJGameTyping::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}
//void MJGameTyping::showTapGuideGame(Vec2 position, float delay_time, std::string gaf_guide_path) {
//	//    auto guide_layout_check = this->getChildByName("guide_layout");
//	//    if (guide_layout_check) return;
//	//    auto guide_layout = cocos2d::ui::Layout::create();
//	//    guide_layout->setContentSize(screen_size);
//	//    Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
//	//    guide_layout->setName("guide_layout");
//
//	//    guide_layout->schedule([=](float) {
//	auto hand = GAFWrapper::createGAF(gaf_guide_path);
//	Director::getInstance()->getRunningScene()->addChild(hand, INT_MAX - 1);
//	hand->setPosition(position);
//	hand->play(true);
//	hand->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
//		hand->removeFromParent();
//	}), NULL));
//
//	//    }, 5, 100, delay_time, "animated_guide");
//}

void  MJGameTyping::inActiveMode() {
	this->scheduleOnce([=](float) {
		auto audio_path = std::string("sounds/typing/typing_mc2.mp3");
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                audio_path = std::string("sounds/typing/typing_mc2_vi_am.mp3");
            }else{
                audio_path = std::string("sounds/typing/typing_mc2_vi_van.mp3");
            }
        }
		auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
		cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
			playMonkeyTypingWithSequenceType(IDLE, true);
            _currentAudio = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
			cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
                    inActiveMode();
			});
		});
	}, interval_time, "inactive_mode");
}

void MJGameTyping::onSpeakerClicked(cocos2d::Ref* sender) {
    unschedule("inactive_mode");
    cocos2d::experimental::AudioEngine::stop(_currentAudio);
    _speaker->setTouchEnabled(false);
    _currentAudio = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
        _speaker->setTouchEnabled(true);
        inActiveMode();
        });
}