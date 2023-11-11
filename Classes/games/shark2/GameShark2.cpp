
#include "GameShark2.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "StoryConfigManager.h"
INITIALIZE_READER(MJGameShark2);

USING_NS_CC;
std::once_flag shark2_reader;
#define CSB_NAME "csb/game/shark2/GameShark2.csb"
#define audioWrong "sounds/shark2/sw_wrong.mp3"

MJGameShark2* MJGameShark2::_mjShark2;
MJGameShark2* MJGameShark2::getInstance()
{
    return _mjShark2;
}

MJGameShark2 * MJGameShark2::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(shark2_reader, [] {
        REGISTER_CSB_READER(MJGameShark2);
        REGISTER_CSB_READER(MJGameShark2_Fish);
    });
    auto p = static_cast<MJGameShark2 *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
		_mjShark2 = p;
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark2::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_shark.plist");
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
    
}

void MJGameShark2::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
    auto list_object = value_data["content_info"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameShark2_Fish::FishInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();
        game_dataset.list_objects.push_back(object_info);
    }
}

void MJGameShark2::onEnter(){
    ms::BaseActivity::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/shark2/shark_bgsound.mp3");
    }
    setInputConfigGame();
    loadFrames();
   
}
void MJGameShark2::loadFrames() {
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker, INT_MAX);
	
    auto screen_size = Director::getInstance()->getWinSize();
	auto bg = cocos2d::utils::findChild(this, "bg");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width) {
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	}

	int sizeListObject = game_dataset.list_objects.size();

	float space = -10;

	this->runAction(Sequence::create(DelayTime::create(4.5f), CallFunc::create([=] {
		auto start_x = (screen_size.width) / (game_dataset.list_objects.size() * 5 * 2);
		float posx = screen_size.width - 60 * sizeListObject * 5 - space * (5 * sizeListObject - 1);

		posx = posx / 2;
		auto temp = game_dataset.list_texts;
		for (int i = 0; i < game_dataset.list_objects.size() * 5; i++) {
			Layout* star_holder = Layout::create();
			star_holder->setContentSize(Size(60, 60));
			star_holder->setPosition(Vec2(posx + 30, screen_size.height - 50));

			star_holder->setPositionX(posx + 30);

			posx = posx + space + 60;

			this->addChild(star_holder, 100000000);
			// cac ngoi sao se duoc them tu giua man hinh

			ImageView* star = ImageView::create("games/shark2/6.png");
			star->ignoreContentAdaptWithSize(false);
			star_holder->addChild(star);
			star->setContentSize(star_holder->getContentSize() / 2);
			star->setName("star_shadow");

			ImageView* star_light = ImageView::create("games/shark2/7.png");
			star_light->ignoreContentAdaptWithSize(false);
			star_holder->addChild(star_light);
			star_light->setContentSize(star_holder->getContentSize() / 2);
			star_light->setOpacity(0);
			star_light->setName("starlight");

			stars.push_back(star_holder);
		}
		}), NULL));


	auto posPanel = cocos2d::utils::findChild(this, "NodePos");
	for (int i = 1; i <= 5; i++){
		std::vector<Vec2> poss;
		for (int j = 1; j <= 2; j++){
			auto nodePos = posPanel->getChildByName(StringUtils::format("Panel%d%d", i, j));
			poss.push_back(nodePos->getPosition());
		}
		listPosMove_.push_back(poss);
    }
	for (int i = 0; i < 5; i++){
		time_call_fishs.push_back((float)random(20, 30) / 10.0f);
	}

	auto object_panel = cocos2d::utils::findChild(this, "objectPanel");
	for (int j = 0; j < 5; j++) {
        auto text = game_dataset.list_texts[random(0, (int)game_dataset.list_objects.size() - 1)];
        auto fish = MJGameShark2_Fish::createFish(text.first);
        if (CONFIG_MANAGER->isGameVMonkey()){
            fish->audio_word = text.second;
        }
		object_panel->addChild(fish, 2);
		fish->setDelegate(this);
		fish->setVisible(false);
		fish->setPoss(listPosMove_.at(j));
		list_fishs.pushBack(fish);
	}

    auto shark = MJGameShark2_Fish::createShark();
    shark->setPosition(Vec2(random(0, 1)?-500:(screen_size.width +500), screen_size.height/2));
    this->addChild(shark, 100);
    shark->setDelegate(this);
	sharks.push_back(shark);
    
    if (CONFIG_MANAGER->isGameVMonkey()){
        auto gameNamePanel = cocos2d::utils::findChild(this, "gameNamePanel");
        gameNamePanel->setVisible(false);
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(CONFIG_MANAGER->isGameVMonkey()?"sounds/shark2/gameName_vi.mp3":"sounds/shark2/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            onStartGame();
            for (int i = 1; i <= 3; i++){
                auto imgBg = cocos2d::utils::findChild<ImageView*>(this, StringUtils::format("Image_%d", i));
                imgBg->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), FadeIn::create(1.0f)));
            }
        });
    }else{
        auto gameNamePanel = cocos2d::utils::findChild(this, "gameNamePanel");
        
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(CONFIG_MANAGER->isGameVMonkey()?"sounds/shark2/gameName_vi.mp3":"sounds/shark2/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            gameNamePanel->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(1.0f)
                , CallFunc::create([=](){
                onStartGame();
            }), RemoveSelf::create(true), nullptr));
            for (int i = 1; i <= 3; i++){
                auto imgBg = cocos2d::utils::findChild<ImageView*>(this, StringUtils::format("Image_%d", i));
                imgBg->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), FadeIn::create(1.0f)));
				if (i == 2) {
					imgBg->setZOrder(INT_MAX - 1);
				}
            }
        });
    }
        
	this->schedule(schedule_selector(MJGameShark2::updateCallFish));
}

