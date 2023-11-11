//
//  SpaceShip_VM.h
//


#include "SpaceShip_VM.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include <math.h>

using namespace cocos2d;
SpaceShip_VM * SpaceShip_VM::createGame(std::string json_file, bool is_auto_play) {

	SpaceShip_VM * obj = new SpaceShip_VM();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(json_file, is_auto_play);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void SpaceShip_VM::didLoadFromCSB(std::string json_file, bool is_auto_play) {

	this->is_auto_play = is_auto_play;
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document value_data;
	/*bool is_convert_plist_2_json = true;
	if (is_convert_plist_2_json) {
		auto value_map = FileUtils::getInstance()->getValueMapFromFile(json_file);
		buffer_string = JsonDump::getInstance()->MapToString(value_map);
	}*/

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

}

void SpaceShip_VM::generateData(rapidjson::Value value_data) {
    
	rapidjson::Value& game_mode = value_data.FindMember("game_mode")->value;
	if (game_mode.HasMember("number_turnplay") && game_mode["number_turnplay"].IsInt()) {
		number_turnplay = game_mode["number_turnplay"].GetInt();

	}
    is_show_image = false;
    if(game_mode.HasMember("have_image") && game_mode["have_image"].IsBool())
        is_show_image = game_mode["have_image"].GetBool();

    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
    
	if (value_data.HasMember("data") && value_data["data"].IsArray()) {
		auto game_data = value_data["data"].GetArray();
		for (int i = 0; i < (int)game_data.Size(); i++) {

            auto item_data = game_data[i].GetObject();

            auto letter = item_data["text_full"].GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);

			SpaceShip_VM_INFO SpaceShip_VM_info;
            SpaceShip_VM_info.text = pImage->getText("name_1");
			SpaceShip_VM_info.audio_path_n = pImage->getAudios("name_1").front();
			SpaceShip_VM_info.audio_path_s = pImage->getAudios("name_n").front();
            SpaceShip_VM_info.image_path = pImage->path;

			// list distracting
			auto list_distracting = item_data["list_distracting"].GetArray();
			std::vector<SpaceShip_VM_Item::METEORITE_INFO> distracting_info;
			for (int z = 0; z < (int)list_distracting.Size(); z++) {
                auto item_id = list_distracting[z].FindMember("text")->value.GetString();
                auto item_Image = math::resource::ResHelper::getImage(images, item_id);
				SpaceShip_VM_Item::METEORITE_INFO meteorite_info;
				meteorite_info.character = item_Image->getText("name_1");
                meteorite_info.audio_path =item_Image->getAudios("name_1").front();
				distracting_info.push_back(meteorite_info);
			}
			// list phonic info
			auto list_character = pImage->getTextPhonic("name_p");
            auto list_audio = pImage->getAudioPhonic("name_p");
			for (int j = 0; j < (int)list_character.size(); j++) {
				CHARACTER_INFO character_info;
				character_info.meteorite_info.character = list_character[j];
				character_info.meteorite_info.audio_path = list_audio[j];
				character_info.list_distracting = getDistracting(character_info.meteorite_info.character, distracting_info);
				SpaceShip_VM_info.list_character.push_back(character_info);
			}
			genCharacterIndex(SpaceShip_VM_info);
			trimUnderline(SpaceShip_VM_info);
			list_item_info.push_back(SpaceShip_VM_info);
		}
		mj::helper::shuffle(list_item_info);
	}
}

void SpaceShip_VM::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->playBackgroundMusic("sounds/SpaceShip_VM/SpaceShip_VM_bg.mp3", 0.8f);
	this->loadFrames();
	disableTouch();
	blur_planet->runBlur();
	blur_star->runBlur();
	showGuideGame(12.0f);
	this->showIntroGame();
	if (is_auto_play) {
		auto lbl_auto_play = cocos2d::ui::Layout::create();
		lbl_auto_play->setContentSize(screen_size);
		lbl_auto_play->setTouchEnabled(true);
		this->addChild(lbl_auto_play, 100000000);
		autoPlay();
	}
}

void SpaceShip_VM::onSpeakerClicked(cocos2d::Ref *sender) {
    AUDIO_ENGINE->stopSound(question_vi);
	AUDIO_ENGINE->playSound(current_character.meteorite_info.audio_path);
}

void SpaceShip_VM::onExitTransitionDidStar() {
	MJMainGBase::onExitTransitionDidStart();
	PHONIC_MANAGER->clearCacheGaf();
}

