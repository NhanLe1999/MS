//
//  GameChickensToCoop.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#include "GameChickensToCoop.h"
#include "Coop.h"
#include "Chicken.h"
#include "GameChickensConstants.h"
#include <math.h>
#include "HSAudioEngine.h"
#include "APProfileManager.h"

#define IS_SUGGET_GAME "key_sugget_game_chicken_to_coop"

INITIALIZE_READER(MJGameChickensToCoop);

USING_NS_CC;

std::once_flag chickens_to_coop_reader;

mj::GameDataSet<mj::ChickensToCoopDataSet> MJGameChickensToCoop::generateData(rapidjson::Value value_data, std::string resource){
	auto images = math::resource::ResHelper::getImages("icons.json");
	mj::GameDataSet<mj::ChickensToCoopDataSet> data;
    mj::ChickensToCoopDataSet set;
    std::vector<mj::GameObject> list_keywords;
    auto arrObjects = value_data["text_info"].GetArray();
    for (int i = 0; i < (int) arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
        mj::GameObject game_object = {
            img_nature->getText(),
            resource + img_nature->path,
            resource + img_nature->getAudio()
        };
        list_keywords.push_back(game_object);
    }
    
    mj::helper::shuffle(list_keywords);
    
    while (!list_keywords.empty()) {
        mj::ChickensToCoopDataSet set;
        
        for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
            set.words.push_back(list_keywords[i]);
        }
        
        list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.words.size());
        
        if (CONFIG_MANAGER->isTestGame()) {
            while (set.words.size() < 4) {
                set.words.push_back(set.words[0]);
            }
        }
        data.sets.push(set);
        if (!CONFIG_MANAGER->isTestGame()) {
            break;
        }
    }
    
    return data;
}

mj::GameDataSet<mj::ChickensToCoopDataSet> MJGameChickensToCoop::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::ChickensToCoopDataSet> data;
    mj::ChickensToCoopDataSet set;
    std::vector<mj::GameObject> list_keywords;
    auto arrObjects = value_map["text_info"].asValueVector();
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

    
    auto gamedict = value_map["game"].asValueMap();
    auto level_string = gamedict["level_story"].asString();

    while (!list_keywords.empty()) {
        mj::ChickensToCoopDataSet set;

        for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
            set.words.push_back(list_keywords[i]);
        }

        list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.words.size());

        if (CONFIG_MANAGER->isTestGame()) {
            while (set.words.size() < 4) {
                set.words.push_back(set.words[0]);
            }
        }
        data.sets.push(set);
        if (!CONFIG_MANAGER->isTestGame()) {
            break;
        }
    }

    return data;
}

mj::GameDataSet<mj::ChickensToCoopDataSet> MJGameChickensToCoop::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::ChickensToCoopDataSet> data;
	mj::ChickensToCoopDataSet set;
	std::vector<mj::GameObject> list_keywords;
	auto arrObjects = value_data["text_info"].GetArray();
	for (int i = 0; i < (int)arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			img_nature->path,
			img_nature->getAudio()
		};
		list_keywords.push_back(game_object);
	}

	mj::helper::shuffle(list_keywords);

	while (!list_keywords.empty()) {
		mj::ChickensToCoopDataSet set;

		for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
			set.words.push_back(list_keywords[i]);
		}

		list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.words.size());

//		if (CONFIG_MANAGER->isTestGame()) {
//			while (set.words.size() < 4) {
//				set.words.push_back(set.words[0]);
//			}
//		}
		if (set.words.size() == 2){
			data.info.level = mj::GameDifficulity::EASY;
		}
		else if (set.words.size() == 3) {
			data.info.level = mj::GameDifficulity::MEDIUM;
		}
		else
		{
			data.info.level = mj::GameDifficulity::HARD;
		}
		data.sets.push(set);
//		if (!CONFIG_MANAGER->isTestGame()) {
//			break;
//		}
	}
	

	return data;
}

