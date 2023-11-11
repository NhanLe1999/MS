#include "SheepVsWolves_VM.h"
#include "HSAudioEngine.h"
//#include <unordered_set>

#define GAME_NAME "SheepVsWolves_VM"

#define IMAGE_BG "games/SheepVsWolves_VM/bg.jpg"
#define IMAGE_LOA "games/SheepVsWolves_VM/loa icon.png"

#define GAF_FENCES "gaf/SheepVsWolves_VM/sheep_fences/sheep_fences.gaf"
#define GAF_FADEIN "gaf/SheepVsWolves_VM/sheep_fadein/sheep_fadein.gaf"
#define GAF_EFFECT "gaf/SheepVsWolves_VM/sheep_effect/sheep_effect.gaf"
#define GAF_KICK "gaf/SheepVsWolves_VM/sheep_kick/sheep_kick.gaf"
#define GAF_LEG "gaf/SheepVsWolves_VM/sheep_leg/sheep_leg.gaf"
#define GAF_SHEEP "gaf/SheepVsWolves_VM/sheep_sheep/sheep_sheep.gaf"
#define GAF_WAREHOUSE "gaf/SheepVsWolves_VM/sheep_warehouse/sheep_warehouse.gaf"
#define GAF_STAR_YELLOW "gaf/SheepVsWolves_VM/stars_yellow/stars_yellow.gaf"

#define SOUND_BGM "sounds/SheepVsWolves_VM/sheep_bgm.mp3"
#define SOUND_BOARD_IN "sounds/SheepVsWolves_VM/sheep_board_in.mp3"
#define SOUND_BOARD_OUT "sounds/SheepVsWolves_VM/sheep_board_out.mp3"
#define SOUND_SHEEP_DOOR_CLOSE "sounds/SheepVsWolves_VM/sheep_door_close.mp3"
#define SOUND_SHEEP_DOOR_OPEN "sounds/SheepVsWolves_VM/sheep_door_open.mp3"
#define SOUND_INTRO "sounds/SheepVsWolves_VM/sheep_intro.mp3"
#define SOUND_INTRO1 "sounds/SheepVsWolves_VM/sheep_intro1.mp3"
#define SOUND_CTW "sounds/SheepVsWolves_VM/complete_the_word.mp3"
#define SOUND_RUN "sounds/SheepVsWolves_VM/sheep_run.mp3"
#define SOUND_RUN_CORRECT "sounds/SheepVsWolves_VM/sheep_run_correct.mp3"
#define SOUND_RUN_IN "sounds/SheepVsWolves_VM/sheep_run_in.mp3"
#define SOUND_RUN_WRONG "sounds/SheepVsWolves_VM/sheep_run_wrong.mp3"
#define SOUND_SHEEP_WALK "sounds/SheepVsWolves_VM/sheep_walk.mp3"
#define SOUND_SELECT "sounds/SheepVsWolves_VM/sheep_select.mp3"
#define SOUND_TEXT_MERGE "sounds/SheepVsWolves_VM/sheep_text_merge.mp3"
#define SOUND_SHEEP_TWINKLE_END "sounds/SheepVsWolves_VM/sheep_twinkle_end.mp3"
#define SOUND_SHEEP_WRONG "sounds/SheepVsWolves_VM/sheep_wrong.mp3"
#define SOUND_SHEEP_CORRECT "sounds/SheepVsWolves_VM/sheep_correct.mp3"

#define SHEEP_WALK_SPEED 150.0f
#define SHEEP_RUN_SPEED 250.0f

SheepVsWolves_VM * SheepVsWolves_VM::createGame(std::string json_path, bool is_auto_play)
{
	auto game = new SheepVsWolves_VM();
	if (game && game->init()) {
		game->didLoadFromCSB(json_path, is_auto_play);
        game->autorelease();
		return game;
	}
	CC_SAFE_DELETE(game);
	return nullptr;
}

