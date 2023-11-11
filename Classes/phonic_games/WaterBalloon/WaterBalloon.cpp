
#include "WaterBalloon.h"
#include "JsonDump.h"
#include "MJHelper.h"
INITIALIZE_READER(WaterBalloon);

std::once_flag WaterBalloon_reader;
#define CSB_NAME "csb/game/WaterBalloon/WaterBalloon.csb"
#define _game_path "json/WaterBalloon/"
//gaf path
#define path_gaf_bach_tuoc1 "gaf/waterballoon/wb_Oct_bitmap/wb_Oct_bitmap.gaf"
#define path_gaf_bach_tuoc2 "gaf/waterballoon/wb_centercastle_bitmap/wb_centercastle_bitmap.gaf"
#define path_gaf_bubbles "gaf/waterballoon/wb_bubbles/wb_bubbles.gaf"
#define path_gaf_character "gaf/waterballoon/wb_Nat/wb_Nat.gaf"
#define path_gaf_balloonF "gaf/waterballoon/wb_balloonF/wb_balloonF.gaf"
#define path_gaf_balloonB "gaf/waterballoon/wb_balloonB/wb_balloonB.gaf"
#define path_gaf_correct "gaf/waterballoon/stars colors/stars colors.gaf"
#define GAME_OCTOPY_SPLASH "hdr/games/waterballoon/Octopy_Splash.png"


//sounds path
#define path_sound_background "sounds/waterballoon/wb_bgm.mp3"
#define path_sound_waterambience "sounds/waterballoon/wb_waterambience-15db.mp3"
#define path_sound_natfadein "sounds/waterballoon/wb_natfadein.mp3"
#define path_sound_natfadeout "sounds/waterballoon/wb_natfadeout.mp3"
#define path_sound_throw "sounds/waterballoon/wb_throw.mp3"
#define path_sound_hit "sounds/waterballoon/wb_hit.mp3"
#define path_sound_correct "sounds/waterballoon/wb_correct.mp3"
#define path_sound_dizzyfadeout "sounds/waterballoon/wb_dizzyfadeout.mp3"
#define path_sound_nat_correct_1 "sounds/waterballoon/Nat_correct1.mp3"
#define path_sound_nat_correct_2 "sounds/waterballoon/Nat_correct2.mp3"
#define path_sound_nat_oops "sounds/waterballoon/NAT_oops.mp3"
#define path_sound_wrong "sounds/waterballoon/waterballoon_wrongeffect.mp3"
#define path_sound_question "sounds/waterballoon/Listen and tap on the correct word.mp3"
#define SOUND_GAME_NAME "sounds/waterballoon/Octopy_splash.mp3"
#define SOUND_TAP_ON "sounds/waterballoon/tap_on1.mp3"

WaterBalloon * WaterBalloon::createGame(std::string json_file, bool isAutoPlay) {
	std::call_once(WaterBalloon_reader, [] {
		REGISTER_CSB_READER(WaterBalloon);
		REGISTER_CSB_READER(WaterBalloon_Item);
	});
	auto p = static_cast<WaterBalloon *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void WaterBalloon::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	so_luot_choi = document.FindMember("game_mode")->value.GetInt();
    if (document.HasMember("option_vm_phonic")){
        hoc_am_hoc_van = document.FindMember("option_vm_phonic")->value.GetString();
    }
	rapidjson::Value& json_data = document.FindMember("data")->value;
     std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	this->generateData(json_data.GetArray(), images);
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;
	flag = true;
	showInformationGame();
}

