//edited by Cuong Cao Nguyen 4/22/2022

#include "SheepVsWolves.h"
#include "HSAudioEngine.h"

#define GAME_NAME "SheepVsWolves"

#define IMAGE_BG "games/sheepvswolves/bg-01.png"
#define IMAGE_LOA "games/sheepvswolves/loa icon.png"

#define GAF_FENCES "gaf/sheepvswolves/sheep_fences/sheep_fences.gaf"
#define GAF_FADEIN "gaf/sheepvswolves/sheep_fadein/sheep_fadein.gaf"
#define GAF_EFFECT "gaf/sheepvswolves/sheep_effect/sheep_effect.gaf"
#define GAF_KICK "gaf/sheepvswolves/sheep_kick/sheep_kick.gaf"
#define GAF_LEG "gaf/sheepvswolves/sheep_leg/sheep_leg.gaf"
#define GAF_SHEEP "gaf/sheepvswolves/sheep_sheep/sheep_sheep.gaf"
#define GAF_WAREHOUSE "gaf/sheepvswolves/sheep_warehouse/sheep_warehouse.gaf"
#define GAF_STAR_YELLOW "gaf/sheepvswolves/stars_yellow/stars_yellow.gaf"

#define SOUND_BGM "sounds/sheepvswolves/sheep_bgm.mp3"
#define SOUND_BOARD_IN "sounds/sheepvswolves/sheep_board_in.mp3"
#define SOUND_BOARD_OUT "sounds/sheepvswolves/sheep_board_out.mp3"
#define SOUND_SHEEP_DOOR_CLOSE "sounds/sheepvswolves/sheep_door_close.mp3"
#define SOUND_SHEEP_DOOR_OPEN "sounds/sheepvswolves/sheep_door_open.mp3"
#define SOUND_INTRO "sounds/sheepvswolves/sheep_intro.mp3"
#define SOUND_INTRO1 "sounds/sheepvswolves/sheep_intro1.mp3"
#define SOUND_CTW "sounds/sheepvswolves/complete_the_word.mp3"
#define SOUND_RUN "sounds/sheepvswolves/sheep_run.mp3"
#define SOUND_RUN_CORRECT "sounds/sheepvswolves/sheep_run_correct.mp3"
#define SOUND_RUN_IN "sounds/sheepvswolves/sheep_run_in.mp3"
#define SOUND_RUN_WRONG "sounds/sheepvswolves/sheep_run_wrong.mp3"
#define SOUND_SHEEP_WALK "sounds/sheepvswolves/sheep_walk.mp3"
#define SOUND_SELECT "sounds/sheepvswolves/sheep_select.mp3"
#define SOUND_TEXT_MERGE "sounds/sheepvswolves/sheep_text_merge.mp3"
#define SOUND_SHEEP_TWINKLE_END "sounds/sheepvswolves/sheep_twinkle_end.mp3"
#define SOUND_SHEEP_WRONG "sounds/sheepvswolves/sheep_wrong.mp3"
#define SOUND_SHEEP_CORRECT "sounds/sheepvswolves/sheep_correct.mp3"

#define SHEEP_WALK_SPEED 150.0f
#define SHEEP_RUN_SPEED 250.0f

SheepVsWolves * SheepVsWolves::createGame(std::string json_path, bool is_auto_play)
{
	auto game = new SheepVsWolves();
	if (game && game->init()) {
		game->didLoadFromCSB(json_path, is_auto_play);
        game->autorelease();
		return game;
	}
	CC_SAFE_DELETE(game);
	return nullptr;
}