MJGameChickensToCoop * MJGameChickensToCoop::createGame(mj::GameDataSet<mj::ChickensToCoopDataSet> data) {
    std::call_once(chickens_to_coop_reader, []() {
        REGISTER_CSB_READER(MJGameChickensToCoop);
        REGISTER_CSB_READER(MJCoop);
        REGISTER_CSB_READER(MJChicken);
    });
    auto game = static_cast<MJGameChickensToCoop *>(CSLoader::createNodeWithVisibleSize("csb/game/chickens_to_coop/game_chickens_to_coop.csb"));
    if (game) {
        game->didLoadFromCSB(data);
        return game;
    }

    CC_SAFE_DELETE(game);
    return nullptr;
}

void MJGameChickensToCoop::didLoadFromCSB(mj::GameDataSet<mj::ChickensToCoopDataSet> data) {
    _data = data;
	auto Image_11 = cocos2d::utils::findChild(this, "Image_11");
	if (Image_11->getContentSize().width<Director::getInstance()->getVisibleSize().width){
		Image_11->setScaleX(Director::getInstance()->getVisibleSize().width / Image_11->getContentSize().width);
	}
    if (CONFIG_MANAGER->isGameVMonkey()){
        auto Image_12 = cocos2d::utils::findChild(this, "Image_12");
        Image_12->setVisible(false);
    }
    loadQuestion();
}    

void MJGameChickensToCoop::adjustDifficulity() {
    mj::helper::shuffle(_current_set.words);
    int num_keywords = 0;

    if (StoryConfigManager::getInstance()->isTestGame()) {
        num_keywords = 4;
        _coop_door = 3;
    } else {
        if (_data.info.level <= mj::GameDifficulity::EASY) {
            _coop_door = 5;
            num_keywords = 2;
        } else if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
            _coop_door = 5;
            num_keywords = 3;
        } else {
            _coop_door = 3;
            num_keywords = 4;
        }
    }

    if (num_keywords <= _current_set.words.size()) {
        _current_set.words.erase(_current_set.words.begin() + num_keywords, _current_set.words.end());
    }
}


void MJGameChickensToCoop::onEnter() {
    MJGameBase::onEnter();
    auto _delay = _time_delay + 1.f;
    if (CONFIG_MANAGER->isGameVMonkey()){
        _delay = 0;
    }
    _callbackAddSpeakAndShowGuide = [this]() {
        showGuide();
        addSpeaker();
    };
    
    this->_callbackCloseActivity = [=] () {
        _callbackAddSpeakAndShowGuide = nullptr;
    };
    
    scheduleOnce([=](float dt) {
        if(MJDEFAULT->getBoolForKey(key_background_music)){
            cocos2d::experimental::AudioEngine::play2d(soundChickenBackground, true);
        }

        audio_path = "";
        if (_data.info.instructions.size()==0){
            audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
        }
        else{
            audio_path = _data.info.instructions[0].audio_path;
        }
        if (CONFIG_MANAGER->isGameVMonkey()){
            audio_path = "sounds/chickens_to_coop/instruction_vn.mp3";
        }
        MJDEFAULT->setStringForKey("key.stop.audio.intro", "No");
        _isShowGui = true;
        MJGameInstructionOverlay::playInstructionAfterDelay(2.5, mj::GameType::CHICKENS_TO_COOP, audio_path, "", nullptr, [this] () {
                if (_callbackAddSpeakAndShowGuide)
                {
                    _callbackAddSpeakAndShowGuide();
                }
            });
        
        _id_audio_chicken_run = cocos2d::experimental::AudioEngine::play2d(soundChickenRun, true);
        auto timeline = CSLoader::createTimeline("csb/game/chickens_to_coop/game_chickens_to_coop.csb");
        timeline->play("logo", false);
        
        this->runAction(timeline);
    }, _delay , "delay_onenter");
}

//7509022662584

void MJGameChickensToCoop::onExit() {
    cocos2d::experimental::AudioEngine::stopAll();
    _callbackAddSpeakAndShowGuide = nullptr;
    MJGameBase::onExit();
}

#define key_count_guide_chicken_coops "key_count_guide_chicken_coops"

