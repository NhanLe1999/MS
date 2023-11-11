//
//  SkiingRacing.cpp
//  mj_gamelib-mobile
//
//  Created by AnhB Nguyen on 3/20/19.
//

#include "SkiingRacing.h"
#include "MJHelper.h"

#define CSB_NAME "csb/game/SkiingRacing/SkiingRacing.csb"
#define SPEED_TURN_1 350
#define SPEED_NORMAL 550
INITIALIZE_READER (SkiingRacing);

std::once_flag SkiingRacing_Reader;
SkiingRacing* SkiingRacing::createGame(std::string json_file, bool is_Autoplay ){
    std::call_once(SkiingRacing_Reader, []{
        REGISTER_CSB_READER(SkiingRacing);
		REGISTER_CSB_READER(SkiingRacing_Race);
		REGISTER_CSB_READER(SkiingRacing_Map);
    });
    
    auto p = static_cast<SkiingRacing*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, is_Autoplay);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void SkiingRacing::didLoadFromCSB(std::string json_file, bool is_Autoplay){
    generateData(json_file);
	showIntro();
}

void SkiingRacing::generateData(std::string json_file)
{
	//    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	auto jsonString = FileUtils::getInstance()->getStringFromFile(json_file);
	if (!jsonString.length())
		return;

	//    std::string schemaPath = "json/skiingrace/schema.json";
	//    std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
	//    if (!schemaJson.length())
	//        return;

	using namespace rapidjson;
	//    Document sd;
	//    if (sd.Parse(schemaJson.c_str()).HasParseError())
	//        return;
	//
	//    SchemaDocument schema(sd);

	Document resultDocument;
	if (resultDocument.Parse(jsonString.c_str()).HasParseError())
		return;

	//    SchemaValidator sv(schema);
	//    if (!resultDocument.Accept(sv)) {
	//        StringBuffer sb;
	//        sv.GetInvalidSchemaPointer().StringifyUriFragment(sb);
	//        CCLOG("Invalid schema: %s\n", sb.GetString());
	//        CCLOG("Invalid keyword: %s\n", sv.GetInvalidSchemaKeyword());
	//        sb.Clear();
	//        sv.GetInvalidDocumentPointer().StringifyUriFragment(sb);
	//        CCLOG("Invalid document: %s\n", sb.GetString());
	//        return;
	//    }

	if (m_questions.size() != 0) {
		m_questions.clear();
	}

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && resultDocument.HasMember("audio_intro_vi")){
       auto audio_intro = resultDocument["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
	auto games = resultDocument["games"].GetArray();
	for (rapidjson::SizeType i = 0; i < games.Size(); i++) {
		SkiingRacingQuestion question;
		auto gameObject = games[i].GetObject();

		auto letter = gameObject["correctAnswer"].GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);
		question.correctAnswer = pImage->getText("name_1");
		question.correctAudio = pImage->getAudios("name_1").front();

		auto answers = gameObject["answers"].GetArray();
		for (rapidjson::SizeType j = 0; j < answers.Size(); j++)
		{
			auto ans_id = answers[j].GetString();
			auto aImage = math::resource::ResHelper::getImage(images, ans_id);
            question.answers.push_back({aImage->getText("name_1"),aImage->getAudios("name_1").front()});
		}
		m_questions.push_back(question);
	}
}

