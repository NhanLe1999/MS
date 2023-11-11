/*
*
* MexicanCactus_VM.cpp
* Created by minhtzy on 3/10/18
*/
#include "MexicanCactus_VM.h"



MexicanCactus_VM::MexicanCactus_VM()
	: item_index{ 0 }
	, m_configpath{ "" }
	, root_layout{ nullptr }
{
}

MexicanCactus_VM * MexicanCactus_VM::createGame(std::string json_file)
{
	auto game = new MexicanCactus_VM();
	if (game && game->init()) {
		game->autorelease();
		game->didLoadFromCSB(json_file);
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	return game;
}

void MexicanCactus_VM::onEnter()
{
	MJMainGBase::onEnter();
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = "fonts/danhvan.ttf";
    }
	generateData();
	loadFrames();
	showGamename();
	playBackgroundMusic("sounds/MexicanCactus_VMes/sfx/cactus_bgm.mp3");
}

void MexicanCactus_VM::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void MexicanCactus_VM::onFinishGame()
{
	MJMainGBase::onFinishGame();
}

void MexicanCactus_VM::didLoadFromCSB(std::string json_file)
{
	m_configpath = json_file;
}

void MexicanCactus_VM::generateData()
{
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_configpath);
	rapidjson::Document value_data;

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
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(m_images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }

	if (value_data.HasMember("number_turnplay") && value_data["number_turnplay"].IsInt()) {
		number_turnplay = value_data["number_turnplay"].GetInt();
	}

	if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
		auto list_item_data = value_data["list_item"].GetArray();
		for (int i = 0; i < (int)list_item_data.Size(); i++) {
			auto item_data = list_item_data[i].GetObject();

			MEXICAN_CACTUS_INFO item_info;			
			auto letter = item_data["text"].GetString();
			auto pImage = math::resource::ResHelper::getImage(m_images, letter);
			item_info.text = pImage->getText("name_1");
			item_info.normal_audio = pImage->getAudios("name_1").front();
			//item_info.slow_audio = pImage->getAudios("name_n").front();
            item_info.dau_cau.text = pImage->getText("kind");
            if (item_info.dau_cau.text!=""){
                item_info.co_dau = true;
                item_info.normal_audio = pImage->getAudios("name_n").front();
                item_info.dau_cau.audio = pImage->getAudios("kind").front();
                item_info.dau_cau.list_index.push_back(pImage->getComparisionValue("position_p"));
                item_info.text_ko_dau = pImage->getText("name_1");
                item_info.audio_ko_dau = pImage->getAudioPhonic("name_1").back();
                auto phonics = pImage->getTextPhonic("name_1");
                auto phonic_audios = pImage->getAudioPhonic("name_1");
                for (int j = 0; j < (int)phonics.size() - 1; j++) {
                    MEXICAN_CACTUS_CHARACTER_INFO character_info;
                    character_info.text =phonics[j];
                    character_info.audio = phonic_audios[j];
                    item_info.list_character_sync_text.push_back(character_info);
                }
            }
            
			//TODO: audio and text phonic
			auto phonics = pImage->getTextPhonic("name_p");
			auto phonic_audios = pImage->getAudioPhonic("name_p");

			for (int j = 0; j < (int)phonics.size(); j++) {
				MEXICAN_CACTUS_CHARACTER_INFO character_info;
				character_info.text =phonics[j];
				character_info.audio = phonic_audios[j];
				item_info.list_character.push_back(character_info);
			}
			genCharacterIndex(item_info);

			list_item_info.push_back(item_info);
		}
	}
}

