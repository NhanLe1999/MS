//
//  MJGameBuildSentence.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/28/17.
//
//

#include "GameBuildSentence.h"
#include "Owl.h"
#include "GameBuildSentenceConstants.h"
#include "HSAudioEngine.h"
#include "MJDefault.h"
#include "APProfileManager.h"
#include "MJDefinitions.h"
#define key_count_guide_flying_owl "key_count_guide_flying_owls"

INITIALIZE_READER(MJGameBuildSentence);
USING_NS_CC;


void MJGameBuildSentence::initReader() {
    REGISTER_CSB_READER(MJGameBuildSentence);
    REGISTER_CSB_READER(MJOwl);
}

MJGameBuildSentence::~MJGameBuildSentence()
{
    _callbackAfterDelay = nullptr;
	if(_current_sentence) {
	
		//delete _current_sentence ;
	
		_current_sentence = nullptr;
	
	}
}
mj::GameDataSet<mj::BuildSentenceDataSet> MJGameBuildSentence::generateData(rapidjson::Value value_data, std::string resource){
	mj::GameDataSet<mj::BuildSentenceDataSet> data;
    std::vector<mj::BuildSentenceDataSet> v_set;
    
    auto arr_sentences = value_data["sentence"].GetArray();
    for (int i = 0; i < (int) arr_sentences.Size(); i++) {
        mj::BuildSentenceDataSet set;
        auto vm_sentence = arr_sentences[i].GetObject();
        
        auto type = vm_sentence["category"].GetInt();
        set.type = (type == 1) ? mj::BuildSentenceType::BY_READ_ORDER : mj::BuildSentenceType::BY_POSITION;
        
        auto arr_text = vm_sentence["text"].GetArray();
        for (int text_it = 0; text_it < (int) arr_text.Size(); text_it++) {
            auto text = arr_text[text_it].GetString();
            set.words.push_back({text, "", ""});
        }
        
        auto arr_shuffle = vm_sentence["jump_down"].GetArray();
        for (int text_it = 0; text_it < (int) arr_shuffle.Size(); text_it++) {
            auto text = arr_shuffle[text_it].GetString();
            set.shuffle_words.push_back({text, "", ""});
        }
        
        auto images = math::resource::ResHelper::getImages("icons.json");
        
		auto img_nature = math::resource::ResHelper::getImage(images, vm_sentence["audio_nature"].GetString());
		auto img_slow = math::resource::ResHelper::getImage(images, vm_sentence["audio_slow"].GetString());

		auto full_sentence = img_slow->getText();

        set.sentence = {
			full_sentence,
			resource + img_nature->path,
			resource + img_nature->getAudio("name_1")
        };
        
        set.sentence_slow = {
			full_sentence,
			"",
			resource + img_slow->getAudio("name_1")
        };
        
		std::vector<math::resource::Sync_Text> syns_texts = img_slow->getSyncText();
		for (int sync_it = 0; sync_it < (int)syns_texts.size(); sync_it++) {
            mj::TextTiming timing = {
                //text, time-start, time-end
				syns_texts[i].w,
                (float) syns_texts[i].s,
				(float) syns_texts[i].e
            };
            
            set.sync_slow.push_back(timing);
        }
        
        v_set.push_back(set);
    }
    
    for (auto set : v_set) {
        data.sets.push(set);
    }
    
    return data;
}