void SheepVsWolves::didLoadFromCSB(std::string json_path, bool is_auto_play)
{
	this->m_is_auto_play = is_auto_play;
	if (json_path.size() < 5) {
		json_path = "json/sheepvswolves/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_path);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());


	rapidjson::Value& game_config_data = document.FindMember("game_config")->value;

	rapidjson::Value& data_config = document.FindMember("data")->value;
	//    rapidjson::Value& game_data = data_config.GetArray()[0].FindMember("game")->value;
	this->generateData(data_config);
}

void SheepVsWolves::generateData(rapidjson::Value& value) {

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
		sheepvswolves::GAME_INFO game_info;

		auto letter = value[i].FindMember("text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		game_info.text_full = pImage->getText("name_1");
		game_info.n_audio = pImage->getAudios("name_1").front();
		game_info.p_audio = pImage->getAudios("name_n").front();
		game_info.board_info.text_full = game_info.text_full;
		game_info.board_info.audio_full = game_info.n_audio;
		game_info.board_info.image_path = pImage->path;

		rapidjson::Value& json_list_answer = value[i].FindMember("answer")->value;
		for (rapidjson::SizeType j = 0; j < json_list_answer.Size(); ++j) {
			rapidjson::Value& phonic_data = json_list_answer[j];
			auto answer_id = phonic_data.FindMember("item")->value.FindMember("text")->value.GetString();
			auto aImage = math::resource::ResHelper::getImage(images, answer_id);
			sheepvswolves::PHONIC_INFO distract_info;
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
			sheepvswolves::PHONIC_INFO phonic_info;
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

		// Gen sync text_info
		int start_index = 0;
		std::queue<int> underline_index;
		for (int i = 0; i < game_info.list_phonics.size(); ++i) {
			auto phonic_data = game_info.list_phonics[i];
			sheepvswolves::SYNC_TEXT_INFO synctext_info;
			synctext_info.text = phonic_data.text_phonic;
			synctext_info.audio = phonic_data.audio_phonic;
			for (int j = 0; j < phonic_data.text_phonic.size(); ++j) {
				if (phonic_data.text_phonic[j] == '_') {
					//underline_index
					underline_index.push(start_index + j);
				}
				else {
					if (underline_index.empty() || underline_index.front() > start_index) {
						synctext_info.list_index.push_back(start_index + j);
					}
					else {
						synctext_info.list_index.push_back(underline_index.front());
						underline_index.pop();
					}
				}
			}
			start_index += phonic_data.text_phonic.size();
			game_info.list_synctext_info.push_back(synctext_info);
		}
		m_game_info.push_back(game_info);
	}

	mj::helper::shuffle(m_game_info);
}

void SheepVsWolves::loadFrames()
{
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	if (screen_size.width > 1024) {
		_scaleImg = true;
	}
	else
	{
		_scaleImg = false;
	}
	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(screen_size);
	m_layout_root->setTouchEnabled(true);
	this->addChild(m_layout_root);


	m_layout_synctext = cocos2d::ui::Layout::create();
	m_layout_synctext->setContentSize(screen_size);
	this->addChild(m_layout_synctext, 100);

	if (FileUtils::getInstance()->isFileExist(IMAGE_BG)) {
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
	}

	m_board = SheepVsWolves_Board::createBoard(m_game_info[0].board_info);
	m_board->setVisible(false);
	m_board->setDelegate(this);
	m_board->setScale(1.1f);
	m_layout_root->addChild(m_board, 36);

	// farm layout
	m_fences_flipped = false;
	m_gaf_fences = GAFWrapper::createGAF(GAF_FENCES);
	m_layout_root->addChild(m_gaf_fences, 35);
	m_gaf_fences->playSequence("idle");
	m_gaf_fences->setVisible(false);
	m_gaf_fences->setOpacity(0);
	m_gaf_fences->setCascadeColorEnabled(true);

	m_gaf_warehouse_back = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_layout_root->addChild(m_gaf_warehouse_back, 10);
	m_gaf_warehouse_back->playSequence("back");
	m_gaf_warehouse_back->setVisible(false);
	m_gaf_warehouse_back->setOpacity(0);
	m_gaf_warehouse_back->setCascadeColorEnabled(true);

	m_gaf_warehouse_front = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_layout_root->addChild(m_gaf_warehouse_front, 33);
	m_gaf_warehouse_front->playSequence("front");
	m_gaf_warehouse_front->setVisible(false);
	m_gaf_warehouse_front->setOpacity(0);
	m_gaf_warehouse_front->setCascadeColorEnabled(true);

	m_gaf_warehouse_door = GAFWrapper::createGAF(GAF_WAREHOUSE);
	m_layout_root->addChild(m_gaf_warehouse_door, 34);
	m_gaf_warehouse_door->playSequence("close");
	m_gaf_warehouse_door->setVisible(false);
	m_gaf_warehouse_door->setOpacity(0);
	m_gaf_warehouse_door->setCascadeColorEnabled(true);

	if (_scaleImg) {
		m_board->setPosition(Vec2(screen_size.width * 0.55 - 140, screen_size.height + 300));
		m_gaf_fences->setPosition(Vec2(screen_size.width * 0.5 - 140, 155));
		m_gaf_warehouse_back->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.85));
		m_gaf_warehouse_front->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.85));
		m_gaf_warehouse_door->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.85));

	}
	else
	{
		m_board->setPosition(Vec2(screen_size.width * 0.55, screen_size.height + 300));
		m_gaf_fences->setPosition(Vec2(screen_size.width * 0.5 + 40, 155));
		m_gaf_warehouse_back->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
		m_gaf_warehouse_front->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
		m_gaf_warehouse_door->setPosition(Vec2(screen_size.width / 2 + 180, screen_size.height * 0.85));
	}

	PHONIC_MANAGER->cacheGAF("gaf_effect", GAF_EFFECT);
	PHONIC_MANAGER->cacheGAF("gaf_leg", GAF_LEG);

	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	_speaker->setTouchEnabled(false);
	_speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		AUDIO_ENGINE->stopEffect(_audioSheepWrong);
		_speaker->setTouchEnabled(false);
		AUDIO_ENGINE->playSound(SOUND_CTW, false, [=] {
			AUDIO_ENGINE->playSound(m_game_info[game_data_index].n_audio, false, [=] {
				stopLearningReminder();
				learningReminder();
				_speaker->setTouchEnabled(true);
				});
			});
		});
}

