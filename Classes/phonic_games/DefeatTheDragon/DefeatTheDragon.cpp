
#include "DefeatTheDragon.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "MSEventLogger.h"
INITIALIZE_READER(DefeatTheDragon);
#define speed_opacity 15

std::once_flag DefeatTheDragon_reader;
#define CSB_NAME "csb/game/DefeatTheDragon/DefeatTheDragon.csb"
#define _game_path "json/DefeatTheDragon/"
//gaf
#define bg_gaf_path "gaf/defeatthedragon/DtD_bg2/DtD_bg2.gaf"
#define knight_gaf_path "gaf/defeatthedragon/DtD_Knight1.3/DtD_Knight1.3.gaf"
#define knight_effect_gaf_path "gaf/defeatthedragon/DtD_Keffects/DtD_Keffects.gaf"
#define dragon_gaf_path "gaf/defeatthedragon/DtD_Dragon1.3/DtD_Dragon1.3.gaf"
#define dragon_effect_gaf_path "gaf/defeatthedragon/DtD_Deffects/DtD_Deffects.gaf"
#define fire_large_gaf_path "gaf/defeatthedragon/DtD_firelarge/DtD_firelarge.gaf"
#define bg_win_gaf_path "gaf/defeatthedragon/DtD_bg_win/DtD_bg_win.gaf"
//sounds
#define bg_music "sounds/defeatthedragon/DtD_bgm.mp3"
#define fade_in_music "sounds/defeatthedragon/DtD_fade_in.mp3"
#define d_word_show_music "sounds/defeatthedragon/DtD_D_word_show.mp3"
#define d_word_more_music "sounds/defeatthedragon/DtD_D_word_more.mp3"
#define d_laugh_music "sounds/defeatthedragon/DtD_D_laugh.mp3"
#define split_text_music "sounds/defeatthedragon/DtD_F_word_split-enlarge.mp3"
#define d_fly_music "sounds/defeatthedragon/DtD_D_fly.mp3"
#define correct_music "sounds/defeatthedragon/DtD_F_correct.mp3"
#define wrong_music "sounds/defeatthedragon/DtD_F_wrong.mp3"
#define k_hit_music "sounds/defeatthedragon/DtD_K_hit.mp3"
#define k_correct_music "sounds/defeatthedragon/DtD_K_correct.mp3"
#define k_attack_normal "sounds/defeatthedragon/DtD_K_attack_merge.mp3"
#define k_attack_last "sounds/defeatthedragon/DtD_K_attack_merge_last.mp3"
#define k_victory "sounds/defeatthedragon/DtD_D_victory.mp3"
#define d_attack1 "sounds/defeatthedragon/DtD_D_attack2s_1.mp3"
#define d_attack2 "sounds/defeatthedragon/DtD_D_attack2s_2.mp3"
#define d_attack_last "sounds/defeatthedragon/DtD_D_attack2s_last.mp3"
#define d_action1 "sounds/defeatthedragon/DtD_D_action1.mp3"
#define k_face_wipe "sounds/defeatthedragon/DtD_K_face_wipe.mp3"
#define intro_music "sounds/defeatthedragon/DtD_intro.mp3"
#define breath_music "sounds/defeatthedragon/DtD_D_breath.mp3"
#define question_music "sounds/defeatthedragon/_find the sound.mp3"

#define audio_path "sample_game/sounds/DefeatTheDragon/"
#define img_prefix "games/DefeatTheDragon/"

#define BG_GAME "games/defeatthedragon/dragon.png"
#define audio_game_name "sounds/defeatthedragon/Tickling_the_Dragon.mp3"




DefeatTheDragon * DefeatTheDragon::createGame(std::string json_file, bool isAutoPlay) {
	std::call_once(DefeatTheDragon_reader, [] {
		REGISTER_CSB_READER(DefeatTheDragon);
		REGISTER_CSB_READER(DefeatTheDragon_Item);
	});
	auto p = static_cast<DefeatTheDragon *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void DefeatTheDragon::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	number_words = document.FindMember("game_mode")->value.GetInt();

	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());

	if (number_words > data.size()) {
		number_words = (int)data.size();
	}
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;
	this->loadFrames();
}