void SheepVsWolves_VM::didLoadFromCSB(std::string json_path, bool is_auto_play)
{
	this->m_is_auto_play = is_auto_play;
	if (json_path.size() < 5) {
		json_path = "json/SheepVsWolves_VM/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_path);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());


	rapidjson::Value& game_config_data = document.FindMember("game_config")->value;
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && document.HasMember("audio_intro_vi")){
       auto audio_intro = document["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
	rapidjson::Value& data_config = document.FindMember("data")->value;
	//    rapidjson::Value& game_data = data_config.GetArray()[0].FindMember("game")->value;
	this->generateData(data_config);
}

void SheepVsWolves_VM::generateData(rapidjson::Value& value) {

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
		SheepVsWolvesVM::GAME_INFO game_info;

		auto letter = value[i].FindMember("text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		game_info.text_full = pImage->getText("name_1");
		game_info.n_audio = pImage->getAudios("name_1").front();
        if (pImage->getComparisionValue("position_p") != -1){
            game_info.co_dau = true;
            game_info.pos_dau = pImage->getComparisionValue("position_p");
            auto phonic_text_list = pImage->getTextPhonic("name_n");
            auto phonic_audio_list = pImage->getAudioPhonic("name_n");
            game_info.text_dau = phonic_text_list.back();
            phonic_text_list.pop_back();
            game_info.audio_dau = phonic_audio_list.back();
            phonic_audio_list.pop_back();
            game_info.text_khong_dau = phonic_text_list.back();
            phonic_text_list.pop_back();
            game_info.audio_khong_dau = phonic_audio_list.back();
            phonic_audio_list.pop_back();
            // Gen sync text_info
            int start_index = 0;
            std::queue<int> underline_index;
            for (int i = 0; i < phonic_text_list.size(); ++i) {
                SheepVsWolvesVM::SYNC_TEXT_INFO synctext_info;
                synctext_info.text = phonic_text_list[i];
                synctext_info.audio = phonic_audio_list[i];
                int index = 0;
                for (int j = 0; j < synctext_info.text.size(); ++j) {
                   CCLOG("test %c",synctext_info.text[j]);
                   if (!checkRomaji(synctext_info.text[j])){
                       if (index == 0){
                           synctext_info.list_index.push_back(start_index + index);
                           index++;
                       }else{
                           if(checkRomaji(synctext_info.text[j-1])){
                               synctext_info.list_index.push_back(start_index + index);
                               index++;
                           }
                       }
                   }else{
                       synctext_info.list_index.push_back(start_index +  index);
                       index++;
                   }
                }
                start_index += (index);
                game_info.list_synctext_info.push_back(synctext_info);
            }
        }else{
            auto phonic_text_list = pImage->getTextPhonic("name_n");
            auto phonic_audio_list = pImage->getAudioPhonic("name_n");
            // Gen sync text_info
            int start_index = 0;
            std::queue<int> underline_index;
            for (int i = 0; i < phonic_text_list.size(); ++i) {
                SheepVsWolvesVM::SYNC_TEXT_INFO synctext_info;
                synctext_info.text = phonic_text_list[i];
                synctext_info.audio = phonic_audio_list[i];
                int index = 0;
                for (int j = 0; j < synctext_info.text.size(); ++j) {
                   CCLOG("test %c",synctext_info.text[j]);
                   if (!checkRomaji(synctext_info.text[j])){
                       if (index == 0){
                           synctext_info.list_index.push_back(start_index + index);
                           index++;
                       }else{
                           if(checkRomaji(synctext_info.text[j-1])){
                               synctext_info.list_index.push_back(start_index + index);
                               index++;
                           }
                       }
                   }else{
                       synctext_info.list_index.push_back(start_index +  index);
                       index++;
                   }
                }
                start_index += (index);
                game_info.list_synctext_info.push_back(synctext_info);
            }
        }
		game_info.board_info.text_full = game_info.text_full;
		game_info.board_info.audio_full = game_info.n_audio;
		game_info.board_info.image_path = pImage->path;

		rapidjson::Value& json_list_answer = value[i].FindMember("answer")->value;
		for (rapidjson::SizeType j = 0; j < json_list_answer.Size(); ++j) {
			rapidjson::Value& phonic_data = json_list_answer[j];
			auto answer_id = phonic_data.FindMember("item")->value.FindMember("text")->value.GetString();
			auto aImage = math::resource::ResHelper::getImage(images, answer_id);
			SheepVsWolvesVM::PHONIC_INFO distract_info;
			distract_info.text_phonic = aImage->getText("name_1");
			distract_info.audio_phonic = aImage->getAudios("name_1").front();
			if (phonic_data["item"].HasMember("is_correct"))
				distract_info.missing = phonic_data.FindMember("item")->value.FindMember("is_correct")->value.GetBool();
			else
				distract_info.missing = false;
			game_info.list_distracting.push_back(distract_info);
		}

		auto phonic_text_list = pImage->getTextPhonic("name_p");
		auto phonic_audio_list = pImage->getAudioPhonic("name_p");
		for (rapidjson::SizeType j = 0; j < phonic_text_list.size(); ++j) {
			SheepVsWolvesVM::PHONIC_INFO phonic_info;
			phonic_info.text_phonic = phonic_text_list[j];
			phonic_info.audio_phonic = phonic_audio_list[j];
			phonic_info.missing = false;
			for (int k = 0; k < game_info.list_distracting.size(); ++k) {
				auto answer = game_info.list_distracting[k];
				if (answer.text_phonic == phonic_info.text_phonic && answer.missing) {
					phonic_info.missing = true;
					game_info.list_distracting.erase(game_info.list_distracting.begin() + k);
				}
			}
			game_info.list_phonics.push_back(phonic_info);
		}

		
		m_game_info.push_back(game_info);
	}

	mj::helper::shuffle(m_game_info);
}


void SheepVsWolves_VM::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(screen_size);
	m_layout_root->setTouchEnabled(true);
	//m_layout_root->setPosition(Vec2(1024, 0));
	this->addChild(m_layout_root);


	m_layout_synctext = cocos2d::ui::Layout::create();
	m_layout_synctext->setContentSize(screen_size);
	this->addChild(m_layout_synctext, 100);

	m_img_bg = cocos2d::ui::ImageView::create(IMAGE_BG);
	m_img_bg->setPosition(screen_size / 2);
	m_layout_root->addChild(m_img_bg);


	m_img_bg_left = cocos2d::ui::ImageView::create(IMAGE_BG);
	m_img_bg_left->setPosition(Vec2(screen_size.width / 2 - m_img_bg_left->getContentSize().width, screen_size.height / 2));
	m_img_bg_left->setFlippedX(true);
	m_layout_root->addChild(m_img_bg_left);

	m_img_bg_right = cocos2d::ui::ImageView::create(IMAGE_BG);
	m_img_bg_right->setPosition(Vec2(screen_size.width / 2 + m_img_bg_right->getContentSize().width, screen_size.height / 2));
	m_img_bg_right->setFlippedX(true);
	m_layout_root->addChild(m_img_bg_right);

	m_board = SheepVsWolves_VM_Board::createBoard(m_game_info[0].board_info);
	m_board->setPosition(Vec2(screen_size.width * 0.55, screen_size.height + 300));
	m_board->setVisible(false);
	m_board->setDelegate(this);
	m_board->setScale(1.1f);
	m_layout_root->addChild(m_board, 36);

	// farm layout
	m_fences_flipped = false;
	m_gaf_fences = GAFWrapper::createGAF(GAF_FENCES);
	m_gaf_fences->setPosition(Vec2(screen_size.width * 0.5 + 40, 155));
	m_layout_root->addChild(m_gaf_fences, 35);
	m_gaf_fences->playSequence("idle");
	m_gaf_fences->setVisible(false);
	m_gaf_fences->setOpacity(0);
	m_gaf_fences->setCascadeColorEnabled(true);

	m_gaf_warehouse_back = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_gaf_warehouse_back->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
	m_layout_root->addChild(m_gaf_warehouse_back, 10);
	m_gaf_warehouse_back->playSequence("back");
	m_gaf_warehouse_back->setVisible(false);
	m_gaf_warehouse_back->setOpacity(0);
	m_gaf_warehouse_back->setCascadeColorEnabled(true);

	m_gaf_warehouse_front = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_gaf_warehouse_front->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
	m_layout_root->addChild(m_gaf_warehouse_front, 33);
	m_gaf_warehouse_front->playSequence("front");
	m_gaf_warehouse_front->setVisible(false);
	m_gaf_warehouse_front->setOpacity(0);
	m_gaf_warehouse_front->setCascadeColorEnabled(true);

	m_gaf_warehouse_door = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_gaf_warehouse_door->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
	m_layout_root->addChild(m_gaf_warehouse_door, 34);
	m_gaf_warehouse_door->playSequence("close");
	m_gaf_warehouse_door->setVisible(false);
	m_gaf_warehouse_door->setOpacity(0);
	m_gaf_warehouse_door->setCascadeColorEnabled(true);

	PHONIC_MANAGER->cacheGAF("gaf_effect", GAF_EFFECT);
	PHONIC_MANAGER->cacheGAF("gaf_leg", GAF_LEG);

}

void SheepVsWolves_VM::onEnter()
{
	MJMainGBase::onEnter();
	this->loadFrames();
	initSheepItems();
	//showGameName();
	playBackgroundMusic(SOUND_BGM);
	
	this->scheduleOnce([=](float) {
		showIntroGame();
	}, 0.5f, "delay_onenter");

}

void SheepVsWolves_VM::onExitTransitionDidStart()
{
	PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}

void SheepVsWolves_VM::update(float dt)
{
}

void SheepVsWolves_VM::showGameName()
{
	m_game_state = SVWGameState_VM::GAMENAME;
	auto layout_gamename = cocos2d::ui::Layout::create();
	layout_gamename->setContentSize(screen_size);
	layout_gamename->setCascadeOpacityEnabled(true);
	this->addChild(layout_gamename, 1000);

	auto gaf_gamename = GAFWrapper::createGAF(GAF_FADEIN);
	gaf_gamename->setPosition(Vec2(screen_size.width* 0.5 - 35, screen_size.height * 0.65));
	gaf_gamename->play(false);
	layout_gamename->addChild(gaf_gamename, 100);

	auto lbl_gamename = cocos2d::ui::Text::create(GAME_NAME, font_name, 100);
	lbl_gamename->enableOutline(cocos2d::Color4B(81, 60, 51, 255), 4);
	lbl_gamename->setPosition(cocos2d::Vec2(screen_size.width / 2, screen_size.height * 0.3));
	layout_gamename->addChild(lbl_gamename, 100);

	auto img_gamename = cocos2d::ui::ImageView::create("games/SheepVsWolves_VM/sheep_fadein.png");
	img_gamename->setPosition(Vec2(screen_size.width* 0.5, screen_size.height * 0.624));
	img_gamename->setVisible(false);
	layout_gamename->addChild(img_gamename, 111);
	float dt = 0;
	float gamename_fadetime = 0.1f;
	for (auto i = 0; i < lbl_gamename->getString().size(); ++i) {

		lbl_gamename->getLetter(i)->setOpacity(0);
		lbl_gamename->getLetter(i)->runAction(Sequence::create(DelayTime::create(dt), EaseQuadraticActionIn::create(FadeIn::create(gamename_fadetime)), NULL));
		dt += gamename_fadetime;
	}
	dt += 1.0f;
	this->scheduleOnce([=](float dt) {
		showIntroGame();
		gaf_gamename->setVisible(false);
		img_gamename->setVisible(true);
		layout_gamename->runAction(Sequence::create(FadeOut::create(0.75f), CallFunc::create([=] {
			layout_gamename->removeFromParent();
		}), NULL));
	}, dt, "hide_gamename");
}

void SheepVsWolves_VM::showIntroGame()
{
	// fences
	/*m_gaf_fences->setVisible(true);
	m_gaf_fences->runAction(Sequence::create(FadeIn::create(0.75f), DelayTime::create(1.5f), CallFunc::create([=] {
		startGame();
	}), NULL));*/
	m_game_state = SVWGameState_VM::INTRO;
	AUDIO_ENGINE->playSound(SOUND_INTRO);
	//AUDIO_ENGINE->playSound(SOUND_INTRO1);
	showFarm(0.75f, [=] {
		int padding = 10;
		if (m_sheep_items.size()> 5) padding = 0;
		auto locations = getFrameLocations(Vec2(0.525* screen_size.width, 0.35 * screen_size.height), cocos2d::Size(125, 125), m_sheep_items.size(), m_sheep_items.size(), Vec2(padding, 0));
		AUDIO_ENGINE->playSound(SOUND_SHEEP_DOOR_OPEN);
		AUDIO_ENGINE->playSound(SOUND_SHEEP_WALK);
		m_gaf_warehouse_door->playSequence("open");
		float dt = 0.0f;
		float board_delay = 1.5f;
		for (int i = 0; i < m_sheep_items.size(); ++i) {
			m_sheep_items[i]->setVisible(true);
			if (m_sheep_items[i]->getInfo().missing) {
				m_sheep_items[i]->setOpacity(0);
				m_sheep_items[i]->setPosition(locations[i]);
				m_sheep_items[i]->runAction(Sequence::create(DelayTime::create(1.5f), FadeIn::create(0.5f), NULL));
			}
			else {
				m_sheep_items[i]->setSheepFlipped(true);
				m_sheep_items[i]->setScale(0.75f);
				float distance1 = 150.0f;//m_sheep_items[i]->getPosition().getDistance(Vec2(screen_size.height / 2 + 125, screen_size.height * 0.65f));
				float distance2 = 250.0f;//Vec2(screen_size.height / 2 + 125, screen_size.height * 0.65f).getDistance(locations[i]);
				m_sheep_items[i]->playSheepSequence("walk", true);
				m_sheep_items[i]->runAction(Sequence::create(DelayTime::create(dt),
					Spawn::createWithTwoActions(
						MoveTo::create(distance1 / SHEEP_RUN_SPEED, Vec2(screen_size.width / 2 + 125, screen_size.height * 0.65f)),
						ScaleTo::create(distance1 / SHEEP_RUN_SPEED, 0.85f)
					),
					CallFunc::create([=] {
					if (i < m_sheep_items.size() / 2) {
						m_sheep_items[i]->setSheepFlipped(false);
					}
				}),
					Spawn::createWithTwoActions(
					MoveTo::create(distance2 / SHEEP_WALK_SPEED, locations[i]),
					ScaleTo::create(distance2 / SHEEP_RUN_SPEED, 1.0f)
					),
					CallFunc::create([=] {
					AUDIO_ENGINE->stopSound(SOUND_SHEEP_WALK);
					m_sheep_items[i]->playSheepSequence("stand_idle", true);
					m_sheep_items[i]->setLocalZOrder(50);
				}), NULL));
				dt += 0.5f;
				board_delay = std::max(board_delay, dt + distance1 / SHEEP_WALK_SPEED);
			}
		}
		// board

		AUDIO_ENGINE->playSound(SOUND_BOARD_IN);
		m_board->setOpacity(0);
		m_board->setVisible(true);
		m_board->runAction(Sequence::create(DelayTime::create(board_delay),
			EaseBackOut::create(MoveTo::create(0.5f, Vec2(screen_size.width * 0.55, screen_size.height * 0.73f))),
			DelayTime::create(0.5f),
			CallFunc::create([=] {
			AUDIO_ENGINE->playSound(SOUND_SHEEP_DOOR_CLOSE);
			m_gaf_warehouse_door->playSequence("close");
			startGame();
		}), NULL));
	});
	// sheep
}

void SheepVsWolves_VM::startGame()
{
	m_game_state = SVWGameState_VM::START;
	float dt = 0.0f;
	//m_img_loa->runAction(FadeIn::create(0.5));
	//m_img_loa->setTouchEnabled(true);
	if (m_sheep_missing_left > 0) {
		this->scheduleOnce([=](float dt) {
			onShowSheepFromLeftSide();
		}, dt, "show sheep from left");
		dt += 725 / SHEEP_RUN_SPEED;
	}
	else if (m_sheep_missing_right > 0) {
		this->scheduleOnce([=](float dt) {
			onShowSheepFromRightSide();
		}, dt, "show sheep from right");
		dt += 725 / SHEEP_RUN_SPEED;
	}
	else {
		onNextGame();
		return;
	}
	std::vector<std::string> sound_chain;

	sound_chain.push_back(CONFIG_MANAGER->isVietnameseStories()?question_vi:SOUND_CTW);
    if (!CONFIG_MANAGER->isVietnameseStories()) sound_chain.push_back(m_game_info[game_data_index].n_audio);
	AUDIO_ENGINE->playChain(sound_chain);
	this->scheduleOnce([=](float dt) {
		randomSheepSound();
		if (m_sheep_missing_left > 0) {
			onChangeSide(true);
		}
		else {
			onChangeSide(false);
		}
//        if (game_data_index == 0)
            showGuideGame(15.0f);
		if (m_is_auto_play) {
			auto autoplay_check = cocos2d::utils::findChild<cocos2d::ui::Layout *>(m_layout_root, "layout_autoplay");
			if (autoplay_check) return;
			auto layout_autoplay = cocos2d::ui::Layout::create();
			layout_autoplay->setContentSize(screen_size);
			layout_autoplay->setTouchEnabled(true);
			m_layout_root->addChild(layout_autoplay, INT_MAX - 2, "layout_autoplay");
			autoPlay(1.5f);
		}
	}, dt, "start game");
}

void SheepVsWolves_VM::onNextSide()
{
	if (m_game_state == SVWGameState_VM::PLAYING_LEFT) {
		//--m_sheep_missing_left;
		if (m_sheep_missing_left <= 0) {
			if (m_sheep_missing_right > 0) {
				for (auto sheep : m_sheep_left_items) {
					if (sheep->getSheepType() == SheepVsWolves_VMType::WOLVES && !sheep->isRunning()) {
						sheep->playSheepSequence("bark", true);
					}
				}
				onShowSheepFromRightSide([=] {
					onChangeSide(false);
				});
			}
			else {
				onTransitionSyncText(m_game_info[game_data_index], Vec2(screen_size.width / 2, screen_size.height *0.12), [=] {
					onNextGame();
				});

			}
		}
	}
	else if (m_game_state == SVWGameState_VM::PLAYING_RIGHT) {
		//--m_sheep_missing_right;
		if (m_sheep_missing_right <= 0) {
			if (m_sheep_missing_left > 0) {
				for (auto sheep : m_sheep_right_items) {
					if (sheep->getSheepType() == SheepVsWolves_VMType::WOLVES && !sheep->isRunning()) {
						sheep->playSheepSequence("bark", true);
					}
				}
				onShowSheepFromLeftSide([=] {
					onChangeSide(true);
				});

			}
			else {
				onTransitionSyncText(m_game_info[game_data_index], Vec2(screen_size.width / 2, screen_size.height *0.12), [=] {
					onNextGame();
				});
			}
		}
	}
}

void SheepVsWolves_VM::onChangeSide(bool left_side)
{
	if (left_side) {
		//m_layout_root->setPosition(Vec2(0, 0));
		m_game_state = SVWGameState_VM::PLAYING_LEFT;
		for (auto sheep : m_sheep_left_items) {
			sheep->setEnableClick(true);
		}
		for (auto sheep : m_sheep_right_items) {
			sheep->setEnableClick(false);
		}
		m_layout_root->stopAllActions();
		m_layout_root->runAction(MoveTo::create(0.5f, Vec2(150.0f, 0)));
		setFencesSide(false);
		// sheeps on left side
		auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 500, 0.375 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(0, 0), Vec2(40, 0));
		for (int i = 0; i < m_sheep_left_items.size(); ++i) {
			m_sheep_left_items[i]->setPosition(left_locations[i]);
		}
		// sheeps on right side
		auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 725, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(0, 0), Vec2(-125, 0));
		for (int i = 0; i < m_sheep_right_items.size(); ++i) {
			m_sheep_right_items[i]->setPosition(right_locations[i]);
		}
	//	// sheep in farm
	//	int padding = 10;
	//	if (m_sheep_items.size() > 5) padding = 0;
	//	auto locations = getFrameLocations(Vec2(0.5* screen_size.width, 0.35 * screen_size.height), cocos2d::Size(125, 125), m_sheep_items.size(), m_sheep_items.size(), Vec2(padding, 0));
	//	for (int i = 0; i < m_sheep_items.size(); ++i) {
	//		m_sheep_items[i]->setPosition(locations[i]);
	//	}
	}
	else {
		//m_layout_root->setPosition(Vec2(0, 0));
		m_game_state = SVWGameState_VM::PLAYING_RIGHT;
		for (auto sheep : m_sheep_left_items) {
			sheep->setEnableClick(false);
		}
		for (auto sheep : m_sheep_right_items) {
			sheep->setEnableClick(true);
		}
		m_layout_root->stopAllActions();
		m_layout_root->runAction(MoveTo::create(0.5f, Vec2(-200.0f, 0)));
		setFencesSide(true);
		// sheeps on left side
		auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 750, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(0, 0), Vec2(125, 0));
		for (int i = 0; i < m_sheep_left_items.size(); ++i) {
			m_sheep_left_items[i]->setPosition(left_locations[i]);
		}
		// sheeps on right side
		auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 575, 0.375 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(10, 0), Vec2(-40, 0));
		for (int i = 0; i < m_sheep_right_items.size(); ++i) {
			m_sheep_right_items[i]->setPosition(right_locations[i]);
		}
		//// sheep in farm
		//int padding = 10;
		//if (m_sheep_items.size() > 5) padding = 0;
		//auto locations = getFrameLocations(Vec2(0.55* screen_size.width, 0.35 * screen_size.height), cocos2d::Size(125, 125), m_sheep_items.size(), m_sheep_items.size(), Vec2(padding, 0));
		//for (int i = 0; i < m_sheep_items.size(); ++i) {
		//	m_sheep_items[i]->setPosition(locations[i]);
		//}
	}
}