mj::GameDataSet<mj::BuildSentenceDataSet> MJGameBuildSentence::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::BuildSentenceDataSet> data;
    std::vector<mj::BuildSentenceDataSet> v_set;

    auto level_string = value_map["game"].asValueMap()["level_story"].asString();
    auto level = mj::game::getLevelIDFromLevelString(level_string);
    auto arr_sentences = value_map["sentence"].asValueVector();

    for (auto sentence_it : arr_sentences) {
        mj::BuildSentenceDataSet set;
        auto vm_sentence = sentence_it.asValueMap();

        auto type = vm_sentence["category"].asInt();
        set.type = (type == 1) ? mj::BuildSentenceType::BY_READ_ORDER : mj::BuildSentenceType::BY_POSITION;

        auto arr_text = vm_sentence["text"].asValueVector();
        for (auto text_it : arr_text) {
            auto text = text_it.asString();
            set.words.push_back({text, "", ""});
        }

        auto arr_shuffle = vm_sentence["jump_down"].asValueVector();
        for (auto text_it : arr_shuffle) {
            auto text = text_it.asString();
            set.shuffle_words.push_back({text, "", ""});
        }

        auto full_sentence = vm_sentence["sentence"].asString();

        set.sentence = {
            full_sentence,
            resource + vm_sentence["image"].asString(),
            resource + vm_sentence["audio_nature"].asValueMap()["path"].asString()
        };

        set.sentence_slow = {
            full_sentence,
            "",
            resource + vm_sentence["audio_slow"].asValueMap()["path"].asString()
        };

        auto arr_sync = vm_sentence["audio_slow"].asValueMap()["sync_data"].asValueVector();
        for (auto sync_it : arr_sync) {
            auto sync_data = sync_it.asValueMap();
            mj::TextTiming timing = {
                //text, time-start, time-end
                sync_data["w"].asString(),
                sync_data["s"].asFloat(),
                sync_data["e"].asFloat()
            };

            set.sync_slow.push_back(timing);
        }

        v_set.push_back(set);
    }

    if (StoryConfigManager::getInstance()->isTestGame()) {

    } else {
        auto limit = 0;
        if (level <= mj::GameDifficulity::EASY) {
            limit = 2;
        } else {
            limit = 3;
        }

        mj::helper::shuffle(v_set);

        if (limit <= (int)v_set.size()) {
            v_set.erase(v_set.begin() + limit, v_set.end());
        }
    }


    for (auto set : v_set) {
        data.sets.push(set);
    }

    return data;
}

mj::GameDataSet<mj::BuildSentenceDataSet> MJGameBuildSentence::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::BuildSentenceDataSet> data;

    auto getAudioPath = [=](std::string i_text)->std::string
    {
        for(auto image : images)
        {
            if(i_text == image->getText())
            {
                auto path = image->getAudios();

                return  path.front();
            }
        }
        return "";
    };
    
	auto arr_sentences = value_data["sentence"].GetArray();
	for (int i = 0; i < (int)arr_sentences.Size(); i++) {
		mj::BuildSentenceDataSet set;
		auto vm_sentence = arr_sentences[i].GetObject();

		if (auto type = vm_sentence["category"].GetInt()) {
		set.type = (type == 1) ? mj::BuildSentenceType::BY_READ_ORDER : mj::BuildSentenceType::BY_POSITION;
		 }

		auto arr_text = vm_sentence["text"].GetArray();
		for (int text_it = 0; text_it < (int)arr_text.Size(); text_it++) {
			auto text = arr_text[text_it].GetString();
			set.words.push_back({ text, "", getAudioPath(text) });
		}

		if(JSON_SAFE_CHECK(vm_sentence, "jump_down", Array))
		{
		auto arr_shuffle = vm_sentence["jump_down"].GetArray();
		for (int text_it = 0; text_it < (int)arr_shuffle.Size(); text_it++) {
			auto text = arr_shuffle[text_it].GetString();
			set.shuffle_words.push_back({ text, "", getAudioPath(text) });
		}
		}
		math::resource::Image* img_nature;
		if (JSON_SAFE_CHECK(vm_sentence, "audio_nature", String)) {
			img_nature = math::resource::ResHelper::getImage(images, vm_sentence["audio_nature"].GetString());
		}
		math::resource::Image* img_slow;
		if (JSON_SAFE_CHECK(vm_sentence, "audio_slow", String)) {
			img_slow = math::resource::ResHelper::getImage(images, vm_sentence["audio_slow"].GetString());
		}

		auto full_sentence = img_slow->getText();

		set.sentence = {
			full_sentence,
			img_nature->path,
			img_nature->getAudio("name_1")
		};

		set.sentence_slow = {
			full_sentence,
			"",
			img_slow->getAudio("name_1")
		};

		std::vector<math::resource::Sync_Text> syns_texts = img_slow->getSyncText();
		for (int sync_it = 0; sync_it < (int)syns_texts.size(); sync_it++) {
			mj::TextTiming timing = {
				//text, time-start, time-end
				syns_texts[sync_it].w,
				(float)syns_texts[sync_it].s,
				(float)syns_texts[sync_it].e
			};

			set.sync_text.push_back({
				math::resource::Sync_Text(syns_texts[sync_it].w,
					(float)syns_texts[sync_it].e,
					(int)syns_texts[sync_it].ts,
					(float)syns_texts[sync_it].s,
					(int)syns_texts[sync_it].te)
			});
			set.sync_slow.push_back(timing);
		}


		data.sets.push(set);
	}

	data.info.instructions.push_back({ "", "", "sounds/build_sentence/intro1.mp3" });
	data.info.instructions.push_back({ "", "", "sounds/build_sentence/intro2.mp3" });
	data.info.instructions.push_back({ "", "", "sounds/build_sentence/intro3.mp3" });

	data.info.level = mj::GameDifficulity::HARD;

	return data;
}