void SheepVsWolves::onEnter()
{
	itemOverLay();
	MJMainGBase::onEnter();
	this->loadFrames();
	initSheepItems();
	//showGameName();
	playBackgroundMusic(SOUND_BGM);
	
	this->scheduleOnce([=](float) {
		showIntroGame();
	}, 0, "delay_onenter");

}

void SheepVsWolves::onExitTransitionDidStart()
{
	PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}

void SheepVsWolves::update(float dt)
{
}

void SheepVsWolves::showGameName()
{
	m_game_state = SVWGameState::GAMENAME;
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

	auto img_gamename = cocos2d::ui::ImageView::create("games/sheepvswolves/sheep_fadein.png");
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

void SheepVsWolves::showIntroGame()
{
	m_game_state = SVWGameState::INTRO;
	AUDIO_ENGINE->playSound(SOUND_INTRO);
	//AUDIO_ENGINE->playSound(SOUND_INTRO1);
	if (_scaleImg) {
		_sheepChangeDistance = 180;
	}
	showFarm(0, [=] {
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
				if (_scaleImg) {
					m_sheep_items[i]->setPosition(locations[i] - Vec2(180, 0));
				}
				else
				{
					m_sheep_items[i]->setPosition(locations[i]);
				}
				m_sheep_items[i]->runAction(Sequence::create(DelayTime::create(1.5f), FadeIn::create(0.5f), NULL));
			}
			else {
				m_sheep_items[i]->setSheepFlipped(true);
				m_sheep_items[i]->setScale(0.75f);
				float distance1 = 150.0f;//m_sheep_items[i]->getPosition().getDistance(Vec2(screen_size.height / 2 + 125, screen_size.height * 0.65f));
				float distance2 = 250.0f;//Vec2(screen_size.height / 2 + 125, screen_size.height * 0.65f).getDistance(locations[i]);
				m_sheep_items[i]->playSheepSequence("walk", true);
				if (_scaleImg) {
					_sheepMove = -55;
				}
				m_sheep_items[i]->runAction(Sequence::create(DelayTime::create(dt),
					Spawn::createWithTwoActions(
						MoveTo::create(distance1 / SHEEP_RUN_SPEED, Vec2(screen_size.width / 2 + _sheepMove, screen_size.height * 0.65f)),
						ScaleTo::create(distance1 / SHEEP_RUN_SPEED, 0.85f)
					),
					CallFunc::create([=] {
					if (i < m_sheep_items.size() / 2) {
						m_sheep_items[i]->setSheepFlipped(false);
					}
				}),
					Spawn::createWithTwoActions(
					MoveTo::create(distance2 / SHEEP_WALK_SPEED, locations[i] - Vec2(_sheepChangeDistance, 0)),
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
			EaseBackOut::create(MoveTo::create(0.5f, Vec2(screen_size.width * 0.55 - _sheepChangeDistance, screen_size.height * 0.73f))),
			DelayTime::create(0.5f),
			CallFunc::create([=] {
			AUDIO_ENGINE->playSound(SOUND_SHEEP_DOOR_CLOSE);
			m_gaf_warehouse_door->playSequence("close");
			startGame();
		}), NULL));
	});
	// sheep
}

void SheepVsWolves::startGame()
{
	m_game_state = SVWGameState::START;
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

	sound_chain.push_back(SOUND_CTW);
	sound_chain.push_back(m_game_info[game_data_index].n_audio);
	AUDIO_ENGINE->playChain(sound_chain);
	this->scheduleOnce([=](float dt) {
		randomSheepSound();
		if (m_sheep_missing_left > 0) {
			onChangeSide(true);
		}
		else {
			onChangeSide(false);
		}
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			_speaker->setTouchEnabled(true);
			removeItemOverLay();
			}), NULL));
		learningReminder();
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

void SheepVsWolves::onNextSide()
{
	if (m_game_state == SVWGameState::PLAYING_LEFT) {
		//--m_sheep_missing_left;
		for (auto sheep : m_sheep_left_items) {
			sheep->setEnableClick(false);
			sheep->runAction(Sequence::create(DelayTime::create(0.75f), FadeOut::create(0.5f), NULL));
		}
		if (m_sheep_missing_left <= 0) {
			if (m_sheep_missing_right > 0) {
				for (auto sheep : m_sheep_left_items) {
					if (sheep->getSheepType() == SheepVsWolvesType::WOLVES && !sheep->isRunning()) {
						sheep->playSheepSequence("bark", true);
					}
				}
				onShowSheepFromRightSide([=] {
					onChangeSide(false);
					removeItemOverLay();
				});
			}
			else {
				onTransitionSyncText(m_game_info[game_data_index], Vec2(screen_size.width / 2, screen_size.height *0.12), [=] {
					onNextGame();
				});

			}
		}
	}
	else if (m_game_state == SVWGameState::PLAYING_RIGHT) {
		//--m_sheep_missing_right;
		if (m_sheep_missing_right <= 0) {
			if (m_sheep_missing_left > 0) {
				for (auto sheep : m_sheep_right_items) {
					if (sheep->getSheepType() == SheepVsWolvesType::WOLVES && !sheep->isRunning()) {
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

void SheepVsWolves::onChangeSide(bool left_side)
{
	if (left_side) {
		//m_layout_root->setPosition(Vec2(0, 0));
		m_game_state = SVWGameState::PLAYING_LEFT;
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
			m_sheep_left_items[i]->setPosition(left_locations[i] - Vec2(_sheepChangeDistance, 0));
		}
		// sheeps on right side
		auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 725, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(0, 0), Vec2(-125, 0));
		for (int i = 0; i < m_sheep_right_items.size(); ++i) {
			m_sheep_right_items[i]->setPosition(right_locations[i]);
		}
	}
	else {
		//m_layout_root->setPosition(Vec2(0, 0));
		m_game_state = SVWGameState::PLAYING_RIGHT;
		for (auto sheep : m_sheep_left_items) {
			sheep->setEnableClick(false);
		}
		for (auto sheep : m_sheep_right_items) {
			sheep->setEnableClick(true);
		}
		m_layout_root->stopAllActions();
		if (_scaleImg) {
			m_layout_root->runAction(MoveTo::create(0.5f, Vec2(100.0f, 0)));
		}
		else
		{
			m_layout_root->runAction(MoveTo::create(0.5f, Vec2(-200.0f, 0)));
		}
		setFencesSide(true);
		// sheeps on left side
		auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 750, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(0, 0), Vec2(125, 0));
		for (int i = 0; i < m_sheep_left_items.size(); ++i) {
			m_sheep_left_items[i]->setPosition(left_locations[i]);
		}
		// sheeps on right side
		auto right_locations = getFrameLocations(Vec2(screen_size.width / 2 + 575, 0.375 * screen_size.height), cocos2d::Size(125, 125), m_sheep_right_items.size(), 1, Vec2(10, 0), Vec2(-40, 0));
		for (int i = 0; i < m_sheep_right_items.size(); ++i) {
			m_sheep_right_items[i]->setPosition(right_locations[i] - Vec2(_sheepChangeDistance, 0));
		}
	}
}

void SheepVsWolves::onTransition()
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
	}), EaseBackOut::create(MoveTo::create(0.5f, Vec2(screen_size.width * 0.55 - _sheepChangeDistance, screen_size.height * 0.73f))),
		CallFunc::create([=] {
		startGame();
	}), NULL));
}

