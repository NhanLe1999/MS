//
//  GameWordMachine.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/26/17.
//
//

#include "GameWordMachine.h"
#include "GAFWrapper.h"
#include "GameWordMachinePipe.h"
#include "GameWordMachineConstants.h"
#include "WordFrame.h"
#include "APProfileManager.h"
#include "HSAudioEngine.h"

USING_NS_CC;
INITIALIZE_READER(MJGameWordMachine);

mj::GameDataSet<mj::WordMachineDataSet> MJGameWordMachine::generateData(rapidjson::Value value_data, std::string resource){
	auto images = math::resource::ResHelper::getImages("icons.json");
	mj::GameDataSet<mj::WordMachineDataSet> data;
	
    
    auto arrQuestions = value_data["question"].GetArray();
    
    std::vector<mj::WordMachineDataSet> v_set;
    for (int i = 0; i < (int) arrQuestions.Size(); i++) {
        auto question = arrQuestions[i].GetObject();
		auto img_nature = math::resource::ResHelper::getImage(images, question["question_data"].GetString());
        mj::WordMachineDataSet set;
        set.sentence = {
			question["question_text"].GetString(),
			img_nature->path,
			img_nature->getAudio()
        };

        auto right_answer_id = question["right_ans"].GetInt();
        auto arr_answers = question["answer"].GetArray();
        for (int index = 0; index < (int) arr_answers.Size(); index++) {
            auto answer = arr_answers[index].GetObject();
			auto answer_id = answer["id_ans"].GetInt();
			mj::GameObject obj = {
				answer["text"].GetString(),
				"",
				""
			};
            
            set.text.push_back(obj);
            
            if (answer_id == right_answer_id) {
                set.answer = obj.text;
            }
        }
        
         v_set.push_back(set);
    }
    
    mj::helper::shuffle(v_set);
    
   
    for (auto set : v_set) {
        data.sets.push(set);
    }
    return data;
}

mj::GameDataSet<mj::WordMachineDataSet> MJGameWordMachine::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::WordMachineDataSet> data;
    
    auto level_string = value_map["game"].asValueMap()["level_story"].asString();
    auto level = mj::game::getLevelIDFromLevelString(level_string);
    auto arrQuestions = value_map["question"].asValueVector();

    std::vector<mj::WordMachineDataSet> v_set;

    for (auto it : arrQuestions) {
        auto question = it.asValueMap();
        mj::WordMachineDataSet set;
        set.sentence = {
            question["text"].asString(),
            resource + question["image_ans_wm"].asString(),
            resource + question["audio_ans_wm"].asString()
        };

        auto right_answer_id = question["right_ans"].asInt();
        auto arr_answers = question["answer"].asValueVector();
        for (auto it_a : arr_answers) {
            auto answer = it_a.asValueMap();
            auto answer_id = answer["order"].asInt();

            mj::GameObject obj = {
                answer["text"].asString(),
                "",
                ""
            };

            set.text.push_back(obj);

            if (answer_id == right_answer_id) {
                set.answer = obj.text;
            }
        }

        v_set.push_back(set);
    }

    mj::helper::shuffle(v_set);

    if (StoryConfigManager::getInstance()->isTestGame()) {

    } else {
        auto limit = 0;
        if (level <= mj::GameDifficulity::MEDIUM) {
            limit = 3;
        } else {
            limit = 5;
        }

        if (limit <= (int) v_set.size()) {
            v_set.erase(v_set.begin() + limit, v_set.end());
        }

    }

    for (auto set : v_set) {
        data.sets.push(set);
    }
    return data;
}

mj::GameDataSet<mj::WordMachineDataSet> MJGameWordMachine::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::WordMachineDataSet> data;
	auto arrQuestions = value_data["question"].GetArray();
	std::vector<mj::WordMachineDataSet> v_set;
	for (int i = 0; i < (int)arrQuestions.Size(); i++) {
		auto question = arrQuestions[i].GetObject();
		auto img_nature = math::resource::ResHelper::getImage(images, question["question_data"].GetString());
        auto listTextHight = img_nature->getSyncText();

        auto text = img_nature->getText();

        auto listText = mj::helper::split(text, ' ');

		mj::WordMachineDataSet set;

        if (listText.size() == listTextHight.size())
        {
            for (int i = 0; i < listTextHight.size(); i++)
            {
                set.listTextHight.push_back(listTextHight[i]);
            }
        }
		set.sentence = {
			question["question_text"].GetString(),
			img_nature->path,
			img_nature->getAudio()
		};

		auto right_answer_id = question["right_ans"].GetInt();
		auto arr_answers = question["answer"].GetArray();
		for (int index = 0; index < (int)arr_answers.Size(); index++) {
			auto answer = arr_answers[index].GetObject();
			auto answer_id = answer["id_ans"].GetInt();
			auto imgAnswer = math::resource::ResHelper::getImage(images, answer["text"].GetString());
            std::string pathAudio = "";
            if (imgAnswer)
            {
                pathAudio = imgAnswer->getAudios("name_1").front();
            }
			mj::GameObject obj = {
				answer["text"].GetString(),
                "",
                pathAudio
			};

			set.text.push_back(obj);

			if (answer_id == right_answer_id) {
				set.answer = obj.text;
			}
		}

		v_set.push_back(set);
	}

	for (auto set : v_set) {
		data.sets.push(set);
	}
	return data;
}

