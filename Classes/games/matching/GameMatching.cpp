//
//  GameMatching.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//


#include "GameMatching.h"
#include "GameMatchingConstants.h"
#include <numeric>
INITIALIZE_READER(MJGameMatching);

USING_NS_CC;

void MJGameMatching::initReader() {
}

std::once_flag matching_csb_reader;

mj::GameDataSet<mj::MatchingGameDataSet> MJGameMatching::generateData(rapidjson::Value value_data, std::string resource) {
    auto images = math::resource::ResHelper::getImages("icons.json");

    mj::GameDataSet<mj::MatchingGameDataSet> data;

    std::vector<mj::GameObject> list_keywords;
    auto arrObjects = value_data["text_info"].GetArray();
    for (int i = 0; i < (int)arrObjects.Size(); i++) {
        auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
        mj::GameObject game_object = {
            img_nature->getText(),
            resource + img_nature->path,
            resource + img_nature->getAudio()
        };
        list_keywords.push_back(game_object);
    }
    //mj::helper::shuffle(list_keywords);

    auto level_string = value_data["game"].GetObject()["level_story"].GetString();
    auto level = mj::game::getLevelIDFromLevelString(level_string);

    int limit = 0;
    if (level <= mj::GameDifficulity::EASY) {
        limit = 4;
    }
    else if (level <= mj::GameDifficulity::MEDIUM) {
        limit = 5;
    }
    else {
        limit = 6;
    }

    auto temp_keywords = list_keywords;

    while (!list_keywords.empty()) {
        mj::MatchingGameDataSet set;

        for (auto i = 0; i < std::min((int)list_keywords.size(), limit); i++) {
            set.objects.push_back(list_keywords[i]);
        }

        list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.objects.size());

        if (!data.sets.empty())
            temp_keywords = data.sets.front().objects;
        while (set.objects.size() < 4) {
            auto random_id = cocos2d::random(0, (int)temp_keywords.size() - 1);
            set.objects.push_back(temp_keywords[random_id]);
            temp_keywords.erase(std::begin(temp_keywords) + random_id);
        }
        data.sets.push(set);
        break;
    }

    return data;
}

mj::GameDataSet<mj::MatchingGameDataSet> MJGameMatching::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::MatchingGameDataSet> data;

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

    //mj::helper::shuffle(list_keywords);


    auto gamedict = value_map["game"].asValueMap();
    auto level_string = gamedict["level_story"].asString();
    auto level = mj::game::getLevelIDFromLevelString(level_string);

    int limit = 0;
    if (level <= mj::GameDifficulity::EASY) {
        limit = 4;
    }
    else if (level <= mj::GameDifficulity::MEDIUM) {
        limit = 5;
    }
    else {
        limit = 6;
    }

    limit = 6;

    auto temp_keywords = list_keywords;

    while (!list_keywords.empty()) {
        mj::MatchingGameDataSet set;

        for (auto i = 0; i < std::min((int)list_keywords.size(), limit); i++) {
            set.objects.push_back(list_keywords[i]);
        }

        list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.objects.size());

        if (!data.sets.empty())
            temp_keywords = data.sets.front().objects;
        while (set.objects.size() < 4) {
            auto random_id = cocos2d::random(0, (int)temp_keywords.size() - 1);
            set.objects.push_back(temp_keywords[random_id]);
            temp_keywords.erase(std::begin(temp_keywords) + random_id);
        }
        data.sets.push(set);
        break;
    }

    return data;
}