std::once_flag mjgamebuildsentence_REGISTER_CSB_READER;
MJGameBuildSentence * MJGameBuildSentence::createGame(mj::GameDataSet<mj::BuildSentenceDataSet> data) {
    std::call_once(mjgamebuildsentence_REGISTER_CSB_READER, []() {
        REGISTER_CSB_READER(MJGameBuildSentence);
        REGISTER_CSB_READER(MJOwl);
    });

    auto csb_name = "csb/game/build_sentence/game_build_sentence.csb";

    auto game = static_cast<MJGameBuildSentence *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (game) {
        game->didLoadFromCSB(data);
        return game;
    }

    CC_SAFE_FREE(game);
    return nullptr;
}

void MJGameBuildSentence::didLoadFromCSB(mj::GameDataSet<mj::BuildSentenceDataSet> data) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_flying_owl, 0);
    _is_guide_showed = !(count < 3);
    
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guide_flying_owl, count + 1);
    }, 20, "delay_update_count_guide_flying_owls");
    
    _data = data;

    auto background_day = ui::Helper::seekWidgetByName(this, "background_day");
    auto background_night = ui::Helper::seekWidgetByName(this, "background_night");
    auto land = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "land"));
    
    if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson))
    {
        background_night->setOpacity(255);
        background_day->setOpacity(0);
        land->loadTexture("games/build_sentence/land_night.png");
    }
    else
    {
        if (_data.sets.front().type == mj::BuildSentenceType::BY_POSITION) {
            background_night->setOpacity(0);
            background_day->setOpacity(255);
            land->loadTexture("games/build_sentence/land.png");
        } else {
            background_night->setOpacity(255);
            background_day->setOpacity(0);
            land->loadTexture("games/build_sentence/land_night.png");
        }
    }

    m_speakable = false;
    auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    speaker->setPosition(Vec2(70,Director::getInstance()->getWinSize().height - 60));
    speaker->addClickEventListener([=](cocos2d::Ref* sender){
        if(m_speakable)
            AUDIO_ENGINE->playSound(_current_sentence->sentence.audio_path);
    });
    this->addChild(speaker);
//    if (loadQuestion()) {
//        doAppear();
//    }
}

void MJGameBuildSentence::getTimeSynctextASync(mj::BuildSentenceDataSet * set)
{
	auto id = cocos2d::experimental::AudioEngine::play2d(set->sentence_slow.audio_path, false, 0.0f);
	this->scheduleOnce([id, set](float) {
		float duration = cocos2d::experimental::AudioEngine::getDuration(id);
		float start = 0.0f;
		float interval = duration / set->words.size();
		if (interval <= 0) interval = 0.5f;
		for (int sync_it = 0; sync_it < (int)set->words.size(); sync_it++) {
			mj::TextTiming timing = {
				//text, time-start, time-end
				set->words[sync_it].text,
				start * 1000,
				(start + interval) * 1000
			};
			start += interval;
			set->sync_slow.push_back(timing);
		}
	}, 0.1f, cocos2d::StringUtils::format("get duration audio %d", id));
}

