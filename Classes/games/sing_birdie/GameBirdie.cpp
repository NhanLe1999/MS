//
//  GameBirdie.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#include "GameBirdie.h"
#include "GameBirdieQuestion.h"
#include "GameBirdieTree.h"
#include "GameBirdieAnswer.h"
#include "GameBirdieAnswerOverlay.h"

#include "GameBirdieConstants.h"

MJGameBirdie* MJGameBirdie::_MJGameBirdie;

INITIALIZE_READER(MJGameBirdie);
USING_NS_CC;
mj::GameDataSet<mj::BirdieDataSet> MJGameBirdie::generateData(rapidjson::Value value_data, std::string resource){
	auto images = math::resource::ResHelper::getImages("icons.json");
	
	mj::GameDataSet<mj::BirdieDataSet> data;
    
	auto level = value_data["game_id"].GetInt();
    
    auto arrObjects = value_data["text_info"].GetArray();
    
    std::vector<mj::GameObject> list_keywords;
    for (int i = 0; i < (int) arrObjects.Size(); i++){
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			resource + img_nature->path,
			resource + img_nature->getAudio()
		};
        list_keywords.push_back(game_object);
    }
    
    mj::helper::shuffle(list_keywords);
    
    auto number_of_sets = (list_keywords.size() + 1) / 2;
    auto number_of_options = 4;
    if (StoryConfigManager::getInstance()->isTestGame()) {
        
    } else {
        if (level <= mj::GameDifficulity::EASY) {
            number_of_sets = 2;
            number_of_options = 2;
        } else if (level <= mj::GameDifficulity::MEDIUM) {
            number_of_sets = 2;
            number_of_options = 3;
        } else {
            number_of_sets = 3;
            number_of_options = 4;
        }
    }
    
    //generate set from list keywords
    for (auto i = 0; i < (int) list_keywords.size(); i++) {
        mj::BirdieDataSet set;
        
        auto temp_list = list_keywords;
        set.pictures.push_back(list_keywords[i]);
        set.options.push_back(list_keywords[i]);
        
        if (i + 1 < (int) list_keywords.size()) {
            set.pictures.push_back(list_keywords[i + 1]);
            set.options.push_back(list_keywords[i + 1]);
            ++i;
        } else {
            auto random_id = random(0, (int)list_keywords.size() - 2);
            set.pictures.push_back(list_keywords[random_id]);
            set.options.push_back(list_keywords[random_id]);
        }
        
        for (auto pic : set.pictures) {
            temp_list.erase(std::remove_if(temp_list.begin(), temp_list.end(), [pic](mj::GameObject obj) {
                return obj.text == pic.text;
            }), temp_list.end());
        }
        
        mj::helper::shuffle(temp_list);
        
        for (auto i = 0; i < number_of_options - (int)set.pictures.size(); i++) {
            set.options.push_back(temp_list[i]);
        }
        mj::helper::shuffle(set.options);
        data.sets.push(set);
        
        number_of_sets--;
        if (number_of_sets == 0) {
            break;
        }
    }
    return data;

    
}

mj::GameDataSet<mj::BirdieDataSet> MJGameBirdie::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::BirdieDataSet> data;

    auto gamedict = value_map["game"].asValueMap();

    auto level_string = gamedict["level_story"].asString();
    auto level = mj::game::getLevelIDFromLevelString(level_string);
    
    auto arrObjects = value_map["text_info"].asValueVector();

    std::vector<mj::GameObject> list_keywords;
    for (auto it : arrObjects) {
        auto obj = it.asValueMap();
        mj::GameObject game_object = {
            obj["text"].asString(),
            resource + obj["img_path"].asString(),
            resource + obj["audio_path"].asString()
        };
        list_keywords.push_back(game_object);
    }

    mj::helper::shuffle(list_keywords);

    auto number_of_sets = (list_keywords.size() + 1) / 2;
    auto number_of_options = 4;
    if (StoryConfigManager::getInstance()->isTestGame()) {

    } else {
        if (level <= mj::GameDifficulity::EASY) {
            number_of_sets = 2;
            number_of_options = 2;
        } else if (level <= mj::GameDifficulity::MEDIUM) {
            number_of_sets = 2;
            number_of_options = 3;
        } else {
            number_of_sets = 3;
            number_of_options = 4;
        }
    }

    //generate set from list keywords
    for (auto i = 0; i < (int) list_keywords.size(); i++) {
        mj::BirdieDataSet set;

        auto temp_list = list_keywords;
        set.pictures.push_back(list_keywords[i]);
        set.options.push_back(list_keywords[i]);

        if (i + 1 < (int) list_keywords.size()) {
            set.pictures.push_back(list_keywords[i + 1]);
            set.options.push_back(list_keywords[i + 1]);
            ++i;
        } else {
            auto random_id = random(0, (int)list_keywords.size() - 2);
            set.pictures.push_back(list_keywords[random_id]);
            set.options.push_back(list_keywords[random_id]);
        }

        for (auto pic : set.pictures) {
            temp_list.erase(std::remove_if(temp_list.begin(), temp_list.end(), [pic](mj::GameObject obj) {
                return obj.text == pic.text;
            }), temp_list.end());
        }

        mj::helper::shuffle(temp_list);

        for (auto i = 0; i < number_of_options - (int)set.pictures.size(); i++) {
            set.options.push_back(temp_list[i]);
        }
        mj::helper::shuffle(set.options);
        data.sets.push(set);

        number_of_sets--;
        if (number_of_sets == 0) {
            break;
        }
    }
    return data;
}