void MJGameShark2::setInputConfigGame(){
   // auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	auto temp = game_dataset.list_texts;
	game_dataset.list_texts.clear();
	for (int i = 0; i < game_dataset.list_objects.size(); i++){
        game_dataset.list_texts.push_back({game_dataset.list_objects.at(i).text,game_dataset.list_objects.at(i).audio_path});
	}
	for (int i = 0; i < temp.size(); i++){
		game_dataset.list_texts.push_back(temp.at(i));
	}


	max_turn = min(max_turn, (int)game_dataset.list_objects.size());
	if (max_turn <= 3)
		max_item = 5;
}

void MJGameShark2::updateCallFish(float dt)
{
	if (!isPlay)
		return;
	for (int i = 0; i < (int)list_fishs.size(); i++){
		if (!list_fishs.at(i)->in_anim){
			list_fishs.at(i)->freeTime += dt;
			if (list_fishs.at(i)->freeTime >= time_call_fishs.at(i)){
				list_fishs.at(i)->freeTime = 0;
				list_fishs.at(i)->run();
			}
		}
	}
}

void MJGameShark2::onStartGame(){
	onLoadLevel();
	auto start_fish = MJGameShark2_Fish::createFish(game_dataset.list_objects[object_index].text);	
	start_fish->resetText(game_dataset.list_objects[object_index].text);
	this->addChild(start_fish, 10000000);
	Layout* startFish = Layout::create();
	startFish->setContentSize(Size(100, 100));
	start_fish->addChild(startFish, 100000000000);
	startFish->setTouchEnabled(true);
	startFish->addClickEventListener([=](cocos2d::Ref* sender) {
		startFish->setTouchEnabled(false);
		onFish(start_fish);
	});
	start_fish->runInStart();
    auto audio_path = std::string("sounds/shark2/shark_mc1.mp3");
    if (CONFIG_MANAGER->isGameVMonkey()){
        if (game_dataset.hoc_am_hoc_van == "hoc_van"){
            audio_path = std::string("sounds/shark2/shark_mc1_vi_van.mp3");
        }else{
            audio_path = std::string("sounds/shark2/shark_mc1_vi_am.mp3");
        }
        
    }
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        inActiveMode();
		isPlay = true;
		auto audio_id_text = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
		cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_text, [=](int au_id, std::string path){
			start_fish->runOutStart();
			_speaker->setTouchEnabled(true);
			_speaker->addClickEventListener([=](cocos2d::Ref* sender) {
				radio();
				});
		});
    });
}