void MexicanCactus_VM::loadFrames()
{
    
    
	this->setContentSize(screen_size);

	root_layout = cocos2d::ui::Layout::create();
	root_layout->setContentSize(screen_size);
	root_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	root_layout->setBackGroundColorOpacity(255);
	root_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	root_layout->setTouchEnabled(true);
	this->addChild(root_layout);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	touch_layout->setBackGroundColor(cocos2d::Color3B::BLACK);
	touch_layout->setBackGroundColorOpacity(0);
	touch_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	this->addChild(touch_layout, 1000);

	if (0 == cocos2d::random(0, 1)) {
		auto background = cocos2d::ui::ImageView::create("games/MexicanCactus_VM/bg.png");
		background->setPosition(Vec2(screen_size.width / 2, 450));
		root_layout->addChild(background, 1, "bg");
	}
	else {
		auto background = cocos2d::ui::ImageView::create("games/MexicanCactus_VM/bg1.png");
		background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		background->setPosition(Vec2(screen_size.width / 2, 0));
		root_layout->addChild(background, 1, "bg");
	}


	auto rocks = cocos2d::ui::ImageView::create("games/MexicanCactus_VM/rocks.png");
	rocks->setPosition(Vec2(screen_size.width / 2, 175));
	root_layout->addChild(rocks, 2, "rocks");

	text_lb = cocos2d::ui::Text::create(" ", font_name, 150);
	text_lb->enableOutline(cocos2d::Color4B::BLACK, 3);
	this->addChild(text_lb, 1001);

	text_hint = cocos2d::ui::Text::create(" ", font_name, 150);
	text_hint->enableOutline(cocos2d::Color4B::BLACK, 3);
	text_hint->setOpacity(0);
	text_hint->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.8));
	this->addChild(text_hint, 1001);

	speaker = cocos2d::ui::Button::create("games/MexicanCactus_VM/loa icon.png");
	speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	speaker->setPosition(Vec2(50, screen_size.height - 50));
	root_layout->addChild(speaker, 100);
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(MexicanCactus_VM::onClickedSpeaker, this));


	//btnSkip = cocos2d::ui::Button::create("games/MexicanCactus_VM/skip.png");
	//btnSkip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//btnSkip->setScale(0.5f);
	//btnSkip->setVisible(false);
	//btnSkip->setPosition(Vec2(screen_size.width - 150 , 75));
	//this->addChild(btnSkip, 1002);
	//btnSkip->setTouchEnabled(true);
	//btnSkip->addClickEventListener(CC_CALLBACK_1(MexicanCactus_VM::onClickedSkip, this));
    
	gaf_confetti = GAFWrapper::createGAF("gaf/MexicanCactus_VM/confetti/confetti.gaf");
	gaf_confetti->setPosition(Vec2(screen_size.width / 2 - 50, screen_size.height / 2 + 150));
	root_layout->addChild(gaf_confetti, 1002);

	disableTouch();

}

void MexicanCactus_VM::showIntroGame()
{
}

void MexicanCactus_VM::showGamename()
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        startGame();
    }else{
        auto game_name = cocos2d::ui::Text::create("Mexican Cactuses", font_name, 100);
        game_name->setPosition(screen_size / 2);
        game_name->enableOutline(cocos2d::Color4B::BLACK, 3);
        root_layout->addChild(game_name, 1000);
        game_name->setOpacity(0);
        game_name->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(0.5f), FadeOut::create(0.5f), CallFunc::create([=] {
            startGame();
        }), NULL));
    }
	
}

void MexicanCactus_VM::startGame()
{
	wrong_count = 0;
	createCactus();
	//showCactus();
	showGuideGame(8.0f);
}

void MexicanCactus_VM::onMovedCactusBegan(cocos2d::Ref * sender)
{
    this->unschedule("inActive_Mode");
	auto cactus = static_cast<MexicanCactus_VM_Item *> (sender);
	cactus->setScale(1.25f);
	//AUDIO_ENGINE->playSound(cactus->getCactusInfo().audio);
	AUDIO_ENGINE->playSound("sounds/MexicanCactus_VM/mc_interact.mp3");
	cactus->setLocalZOrder(15);
}