mj::GameDataSet<mj::BirdieDataSet> MJGameBirdie::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::BirdieDataSet> data;

    int level = 0;

    rapidjson::Value arrObjects;

    if (JSON_SAFE_CHECK(value_data, "game_id", Int))
    {
        level = value_data["game_id"].GetInt();
    }

    if (JSON_SAFE_CHECK(value_data, "text_info", Array))
    {
        arrObjects = value_data["text_info"].GetArray();
    }

	std::vector<mj::GameObject> list_keywords;
	for (int i = 0; i < (int)arrObjects.Size(); i++){
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			img_nature->path,
			img_nature->getAudio()
		};
		list_keywords.push_back(game_object);
	}

	mj::helper::shuffle(list_keywords);

	auto number_of_sets = (list_keywords.size() + 1) / 2;
	auto number_of_options = 4;
	if (StoryConfigManager::getInstance()->isTestGame()) {

	}
	else {
		if (level <= mj::GameDifficulity::EASY) {
			number_of_sets = 2;
			number_of_options = 2;
		}
		else if (level <= mj::GameDifficulity::MEDIUM) {
			number_of_sets = 2;
			number_of_options = 3;
		}
		else {
			number_of_sets = 3;
			number_of_options = 4;
		}
	}

	//generate set from list keywords
	for (auto i = 0; i < (int)list_keywords.size(); i++) {
		mj::BirdieDataSet set;

		auto temp_list = list_keywords;
		set.pictures.push_back(list_keywords[i]);
		set.options.push_back(list_keywords[i]);

		if (i + 1 < (int)list_keywords.size()) {
			set.pictures.push_back(list_keywords[i + 1]);
			set.options.push_back(list_keywords[i + 1]);
			++i;
		}
		else {
			auto random_id = random(0, (int)list_keywords.size() - 2);
			set.pictures.push_back(list_keywords[random_id]);
			set.options.push_back(list_keywords[random_id]);
		}

		for (auto pic : set.pictures) {
			temp_list.erase(std::remove_if(temp_list.begin(), temp_list.end(), [pic](mj::GameObject obj) {
				return obj.text == pic.text;
			}), temp_list.end());
		}

		mj::helper::shuffle(temp_list);

		for (auto i = 0; i < number_of_options - (int)set.pictures.size(); i++) {
			set.options.push_back(temp_list[i]);
		}
		mj::helper::shuffle(set.options);
		data.sets.push(set);

		number_of_sets--;
		if (number_of_sets == 0) {
			break;
		}
	}
	return data;
}

std::once_flag birdie_reader;
void MJGameBirdie::setHandShowed(bool handShowed)
{
    _handShowed = handShowed;
}
MJGameBirdie* MJGameBirdie::getInstance()
{
    return _MJGameBirdie;
}
MJGameBirdie * MJGameBirdie::createGame(mj::GameDataSet<mj::BirdieDataSet> data) {
    std::call_once(birdie_reader, [] {
        REGISTER_CSB_READER(MJGameBirdie);
        REGISTER_CSB_READER(MJGameBirdieQuestion);
        REGISTER_CSB_READER(MJGameBirdieTree);
        REGISTER_CSB_READER(MJGameBirdieAnswer);
        REGISTER_CSB_READER(MJGameBirdieAnswerOverlay);
    });
    auto game = static_cast<MJGameBirdie *>(CSLoader::createNodeWithVisibleSize("csb/game/birdie/game_birdie.csb"));
    if (game) {
        game->didLoadFromCSB(data);
        _MJGameBirdie = game;
        return game;
    }

    CC_SAFE_DELETE(game);
    return nullptr;
}