void DefeatTheDragon::generateData(rapidjson::Value value_data) {
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {

		auto letter = value_data[i].FindMember("full_text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		auto p_list = pImage->getTextPhonic("name_p");
		auto p_audio = pImage->getAudioPhonic("name_p");

		auto p = value_data[i].FindMember("items")->value.FindMember("text")->value.GetString();
		auto p_img = math::resource::ResHelper::getImage(images, p);
		auto right_text = p_img->getText("name_1");

		Word w;
		w.full_text = pImage->getText("name_1");
		w.audio_full_text = pImage->getAudios("name_1").front();
		for (int i = 0; i < p_list.size(); i++) {
			Phonic phonic;
			phonic.text = p_list[i];
			phonic.audio = p_audio[i];
			if (phonic.text == right_text) {
//                if (phonic.audio == p_img->getAudios().front()) {
				phonic.is_right = true;
			}
			w.phonics.push_back(phonic);
		}

		data.push_back(w);

	}
	mj::helper::shuffle(data);
}


DefeatTheDragon * DefeatTheDragon::createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay, std::string game_type) {
	std::call_once(DefeatTheDragon_reader, [] {
		REGISTER_CSB_READER(DefeatTheDragon);
		REGISTER_CSB_READER(DefeatTheDragon_Item);
	});
	auto p = static_cast<DefeatTheDragon *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(list_PHONIC_INFO, game_type, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void DefeatTheDragon::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;
	this->loadFrames();
}

void DefeatTheDragon::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);

	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

	}), NULL));

	if (autoPlay) {
		this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=] {
			this->onAutoPlay();
		}), NULL));

	}
}