mj::GameDataSet<mj::MatchingGameDataSet> MJGameMatching::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    mj::GameDataSet<mj::MatchingGameDataSet> data;

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
    //mj::helper::shuffle(list_keywords);


    auto temp_keywords = list_keywords;

    data.info.level = mj::GameDifficulity::HARD;
    auto limit = 6;

    while (!list_keywords.empty()) {
        mj::MatchingGameDataSet set;

        for (auto i = 0; i < std::min((int)list_keywords.size(), limit); i++) {
            set.objects.push_back(list_keywords[i]);
        }

        list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.objects.size());

        if (!data.sets.empty())
            temp_keywords = data.sets.front().objects;
        while (set.objects.size() < 4) {
            auto random_id = cocos2d::random(0, (int)temp_keywords.size() - 1);
            set.objects.push_back(temp_keywords[random_id]);
            temp_keywords.erase(std::begin(temp_keywords) + random_id);
        }

        data.sets.push(set);
        break;
    }

    return data;
}

MJGameMatching* MJGameMatching::createGame(mj::GameDataSet<mj::MatchingGameDataSet> data) {
    std::call_once(matching_csb_reader, [] {
        REGISTER_CSB_READER(MJGameMatching);
        REGISTER_CSB_READER(MJGameMatchingFrame);
        REGISTER_CSB_READER(MJGameMatchingAnswer);
        });

    auto undersea = data.info.extras["matching_6_type"] == "sea";
    auto csb_name = undersea ? "csb/game/matching/GameMatching_2.csb" : "csb/game/matching/GameMatching_1.csb";
    auto p = static_cast<MJGameMatching*>(cocos2d::CSLoader::createNodeWithVisibleSize(csb_name));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameMatching::adjustDifficulity() {
    if (StoryConfigManager::getInstance()->isTestGame()) {
        return;
    }
    //int limit = 6;

//    if (_data.info.level <= mj::GameDifficulity::EASY) {
//        limit = 4;
//    } else if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
//        limit = 5;
//    } else {
//        limit = 6;
//    }

    mj::helper::shuffle(_current_set.objects);
    //if (limit <= (int)_current_set.objects.size()) {
    //    _current_set.objects.erase(_current_set.objects.begin() + limit, _current_set.objects.end());
    //}
}

#include "MJDefault.h"
#include <story/HSAudioEngine.h>
#define key_count_guide_matching_1 "key_count_guide_matching_1"
#define key_count_guide_matching_2 "key_count_guide_matching_2"

void MJGameMatching::didLoadFromCSB(mj::GameDataSet<mj::MatchingGameDataSet> data) {
    addLayoutForNotClickItem();
    _data = data;
    _current_set = _data.sets.front();
    adjustDifficulity();
    _is_undersea = _data.info.extras["matching_6_type"] == "sea";
    radio();

    _callbackAudio = [=] () {
        flag = true;
        _isSpeaking = true;
        std::string audio_path = "";
        audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);

        AUDIO_ENGINE->stopSound(audio_path);
        inActiveMode();
        if (answerCorrect == 0) {
            showTutorial(10.0f);
        }
        if (_speaker)
        {
            _speaker->setTouchEnabled(true);
        }
    };
    
    _callbackLoadAnswers = [=] () {
        _is_undersea = _data.info.extras["matching_6_type"] == "sea";
        this->loadFrames();
        this->refreshLayout();
        this->loadAnswers();
        this->doAppear();
    };

    /*if (_is_undersea) {
        auto count = MJDEFAULT->getIntegerForKey(key_count_guide_matching_1, 0);
        _is_guide_showed = !(count < 3);
    } else {
        auto count = MJDEFAULT->getIntegerForKey(key_count_guide_matching_2, 0);
        _is_guide_showed = !(count < 3);
    }*/
}

MJGameMatching::~MJGameMatching()
{
    _callbackAudio = nullptr;
    _callbackLoadAnswers = nullptr;
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
}