void SheepVsWolves::onNextGame()
{
	if (game_data_index < m_game_info.size() - 1) {
		++game_data_index;
		onTransition();
	}
	else {
		onFinishGame();
	}
}

void SheepVsWolves::onFinishGame()
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
			MoveTo::create(distance / SHEEP_WALK_SPEED, Vec2(screen_size.width / 2 + _sheepMove, screen_size.height * 0.6f)),
			MoveTo::create(150 / SHEEP_WALK_SPEED, Vec2(screen_size.width / 2 + _sheepMove, screen_size.height * 0.75f)), NULL));
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

void SheepVsWolves::onRefresh()
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
		m_sheep_items[i]->setPosition(locations[i] - Vec2(_sheepChangeDistance, 0));
		m_sheep_items[i]->setOpacity(255);
		m_sheep_items[i]->setVisible(true);
	}
}

void SheepVsWolves::learningReminder() {
	_speaker->setTouchEnabled(true);
	this->schedule([=](float) {
		std::vector<std::string> sound_chain;
		sound_chain.push_back(SOUND_CTW);
		sound_chain.push_back(m_game_info[game_data_index].n_audio);
		AUDIO_ENGINE->playChain(sound_chain);
		}, 13, 100, 10.5, "learning_reminder");
}

void SheepVsWolves::stopLearningReminder() {
	AUDIO_ENGINE->stopSound(SOUND_CTW);
	AUDIO_ENGINE->stopSound(m_game_info[game_data_index].n_audio);
	this->unschedule("learning_reminder");
}