void SheepVsWolves_VM::onTransition()
{
	onRefresh();
	AUDIO_ENGINE->playSound(SOUND_BOARD_OUT);
	m_board->setLocalZOrder(36);
	m_board->setAnchorPoint(Vec2(0.5f, 0.3f));
	m_board->setClickedBoardEnabled(true);
	m_board->setIgnoreAnchorPointForPosition(false);
	m_board->runAction(Sequence::create(
		EaseQuadraticActionOut::create(MoveBy::create(0.5f, Vec2(0, screen_size.height))),
		ScaleTo::create(0.0f, 1.1f),
		CallFunc::create([=] {
		AUDIO_ENGINE->playSound(SOUND_BOARD_IN);
		m_board->setInfo(m_game_info[game_data_index].board_info);
	}), EaseBackOut::create(MoveTo::create(0.5f, Vec2(screen_size.width * 0.55, screen_size.height * 0.73f))),
		CallFunc::create([=] {
		startGame();
	}), NULL));
}

void SheepVsWolves_VM::onNextGame()
{
	if (game_data_index < m_game_info.size() - 1) {
		++game_data_index;
		onTransition();
	}
	else {
		onFinishGame();
	}
}

void SheepVsWolves_VM::onFinishGame()
{
	PHONIC_MANAGER->clearCacheGaf();
	//m_img_loa->setTouchEnabled(false);
	AUDIO_ENGINE->playSound(SOUND_BOARD_OUT);
	m_board->runAction(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, screen_size.height))));
	m_layout_root->runAction(EaseQuadraticActionOut::create(MoveTo::create(0.45f, Vec2(0, 0))));

	float dt = 0.5f;
	float finish_delay = 0.0f;
	for (auto sheep : m_sheep_items) {
		float distance = 250;//sheep->getPosition().getDistance(Vec2(screen_size.width / 2 + 125, screen_size.height * 0.6f));
		sheep->playSheepSequence("walk", true);
		sheep->runAction(Sequence::create(DelayTime::create(dt),
			MoveTo::create(distance / SHEEP_WALK_SPEED, Vec2(screen_size.width / 2 + 125, screen_size.height * 0.6f)),
			MoveTo::create(150 / SHEEP_WALK_SPEED, Vec2(screen_size.width / 2 + 125, screen_size.height * 0.75f)), NULL));
		dt += 0.5f;
		finish_delay = std::max(finish_delay, dt + 400 / SHEEP_WALK_SPEED);
	}
	m_gaf_warehouse_door->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(SOUND_SHEEP_DOOR_OPEN);
		m_gaf_warehouse_door->playSequence("open");
	}), DelayTime::create(finish_delay - 0.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(SOUND_SHEEP_DOOR_CLOSE);
		m_gaf_warehouse_door->playSequence("close");
	}), NULL));
	scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(SOUND_SHEEP_WALK);
		//AUDIO_ENGINE->playSound(SOUND_RUN);
	}, 0.5f, "play sound walk and run");
	scheduleOnce([=](float dt) {
		AUDIO_ENGINE->stopSound(SOUND_SHEEP_WALK);
		//AUDIO_ENGINE->stopSound(SOUND_RUN);
	}, finish_delay, "stop sound walk and run");
	for (auto sheep : m_sheep_left_items) {
		sheep->setSheepFlipped(true);
		sheep->playSheepSequence("wrun", true);
		sheep->runAction(Sequence::create(DelayTime::create(0.5f),
			Spawn::create(MoveBy::create(500 / SHEEP_RUN_SPEED, Vec2(-500.0f, 0)), FadeOut::create(2.0f), NULL), NULL));
	}
	for (auto sheep : m_sheep_right_items) {
		sheep->setSheepFlipped(false);
		sheep->playSheepSequence("wrun", true);
		sheep->runAction(Sequence::create(DelayTime::create(0.5f),
			Spawn::create(MoveBy::create(500 / SHEEP_RUN_SPEED, Vec2(500.0f, 0)), FadeOut::create(2.0f), NULL), NULL));
	}
	this->scheduleOnce([=](float dt) {
        AUDIO_ENGINE->stopAllAudio();
		MJMainGBase::onFinishGame();
	}, finish_delay, "finish game");

}