void MJGameMatching::onEnter() {
    MJGameBase::onEnter();
    auto csb_name = _is_undersea ? "csb/game/matching/GameMatching_2.csb" : "csb/game/matching/GameMatching_1.csb";
    auto timeline = CSLoader::createTimeline(csb_name);
    this->runAction(timeline);
    timeline->gotoFrameAndPause(0);

    if (auto game_name = cocos2d::utils::findChild<ImageView*>(this, "Image_4")) {
        if (CONFIG_MANAGER->isVietnameseStories()) game_name->setVisible(false);
    }
    if (auto game_name = cocos2d::utils::findChild<ImageView*>(this, "Image_11")) {
        if (CONFIG_MANAGER->isVietnameseStories()) game_name->setVisible(false);
    }
    auto bg = cocos2d::utils::findChild(this, "Image_1");
    if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width) {
        bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
    }
    this->scheduleOnce([=](float dt) {
        timeline->play("logo", false);
        timeline->setLastFrameCallFunc([=]() {
            this->loadFrames();
            this->refreshLayout();
            this->loadAnswers();
            this->doAppear();
            });
        }, _time_delay + 1.f, "delay_onenter");
    frameClick();
    cocos2d::experimental::AudioEngine::play2d(_is_undersea ? soundSeaBackground : soundLightBackground, true);
}