void MexicanCactus_VM::onMovedCactusEnded(cocos2d::Ref * sender)
{
	AUDIO_ENGINE->playSound("sounds/MexicanCactus_VM/mc_release.mp3");
	auto cactus = static_cast<MexicanCactus_VM_Item *> (sender);
	cactus->setScale(1.0f);
	cactus->setLocalZOrder(10);

	MexicanCactus_VM_Item * d_cactus = nullptr;

	float max_distance = screen_size.width + screen_size.height;
	auto cactus_boundingbox = cactus->getBoundingBox();
	for (auto item : list_cactus_item) {
		if (item != cactus && item->getBoundingBox().intersectsRect(cactus_boundingbox)) {
			CCLOG("SCRAMBLE");
			auto distance = abs(map_index_items.find(cactus)->second - map_index_items.find(item)->second);//cactus->getPosition().distance(item->getPosition());
			if (distance < max_distance) {
				d_cactus = item;
				max_distance = distance;
			}
		}
	}
	if (d_cactus == nullptr) {
		auto it = map_index_items.find(cactus);
		cactus->stopAllActions();
		cactus->runAction(cocos2d::MoveTo::create(0.25f, cactus_locations[it->second]));
        inActiveMode();
        enableTouch();
	}
	else {
		onScrambleCactus(cactus, d_cactus);
	}
   
}

void MexicanCactus_VM::onScrambleCactus(MexicanCactus_VM_Item * s_cactus, MexicanCactus_VM_Item * d_cactus)
{
	hideGuideGame();
	disableTouch();
	int s_index = map_index_items.find(s_cactus)->second;
	int d_index = map_index_items.find(d_cactus)->second;
	if (s_index > d_index) {
		for (auto item : list_cactus_item) {
			auto it = map_index_items.find(item);
			if (it != map_index_items.end() && it->second >= d_index && it->second < s_index) {
				it->second += 1;
			}
		}
	}
	else {
		for (auto item : list_cactus_item) {
			auto it = map_index_items.find(item);
			if (it != map_index_items.end() && it->second > s_index && it->second <= d_index) {
				it->second -= 1;
			}
		}
	}
	map_index_items.find(s_cactus)->second = d_index;
	float dt = 0.25f;
	for (auto item : list_cactus_item) {
		auto it = map_index_items.find(item);
		if (it != map_index_items.end()) {
			item->stopAllActions();
			item->runAction(Sequence::create(cocos2d::MoveTo::create(dt, cactus_locations[it->second]), CallFunc::create([=] {
			}), NULL));
		}
	}
	this->scheduleOnce([=](float t) {
		checkWinGame();
	}, dt, "check win game");

}

void MexicanCactus_VM::showCactus() {
	CCLOG("Debug : Show Cactus");
    inActiveMode();
	disableTouch();
	std::vector<std::string> chain;
    if(item_index == 0){
        if (CONFIG_MANAGER->isGameVMonkey()){
            chain.push_back(question_vi);
        }else{
            chain.push_back(RandomHelper::random_int(0,1)?"sounds/MexicanCactus_VM/intro1.mp3": "sounds/MexicanCactus_VM/intro2.mp3");
        }
    }
    chain.push_back(current_item_info.normal_audio);
    AUDIO_ENGINE->playChain(chain, [=] {
        enableTouch();
    });
	float dt = 0.25f;
	for (auto it = map_index_items.begin(); it != map_index_items.end(); ++it) {
		it->first->showCactus(dt);
		//dt += 0.15f;
	};
}

void MexicanCactus_VM::createCactus()
{
	current_item_info = list_item_info[item_index];
	list_cactus_item.clear();
	int index = 0;

	// create cactus
	for (auto info : current_item_info.list_character) {
		MexicanCactus_VM_Item::MexicanCactus_VM_ItemInfo c_info;
		c_info.text = info.text;
		c_info.audio = info.audio;
		for (auto index : info.list_available_index) {
			c_info.correct_index.push_back(index);
		}
		MexicanCactus_VM_Item * cactus = MexicanCactus_VM_Item::createCactus(c_info, ++index);
		cactus->setDelegate(this);
		cactus->setCactusTouchable(true);
		root_layout->addChild(cactus, 10);
		list_cactus_item.push_back(cactus);
	}

	// random pos
	do {
		map_index_items.clear();
		mj::helper::shuffle(list_cactus_item);
		for (int i = 0; i < list_cactus_item.size(); ++i) {
			map_index_items.insert({ list_cactus_item[i],i });
		}
	} while (checkCorrectIndex());

	// set pos
	float remain_space = screen_size.width - list_cactus_item[0]->getContentSize().width * list_cactus_item.size();
	float padding = remain_space / list_cactus_item.size();
	if (padding > 100) {
		padding = 100;
	}
	if (padding < 0) {
		padding = 0;
	}
	Size cactus_size = list_cactus_item[0]->getContentSize();
	Vec2 middle_pos = Vec2(screen_size.width / 2, root_layout->getChildByName("rocks")->getPositionY() + cactus_size.height / 2);
	cactus_locations = getFrameLocations(list_cactus_item.size(), middle_pos, cactus_size, padding);

	for (auto it = map_index_items.begin(); it != map_index_items.end(); ++it) {
		it->first->setPosition(cactus_locations[it->second]);
	};

	text_hint->stopAllActions();
	text_hint->setString(current_item_info.text);
	text_hint->setOpacity(0);

	showCactus();
}