void DefeatTheDragon::loadFrames() {
	mj::helper::shuffle(data);
	x_layout = utils::findChild<Layout*>(this, "x_panel");
	x_layout->setScale(screen_size.width / (3326 / 2) + screen_size.width / 10000);


	//root layout
	root_layout = utils::findChild<Layout*>(this, "root_panel");
	game_name = utils::findChild<ui::Text*>(this, "game_name");
	game_name->setZOrder(1000);
	game_name->setOpacity(0);
	//game_name->runAction(FadeIn::create(0.1f));

	Layout* sound_button = utils::findChild<Layout*>(this, "sound");
	sound_button->setZOrder(1000);
	sound_button->setPosition(root_layout->convertToNodeSpace(Vec2(40, screen_size.height - 40)));

	background_gaf = GAFWrapper::createGAF(bg_gaf_path);
	root_layout->addChild(background_gaf, 2);
	background_gaf->setScale(1.f);
	background_gaf->setName("background");
	background_gaf->setPosition(screen_size / 2);
	background_gaf->setPositionX(background_gaf->getPositionX() - 50);

	dragon_gaf = GAFWrapper::createGAF(dragon_gaf_path);
	dragon_gaf->setScale(0.8);
	dragon_gaf->setAnchorPoint(Vec2(0.5, 0));
	dragon_gaf->setPosition(Vec2(screen_size.width / 2 + 350, 0));
	root_layout->addChild(dragon_gaf, 2);


	dragon_effect_gaf = GAFWrapper::createGAF(dragon_effect_gaf_path);
	dragon_effect_gaf->setScale(0.8);
	dragon_effect_gaf->setAnchorPoint(Vec2(0.5, 0));
	dragon_effect_gaf->setPosition(dragon_gaf->getPosition());
	root_layout->addChild(dragon_effect_gaf, 2);


	bg_win_gaf = GAFWrapper::createGAF(bg_win_gaf_path);
	bg_win_gaf->setAnchorPoint(Vec2(0.5, 0.5));

	root_layout->addChild(bg_win_gaf, 2);
	bg_win_gaf->setVisible(false);

	knight_gaf = GAFWrapper::createGAF(knight_gaf_path);
	knight_gaf->setScale(0.8);
	knight_gaf->setAnchorPoint(Vec2(0.5, 0));
	knight_gaf->setPosition(Vec2(screen_size.width / 2 - 80, -screen_size.height * 0.24));
	bg_win_gaf->setPosition(Vec2(knight_gaf->getPositionX() - 400, knight_gaf->getPositionY() + 500));
	root_layout->addChild(knight_gaf, 2);

	knight_effect_gaf = GAFWrapper::createGAF(knight_effect_gaf_path);
	knight_effect_gaf->setScale(0.8);
	knight_effect_gaf->setAnchorPoint(Vec2(0.5, 0));
	knight_effect_gaf->setPosition(knight_gaf->getPosition());
	root_layout->addChild(knight_effect_gaf, 2);

	fire_large_gaf = GAFWrapper::createGAF(fire_large_gaf_path);
	fire_large_gaf->setAnchorPoint(Vec2(0.5, 0.5));
	fire_large_gaf->setScale(0.8);
	fire_large_gaf->setPosition(Vec2(dragon_gaf->getPositionX() - 350, screen_size.height*0.75));
	root_layout->addChild(fire_large_gaf, 2);
	fire_large_gaf->setVisible(false);

	fire_small_gaf = GAFWrapper::createGAF("gaf/defeatthedragon/DtD_firesmall/DtD_firesmall.gaf");
	fire_small_gaf->setAnchorPoint(Vec2(0.5, 0.5));
	root_layout->addChild(fire_small_gaf, 2);
	fire_small_gaf->setVisible(false);

	look_sound_button = true;
	current_index = 0;
	check_dragon_idle = false;

	if (FileUtils::getInstance()->isFileExist(bg_gaf_path)) {
		_background_gaf = GAFWrapper::createGAF(bg_gaf_path);
		root_layout->addChild(_background_gaf, INT_MAX - 1);
		_background_gaf->setScale(1.f);
		_background_gaf->setPosition(screen_size / 2);
		_background_gaf->setPositionX(_background_gaf->getPositionX() - 50);
	}
	

	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (FileUtils::getInstance()->isFileExist(BG_GAME)) {
		_backgoundGame = Sprite::create(BG_GAME);
		_backgoundGame->setPosition(Vec2(visibleSize / 2));
		_backgoundGame->setName("bg_game");
		this->addChild(_backgoundGame, INT_MAX - 1);
	}

	this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(audio_game_name);
		}), NULL));

	this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
		if (_backgoundGame) {
			_backgoundGame->runAction(FadeOut::create(1.0f));
		}
		}), DelayTime::create(1.5f), CallFunc::create([=] {
			if (_background_gaf) {
				_background_gaf->removeFromParent();
			}
			this->showGame();
		}), NULL));
}

void DefeatTheDragon::showGame() {

	AUDIO_ENGINE->playBackgroundMusic(bg_music, 0.8);
	AUDIO_ENGINE->play(fade_in_music);
	AUDIO_ENGINE->play(intro_music, false, 1.0f, [=] {
		AUDIO_ENGINE->play(d_word_show_music);
		dragon_gaf->playSequence("word_show", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			dragon_gaf->playSequence("idle", true);
			// bat dau choi
			this->dragonShowText(current_index);
		});
	});
	background_gaf->play(true);
	dragon_gaf->playSequence("fade_in", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		//game_name->runAction(FadeOut::create(0.5f));
		dragon_gaf->playSequence("idle", true);

	});
	dragon_effect_gaf->playSequence("idle", true);
	knight_gaf->playSequence("fade_in", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		knight_gaf->playSequence("idle", true);
	});
	knight_effect_gaf->playSequence("idle", true);
}