void MJGameBuildSentence::onEnter() {
    MJGameBase::onEnter();

    _callbackAfterDelay = [this]() {
        if (auto picture = static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "image"))) {
            this->scheduleOnce([=](float) {
                picture->setTouchEnabled(true);
                m_speakable = true;
                }, 1, "enable_touch_image");
        }

        if (_bling) {

            _bling->runAction(Sequence::create(Show::create(), CallFuncN::create([](Node* n) {
                if (auto cc = reinterpret_cast<GAFWrapper*>(n))
                {
                    cc->play(true);
                }

                }), DelayTime::create(1.25), NULL));

        }
    };

    if(CONFIG_MANAGER->isGameVMonkey())//hide logo and skip intro time.
    {
        if(auto logo = utils::findChild(this,"Image_2"))
        {
            logo->setVisible(false);
        }
        
        _time_delay = -0.5f;
    }
    
    _last_sentence_type = _data.sets.front().type;
    auto csb_name = "csb/game/build_sentence/game_build_sentence.csb";
    auto timeline = CSLoader::createTimeline(csb_name);
    auto info = timeline->getAnimationInfo("intro");
    this->runAction(timeline);
    timeline->gotoFrameAndPause(info.startIndex);
    this->scheduleOnce([=](float) {
        timeline->play("intro", false);
        timeline->setAnimationEndCallFunc("intro", [=] {
            if (loadQuestion()) {
                if(MJDEFAULT->getBoolForKey(key_background_music)){
                    cocos2d::experimental::AudioEngine::play2d(soundOwlBackground, true);
                }
                doAppear();
            }
        });
        
//        this->runAction(timeline);
    }, _time_delay + 1.f, "delay_onenter");

	auto bg_day = cocos2d::utils::findChild(this, "background_day");
	if (bg_day->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg_day->setScale(Director::getInstance()->getVisibleSize().width / bg_day->getContentSize().width);
	}
	auto bg_night = cocos2d::utils::findChild(this, "background_day");
	if (bg_night->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg_night->setScale(Director::getInstance()->getVisibleSize().width / bg_night->getContentSize().width);
	}
}

void MJGameBuildSentence::onExit() {
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
    _callbackAfterDelay = nullptr;
    MJGameBase::onExit();
}

void MJGameBuildSentence::adjustDifficulity() {
    if (StoryConfigManager::getInstance()->isTestGame()) {
        return;
    }
    auto limit = 0;
    if (_data.info.level <= mj::GameDifficulity::EASY) {
        limit = 3;
    } else if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
        limit = 4;
    } else {
        limit = 6;
    }

//    mj::helper::shuffle(_current_sentence.shuffle_words);
   if (limit <= (int)_current_sentence->shuffle_words.size()) {
		_current_sentence->shuffle_words.erase(_current_sentence->shuffle_words.begin() + limit, _current_sentence->shuffle_words.end());
    }
}

bool MJGameBuildSentence::loadQuestion() {
    if (_data.sets.empty()) {
        return false;
    }

    for (auto owl : _owls) {
        owl->removeFromParent();
    }
    _owls.clear();
	
	if(_current_sentence==nullptr) {
	
		_current_sentence = new mj::BuildSentenceDataSet();
	
	}
	
	_current_sentence = &_data.sets.front();
    adjustDifficulity();
    _total_owl_on_stand = 0;
    _separated_word_read = false;
    _shuffle_word_index = 0;

	// get sync slow
	if (_current_sentence->sync_slow.size() == 0) {
		getTimeSynctextASync(_current_sentence);
	}
    auto frame = static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "frame"));
    frame->loadTexture("games/build_sentence/frame.png");
    frame->setZOrder(INT_MAX);
    frame->setScale(0.9f);

    auto picture = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "image"));
    picture->loadTexture(_current_sentence->sentence.image_path);
    picture->setZOrder(INT_MAX - 1);
    picture->addClickEventListener(CC_CALLBACK_1(MJGameBuildSentence::onPictureClicked, this));
    picture->setTouchEnabled(false);
    _bling = GAFWrapper::createGAF("khung/sao.gaf");
    _bling->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 2 * Director::getInstance()->getVisibleSize().height / 3));
    this->addChild(_bling, 100, "bling");
    _bling->setVisible(false);

    m_speakable = false;

    auto owl_stand = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "top_stand"));
    auto ww = 0.f;
    auto margin = 20;
    owl_stand->setVisible(false);
	// scale owl - nam
	float rate_scale = 1.0;
	if (_current_sentence->words.size() > 5) rate_scale = (5 * 1.0) / _current_sentence->words.size();
	for (auto i = 0; i < _current_sentence->words.size(); i++) {
		auto owl = MJOwl::createOwl(_current_sentence->words[i], i, _current_sentence->type == mj::BuildSentenceType::BY_READ_ORDER);
        owl->setVisible(false);
        owl->setDelegate(this);
        owl->setAnchorPoint(Point::ANCHOR_MIDDLE);
		owl->setOwlSize(rate_scale);
        owl->setPosition(Point(ww, -10) + owl->getContentSize() / 2);
        owl->setVisible(false);
        owl_stand->addChild(owl);
        ww += owl->getContentSize().width + margin;
        _owls.push_back(owl);
    }

    ww -= margin;
    owl_stand->setContentSize(Size(ww, owl_stand->getContentSize().height));

    ui::Helper::doLayout(this);
    return true;
}