void MexicanCactus_VM::changeInfoCactus()
{
	current_item_info = list_item_info[item_index];

	wrong_count = 0;
	text_hint->stopAllActions();
	text_hint->setString(current_item_info.text);
	text_hint->setOpacity(0);

	std::vector<MexicanCactus_VM_Item::MexicanCactus_VM_ItemInfo> list_info;
	for (auto info : current_item_info.list_character) {
		MexicanCactus_VM_Item::MexicanCactus_VM_ItemInfo c_info;
		c_info.text = info.text;
		c_info.audio = info.audio;
		for (auto index : info.list_available_index) {
			c_info.correct_index.push_back(index);
		}
		list_info.push_back(c_info);
	}
	do {
		mj::helper::shuffle(list_cactus_item);
		for (int i = 0; i < list_cactus_item.size(); ++i) {
			list_cactus_item[i]->setCactusInfo(list_info[i]);
		}
	} while (checkCorrectIndex());


}


void MexicanCactus_VM::checkWinGame()
{
	CCLOG("Debug: Check win game");
	if (checkCorrectIndex()) {
		onMergeWords();
	}
	else {
		++wrong_count;
		showHint();
		enableTouch();
	}
}

void MexicanCactus_VM::onMergeWords()
{
	CCLOG("Debug: Merge Word");
	text_hint->setOpacity(0);
	initText();
	auto index_cmp = [=](MexicanCactus_VM_Item * a, MexicanCactus_VM_Item * b) {
		return map_index_items.find(a)->second < map_index_items.find(b)->second;
	};
	std::sort(list_cactus_item.begin(), list_cactus_item.end(), index_cmp);


	onMergeWord(0, [=] {
        this->scheduleOnce([=](float dt_) {
            if (current_item_info.co_dau){
                text_dau = cocos2d::ui::Text::create(current_item_info.dau_cau.text, font_name, 100);
                text_dau->enableOutline(cocos2d::Color4B::BLACK, 3);
                this->addChild(text_dau);
                auto lb = text_lb->getLetter(current_item_info.dau_cau.list_index.front());
                auto pos_dau = lb->getParent()->convertToWorldSpace(lb->getPosition());
                pos_dau = (current_item_info.dau_cau.text == ".") ?Vec2(pos_dau.x,pos_dau.y-lb->getContentSize().height*0.5):Vec2(pos_dau.x,pos_dau.y+lb->getContentSize().height*0.5);
                text_dau->setPosition(pos_dau);
            }
        }, 0.35f, "add dau");
		this->scheduleOnce([=](float dt_) {
			playSyncText(0.5f);
		}, 0.75f, "play sync text");
	});

}