void SkiingRacing::loadFrame(){
	AUDIO_ENGINE->playSound("sounds/skiingracing/start_music.mp3", true);
	
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    select_layout = utils::findChild<Layout*>(this, "select_layout");
	hidden_layout = utils::findChild<Layout*>(this, "hidden_layout");
	_bg = utils::findChild<Layout*>(this, "_bg");
	showAudioCorrect();

	current_question = 0;
	
	look_for_jump = false;
    // tao cac race
	for (int i = 0; i < m_questions.size(); i++) {
		auto race = SkiingRacing_Race::createRace(m_questions[i],i);
		race->setAnchorPoint(Vec2(0, 0));
		_bg->addChild(race, 2);
		race->setPosition(Vec2(hidden_layout->getPositionX() + (race->getContentSize().width-5)*i, hidden_layout->getPositionY()));
		races.push_back(race);
	}
	
	race_finish = SkiingRacing_Race::createRace();
	race_finish->setAnchorPoint(Vec2(0, 0));
	_bg->addChild(race_finish, 2);
	race_finish->setPosition(Vec2(hidden_layout->getPositionX() + (race_finish->getContentSize().width - 5)*m_questions.size(), hidden_layout->getPositionY()));
	
	long_races = race_finish->convertToWorldSpace(utils::findChild<Sprite*>(race_finish, "bg_finish")->getPosition()).x - hidden_layout->getPosition().x + 100;
	result = 1;
	//
    std::vector<std::string> character_name = {
        "nat", "dan", "finn"
    };
//    if (CONFIG_MANAGER->isGameVMonkey()){
//        character_name = {
//            "mèo", "chó", "cáo"
//        };
//    }
    for(int i = 0; i < 3; i++){
        auto name = character_name[i];
        
        auto bg_layout = utils::findChild<Layout*>(this, StringUtils::format("bg_%s", name.c_str()));
		bg_layout->setName(name);
        auto bg_pos = Vec2(bg_layout->getContentSize()/2);
       
        GAFWrapper* bg_gaf = GAFWrapper::createGAF(StringUtils::format("gaf/skiingracing/Rc_selbg_%s/Rc_selbg_%s.gaf", name.c_str(), name.c_str()));
        bg_layout->addChild(bg_gaf, 1);
        bg_gaf->setPosition(bg_pos);
        bg_layout->setTag(i);
        bg_gaf->setName("bg_gaf");
        bg_layouts.push_back(bg_layout);
        
        auto character = GAFWrapper::createGAF(StringUtils::format("gaf/skiingracing/Rc_%s/Rc_%s.gaf", name.c_str(), name.c_str()));
        character->setAnchorPoint(Point(0.4, 0.5));
		if (name == "dan") {
			character->setAnchorPoint(Point(0.375, 0.5));
		}
        character->setScale(0.1);
        bg_layout->addChild(character, 1);
        character->setPosition(Vec2(bg_pos.x, bg_pos.y - 50));
        character->setName("character");
		character->setTag(0);
        auto text_name = name;
        if (CONFIG_MANAGER->isVietnameseStories()){
            if (name == "nat") text_name = "mèo";
            if (name == "dan") text_name = "chó";
            if (name == "finn") text_name = "cáo";
        }
        auto text = Text::create(mj::helper::to_upper(text_name) ,CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF", 60.0f);
        bg_layout->addChild(text);
        text->setVisible(false);
        text->setPosition(character->getPosition());
        text->setName("text");
    }
    
    Vector<FiniteTimeAction*> actions;
    auto fade_in = CallFunc::create([=](){
		AUDIO_ENGINE->play(CONFIG_MANAGER->isVietnameseStories()?"sounds/skiingracing/choose_your_character_vi.mp3":"sounds/skiingracing/choose_your_character.mp3");
        for(int i = 0; i < bg_layouts.size(); i++){
            auto bg_gaf = utils::findChild<GAFWrapper*>(bg_layouts[i], "bg_gaf");
            auto character = utils::findChild<GAFWrapper*>(bg_layouts[i], "character");
           
            auto delay = DelayTime::create(0.3f + i * 0.5f);
            auto func = CallFunc::create([=]() {
                AUDIO_ENGINE->play("sounds/skiingracing/choose.mp3");
                bg_gaf->playSequence("fade_in", false, [=](gaf::GAFObject* obj, std::string path){
                    
                });
                character->runAction(Spawn::create(ScaleTo::create(0.1, 1), CallFunc::create([=](){
                    character->playSequence("fade_in", false);
                }) , NULL));
            });
            bg_gaf->runAction(Sequence::createWithTwoActions(delay, func));
        }
    });
    
    auto delay1 = DelayTime::create(2.5);
    
    auto idle = CallFunc::create([=](){
        for(int i = 0; i < bg_layouts.size(); i++){
            auto bg_gaf = utils::findChild<GAFWrapper*>(bg_layouts[i], "bg_gaf");
            auto text = utils::findChild<Text*>(bg_layouts[i], "text");
            bg_gaf->playSequence("gcd", false, [=](gaf::GAFObject* obj, std::string path){
                 text->setVisible(true);
                text->setLocalZOrder(2);
                bg_gaf->playSequence("idle", true);
                text->runAction(EaseOut::create(MoveBy::create(0.2, Vec2(0, 250)), 2));
				
            });
        }

    });
	auto delay2 = DelayTime::create(1);
	auto unlook = CallFunc::create([=]() {
		
		look_select_character = false;
	});
    actions.pushBack(fade_in);
    actions.pushBack(delay1);
    actions.pushBack(idle);
	actions.pushBack(delay2);
	actions.pushBack(unlook);
	
	
    this->runAction(Sequence::create(actions));
}
void SkiingRacing::onSelectCharacter(Ref* sender) {
	if (!look_select_character) {
		AUDIO_ENGINE->play("sounds/skiingracing/selected.mp3");
		look_select_character = true;
		CCLOG("gaf_name:%i", ((Layout*)sender)->getTag());

		auto layout = ((Layout*)sender);
		main_character = utils::findChild<GAFWrapper*>(layout, "character");
		main_character->setName(layout->getName());
		auto pos = main_character->getParent()->convertToWorldSpace(main_character->getPosition());
		main_character->retain();
		main_character->removeFromParent();
		root_layout->addChild(main_character);
        main_character->release();
		main_character->setPosition(pos);

		main_character->playSequence("select", false, [=](gaf::GAFObject* obj, string path) {
			main_character->playSequenceWithPause("run", true, [=](gaf::GAFObject* obj, string path) {
				main_character->playSequence("run_idle", false);
			});
			startGame();
		});		
		auto bg = utils::findChild<GAFWrapper*>(layout, "bg_gaf");
		bg->playSequence("select", false, [=](gaf::GAFObject* obj, string path) {
			bg->playSequence("select_idle", false);

		});

		for (auto b : bg_layouts) {
			if (b->getTag() != layout->getTag()) {				
				utils::findChild<GAFWrapper*>(b, "bg_gaf")->playSequence("fade_out", false, [=](gaf::GAFObject* obj, string path) {
//                    obj->removeFromParent();
                    obj->runAction(FadeOut::create(0.2));
				});
				if (auto text = utils::findChild<Text*>(b, "text" )) {
					//text->runAction(FadeIn::create(0.2));
					text->removeFromParent();
				}
				auto character = utils::findChild<GAFWrapper*>(b, "character");
				character->setName(b->getName());
				character->runAction(Sequence::create(Spawn::create(RotateTo::create(0.4, 360), ScaleTo::create(0.4, 0.1), NULL), CallFunc::create([=]() {
					character->setVisible(false);
					character->setScale(0.8);
					character->retain();
					character->removeFromParent();
					root_layout->addChild(character);
                    character->release();
					characters.push_back(character);
				}), NULL));
			}
		}
	}
}

void SkiingRacing::startGame(){

    if (!CONFIG_MANAGER->isVietnameseStories()){
        AUDIO_ENGINE->playSound("sounds/skiingracing/characters_in_race.mp3", false);
    }
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->play(CONFIG_MANAGER->isGameVMonkey()?question_vi:"sounds/skiingracing/question.mp3");
    }, CONFIG_MANAGER->isVietnameseStories()?2.f:3.7f, "question mp3");
	move_speed = SPEED_TURN_1; // cau dau tien chay cham thoi
	is_visible_answer = false;
	this->schedule(schedule_selector(SkiingRacing::update_main_character_jump));
    std::vector<Vec2> positions = {
        Vec2(screen_size.width / 2 - 712, 700), Vec2(screen_size.width / 2 - 712, 500), Vec2(screen_size.width / 2 - 312, 250),
    };
	float delta_time = 2;
	main_character->runAction(Spawn::create(ScaleTo::create(delta_time, 0.8), MoveTo::create(delta_time, positions[2]), nullptr));
	main_character->setLocalZOrder(2);
	auto b = hidden_layout->getPositionX() - select_layout->getPositionX() + select_layout->getContentSize().width / 2 ;
    select_layout->runAction(Sequence::create(MoveBy::create(delta_time, Vec2(-b, 0)), CallFunc::create([=]{
		main_character->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			main_character->playSequence("stop_idle", true);
		});
        int i = 0;
        for (auto c : characters) {
            c->retain();
			c->setPosition(hidden_layout->convertToNodeSpace(positions[i]));
			c->removeFromParentAndCleanup(false);
			hidden_layout->addChild(c, 100);
            c->release();
			c->setLocalZOrder(i);
            i++;
            c->setVisible(true);
            c->playSequence("run", true);
			c->runAction(Sequence::createWithTwoActions(MoveBy::create(1, Vec2(400, 0)), CallFunc::create([=]() {
				c->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
					c->playSequence("stop_idle", true);
				});
			})));
        }       
    }), CallFunc::create([=]{
        select_layout->removeFromParent();
    }) ,NULL));
	_bg->runAction(MoveBy::create(delta_time, Vec2(-b, 0)));
	hidden_layout->runAction(MoveBy::create(delta_time, Vec2(-b-100, 0)));
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->stopSound("sounds/skiingracing/start_music.mp3");
		AUDIO_ENGINE->playSound("sounds/skiingracing/music_bg.mp3", true,nullptr,0.75);
		
		for (int i = 0; i < characters.size(); i++) {
			characters[i]->runAction(RepeatForever::create(MoveBy::create(1, Vec2(move_speed - ((i==0)?20:50), 0))));
			characters[i]->playSequence("start", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				characters[i]->playSequence("run_barry_run", true);
			});
		}
		main_character->playSequence("start", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			main_character->playSequence("run_barry_run", true);
		});
		this->schedule(schedule_selector(SkiingRacing::update_character_other));
		
		this->showQuestion();
		// create map
		map_race = SkiingRacing_Map::createMap(characters[0]->getName(), characters[1]->getName(),main_character->getName());
		map_race->setAnchorPoint(Vec2(0.5, 0));
		map_race->setPosition(Vec2(screen_size.width/2,0));
		this->addChild(map_race, 1000);

		//
	}, delta_time+3., "START");
   
    
    //    AUDIO_ENGINE->play("sounds/skiingracing/question.mp3", false, 1.0, [=]() {
    //showNextQuestion();
    //    });
    
}