void MJGameBuildSentence::doAppear() {
    cocos2d::experimental::AudioEngine::play2d(soundOwlEndSet);

	if (_last_sentence_type != _current_sentence->type) {
        //change theme
        auto background_day = ui::Helper::seekWidgetByName(this, "background_day");
        auto background_night = ui::Helper::seekWidgetByName(this, "background_night");
        auto land = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "land"));

        if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson))
        {
            background_night->setOpacity(255);
            background_day->setOpacity(0);
            land->loadTexture("games/build_sentence/land_night.png");
        }
        else
        {
            if (_last_sentence_type == mj::BuildSentenceType::BY_POSITION) {
                background_day->runAction(FadeOut::create(0.5));
                background_night->runAction(FadeIn::create(0.5));
                land->loadTexture("games/build_sentence/land_night.png");
            } else {
                background_day->runAction(FadeIn::create(0.5));
                background_night->runAction(FadeOut::create(0.5));
                land->loadTexture("games/build_sentence/land.png");
            }
        }
    }
    _last_sentence_type = _current_sentence->type;

    auto action_timeline = CSLoader::createTimeline("csb/game/build_sentence/game_build_sentence.csb");
    action_timeline->play("appear", false);
    this->runAction(action_timeline);

    auto owl_stand = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "top_stand"));
    owl_stand->setVisible(true);
    int counter = 0;
    for (auto owl : _owls) {
        owl->flyIn(counter++, (int)_owls.size());
        owl->setVisible(true);
        owl->setCascadeOpacityEnabled(true);
        owl->runAction(FadeIn::create(0.5));
    }
}

void MJGameBuildSentence::onOwlFliedToStand(MJOwl *sender) {
    _total_owl_on_stand++;
    CCLOG("Owl flied to stand: %d", _total_owl_on_stand);
	if (_total_owl_on_stand == _current_sentence->words.size()) {
        if (!_separated_word_read) {
            _separated_word_read = true;
            readOutLoud(0);
        }
        else
            readSentence();
    }
}