std::once_flag word_machine_REGISTER_CSB_READER;
MJGameWordMachine * MJGameWordMachine::createGame(mj::GameDataSet<mj::WordMachineDataSet> data) {
    std::call_once(word_machine_REGISTER_CSB_READER, []() {
        REGISTER_CSB_READER(MJGameWordMachine);
        REGISTER_CSB_READER(MJGameWordMachinePipe);
    });
    auto p = static_cast<MJGameWordMachine *>(CSLoader::createNodeWithVisibleSize("csb/game/word_machine/game_word_machine_new.csb"));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

ui::Widget::ccWidgetClickCallback MJGameWordMachine::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onNextQuestion") {
        return CC_CALLBACK_1(MJGameWordMachine::onNextQuestion, this);
    }

    return nullptr;
}

void MJGameWordMachine::onNextQuestion(cocos2d::Ref *sender) {
    _data.sets.pop();
    if (_data.sets.empty()) {
        this->scheduleOnce([=](float) {
            cocos2d::experimental::AudioEngine::stop(bg_music_id);
            this->unschedule("reload");
            this->unschedule("stop_machine");
            this->pause();
            this->onGameOver();
        }, 1, "delay_gameover");
        return;
    }
    auto time_delay = reloadAnimation();

    this->scheduleOnce([=](float dt) {
        auto options_holder = ui::Helper::seekWidgetByName(this, "text_holder");
        options_holder->removeAllChildren();
        _current_set = _data.sets.front();
        adjustDifficulity();
        this->loadQuestion(_current_set);
        this->loadAnimation();
    }, time_delay + 0.5, "reload");
}

double MJGameWordMachine::reloadAnimation() {
    auto timeline = CSLoader::createTimeline("csb/game/word_machine/game_word_machine.csb");
    this->runAction(timeline);
    timeline->play("unload", false);

    _machine_gaf->play(false);
    cocos2d::experimental::AudioEngine::play2d(soundWMMachine);

    auto options_holder = ui::Helper::seekWidgetByName(this, "text_holder");
    for (auto c : options_holder->getChildren()) {
        c->runAction(EaseCubicActionIn::create(MoveBy::create(0.5, Vec2(0, 200))));
    }
    return _pipe_structure->runAnimationReload();
}

void MJGameWordMachine::loadAnimation() {
    auto timeline = CSLoader::createTimeline("csb/game/word_machine/game_word_machine.csb");
    this->runAction(timeline);
    timeline->play("load", false);

    auto options_holder = ui::Helper::seekWidgetByName(this, "text_holder");
    for (auto c : options_holder->getChildren()) {
        c->setPosition(c->getPosition() + Point(0, 200));
        c->runAction(EaseCubicActionIn::create(MoveBy::create(0.5, Vec2(0, -200))));
    }

    auto delay_time = _pipe_structure->runAnimationEnter();
    this->scheduleOnce([=](float) {
        _machine_gaf->forceLoop(false, [=](gaf::GAFObject *) {
            _machine_gaf->reset();
			if (cur_level>=1){
				if (!game_play_enable){
				this->pauseGame();
				}
			}
			cur_level++;
        });
    }, delay_time, "stop_machine");
}

#include "MJDefault.h"
#define key_count_guide_word_machine "key_count_guide_word_machine"

void MJGameWordMachine::didLoadFromCSB(mj::GameDataSet<mj::WordMachineDataSet> data) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_word_machine, 0);
    
    _data = data;
    
    auto filter = ui::Helper::seekWidgetByName(this, "filter");
    _filter_gaf = GAFWrapper::createGAF("gaf/filter/pheu.gaf", filter);

    auto machine = ui::Helper::seekWidgetByName(this, "machine");
    _machine_gaf = GAFWrapper::createGAF("gaf/machine/machine.gaf", machine);

	auto bg = cocos2d::utils::findChild(this, "background");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	}
    static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "text_holder"))->setEnabled(true);
}