void MexicanCactus_VM::onMergeWord(int index, std::function<void()> callback)
{
	if (index < list_cactus_item.size()) {
		auto phonic_info = current_item_info.list_character[index];
		auto locations = getFrameLocations(phonic_info.text.size(), list_cactus_item[index]->getLblText()->getWorldPosition(), cocos2d::Size(40, 60));
		int ind = 0;

		AUDIO_ENGINE->playSound(StringUtils::format("sounds/MexicanCactus_VM/mc_star_correct%d.mp3", (index + 1) % 4));

		auto gaf_star_color = GAFWrapper::createGAF("gaf/MexicanCactus_VM/stars colors/stars colors.gaf");
		gaf_star_color->setPosition(list_cactus_item[index]->getLblText()->getWorldPosition());
		this->addChild(gaf_star_color, 5);
		gaf_star_color->play(false, [=](gaf::GAFObject * obj) {
			gaf_star_color->removeFromParent();
		});

		this->scheduleOnce([=](float dt) {
			onMergeWord(index + 1, callback);
		}, 0.15f, cocos2d::StringUtils::format("merge word %d", index));
        list_cactus_item[index]->getLblText()->setOpacity(0);
        auto text = cocos2d::ui::Text::create(phonic_info.text, font_name, 100);
        text->enableOutline(cocos2d::Color4B::BLACK, 3);
        text->setPosition(root_layout->convertToWorldSpace(locations.front()));
        root_layout->addChild(text, 20);
        auto letter = text_lb->getLetter(phonic_info.list_index[ind]);
        Vec2 move_location = text_lb->getPosition();
        if (letter) {
            move_location = text_lb->convertToWorldSpace(letter->getPosition());
        }
        text->runAction(Sequence::create(
            Spawn::create(ScaleTo::create(0.5f, 2.0f), MoveTo::create(0.5f, move_location), NULL),
            CallFunc::create([=] {
            for (int i = 0; i < text_lb->getStringLength(); ++i) {
                auto letter = text_lb->getLetter(i);
                if (letter) {
                    letter->setOpacity(255);
                }
            }
            
        }),RemoveSelf::create(), NULL));

        ++ind;

	}
	else {
		if (callback) {
			callback();
		}
	}
}

void MexicanCactus_VM::playSyncText(float delay_time)
{
	CCLOG("Debug: Play Sync Text ");
	disableTouch(0);
    
    if (!current_item_info.co_dau){
        this->scheduleOnce([=](float dt_) {
            float dt = 0.0f;
            for (int i = 0; i < current_item_info.list_character.size(); ++i) {
                this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
                    AUDIO_ENGINE->playSound(current_item_info.list_character[i].audio);
                    for (int index : current_item_info.list_character[i].list_index) {
                        auto lb = text_lb->getLetter(index);
                        text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
                            text_lb->getLetter(index)->setColor(Color3B::RED);
                        }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
                            text_lb->getLetter(index)->setColor(Color3B::WHITE);
                        }), NULL));
                    }
                }), NULL));
                dt += 0.65f;
            }
            this->scheduleOnce([=](float) {
                //playSlowText();
                playNormalText();
            }, dt + 0.1f, "play sync text finnished");
        }, delay_time, "delay synctext");
    }else{
        
        this->scheduleOnce([=](float dt_) {
            float dt = 0.0f;
            for (int i = 0; i < current_item_info.list_character_sync_text.size(); ++i) {
                this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
                    AUDIO_ENGINE->playSound(current_item_info.list_character_sync_text[i].audio);
                    for (int index : current_item_info.list_character_sync_text[i].list_index) {
                        text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
                            text_lb->getLetter(index)->setColor(Color3B::RED);
                        }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
                            text_lb->getLetter(index)->setColor(Color3B::WHITE);
                        }), NULL));
                    }
                }), NULL));
                dt += 0.65f;
            }
            this->scheduleOnce([=](float) {
                text_lb->runAction(Sequence::create(CallFunc::create([=] {
                    this->setColorForText(Color3B::RED);
                }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
                    this->setColorForText(Color3B::WHITE);
                }), NULL));
                AUDIO_ENGINE->playSound(current_item_info.audio_ko_dau,false,[=](){
                    this->setColorForText(Color3B::WHITE);
                    text_dau->runAction(Sequence::create(CallFunc::create([=] {
                        text_dau->setColor(Color3B::RED);
                    }), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
                        text_dau->setColor(Color3B::WHITE);
                        playNormalText();
                    }), NULL));
                    AUDIO_ENGINE->playSound(current_item_info.dau_cau.audio,false,[=](){
                        
                    });
                });
                this->setColorForText(Color3B::RED);
            }, dt + 0.5f, "play sync text finnished");
        }, delay_time, "delay synctext");
    }
}

