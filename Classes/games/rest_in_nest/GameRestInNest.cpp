
#include "GameRestInNest.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GameRestInNest);
#define speed_opacity 15

std::once_flag GameRestInNest_reader;
#define CSB_NAME "csb/game/rest_in_nest/GameRestInNest.csb"

#define posItem1 0.2
#define posItem2 0.5
#define posItem3 0.8

GameRestInNest * GameRestInNest::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameRestInNest_reader, [] {
        REGISTER_CSB_READER(GameRestInNest);
        REGISTER_CSB_READER(GameRestInNest_Item);
    });
    auto p = static_cast<GameRestInNest *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameRestInNest::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/game_sample.plist");
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

void GameRestInNest::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	
	game_dataset.number_level = value_data["content_info"]["number_level"].GetInt();
	auto list_object = value_data["content_info"]["main_text"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MainTextData data_info;
        auto object_value = list_object[i].GetObject();
		data_info.text = object_value["w"].GetString();
		data_info.audio_path = object_value["audio_path"].GetString();

		//game_dataset.list_texts.push_back(object_value["w"].GetString());
        game_dataset.list_main_texts.push_back(data_info);
    }
	if (value_data["content_info"].HasMember("other_text")){
		auto list_other_text = value_data["content_info"]["other_text"].GetArray();
		for (int i = 0; i < (int)list_other_text.Size(); i++) {
			game_dataset.list_texts.push_back(list_other_text[i].GetString());
		}
	}
    if (value_data["content_info"].HasMember("option_vm_phonic")){
        game_dataset.hoc_am_hoc_van = value_data["content_info"]["option_vm_phonic"].GetString();
    }
}

void GameRestInNest::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();
	auto list_object = value_data["main_text"].GetArray();
	
	if (value_data.HasMember("other_text")){
		auto list_other_text = value_data["other_text"].GetArray();
		for (int i = 0; i < (int)list_other_text.Size(); i++) {
			game_dataset.list_texts.push_back(list_other_text[i].GetString());
		}
	}
	bool is_need_more_text = false;
	if (game_dataset.list_texts.size()==0){
		is_need_more_text = true;
	}
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MainTextData data_info;
		
		auto dataFull = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		data_info.text = dataFull->getText();
		data_info.audio_path = dataFull->getAudio();
		
		if (is_need_more_text)
			game_dataset.list_texts.push_back(dataFull->getText());
		game_dataset.list_main_texts.push_back(data_info);
	}
	
}

void GameRestInNest::onEnter(){
    ms::BaseActivity::onEnter();
    auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
    auto gameName = panelRun->getChildByName<ImageView*>("imgGamename");
    if (CONFIG_MANAGER->isGameVMonkey()) gameName->setVisible(false);
    setInputConfigGame();
    loadFrames();
    setCallbackKienRunAction();
}

void GameRestInNest::setInputConfigGame(){
	//    auto limit = 3;
	mj::helper::shuffle(game_dataset.list_main_texts);
	item_per_level = 5;
	if (game_dataset.number_level == 1)
		item_per_level = 10;

	for (int i = 0; i < game_dataset.number_level; i++){
		for (int  k = 0; k < item_per_level; k++){
			GameRestInNest_Item::ItemInfo iteminfo;
			iteminfo.main_text = game_dataset.list_main_texts.at(i).text;
			iteminfo.audio_path = game_dataset.list_main_texts.at(i).audio_path;
			iteminfo.texts.push_back(game_dataset.list_main_texts.at(i).text);
			while (iteminfo.texts.size() < 3){
				int index = rand() % game_dataset.list_texts.size();
				bool is_contain = false;
				for (int j = 0; j < (int)iteminfo.texts.size(); j++){
					if (iteminfo.texts.at(j).compare(game_dataset.list_texts.at(index)) == 0){
						is_contain = true;
					}
				}
				if (!is_contain)
					iteminfo.texts.push_back(game_dataset.list_texts.at(index));
			}
			item_infos.push_back(iteminfo);
		}
	}
}