void MJGameShark2::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameShark2_Fish::FishInfo object_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		object_info.img_path = img_nature->path;

		game_dataset.list_objects.push_back(object_info);
		
	}
    game_dataset.hoc_am_hoc_van = "";
    if (value_data.HasMember("option_vm_phonic") && value_data.FindMember("option_vm_phonic")->value.GetString()){
        game_dataset.hoc_am_hoc_van = value_data.FindMember("option_vm_phonic")->value.GetString();
    }
	if (value_data.HasMember("distracting")){
		for (int i = 0; i < value_data["distracting"].Size(); i++){
            game_dataset.list_texts.push_back({value_data["distracting"][i].GetString(),""});
		}
		
	}
}

void MJGameShark2::onFinishGame(){
	_speaker->setTouchEnabled(false);
	this->runAction(Sequence::create(DelayTime::create(1.25), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        if (CONFIG_MANAGER->isGameVMonkey()){
            onCloseGameVmonkey();
        }else{
            commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/shark2/shark_endgame_gaf/shark_endgame_gaf.gaf", false);
        }
	}), NULL));
}

void MJGameShark2::onFish(cocos2d::Ref *sender){
	//_speaker->setTouchEnabled(true);
    if (!isPlay) return;
    auto fish = dynamic_cast<MJGameShark2_Fish *>(sender);
	if (fish && fish->text_in_fish == game_dataset.list_objects[object_index].text) {
        fish->stopAllActions();
		AUDIO_ENGINE->playSound(game_dataset.list_objects[object_index].audio_path);
        AUDIO_ENGINE->playEffect("sounds/shark2/shark_correct.mp3", false, 1, 1, 0.5);

		CCLOG("fish text = %s", fish->text_in_fish.c_str());
		bool isGotReadyShark = false;
		for (int i = 0; i < sharks.size(); i++){
			if (!sharks.at(i)->in_anim){
				isGotReadyShark = true;
				sharks.at(i)->setLocalZOrder(++zOrderShark);
				sharks.at(i)->sharkRunToGoal(fish, fish->flipx);
				stopInActiveMode();
				inActiveMode();
				break;
			}
		}
		if (!isGotReadyShark){
			auto shark = MJGameShark2_Fish::createShark();
			shark->setPosition(Vec2(-500, 0));
			this->addChild(shark, ++zOrderShark);
			shark->setDelegate(this);
			sharks.push_back(shark);
			shark->sharkRunToGoal(fish, fish->flipx);
		}
		count_fish_tap++;
		if (count_fish_tap == max_item) {
			_speaker->setTouchEnabled(false);
			object_index++;
			count_fish_tap = 0;
			if (object_index >= max_turn) {
				//onFinishGame();
			}
			isPlay = false;
			finishFish = fish;
			onFinishLevel();
		}
		else{
			if (!game_play_enable){
				auto value_pause = min(3, max_item);
				if (count_fish_tap >= value_pause) {
					this->pauseGame();
					return;
				}
			}
		}
    }else{
        AUDIO_ENGINE->stopSound(fish->audio_word);
        AUDIO_ENGINE->playSound(fish->audio_word,false);
		AUDIO_ENGINE->playSound(audioWrong);
        fish->taped = false;
    }
}

void MJGameShark2::onSharkRun(cocos2d::Ref *sender){
	total_count++;
	stars[total_count - 1]->getChildByName("starlight")->runAction(Spawn::create(FadeIn::create(0.5), Sequence::create(ScaleTo::create(0.2, 1.2), ScaleTo::create(0.2, 1), NULL), NULL));
	auto fish = static_cast<MJGameShark2_Fish*>(sender);
    if (fish) {
		//fish->setVisible(false);
		fish->stopAllActions();
		fish->setPosition(Vec2(0,0));
		fish->resetGaf();
		fish->in_anim = false;
		fish->freeTime = -2.0f;
		if (object_index >= max_turn) {
			onFinishGame();
		}
    }
}