void SkiingRacing::showAudioCorrect()
{
    // guide btn
	_speaker = ui::Button::create("games/skiingracing/speaker.png");
    auto visibleSize = Director::getInstance()->getVisibleSize();
	_speaker->setPosition(cocos2d::Vec2(_speaker->getContentSize().width / 2 + 20, visibleSize.height - _speaker->getContentSize().height / 2));
    root_layout->addChild(_speaker);
	_speaker->setOpacity(0);
	_speaker->addClickEventListener([=](Ref*) {
        AUDIO_ENGINE->playSound(m_questions[current_question].correctAudio);
    });
    
    // AUDIO_ENGINE->play("sounds/skiingracing/question.mp3", false, 1.0, [=](){
    
}

void SkiingRacing::createObstacle(){
    
}

void SkiingRacing::showQuestion()
{
	/*auto bg1 = utils::findChild<Sprite*>(this, "bg_1");
	bg1->setPosition(Vec2(0, 550));
	bg1->setTexture("games/skiingracing/rc_bg_without_start.png");*/

	look_for_jump = false;
	float distance = race_width - 1130 - (screen_size.width - 1024)/2;
	float time = distance / move_speed;
	if (current_question == 0) {
		hidden_layout->runAction(EaseInOut::create(MoveBy::create(time, Vec2(-distance, 0)), 1.3f));
		_bg->runAction(Sequence::createWithTwoActions(EaseInOut::create(MoveBy::create(time, Vec2(-distance, 0)), 1.3f), CallFunc::create([=]() {
			main_character->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				main_character->playSequence("stop_idle", true);
			}); })));
	}
	else {
		hidden_layout->runAction(EaseOut::create(MoveBy::create(time, Vec2(-distance, 0)), 1.f));
		_bg->runAction(Sequence::createWithTwoActions(EaseOut::create(MoveBy::create(time, Vec2(-distance, 0)), 1.f), CallFunc::create([=]() {
			main_character->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				main_character->playSequence("stop_idle", true);
			}); })));
	}
	
	//this->schedule(schedule_selector(SkiingRacing::update_bg));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    std::vector<std::pair<std::string,std::string>> answers = m_questions[current_question].answers;
	std::string correctAnswer = m_questions[current_question].correctAnswer;
    std::random_shuffle(answers.begin(), answers.end());
	float delay_show_answer = (current_question == 0) ? 3.f : 1.f;
	this->scheduleOnce([=](float) {
		//AUDIO_ENGINE->play(m_questions[current_question].correctAudio);
		AUDIO_ENGINE->play(m_questions[current_question].correctAudio, false, 1.0, [=]() {
			_speaker->runAction(FadeIn::create(0.2));
			});
	}, delay_show_answer +2*0.15+0.3, "sound correct");
	this->scheduleOnce([=](float) {
		auto pos = 500;
		auto offset = 200;
		AUDIO_ENGINE->playSound("sounds/skiingracing/question_show.mp3", false);
		for (auto i = 0; i < answers.size(); i++) {
			auto btn = ui::Button::create("games/skiingracing/button.png");
			btn->setPosition(cocos2d::Vec2(visibleSize.width/2 + 262, pos));
			btn->setZoomScale(0);
			btn->setScale(0.25f);
			auto scale = btn->getScale();

			// appearing btn animation
			btn->setScale(0);
			auto delay = DelayTime::create(i*0.15f);
			auto popSound = CallFunc::create([=]() {
				is_visible_answer = true;
				AUDIO_ENGINE->play("sounds/skiingracing/pop.mp3");
			});
			auto zoomAppear = ScaleTo::create(0.3f, scale);
			btn->runAction(Sequence::create(delay, popSound, zoomAppear, nullptr));

			root_layout->addChild(btn, 2);
			pos -= offset;

            auto label = Label::create(answers[i].first, CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF", 40.0f);
			label->setPosition(btn->getContentSize() / 2);
			label->setScale(4);
			label->setColor(Color3B(0xA0, 0x2A, 0x1E));
			btn->addChild(label);
            if (answers[i].first == correctAnswer) {
				pos_right = btn->getPosition();
				this->inActiveMode();
				
			}
			btn->addClickEventListener([=](Ref* sender) {
                
                this->hideTapGuideGame();
				GAFWrapper* effect = nullptr;

                if (answers[i].first != correctAnswer) {
					unschedule("active_mode");
					this->inActiveMode(8);
                    if (CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->play(answers[i].second);
					AUDIO_ENGINE->play("sounds/skiingracing/wrong.mp3");
					btn->loadTextureNormal("games/skiingracing/button_wrong.png");
				}
				else {
					unschedule("active_mode");
                    ((ui::Button*)sender)->setTouchEnabled(false);
					AUDIO_ENGINE->play(("sounds/skiingracing/correct.mp3"), false, 1.0f,[=]() {
						AUDIO_ENGINE->play(answers[i].second);
					});

					effect = GAFWrapper::createGAF("gaf/skiingracing/effect.gaf");
					effect->play(true);
					effect->setPosition(btn->getPosition());
					root_layout->addChild(effect);

				}

				btn->stopAllActions();
				auto scaleUp = ScaleTo::create(0.2f, 1.5f * scale);
				auto scaleDown = ScaleTo::create(0.2f, scale);
				auto clearAction = CallFunc::create([=]() {
                    if (effect){
						effect->removeFromParent();
                    }
                    if (answers[i].first == correctAnswer) {
						onCorrectAnswer();
						removeAllAnswerBtns();
					}
					else {
						btn->loadTextureNormal("games/skiingracing/button.png");
				
					}
				});
				btn->runAction(Sequence::create(EaseBounceOut::create(scaleUp),scaleDown, clearAction,nullptr));

			});
			m_answerBtns.push_back(btn);
		}
	}, delay_show_answer, "hien dap an");
   

}

void SkiingRacing::onCorrectAnswer() {
	is_visible_answer = false;
	//main_character->playSequence("start", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		main_character->playSequence("run_barry_run", true);
	//});
	if (current_question < m_questions.size() - 1) {
		if (current_question == 0) {
			move_speed = SPEED_NORMAL;
			for (int i = 0; i < characters.size(); i++) {
				if (characters[i]->getTag() == 0) {
					characters[i]->stopAllActions();
					characters[i]->runAction(RepeatForever::create(MoveBy::create(1, Vec2(move_speed - ((i == 0) ? 80 : 130), 0))));
				}
			}
		}
		float deltaX = races[current_question + 1]->convertToWorldSpace(utils::findChild<Node*>(races[current_question + 1], "Node_start")->getPosition()).x - 200;
		hidden_layout->stopAllActions();
		_bg->stopAllActions();
		hidden_layout->runAction(/*EaseIn::create*/(MoveBy::create((deltaX) / move_speed, Vec2(-deltaX, 0))/*, 1.3f*/));
		_bg->runAction(Sequence::create(/*EaseIn::create*/(MoveBy::create((deltaX) / move_speed, Vec2(-deltaX, 0))/*, 1.3f*/), CallFunc::create([=]() {
			if (current_question < m_questions.size() - 1) {
				current_question++;
				
				showQuestion();
			}
		}), nullptr));
	}
	else {
		// main character ve dich
		float distance = race_finish->convertToWorldSpace(utils::findChild<Sprite*>(race_finish, "bg_finish")->getPosition()).x - root_layout->convertToWorldSpace(main_character->getPosition()).x + 100;
		// them theo yeu cau
		distance;
		hidden_layout->stopAllActions();
		_bg->stopAllActions();
		// lam tram tu tu
		
		
		hidden_layout->runAction(Sequence::create(MoveBy::create((distance) / move_speed, Vec2(-distance, 0)), MoveBy::create((125) / (move_speed*0.75), Vec2(-125, 0))
			, MoveBy::create((125) / (move_speed*0.5), Vec2(-125, 0)), nullptr));
		_bg->runAction(Sequence::create(MoveBy::create((distance) / move_speed, Vec2(-distance, 0))
			,CallFunc::create([=]() {
				main_character->setTag(result);
				result++;
				// tang toc cho nhanh end game
				for (int i = 0; i < characters.size(); i++) {
					if (characters[i]->getTag() == 0) {
						Vec2 pos = hidden_layout->convertToWorldSpace(characters[i]->getPosition());
						CCLOG("Test %d : %f", i, pos.x);
						if (pos.x < -100) {
							characters[i]->stopAllActions();
							characters[i]->runAction(RepeatForever::create(MoveBy::create(1, Vec2((move_speed - ((i == 0) ? 80 : 130))*3.5, 0))));
						}

					}
				}
				kt_characters.clear();
				kt_characters.push_back(true);
				kt_characters.push_back(true);
				this->schedule(schedule_selector(SkiingRacing::update_character_other_speed));
			})
			, MoveBy::create((125) / (move_speed*0.75), Vec2(-125, 0))
			, MoveBy::create((125) / (move_speed*0.5), Vec2(-125, 0))
			, CallFunc::create([=]() {
				main_character->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
					main_character->playSequence("stop_idle", true);
				});
			})
			, nullptr));
	}
	
	

}