void SheepVsWolves::onTransitionSyncText(sheepvswolves::GAME_INFO info, cocos2d::Vec2 pos, std::function<void()> call_back)
{
	m_game_state = SVWGameState::SYNC_TEXT;
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
			for (int j = 0; j < phonic_info.text_phonic.size(); ++j) {
				if (phonic_info.text_phonic[j] != '_') {
					auto sub = phonic_info.text_phonic.substr(j,1);
					auto text = cocos2d::ui::Text::create(sub, font_name, 100);
					//text->enableOutline(cocos2d::Color4B(81, 60, 51, 255), 1);
					text->setColor(cocos2d::Color3B::BLACK);
					text->setPosition(m_layout_root->convertToWorldSpace(locations[j]));
					m_layout_synctext->addChild(text, 10001);
					text->runAction(Sequence::create(DelayTime::create(total_time - dt), MoveTo::create(0.5f, move_locations[list_index[ind]]), CallFunc::create([=] {
						text->removeFromParent();
					}), NULL));
					++ind;
				}
			}
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

void SheepVsWolves::readSyncText(sheepvswolves::GAME_INFO info, cocos2d::Vec2 pos, std::function<void()> call_back)
{

	auto lbl_synctext = cocos2d::ui::Text::create(info.text_full, font_name, 120);
	lbl_synctext->setPosition(pos);
	lbl_synctext->setColor(Color3B::BLACK);
	m_layout_synctext->addChild(lbl_synctext, 1000000, "lbl_synctext");
	float dt = 0.25;
	for (int i = 0; i < info.list_synctext_info.size(); ++i) {
		m_layout_synctext->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->play(info.list_synctext_info[i].audio);
			for (int index : info.list_synctext_info[i].list_index) {
				lbl_synctext->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					lbl_synctext->getLetter(index)->setColor(Color3B(250, 54, 208));
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					lbl_synctext->getLetter(index)->setColor(Color3B::BLACK);
				}), NULL));
			}
		}), NULL));
		dt += 0.65f;
	}
	auto lbl_synctext1 = cocos2d::ui::Text::create(info.text_full, font_name, 120);
	lbl_synctext1->setPosition(pos);
	lbl_synctext1->setVisible(false);
	m_layout_synctext->addChild(lbl_synctext1, 1000000, "lbl_synctext1");
	lbl_synctext1->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
		lbl_synctext->removeFromParent();
		lbl_synctext1->setVisible(true);
		AUDIO_ENGINE->play(info.n_audio);
		lbl_synctext1->setTextColor(Color4B(250, 54, 208, 255));
	}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
		lbl_synctext1->setTextColor(Color4B::BLACK);
	}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), FadeOut::create(0.2f), CallFunc::create([=] {
		lbl_synctext1->removeFromParent();
	}), NULL));
	dt += 0.8f;
	scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, dt, "synctext call_back");
}