void SheepVsWolves_VM::onRefresh()
{
	for (auto sheep : m_sheep_items) {
		sheep->removeFromParent();
	}
	for (auto sheep : m_sheep_left_items) {
		sheep->removeFromParent();
	}
	for (auto sheep : m_sheep_right_items) {
		sheep->removeFromParent();
	}

	initSheepItems();
	int padding = 10;
	if (m_sheep_items.size()> 5) padding = 0;
	auto locations = getFrameLocations(Vec2(0.525* screen_size.width, 0.35 * screen_size.height), cocos2d::Size(125, 125), m_sheep_items.size(), m_sheep_items.size(), Vec2(padding, 0));
	for (int i = 0; i < m_sheep_items.size(); ++i) {
		m_sheep_items[i]->setLocalZOrder(50);
		m_sheep_items[i]->setPosition(locations[i]);
		m_sheep_items[i]->setOpacity(255);
		m_sheep_items[i]->setVisible(true);
	}
}

void SheepVsWolves_VM::showGuideGame(float dt)
{

    this->schedule([=](float){
        AUDIO_ENGINE->playSound(CONFIG_MANAGER->isVietnameseStories()?question_vi:SOUND_CTW);
    }, 15, 1000, dt, "playGuideSound");
    
    
	if (m_game_state == SVWGameState_VM::PLAYING_LEFT) {
		for (auto sheep : m_sheep_left_items) {
			for (auto sh : m_sheep_items) {
				if (sh->getInfo().missing && sh->getInfo().text_phonic == sheep->getInfo().text_phonic) {
					showTapGuideGame(sheep->getPosition() + Vec2(150, 0), dt);
					break;
				}
			}
		}
	}
	else if (m_game_state == SVWGameState_VM::PLAYING_RIGHT) {
		for (auto sheep : m_sheep_right_items) {
			for (auto sh : m_sheep_items) {
				if (sh->getInfo().missing && sh->getInfo().text_phonic == sheep->getInfo().text_phonic) {
					showTapGuideGame(sheep->getPosition() - Vec2(200, 0), dt);
					break;
				}
			}
		}
	}

}