void WaterBalloon::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images) {
	//std::vector<Three_Words>data_game;
//    data_game.clear();
//    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
//        Three_Words tw;
//        auto data_three_words = value_data[i].GetArray();
//        std::string tmp;
//        for (int j = 0; j < 3; j++) {
//            WaterBalloon_Item::Word w;
//            w.text = data_three_words[j].FindMember("full_text")->value.GetString();
//            w.audio_1 = data_three_words[j].FindMember("audio_full_text")->value.GetString();
//            w.audio_2 = data_three_words[j].FindMember("audio_2")->value.GetString();
//            tmp= data_three_words[j].FindMember("true")->value.GetString();
//            tw.words.push_back(w);
//        }
//        results.push_back(tmp);
//        data_game.push_back(tw);
//    }
	//mj::helper::shuffle(data_game);
    
    data_game.clear();
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
      
        Three_Words tw;
        auto data_three_words = value_data[i].GetArray();
        std::string tmp;
        for (int j = 0; j < 3; j++) {
            WaterBalloon_Item::Word w;
            auto letter = data_three_words[j].FindMember("full_text")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
            w.text = pImage->getText("name_1");
            w.audio_1 = pImage->getAudios("name_1").front(); //normal sound
            w.audio_2 = pImage->getAudios("name_n").front(); //slow sound
            //data_three_words[j].FindMember("audio_full_text")->value.GetString();
            //data_three_words[j].FindMember("audio_2")->value.GetString();
            
            if(data_three_words[j].HasMember("is_correct") && data_three_words[j].FindMember("is_correct")->value.GetBool()){
                tmp = w.text;
            }
//            tmp= data_three_words[j].FindMember("true")->value.GetString();
            tw.words.push_back(w);
        }
        
        results.push_back(tmp);
        data_game.push_back(tw);
    }
    
	so_luot_choi = min((int)data_game.size(), so_luot_choi);
}