void SheepVsWolves::initSheepItems()
{
	m_sheep_missing_left = 0;
	m_sheep_missing_right = 0;
	m_sheep_items.clear();
	m_sheep_left_items.clear();
	m_sheep_right_items.clear();
	// sheep item
//	auto locations = getFrameLocations(Vec2(0.55* screen_size.width,0.35 * screen_size.height), cocos2d::Size(125, 125), m_game_info[0].list_phonics.size(), m_game_info[0].list_phonics.size(), Vec2(10, 0));
	std::vector<sheepvswolves::PHONIC_INFO> list_phonic(m_game_info[game_data_index].list_phonics);
	for (int i = 0; i < list_phonic.size(); ++i) {
		auto phonic = list_phonic[i];
		if (!phonic.missing && phonic.text_phonic.find_first_of('_') != std::string::npos) {
			auto list_char = splitString(phonic.text_phonic, '_');
			list_phonic[i].text_phonic = list_char[0];
			sheepvswolves::PHONIC_INFO info;
			info.text_phonic = list_char[1];
			info.missing = false;
			list_phonic.insert(list_phonic.begin() + i + 2, info);
		}
	}
	for (int i = 0; i < list_phonic.size(); ++i) {
		auto sheep = SheepVsWolves_Item::createItem(list_phonic[i], SheepVsWolvesType::SHEEP_ONFARM);
		if (_scaleImg) {
			sheep->setPosition(Vec2(screen_size.width / 2 - 15, screen_size.height * 0.75));
		}
		else
		{
			sheep->setPosition(Vec2(screen_size.width / 2 + 125, screen_size.height * 0.75));
		}
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
			auto sheep = SheepVsWolves_Item::createItem(info);
			sheep->setCascadeOpacityEnabled(true);
			sheep->setVisible(false);
			sheep->setDelegate(this);
			m_sheep_left_items.push_back(sheep);
			m_layout_root->addChild(sheep, 101);
		}
	}
	mj::helper::shuffle(m_game_info[game_data_index].list_distracting);
	for (auto distract_info : m_game_info[game_data_index].list_distracting) {
		auto sheep = SheepVsWolves_Item::createItem(distract_info, SheepVsWolvesType::WOLVES);
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
			auto sheep = SheepVsWolves_Item::createItem(info);
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
		auto sheep = SheepVsWolves_Item::createItem(distract_info, SheepVsWolvesType::WOLVES);
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

void SheepVsWolves::onShowSheepFromLeftSide(std::function<void()> call_back)
{
	//m_game_state = SVWGameState::PLAYING_LEFT;
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
		m_sheep_left_items[i]->runAction(cocos2d::Sequence::create(MoveTo::create(600 / SHEEP_RUN_SPEED, Vec2(left_locations[i].x - _sheepChangeDistance, left_locations[i].y)),
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

void SheepVsWolves::onShowSheepFromRightSide(std::function<void()> call_back)
{
	//m_game_state = SVWGameState::PLAYING_RIGHT;
	setFencesSide(true);
	// sheeps on left side
	auto left_locations = getFrameLocations(Vec2(screen_size.width / 2 - 750, 0.4 * screen_size.height), cocos2d::Size(125, 125), m_sheep_left_items.size(), 1, Vec2(0, 0), Vec2(125, 0));
	for (int i = 0; i < m_sheep_left_items.size(); ++i) {
		m_sheep_left_items[i]->setPosition(left_locations[i]);
	}
	// set postiton
	m_layout_root->stopAllActions();
	if (_scaleImg) {
		m_layout_root->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(-600, 0)), MoveTo::create(600 / SHEEP_RUN_SPEED, Vec2(0, 0)), NULL));
	}
	else
	{
		m_layout_root->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(-800, 0)), MoveTo::create(600 / SHEEP_RUN_SPEED, Vec2(-200, 0)), NULL));
	}
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
		m_sheep_right_items[i]->runAction(cocos2d::Sequence::create(MoveTo::create(600 / SHEEP_RUN_SPEED, right_locations[i] - Vec2(_sheepChangeDistance, 0)),
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

void SheepVsWolves::onClickSheepVsWolvesDelegate(cocos2d::Ref * sender)
{
	itemOverLay();
	if (_board) {
		_board->setClickedBoardEnabled(true);
	}
	if (_audioSheep > 0) {
		AUDIO_ENGINE->stopEffect(_audioSheep);
	}
	stopLearningReminder();
	auto sheep = static_cast<SheepVsWolves_Item*>(sender);
	if (sheep->getSheepType() == SheepVsWolvesType::SHEEP_ONFARM) {
		if (sheep->getInfo().missing == false) {
			_speaker->setTouchEnabled(true);
			AUDIO_ENGINE->stopSound(sheep->getInfo().audio_phonic);
			_audioSheep = AUDIO_ENGINE->playSound(sheep->getInfo().audio_phonic);
		}
		removeItemOverLay();
		return;
	}
	sheep->setEnableClick(false);
	_speaker->setTouchEnabled(true);
	SheepVsWolves_Item* sheep_correct = nullptr;
	AUDIO_ENGINE->playSound(SOUND_SELECT);
	AUDIO_ENGINE->stopSound(sheep->getInfo().audio_phonic);
	_audioSheepWrong = AUDIO_ENGINE->playSound(sheep->getInfo().audio_phonic);
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
		if (m_game_state == SVWGameState::PLAYING_LEFT) {
			--m_sheep_missing_left;
		}
		else if (m_game_state == SVWGameState::PLAYING_RIGHT) {
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
		_flag = false;
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
				gaf_star_yellow->play(false, [=](gaf::GAFObject* obj) {
					gaf_star_yellow->removeFromParent();
					});
				sheep_correct->setSheepMissing(false);
				sheep->setVisible(false);
				}), NULL));
		this->scheduleOnce([=](float dt) {
			onNextSide();
			}, time_run + 0.5f, "next_side");
	}
	else {
		if (sheep->getPositionX() < screen_size.width / 2 && m_sheep_missing_left <= 0) {
			sheep->playSheepSequence("bark", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
				sheep->playSheepSequence("stand_idle", true);
				});
			return;
		}
		else if (sheep->getPositionX() > screen_size.width / 2 && m_sheep_missing_right <= 0) {
			sheep->playSheepSequence("bark", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
				sheep->playSheepSequence("stand_idle", true);
				});
			return;
		}
		sheep->playSheepSequence("wjump", true);

		AUDIO_ENGINE->playSound(SOUND_RUN_WRONG);
		sheep->runAction(
			// sheep
			Sequence::create(EaseQuadraticActionIn::create(MoveTo::create(1.5f, pos_jump)), CallFunc::create([=] {
				sheep->getLableInfo()->setOpacity(0);
				sheep->runAction(Sequence::create(EaseQuadraticActionOut::create(MoveTo::create(22.0f / 32, start_pos)), NULL));
				AUDIO_ENGINE->play(SOUND_SHEEP_WRONG);
				sheep->playSheepSequence("kicked", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
					sheep->playSheepSequence("hit", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
						sheep->playSheepSequence("bark", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
							sheep->playSheepSequence("stand_idle");
							});
						sheep->getLableInfo()->setOpacity(255);
						sheep->setEnableClick(true);
						removeItemOverLay();
						stopLearningReminder();
						learningReminder();
						});
					});
				auto gaf_effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["gaf_effect"]);
				gaf_effect->setPosition(Vec2(start_pos.x - 16, start_pos.y));
				gaf_effect->setVisible(false);
				m_layout_root->addChild(gaf_effect, 1000);
				gaf_effect->runAction(Sequence::create(DelayTime::create(22.0f / 32), CallFunc::create([=] {
					gaf_effect->setVisible(true);
					gaf_effect->playSequence("hit", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
						gaf_effect->removeFromParent();
						});
					}), NULL));
				}), NULL));
	}
}