void DefeatTheDragon::dragonShowText(int index) {
	fire_large_gaf->setScale(1.2);
	// tao text full de scale
	text_full = Text::create();
	text_full->setString(data[current_index].full_text);
	text_full->setFontName(font_name);
	text_full->setFontSize(font_sz);
	text_full->setColor(Color3B::BLACK);

	text_full->setOpacity(0);
	text_full->setPosition(fire_large_gaf->getPosition());
	root_layout->addChild(text_full, 3);

	// tao cac Text rieng va sap xep
	pos1.clear();
	pos2.clear();
	items.clear();
	float width_text = 0;
	for (int i = 0; i < data[index].phonics.size(); i++) {
		Text* t = Text::create();
		t->setString(data[index].phonics[i].text);
		t->setFontName(font_name);
		t->setFontSize(font_sz);
		t->setTextColor(Color4B::BLACK);
		DefeatTheDragon_Item* item = DefeatTheDragon_Item::createItem(t, data[index].phonics[i].is_right, i);
        
        if(data[index].phonics[i].is_right)
        {
            _pathAudioCorrect = data[index].phonics[i].audio;
        }
		root_layout->addChild(item, 2);
		width_text += t->getContentSize().width;
		item->setVisible(false);
		item->setDelegate(this);
		item->lockTouch();
		items.push_back(item);
	}
	float origin_x = fire_large_gaf->getPositionX() - width_text / 2;
	for (int i = 0; i < items.size(); i++) {
		items[i]->setAnchorPoint(Vec2(0.5, 0.5));
		items[i]->setPositionX(origin_x + items[i]->getText()->getContentSize().width / 2);
		origin_x += items[i]->getText()->getContentSize().width;
		items[i]->setPositionY(fire_large_gaf->getPositionY());
		pos1.push_back(items[i]->getPosition());
	}

	// tim vi tri moi cua cac fire small
	origin_x = fire_large_gaf->getPositionX() - ((items.size() - 1)*1.0 / 2) * 150 - 50;
	auto world_space = root_layout->convertToWorldSpace(Vec2(origin_x,0));
	CCLOG("DTD origin first: %f world  pos: %f", origin_x, world_space.x);

    if(world_space.x < 75) origin_x = origin_x + 75 - world_space.x;
	CCLOG("DTD origin second: %f", origin_x);
	for (int i = 0; i < items.size(); i++) {
		pos2.push_back(Vec2(origin_x + i * 150, fire_large_gaf->getPositionY()));
	}

	// show
	AUDIO_ENGINE->play(d_word_more_music);
	this->scheduleOnce([=](float dt) {
		AUDIO_ENGINE->play(data[current_index].audio_full_text);
	}, 0.5f, cocos2d::StringUtils::format("audio"));
	dragon_gaf->playSequence("word_more", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		dragon_gaf->playSequence("idle", true);
		// gan check de kiem tra idle
		check_dragon_idle = true;
		this->checkIdle();

		fire_large_gaf->play(true);
		fire_large_gaf->setVisible(true);
		text_full->runAction(FadeIn::create(0.2f));

		this->scheduleOnce([=](float dt) {
			for (DefeatTheDragon_Item* i : items) {
				i->setVisible(true);
			}
			AUDIO_ENGINE->play(split_text_music);
			for (int i = 0; i < items.size(); i++) {
				items[i]->runAction(MoveTo::create(0.5f, pos2[i]));
				items[i]->showFire();
				fire_large_gaf->setVisible(false);
				text_full->setVisible(false);
			}
		}, 1.f, cocos2d::StringUtils::format("tach cac chu"));

		this->scheduleOnce([=](float dt) {
			for (DefeatTheDragon_Item* item : items) {
				item->unLockTouch();
			}
            id_audio_question = cocos2d::experimental::AudioEngine::play2d(question_music);
			cocos2d::experimental::AudioEngine::setFinishCallback(id_audio_question, [=](int au_id, std::string path) {
                id_audio_question = AUDIO_ENGINE->play(_pathAudioCorrect);
				this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
					look_sound_button = false;
					showHint();
					}), NULL));
			});
		}, 1.5f, cocos2d::StringUtils::format("mo khoa cham"));

	});

}