void SkiingRacing::showIntro()
{
	auto rootLayout = utils::findChild<Layout*>(this, "root_layout");
	look_select_character = true;

	//_bg
	auto gameName = ui::ImageView::create("games/skiingracing/running_championship.png");
	gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gameName->setPosition(rootLayout->getContentSize() / 2);
	gameName->setOpacity(0);
	rootLayout->addChild(gameName);
	AUDIO_ENGINE->play("sounds/skiingracing/running_championship.mp3", false, 1.0f, [=]() {
		loadFrame();
		});
	gameName->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.5), FadeOut::create(0.5f), RemoveSelf::create(true), NULL));
}

void SkiingRacing::onEndGame() {
	_speaker->setVisible(false);
	_speaker->removeFromParent();
	this->unscheduleAllSelectors();
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->play("sounds/skiingracing/winner.mp3",false,0.5f);
	map_race->removeFromParent();
	auto layout_endgame = utils::findChild<Layout*>(this, "endgame_layout");

	auto deco = GAFWrapper::createGAF("gaf/skiingracing/Rc_deco/Rc_deco.gaf");
	deco->setAnchorPoint(Vec2(0.5, 0.5));
	layout_endgame->addChild(deco);
	deco->setPosition(Vec2(layout_endgame->getContentSize().width / 2 - 100, layout_endgame->getContentSize().height / 2 + 100));
	
	deco->play(false);
	auto star = GAFWrapper::createGAF("gaf/skiingracing/Rc_peffect2/Rc_peffect2.gaf");
	star->setAnchorPoint(Vec2(0.5, 0.5));
	layout_endgame->addChild(star);
	star->setScale(0.5);
	star->setPosition(utils::findChild<Node*>(layout_endgame, StringUtils::format("Node_1"))->getPosition());
	star->play(true);
	for (int i = 1; i <= 3; i++) {
		auto bubble = GAFWrapper::createGAF("gaf/skiingracing/Rc_peffect1/Rc_peffect1.gaf");
		setAnchorPoint(Vec2(0.5, 0.5));
		layout_endgame->addChild(bubble);
		bubble->setPosition(utils::findChild<Node*>(layout_endgame, StringUtils::format("bubble_%d", i))->getPosition());
		this->scheduleOnce([=](float) {
			bubble->play(true);
		}, 0.5*i, StringUtils::format("bb%d",i));
	}
	
	if (main_character->getTag() != 1) {
		AUDIO_ENGINE->play("sounds/skiingracing/good job.mp3");
	}
	else {
		AUDIO_ENGINE->play("sounds/skiingracing/win.mp3");
	}

	characters.push_back(main_character);
	for (auto c : characters) {
        c->retain();
        c->removeFromParentAndCleanup(false);
		layout_endgame->addChild(c);
        c->release();
		Vec2 pos = utils::findChild<Node*>(layout_endgame, StringUtils::format("Node_%d", c->getTag()))->getPosition();
		c->setAnchorPoint(Vec2(0.4, 0.5));
		c->setScale(1.f);
		c->setPosition(pos);
		
		if (c->getTag() == 1) {
			c->playSequence("win", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				c->playSequence("win_idle", true);
			});
		}
		else {
			c->playSequence("stop_idle", true);
		}
		if (c->getTag() == 3) {
			c->setRotationY(180);
		}
		if (c->getTag() == 2 && c->getName() == "finn") {
			c->setPositionX(c->getPositionX() - 50);
		}
		
	}
	//layout_endgame->removeFromParent();
	//this->addChild(layout_endgame, 100000);
	this->setPosition(screen_size / 2);
	this->setOpacity(255);
	layout_endgame->setAnchorPoint(Vec2(0.5, 0.5));
	layout_endgame->setPosition(screen_size / 2);
	// endgame
	scheduleOnce([=](float) {
		onCloseGame();
	}, 4.0f, "remove_all");

}
void SkiingRacing::removeAllAnswerBtns()
{
	for (auto it = m_answerBtns.begin(); it != m_answerBtns.end();) {
		auto btn = *it;
		btn->removeFromParent();
		it = m_answerBtns.erase(it);
	}
}