void MJGameMatching::onExit() {
    this->unschedule("inActive_Mode");
    if(auto ovlerlay = utils::findChild(Director::getInstance()->getRunningScene(), "layout_can_not_click"))
    {
        ovlerlay->removeFromParent();
    }
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    _callbackAudio = nullptr;
    _callbackLoadAnswers = nullptr;
    MJGameBase::onExit();
}
void MJGameMatching::doAppear() {
    if (finishedGame) {
        return;
    }
    _isSpeaking = false;
    auto total_frame = _frames.size();
    //showTutorial(0.5f);
    //unschedule("show_guide_matching");
    if (!_is_undersea) {
        std::vector<double> angles;
        if (total_frame == 4) {
            angles = { 135, 45, 225, 315 };
        }
        else if (total_frame == 5) {
            angles = { 162, 90, 18, 234, 306 };
        }
        else if (total_frame == 6) {
            angles = { 150, 90, 30, 210, 270, 330 };
        }

        for (auto i = 0; i < total_frame; i++) {
            auto unit = Vec2(100, 0);
            //unit.rotate(Point::ZERO, CC_DEGREES_TO_RADIANS(angles[i]));

            auto origin = _frames[i]->getPosition();
            auto destination = origin;
            destination = _frames[i]->getParent()->convertToWorldSpace(destination);

            while (destination.x > -_frames[i]->getContentSize().width / 2 && destination.x < Director::getInstance()->getVisibleSize().width + _frames[i]->getContentSize().width / 2 &&
                destination.y > -_frames[i]->getContentSize().height / 2 && destination.y < Director::getInstance()->getVisibleSize().height + _frames[i]->getContentSize().height / 2) {
                destination += unit;
            }

            destination = _frames[i]->getParent()->convertToNodeSpace(destination);
            destination = destination;

            _frames[i]->setPosition(destination);
            _frames[i]->runAction(Sequence::create(DelayTime::create(random(0.f, .5f)), EaseElasticOut::create(MoveTo::create(2, origin), 0.6), nullptr));
        }
        std::string audio_path = "";
        if (answerCorrect < 3) {
            if (_data.info.instructions.size() == 0) {
                audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
            }
            else {
                audio_path = _data.info.instructions[0].audio_path;
            }
        }
        MJGameInstructionOverlay::playInstructionAfterDelay(1, mj::GameType::LIGHT_THEM_UP, audio_path, "", nullptr, [=]() {
            if(_callbackAudio)
            {
                _callbackAudio();
            }
        });
    }
    else {
        for (auto f : _frames) {
            f->setPosition(f->getPosition() + Vec2(1000, 0));
            f->runAction(MoveBy::create(2, Vec2(-1000, 0)));
            f->shake();
        }
        std::string audio_path = "";
        if (answerCorrect < 3) {
            if (_data.info.instructions.size() == 0) {
                if (_speaker)
                {
                    _speaker->setTouchEnabled(false);
                }
                audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
            }
            else {
                audio_path = _data.info.instructions[0].audio_path;
                if (_speaker)
                {
                    _speaker->setTouchEnabled(false);
                }
            }
        }
        MJGameInstructionOverlay::playInstructionAfterDelay(1, mj::GameType::LIGHT_THEM_UP, audio_path, "", nullptr, [=]() {
            if(_callbackAudio)
            {
                _callbackAudio();
            }
            });
    }
}
void MJGameMatching::loadFrames() {
    flag = false;
    this->runAction(Sequence::create(DelayTime::create(3.75), CallFunc::create([=] {
        if (_speaker)
        {
            _speaker->setVisible(true);
        }
        }), NULL));
    _frames.clear();
    auto listview = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "list_object"));
    auto listview_2 = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "list_object_2"));
    listview->setTouchEnabled(true);
    listview_2->setTouchEnabled(true);
    auto n_first_row = 0;
    auto n_second_row = 0;
    if (_current_set.objects.size() > 3) {
        n_first_row = ((int)_current_set.objects.size() + 1) / 2;
        n_second_row = (int)_current_set.objects.size() - n_first_row;
    }
    else {
        n_first_row = (int)_current_set.objects.size();
        n_second_row = 0;
    }
    //first_row
    auto list_width = 0.0;
    int margin = 50;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (answerCorrect < 3) {
        for (auto i = 0; i < n_first_row; i++) {
            auto frame = MJGameMatchingFrame::createFrame(_current_set.objects[i], _is_undersea);
            if (visibleSize.width / visibleSize.height > 1.5) {
                frame->setScale(1.2);
                margin = 100;
            }
            frame->setTouchEnabled(true);
            frame->setCallbackName([=]() {
                if (!flag) {
                    return;
                }
                stopGuiding();
                unschedule("show_guide_matching");
                AUDIO_ENGINE->stopSound(mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id));
                AUDIO_ENGINE->playSound(_current_set.objects[i].audio_path, false, [=]() {
                    if (answerCorrect == 0) {
                        showTutorial(10.0f);
                    }
                    inActiveMode();
                    if (_speaker)
                    {
                        _speaker->setTouchEnabled(true);
                    }
                });
            });
            frame->setGameMatching2(this);
            frame->setAnchorPoint(Point::ANCHOR_MIDDLE);

            frame->setPosition(Point(list_width, -150) + frame->getContentSize() / 2);
            listview->addChild(frame);
           // frame->setGlobalZOrder(INT_MAX);
            _frames.push_back(frame);
            list_width += frame->getContentSize().width + margin;
        }
        list_width -= margin;
        listview->setContentSize(Size(list_width, listview->getContentSize().height));
    }
    else {
        list_width = 0;
        for (auto i = n_first_row; i < (int)_current_set.objects.size(); i++) {         
            auto frame = MJGameMatchingFrame::createFrame(_current_set.objects[i], _is_undersea);
            frame->setGameMatching2(this);
            frame->setTouchEnabled(true);
            frame->setCallbackName([=]() {
                if (!flag) {
                    return;
                }
                AUDIO_ENGINE->stopSound(mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id));
                AUDIO_ENGINE->playSound(_current_set.objects[i].audio_path, false, [=]() {
                    inActiveMode();
                    if (_speaker)
                    {
                        _speaker->setTouchEnabled(true);
                    }
                    });
                });

            if (visibleSize.width / visibleSize.height > 1.5) {
                frame->setScale(1.2);
                margin = 100;
            }

            frame->setAnchorPoint(Point::ANCHOR_MIDDLE);
            frame->setPosition(Point(list_width, 150) + frame->getContentSize() / 2);
            listview_2->addChild(frame,INT_MAX -1);
            _frames.push_back(frame);
            list_width += frame->getContentSize().width + margin;
        }
        list_width -= margin;
        listview_2->setContentSize(Size(list_width, listview_2->getContentSize().height));
    }
}