void DefeatTheDragon::dragonFly() {
	AUDIO_ENGINE->play(d_fly_music);
	dragon_gaf->playSequence("fly", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		dragon_gaf->playSequence("idle", true);
	});
	dragon_effect_gaf->playSequence("fly", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		dragon_effect_gaf->playSequence("idle", true);
	});
}

void DefeatTheDragon::dragonBreathe() {
	AUDIO_ENGINE->play(breath_music);
	dragon_gaf->playSequence("breathe", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		dragon_gaf->playSequence("idle", true);
	});
	this->scheduleOnce([=](float dt) {
		dragon_effect_gaf->playSequence("breath", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			dragon_effect_gaf->playSequence("idle", true);

		});
	}, 0.3f, cocos2d::StringUtils::format("mo khoa cham"));
}

void DefeatTheDragon::dragonFallAndGU() {
	dragon_gaf->playSequence("fall", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		dragon_gaf->playSequence("laugh_2", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			dragon_gaf->playSequence("getting_up", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				dragon_gaf->playSequence("idle", true);

			});
		});
	});
}


void DefeatTheDragon::attackDragonNormal() {
	int key = random(1, 1);
	Vec2 knight_pos_old = knight_gaf->getPosition();
	std::string s = random(0, 1) ? "attack1" : random(0, 1) ? "attack2" : "attack3";
	AUDIO_ENGINE->play(k_correct_music);
	knight_gaf->playSequence("correct", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		this->scheduleOnce([=](float dt) {
			AUDIO_ENGINE->play(k_attack_normal);
		}, 0.5f, cocos2d::StringUtils::format("hien lua to"));
		knight_gaf->playSequence("attack", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			knight_gaf->setPositionX(dragon_gaf->getPositionX() + 450);
			knight_gaf->playSequence(s, false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				knight_gaf->setPosition(knight_pos_old);
				knight_gaf->playSequence("idle", true);
				knight_effect_gaf->playSequence("goback", false);
			});
			AUDIO_ENGINE->play(random(0, 1) ? d_attack1 : d_attack2);
			dragon_gaf->playSequence(s, false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				key ? this->dragonBreathe() : this->dragonFallAndGU();
			});

		});
		knight_effect_gaf->playSequence("attack", false);
	});
	this->scheduleOnce([=](float dt) {
		this->nextGame();
	}, key ? 4.f : 5.25f, cocos2d::StringUtils::format("kiem tra next game"));
}
void DefeatTheDragon::attackDragonUltimate() {

	AUDIO_ENGINE->play(k_correct_music);
	knight_gaf->playSequence("correct", true, [=](gaf::GAFObject *obj, std::string name_sequence) {
		AUDIO_ENGINE->play(d_action1);
		dragon_gaf->playSequence("action1", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			dragon_gaf->playSequence("idle", true);
			this->scheduleOnce([=](float dt) {
				Vec2 knight_pos_old = knight_gaf->getPosition();
				this->scheduleOnce([=](float dt) {
					AUDIO_ENGINE->play(k_attack_normal);
					AUDIO_ENGINE->play(d_attack_last);
				}, 0.5f, cocos2d::StringUtils::format("play audio tan cong"));
				knight_gaf->playSequence("attack", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
					// room man hinh
					root_layout->runAction(ScaleTo::create(0.2, 1.8));
					root_layout->runAction(MoveBy::create(0.2, Vec2(-1000, -200)));
					//
					knight_gaf->setPositionX(dragon_gaf->getPositionX() + 450);
					knight_gaf->playSequence("attack1", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
						AUDIO_ENGINE->play(k_attack_normal);
						knight_gaf->playSequence("attack2", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
							AUDIO_ENGINE->play(k_attack_normal);
							knight_gaf->playSequence("attack3", false, [=](gaf::GAFObject *obj, std::string name_sequence) {

								//knight_gaf->setPosition(knight_pos_old);
								knight_gaf->runAction(Sequence::create(MoveTo::create(0.3, knight_pos_old),
									CallFunc::create([=] {
									knight_gaf->playSequence("idle", true);
									knight_effect_gaf->playSequence("goback", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
										this->nextGame();
									});
								}
								), NULL));
								AUDIO_ENGINE->play(d_laugh_music);
								dragon_gaf->playSequence("fall", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
									
									dragon_gaf->playSequence("laugh_2", true);
								});
								root_layout->runAction(MoveBy::create(0.25, Vec2(1000, 200)));
								root_layout->runAction(ScaleTo::create(0.25, 1.0));
							});
							dragon_gaf->playSequence("attack3", false, [=](gaf::GAFObject *obj, std::string name_sequence) {


							});
						});
						dragon_gaf->playSequence("attack2", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
							dragon_gaf->playSequence("idle", true);
						});
					});
					dragon_gaf->playSequence("attack1", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
						dragon_gaf->playSequence("idle", true);
					});
				});
				knight_effect_gaf->playSequence("attack", false);
			}, 0.2f, cocos2d::StringUtils::format("ultimate"));
		});
	});
}