void MJGameWordMachine::onEnter() {
    MJGameBase::onEnter();
    auto placeholder = utils::findChild(this, "pipe_structure");
    if (placeholder) {
        _pipe_structure = static_cast<MJGameWordMachinePipe *>(CSLoader::createNode("csb/game/word_machine/pipe.csb"));
        _pipe_structure->setPosition(placeholder->getPosition());
        _pipe_structure->setAnchorPoint(placeholder->getAnchorPoint());
        _pipe_structure->setName("pipe_structure");
        placeholder->getParent()->addChild(_pipe_structure);
        placeholder->removeFromParent();
    } else {
        CCLOG("WTF");
    }
    _img = static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "image"));
    _img->setTouchEnabled(false);
    _img->addClickEventListener([=](Ref* sender)
    {
            _pathAudioImg = _current_set.sentence.audio_path;
            _is_guide_showed = false;
            if (auto gafStar = utils::findChild(_img, "star_bink_bink"))
            {
                gafStar->removeFromParent();
            }

        if (_pathAudioImg != "")
        {
            if(_isPlayAudioIntro)
            {
                _isPlayAudioIntro = false;
                AUDIO_ENGINE->playSound(_pathAudioImg, false, [this] {
                    _isPlayAudioIntro = true;
                    });
            }

        }
    });

    _pipe_structure->setVisible(false);
    _pipe_structure->setDelegate(this);

    scheduleOnce([=](float dt) {
        auto logo = ui::Helper::seekWidgetByName(this, "logo");
        this->runAction(Sequence::create(
            CallFunc::create([=] {
                logo->runAction(Sequence::create(DelayTime::create(.5), FadeOut::create(0.5), CallFunc::create([=] {
                    _current_set = _data.sets.front();
                    adjustDifficulity();

                    _img->loadTexture(_current_set.sentence.image_path);
                    ui::Helper::seekWidgetByName(this, "text_holder")->setLocalZOrder(2);
                    _pipe_structure->loadSentence(_current_set.sentence, _current_set.answer, true, _current_set.listTextHight, _current_set.sentence.audio_path);                   
                    _pipe_structure->setVisible(true);

                    _machine_gaf->play(false);
                    cocos2d::experimental::AudioEngine::play2d(soundWMMachine);
                    loadAnimation();
                    }),
                    DelayTime::create(3.0f),
                    CallFunc::create([=] {
                        std::string audio_path = "";
                        if (_data.info.instructions.size() == 0) {
                            audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
                        }
                        else {
                            audio_path = _data.info.instructions[0].audio_path;
                        }

                        MJGameInstructionOverlay::playInstructionAfterDelay(_time_delay + 1.f, mj::GameType::WORD_MACHINE, "", "", this);
                        AUDIO_ENGINE->playSound(audio_path, false, [=] {
                            loadQuestion(_current_set, true);
                        });
                    }),
                    nullptr));
                }), NULL));
    }, _time_delay + 1.f, "delay_onenter");

    if(MJDEFAULT->getBoolForKey(key_background_music)){
        bg_music_id = cocos2d::experimental::AudioEngine::play2d(soundWMBackground, true);
    }
}