void MJGameMatching::loadAnswers() {
    if(_overlay)
    {
        _overlay->setVisible(false);
    }
    _answersCopy.clear();
    _answers.clear();
    _indexTrueAnswer = 0;
    auto listview = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "list_answer"));
    listview->removeAllChildren();
    const int margin = 50;
    //first_row
    if (answerCorrect < 3) {
        auto list_width = 0.0;
        if (_current_set.objects.size() == 4) {
            for (auto i = 0; i < 2; i++) {
                auto answer = MJGameMatchingAnswer::createAnswer(_current_set.objects[i], _is_undersea);
                answer->setTag(i);
                _answers.push_back(answer);
            }
            mj::helper::shuffle(_answers);
            for (auto i = 0; i < 2; i++) {
                listview->addChild(_answers[i]);
                _answers[i]->setScale(1.2);
                _answers[i]->setGameMatching(this);
                _answers[i]->setPosition(Point(list_width, 0) + _answers[i]->getContentSize() / 2);
                _answers[i]->setDelegate(this);
                list_width += _answers[i]->getContentSize().width + margin;
            }
            answerCorrect = 1;
        }
        else
        {
            for (auto i = 0; i < 3; i++) {
                auto answer = MJGameMatchingAnswer::createAnswer(_current_set.objects[i], _is_undersea);
                answer->setTag(i);
                _answers.push_back(answer);
            }
            mj::helper::shuffle(_answers);
            for (auto i = 0; i < 3; i++) {
                listview->addChild(_answers[i]);
                _answers[i]->setScale(1.2);
                _answers[i]->setGameMatching(this);
                _answers[i]->setPosition(Point(list_width, 0) + _answers[i]->getContentSize() / 2);
                _answers[i]->setDelegate(this);
                list_width += _answers[i]->getContentSize().width + margin;
            }
        }
        list_width -= margin;
        listview->setContentSize(Size(list_width, listview->getContentSize().height));
    }
    else {
        auto list_width = 0.0;
        if (_current_set.objects.size() == 4) {
            for (auto i = 2; i < _current_set.objects.size(); i++) {
                auto answer = MJGameMatchingAnswer::createAnswer(_current_set.objects[i], _is_undersea);
                answer->setTag(i);
                _answers.push_back(answer);
            }
            mj::helper::shuffle(_answers);
            for (auto i = 0; i < _current_set.objects.size() - 2; i++) {
                if (i < _answers.size()) {
                    listview->addChild(_answers[i]);
                    _answers[i]->setScale(1.2);
                    _answers[i]->setGameMatching(this);
                    _answers[i]->setPosition(Point(list_width, 0) + _answers[i]->getContentSize() / 2);
                    _answers[i]->setDelegate(this);
                    list_width += _answers[i]->getContentSize().width + margin;
                }
            }
        }
        else
        {
            for (auto i = 3; i < _current_set.objects.size(); i++) {
                auto answer = MJGameMatchingAnswer::createAnswer(_current_set.objects[i], _is_undersea);
                answer->setTag(i);
                _answers.push_back(answer);
            }
            mj::helper::shuffle(_answers);
            for (auto i = 0; i < _current_set.objects.size() - 3; i++) {
                if (i < _answers.size()) {
                    listview->addChild(_answers[i]);
                    _answers[i]->setScale(1.2);
                    _answers[i]->setGameMatching(this);
                    _answers[i]->setPosition(Point(list_width, 0) + _answers[i]->getContentSize() / 2);
                    _answers[i]->setDelegate(this);
                    list_width += _answers[i]->getContentSize().width + margin;
                }
            }
        }
        list_width -= margin;
        listview->setContentSize(Size(list_width, listview->getContentSize().height));
    }
    
    _answersCopy = _answers;
}