void SheepVsWolves_VM::hideGuideGame()
{
    AUDIO_ENGINE->stopSound(CONFIG_MANAGER->isVietnameseStories()?question_vi:SOUND_CTW);
	unschedule("playGuideSound");
	MJMainGBase::hideTapGuideGame();
}

void SheepVsWolves_VM::onTransitionSyncText(SheepVsWolvesVM::GAME_INFO info, cocos2d::Vec2 pos, std::function<void()> call_back)
{
	m_game_state = SVWGameState_VM::SYNC_TEXT;
	this->unschedule("random_sound_sheep");
	//m_board->ignoreAnchorPointForPosition(true);
	//m_board->setAnchorPoint(Vec2(0.5 - m_layout_root->getPositionX() /screen_size.width, 0.7));
	for (auto sheep : m_sheep_left_items) {
		sheep->setEnableClick(false);
		sheep->runAction(Sequence::create(DelayTime::create(0.75f), FadeOut::create(0.5f), NULL));
	}
	for (auto sheep : m_sheep_right_items) {
		sheep->setEnableClick(false);
		sheep->runAction(Sequence::create(DelayTime::create(0.75f), FadeOut::create(0.5f), NULL));
	}
	AUDIO_ENGINE->playSound(SOUND_BOARD_IN);
	float dt = 0;
	float total_time = m_sheep_items.size() * 0.25f;
	//m_img_loa->setTouchEnabled(false);
	//m_img_loa->runAction(FadeOut::create(0.5f));
	m_board->setLocalZOrder(10000);
	m_board->setClickedBoardEnabled(false);
	m_board->runAction(Sequence::create(
		DelayTime::create(0.75f),
		Spawn::create(ScaleTo::create(0.5, 2.0f),
			EaseQuadraticActionIn::create(MoveTo::create(0.5f, m_layout_root->convertToNodeSpace(Vec2(screen_size.width / 2, screen_size.height * 0.6)))), NULL), NULL));
	auto move_locations = getFrameLocations(pos, cocos2d::Size(60, 60), info.text_full.size(), info.text_full.size());
	
	int start_index = 0;
	std::queue<int> underline_index;
	for (int i = 0; i < m_sheep_items.size(); ++i) {
		auto phonic_info = m_sheep_items[i]->getInfo();//info.list_synctext_info[i];
		auto locations = getFrameLocations(m_sheep_items[i]->getPosition(), cocos2d::Size(40, 60), phonic_info.text_phonic.size(), phonic_info.text_phonic.size(), Vec2(10, 0));
		auto sheep = m_sheep_items[i];
		sheep->setLocalZOrder(20 + i);
		AUDIO_ENGINE->playSound(SOUND_SHEEP_TWINKLE_END);

		// tao vi tri
		std::vector<int> list_index;
		for (int j = 0; j < phonic_info.text_phonic.size(); ++j) {
			if (phonic_info.text_phonic[j] == '_') {
				//underline_index
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					list_index.push_back(start_index + j);
				}
				else {
					list_index.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_info.text_phonic.size();


		sheep->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			auto effect_star = GAFWrapper::createGAF(GAF_STAR_YELLOW);
			effect_star->setPosition(sheep->getPosition());
			m_layout_root->addChild(effect_star, 10002);
			effect_star->play(false, [=](gaf::GAFObject * obj) {
				effect_star->removeFromParent();
			});
			sheep->getLableInfo()->setVisible(false);
			//	m_sheep_items[i]->setVisible(false);
			int ind = 0;
            auto text = cocos2d::ui::Text::create(phonic_info.text_phonic, font_name, 100);
            //text->enableOutline(cocos2d::Color4B(81, 60, 51, 255), 1);
            text->setColor(cocos2d::Color3B::BLACK);
            text->setPosition(m_layout_root->convertToWorldSpace(locations.front()));
            m_layout_synctext->addChild(text, 10001);
            text->runAction(Sequence::create(DelayTime::create(total_time - dt), MoveTo::create(0.5f, move_locations[list_index[ind]]), CallFunc::create([=] {
                text->removeFromParent();
            }), NULL));
            ++ind;
				
		}), NULL));
		dt += 0.25f;
	}
	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->playSound(SOUND_TEXT_MERGE);
	}, total_time + 0.25, "play sound text merge");
	dt += 0.5f;
	this->scheduleOnce([=](float dt) {
		readSyncText(info, pos, call_back);
	}, dt, "read sync text");
}