void SpaceShip_VM::loadFrames() {
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	this->setTouchEnabled(true);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	this->addChild(touch_layout, 100000000);
	touch_layout->setTouchEnabled(true);

	auto bg = cocos2d::ui::ImageView::create("games/SpaceShip_VM/SpaceShip_VM_bg.png");
	this->addChild(bg);
	bg->setPosition(screen_size / 2);

	blur_planet = SpaceShip_VM_Blur::createBlur(SpaceShip_VM_Blur::PLANET_TYPE);
	this->addChild(blur_planet, 2);

	blur_star = SpaceShip_VM_Blur::createBlur(SpaceShip_VM_Blur::STAR_TYPE);
	this->addChild(blur_star, 3);

	blur_background = SpaceShip_VM_Blur::createBlur(SpaceShip_VM_Blur::BG_TYPE);
	this->addChild(blur_background, 4);
	blur_background->setCascadeOpacityEnabled(true);
	blur_background->setOpacity(0);
	blur_background->setVisible(false);


	PHONIC_MANAGER->cacheGAF("meteorite", "gaf/SpaceShip_VM/thienthach/thienthach.gaf");
	PHONIC_MANAGER->cacheGAF("ship", "gaf/SpaceShip_VM/ship/ship.gaf");

	ship = SpaceShip_VM_Item::createShip();
	this->addChild(ship, 8);
	ship->setPosition(Vec2(-500, screen_size.height / 2));
	ship->playSequence("fly", true);

	gachngang_layout = cocos2d::ui::Layout::create();
	this->addChild(gachngang_layout, 10000000);
	gachngang_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	gachngang_layout->setPosition(Vec2(screen_size.width / 2, screen_size.height - 50));

	text_lb = cocos2d::ui::Text::create("", PHONIC_MANAGER->getGameFontName(), 100);
	this->addChild(text_lb, 1000);
	text_lb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	auto image_size = cocos2d::Size(760, 420);
	img_character = cocos2d::ui::ImageView::create();
	this->addChild(img_character, 999);
	//img_character->ignoreContentAdaptWithSize(false);
	img_character->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	//img_character->setColor(cocos2d::Color3B::BLUE);
	//img_character->setScale9Enabled(true);
	//auto vitural_size = img_character->getVirtualRendererSize();
	//img_character->setCapInsets(cocos2d::Rect(1, 2, vitural_size.width - 2, vitural_size.height));
	img_character->setOpacity(0);
	img_character->setContentSize(image_size);

	speaker = cocos2d::ui::Button::create();
	speaker->loadTextures("games/SpaceShip_VM/loa icon.png", "games/SpaceShip_VM/loa icon.png");
	speaker->addClickEventListener(CC_CALLBACK_1(SpaceShip_VM::onSpeakerClicked, this));
	speaker->setPosition(Vec2(80, screen_size.height - 70));
	this->addChild(speaker, 10000);
}

void SpaceShip_VM::showText() {
	list_letter.clear();
	list_gachngang.clear();
	list_pos_letter.clear();
	current_item = list_item_info[index_item];
	auto width_gachngang = 81, w_add_gachngang = 5;
	auto num_gachngang = StringUtils::StringUTF8(current_item.text).length();
	for (int i = 0; i < num_gachngang; i++) {
		auto gachngang = cocos2d::ui::ImageView::create("games/SpaceShip_VM/SpaceShip_VM_gachchan.png");
		gachngang_layout->addChild(gachngang);
		gachngang->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		gachngang->setPositionX(i*(width_gachngang + w_add_gachngang));
		gachngang->setScale(0, 1.0f);
		gachngang->runAction(ScaleTo::create(0.25f, 1.0f));
		list_gachngang.push_back(gachngang);
	}
	gachngang_layout->setContentSize(cocos2d::Size(num_gachngang*(width_gachngang + w_add_gachngang) - w_add_gachngang, 50));
	//    gachngang_layout->setPositionX(screen_size.width/2 - gachngang_layout->getContentSize().width/2);

	text_lb->setScale(1.0f);
	text_lb->setString(current_item.text);
	auto pos_text = gachngang_layout->convertToWorldSpace(Vec2(0, list_gachngang[0]->getPositionY()));
	text_lb->setPosition(Vec2(screen_size.width / 2, pos_text.y + 10));
	for (int i = 0; i < (int)list_gachngang.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			auto pos_convert = text_lb->convertToNodeSpace(gachngang_layout->convertToWorldSpace(list_gachngang[i]->getPosition()));
			list_pos_letter.push_back(letter->getPosition() + Vec2(0, 0));
			CCLOG("y = %f", letter->getPositionY() - letter->getContentSize().height*0.5);
			letter->setPosition(Vec2(pos_convert.x + width_gachngang * 0.5, letter->getPositionY() - 23));
			letter->setVisible(false);
			list_letter.push_back(letter);
		}
	}
}