WaterBalloon * WaterBalloon::createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay, std::string game_type) {
	std::call_once(WaterBalloon_reader, [] {
		REGISTER_CSB_READER(WaterBalloon);
		REGISTER_CSB_READER(WaterBalloon_Item);
	});
	auto p = static_cast<WaterBalloon *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(list_PHONIC_INFO, game_type, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void WaterBalloon::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
	//    _list_PHONIC_INFO = list_PHONIC_INFO;
	//    _list_phonic_info = _list_PHONIC_INFO.list_phonic;
	//    type_game = game_type;
	//   if(max_game > list_PHONIC_INFO.list_phonic.size()){
	//       max_game = (int) list_PHONIC_INFO.list_phonic.size();
	//   }
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;
	flag = true;
	showInformationGame();
}

void WaterBalloon::onEnter() {
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

void WaterBalloon::loadFrames() {
	//mj::helper::shuffle(data);
	//root layout
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	root_layout = utils::findChild<Layout*>(this, "root_layout");

    sound_button = utils::findChild<Layout*>(this, "sound");
    sound_button->setZOrder(1000);
    sound_button->setPosition(root_layout->convertToNodeSpace(Vec2(60, screen_size.height - 60)));
    sound_button->setTouchEnabled(true);
	sound_button->setVisible(true);

	gaf_balloonB = GAFWrapper::createGAF(path_gaf_balloonB);
	gaf_balloonB->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_balloonB->setPositionX(screen_size.width / 2 + 40);
	gaf_balloonB->setPositionY(screen_size.height / 2 - 30);
	root_layout->addChild(gaf_balloonB, 2);

	gaf_bach_tuoc_1 = GAFWrapper::createGAF(path_gaf_bach_tuoc1);
	root_layout->addChild(gaf_bach_tuoc_1, 2);
	gaf_bach_tuoc_1->setPositionX(screen_size.width / 2 + 29);
	gaf_bach_tuoc_1->setPositionY(screen_size.height / 2 - 20);
	gaf_bach_tuoc_1->setScale(0.95);

	gaf_bach_tuoc_2 = GAFWrapper::createGAF(path_gaf_bach_tuoc2);
	root_layout->addChild(gaf_bach_tuoc_2, 2);
	gaf_bach_tuoc_2->setPositionX(screen_size.width / 2 + 29);
	gaf_bach_tuoc_2->setPositionY(screen_size.height / 2 - 20);
	gaf_bach_tuoc_2->setScale(0.95);

	gaf_bubbles_left_2 = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_left_2, 2);
	gaf_bubbles_left_2->setPositionX(screen_size.width / 2 - 500);
	gaf_bubbles_left_2->setPositionY(screen_size.height / 2 - 150 - 82);
	gaf_bubbles_left_2->playSequence("bubbles_l2", true);

	ImageView* image_cau = utils::findChild<ImageView*>(this, "image_cau");
	image_cau->setZOrder(gaf_bach_tuoc_2->getZOrder() + 1);
	image_cau->setPositionX(screen_size.width / 2 - 700);
	image_cau->setPositionY(image_cau->getPositionY() +25);

	gaf_bubbles_left_1 = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_left_1, 2);
	gaf_bubbles_left_1->setPositionX(screen_size.width / 2 - 490);
	gaf_bubbles_left_1->setPositionY(screen_size.height / 2 - 270 - 60);
	gaf_bubbles_left_1->setZOrder(image_cau->getZOrder() + 1);
	gaf_bubbles_left_1->playSequence("bubbles_0", true);
	gaf_bubbles_left_1->setScale(0.9);

	gaf_bubbles_mid = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_mid, 2);
	gaf_bubbles_mid->setPositionX(screen_size.width / 2 - 20);
	gaf_bubbles_mid->setPositionY(screen_size.height / 2 - 210 - 75);
	gaf_bubbles_mid->playSequence("bubbles_c", true);

	gaf_bubbles_right_1 = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_right_1, 2);
	gaf_bubbles_right_1->setPositionX(screen_size.width / 2 + 325);
	gaf_bubbles_right_1->setPositionY(screen_size.height / 2 - 150 - 82);
	gaf_bubbles_right_1->playSequence("bubbles_r1", true);

	gaf_bubbles_right_2 = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_right_2, 2);
	gaf_bubbles_right_2->setPositionX(screen_size.width / 2 + 440);
	gaf_bubbles_right_2->setPositionY(screen_size.height / 2 - 140 - 82);
	gaf_bubbles_right_2->playSequence("bubbles_r2", true);

	gaf_bubbles_right_3 = GAFWrapper::createGAF(path_gaf_bubbles);
	root_layout->addChild(gaf_bubbles_right_3, 2);
	gaf_bubbles_right_3->setPositionX(screen_size.width / 2 + 565);
	gaf_bubbles_right_3->setPositionY(screen_size.height / 2 - 145 - 70);
	gaf_bubbles_right_3->playSequence("bubbles_r3", true);

	gaf_character = GAFWrapper::createGAF(path_gaf_character);
	//gaf_character->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_character->setPositionX(screen_size.width / 2 + 700);
	gaf_character->setPositionY(screen_size.height / 2 - 1050 - 60);
	root_layout->addChild(gaf_character,4);
	
	gaf_balloonF = GAFWrapper::createGAF(path_gaf_balloonF);
	gaf_balloonF->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_balloonF->setPositionX(screen_size.width / 2 + 40);
	gaf_balloonF->setPositionY(screen_size.height / 2 - 30);
	root_layout->addChild(gaf_balloonF, 2);

	gaf_correct = GAFWrapper::createGAF(path_gaf_correct);
	gaf_correct->setAnchorPoint(Vec2(0.5, 0.5));
	root_layout->addChild(gaf_correct, 2);
	gaf_correct->setVisible(false);

	// gen cac item
	current_index = 0;
	look_sound_button = true;
	current_result = random(0, 2);
    this->showGame();
}

void WaterBalloon::showGame() {
	playBackgroundMusic(path_sound_background);
	AUDIO_ENGINE->play(path_sound_waterambience, true);

	gaf_bach_tuoc_2->play(true);
	gaf_bach_tuoc_1->playSequence("intro", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		gaf_bach_tuoc_1->playSequence("idle_pre", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			gaf_bach_tuoc_1->playSequence("idle", true);
		
		});
	});
	
	AUDIO_ENGINE->play(path_sound_natfadein, false, 0.6);
    auto path_question = CONFIG_MANAGER->isGameVMonkey()?(hoc_am_hoc_van=="hoc_am"?"sounds/waterballoon/question_vi_am.mp3":"sounds/waterballoon/question_vi_van.mp3"):path_sound_question;
	AUDIO_ENGINE->play(path_question, false, 1.0f, [=] {
		this->showQuestion();
	});

	gaf_character->playSequence("fade_in", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		
		gaf_character->playSequence("turning", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			gaf_character->playSequence("turned back", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
				//gaf_character->playSequence("idle", true);
				
				//this->showQuestion();
			});
		});
	});
}