void SheepVsWolves_VM::readSyncText(SheepVsWolvesVM::GAME_INFO info, cocos2d::Vec2 pos, std::function<void()> call_back)
{
    
    auto lbl_synctext = cocos2d::ui::Text::create(info.co_dau?info.text_khong_dau:info.text_full, font_name, 120);
	lbl_synctext->setPosition(pos);
	//lbl_synctext->setTextColor(Color4B::BLACK);
	m_layout_synctext->addChild(lbl_synctext, 1000000, "lbl_synctext");
    if (info.co_dau){
        lbl_dau = cocos2d::ui::Text::create(info.text_dau, font_name, 120);
        auto lbl = lbl_synctext->getLetter(info.pos_dau);
        auto pos = lbl->getPosition();
        lbl_dau->setPosition(Vec2(pos.x,pos.y+lbl->getContentSize().height/2));
        if (info.text_dau == "/"){
            lbl_dau->setPosition(Vec2(pos.x+10,pos.y+lbl->getContentSize().height/2));
        }
        //lbl_dau->setTextColor(Color4B::BLACK);
        lbl_synctext->addChild(lbl_dau);
        lbl_dau->setName("dau");
    }else{
        lbl_dau = nullptr;
    }
	float dt = 0.25;
	for (int i = 0; i < info.list_synctext_info.size(); ++i) {
		m_layout_synctext->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->play(info.list_synctext_info[i].audio);
			for (int index : info.list_synctext_info[i].list_index) {
				lbl_synctext->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					lbl_synctext->getLetter(index)->setColor(Color3B::RED);
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					lbl_synctext->getLetter(index)->setColor(Color3B::WHITE);
				}), NULL));
			}
		}), NULL));
		dt += 0.65f;
	}
    if (info.co_dau){
        this->runAction(Sequence::create(DelayTime::create(dt),CallFunc::create([=] {
                AUDIO_ENGINE->play(info.audio_khong_dau);
                for (int i = 0; i < lbl_synctext->getStringLength(); i++){
                    lbl_synctext->getLetter(i)->runAction(Sequence::create(CallFunc::create([=] {
                        lbl_synctext->getLetter(i)->setColor(Color3B::RED);
                    }), ScaleTo::create(0.45f, 1, 1.2f), ScaleTo::create(0.45f, 1, 1), CallFunc::create([=] {
                        lbl_synctext->getLetter(i)->setColor(Color3B::WHITE);
                    }), NULL));
                }
            }),DelayTime::create(1.f),CallFunc::create([=] {
            AUDIO_ENGINE->play(info.audio_dau);
            if (lbl_dau){
                lbl_dau->setTextColor(Color4B::RED);
                lbl_dau->runAction(Sequence::create(ScaleTo::create(0.45f, 1, 1.2f), ScaleTo::create(0.45f, 1, 1),CallFunc::create([=] {
                    lbl_dau->setTextColor(Color4B::WHITE);
                }),NULL));
            }
        }), NULL));
        dt+=2.f;
    }
    
//	auto lbl_synctext1 = cocos2d::ui::Text::create(info.text_full, font_name, 120);
//	lbl_synctext1->setPosition(pos);
//	lbl_synctext1->setVisible(false);
//	m_layout_synctext->addChild(lbl_synctext1, 1000000, "lbl_synctext1");
//	lbl_synctext1->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
//		lbl_synctext->removeFromParent();
//		lbl_synctext1->setVisible(true);
//        AUDIO_ENGINE->play(info.n_audio);
//		lbl_synctext1->setTextColor(Color4B(250, 54, 208, 255));
//	}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
//		lbl_synctext1->setTextColor(Color4B::BLACK);
//	}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), FadeOut::create(0.2f), CallFunc::create([=] {
//		lbl_synctext1->removeFromParent();
//	}), NULL));
    
    lbl_synctext->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
        AUDIO_ENGINE->play(info.n_audio);
        lbl_synctext->setTextColor(Color4B::RED);
        if (lbl_dau){
            lbl_dau->setTextColor(Color4B::RED);
        }
    }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
        lbl_synctext->setTextColor(Color4B::WHITE);
        if (lbl_dau){
            lbl_dau->setTextColor(Color4B::WHITE);
        }
    }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), FadeOut::create(0.2f), CallFunc::create([=] {
        lbl_synctext->removeFromParent();
    }), NULL));
	dt += 0.8f;
	scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, dt, "synctext call_back");
}

void SheepVsWolves_VM::initSheepItems()
{
	m_sheep_missing_left = 0;
	m_sheep_missing_right = 0;
	m_sheep_items.clear();
	m_sheep_left_items.clear();
	m_sheep_right_items.clear();
	// sheep item

	std::vector<SheepVsWolvesVM::PHONIC_INFO> list_phonic(m_game_info[game_data_index].list_phonics);
	for (int i = 0; i < list_phonic.size(); ++i) {
		auto phonic = list_phonic[i];
		if (!phonic.missing && phonic.text_phonic.find_first_of('_') != std::string::npos) {
			auto list_char = splitString(phonic.text_phonic, '_');
			list_phonic[i].text_phonic = list_char[0];
			SheepVsWolvesVM::PHONIC_INFO info;
			info.text_phonic = list_char[1];
			info.missing = false;
			list_phonic.insert(list_phonic.begin() + i + 2, info);
		}
	}
	for (int i = 0; i < list_phonic.size(); ++i) {
		auto sheep = SheepVsWolves_VM_Item::createItem(list_phonic[i], SheepVsWolves_VMType::SHEEP_ONFARM);
		sheep->setPosition(Vec2(screen_size.width / 2 + 125, screen_size.height * 0.75));
		sheep->setCascadeOpacityEnabled(true);
		sheep->setVisible(false);
		sheep->setDelegate(this);
		m_sheep_items.push_back(sheep);
		m_layout_root->addChild(sheep, 20 - i);
		if (i < list_phonic.size() / 2) {
			if (list_phonic[i].missing) {
				++m_sheep_missing_left;
			}
		}
		else {
			sheep->setSheepFlipped(true);
			if (list_phonic[i].missing) {
				++m_sheep_missing_right;
			}
		}
	}
	// sheep vs wolves on left side

	for (int i = 0; i < list_phonic.size() / 2; ++i) {
		if (list_phonic[i].missing) {
			auto info = list_phonic[i];
			info.missing = false;
			auto sheep = SheepVsWolves_VM_Item::createItem(info);
			sheep->setCascadeOpacityEnabled(true);
			sheep->setVisible(false);
			sheep->setDelegate(this);
			m_sheep_left_items.push_back(sheep);
			m_layout_root->addChild(sheep, 101);
		}
	}
	mj::helper::shuffle(m_game_info[game_data_index].list_distracting);
	for (auto distract_info : m_game_info[game_data_index].list_distracting) {
		auto sheep = SheepVsWolves_VM_Item::createItem(distract_info, SheepVsWolves_VMType::WOLVES);
		sheep->setCascadeOpacityEnabled(true);
		sheep->setVisible(false);
		sheep->setDelegate(this);
		m_sheep_left_items.push_back(sheep);
		m_layout_root->addChild(sheep, 101);
		if (m_sheep_left_items.size() >= 4) break;
	}
	mj::helper::shuffle(m_sheep_left_items);

	// sheep vs wolves on right side
	for (int i = list_phonic.size() / 2; i < list_phonic.size(); ++i) {
		if (list_phonic[i].missing) {
			auto info = list_phonic[i];
			info.missing = false;
			auto sheep = SheepVsWolves_VM_Item::createItem(info);
			sheep->setCascadeOpacityEnabled(true);
			sheep->setVisible(false);
			sheep->setDelegate(this);
			sheep->setSheepFlipped(true);
			m_sheep_right_items.push_back(sheep);
			m_layout_root->addChild(sheep, 101);
		}
	}
	mj::helper::shuffle(m_game_info[game_data_index].list_distracting);
	for (auto distract_info : m_game_info[game_data_index].list_distracting) {
		auto sheep = SheepVsWolves_VM_Item::createItem(distract_info, SheepVsWolves_VMType::WOLVES);
		sheep->setCascadeOpacityEnabled(true);
		sheep->setVisible(false);
		sheep->setDelegate(this);
		sheep->setSheepFlipped(true);
		m_sheep_right_items.push_back(sheep);
		m_layout_root->addChild(sheep, 101);
		if (m_sheep_right_items.size() >= 4) break;
	}
	mj::helper::shuffle(m_sheep_right_items);
	// disable click

	for (auto sheep : m_sheep_left_items) {
		sheep->setEnableClick(false);
	}
	for (auto sheep : m_sheep_right_items) {
		sheep->setEnableClick(false);
	}
}