bool MJGameChickensToCoop::loadQuestion() {
    _current_set = _data.sets.front();
    adjustDifficulity();
    mj::helper::shuffle(_current_set.words);
    loadCoop();
    loadChicken();
    
    
    this->scheduleOnce([=](float){
         inActiveMode();
    }, 10, "inActive");
    
   
    return true;
}

#include "MJDefault.h"
void MJGameChickensToCoop::showGuide() {
    this->scheduleOnce([=](float) {
        auto chicken = _chickens[3];
        auto coop = * std::find_if(_coops.begin(), _coops.end(), [chicken](MJCoop * c) {
            return c->getWord() == chicken->getWord();
        });

        auto parentChicken = chicken->getParent();
    
        auto stopping_point = coop->getPosition();
        stopping_point.y += 100;
        stopping_point = coop->getParent()->convertToWorldSpace(stopping_point);
        stopping_point = chicken->getParent()->convertToNodeSpace(stopping_point);
    
        if (parentChicken)
        {
        
            chicken->setTouchEnabled(false);
            chicken->onTouchChicken(chicken, ui::Widget::TouchEventType::BEGAN);
            chicken->setName("chicken_suggetion");

            auto starting_point = chicken->getPosition();

            auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
            this->addChild(hand, INT_MAX - 1);
            if (parentChicken)
            {
                hand ->setPosition(parentChicken->convertToWorldSpace(starting_point) - Point(0, 75));
            }
            hand->setName("hand_suggetion");

            auto moving_action = EaseCircleActionOut::create(ActionFloat::create(2, 0, 1, [=](float v) {
                auto p = (stopping_point - starting_point) * v + starting_point;
                if (parentChicken && hand)
                {

                    hand->setPosition(parentChicken->convertToWorldSpace(p) - Point(0, 75));
                }

                chicken->setPosition(p);
                }));
            _postBeginChickenSugettion = chicken->getPosition();
            chicken->runAction(Sequence::create(DelayTime::create(1), moving_action, CallFunc::create([=] {
                chicken->onTouchChicken(chicken, ui::Widget::TouchEventType::ENDED);
                hand->removeFromParent();
                }), NULL));
            chicken->setActionTag(3.0);
            
        }
        }, 10.0, "guide_chicken");
}

void  MJGameChickensToCoop::loadCoop() {
    auto coops = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "coops"));
    coops->removeAllChildren();

    auto ww = 0;
    auto margin = 5;

    for (auto word : _current_set.words) {
        auto coop = MJCoop::createCoop(word, _coop_door);
        coop->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        coop->setPosition(Point(ww, 0) + Point(coop->getContentSize().width / 2, 0));
        coops->addChild(coop);
        ww += coop->getContentSize().width + margin;

        _coops.push_back(coop);
    }
    ww -= margin;
    coops->setContentSize(Size(ww, coops->getContentSize().height));
    ui::Helper::doLayout(this);
}

void MJGameChickensToCoop::loadChicken() {
    auto repeat_number = _coop_door;

    for (auto i = 0; i < repeat_number; i++) {
        _shuffle_words.insert(std::end(_shuffle_words), std::begin(_current_set.words), std::end(_current_set.words));
    }
    mj::helper::shuffle(_shuffle_words);

    auto chicken_counter = 0;

    for (auto i = 0; i < 6; i++) {
        spawnChicken(_shuffle_words[0], chicken_counter++ * 75.f);
        _shuffle_words.erase(_shuffle_words.begin());
    }

    reOrderChickens();
}

void MJGameChickensToCoop::run(MJChicken * chicken) {
}

void MJGameChickensToCoop::spawnChicken(mj::GameObject word, float position_y) {
    auto yard = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "yard"));
    auto chicken = MJChicken::createChicken(word);
    chicken->setDelegate(this);
    chicken->setAnchorPoint(Point::ANCHOR_MIDDLE);

    auto seed = random(0, 1);
    Point initial_position;
    if (seed == 0) {
        initial_position = Point(-100, position_y);
    } else {
        initial_position = Point(Director::getInstance()->getVisibleSize().width + 100, position_y);
    }

    initial_position = yard->convertToNodeSpace(initial_position);

    chicken->setPosition(Point(initial_position.x, position_y));

    yard->addChild(chicken);
    _chickens.push_back(chicken);
    chicken->run();
}