void GameRestInNest::updateRestInNest(float delta)
{
	if (game_state_rin == REST_IN_NEST_GAME_STATE::RIN_GS_PRE_GAME){
		auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
		for (int i = 0; i < bgs.size(); i++){
			if (panelRun->convertToWorldSpace(bgs.at(i)->getPosition()).y < -bgs.at(i)->getContentSize().height*0.5f*bgs.at(i)->getScaleX() - 10){
				int index_bg = i - 1;
				if (index_bg==-1){
					index_bg = bgs.size() - 1;
				}
				bgs.at(i)->setPositionY(bgs.at(index_bg)->getPositionY() + bgs.at(i)->getContentSize().height*bgs.at(i)->getScaleX());
				bgs.at(i)->setScaleY(-bgs.at(index_bg)->getScaleY());
			}
		}
		if (panelRun->convertToWorldSpace(bird->getPosition()).y > Director::getInstance()->getVisibleSize().height/2){
			panelRun->setPositionY(panelRun->getPositionY() - (bird->getPositionY() - posBird.y));
		}

	}
	if (game_state_rin == REST_IN_NEST_GAME_STATE::RIN_GS_PLAYING || game_state_rin == REST_IN_NEST_GAME_STATE::RIN_GS_PLAY_VIDEO){
		auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
		if (panelRun->convertToWorldSpace(bird->getPosition()).y > REST_IN_NEST_ITEM_SIZE.y / 2){
			panelRun->setPositionY(panelRun->getPositionY() - (bird->getPositionY() - posBird.y));
		}
	}
	if (game_state_rin == REST_IN_NEST_GAME_STATE::RIN_GS_PRE_FINISH ){
		auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
		if (panelRun->convertToWorldSpace(bird->getPosition()).y > REST_IN_NEST_ITEM_SIZE.y*0.8f){
			panelRun->setPositionY(panelRun->getPositionY() - (bird->getPositionY() - posBird.y));
		}
	}
	posBird = bird->getPosition();
}

void GameRestInNest::clickTrueItem(GameRestInNest_Item* aItem, ImageView* aImg)
{
	game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_PLAY_VIDEO;
    this->unschedule("inActive_mode");
    for (auto c : aItem->getChildren()) {
        if (c == aImg) {
            continue;
        }
        
        auto text = c->getChildByName("text");
        if (!text) {
            continue;
        }
        
        text->runAction(FadeOut::create(0.5));
    }

	int total_level = item_per_level*game_dataset.number_level;
	if (totalTime == total_level - 1){
		game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_PRE_FINISH;
	}
	auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
	bird->playSequence("fly", false, [=](gaf::GAFObject *obj, std::string name_sequence){
		bird->playSequence("idle", true);
	});
	AUDIO_ENGINE->playEffect("sounds/rest_in_nest/jump_fly_land.mp3");
	this->scheduleOnce([=](float dt){
		bird->runAction(EaseInOut::create(MoveTo::create(1.0f, panelRun->convertToNodeSpace(aItem->convertToWorldSpace(aImg->getPosition()))),1));
		bird->runAction(RotateTo::create(0.2f, getAngelObjectPlayer(bird->getPosition(), panelRun->convertToNodeSpace(aItem->convertToWorldSpace(aImg->getPosition())))));
	}, 1.0f, "bay");

	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->playEffect(aItem->_item_info.audio_path);
		bird->runAction(Sequence::createWithTwoActions(RotateTo::create(0.2f, 0), CallFunc::create([=](){  })));
	}, 2.0f, "quay");

	this->scheduleOnce([=](float dt){
		game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_PLAYING;
		curTime++;
		totalTime++;
		if (curTime == item_per_level){
			curText++;
			if (curText == game_dataset.number_level){
				onFinishLevel();
			}
			else{
				showTextPreGame(item_infos.at(totalTime));
				curTime = 0;
			}
		}

		{ // doi cho bg + item 
			auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
			for (int i = 0; i < bgs.size(); i++){
				if (panelRun->convertToWorldSpace(bgs.at(i)->getPosition()).y < -bgs.at(i)->getContentSize().width*0.5f*bgs.at(i)->getScaleX() - 10){
					int index_bg = i - 1;
					if (index_bg == -1){
						index_bg = bgs.size() - 1;
					}
					bgs.at(i)->setPositionY(bgs.at(index_bg)->getPositionY() + bgs.at(i)->getContentSize().height*bgs.at(i)->getScaleX());
					bgs.at(i)->setScaleY(-bgs.at(index_bg)->getScaleY());
				}
			}
			
			for (int i = 0; i < (int)rest_items.size(); i++)	{
				if (panelRun->convertToWorldSpace(rest_items.at(i)->getPosition()).y < -REST_IN_NEST_ITEM_SIZE.y*0.8f){
					CCLOG("doi cho roi ne");
					int index_bg = i - 1;
					if (index_bg == -1){
						index_bg = rest_items.size() - 1;
					}
					rest_items.at(i)->setPositionY(rest_items.at(index_bg)->getPositionY() + REST_IN_NEST_ITEM_SIZE.y);
					
					if (totalTime + 2>total_level){
						rest_items.at(i)->setVisible(false);
                        
                        for (auto c : rest_items.at(i)->getChildren()) {
                            auto text = c->getChildByName("text");
                            if (!text) {
                                continue;
                            }
                            
                            text->setOpacity(0xff);
                        }
					}
					else if (totalTime+2==total_level){
						rest_items.at(i)->setForFinish();
					}
					else{
						rest_items.at(i)->reloadInfo(item_infos.at(totalTime + 2));
					}
				}
			}
		}
        this->inActiveMode();
	}, 2.5f, "set_pos_item");
	
}