void DefeatTheDragon::onExitTransitionDidStart() {
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    AUDIO_ENGINE->stopAllSound();
    cocos2d::experimental::AudioEngine::stopAll();
	MJMainGBase::onExitTransitionDidStart();
}


void DefeatTheDragon::onClickedDelegate(cocos2d::Ref *sender, ui::Text* text, int id) {
	DefeatTheDragon_Item* item_choosed;
	hideHint();
	AUDIO_ENGINE->stopEffect(id_audio_question);
	look_sound_button = true;
	for (auto i : items) {
		if (i->id == id) {
			item_choosed = i;
			break;
		}
	}
	bool is_true = item_choosed->isTrue();
	if (is_true) {
		CCLOG("DUNG");
		check_dragon_idle = false;
		this->unschedule("check_idle");
		dragon_gaf->stopAllActions();
		AUDIO_ENGINE->play(correct_music);
		this->scheduleOnce([=](float dt) {
			for (int i = 0; i < data[current_index].phonics.size(); i++) {
				if (data[current_index].phonics[i].is_right) {
					AUDIO_ENGINE->play(data[current_index].phonics[i].audio);
					break;
				}
			}
		}, 0.3f, cocos2d::StringUtils::format("audio phonic"));
		for (DefeatTheDragon_Item* item : items) {
			item->lockTouch();
			if (item->isTrue()) {
				item->getText()->setTextColor(Color4B::BLUE);
				item->showStar();
			}
		}

		this->scheduleOnce([=](float dt) {
			for (int i = 0; i < items.size(); i++) {
				items[i]->runAction(EaseIn::create(MoveTo::create(0.5f, pos1[i]), 1.5f));
			}

			this->scheduleOnce([=](float dt) {
				fire_large_gaf->setVisible(true);
			}, 0.4f, cocos2d::StringUtils::format("hien lua to"));

			this->scheduleOnce([=](float dt) {

				text_full->setVisible(true);
				//to mau
				int highligh_index_1 = 0, highligh_index_2;

				std::string right_phonic = "";
				for (int i = 0; i < data[current_index].phonics.size(); ++i) {
					if (data[current_index].phonics[i].is_right) {
						right_phonic = data[current_index].phonics[i].text;
						highligh_index_2 = highligh_index_1 + data[current_index].phonics[i].text.size();
						int j = 0;
						for (int ik = highligh_index_1; ik < highligh_index_2; ik++) {
							if (text_full->getString()[ik] == right_phonic[j]) {
								auto letter = text_full->getLetter(ik);
								if (letter) {
									letter->setColor(Color3B::BLUE);
								}
							}
							j++;
						}
					}
					highligh_index_1 += data[current_index].phonics[i].text.size();
				}

				text_full->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.3f, 1.3), 1.5f), CallFunc::create([=] {
				}), NULL));

				for (int i = 0; i < items.size(); i++) {
					items[i]->getFire()->setVisible(false);
					items[i]->getText()->setVisible(false);
				}
				fire_large_gaf->runAction(EaseIn::create(ScaleTo::create(0.3f, 1.3), 1.5f));
				AUDIO_ENGINE->play(data[current_index].audio_full_text);

				this->scheduleOnce([=](float dt) {
					// FIRE BREAK
					AUDIO_ENGINE->play(breath_music);
					fire_large_gaf->setVisible(false);
					text_full->setVisible(false);
					for (int i = 0; i < items.size(); i++) {
						items[i]->breakFire();
					}
				}, 1.f, cocos2d::StringUtils::format("no lua to"));

				this->scheduleOnce([=](float dt) {
					if (current_index < number_words - 1) {
						this->attackDragonNormal();// kiem tra next game trong day
					}
					else {
						this->attackDragonUltimate();
					}

				}, 0.5f, cocos2d::StringUtils::format("doc xong tu danh luon"));

			}, 0.5f, cocos2d::StringUtils::format("hop cac lua nho"));

		}, 0.8f, cocos2d::StringUtils::format("hop cac tu lai thanh 1"));

		//this->showGGame();
	}
	else {
		CCLOG("SAI");
		AUDIO_ENGINE->play(wrong_music);
		this->scheduleOnce([=](float dt) {
			for (int i = 0; i < data[current_index].phonics.size(); i++) {
				if (data[current_index].phonics[i].text == item_choosed->getText()->getString()) {
					AUDIO_ENGINE->play(data[current_index].phonics[i].audio);
					showHint();
					break;
				}
			}
		}, 0.3f, cocos2d::StringUtils::format("audio phonic"));
		text->setTextColor(Color4B::RED);
		Vec2 head_knight_kc = Vec2(knight_gaf->getPositionX() - 380, knight_gaf->getPositionY() + 500) - item_choosed->getPosition();
		Vec2 old_pos = item_choosed->getFire()->getPosition();
		item_choosed->getFire()->runAction(Spawn::create(EaseIn::create(MoveBy::create(0.4f, Vec2(head_knight_kc.x, 0)), 3.f), MoveBy::create(0.4f, Vec2(0, head_knight_kc.y)), NULL));
		item_choosed->getText()->runAction(Spawn::create(EaseIn::create(MoveBy::create(0.4f, Vec2(head_knight_kc.x, 0)), 3.f), MoveBy::create(0.4f, Vec2(0, head_knight_kc.y)), NULL));
		this->scheduleOnce([=](float dt) {
			AUDIO_ENGINE->play(k_hit_music);
			item_choosed->getFire()->setPosition(old_pos);
			item_choosed->getText()->setPosition(old_pos);
			item_choosed->getFire()->runAction(EaseIn::create(ScaleTo::create(0.1, 1.f), 1.5f));
			item_choosed->getText()->runAction(EaseIn::create(ScaleTo::create(0.1, 1.f), 1.5f));
		}, 0.4f, cocos2d::StringUtils::format("dua cau lua tro lai"));
		for (DefeatTheDragon_Item* item : items) {
			item->lockTouch();
		}
		this->scheduleOnce([=](float dt) {
			AUDIO_ENGINE->play(d_laugh_music);
			dragon_gaf->playSequence("laugh1", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				dragon_gaf->playSequence("idle", true);
			});
			knight_gaf->playSequence("hit2", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				AUDIO_ENGINE->play(k_face_wipe);
				knight_gaf->playSequence("face_wipe", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
					knight_gaf->playSequence("idle", true);
					text->setTextColor(Color4B::BLACK);
					for (DefeatTheDragon_Item* item : items) {
						item->unLockTouch();
						look_sound_button = false;
					}
				});
			});
		}, 0.4f, cocos2d::StringUtils::format("bong dap vao mat"));
	}
}