void MJGameWordMachine::loadQuestion(mj::WordMachineDataSet question, bool isLoadSentence) {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    this->handSuggetion();
    _pathAudioSpeak = question.sentence.audio_path;
    if (!isLoadSentence)
    {
        static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "image"))->loadTexture(question.sentence.image_path);
        ui::Helper::seekWidgetByName(this, "text_holder")->setLocalZOrder(2);
        _pipe_structure->loadSentence(question.sentence, question.answer, isLoadSentence, _current_set.listTextHight, _current_set.sentence.audio_path);
    }

    mj::helper::shuffle(question.text);

    auto ww = 0;
    auto margin = 50;

    auto options = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "text_holder"));
    auto optionsPosX = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "filter"))->getPositionX();
    int i = 0;
    int sumSizeTextBackground = 0;
    position_list.clear();

    int count_text = 0, j = 0;
    for (auto t : question.text)
    {
        j++;
        count_text += t.text.length();
    }
    CCLOG("%d__________", count_text);
    int k = 0;
    float distance = 30;
    float xBoudingbox = 0;
    for (auto t : question.text) {
        distance = (k == 0) ? 0 : 50;
        auto text_background = ui::ImageView::create("games/word_machine/text_background.png");
        ui::Text* text = ui::Text::create(t.text, "fonts/monkeystories_medium.ttf", 39);
        text->setAnchorPoint(Point::ANCHOR_MIDDLE);
        text->setPositionPercent(Point(0.5, 0.5));
        text->setPositionType(ui::Widget::PositionType::PERCENT);
        text->setName("text");
        text_background->addChild(text);
        std::string nameBackground = "name_background" + std::to_string(k);
        text_background->setName(nameBackground);

        text_background->setScale9Enabled(true);
        text_background->setCapInsets(Rect(27, 27, 100, 1));
        text_background->setContentSize(Size(text->boundingBox().size.width + 50, 60));
        text_background->setAnchorPoint(Point(0, 0.5));

        float xTextBg = distance + xBoudingbox;

        Point pointTextBg = Point(ww, 0) + text_background->getContentSize() / 2;

        text_background->setPosition(Vec2(xTextBg, pointTextBg.y));
        xBoudingbox = text_background->getPosition().x + text_background->getBoundingBox().size.width;

        position_list.push_back(text_background->getPosition());
        text_background->setTag(i);
        i++;
        k++;
        text_background->setTouchEnabled(true);
        text_background->addTouchEventListener(CC_CALLBACK_2(MJGameWordMachine::onTouchOption, this));

        CCLOG("posstion %s_%f_y", t.text.c_str(), text_background->getBoundingBox().size.width);
        sumSizeTextBackground = sumSizeTextBackground  + text_background->getBoundingBox().size.width;

        options->addChild(text_background);
        ww += text_background->getContentSize().width + margin;
    }

    ww -= margin;
    options->setContentSize(Size(ww, 60));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    ui::Helper::doLayout(this);

    auto sizeX = sumSizeTextBackground;
    bool isSetPostion = false;
    int isSetContenSize = 0;

    if (sumSizeTextBackground / 3 < 280)
    {
        auto content = utils::findChild(this, "content");

        auto space = (options->getBoundingBox().size.width - sumSizeTextBackground) / 4;

        for (int i = 0; i < k; i++)
        {
            std::string nameBackground = "name_background" + std::to_string(i);
            auto text_background = utils::findChild(this, nameBackground);
            if (text_background)
            {
                if (text_background->getContentSize().width < 280)
                {
                    isSetContenSize++;
                }
            }
        }

        for (int i = 0; i < k; i++)
        {
            std::string nameBackground = "name_background" + std::to_string(i);
            auto text_background = utils::findChild(this, nameBackground);
            if (text_background && isSetContenSize == k)
            {
                if (text_background->getContentSize().width < 280)
                {
                    text_background->setContentSize(Size(280, text_background->getContentSize().height));
                    isSetPostion = true;
                }
            }
        }

        if (isSetPostion)
        {
            options->setContentSize(Size(280*3 + space * 4, 60));
            float posX = 0;
            position_list.clear();
            for (int i = 0; i < k; i++)
            {
                std::string nameBackground = "name_background" + std::to_string(i);
                auto text_background = utils::findChild<ui::ImageView*>(this, nameBackground);
                if (text_background)
                {
                    if (i == 0)
                    {
                        posX = posX + space;
                    }
                    else {
                        posX = posX + space + text_background->getBoundingBox().size.width / 2;
                    }
                    
                    text_background->setPositionX(posX);
                    position_list.push_back(text_background->getPosition());
                    text_background->setTouchEnabled(true);
                    text_background->addTouchEventListener(CC_CALLBACK_2(MJGameWordMachine::onTouchOption, this));
                    posX = posX + text_background->getBoundingBox().size.width / 2;
                }
            }
        }
    }

    auto postOption = options->getPosition();
    auto sizeScreen = Director::getInstance()->getRunningScene()->getContentSize();
    options->setPosition(cocos2d::Vec2(options->getPositionX(), sizeScreen.height + 50));

    options->runAction(Sequence::create(
        MoveTo::create(isLoadSentence ? 0.5f : 1.0f, postOption),
        CallFunc::create([=] {
            addSpeaker();
            readGafBlinklink();
            _img->setTouchEnabled(true);
            _speak->setTouchEnabled(true);
        }),
        nullptr
    ));
}