void SpaceShip_VM::createMeteorite() {
	list_meteorite.clear();
	current_character = current_item.list_character[index_character];
	mj::helper::shuffle(current_character.list_distracting);

	std::vector<SpaceShip_VM_Item::METEORITE_INFO> list_meteorite_info;
	list_meteorite_info.push_back(current_character.meteorite_info);
	list_meteorite_info.push_back(current_character.list_distracting[0]);
	list_meteorite_info.push_back(current_character.list_distracting[1]);
	mj::helper::shuffle(list_meteorite_info);
	AUDIO_ENGINE->playSound("sounds/SpaceShip_VM/thien_thach_bay_ra.mp3");
	for (int i = 0; i < 3; i++) {
		auto meteorite = SpaceShip_VM_Item::createMeteorite(list_meteorite_info[i]);
		this->addChild(meteorite, 9);
		auto pos = Vec2(screen_size.width - 250 - i % 2 * 80, 130 + 200 * i);
		pos.x = pos.x + 500;
		meteorite->setPosition(pos);
		meteorite->runAction(Sequence::create(DelayTime::create(0.2*(i % 2)), EaseCubicActionOut::create(MoveBy::create(1.0f, Vec2(-500, 0))), NULL));
		meteorite->playSequence("idle", true);
		meteorite->setDelegate(this);
		list_meteorite.push_back(meteorite);
	}

	this->runEffectGachNgang();
}
void SpaceShip_VM::removeAllMeteorute() {

}

void SpaceShip_VM::runEffectGachNgang() {
	for (int i = 0; i < (int)current_character.list_index.size(); i++) {
		auto index = current_character.list_index[i];
		CCLOG("index = %d", index);
		auto gach_ngang = list_gachngang[index];
		gach_ngang->runAction(Repeat::create(Sequence::create(CallFunc::create([=] {
			gach_ngang->setColor(Color3B::RED);
		}), DelayTime::create(0.3f), CallFunc::create([=] {
			gach_ngang->setColor(Color3B::WHITE);
		}), DelayTime::create(0.3f), NULL), 100000));
	}
}

void SpaceShip_VM::showIntroGame() {
	this->showText();
    inactiveMode(true, 10.0f);
	
	ship->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(1.0f, Vec2(180, screen_size.height / 2))), CallFunc::create([=] {
		this->createMeteorite();
		std::vector<std::string> chain;
		//chain.push_back(question_vi);
		chain.push_back(current_character.meteorite_info.audio_path);
        AUDIO_ENGINE->playSound(question_vi,false,[=] {
            AUDIO_ENGINE->playSound(current_character.meteorite_info.audio_path,false, [=] {
                this->enableTouch();
            });
        });
       
	}), NULL));
}

void SpaceShip_VM::onItemClicked(cocos2d::Ref *sender) {
	hideGuideGame();
    inactiveMode(false);
	this->disableTouch();
	auto item = static_cast<SpaceShip_VM_Item *>(sender);
	if (item->item_info.character == current_character.meteorite_info.character) {
		this->chooseRight(item);
	}
	else {
        AUDIO_ENGINE->playSound(item->item_info.audio_path);
		this->chooseWrong(item);
	}
}

void SpaceShip_VM::chooseRight(SpaceShip_VM_Item *item) {
	auto pos_goal = Vec2(ship->getPositionX(), item->getPositionY());
	auto duration = 1.0f / 1024 * ship->getPosition().distance(pos_goal);
	ship->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=] {
		AUDIO_ENGINE->playSound("sounds/SpaceShip_VM/shot.mp3");
		AUDIO_ENGINE->playSound(current_character.meteorite_info.character);
		ship->shoot(ship->convertToNodeSpace(item->getPosition()), [=] {
			AUDIO_ENGINE->playSound("sounds/SpaceShip_VM/shot_down.mp3");
			this->mergeCharacter(item->click_layout->convertToWorldSpace(item->text->getPosition()));
			item->explosive([=] {
//                this->enableTouch();
				item->setVisible(false);
			});
		});
	}), NULL));
}