void SheepVsWolves_VM::onShowSheepFromLeftSide(std::function<void()> call_back)
{
	//m_game_state = SVWGameState_VM::PLAYING_LEFT;
	// set postiton
	setFencesSide(false);
	// sheeps on right side
	auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 725, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(0, 0), Vec2(-125, 0));
	for (int i = 0; i < m_sheep_right_items.size(); ++i) {
		m_sheep_right_items[i]->setPosition(right_locations[i]);
	}
	AUDIO_ENGINE->playSound(SOUND_RUN);
	scheduleOnce([=](float dt) {
		AUDIO_ENGINE->stopSound(SOUND_RUN);
	}, 600 / SHEEP_RUN_SPEED, "stop sound walk");
	m_layout_root->stopAllActions();
	m_layout_root->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(800, 0)), MoveTo::create(600 / SHEEP_RUN_SPEED, Vec2(150, 0)), NULL));
	auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 500, 0.375 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(10, 0), Vec2(40, 0));
	for (int i = 0; i < m_sheep_left_items.size(); ++i) {
		m_sheep_left_items[i]->setLocalZOrder(50 - i);
		m_sheep_left_items[i]->setVisible(true);
		m_sheep_left_items[i]->playSheepSequence("run", true);
		m_sheep_left_items[i]->setPosition(Vec2(left_locations[i].x - 600, left_locations[i].y));
		m_sheep_left_items[i]->runAction(cocos2d::Sequence::create(MoveTo::create(600 / SHEEP_RUN_SPEED, left_locations[i]),
			CallFunc::create([=] {
			m_sheep_left_items[i]->playSheepSequence("stand_idle", true);
		}), NULL));
	}
	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, 725 / SHEEP_RUN_SPEED, "sheep left call back");
}

void SheepVsWolves_VM::onShowSheepFromRightSide(std::function<void()> call_back)
{
	//m_game_state = SVWGameState_VM::PLAYING_RIGHT;
	setFencesSide(true);
	// sheeps on left side
	auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 750, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(0, 0), Vec2(125, 0));
	for (int i = 0; i < m_sheep_left_items.size(); ++i) {
		m_sheep_left_items[i]->setPosition(left_locations[i]);
	}
	// set postiton
	m_layout_root->stopAllActions();
	m_layout_root->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(-800, 0)), MoveTo::create(600 / SHEEP_RUN_SPEED, Vec2(-200, 0)), NULL));
	auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 575, 0.375 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(10, 0), Vec2(-40, 0));

	AUDIO_ENGINE->playSound(SOUND_RUN);
	scheduleOnce([=](float dt) {
		AUDIO_ENGINE->stopSound(SOUND_RUN);
	}, 600 / SHEEP_RUN_SPEED, "stop sound walk");

	for (int i = 0; i < m_sheep_right_items.size(); ++i) {
		m_sheep_right_items[i]->setLocalZOrder(50 - i);
		m_sheep_right_items[i]->setVisible(true);
		m_sheep_right_items[i]->playSheepSequence("run", true);
		m_sheep_right_items[i]->setPosition(Vec2(right_locations[i].x + 600, right_locations[i].y));
		m_sheep_right_items[i]->runAction(cocos2d::Sequence::create(MoveTo::create(600 / SHEEP_RUN_SPEED, right_locations[i]),
			CallFunc::create([=] {
			m_sheep_right_items[i]->playSheepSequence("stand_idle", true);
		}), NULL));
	}
	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, 725 / SHEEP_RUN_SPEED, "sheep right call back");
}

void SheepVsWolves_VM::onClickSheepVsWolves_VMDelegate(cocos2d::Ref * sender)
{

	auto sheep = static_cast<SheepVsWolves_VM_Item *>(sender);
	if (sheep->getSheepType() == SheepVsWolves_VMType::SHEEP_ONFARM) {
		if (sheep->getInfo().missing == false) {
			//AUDIO_ENGINE->playSound(sheep->getInfo().audio_phonic);
		}
		return;
	}
	hideGuideGame();
	sheep->setEnableClick(false);
	SheepVsWolves_VM_Item * sheep_correct = nullptr;
	AUDIO_ENGINE->playSound(SOUND_SELECT);
	AUDIO_ENGINE->playSound(sheep->getInfo().audio_phonic);
	for (auto sh : m_sheep_items) {
		if (sh->getInfo().missing && sh->getInfo().text_phonic == sheep->getInfo().text_phonic) {
			sheep_correct = sh;
			break;
		}
	}
	Vec2 start_pos = sheep->getPosition();
	Vec2 pos_jump;
	float thresh_height = 0.375 * screen_size.height;
	float thresh_weight = 0.5 * screen_size.width;
	if (sheep_correct) {
		pos_jump = sheep_correct->getPosition();
		if (start_pos.y > thresh_height) {
			sheep->setLocalZOrder(49);
		}
		else {
			sheep->setLocalZOrder(51);
		}
		if (m_game_state == SVWGameState_VM::PLAYING_LEFT) {
			--m_sheep_missing_left;
		}
		else if (m_game_state == SVWGameState_VM::PLAYING_RIGHT) {
			--m_sheep_missing_right;
		}
	}
	else {
		if (start_pos.y < thresh_height) {
			if (start_pos.x < thresh_weight) {
				pos_jump = Vec2(start_pos.x + 250, std::min(start_pos.y + 100.0f, thresh_height));
			}
			else {
				pos_jump = Vec2(start_pos.x - 250, std::min(start_pos.y + 100.0f, thresh_height));
			}
		}
		else {
			if (start_pos.x < thresh_weight) {
				pos_jump = Vec2(start_pos.x + 250, std::max(start_pos.y - 100, thresh_height));
			}
			else {
				pos_jump = Vec2(start_pos.x - 250, std::max(start_pos.y - 100, thresh_height));
			}
		}
	}

	// run Action

	if (sheep_correct) {
		// sheep
		float distance = start_pos.getDistance(pos_jump);
		float num_step = distance / 150;
		float time_run = num_step * 0.75f;
		sheep->playSheepSequence("jump", true);
		float xDist = pos_jump.x - start_pos.x;
		float yDist = pos_jump.y - start_pos.y;
		float dist = xDist * xDist + yDist * yDist;
		float fraction_of_total = 75 / distance;
		auto pos_step = Vec2(start_pos.x + xDist * fraction_of_total, start_pos.y + yDist * fraction_of_total);
		AUDIO_ENGINE->playSound(SOUND_RUN_CORRECT, true);
		this->scheduleOnce([=](float dt) {
			AUDIO_ENGINE->stopSound(SOUND_RUN_CORRECT);
		}, time_run, "stop_sound_run_correct");
		sheep->runAction(
			Sequence::create(MoveTo::create(0.75, pos_step), MoveTo::create(time_run - 0.75, pos_jump), CallFunc::create([=] {
			//AUDIO_ENGINE->stopSound(SOUND_RUN_IN);
			AUDIO_ENGINE->playSound(SOUND_SHEEP_CORRECT);
			auto gaf_star_yellow = GAFWrapper::createGAF(GAF_STAR_YELLOW);
			gaf_star_yellow->setPosition(sheep_correct->getPosition());
			m_layout_root->addChild(gaf_star_yellow, 1000);
			gaf_star_yellow->play(false, [=](gaf::GAFObject * obj) {
				gaf_star_yellow->removeFromParent();
			});
			sheep_correct->setSheepMissing(false);
			sheep->setVisible(false);
			//sheep->playSheepSequence("stand_idle", true);	
		}), NULL));
		this->scheduleOnce([=](float dt) {
			onNextSide();
		}, time_run + 0.5f, "next_side");
	}
	else {
		if (sheep->getPositionX() < screen_size.width / 2 && m_sheep_missing_left <= 0) {
			sheep->playSheepSequence("bark", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				sheep->playSheepSequence("stand_idle", true);
			});
			return;
		}
		else if (sheep->getPositionX() > screen_size.width / 2 && m_sheep_missing_right <= 0) {
			sheep->playSheepSequence("bark", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				sheep->playSheepSequence("stand_idle", true);
			});
			return;
		}
		sheep->playSheepSequence("wjump", true);
		// gaf leg
		auto gaf_leg = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_leg"]);
		gaf_leg->setPosition(pos_jump + Vec2(-300, 150));
		m_layout_root->addChild(gaf_leg, 1000);
		gaf_leg->setVisible(false);
		if (sheep->getPositionX() > screen_size.width / 2) {
			gaf_leg->setScaleX(-1);
			gaf_leg->setPositionX(gaf_leg->getPositionX() + 350);
		}
		gaf_leg->runAction(Sequence::create(DelayTime::create(27.0f / 32),
			CallFunc::create([=] {
			gaf_leg->setVisible(true);
			auto gaf_effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_effect"]);
			gaf_effect->setPosition(pos_jump);
			gaf_effect->setVisible(false);
			m_layout_root->addChild(gaf_effect, 1000);
			gaf_effect->runAction(Sequence::create(DelayTime::create(15.0f / 32), CallFunc::create([=] {
				gaf_effect->setVisible(true);
				gaf_effect->playSequence("kicked", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
					gaf_effect->removeFromParent();
				});
			}), NULL));
		}), Spawn::create(
			EaseQuadraticActionInOut::create(MoveBy::create(25.0f / 32, Vec2(150, 0))),
			CallFunc::create([=] {
			gaf_leg->play(false, [=](gaf::GAFObject * obj) {
				gaf_leg->removeFromParent();
			});
		}), NULL), NULL));
		AUDIO_ENGINE->playSound(SOUND_RUN_WRONG);
		sheep->runAction(
			// sheep
			Sequence::create(EaseQuadraticActionIn::create(MoveTo::create(1.5f, pos_jump)), CallFunc::create([=] {
			sheep->getLableInfo()->setOpacity(0);
			sheep->runAction(Sequence::create(EaseQuadraticActionOut::create(MoveTo::create(22.0f / 32, start_pos)), NULL));
			AUDIO_ENGINE->play(SOUND_SHEEP_WRONG);
			sheep->playSheepSequence("kicked", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				sheep->playSheepSequence("hit", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
					sheep->playSheepSequence("bark", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
						sheep->playSheepSequence("stand_idle");
					});
					sheep->getLableInfo()->setOpacity(255);
					sheep->setEnableClick(true);
				});
			});
			auto gaf_effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_effect"]);
			gaf_effect->setPosition(Vec2(start_pos.x - 16, start_pos.y));
			gaf_effect->setVisible(false);
			m_layout_root->addChild(gaf_effect, 1000);
			gaf_effect->runAction(Sequence::create(DelayTime::create(22.0f / 32), CallFunc::create([=] {
				gaf_effect->setVisible(true);
				gaf_effect->playSequence("hit", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
					gaf_effect->removeFromParent();
				});
			}), NULL));
		}), NULL));
	}
}