float GameRestInNest::getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to)
{
	/*Vec2 pos3;
	pos3.x = pos_objectplayer.x;
	pos3.y = pos_to.y;
	float tan_alpha = pos3.distance(pos_to) / pos3.distance(pos_objectplayer);*/
	float origin_line;
	auto win_size = Director::getInstance()->getVisibleSize();
	if (fabs(pos_objectplayer.x - win_size.width*posItem1) < 20) {
		origin_line = win_size.width*posItem1;
	}
	else if (fabs(pos_objectplayer.x - win_size.width*posItem3) < 20){
		origin_line = win_size.width*posItem3;
	}
	else origin_line = win_size.width*posItem2;

	float tan_alpha = (pos_to.x - origin_line) / (pos_to.y - pos_objectplayer.y);
	float  Radian_Gocquay = atanf(tan_alpha);
	float cocosAngle = Radian_Gocquay * 180 / M_PI;
	CCLOG("cocosAngle = %f", cocosAngle);

	return cocosAngle;
}

void GameRestInNest::showTextPreGame(GameRestInNest_Item::ItemInfo a_info)
{
	auto preGame = cocos2d::utils::findChild<Layout*>(this, "preGame");
	preGame->setOpacity(0);
	preGame->setVisible(true);

	auto imgPreGame = cocos2d::utils::findChild<ImageView*>(this, "imgPreGame");
	imgPreGame->setScale(0.01f);
	Text* textPreGame = cocos2d::utils::findChild<Text*>(this, "textPreGame");
	textPreGame->setString(a_info.main_text);
    if (CONFIG_MANAGER->isGameVMonkey()){
        textPreGame->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
    }
	preGame->runAction(Sequence::create(FadeIn::create(0.3f)
		, CallFunc::create([=](){
		AUDIO_ENGINE->playEffect("sounds/rest_in_nest/show_text.mp3");
		imgPreGame->runAction(Sequence::create(ScaleTo::create(0.3f, 1.0f)
			, CallFunc::create([=](){
			AUDIO_ENGINE->playEffect(a_info.audio_path);
		}),nullptr));
	}), DelayTime::create(2.0f)
		, CallFunc::create([=](){
		preGame->setVisible(false);
        inActiveMode();
	}), nullptr));
}

void GameRestInNest::playKien(GAFWrapper* kien)
{
    if(_callBackKienRunAction)
    {
        _callBackKienRunAction(kien);
    }
}

void GameRestInNest::setCallbackKienRunAction()
{
    _callBackKienRunAction = [=] (GAFWrapper* kien) {
        int randStartItem = rand() % 4;
        int randStartNest = rand() % 3;

        int randEndItem = rand() % 4;
        int randEndNest = rand() % 3;

        auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");

        Vec2 startPos = panelRun->convertToNodeSpace(rest_items.at(randStartItem)->convertToWorldSpace(rest_items.at(randStartItem)->imgs.at(randStartNest)->getPosition()));
        Vec2 endPos = panelRun->convertToNodeSpace(rest_items.at(randEndItem)->convertToWorldSpace(rest_items.at(randEndItem)->imgs.at(randEndNest)->getPosition()));
        kien->setRotation(getAngelObjectPlayer(startPos, endPos));
        kien->setPosition(startPos);
        kien->runAction(Sequence::create(MoveTo::create(rand() % 2 + 2, endPos), CallFunc::create([=](){playKien(kien); }), nullptr));
    };
}