void SpaceShip_VM::chooseWrong(SpaceShip_VM_Item *item) {

	AUDIO_ENGINE->playSound("sounds/SpaceShip_VM/wrong.mp3");
	auto duration = 0.5f;
	auto range = 20;
	auto shake = Repeat::create(Sequence::create(MoveBy::create(duration / 4, Vec2(-range, 0)),
		MoveBy::create(duration / 4, Vec2(range, 0)), NULL), 2);
	item->runAction(shake);
	item->playSequence("lose", false, [=] {
		item->playSequence("idle", true);
        this->enableTouch();
        inactiveMode(true);
	});
//    ship->playSequence("wrong", false, [=] {
//        ship->playSequence("fly", true);
//
//    });

}

void SpaceShip_VM::mergeCharacter(Vec2 pos_text) {
	auto char_lb = cocos2d::ui::Text::create(current_character.meteorite_info.character, PHONIC_MANAGER->getGameFontName(), 100);
	this->addChild(char_lb, 1000);
	char_lb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	char_lb->setPosition(pos_text);
	auto gachchan = list_gachngang[current_character.list_index[0]];
	auto pos_goal = gachngang_layout->convertToWorldSpace(gachchan->getPosition());
	char_lb->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(pos_goal.x + 40.5, pos_goal.y + char_lb->getContentSize().height / 4)), CallFunc::create([=] {
		for (int i = 0; i < (int)current_character.list_index.size(); i++) {
			auto letter = list_letter[current_character.list_index[i]];
			letter->setVisible(true);
			list_gachngang[current_character.list_index[i]]->stopAllActions();
			list_gachngang[current_character.list_index[i]]->setColor(Color3B::WHITE);
		}
		char_lb->removeFromParent();
		// remain meteorite before next character
		AUDIO_ENGINE->playSound("sounds/SpaceShip_VM/all_meteoroid_destroy.mp3");
		for (int i = 0; i < (int)list_meteorite.size(); i++) {
			auto meteorite = list_meteorite[i];
			if (meteorite && meteorite->item_info.character != current_character.meteorite_info.character) {
				/*auto pos_add = Vec2(-(i+1)*100, -screen_size.height);
				auto pos_goal = meteorite->getPosition() + pos_add;
				auto vector = meteorite->getPosition() - pos_goal;
				auto angle = atan2(vector.y, vector.x);
				angle = angle * (180 / M_PI);
				meteorite->setRotation(-angle);
				auto duration = 1.0f/1024*meteorite->getPosition().distance(pos_goal);

				meteorite->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
					meteorite->removeFromParent();
				}), NULL));*/
				meteorite->playSequence("no", false, [=] {
					meteorite->removeFromParent();
				});
				auto text = cocos2d::utils::findChild(meteorite->click_layout, "lbl_info");
				text->runAction(FadeOut::create(0.5f));
			}
		}
		list_meteorite.clear();
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			this->onNextCharacter();
		}), NULL));
	}), NULL));

}

void SpaceShip_VM::onNextCharacter() {
	this->disableTouch();
	index_character++;
	if (index_character >= (int)current_item.list_character.size()) {
        inactiveMode(false);
		this->mergeWord();
	}
	else {
        inactiveMode(true,10.0f);
		this->createMeteorite();
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			AUDIO_ENGINE->playSound(current_character.meteorite_info.audio_path, false, [=] {
				this->enableTouch();
			});
		}), NULL));
	}
}

void SpaceShip_VM::onNextWord() {
	index_character = 0;
	auto duration = 1.0f / 1024 * ship->getPosition().distance(screen_size / 2);
	auto pos_x = ship->getPositionX();
	ship->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.5f, Vec2(ship->getPositionX(), screen_size.height / 2))), CallFunc::create([=] {
		blur_background->setVisible(true);
		blur_background->runBlurFast([=] {
			ship->playSequence("fly", true);
			// ship->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(duration, Vec2(pos_x, screen_size.height/2))), NULL));
		});
		ship->playSequence("fly_fast", true);
		blur_background->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(2.75f), CallFunc::create([=] {
			this->showIntroGame();
		}), NULL));
	}), EaseBackIn::create(MoveTo::create(duration, screen_size / 2)), NULL));
}