void MJGameMatching::refreshLayout() {

    ui::Helper::doLayout(this);

    auto list_frame = static_cast<ui::ListView*>(ui::Helper::seekWidgetByName(this, "list_object"));
    auto list_frame_2 = static_cast<ui::ListView*>(ui::Helper::seekWidgetByName(this, "list_object_2"));

    if (_current_set.objects.size() <= 3) {
        list_frame_2->setVisible(false);
        list_frame->setPositionType(ui::Widget::PositionType::PERCENT);
        list_frame->setPositionPercent(Point(0.5, 0.5));
    }
}

void MJGameMatching::addLayoutForNotClickItem()
{
    _overlay = ui::Layout::create();
    _overlay->setContentSize(Director::getInstance()->getVisibleSize());
    _overlay->setTouchEnabled(true);
    _overlay->setName("layout_can_not_click");
    _overlay->setEnabled(true);
    Director::getInstance()->getRunningScene()->addChild(_overlay, INT_MAX);
}

void MJGameMatching::onAnswerMoved(MJGameMatchingAnswer* answer, cocos2d::Point position) {
    CCLOG("Touch Ended: %f %f", position.x, position.y);
    for (auto frame : _frames)
        if (!frame->getAnswered())
        {
            auto bounding_box = frame->getBoundingBox();
            bounding_box.origin = frame->getParent()->convertToWorldSpace(bounding_box.origin);
            if (bounding_box.containsPoint(position)) {
                if (frame->getObjectName() == answer->getAnswer()) {
                    answerCorrect++;
                    _indexTrueAnswer++;
                    
                    if(_indexTrueAnswer == _frames.size() && _overlay)
                    {
                        _overlay->setVisible(true);
                    }
                    
                    inActiveMode();
                    fadeUnanswered(255);
                    _handShowed = true;
                    frame->addRightAnswer(answer, position);
                    spawnAnswer(answer);
                    auto is_over = std::accumulate(_frames.begin(), _frames.end(), true, [](bool all_answered, MJGameMatchingFrame* frame) {
                        return all_answered & frame->getAnswered();
                        });
                    if (is_over && !finishedGame) {
                        flag = false;
                        if (_speaker)
                        {
                            _speaker->setTouchEnabled(false);
                        }
                        if (!_is_undersea) {
                            this->scheduleOnce([=](float dt) {
                                this->doDisappear();
                                }, 1.5f, "do_disappear");
                        }
                        else {
                            if (answerCorrect >= 6) {
                                this->scheduleOnce([=](float dt) {
                                    _callbackLoadAnswers = nullptr;
                                    _overlay->removeFromParent();
                                    this->onGameOver();
                                    }, 2.1f, "game_over");
                            }
                            this->scheduleOnce([=](float dt) {
                                this->doDisappear();
                                }, 1.5f, "do_disappear");
                        }
                    }
                    if (!game_play_enable) {
                        this->pauseGame();
                    }
                    return;
                }
                else {
                    frame->showWrongAnswer();
                    inActiveMode();
                    if (answerCorrect == 0) {
                        showTutorial(10.0f);
                    }
                    break;
                }
            }
            else {
                inActiveMode();
                if (answerCorrect == 0) {
                    showTutorial(10.0f);
                }
            }
        }
        

    fadeUnanswered(255);
    answer->moveBackToOrigin();
}