void MJGameChickensToCoop::reOrderChickens() {
    std::sort(_chickens.begin(), _chickens.end(), [](MJChicken * a, MJChicken * b) {
        return a->getPositionY() > b->getPositionY();
    });

    for (auto i = 0; i < (int)_chickens.size(); i++) {
        _chickens[i]->setLocalZOrder(i);
    }
}

void MJGameChickensToCoop::onChickenTouchedTo(){
     this->unschedule("inActive_Mode");
}

void MJGameChickensToCoop::onChickenDraggedTo(MJChicken *chicken, cocos2d::Point position) {
   
    this->unschedule("guide_chicken");

    for (auto coop : _coops) {
        auto bb = coop->getBoundingBox();
        bb.origin = coop->getParent()->convertToWorldSpace(bb.origin + Vec2(20, 80));
        if (bb.containsPoint(position)) {
            if (!coop->isFull() && (coop->getWord() == chicken->getWord())) {
                cocos2d::experimental::AudioEngine::play2d(soundChickenRight);
                coop->putChicken();
                inActiveMode();
                if (!_shuffle_words.empty()) {
                    spawnChicken(_shuffle_words[0], chicken->getOriginPositionY());
                    _shuffle_words.erase(_shuffle_words.begin());
                    reOrderChickens();
                } else {
                }
                _isShowGui = false;
                if (chicken->isPlayAudioChicken())
                {
                    cocos2d::experimental::AudioEngine::play2d(chicken->getAudio());
                }
                chicken->removeFromParent();
                _chickens.erase(std::remove(_chickens.begin(), _chickens.end(), chicken), _chickens.end());
				chicken_to_coop++;
				if (!game_play_enable && chicken_to_coop == 3){
					pauseGame();
					return;
				}
                if (_chickens.empty()) {
                    this->unschedule("inActive_Mode");
                    _data.sets.pop();
                    if (_data.sets.empty()) {
                        
                        this->scheduleOnce([=](float dt) {
                            this->onGameOver();
                        }, 1.5f, "delay_game_over");
                    } else {
                        auto game = MJGameChickensToCoop::createGame(_data);
                        game->setName("game_game");
                        this->getParent()->addChild(game, 10);
                        this->removeFromParent();
                    }

                    cocos2d::experimental::AudioEngine::stop(_id_audio_chicken_run);
                }
                return;
            } else {
                
                cocos2d::experimental::AudioEngine::play2d(soundChickenWrong);
            }
        }
    }
    if (_isShowGui)
    {
        showGuide();
    }
    chicken->drop([=]{
        inActiveMode();
    });
}

#include "GAFWrapper.h"
void MJGameChickensToCoop::inActiveMode(){
    this->schedule([=](float){
        if (CONFIG_MANAGER->isGameVMonkey()){
            cocos2d::experimental::AudioEngine::play2d("sounds/chickens_to_coop/name_vi.mp3");
        }else{
            if (_isPlayAudioIntro)
            {
                _isPlayAudioIntro = false;
                AUDIO_ENGINE->playSound(audio_path, false, [this] {
                    _isPlayAudioIntro = true;
                });
            }
        }
        
    }, 15, "inActive_Mode");
}

void MJGameChickensToCoop::addSpeaker()
{
    auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    auto screenSize = cocos2d::Director::getInstance()->getWinSize();
    this->addChild(speaker, 10000000);
    speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    speaker->setPosition(Vec2(20, Director::getInstance()->getWinSize().height - 20));
    speaker->addClickEventListener([=](Ref* sender) {
        if (_isPlayAudioIntro)
        {
            _isPlayAudioIntro = false;
            AUDIO_ENGINE->playSound(audio_path, false, [=] {
                _isPlayAudioIntro = true;
                });
        }
    });
}