void MJGameShark2::resetText(cocos2d::Ref *sender)
{
	if (object_index >= max_turn)
		return;
	auto fish = dynamic_cast<MJGameShark2_Fish *>(sender);
	int rand_num = -1;
	if (list_text.size()>=2){
		if (list_text.at(list_text.size() - 1) != object_index && list_text.at(list_text.size() - 2) != object_index){
			rand_num = object_index;
		}
	}
	if (rand_num == -1){
		if (list_text.size() > 1){
			if (list_text.at(list_text.size()-1)==object_index){
				do 
				{
					rand_num = random(0, (int)game_dataset.list_texts.size() - 1);
				} while (rand_num == object_index);
			}
			else
				rand_num = random(0, (int)game_dataset.list_texts.size() - 1);
		}
		else
		{
			rand_num = random(0, (int)game_dataset.list_texts.size() - 1);
		}
	}
	list_text.push_back(rand_num);
	if (list_text.size() == 6)
		list_text.erase(list_text.begin());
	
    fish->resetText(game_dataset.list_texts[rand_num].first);
    fish->audio_word = game_dataset.list_texts[rand_num].second;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameShark2::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameShark2::onRefresh, this);
    }
    return nullptr;
}

void MJGameShark2::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = MJGameShark2::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
    
}

void MJGameShark2::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void MJGameShark2::onLoadLevel()
{
	for (int i = 0; i < list_fishs.size(); i++){
		resetText(list_fishs.at(i));
		//list_fishs.at(i)->resetText(game_dataset.list_texts.at(random(0, (int)game_dataset.list_texts.size() - 1)));
		list_fishs.at(i)->freeTime -= (float)random(-20, 20) / 10.0f;
	}
}

void MJGameShark2::onFinishLevel()
{
	_speaker->setTouchEnabled(false);
	for (int i = 0; i < list_fishs.size(); i++){
		if (list_fishs.at(i) != finishFish)
			list_fishs.at(i)->returnBase();
	}
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.75f)
		, CallFunc::create([=](){
		if (object_index < max_turn) {
			onStartGame();
		}
	})));
}

void MJGameShark2::inActiveMode(){
    this->schedule([=](float){
        auto audio_path = std::string("sounds/shark2/shark_mc1.mp3");
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (game_dataset.hoc_am_hoc_van == "hoc_van"){
                audio_path = std::string("sounds/shark2/shark_mc1_vi_van.mp3");
            }else{
                audio_path = std::string("sounds/shark2/shark_mc1_vi_am.mp3");
            }
            
        }
		audio_id = AUDIO_ENGINE->playEffect(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            isPlay = true;
			audio_id_text = AUDIO_ENGINE->playEffect(game_dataset.list_objects[object_index].audio_path);
        });
        
    }, 10, 100, 10, "inActive_Mode");
}

void MJGameShark2::stopInActiveMode() {
	this->unschedule("inActive_Mode");
	AUDIO_ENGINE->stopEffect(audio_id);
	AUDIO_ENGINE->stopEffect(audio_id_text);
}

void MJGameShark2::radio() {
	stopInActiveMode();
	inActiveMode();
	_speaker->setTouchEnabled(false);
	auto audio_path = std::string("sounds/shark2/shark_mc1.mp3");
	if (CONFIG_MANAGER->isGameVMonkey()) {
		if (game_dataset.hoc_am_hoc_van == "hoc_van") {
			audio_path = std::string("sounds/shark2/shark_mc1_vi_van.mp3");
		}
		else {
			audio_path = std::string("sounds/shark2/shark_mc1_vi_am.mp3");
		}

	}

	AUDIO_ENGINE->stopSound(audio_path);

	_radio = AUDIO_ENGINE->playSound(audio_path, false, [=] {
		AUDIO_ENGINE->stopSound(game_dataset.list_objects[object_index].audio_path);
		_radio2 = AUDIO_ENGINE->playSound(game_dataset.list_objects[object_index].audio_path, false, [=] {
			_speaker->setTouchEnabled(true);
			});
	});
}

void MJGameShark2::stopRadio() {
	AUDIO_ENGINE->stopEffect(_radio);
	AUDIO_ENGINE->stopEffect(_radio2);
}