void SheepVsWolves_VM::onClickedBoardDelegate(cocos2d::Ref * sender)
{
	auto board = static_cast<SheepVsWolves_VM_Board *> (sender);
	board->setClickedBoardEnabled(false);
	std::vector<std::string> sound_chain;
	for (auto info : m_game_info) {
		if (board->getInfo().text_full == info.text_full) {
			AUDIO_ENGINE->playSound(info.n_audio, false, [=] {
				board->setClickedBoardEnabled(true);
			});
			//sound_chain.push_back(info.n_audio);
			//sound_chain.push_back(info.p_audio);
			break;
		}
	}
	/*AUDIO_ENGINE->playChain(sound_chain, [=] {
		board->setClickedBoardEnabled(true);
	});*/
}

void SheepVsWolves_VM::showFarm(float dt, std::function<void()> call_back)
{
	m_gaf_fences->setVisible(true);
	m_gaf_warehouse_back->setVisible(true);
	m_gaf_warehouse_front->setVisible(true);
	m_gaf_warehouse_door->setVisible(true);
	m_gaf_fences->runAction(FadeIn::create(dt));
	m_gaf_warehouse_back->runAction(FadeIn::create(dt));
	m_gaf_warehouse_door->runAction(FadeIn::create(dt));
	m_gaf_warehouse_front->runAction(FadeIn::create(dt));
	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, dt, "call_back show Farm");
}

void SheepVsWolves_VM::hideFarm(float dt, std::function<void()> call_back)
{
	m_gaf_fences->runAction(FadeOut::create(dt));
	m_gaf_warehouse_back->runAction(FadeOut::create(dt));
	m_gaf_warehouse_door->runAction(FadeOut::create(dt));
	m_gaf_warehouse_front->runAction(FadeOut::create(dt));
	this->scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, dt, "call_back hide Farm");
}

void SheepVsWolves_VM::setFencesSide(bool flipped, bool transition)
{
	if (m_fences_flipped == flipped) return;
	m_fences_flipped = flipped;
	if (m_fences_flipped) {
		if (transition) {
			m_gaf_fences->playSequence("action", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				m_gaf_fences->playSequence("idle2", false);
			});
		}
		else {
			m_gaf_fences->playSequence("idle2", false);
		}

	}
	else {
		if (transition) {
			m_gaf_fences->playSequence("action2", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				m_gaf_fences->playSequence("idle", false);
			});
		}
		else {
			m_gaf_fences->playSequence("idle", false);
		}

	}
}

void SheepVsWolves_VM::randomSheepSound()
{
	AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/SheepVsWolves_VM/sheep_sheep%d.mp3", cocos2d::random(1, 4)));
	float dt = cocos2d::random(2.0f, 6.0f);
	this->scheduleOnce([=](float dt) {
		randomSheepSound();
	}, dt, "random_sound_sheep");
}

std::vector<cocos2d::Vec2> SheepVsWolves_VM::getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding, cocos2d::Vec2 skew)
{
	std::vector<cocos2d::Vec2> locations;
	int col = max_col;
	int row = total_element / max_col;
	if (row * col < total_element) ++row;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(mid_point.x - (col - 1) * (element_size.width + padding.x) * 0.5f - (row - 1) * skew.x / 2, mid_point.y - (row - 1) * (element_size.height + padding.y) / 2 - (col - 1) * skew.y / 2);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			cocos2d::Vec2 pos(start_pos.x + j * (element_size.width + padding.x) + i * skew.x, start_pos.y + i * (element_size.height + padding.y) + j * skew.y);
			locations.push_back(pos);
		}
	}
	return locations;
}

std::vector<std::string> SheepVsWolves_VM::splitString(const std::string & s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void SheepVsWolves_VM::autoPlay(float dt)
{
	this->schedule([=](float dt) {
		if (m_game_state == SVWGameState_VM::PLAYING_LEFT) {
			std::vector<SheepVsWolves_VM_Item*> sheeps_available;
			for (auto sheep : m_sheep_left_items) {
				if (sheep->isClickEnabled()) {
					sheeps_available.push_back(sheep);
				}
			}
			int ind = cocos2d::random(0, (int)sheeps_available.size() - 1);
			onClickSheepVsWolves_VMDelegate(sheeps_available[ind]);
		}
		else if (m_game_state == SVWGameState_VM::PLAYING_RIGHT) {
			std::vector<SheepVsWolves_VM_Item*> sheeps_available;
			for (auto sheep : m_sheep_right_items) {
				if (sheep->isClickEnabled()) {
					sheeps_available.push_back(sheep);
				}
			}
			int ind = cocos2d::random(0, (int)sheeps_available.size() - 1);
			onClickSheepVsWolves_VMDelegate(sheeps_available[ind]);
		}
	}, 3.0f, 10000000, dt, "auto play");
}
bool SheepVsWolves_VM::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    return false;
}