void GameRestInNest::loadFrames() {	
	
	auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
	auto objectPanel = cocos2d::utils::findChild(this, "objectPanel");

	for (int i = 0; i < 3; i ++){
		std::string bg_name = StringUtils::format("%s%d", "bg", i + 1);
		auto bg = cocos2d::utils::findChild<ImageView*>(this, bg_name);
		if (bg->getContentSize().width<Director::getInstance()->getVisibleSize().width){
			//if (bg->getScaleY()>0)
			bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
			if (i != 0){
				bg->setPositionY(bgs.at(i - 1)->getPositionY() + bg->getContentSize().height*bg->getScaleX());
			}

			if (i == 1)	{
				bg->setScaleY(-bg->getScaleY());
			}
		}
		bgs.pushBack(bg);
	}
	
	auto posFirst = panelRun->getChildByName<ImageView*>("posFirst");
	auto posStart = panelRun->getChildByName("posStart");

	for (int i = 0; i < 4; i++){
		auto aItem = GameRestInNest_Item::createItem();
		objectPanel->addChild(aItem);
		rest_items.push_back(aItem);

		aItem->reloadInfo(item_infos.at(i));
		aItem->setPositionY(posFirst->getPositionY() + REST_IN_NEST_ITEM_SIZE.y / 2 + REST_IN_NEST_ITEM_SIZE.y * (i));
		for (int j = 0; j < 3; j ++){
			aItem->imgs.at(j)->addClickEventListener([=](Ref* sender){
				if (curText==1 && !game_play_enable){
					pauseGame();
					return;
				}
				if (game_state_rin != REST_IN_NEST_GAME_STATE::RIN_GS_PLAYING)
					return;

				if (aItem->getPositionY() < bird->getPositionY())
					return;
				
//                if (aItem->onClick(j)){
//                    clickTrueItem(aItem, aItem->imgs.at(j));
//                }
                
                //TODO: disable touch
                if(bird->getTag() - i == 3){
                    bird->setTag(-1);
                }
                
                if(i == bird->getTag() +1){
                    if (aItem->onClick(j)){
                        bird->setTag(i);
                        clickTrueItem(aItem, aItem->imgs.at(j));
                    }
                }
                
			});
		}
	}
	
	Text* textPreGame = cocos2d::utils::findChild<Text*>(this, "textPreGame");
	textPreGame->enableOutline(Color4B::WHITE, 2);
	
	bird = GAFWrapper::createGAF("gaf/rest_in_nest/bird/bird.gaf");
	objectPanel->addChild(bird);
	bird->setPosition(posStart->getPosition());
    bird->setTag(-1);
	posBird = bird->getPosition();
	//onLoadLevel();

	auto node_kien = cocos2d::utils::findChild(this, "nodeKien");
	for (int i = 0; i < 3; i++){
		auto kien = GAFWrapper::createGAF("gaf/rest_in_nest/ant/ant.gaf");
		kien->play(true);
		node_kien->addChild(kien);
		playKien(kien);
        kiens.push_back(kien);
	}
    
    if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false)){
        this->scheduleOnce([=](float){
            onPreGame();
        }, 1.0, "preGame");
    }else{
        if (CONFIG_MANAGER->isGameVMonkey()){
            this->scheduleOnce([=](float){
                onPreGame();
            }, 1.0, "preGame");
        }else{
            auto audio_id1 = cocos2d::experimental::AudioEngine::play2d("sounds/rest_in_nest/gameName.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id1, [=](int au_id, std::string path){
                onPreGame();
            });
        }
    }
}