void MexicanCactus_VM::playNormalText()
{
	CCLOG("Debug: Play Normal Text");
    auto audio_id = AUDIO_ENGINE->play(current_item_info.normal_audio);
	this->setColorForText(Color3B::RED);
    if (text_dau) text_dau->setColor(Color3B::RED);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		if (duration <= 0) {
			duration = 0.5f;
		}
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
        if (text_dau){
            auto scale_vl = text_dau->getScale();
            auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
            auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
            auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
            auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
            auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
            text_dau->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
                if (text_dau) text_dau->setColor(Color3B::WHITE);
            }), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), NULL));
        }
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
             if (text_dau) text_dau->setColor(Color3B::WHITE);
		}), CallFunc::create([=] {
			//this->onNextWord();
			
			for (auto c : list_cactus_item) {
				c->onCorrectCactus(0.10f);
			}
			gaf_confetti->play(false, [=](gaf::GAFObject * obj) {
				//	gaf_confetti->removeFromParent();
				gaf_confetti->reset();
			});
			AUDIO_ENGINE->playSound(StringUtils::format("sounds/MexicanCactus_VM/mc_correct%d.mp3", item_index % 2 + 1));
			//// show skip
			//btnSkip->setVisible(true);
			//this->scheduleOnce([=](float dt_) {
			//	btnSkip->setVisible(false);
			//}, 11.0f, "hide skip");

			this->scheduleOnce([=](float dt_) {
				onNextWord();
			}, 12.10f, "on next word");
		}), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), NULL));  //DelayTime::create(12.10f),
	}), NULL));
}

void MexicanCactus_VM::onNextWord()
{
	CCLOG("Debug: Next Word");
    this->unschedule("inActive_Mode");
	item_index++;
	disableTouch();
	if (item_index >= (int)list_item_info.size()) {
		this->onNextGame();

	}
	else {
		changeInfoCactus();
		showCactus();
	}
}

void MexicanCactus_VM::onNextGame()
{
	CCLOG("Debug: Next Game");
	float dt = 0.15f;

	/*for (auto cactus : list_cactus_item) {
		cactus->hideCactus(dt);
		dt += 0.15f;
	}*/
	this->scheduleOnce([=](float dt_) {
		onFinishGame();
	}, dt, "end game");
}

void MexicanCactus_VM::showHint()
{
	if (wrong_count > 2) {
		text_hint->stopAllActions();
		if (wrong_count == 3 || wrong_count == 4) {
			std::vector<std::string> chain;
			chain.push_back("sounds/MexicanCactus_VM/make the word.mp3");
			chain.push_back(current_item_info.normal_audio);
			AUDIO_ENGINE->playChain(chain);
		}
		else if (wrong_count == 5) {
			text_hint->runAction(Sequence::create(FadeIn::create(0.25f), DelayTime::create(1.5f), FadeOut::create(0.25f), NULL));
			AUDIO_ENGINE->playSound(current_item_info.slow_audio);
		}
		else {
			text_hint->runAction(FadeIn::create(0.25f));
		//	AUDIO_ENGINE->playSound(current_item_info.slow_audio);
		}
	}


	
}

inline void MexicanCactus_VM::setColorForText(cocos2d::Color3B color_text)
{
	for (int i = 0; i < (int)text_lb->getString().size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}


void MexicanCactus_VM::enableTouch()
{
	touch_layout->setVisible(false);
}

void MexicanCactus_VM::disableTouch(int bg_opacity)
{
	touch_layout->setVisible(true);
	touch_layout->setBackGroundColorOpacity(bg_opacity);
}

void MexicanCactus_VM::showGuideGame(float delay_time)
{
	this->scheduleOnce([=](float dt) {
		disableTouch();
		auto move_cactus = list_cactus_item[0];
		auto start_pos = move_cactus->getPosition();
		auto end_index = 1;
		auto end_pos = list_cactus_item[end_index]->getPosition();
		MJMainGBase::showDragGuideGame(start_pos, end_pos,0, 0.10f);
		move_cactus->setLocalZOrder(15);
		//move_cactus->setScale(1.25f);
		move_cactus->runAction(Sequence::create(DelayTime::create(0.1f),ScaleTo::create(7.0f/24,1.25f),DelayTime::create(5.0f/24),MoveTo::create(0.6f, end_pos), DelayTime::create(0.25f), MoveTo::create(0.15f, start_pos), CallFunc::create([=] {
			enableTouch();
			move_cactus->setLocalZOrder(10);
			move_cactus->setScale(1.0f);
			hideTapGuideGame();
			showGuideGame(4.5f);
		}), NULL));
	},delay_time,"cactus_guide_game");
	
}

void MexicanCactus_VM::hideGuideGame()
{
    enableTouch();
	this->unschedule("cactus_guide_game");
	MJMainGBase::hideTapGuideGame();
}

void MexicanCactus_VM::initText()
{
	text_lb->setString(current_item_info.text);
	text_lb->setScale(1.0f);
	text_lb->setPosition(Vec2(screen_size.width / 2, 100));

	for (int i = 0; i < text_lb->getStringLength(); ++i) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setOpacity(0);
		}
	}
}