void MJGameBirdie::onEnter() {
    MJGameBase::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        cocos2d::experimental::AudioEngine::play2d(soundBirdieBackground);
    }
    scheduleOnce([=](float dt) {
        doAppear();
		if (_data.info.instructions.size() == 0){
            _audioPath = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
		}
		else{
            _audioPath = _data.info.instructions[0].audio_path;
		}
        MJGameInstructionOverlay::playInstructionAfterDelay(3, mj::GameType::SING_BIRDIE, _audioPath);
    }, _time_delay + 1.5f, "delay_onenter");
}

#include "MJDefault.h"
#include <story/HSAudioEngine.h>
#define key_count_guide_birdie "key_count_guide_birdie"

void MJGameBirdie::didLoadFromCSB(mj::GameDataSet<mj::BirdieDataSet> data) {
    _is_guide_showed = !(MJDEFAULT->getIntegerForKey(key_count_guide_birdie, 0) < 3);
    
    _data = data;

    auto event_one_set_done_listener = EventListenerCustom::create(kEventBirdieOneSetDone, [=](EventCustom *) {
        this->scheduleOnce([=](float) {
            _handShowed = true;
            _data.sets.pop();
            if (_data.sets.empty()) {
                this->scheduleOnce([=](float dt) {
                    if (_layoutImage1 && _layoutImage2) {
                        _layoutImage1->removeFromParent();
                        _layoutImage2->removeFromParent();
                    }
                    this->onGameOver();
                }, 1, "delay_gameover");
                //                this->onGameOver();
                return;
            }
            this->loadQuestion();
        }, 3, "load_next_question");
    });

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_one_set_done_listener, this);
}

void MJGameBirdie::doAppear() {
    this->scheduleOnce([=](float dt) {
        this->loadQuestion();
    }, .5f, "appear");
}

void MJGameBirdie::loadQuestion() {
    _speaker = mj::helper::addSpeaker(this);
    _speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
    _speaker->setScale(0.9f);
    _speaker->addClickEventListener([=](Ref * ) {
        unscheduleInactiveMode();
        AUDIO_ENGINE->stopSound(_audioPath);
        AUDIO_ENGINE->playSound(_audioPath, false, [=]() {
            inActiveMode();
            });
    });

    cocos2d::experimental::AudioEngine::play2d(soundBirdieTransition);

    _current_set = _data.sets.front();

    mj::helper::shuffle(_current_set.options);

    auto questions = ui::Helper::seekWidgetByName(this, "questions");
    auto max_x_question = 0.f;
    for (auto c : questions->getChildren()) {
        max_x_question = std::max(max_x_question, c->getPositionX() + c->getContentSize().width);
    }
  
    auto question = MJGameBirdieQuestion::createQuestion(_current_set, game_play_enable, this);
    question->setTouchEnabled(true);
    question->setPosition(Point(max_x_question, 0));
    questions->addChild(question);
    _current_question = question;


    auto overlay = ui::Helper::seekWidgetByName(this, "answers");
    auto answer_overlay = MJGameBirdieAnswerOverlay::createOverlay(_current_set);
    answer_overlay->setPosition(Point(max_x_question, 0));
    overlay->addChild(answer_overlay);
    _current_answers = answer_overlay;
    _current_answers->setDelegate(_current_question);

    auto max_position_question_in_world = questions->convertToWorldSpace(question->getPosition()) + question->getContentSize();

    updateGround(max_position_question_in_world.x + question->getContentSize().width);

    this->unschedule("show_guide_birdie");
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    auto node = ui::Helper::seekWidgetByName(this, "root");
    node->runAction(Sequence::create(EaseQuarticActionIn::create(MoveBy::create(1.5, Vec2(-Director::getInstance()->getVisibleSize().width, 0))), CallFunc::create([=] {
        auto list_quest_child = questions->getChildren();
        for (int i = 0; i < (int)list_quest_child.size(); i++) {
            if (list_quest_child.at(i) != _current_question) {
                CCLOG("Removed old question!");
                list_quest_child.at(i)->removeFromParent();
            }
        }
        
        auto list_over_child = overlay->getChildren();
        for (int i = 0; i < (int)list_over_child.size(); i++) {
            if (list_over_child.at(i) != _current_answers) {
                CCLOG("Removed old answers!");
                list_over_child.at(i)->removeFromParent();
            }
        }
        
        if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson)){
            _is_guide_showed = true;
        }
        createLayoutAboveImage();
    }), nullptr));
    inActiveMode(15);
}