void GameRestInNest::onPreGame()
{
	game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_PRE_GAME;
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playEffect("sounds/rest_in_nest/music.mp3", true);
    }
	auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");
	auto posFirst = panelRun->getChildByName<ImageView*>("posFirst");

	auto gameName = panelRun->getChildByName<ImageView*>("imgGamename");
    if (CONFIG_MANAGER->isGameVMonkey()) gameName->setVisible(false);
    if (CONFIG_MANAGER->isGameVMonkey()){
        auto path_question = game_dataset.hoc_am_hoc_van=="hoc_am"?"sounds/rest_in_nest/question_vi_am.mp3":"sounds/rest_in_nest/question_vi_van.mp3";
        AUDIO_ENGINE->playEffect(path_question);
    }
	gameName->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(0.2f)
		, CallFunc::create([=](){
        auto delay_time = 3.1f;
        if (!CONFIG_MANAGER->isGameVMonkey()){
            delay_time = 1.1f;
            AUDIO_ENGINE->playEffect("sounds/rest_in_nest/Let the bird fly to the leaves with the word....mp3");
        }
		bird->playSequence("fly", true);
		audio_id = AUDIO_ENGINE->playEffect("sounds/rest_in_nest/wing_flap.mp3",true);
		bird->runAction(Sequence::create(
			EaseOut::create(MoveTo::create(2.5f, posFirst->getPosition()),1.5f)
			, CallFunc::create([=](){
			AUDIO_ENGINE->playEffect("sounds/rest_in_nest/fly_land_2.mp3");
			bird->playSequence("idle", true);
			AUDIO_ENGINE->stopEffect(audio_id);
			panelRun->runAction(MoveBy::create(0.5f, Vec2(0, -(Director::getInstance()->getVisibleSize().height / 2 - REST_IN_NEST_ITEM_SIZE.y/2))));

			this->scheduleOnce([=](float dt){
				onStartGame();

			}, delay_time, "start_lvl");
		}),nullptr));
	
	}), nullptr));
	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->playEffect("sounds/rest_in_nest/bird_sound.mp3");
	}, 1.1f, "chipchip");

	this->schedule(schedule_selector(GameRestInNest::updateRestInNest), 0);
}

void GameRestInNest::onStartGame(){

	game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_PLAYING;
	showTextPreGame(item_infos.at(totalTime));
	AUDIO_ENGINE->playEffect("sounds/rest_in_nest/forestsound.mp3", true);
}

void GameRestInNest::onFinishGame(){
    this->unschedule("inActive_mode");
    for(int i = 0; i < kiens.size(); i++)
    {
        if(kiens[i])
        {
            kiens[i]->stopAllActions();
            kiens[i]->removeFromParent();
        }
    }
    kiens.clear();
    
    _callBackKienRunAction = nullptr;
    
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
        //_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        if (CONFIG_MANAGER->isVietnameseStories()) {
            this->onCloseGameVmonkey();
        }else{
            commonEndGame(ms::BaseActivity::EGT_COMMON);
        }
	}), NULL));
}

void GameRestInNest::onFinishLevel()
{
	game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_FINISH;
	auto panelRun = cocos2d::utils::findChild<Layout*>(this, "panelRun");

	bird->playSequence("fly", false, [=](gaf::GAFObject *obj, std::string name_sequence){
		bird->playSequence("idle", true);
	});
	AUDIO_ENGINE->playEffect("sounds/rest_in_nest/jump_fly_land.mp3");
	this->scheduleOnce([=](float dt){
		bird->runAction(EaseInOut::create(MoveTo::create(1.0f, panelRun->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height-150))), 1));
		bird->runAction(RotateTo::create(0.2f, getAngelObjectPlayer(bird->getPosition(), panelRun->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height - 150)))));
	}, 1.0f, "bay");

	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->stopBackgroundMusic();
		AUDIO_ENGINE->stopAllEffects();
		bird->runAction(Sequence::create(RotateTo::create(0.2f, 0), CallFunc::create([=](){
			AUDIO_ENGINE->playEffect("sounds/rest_in_nest/win.mp3");
		})
			,DelayTime::create(3.0f)
			, CallFunc::create([=](){
			onFinishGame();
		}),nullptr));
		
	}, 2.0f, "quay");
}

#pragma callback

void GameRestInNest::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = GameRestInNest::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameRestInNest::onExitTransitionDidStart(){
    for(int i = 0; i < kiens.size(); i++)
    {
        if(kiens[i])
        {
            kiens[i]->stopAllActions();
            kiens[i]->removeFromParent();
        }
    }
    kiens.clear();
    _callBackKienRunAction = nullptr;
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void GameRestInNest::inActiveMode(){
    this->schedule([=](float){
        AUDIO_ENGINE->playEffect(item_infos.at(totalTime).audio_path);
    }, 8, "inActive_mode");
}