void WaterBalloon::showQuestion() {
	flag = true;
	gaf_bach_tuoc_1->playSequence("idle", true);
	for (WaterBalloon_Item* item : items) {
		item->removeFromParent();
	}
	items.clear();
	mj::helper::shuffle(data_game[current_index].words);
    auto item_font_sz = getItemFontSize();
    
	for (int i = 0; i < 3; i++) {
		
		WaterBalloon_Item* item = WaterBalloon_Item::createItem(data_game[current_index].words[i], i);
		item->setAnchorPoint(Vec2(0.5, 0.5));
		item->setDelegate(this);
        item->setFontSize(item_font_sz);
        if (CONFIG_MANAGER->isVietnameseStories() && data_game[current_index].words[i].text.length() <= 3){
            item->setFontSize(100);
        }
		root_layout->addChild(item, 2);
		items.push_back(item);
		item->setPositionY(690.f - abs(i - 1) * 30);
		item->setPositionX(screen_size.width / 2 + (i - 1) * 275);
		item->runAction(FadeIn::create(0.75f));
		if (data_game[current_index].words[i].text == results[current_index]) {
			current_result = i;
			gaf_correct->setPosition(item->getPosition());
			gaf_correct->setZOrder(item->getZOrder() + 1);
		}

	}


    audio_id_tu = cocos2d::experimental::AudioEngine::play2d(data_game[current_index].words[current_result].audio_1);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_tu, [=](int au_id, std::string path) {
		this->inActiveMode();
		sound_button->setTouchEnabled(true);
		look_sound_button = false;
        for (WaterBalloon_Item* item : items) {
            item->unLockBalloon();
        }
	});
   
	this->scheduleOnce([=](float dt) {
		for (auto item : items) {
			item->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.6, 1.1), ScaleTo::create(0.6, 1.0), NULL)));
			//item->runAction(Sequence::create(ScaleTo::create(0.2, 1.2), ScaleTo::create(0.2, 0.8), NULL));
		}
		
	}, 0.75f, cocos2d::StringUtils::format("lam  cho chu phap phong"));
}

void WaterBalloon::nemBong(int id, bool is_true) {
	string s1 = is_true ? "correct_" : "wrong_";
	string s2 = "R";
	if (id == 1) s2 = "M";
	if (id == 0) s2 = "L";
	
	CCLOG("RESULT %s%s", s1.c_str(), s2.c_str());
	AUDIO_ENGINE->play(path_sound_throw);

	gaf_character->playSequence("throw", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		gaf_character->playSequence("idle", true);
	});
	
	gaf_balloonF->playSequence(s1 + s2, false);
	if (!is_true){
		gaf_balloonB->playSequence("wrong_" + s2, false);
		if (id == 0) s2 = (random(0, 1) ? "M" : "R");
		if (id == 1) s2 = (random(0, 1) ? "L" : "R");
		if (id == 2) s2 = (random(0, 1) ? "M" : "L");
	}

	this->scheduleOnce([=](float dt) {
		if (is_true) {
			AUDIO_ENGINE->play(path_sound_hit);
			this->scheduleOnce([=](float dt) {
				if (current_index != so_luot_choi - 1) {
					AUDIO_ENGINE->play(random(0, 1) ? path_sound_nat_correct_1 : path_sound_nat_correct_2);
					gaf_character->playSequence("yay", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
						gaf_character->playSequence("idle", true);
					});
				}
			}, 0.6f, cocos2d::StringUtils::format("sound nat correct"));

		}
		else {
			AUDIO_ENGINE->play(path_sound_nat_oops);
		}
			
	}, 0.7f, cocos2d::StringUtils::format("sound hit"));
	
	gaf_bach_tuoc_1->playSequence(s1+s2, false, [=](gaf::GAFObject *obj, std::string name_sequence) {
		if (is_true) {
			this->nextGame();
		}
		else {
			this->showQuestion();
		}
	});
}