void MJGameBuildSentence::readOutLoud(int index) {
//    index = index % (_current_sentence.words.size());
//    _owls[index]->read(true);

	assert(_current_sentence->sync_slow.size() == _current_sentence->words.size() && "Sync data mismatch.");

	auto id_read_slow = cocos2d::experimental::AudioEngine::play2d(_current_sentence->sentence_slow.audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(id_read_slow, [=](int, std::string) {
        this->scheduleOnce([=] (float dt) {
            auto id_instruction = -1;  
			std::string audio_path = "";
			if (_data.info.instructions.size() == 0){
				audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
			}
			
            id_instruction = cocos2d::experimental::AudioEngine::play2d(audio_path);
       /*     cocos2d::experimental::AudioEngine::setFinishCallback(id_instruction, [=](int, std::string) {
                this->read(0);
            });*/
//            });

            if (id_instruction == -1) {
                this->read(0);
            }
        }, 0.5, "read_yourself");
//        this->read(0);
    });

    for (auto i = 0; i < _owls.size(); i++) {
		_owls[i]->readAfterDelay(_current_sentence->sync_slow[i]);
    }
}

void MJGameBuildSentence::read(int index) {
//    index = index % (_current_sentence.words.size());
//    _owls[index]->read(false);
	auto id_read_slow = cocos2d::experimental::AudioEngine::play2d(_current_sentence->sentence_slow.audio_path, false, 0.f);
    cocos2d::experimental::AudioEngine::setFinishCallback(id_read_slow, [=](int, std::string) {
        this->shuffle();
    });

}

void MJGameBuildSentence::readSentence() {
    auto audio_id=cocos2d::experimental::AudioEngine::play2d(_current_sentence->sentence_slow.audio_path);
    for (auto i = 0; i < _owls.size(); i++) {
        _owls[i]->readAfterDelay(_current_sentence->sync_slow[i]);
    }
    //auto audio_id = cocos2d::experimental::AudioEngine::play2d(_current_sentence->sentence_slow.audio_path, false, 0.f);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
        this->scheduleOnce([=](float dt) {
            this->doDisappear();
        }, 1, "disappear");
    });
}

void MJGameBuildSentence::onOwlRead(MJOwl * owl, int index, bool with_sound) {
    CCLOG("Owl audio read.");
    index++;
    if (index == _current_sentence->words.size()) {
        if (with_sound) {
            read(index);
        } else {
            shuffle();
        }
    } else {
        if (with_sound) {
            readOutLoud(index);
        } else {
            read(index);
        }
    }
}

void MJGameBuildSentence::shuffle() {
    cocos2d::experimental::AudioEngine::play2d(soundOwlDown);
    std::vector<int> shuffle_slot;
	for (auto i = 0; i < _current_sentence->words.size(); i++) {
        shuffle_slot.push_back(i);
    }

    mj::helper::shuffle(shuffle_slot);

    for (int i = 0; i < _owls.size(); i++) {
		auto word = _current_sentence->words[i];
		if (mj::helper::isContain(_current_sentence->shuffle_words, word)) {
            _total_owl_on_stand --;
            _owls[i]->moveOwlToSlot(shuffle_slot[i]);
            _owls[i]->enableTouch();
        }
    }

    auto instruction_id = 0;
	if (_current_sentence->type == mj::BuildSentenceType::BY_POSITION) {
        instruction_id = 2;
    } else {
        instruction_id = 1;
    }
    if (CONFIG_MANAGER->isVietnameseStories()){
        instruction_id = 1;
    }
	std::string audio_path = "";
	if (_data.info.instructions.size() == 0){
		audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
	}
	else
    {
        if(instruction_id<_data.info.instructions.size())
        {
            audio_path = _data.info.instructions[instruction_id].audio_path;
        }
        else if(!_data.info.instructions.empty())
        {
            audio_path = _data.info.instructions.front().audio_path;
        }
	}

    MJGameInstructionOverlay::playInstructionAfterDelay(2, mj::GameType::BUILD_SENTENCE, audio_path, "",nullptr, [this]() {
            
        if (_callbackAfterDelay) {
            _callbackAfterDelay();
        }
    });
    if (!_is_guide_showed) {
        _is_guide_showed = true;
        this->scheduleOnce([=](float) {
            auto os = _owls;
            Point start, stop;
            for (auto o : os) {
                if (o->getOriginalPosition() != o->getOwlPosition()) {
                    CCLOG("owl dropped: %s", o->getWord().c_str());
                    start = o->getOwlPosition();
                    stop = o->getOriginalPosition();
                    break;
                }
            }
            
            this->schedule([this, start, stop](float) {
                showGuide(start, stop);
            }, 5, 100, 1, "show_guide_flying_owls");
        }, 5, "delay_show_guide_flying_owls");
    }
//    thi s->scheduleOnce([](float) {
//        cocos2d::experimental::AudioEngine::play2d(_data.info.instructions[0].audio_path);
//    }, 2, "read_instruction_2");
}