void MJGameBirdie::updateGround(float limit) {
    auto ground = ui::Helper::seekWidgetByName(this, "ground");
    auto max_x_ground = 0.f;
    for (auto c : ground->getChildren()) {
        max_x_ground = std::max(max_x_ground, c->getPositionX() + c->getContentSize().width);
    }

    auto max_position = ground->convertToWorldSpace(Point(max_x_ground, 0));
    auto max_x_ground_world = max_position.x;

    if (max_x_ground_world < limit) {
        auto new_ground = ui::ImageView::create("games/birdie/ground.png");
        new_ground->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        new_ground->setPosition(Point(max_x_ground, 0));
        ground->addChild(new_ground);

		auto new_ground2 = ui::ImageView::create("games/birdie/ground.png");
		new_ground2->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		new_ground2->setPosition(Point(max_x_ground + new_ground2->getContentSize().width, 0));
		ground->addChild(new_ground2);
    }

	for (int i = 0; i < (int)ground->getChildren().size(); i++) {
		auto c = ground->getChildren().at(i);
        auto position = ground->convertToWorldSpace(c->getPosition());
        if (position.x + c->getContentSize().width < 0) {
            c->removeFromParent();
        }
    }
}

void MJGameBirdie::onExit() {
    unscheduleInactiveMode();
    MJGameBase::onExit();
}

void MJGameBirdie::onPauseQuestion()
{
	pauseGame();
}

void MJGameBirdie::showGuide(Point start, Point stop) {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    
    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
    
    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
    overlay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    
    hand->runAction(Sequence::create(MoveTo::create(2, stop - Point(0, 75)), RemoveSelf::create(), nullptr));
}

void MJGameBirdie::inActiveMode(int time){
    unscheduleInactiveMode();
    this->scheduleOnce([=](float) {
        if (!_handShowed) {
            showGuide();
        }
        AUDIO_ENGINE->playSound(_audioPath, false, [=]() {
            inActiveMode();
            });
    }, time, "show_guide_birdie");
   
}
void MJGameBirdie::showGuide() {
    _countWrongClicked = 0;
    auto tree = _current_question->getUnanswerTree();
    if (tree) {
        auto stopping_point = _current_question->getTreePosition(tree);
        auto starting_point = _current_answers->getAnswerPosition(_current_question->getTreeWord(tree));
        Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
        this->showGuide(starting_point, stopping_point);
    }
}
void MJGameBirdie::onAnswerBegan() {
    unscheduleInactiveMode();
    if (++_countWrongClicked >= 3) {
        if (!_handShowed) {
            showGuide();
        }
    }
};
void MJGameBirdie::onAnswerEnded() {
    inActiveMode();
}
void MJGameBirdie::onAnswerDropped(MJGameBirdieAnswer* answer, cocos2d::Point position)
{
    _handShowed = true;
}
void MJGameBirdie::unscheduleInactiveMode()
{
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    this->unschedule("show_guide_birdie");
    AUDIO_ENGINE->stopSound(_audioPath);
}
;
void MJGameBirdie::createLayoutAboveImage() {
    if(_current_question !=nullptr){
        auto positionLayout1 = _current_question->getFirstTreePosition();
        auto positionLayout2 = _current_question->getSecondTreePosition();

        _layoutImage1 = Layout::create();
        _layoutImage1->setContentSize(Size(350, 350));
        _layoutImage1->setBackGroundColorOpacity(0);
        _layoutImage1->setBackGroundColorType(BackGroundColorType::SOLID);
       this->addChild(_layoutImage1);
        _layoutImage1->setTouchEnabled(true);
        _layoutImage1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _layoutImage1->setPosition(positionLayout1);

        _layoutImage1->addClickEventListener([=](Ref*) {
            unscheduleInactiveMode();
            if (_isSpeakable) {
                _isSpeakable = false;
                if (_current_question->getUnanswerTree()) {
                    if (_current_question->getUnanswerTree()->getSound() != "") {
                        AUDIO_ENGINE->play(_current_question->getUnanswerTree()->getSound(), false, 1.0f, [=]() {
                            _isSpeakable = true;
                            inActiveMode();
                            });
                    }
                }
                
            }
            });

        _layoutImage2 = Layout::create();
        _layoutImage2->setContentSize(Size(350, 350));
        _layoutImage2->setBackGroundColorOpacity(0);
        _layoutImage2->setBackGroundColorType(BackGroundColorType::SOLID);
        this->addChild(_layoutImage2);
        _layoutImage2->setTouchEnabled(true);
        _layoutImage2->setAnchorPoint(Vec2::ANCHOR_MIDDLE)
;
        _layoutImage2->setPosition(positionLayout2);

        _layoutImage2->addClickEventListener([=](Ref*) {
            unscheduleInactiveMode();
            if (_isSpeakable) {
                _isSpeakable = false;
                if (_current_question->getUnanswerTree(true)) {
                    if (_current_question->getUnanswerTree(true)->getSound() != "") {
                        AUDIO_ENGINE->play(_current_question->getUnanswerTree(true)->getSound(), false, 1.0f, [=]() {
                            _isSpeakable = true;
                            inActiveMode();
                            });
                    }
                }
            }
            });
    }
    
}