void WaterBalloon::onExitTransitionDidStart() {
	MJMainGBase::onExitTransitionDidStart();
    
}


void WaterBalloon::onClickedDelegate(cocos2d::Ref *sender, int id) {
	sound_button->setTouchEnabled(false);
	if (!flag) {
		return;
	}
	flag = false;
	WaterBalloon_Item* item_choosed = (WaterBalloon_Item*)sender;
	AUDIO_ENGINE->stopEffect(audio_id_danh_van);
	this->unschedule("animated_guide");
	AUDIO_ENGINE->stopSound(SOUND_TAP_ON);
	for (auto item : items) {
		item->stopAllActions();
		item->setScale(1.0f);
		item->lockBalloon();
	}

	if (id == current_result) {
		CCLOG("DUNG");
		
		gaf_correct->setVisible(true);
		gaf_correct->play(false);
		for (WaterBalloon_Item* item : items) {
			if (item->id != id) {
				item->runAction(Sequence::create(FadeOut::create(0.25f), NULL));
			}
			else {
				item->getText()->setTextColor(Color4B::GREEN);
				item->runAction(Sequence::create(ScaleTo::create(0.25, 1.3), DelayTime::create(0.5), FadeOut::create(0.25f), NULL));
			}
		}
		//auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_sound_correct);
		//AUDIO_ENGINE->play(data_game[current_index].words[current_result].audio_1);
		std::vector<std::string> chain;
		chain.push_back(path_sound_correct);
		chain.push_back(data_game[current_index].words[current_result].audio_1);
		AUDIO_ENGINE->playChain(chain);
		
		this->scheduleOnce([=](float dt) {
			nemBong(id, true);
		}, 1.f, cocos2d::StringUtils::format("nembong"));

	}
	else {
		CCLOG("SAI");
		AUDIO_ENGINE->play(path_sound_wrong, false, 1.0f, [=]() {
			
			});
		for (WaterBalloon_Item* item : items) {
			if (item->id != id) {
				item->runAction(Sequence::create(FadeOut::create(0.4f), NULL));
			}
			else {
				item->getText()->setTextColor(Color4B::RED);
                if (CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->play(item->getAudio());
				float time = 0.06;
				item->runAction(Sequence::create(MoveBy::create(time,Vec2(-20,0)), MoveBy::create(2*time, Vec2(40, 0)), MoveBy::create(2*time, Vec2(-40, 0)), MoveBy::create(2*time, Vec2(40, 0)), MoveBy::create(2 * time, Vec2(-40, 0)), MoveBy::create(time, Vec2(20, 0)), FadeOut::create(0.15f), NULL));
			}
		}
		this->scheduleOnce([=](float dt) {
			nemBong(id, false);
		}, 0.3f, cocos2d::StringUtils::format("nembong"));
		
    }
    this->inActiveMode();
    
}



void WaterBalloon::onNextStage() {
	this->unscheduleAllCallbacks();
}

void WaterBalloon::onFinishGame() {
	AUDIO_ENGINE->stopAllAudio();
//    AUDIO_ENGINE->stopAllEffects();
//    MJMainGBase::onFinishGame(CHARACTER::NAT);
    MJMainGBase::onFinishGame();
}

void WaterBalloon::nextGame() {
	this->unscheduleAllCallbacks();
	current_index++;
	current_result = random(0, 2);
	if (current_index < so_luot_choi) {
		this->showQuestion();
	}
	else {
		flag = false;
		sound_button->setTouchEnabled(false);

		// chay hieu ung ket thuc
		AUDIO_ENGINE->stopAllAudio();
		AUDIO_ENGINE->play(path_sound_dizzyfadeout);
		gaf_bach_tuoc_1->playSequence("fade_out", false, [=](gaf::GAFObject *obj, std::string name_sequence) {
			onFinishGame();
		});

		AUDIO_ENGINE->play(path_sound_natfadeout);
		gaf_character->playSequence("fade_out", false, [=](gaf::GAFObject *obj, std::string name_sequence) {			
		});

	}
}



void WaterBalloon::inActiveMode(float delay_time) {
	this->schedule([=](float) {

		AUDIO_ENGINE->playSound(SOUND_TAP_ON, false, [=]() {
			audio_id_tu = cocos2d::experimental::AudioEngine::play2d(data_game[current_index].words[current_result].audio_1);
			});

	}, 10, 100, delay_time, "animated_guide");

}
void WaterBalloon::playSound(cocos2d::Ref* sender) {
	if (!look_sound_button) {
		this->unschedule("animated_guide");
		AUDIO_ENGINE->stopEffect(audio_id_tu);
		AUDIO_ENGINE->stopEffect(audio_id_danh_van);
		AUDIO_ENGINE->playSound(SOUND_TAP_ON, false, [=]() {
			audio_id_tu = cocos2d::experimental::AudioEngine::play2d(data_game[current_index].words[current_result].audio_1);
			this->inActiveMode();
			});
	}
}
ui::Widget::ccWidgetClickCallback WaterBalloon::onLocateClickCallback(const std::string &callback_name) {
	if (callback_name == "play_sound") {
		return CC_CALLBACK_1(WaterBalloon::playSound, this);
	}
	return nullptr;
}


#define auto_testing_delay 1.5f
void WaterBalloon::onAutoPlay()
{
	this->scheduleOnce([=](float dt) {
		this->autoPlayRun(1.5f);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void WaterBalloon::autoPlayRun(float dt)
{
	this->scheduleOnce([=](float dt) {
		this->onAutoPlay();
	}, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}

int WaterBalloon::getItemFontSize(){
    string longest = "";
    for(auto p: data_game[current_index].words){
        if(p.text.length() > longest.length()){
            longest = p.text;
        }
    }
    auto size = font_sz;
    Text* example = Text::create(longest, font_name, size);
    while (example->getVirtualRendererSize().width > 200) {
        CCLOG("text_size: %f, this size: %f", example->getVirtualRendererSize().width, getContentSize().width * 0.9);
        size = size-2;
        example->setFontSize(size);
    }
    return size;
}

void WaterBalloon::showInformationGame() {

	sound_button = utils::findChild<Layout*>(this, "sound");
	sound_button->setVisible(false);
	
	this->runAction(Sequence::create(DelayTime::create(0.5f)
		, CallFunc::create([=]() {
			if (auto infoGame = Sprite::create(GAME_OCTOPY_SPLASH)) {
				auto visibleSize = Director::getInstance()->getVisibleSize();
				infoGame->setAnchorPoint(Vec2(0.5, 0.5));
				infoGame->setPosition(Vec2(-500, visibleSize.height / 2));
				if (visibleSize.width / visibleSize.height < 1.5) {
					infoGame->setScale(0.6);
				}
				else {
					if (visibleSize.width / visibleSize.height < 1.8) {
						infoGame->setScale(0.8);
					}
					else {
						infoGame->setScale(1);
					}
				}
				infoGame->setName("gameName");
				this->addChild(infoGame);

				infoGame->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.0f, Vec2(visibleSize / 2)))
					, DelayTime::create(0.5f)
					, CallFunc::create([=]() {
						AUDIO_ENGINE->playSound(SOUND_GAME_NAME);
						})
					, DelayTime::create(3.0f)
							, MoveTo::create(0.5f, Vec2(visibleSize.width + 500, visibleSize.height / 2)), NULL));
			}
			})
		, DelayTime::create(5.5f)
				, CallFunc::create([=]() {
				if (auto child = utils::findChild(this, "gameName")) {
					child->removeFromParent();
				}
				this->loadFrames();
					}), NULL));
}