void MJGameWordMachine::onTouchOption(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType event_type) {
    auto option = static_cast<ui::ImageView *>(sender);
    _is_guide_showed = false;
    switch (event_type) {
        case ui::Widget::TouchEventType::BEGAN:
            Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
            this->unschedule("show_guide_wm");
            cocos2d::experimental::AudioEngine::stop(q_id);
            cocos2d::experimental::AudioEngine::play2d(soundWMDrag);
//            _saved_option_position = option->getPosition();
            _saved_option_position = position_list[option->getTag()];
            break;

        case ui::Widget::TouchEventType::MOVED:
            option->setPosition(option->getParent()->convertToNodeSpace(option->getTouchMovePosition()));
            break;

        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
            onOptionMoved(option, option->getTouchEndPosition());
            break;

        default:
            break;
    }
}

void MJGameWordMachine::onOptionMoved(ui::ImageView * option, Point position) {
    auto option_text = option->getChildByName<ui::Text *>("text")->getString();
    auto filter_bb = _filter_gaf->getBoundingBox();
    filter_bb.origin = _filter_gaf->getParent()->convertToWorldSpace(filter_bb.origin);

    auto machine_bb = _machine_gaf->getBoundingBox();
    machine_bb.origin = _machine_gaf->getParent()->convertToWorldSpace(machine_bb.origin);

    if ((filter_bb.containsPoint(position) || machine_bb.containsPoint(position)) && (option_text == _current_set.answer)) {
        auto options = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "text_holder"));
        for(auto option : options->getChildren())
        {
            if(static_cast<ui::Widget*>(option))
                static_cast<ui::Widget*>(option)->setTouchEnabled(false);
        }
        runAnimationEndQuestion(option);
    } else {
        auto ageProfile = ap::ProfileManager::getInstance().getInformationOfProfile().age;
        if (ageProfile < 4)
        {
            this->runAction(Sequence::create(
                CallFunc::create([=] {
                    if (option->getTag() < _current_set.text.size())
                    {
                        auto audioPath = _current_set.text[option->getTag()].audio_path;
                        cocos2d::experimental::AudioEngine::play2d(audioPath);
                    }
                }),
                CallFunc::create([=] {
                    cocos2d::experimental::AudioEngine::play2d(soundWMWrong);
                }), 
                nullptr));
        }
        option->runAction(EaseElasticOut::create(MoveTo::create(1, _saved_option_position)));
        if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false)){
            this->schedule([=](float) {
                Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
                this->showGuide();
            }, 15, 3000,10, "show_guide_wm");
        }
    }
}

void MJGameWordMachine::runAnimationEndQuestion(ui::ImageView * option) {
    this->unschedule("show_guide_wm");
    cocos2d::experimental::AudioEngine::stop(q_id);
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    this->unschedule("hand_suggetion");

    if (_guide_count < 3) {
        auto count = MJDEFAULT->getIntegerForKey(key_count_guide_word_machine, 0);
        MJDEFAULT->setIntegerForKey(key_count_guide_word_machine, count + 1);
    }
    
    auto position_on_top_filter = _filter_gaf->getPosition() + Point(0, 75);
    position_on_top_filter = _filter_gaf->getParent()->convertToWorldSpace(position_on_top_filter);
    position_on_top_filter = option->getParent()->convertToNodeSpace(position_on_top_filter);
    auto move_on_top_filter = EaseCubicActionIn::create(MoveTo::create(0.25, position_on_top_filter));

    auto move_into_filter = Spawn::create(CallFunc::create([=] {
        ui::Helper::seekWidgetByName(this, "text_holder")->setLocalZOrder(0);
    }), EaseCubicActionOut::create(MoveBy::create(0.25, Point(0, -100))), ScaleTo::create(0.25, 0.5, 1), CallFunc::create([option] {
        option->setVisible(false);
    }), nullptr);

    auto full_anim = Sequence::create(move_on_top_filter, move_into_filter, NULL);

    option->runAction(full_anim);
    _img->setTouchEnabled(false);
    _speak->setTouchEnabled(false);
    cocos2d::experimental::AudioEngine::play2d(soundWMFilter);
    _filter_gaf->setLocalZOrder(1);
    _filter_gaf->playSequence("1", false, [=](gaf::GAFObject * object, const std::string & sequence) {
        cocos2d::experimental::AudioEngine::play2d(soundWMMachine);
        _filter_gaf->reset();
        _filter_gaf->playSequence("2");
        _machine_gaf->play(false, [=](gaf::GAFObject * object) {
            object->setAnimationFinishedPlayDelegate(nullptr);
            object->gotoAndStop(0);
            CCLOG("Play pipe animation");
            cocos2d::experimental::AudioEngine::play2d(soundWMPipe);
            _pipe_structure->runAnimation();

			
        });
    });


}