void MJGameBuildSentence::onOwlDroppedOnPosition(MJOwl *sender, cocos2d::Point position) {
    // by slot
	if (_current_sentence->type == mj::BuildSentenceType::BY_POSITION) {
        auto bb = sender->getBoundingBox();
        bb.origin = sender->getParent()->convertToWorldSpace(bb.origin);

        auto wordObject = sender->getWordObject();
        
        if(!wordObject.audio_path.empty())
        {
            cocos2d::experimental::AudioEngine::play2d(wordObject.audio_path);
        }
        
        if (bb.containsPoint(position)) {
            //right answer
            Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
            this->unschedule("delay_show_guide_flying_owls");
            this->unschedule("show_guide_flying_owls");
            cocos2d::experimental::AudioEngine::play2d(soundOwlRight);
            sender->flyBackToStand(true);
            sender->setTouchEnabled(false);
        } else {
            //wrong answer
            cocos2d::experimental::AudioEngine::play2d(soundOwlWrong);
            sender->flyBack();
        }

        return;
    }

	if (_current_sentence->type == mj::BuildSentenceType::BY_READ_ORDER) {
        auto answer_zone = ui::Helper::seekWidgetByName(this, "answer_zone");
        auto bb = answer_zone->getBoundingBox();
        bb.origin = answer_zone->getParent()->convertToWorldSpace(bb.origin);
        answer_zone->setZOrder(100);
        
        auto wordObject = sender->getWordObject();
        
        if(!wordObject.audio_path.empty())
        {
            cocos2d::experimental::AudioEngine::play2d(wordObject.audio_path);
        }

		auto age = ap::ProfileManager::getInstance().getInformationOfProfile().age;
		if (bb.containsPoint(position) || age < CHILDREN_AGED) {

                cocos2d::experimental::AudioEngine::play2d(soundOwlWrong);

			if (sender->getWord() == _current_sentence->shuffle_words[_shuffle_word_index].text) {
                    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
                    this->unschedule("delay_show_guide_flying_owls");
                    this->unschedule("show_guide_flying_owls");
                    _shuffle_word_index++;
                    cocos2d::experimental::AudioEngine::play2d(soundOwlRight);
                    sender->flyBackToStand(true);
                    sender->setTouchEnabled(false);
				cocos2d::experimental::AudioEngine::play2d(soundOwlRight);
            } else {
                    cocos2d::experimental::AudioEngine::play2d(soundOwlWrong);
                    sender->flyBack();
            }
        } else {
                cocos2d::experimental::AudioEngine::play2d(soundOwlWrong);
                sender->flyBack();
            }
        
    }
}

void MJGameBuildSentence::doDisappear() {
    auto csb_name = "csb/game/build_sentence/game_build_sentence.csb";
	if (!game_play_enable){
		pauseGame();
		return;
	}

    if (utils::findChild(this, "bling") != NULL) {
        _bling->setVisible(false);
    }

    auto action_timeline = CSLoader::createTimeline(csb_name);
    action_timeline->play("disappear", false);
    action_timeline->setLastFrameCallFunc([=]() {
        _data.sets.pop();

        if (this->loadQuestion()) {
            this->doAppear();
        } else {
            this->onGameOver();
        }
    });
    this->runAction(action_timeline);
	auto bg_day = cocos2d::utils::findChild(this, "background_day");
	if (bg_day->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg_day->setScale(Director::getInstance()->getVisibleSize().width / bg_day->getContentSize().width);
	}
	auto bg_night = cocos2d::utils::findChild(this, "background_day");
	if (bg_night->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg_night->setScale(Director::getInstance()->getVisibleSize().width / bg_night->getContentSize().width);
	}
}

void MJGameBuildSentence::showGuide(Point start, Point stop) {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
    
    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
    overlay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    
    hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}

void MJGameBuildSentence::onPictureClicked(Ref * sender) {
	auto picture = (ui::ImageView*)sender;
	picture->setTouchEnabled(false);

    if(utils::findChild(this, "bling") != NULL) {
        _bling->setVisible(false);
    }

	AUDIO_ENGINE->stopSound(_current_sentence->sentence.audio_path);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(_current_sentence->sentence.audio_path);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
		picture->setTouchEnabled(true);
	});
}