void DefeatTheDragon::showHint(float dt)
{
    CRASH_AUTO_LOG;
    hideHint();
	this->schedule([=](float _dt) {
        _numberShowHint++;
        MS_LOGGER.logEvent("hint_game", {
            {"number_showhit", cocos2d::Value(_numberShowHint)}
        });
		this->playSound(this);
	}, 12.5f, 100, dt, "hint_game");
}

void DefeatTheDragon::hideHint()
{
	this->unschedule("hint_game");
}



void DefeatTheDragon::onNextStage() {
	this->unscheduleAllCallbacks();

}

void DefeatTheDragon::onFinishGame() {

	//this->scheduleOnce([=](float dt) {
	AUDIO_ENGINE->play(k_victory);
	knight_gaf->playSequence("victory", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		knight_gaf->playSequence("v_idle", true);
	});
	this->scheduleOnce([=](float dt) {
		root_layout->setPositionX(root_layout->getPositionX() + 80);
		x_layout->runAction(ScaleTo::create(0.2, 1.8));
		x_layout->runAction(MoveBy::create(0.2, Vec2(700, 125)));
	}, 1.8f, cocos2d::StringUtils::format("zoom vao knight"));
	this->scheduleOnce([=](float dt) {
		game_name->setString("VICTORY");
		game_name->setTextColor((Color4B)Color3B(106, 32, 26));
		game_name->setFontSize(100);
		game_name->setFontName(font_name);
		game_name->setPositionX(game_name->getPositionX() - 480);
		game_name->setPositionY(game_name->getPositionY() + 75);
		game_name->runAction(FadeIn::create(0.1f));
		bg_win_gaf->setVisible(true);
		bg_win_gaf->setScale(0.6);
		bg_win_gaf->play(true);
		this->scheduleOnce([=](float dt) {
			MJMainGBase::onCloseGame();
		}, 1.5f, cocos2d::StringUtils::format("chuyen toi onfinish"));
	}, 2.45f, cocos2d::StringUtils::format("hien bg win"));
	//}, 8.f, cocos2d::StringUtils::format("show background win"));
}