void SheepVsWolves::onClickedBoardDelegate(cocos2d::Ref * sender)
{
	AUDIO_ENGINE->stopEffect(_audioSheep);
	stopLearningReminder();
	learningReminder();
	_board = static_cast<SheepVsWolves_Board *> (sender);
	_board->setClickedBoardEnabled(false);
	std::vector<std::string> sound_chain;
	for (auto info : m_game_info) {
		if (_board->getInfo().text_full == info.text_full) {
			AUDIO_ENGINE->stopSound(info.n_audio);
			AUDIO_ENGINE->playSound(info.n_audio, false, [=] {
				_board->setClickedBoardEnabled(true);
			});
			break;
		}
	}
}

void SheepVsWolves::showFarm(float dt, std::function<void()> call_back)
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

void SheepVsWolves::hideFarm(float dt, std::function<void()> call_back)
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

void SheepVsWolves::setFencesSide(bool flipped, bool transition)
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

void SheepVsWolves::randomSheepSound()
{
	AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/sheepvswolves/sheep_sheep%d.mp3", cocos2d::random(1, 4)));
	float dt = cocos2d::random(2.0f, 6.0f);
	this->scheduleOnce([=](float dt) {
		randomSheepSound();
	}, dt, "random_sound_sheep");
}

std::vector<cocos2d::Vec2> SheepVsWolves::getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding, cocos2d::Vec2 skew)
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