void MexicanCactus_VM::genCharacterIndex(MexicanCactus_VM::MEXICAN_CACTUS_INFO & info)
{
	// Gen character index	
	int start_index = 0;
	std::queue<int> underline_index;

	for (int i = 0; i < info.list_character.size(); ++i) {
        auto phonic_data = info.list_character[i];
        auto utf8 = StringUtils::StringUTF8(phonic_data.text);
        //check chữ
        int index = 0;
		for (int j = 0; j < utf8.length(); ++j) {
            CCLOG("test %c",phonic_data.text[j]);
            info.list_character[i].list_index.push_back(start_index + index);
            index++;
		}
		start_index += index;
	}
	for (int i = 0; i < info.list_character.size(); ++i) {
		for (int j = 0; j < info.list_character.size(); ++j) {
			if (info.list_character[i].text == info.list_character[j].text) {
				info.list_character[i].list_available_index.push_back(j);
			}
		}
	}
    if (info.co_dau){
        for (int i = 0; i < info.list_character.size(); ++i) {
            info.list_character_sync_text[i].list_index = info.list_character[i].list_index;
        }
    }
}

std::vector<Vec2> MexicanCactus_VM::getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding)
{
	std::vector<Vec2> locations;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(center.x - (number - 1) * (size.width + padding) / 2, center.y);
	for (int i = 0; i < number; ++i) {
		Vec2 pos(start_pos.x + i * (size.width + padding), start_pos.y);
		locations.push_back(pos);

	}
	return locations;
}

bool MexicanCactus_VM::checkCorrectIndex()
{
	bool resVal = true;
	for (auto cactus : list_cactus_item) {
		auto it = map_index_items.find(cactus);
		if (it != map_index_items.end()) {
			int d_index = it->second;
			bool found = false;
			for (auto index : cactus->getCactusInfo().correct_index) {
				if (index == d_index) {
					found = true;
					break;
				}
			}
			if (!found) {
				resVal = false;
				break;
			}
		}
	}
	return resVal;
}

void MexicanCactus_VM::onClickedSpeaker(cocos2d::Ref * sender)
{
    AUDIO_ENGINE->stopEffect(speak_id);
	speak_id = AUDIO_ENGINE->playEffect(current_item_info.normal_audio);
}

void MexicanCactus_VM::onClickedSkip(cocos2d::Ref * sender)
{
	CCLOG(__FUNCTION__);
	btnSkip->setVisible(false);
	for (auto c : list_cactus_item) {
		c->resetCactus();
	}
	gaf_confetti->reset();
	AUDIO_ENGINE->stopSound("sounds/MexicanCactus_VM/mc_correct1.mp3");
	AUDIO_ENGINE->stopSound("sounds/MexicanCactus_VM/mc_correct2.mp3");
	text_lb->stopAllActions();
	text_lb->runAction(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))));
	this->unschedule("on next word");
	this->unschedule("hide skip");
	onNextWord();
}

void MexicanCactus_VM::inActiveMode(){
    this->schedule([=](float){
          speak_id = AUDIO_ENGINE->playEffect(current_item_info.normal_audio);
    }, 10, "inActive_Mode");
}
bool MexicanCactus_VM::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    if (c == ' '){
        return true;
    }
    return false;
}