void SkiingRacing::update_character_other(float dt){
	if (result == 4) {
		// end game
		result = 0;
		Layout* black = cocos2d::ui::Layout::create();
		black->setContentSize(screen_size);
		black->setBackGroundColor(Color3B::BLACK);
		black->setBackGroundColorOpacity(255);
		black->setOpacity(0);
		black->setBackGroundColorType(BackGroundColorType::SOLID);
		black->setVisible(true);
		black->setAnchorPoint(Vec2(0.5, 0.5));
		Director::getInstance()->getRunningScene()->addChild(black,INT_MAX-1);
		black->setPosition(screen_size / 2);
		black->runAction(Sequence::create(
			FadeIn::create(0.5f),
			CallFunc::create([=]() {
				black->removeFromParent();
				onEndGame();
			}), 
			nullptr));
		
	}
	else {
		for (int i = 0; i < characters.size(); i++) {
			// xet de doi thu nhay len
			float posX_charater_other = hidden_layout->convertToWorldSpace(characters[i]->getPosition()).x;
			CCLOG("other %f", posX_charater_other);
			if ((posX_charater_other > (screen_size.width / 2 )) && (posX_charater_other < (screen_size.width / 2 + 10)) && is_visible_answer) {
				characters[i]->playSequence("jump", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
					characters[i]->playSequence("run_barry_run", true);
				});
			}
			// xet de update map va dung doi thu
			float distance_character = characters[i]->getPosition().x + (screen_size.width - 1024) / 2;
			if (i == 0) {
				map_race->update_character1(distance_character / long_races);
				float distance_maincharacter_to_character1 = (hidden_layout->convertToWorldSpace(characters[i]->getPosition()).x - root_layout->convertToWorldSpace(main_character->getPosition()).x);
				map_race->update_main_character((distance_character - distance_maincharacter_to_character1) / long_races);
			}
			else map_race->update_character2(distance_character / long_races);
			if ((distance_character > (long_races+500)) && characters[i]->getTag() == 0) {
				characters[i]->stopAllActions();
				characters[i]->setTag(result);
				result++;
				characters[i]->playSequence("stop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
					characters[i]->playSequence("stop_idle", true);
				});
			}
			//c->runAction(RepeatForever::create(MoveBy::create(1, Vec2(move_speed - RandomHelper::random_int(50, 80), 0))));
		}
	}
	
}
void SkiingRacing::update_character_other_speed(float dt) {	
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getTag() == 0) {
			Vec2 pos = hidden_layout->convertToWorldSpace(characters[i]->getPosition());
			CCLOG("Test %d : %f", i, pos.x);
			if (pos.x > -100 && kt_characters[i]) {
				kt_characters[i] = false;
				characters[i]->stopAllActions();
				characters[i]->runAction(RepeatForever::create(MoveBy::create(1, Vec2(move_speed - ((i == 0) ? 80 : 130), 0))));
			}

		}
	}
}
void SkiingRacing::update_main_character_jump(float dt) {
	float x = races[current_question]->convertToWorldSpace(utils::findChild<Node*>(races[current_question], "Node_1")->getPosition()).x - (screen_size.width-1024)/2;
	if (main_character->getName() == "dan") x += 100;
	if (x < 650 && !look_for_jump) {
		look_for_jump = true;
		main_character->playSequence("jump", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			main_character->playSequence("run_barry_run", true);
		});
		AUDIO_ENGINE->playSound("sounds/skiingracing/jump.mp3", false);
	}
}


ui::Widget::ccWidgetClickCallback SkiingRacing::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onSelect") {
        return CC_CALLBACK_1(SkiingRacing::onSelectCharacter, this);
    }
    return nullptr;
}
void SkiingRacing::showTapGuide(Vec2 position, std::string gaf_guide_path) {
	auto hand = GAFWrapper::createGAF(gaf_guide_path);
	Director::getInstance()->getRunningScene()->addChild(hand, INT_MAX - 1);
	hand->setPosition(position);
	hand->play(true);
	hand->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
		hand->removeFromParent();
	}), NULL));
}
void SkiingRacing::inActiveMode( float delay_time) {
	this->schedule([=](float) {
		AUDIO_ENGINE->playSound(m_questions[current_question].correctAudio);
	}, 7, 100, delay_time, "active_mode");
	
}