void MJGameMatching::doDisappear() {
    if (!_is_undersea) {
        flag = false;
        _speaker->setVisible(false);
        this->unschedule("inActive_Mode");
        auto delay_counter = 4;
        for (int i = 0; i < _frames.size(); i++) {
            auto f = _frames.at(i);
            f->runAction(Sequence::create(RotateBy::create(0.1, 5), CallFunc::create([f] {
                f->runAction(Repeat::create(Sequence::create(RotateBy::create(0.2, -10), RotateBy::create(0.2, 10), NULL), 20));
                }), NULL));
            f->runAction(Sequence::create(DelayTime::create(delay_counter++ * 0.2), ScaleTo::create(0.2, 1.2), EaseQuarticActionIn::create(ScaleTo::create(0.4, 0)), CallFunc::create([f] {
                f->removeFromParent();
                }), NULL));
            if (i == _frames.size() - 1 && answerCorrect <= 3) {
                unschedule("inActive_Mode");
                cocos2d::experimental::AudioEngine::play2d(soundMatchingGameOver);
                this->runAction(Sequence::create(DelayTime::create(2.5), CallFunc::create([=] {
                   
                    if(_callbackLoadAnswers)
                    {
                        _callbackLoadAnswers();
                    }
                   
                    }), NULL));
            }
        }

        if (_frames.size() >= _current_set.objects.size()-3 && answerCorrect > 3) {
            _frames.clear();
        }
        if (_frames.empty()) {
            _data.sets.pop();
            if (_data.sets.empty()) {
                auto listview = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "list_answer"));
                listview->removeAllChildren();
                while (isScheduled("inActive_Mode")) {
                    this->unschedule("inActive_Mode");
                }
                while (isScheduled("do_disappear")) {
                    this->unschedule("do_disappear");
                }
                finishedGame = true;
                cocos2d::experimental::AudioEngine::play2d(soundMatchingGameOver);
                this->scheduleOnce([=](float dt) {
                    _callbackLoadAnswers = nullptr;
                    _overlay->removeFromParent();
                    this->onGameOver();
                    }, 2.1f, "game_over");
                return;
            }
            return;
        }
    }
    else {
        flag = false;
        _speaker->setVisible(false);
        for (int i = 0; i < _frames.size(); i++) {
            auto f = _frames.at(i);
            f->shake();
            f->runAction(Sequence::create(DelayTime::create(1), MoveBy::create(1, Vec2(-Director::getInstance()->getVisibleSize().width, 0)), CallFunc::create([f] {
                f->removeFromParent();
                
                
                }), nullptr));
            if (i == _frames.size() - 1 && answerCorrect <= 3) {
                cocos2d::experimental::AudioEngine::play2d(soundMatchingGameOver);
                unschedule("inActive_Mode");
                this->runAction(Sequence::create(DelayTime::create(3.75), CallFunc::create([=] {
                   if(_callbackLoadAnswers)
                   {
                       _callbackLoadAnswers();
                   }
                    }), NULL));
            }
        }
        if (_frames.size() == 2) {
            _frames.clear();
        }
        if (_frames.empty()) {
            _data.sets.pop();
            if (_data.sets.empty()) {
                auto listview = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(this, "list_answer"));
                listview->removeAllChildren();
                 this->unschedule("inActive_Mode");
                 this->unschedule("do_disappear");
                 AUDIO_ENGINE->stopEffect(_audio);
                 finishedGame = true;
                cocos2d::experimental::AudioEngine::play2d(soundMatchingGameOver);
                this->scheduleOnce([=](float dt) {
                    _callbackLoadAnswers = nullptr;
                    _overlay->removeFromParent();
                    this->onGameOver();
                    }, 2.1f, "game_over");
                return;
            }
            return;
        }

    }
}

void MJGameMatching::radio()
{
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(60, Director::getInstance()->getWinSize().height - 60));
    _speaker->setVisible(false);
    this->addChild(_speaker);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        stopGuiding();

        //_isSpeaking = false;
        _speaker->setTouchEnabled(false);
        //_isReminding = false;
        std::string audio_path = "";
        audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
        AUDIO_ENGINE->stopSound(audio_path);
        AUDIO_ENGINE->playSound(audio_path, false, [=]() {
            _speaker->setTouchEnabled(true);
            /*_isSpeaking = true;
            _isReminding = true;*/
            unschedule("inActive_Mode");
            if (answerCorrect == 0) {
                showTutorial(10.0f);
            }
            inActiveMode();
            });
        });
}