void SpaceShip_VM::mergeWord() {
	this->disableTouch();
    inactiveMode(false);
	ship->runAction(EaseSineIn::create(MoveBy::create(1.0f, Vec2(0, -screen_size.height))));
	this->runAction(Sequence::create(DelayTime::create(0.35f), CallFunc::create([=] {
		//        AUDIO_ENGINE->playSound("sounds/gingerbreadman/gbm_whop.mp3");
		auto duration = 0.5f;
		for (int i = 0; i < (int)list_gachngang.size(); i++) {
			auto gachngang = list_gachngang[i];
			gachngang->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([gachngang] {
				gachngang->removeFromParent();
			}), NULL));
		}
		list_gachngang.clear();
		auto text_lb_pos = Vec2(screen_size.width / 2, screen_size.height*0.4);
		if (is_show_image && !current_item.image_path.empty()) {
			auto image_size = cocos2d::Size(760, 420);
			text_lb_pos.y -= image_size.height / 2;
			auto image_pos = cocos2d::Vec2(screen_size.width / 2, screen_size.height * 0.65);

			img_character->setPosition(cocos2d::Vec2(screen_size.width / 2, screen_size.height + image_size.height));
			img_character->loadTexture(current_item.image_path);
			auto scale = image_size.height / img_character->getContentSize().height;
			img_character->setScale(scale);
			img_character->runAction(Sequence::create(DelayTime::create(0.5f),
				Spawn::create(EaseSineIn::create(MoveTo::create(duration, image_pos)), FadeIn::create(duration), NULL), NULL));
		}
		text_lb->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(EaseSineIn::create(MoveTo::create(duration, text_lb_pos)),
			ScaleTo::create(duration, 1.7f), NULL), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playSyncText();
		}), NULL));
		for (int i = 0; i < (int)list_pos_letter.size(); i++) {
			list_letter[i]->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(duration, list_pos_letter[i]), NULL));
		}
	}), NULL));
}
void SpaceShip_VM::playSyncText()
{
	float dt = 0.0f;
	for (int i = 0; i < current_item.list_character.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->playSound(current_item.list_character[i].meteorite_info.audio_path);
			for (int index : current_item.list_character[i].list_index) {
				text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::RED);
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::WHITE);
				}), NULL));
			}
		}), NULL));
		dt += 1.0f;
	}
	scheduleOnce([=](float dt) {
		playSlowText();
	}, dt, "synctext finish");
}
void SpaceShip_VM::playSlowText() {
	auto audio_path = std::string(current_item.audio_path_s);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		this->setColorForText(Color3B::RED);
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 20));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -20));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));
}
void SpaceShip_VM::playNormalText() {
	auto audio_id = AUDIO_ENGINE->play(current_item.audio_path_n);
	this->setColorForText(Color3B::RED);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(1.0f), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), DelayTime::create(0.25f), CallFunc::create([=] {
			index_item++;
			if (index_item >= (int)list_item_info.size() || index_item >= number_turnplay) {
				this->onFinishGame();
			}
			else {
				this->onNextWord();
			}
		}), NULL));
		img_character->runAction(Sequence::create(DelayTime::create(duration + 1.0f),
			Spawn::create(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, screen_size.height))), FadeOut::create(0.5f), NULL), NULL));
	}), NULL));
}