void MJGameWordMachine::onExit() {
    MJGameBase::onExit();
}

void MJGameWordMachine::adjustDifficulity() {
    if (StoryConfigManager::getInstance()->isTestGame()) {
        return;
    }
    mj::helper::shuffle(_current_set.text);

    for (auto i = 0; i < _current_set.text.size(); i++) {
        if (_current_set.text[i].text == _current_set.answer) {
            auto temp = _current_set.text[i];
            _current_set.text.erase(_current_set.text.begin() + i);
            _current_set.text.insert(_current_set.text.begin(), temp);
        }
    }

    auto limit = 0;
    if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
        limit = 2;
    } else {
        limit = 3;
    }
    
    limit = 3;

    if (limit <= (int) _current_set.text.size())
        _current_set.text.erase(_current_set.text.begin() + limit, _current_set.text.end());
}

void MJGameWordMachine::onReadSentence() {
    this->onNextQuestion(nullptr);
}

void MJGameWordMachine::handSuggetion()
{
    this->scheduleOnce([=](float) {
        if (_is_guide_showed)
        {
            showGuide();
        }
        else {
            _is_guide_showed = true;
        }
        handSuggetion();
    }, 10.0f, "hand_suggetion");
}

void MJGameWordMachine::showGuide() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
  
    if (_isPlayAudioIntro)
    {
        _isPlayAudioIntro = false;
        AUDIO_ENGINE->playSound(_current_set.sentence.audio_path, false, [=] {
            _isPlayAudioIntro = true;
            });
    }
        
    _guide_count++;
    auto options = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "text_holder"));
    ui::Widget * answer;
    for (auto c : options->getChildren()) {
        auto option_text = c->getChildByName<ui::Text *>("text")->getString();
        
        if (option_text == _current_set.answer) {
            answer = reinterpret_cast<ui::Widget *>(c);
            break;
        }
    }
    
    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
    if (answer->clone())
    {
        auto shadow = reinterpret_cast<ui::ImageView*>(answer->clone());
        shadow->setOpacity(100);
        auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
        overlay->addChild(shadow, INT_MAX - 1);
        overlay->addChild(hand, INT_MAX - 1);
        auto starting_point = answer->getParent()->convertToWorldSpace(answer->getPosition() + Vec2(answer->getBoundingBox().size.width / 2, 0));
        shadow->setPosition(starting_point);
        hand->setPosition(starting_point - Point(0, 75));

            auto bb = _filter_gaf->getBoundingBox();
            auto stopping_point = Point(bb.getMidX(), bb.getMidY());

            stopping_point = _filter_gaf->getParent()->convertToWorldSpace(stopping_point);

        hand->runAction(Sequence::create(MoveTo::create(2, stopping_point - Point(0, 75)), RemoveSelf::create(), nullptr));
        shadow->runAction(Sequence::create(MoveTo::create(2, stopping_point), RemoveSelf::create(), nullptr));
        shadow->setVisible(false);
    }
}

void MJGameWordMachine::addSpeaker()
{
    if (_speak)
    {
        _speak->removeFromParent();
    }
    _speak = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    auto screenSize = cocos2d::Director::getInstance()->getWinSize();
    this->addChild(_speak, 10000000);
    _speak->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _speak->setName("speak_speak");
    _speak->setPosition(Vec2(5, screenSize.height + 5));
    _speak->setScale(1);
    _speak->addClickEventListener([=](Ref* sender) {
        _is_guide_showed = false;
        if (_isPlayAudioIntro)
        {
            _img->setTouchEnabled(false);
            _isPlayAudioIntro = false;
            AUDIO_ENGINE->playSound(_pathAudioSpeak, false, [this] {
                _isPlayAudioIntro = true;
                _img->setTouchEnabled(true);
                });
        }
    });
}

void MJGameWordMachine::readGafBlinklink()
{
    auto gafStar = GAFWrapper::createGAF("khung/sao.gaf");
    gafStar->setAnchorPoint(Point(0.5, 0.5));
    gafStar->setPosition(cocos2d::Vec2(_img->getBoundingBox().size / 2));
    gafStar->setName("star_bink_bink");
    gafStar->runAction(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node* n) {
        reinterpret_cast<GAFWrapper*>(n)->play(true);
    }), DelayTime::create(1.25), NULL));
    _img->addChild(gafStar, 10000);
}