void MJGameMatching::spawnAnswer(MJGameMatchingAnswer* be_replaced) {
    if (_current_set.objects.empty()) {
        be_replaced->removeFromParent();
        return;
    }

    /*auto new_answer = MJGameMatchingAnswer::createAnswer(_current_set.objects[0], _is_undersea);
    _current_set.objects.erase(_current_set.objects.begin());
    new_answer->setPosition(be_replaced->getPosition());
    new_answer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    new_answer->setDelegate(this);
    be_replaced->getParent()->addChild(new_answer);

    new_answer->setScale(0);
    new_answer->setOpacity(0);
    new_answer->runAction(FadeIn::create(0.4));
    new_answer->runAction(EaseBackOut::create(ScaleTo::create(0.5, 1)));*/

    be_replaced->removeFromParent();
}

void MJGameMatching::onAnswerMoving(MJGameMatchingAnswer* answer, cocos2d::Point position) {
    stopGuiding();
    AUDIO_ENGINE->stopSound(mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id));
    unschedule("show_guide_matching");
    unschedule("inActive_Mode");
    
    fadeUnanswered(100);
    if (_speaker)
    {
        _speaker->setTouchEnabled(true);
    }
    for (auto frame : _frames) if (!frame->getAnswered()) {
        auto bounding_box = frame->getBoundingBox();
        bounding_box.origin = frame->getParent()->convertToWorldSpace(bounding_box.origin);
        if (bounding_box.containsPoint(position)) {
            frame->setOpacity(255);
        }
    }
}

void MJGameMatching::onAnswerBegan(MJGameMatchingAnswer* answer, cocos2d::Point position)
{
    AUDIO_ENGINE->stopSound(mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id));
    if (_speaker)
    {
        _speaker->setTouchEnabled(true);
    }
}


#pragma callback

void MJGameMatching::fadeUnanswered(int opacity) {
    for (auto f : _frames) {
        if (!f->getAnswered()) {
            f->setOpacity(opacity);
        }
    }
}

void MJGameMatching::showGuide(Point start, Point stop) {

    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
    this->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    hand->setName("guidingHand");

    hand->runAction(Sequence::create(MoveTo::create(2.3f, stop), RemoveSelf::create(), nullptr));
}

void MJGameMatching::inActiveMode() {
    this->schedule([=](float dt) {
        std::string audio_path = "";
        if (_data.info.instructions.size() == 0) {
            audio_path = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
        }
        else {
            audio_path = _data.info.instructions[0].audio_path;
        }
        _audio = AUDIO_ENGINE->playSound(audio_path);
        if (_speaker)
        {
            _speaker->setTouchEnabled(true);
        }
        }, 10.0f, "inActive_Mode");
}

void MJGameMatching::showTutorial(float dt) {
    Point start, stop;
    auto a = _answers.at(0);
    for (auto f : _frames) {
        if (f->getObjectName() == a->getAnswer()) {
            auto bb_start = a->getBoundingBox();
            auto bb_stop = f->getBoundingBox();
            start = Point(bb_start.getMidX(), bb_start.getMidY());
            start = a->getParent()->convertToWorldSpace(start);
            stop = Point(bb_stop.getMidX(), bb_stop.getMidY());
            stop = f->getParent()->convertToWorldSpace(stop);
            break;
        }
    }
    this->schedule([=](float) {
        if (!_handShowed) {
            this->showGuide(start, stop);
        }
        }, dt, 100, 10.0f, "show_guide_matching");
}
void MJGameMatching::frameClick()
{
    auto event_listener = EventListenerCustom::create("click_frame", [this](EventCustom* e) {
        inActiveMode();
        });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);
}


void MJGameMatching::stopGuiding() {
    if (auto hand = utils::findChild<ImageView*>(this, "guidingHand")) {
        hand->stopAllActions();
        hand->removeFromParent();
    }
}