void SpaceShip_VM::setColorForText(Color3B color_text) {
	for (int i = 0; i < (int)list_letter.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

std::vector<SpaceShip_VM_Item::METEORITE_INFO> SpaceShip_VM::getDistracting(std::string key, std::vector<SpaceShip_VM_Item::METEORITE_INFO> distracting)
{
	std::vector<SpaceShip_VM_Item::METEORITE_INFO> res;
	auto key_length = key.size();
	for (auto dis : distracting) {
		if (dis.character != key && dis.character.size() == key_length) {
			res.push_back(dis);
		}
	}
	int pad = 1;
	while (res.size() < 2) {
		for (auto dis : distracting) {
			if (std::abs(int(dis.character.size() - key_length)) == pad) {
				res.push_back(dis);
			}
		}
		++pad;
	}
	mj::helper::shuffle(res);
	return res;
}

void SpaceShip_VM::genCharacterIndex(SpaceShip_VM_INFO &ship_info)
{
    int start_index = 0;
    std::queue<int> underline_index;
    for (int i = 0; i < ship_info.list_character.size(); ++i) {
        auto phonic_data = ship_info.list_character[i].meteorite_info;
        //check chữ
        int index = 0;
        for (int j = 0; j < phonic_data.character.size(); ++j) {
            if (phonic_data.character[j] == '_') {
                //underline_index
                underline_index.push(start_index + index);
                index++;
            }
            else {
                if (underline_index.empty() || underline_index.front() > start_index) {
                    CCLOG("test %c",phonic_data.character[j]);
                    if (!checkRomaji(phonic_data.character[j])){
                        if (j == 0){
                            ship_info.list_character[i].list_index.push_back(start_index + index);
                            index++;
                        }else{
                            if(checkRomaji(phonic_data.character[j-1])){
                                ship_info.list_character[i].list_index.push_back(start_index + index);
                                index++;
                            }
                        }
                    }else{
                        ship_info.list_character[i].list_index.push_back(start_index + index);
                        index++;
                    }

                }
                else {
                    ship_info.list_character[i].list_index.push_back(underline_index.front());
                    underline_index.pop();
                }
            }
        }
        start_index += index;
    }
	// Gen character index
//	int start_index = 0;
//	std::queue<int> underline_index;
//	for (int i = 0; i < ship_info.list_character.size(); ++i) {
//		auto phonic_data = ship_info.list_character[i].meteorite_info;
//		for (int j = 0; j < phonic_data.character.size(); ++j) {
//			if (phonic_data.character[j] == '_') {
//				//underline_index
//				underline_index.push(start_index + j);
//			}
//			else {
//				if (underline_index.empty() || underline_index.front() > start_index) {
//					ship_info.list_character[i].list_index.push_back(start_index + j);
//				}
//				else {
//					ship_info.list_character[i].list_index.push_back(underline_index.front());
//					underline_index.pop();
//				}
//			}
//		}
//		start_index += phonic_data.character.size();
//	}
}

void SpaceShip_VM::trimUnderline(SpaceShip_VM_INFO & ship_info)
{
	for (int i = 0; i < ship_info.list_character.size(); ++i) {
		auto text = &ship_info.list_character[i].meteorite_info.character;
		int index = text->find("__");
		while (index != std::string::npos) {
			text->erase(text->begin() + index, text->begin() + index + 1);
			index = text->find("__");
		}
	}
}

void SpaceShip_VM::showGuideGame(float dt)
{
	if (list_meteorite.size() > 0 && !touch_layout->isVisible()) {
		for (auto meteo : list_meteorite) {
			if (meteo->item_info.character == current_character.meteorite_info.character) {
				showTapGuideGame(meteo->getPosition(), dt);
				break;
			}
		}
	}
}

void SpaceShip_VM::hideGuideGame()
{
	MJMainGBase::hideTapGuideGame();
}

void SpaceShip_VM::disableTouch() {
	touch_layout->setVisible(true);
}

void SpaceShip_VM::enableTouch() {
	touch_layout->setVisible(false);
}

// auto play
#define auto_play_delay_time 1.5f
void SpaceShip_VM::autoPlay()
{
	this->schedule([=](float dt) {
		if (!touch_layout->isVisible()) {
			if (list_meteorite.size() > 0) {
				bool can_play = true;
				for (auto meteo : list_meteorite) {
					if ( meteo->getPositionX() > screen_size.width) {
						can_play = false;
						break;
					}
				}
				if (can_play) {
					auto_play_index = (auto_play_index + 1) % list_meteorite.size();
					onItemClicked(list_meteorite[auto_play_index]);

				}
			}
		}
	}, auto_play_delay_time, 10000000, 2.0f, "auto play");
}


void SpaceShip_VM::inactiveMode(bool enabled,float delay)
{
    if(enabled)
    {
        if(this->isScheduled("inactive_mode")){
            this->unschedule("inactive_mode");
            AUDIO_ENGINE->stopSound(question_vi);
            AUDIO_ENGINE->stopSound(current_character.meteorite_info.audio_path);
        }
        this->schedule([=](float dt)
        {
            std::vector<std::string> chain;
            chain.push_back(std::string(question_vi));
            chain.push_back(current_character.meteorite_info.audio_path);
            AUDIO_ENGINE->playChain(chain);
        },12.0f,1000,delay,"inactive_mode");
    }
    else
    {
        this->unschedule("inactive_mode");
        AUDIO_ENGINE->stopSound(question_vi);
        AUDIO_ENGINE->stopSound(current_character.meteorite_info.audio_path);
    }
}
bool SpaceShip_VM::checkRomaji(char c){
    if (c >= 'a' && c <= 'z'){
        return true;
    }
    if (c >= 'A' && c <= 'Z'){
        return true;
    }
    return false;
}