std::vector<std::string> SheepVsWolves::splitString(const std::string & s, char delimiter)
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

void SheepVsWolves::autoPlay(float dt)
{
	this->schedule([=](float dt) {
		if (m_game_state == SVWGameState::PLAYING_LEFT) {
			std::vector<SheepVsWolves_Item*> sheeps_available;
			for (auto sheep : m_sheep_left_items) {
				if (sheep->isClickEnabled()) {
					sheeps_available.push_back(sheep);
				}
			}
			int ind = cocos2d::random(0, (int)sheeps_available.size() - 1);
			onClickSheepVsWolvesDelegate(sheeps_available[ind]);
		}
		else if (m_game_state == SVWGameState::PLAYING_RIGHT) {
			std::vector<SheepVsWolves_Item*> sheeps_available;
			for (auto sheep : m_sheep_right_items) {
				if (sheep->isClickEnabled()) {
					sheeps_available.push_back(sheep);
				}
			}
			int ind = cocos2d::random(0, (int)sheeps_available.size() - 1);
			onClickSheepVsWolvesDelegate(sheeps_available[ind]);
		}
	}, 3.0f, 10000000, dt, "auto play");
}

void SheepVsWolves::itemOverLay() {
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

void SheepVsWolves::removeItemOverLay() {
	if (cocos2d::utils::findChild(this, "overlay")) {
		this->removeChildByName("overlay");
	}
}