void DefeatTheDragon::nextGame() {
	this->unscheduleAllCallbacks();
	current_index++;
	if (current_index < number_words) {

		this->dragonShowText(current_index);
		showHint();
	}
	else {
		// ket thuc game
		//attackDragonUltimate();
        
        
		this->onFinishGame();
	}
}

void DefeatTheDragon::checkIdle() {
	this->schedule([=](float dt) {
		if (check_dragon_idle) {
			random(0, 0) ? this->dragonBreathe() : this->dragonFly();
		}
	}, random(7, 9), 1000, 0.f, "check_idle");
}

void DefeatTheDragon::playSound(cocos2d::Ref* sender) {
	if (!look_sound_button) {
		//AUDIO_ENGINE->stop(id_audio_question);
		look_sound_button = true;
		id_audio_question = cocos2d::experimental::AudioEngine::play2d(question_music);
		cocos2d::experimental::AudioEngine::setFinishCallback(id_audio_question, [=](int au_id, std::string path) {
            id_audio_question = AUDIO_ENGINE->play(_pathAudioCorrect, false, 1.0f, [=] {
                look_sound_button = false;
                showHint();
            });
		});
	}
}
ui::Widget::ccWidgetClickCallback DefeatTheDragon::onLocateClickCallback(const std::string &callback_name) {
	if (callback_name == "play_sound") {
		return CC_CALLBACK_1(DefeatTheDragon::playSound, this);
	}
	return nullptr;
}

#define auto_testing_delay 1.5f
void DefeatTheDragon::onAutoPlay()
{
	this->scheduleOnce([=](float dt) {
		this->autoPlayRun(1.5f);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void DefeatTheDragon::autoPlayRun(float dt)
{
	this->scheduleOnce([=](float dt) {
		this->onAutoPlay();
	}, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}



