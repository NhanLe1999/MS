
#include "GameQA.h"
#include "AudioEngine.h"
#include "audio/include/AudioEngine.h"
#include "HSLib.h"
#include "MJHelper.h"
#include "HSAudioEngine.h"
INITIALIZE_READER(MJGameQA);

USING_NS_CC;
void MJGameQA::initReader() {
}

mj::GameDataSet<mj::QADataSet> MJGameQA::generateData(rapidjson::Value value_data, std::string resource) {
    auto images = math::resource::ResHelper::getImages("icons.json");

    mj::GameDataSet<mj::QADataSet> data;
    mj::QADataSet set;

    auto arrQuestions = value_data["question"].GetArray();
    for (int i = 0; i < (int)arrQuestions.Size(); i++) {
        auto question = arrQuestions[i].GetObject();
        auto img_nature = math::resource::ResHelper::getImage(images, question["question_data"].GetString());
        mj::QA q;
        q.question = {
            img_nature->getText(),
            "",
            resource + img_nature->getAudio()
        };

        auto right_answer_id = question["right_ans"].GetInt();
        auto arr_answers = question["answer"].GetArray();
        for (int index = 0; index < (int)arr_answers.Size(); index++) {
            auto answer = arr_answers[index].GetObject();
            auto answer_id = answer["id_ans"].GetInt();
            auto img_ans = math::resource::ResHelper::getImage(images, answer["text"].GetString());
            mj::GameObject obj = {
                img_ans->getText(),
                "",
                resource + img_ans->getAudio()
            };

            q.answers.push_back(obj);

            if (answer_id == right_answer_id) {
                q.answer = obj.text;

            }

        }
        set.questions.push_back(q);
    }
    data.sets.push(set);

    return data;
}

mj::GameDataSet<mj::QADataSet> MJGameQA::generateData(cocos2d::ValueMap value_map, std::string resource) {
    mj::GameDataSet<mj::QADataSet> data;
    mj::QADataSet set;

    auto arrQuestions = value_map["question"].asValueVector();
    for (auto it : arrQuestions) {
        auto question = it.asValueMap();
        mj::QA q;
        q.question = {
            question["text"].asString(),
            "",
            resource + question["audio"].asValueMap()["path"].asString()
        };

        auto right_answer_id = question["right_ans"].asInt();
        auto arr_answers = question["answer"].asValueVector();
        for (auto it_a : arr_answers) {
            auto answer = it_a.asValueMap();
            auto answer_id = answer["order"].asInt();

            mj::GameObject obj = {
                answer["text"].asString(),
                "",
                resource + answer["audio"].asValueMap()["path"].asString()
            };

            q.answers.push_back(obj);

            if (answer_id == right_answer_id) {
                q.answer = obj.text;
            }
        }
        set.questions.push_back(q);
    }
    data.sets.push(set);

    return data;
}

mj::GameDataSet<mj::QADataSet> MJGameQA::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    mj::GameDataSet<mj::QADataSet> data;
    mj::QADataSet set;

    auto arrQuestions = value_data["question"].GetArray();
    for (int i = 0; i < (int)arrQuestions.Size(); i++) {
        auto question = arrQuestions[i].GetObject();
        auto img_nature = math::resource::ResHelper::getImage(images, question["question_data"].GetString());
        mj::QA q;
        q.question = {
            img_nature->getText(),
            "",
            img_nature->getAudio()
        };

        auto img_right_ans = math::resource::ResHelper::getImage(images, question["right_ans"].GetString());
        mj::GameObject obj_r = {
            img_right_ans->getText(),
            "",
            img_right_ans->getAudio()
        };
        q.answers.push_back(obj_r);
        q.answer = obj_r.text;
        auto arr_answers = question["answer_w"].GetArray();
        for (int index = 0; index < (int)arr_answers.Size(); index++) {
            auto img_ans = math::resource::ResHelper::getImage(images, arr_answers[index].GetString());
            mj::GameObject obj = {
                img_ans->getText(),
                "",
                img_ans->getAudio()
            };

            q.answers.push_back(obj);
        }
        mj::helper::shuffle(q.answers);
        set.questions.push_back(q);
    }
    data.sets.push(set);

    data.info.story_title = value_data["story_name"].GetString();
    auto img_thumb = math::resource::ResHelper::getImage(images, value_data["img_thumb"].GetString());
    data.info.story_thumb = img_thumb->path;

    data.info.level = mj::GameDifficulity::HARD;

    is_auto_submit = false;
    if (value_data.HasMember("auto_submit"))
    {
        is_auto_submit = value_data["auto_submit"].GetBool();
    }

    return data;
}

std::once_flag q_and_a_reader;
MJGameQA* MJGameQA::createGame(mj::GameDataSet<mj::QADataSet> data) {
    std::call_once(q_and_a_reader, [] {
        REGISTER_CSB_READER(MJGameQA);
        REGISTER_CSB_READER(MJGameQA_Item);
        REGISTER_CSB_READER(MJGameQA_Answer);
        REGISTER_CSB_READER(MJGameQA_AnswerItem);
        });
    auto p = static_cast<MJGameQA*>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/game/qa/GameQA.csb"));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameQA::adjustDifficulity() {
    //    mj::helper::shuffle(_current_set.questions);

    auto limit = 0;
    if (_data.info.level <= mj::GameDifficulity::MEDIUM) {
        limit = 3;
    }
    else {
        limit = 5;
    }

    if (limit <= (int)_current_set.questions.size()) {
        _current_set.questions.erase(_current_set.questions.begin() + limit, _current_set.questions.end());
    }
}

void MJGameQA::didLoadFromCSB(mj::GameDataSet<mj::QADataSet> data) {
    win_size = Director::getInstance()->getWinSize();
    _data = data;
    _current_set = data.sets.front();
    adjustDifficulity();
    bg = this->getChildByName<ImageView*>("bg");
    bg->setTouchEnabled(true);
    bg->addClickEventListener([=](Ref*) {
        CCLOG("Clicked bg");
        scheduleSoundGuidingGame();
        });
    results_lb = this->getChildByName<Text*>("results_lb");
    results_lb->setLocalZOrder(10000);
    alert_true = this->getChildByName<ImageView*>("alert_true");
    alert_false = this->getChildByName<ImageView*>("alert_false");

    btnext = this->getChildByName<Button*>("btnext");

    title = this->getChildByName<Text*>("title");
    title->setTouchEnabled(true);
    title->addClickEventListener([=](Ref*) {
        scheduleSoundGuidingGame();
        CCLOG("clicked title"); });
    title->setString(_data.info.story_title);//storyname

    title->setTextAreaSize(cocos2d::Size(win_size.width * 0.8, 80));
    title->setTextVerticalAlignment(cocos2d::TextVAlignment::TOP);
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);

    auto layout_touch = this->getChildByName<Layout*>("layout_touch");
    layout_touch->setTouchEnabled(true);
    layout_touch->addClickEventListener([=](Ref*) {
        scheduleSoundGuidingGame();
        CCLOG("clicked Layout touch");
        }); 
}

void MJGameQA::onEnter() {
    MJGameBase::onEnter();
    cocos2d::experimental::AudioEngine::lazyInit();
    setInputConfigGame();
    if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
        loadFrames();
    }
    else
    {
        this->onShowUI();
    };
};

void MJGameQA::onExitTransitionDidStart() {
    Layout::onExitTransitionDidStart();
};

void MJGameQA::onShowUI() {
    bg->setVisible(false);
    title->setVisible(false);
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/game/qa/GameQA.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    runani->gotoFrameAndPause(0);

    auto img_title = ImageView::create("games/qa/qa_title.png");
    this->addChild(img_title);
    img_title->setPosition(win_size / 2);
    //    img_title->setScale(0);

    img_title->runAction(Sequence::create(DelayTime::create(_time_delay), EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)), DelayTime::create(2.5f), FadeOut::create(0.25f), CallFunc::create([=] {
        bg->setVisible(true);
        title->setVisible(true);
        runani->play("show", false);
        runani->setLastFrameCallFunc([=] {
            loadFrames();
            });
        }), NULL));
}

void MJGameQA::loadFrames() {
    //add thumb story
    win_size = Director::getInstance()->getWinSize();
    _layout_touch = Layout::create();
    _layout_touch->setVisible(true);
    _layout_touch->setTouchEnabled(true);
    _layout_touch->setContentSize(win_size);
    this->addChild(_layout_touch, INT_MAX-1);

    isEndGame = false;
    if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
        isVM = true;
    }
    else {
        isVM = false;
    }
    ImageView* thumb_story;
    if (_data.info.story_thumb != "") {
        thumb_story = ImageView::create(_data.info.story_thumb);
    }
    else thumb_story = ImageView::create(XHSLib->storythumb);

    this->addChild(thumb_story, 101);
    thumb_story->setPosition(this->getChildByName("icon_bg")->getPosition() + Vec2(2, -4));
    //    thumb_story->addClickEventListener(CC_CALLBACK_1(MJGameQA::onRefresh, this));
    //    thumb_story->setTouchEnabled(true);

    _icon_white = ImageView::create("games/qa/qa_icon_white.png");
    _icon_white->setTouchEnabled(true);
    _icon_white->addClickEventListener([=](Ref*) {
        CCLOG("qa_icon_white.png");
        scheduleSoundGuidingGame();
        });
    this->addChild(_icon_white, 100);
    CCLOG("thumb_story = %f x %f", thumb_story->getContentSize().width, thumb_story->getContentSize().height);
    auto thumb_size = thumb_story->getContentSize();

    _icon_white->setPosition(thumb_story->getPosition());
    thumb_story->setScale(0);

    thumb_story->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 127.0f / thumb_story->getContentSize().width, 169.0f / thumb_story->getContentSize().height)));
    _icon_white->setScale(0);
    _icon_white->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 0.75)));

    auto w_item = 0;
    for (int i = 0; i < _numques; i++) {
        //add item
        auto item = MJGameQA_Item::createFrame(i + 1);
        item->setTouchEnabled(false);
        item->addClickEventListener([=](Ref*) {
            scheduleSoundGuidingGame();
            CCLOG("a item");
            });
        item->setName("item" + to_string(i));
        this->addChild(item);
        if (i == 0) {
            w_item = item->getContentSize().width;
        }
        item->setPosition(Vec2(75 + i * w_item, 518));
        item->setLocalZOrder(100 - i);
        item->setDelegate(this);
        list_item.push_back(item);
        item->setTag(i + 100);
        item->setScale(0);
        item->runAction(Sequence::create(DelayTime::create(0.05f * (i + 1)), EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)), NULL));

        //add Answer Content
        auto answer_content = MJGameQA_Answer::createFrame(_current_set.questions[i], i);
        this->addChild(answer_content, 1000);
        answer_content->setPositionX(answer_content->getPositionX() + 50);
        answer_content->setDelegate(this);
        answer_content->setTag(i + 200);
        list_answercontent.push_back(answer_content);
        if (i != 0) {
            answer_content->setVisible(false);
        }
    }

    //add button Done
    btdone = Button::create("games/qa/qa_item_normal.png", "games/qa/qa_item_normal.png", "games/qa/qa_item_normal.png");
    btdone->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btdone->setPosition(Vec2(75 + _numques * w_item, 518));
    btdone->addClickEventListener(CC_CALLBACK_1(MJGameQA::onDoneGame, this));
    this->addChild(btdone);
    btdone->setVisible(false);

    auto text_btdone = Text::create(isVM ? "chọn" : "submit", "fonts/monkeystories_thin.ttf", 20);
    text_btdone->setPosition(Vec2(btdone->getContentSize().width / 2, btdone->getContentSize().height / 2 - 10));
    btdone->addChild(text_btdone);
    text_btdone->setTextColor(Color4B(0, 92, 167, 255));

    img_btdone = ImageView::create("games/qa/qa_item_current.png");
    img_btdone->setPosition(Vec2(75 + _numques * w_item - 1 + w_item / 2, 511));
    img_btdone->setTouchEnabled(true);
    img_btdone->addClickEventListener([=](Ref*) {
        CCLOG("clicked img_btdone");
        scheduleSoundGuidingGame();
        });
    this->addChild(img_btdone, 105);
    img_btdone->setVisible(false);
  
    auto done_label = is_auto_submit ? (isVM ? "xong" : "done") : (isVM ? "chọn" : "submit");
    text_btdone = Text::create(done_label, "fonts/monkeystories_thin.ttf", 22);
    text_btdone->setPosition(Vec2(img_btdone->getContentSize().width / 2, img_btdone->getContentSize().height / 2 + 30));
    img_btdone->addChild(text_btdone);
    text_btdone->setTextColor(Color4B::BLACK);
    AUDIO_ENGINE->stopAllSound();
    if (!list_answercontent.empty())
    {
        list_answercontent[0]->onShow();
        auto btspeaker = utils::findChild<ui::Button*>(list_answercontent[0], "btspeaker");
        btspeaker->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
            this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
            ));
            });
        for (auto i : list_answercontent[0]->getList_AnswerItem()) {
            utils::findChild<ui::ImageView*>(i, "Image_speaker")->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
                this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
                ));
                });
            utils::findChild<ui::Button*>(i, "btspeaker")->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
                this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
                ));
                });
        }
    }
        
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
        if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/qa/intro_game.mp3")) {
            AUDIO_ENGINE->playSound("sounds/qa/intro_game.mp3", false, ([=] {              
                list_answercontent[0]->onPlayAudioQues([=] {
                    _enableTouchItem = true;
                    _layout_touch->setVisible(false);
                    list_item[0]->setWasChooseQes();
                    scheduleSoundGuidingGame();
                    });
                CCLOG("thumb_story = %f x %f", thumb_story->getBoundingBox().size.width, thumb_story->getBoundingBox().size.height);
                }));
        }
        else {          
            list_answercontent[0]->onPlayAudioQues([=] {
                _enableTouchItem = true;
                _layout_touch->setVisible(false);
                list_item[0]->setWasChooseQes();
                scheduleSoundGuidingGame();
                });
            this->unschedule("sound_guide");
            CCLOG("thumb_story = %f x %f", thumb_story->getBoundingBox().size.width, thumb_story->getBoundingBox().size.height);
        }}), nullptr));

}

void MJGameQA::scheduleSoundGuidingGame(int tag)
{
    this->unschedule("sound_guide");
    CCLOG("schedule sound guild");
    this->schedule([=](float)
        {
            if (_enableTouchItem)
            {
                if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/qa/intro_game.mp3")) {
                    if (tag != -1) {
                        if (list_answercontent[tag]->isReadingDone()) {
                            AUDIO_ENGINE->stopSound("sounds/qa/intro_game.mp3");
                            AUDIO_ENGINE->playSound("sounds/qa/intro_game.mp3", false, ([=] {
                                scheduleSoundGuidingGame();
                                }));
                        }
                    }
                    else {
                        AUDIO_ENGINE->stopSound("sounds/qa/intro_game.mp3");
                        AUDIO_ENGINE->playSound("sounds/qa/intro_game.mp3", false, ([=] {
                            scheduleSoundGuidingGame();
                            }));
                    }
                                      
                }
                else {
                    scheduleSoundGuidingGame();
                }
            }
        }, 10, 100, 10, "sound_guide");
}

void MJGameQA::onChooseQuesItem(Ref* sender) {
    this->unschedule("sound_guide");  
    if (_enableTouchItem) {
        int countCorrectAnswer = 0;
        for (int i = 0; i < list_item.size(); i++) {
            if (list_item[i]->getWasChooseCorrectAnswer()) {
                countCorrectAnswer++;
            }
            list_item[i]->hideArrowGuide();
        }
        // check win 
        if (countCorrectAnswer == _numques) {
            this->unschedule("sound_guide");
            this->unschedule("checkSoundCount");
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                this->scheduleOnce([this](float dt) {
                    this->onCloseGameVmonkey();
                    }, 0.5, "finish_game");
                });
        }

        checkCountclickedQues();
        for (int i = 0; i < list_item.size(); i++) {
            if (list_item[i]->getWasChooseAnswer()) {
                if (list_item[i]->getWasChooseCorrectAnswer()) continue;
                else {
                    list_item[i]->showArrowGuide();
                    break;
                }
            }
            else {
                break;
            }
        }
        if (isEndGame) return;
        auto item = dynamic_cast<MJGameQA_Item*>(sender);
        int tag = 0;
        if (item) {
            tag = (int)item->getTag() - 100;
        }       
        if (tag == itemcurrent_index) return;
        if (tag >= list_item.size()) return;
        if (itemcurrent_index < _numques) {
            list_answercontent[itemcurrent_index]->stopAllAudio();
        }

        scheduleSoundGuidingGame(tag);
        // set color for all
        if (_allQuesWasChoosed) {
            if (list_item[tag]->getWasChooseAnswer() == true)
            {
                if (list_item[tag]->getWasChooseCorrectAnswer() == false) {
                    setColorforAll("red", tag);
                }
                else {
                    setColorforAll("green", tag);
                }
            }
            else setColorforAll("origin", tag);
        }
        else {
            if (list_item[tag]->getWasChooseAnswer() == false) {
                setColorforAll("origin", tag);
            }
            if (list_item[tag]->getWasChooseAnswer() == true) {
                if (list_item[tag]->getWasChooseCorrectAnswer() == false) {
                    setColorforAll("red", tag);
                }
                else {
                    setColorforAll("green", tag);
                }
            }
        }// done

        auto btspeaker = utils::findChild<ui::Button*>(list_answercontent[tag], "btspeaker");
        btspeaker->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
            this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
            ));
            });
        for (auto i : list_answercontent[tag]->getList_AnswerItem()) {
            utils::findChild<ui::ImageView*>(i, "Image_speaker")->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
                this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
                ));
                });
            utils::findChild<ui::Button*>(i, "btspeaker")->addTouchEventListener([=](cocos2d::Ref* sender, ui::Widget::TouchEventType event_type) {
                this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=] {scheduleSoundGuidingGame(); }), nullptr
                ));
                });         
        }

        for (int i = 0; i < _numques; i++) {
            if (i == tag) {
                list_item[i]->setLocalZOrder(100);
                list_item[i]->setCurrentItem();

                list_answercontent[i]->setVisible(true);
                if (list_item[i]->getWasChooseQes() == false) {
                    _enableTouchItem = false;
                    _layout_touch->setVisible(true);
                }

                list_answercontent[i]->onShow();
                list_answercontent[i]->onPlayAudioQues([=] {
                    _enableTouchItem = true;
                    _layout_touch->setVisible(false);
                    scheduleSoundGuidingGame();
                    list_item[i]->setWasChooseQes();
                    });
            }
            else {
                list_item[i]->setLocalZOrder(80);
                list_answercontent[i]->setVisible(false);
            }

            if (i == itemcurrent_index) {
                list_item[i]->setStateItem();
            }
        }

        itemcurrent_index = tag;

        img_btdone->setVisible(false);
        alert_true->setVisible(false);
        alert_false->setVisible(false);
        results_lb->setVisible(false);
        btnext->setVisible(false);
        cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_click.mp3");
    }  
}

void MJGameQA::onChooseAnswer(Ref* sender) {

    this->unschedule("sound_guide");
    if (isEndGame) return;
    auto answer_content = static_cast<MJGameQA_Answer*>(sender);
    int tag = (int)answer_content->getTag() - 200;
    if (tag >= list_item.size()) return;

    auto check_showBtDone = 0;
    list_item[itemcurrent_index]->setWasChooseAnswer();
    //if (is_auto_submit)
    list_item[tag]->_state = MJGameQA_Item::QAITEM_STATE::CHECK_STATE;
    list_item[tag]->setCheckTrueFalseItem(answer_content->isCheckAnswer);
    list_item[tag]->hideArrowGuide();
    if (answer_content->isCheckAnswer)
    {
        if (tag < (int)list_item.size() - 1) {
            list_item[tag + 1]->showArrowGuide();
        }
        
        list_answercontent[tag]->setTouchSpeaker(false);
        for (auto i : list_answercontent[tag]->getList_AnswerItem()) {
            i->disableTouch();
        }
        int sound_id = cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_win.mp3");

        // set orange color
        setColorforAll("green", tag);

        list_item[tag]->setWasChooseCorrectAnswer(true);
        cocos2d::experimental::AudioEngine::setFinishCallback(sound_id, [=](int au_id, std::string path) {
            auto d = answer_content->_data;
            std::string audio_answer;
            for (auto a : d.answers) {
                if (a.text == d.answer) {
                    audio_answer = a.audio_path;
                    this->unschedule("sound_guide");
                    checkAllQAItem(tag);
                    this->scheduleSoundGuidingGame();
                }
            }
            });
    }
    else
    {
        list_item[tag]->setWasChooseCorrectAnswer(false);
        auto sound_id = cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_lose.mp3");
        // set color for all
        setColorforAll("red", tag);
        if (tag + 1 < list_item.size()) {
            list_item[tag + 1]->hideArrowGuide();
        }
        cocos2d::experimental::AudioEngine::setFinishCallback(sound_id, [=](int au_id, std::string path) {
            std::string qA_sound_path = list_answercontent[itemcurrent_index]->getQASoundPath();
            auto qa_audi_id = cocos2d::experimental::AudioEngine::play2d(qA_sound_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(qa_audi_id, [=](int au_id, std::string path) {
                scheduleSoundGuidingGame();
                });
            });


    }

    for (int i = 0; i < _numques; i++) {
        if (!btdone->isVisible() && list_answercontent[i]->isCheckAnswer) {
            check_showBtDone++;
        }
    }
    if (!isVM) {
        results_lb->setString(StringUtils::format("You have %d out of %d right answers.", check_showBtDone, _numques));
    }
    else {
        results_lb->setString(StringUtils::format("Bạn có %d trên %d đáp án đúng.", check_showBtDone, _numques));
    }



    //auto check_showBtDone = 0;
    
    if (check_showBtDone == _numques) {
        this->unschedule("sound_guide");
        this->unschedule("checkSoundCount");
        if (CONFIG_MANAGER->isVietnameseStories()) {
            isEndGame = true;
            auto d = answer_content->_data;
            std::string audio_answer;
            for (auto a : d.answers) {
                if (a.text == d.answer) {
                    audio_answer = a.audio_path;
                }
            }
            this->scheduleOnce([this, audio_answer, tag](float dt) {
                int sound_id = cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_win.mp3");
                // set color 
                setColorforAll("green", tag);
                list_item[tag]->setWasChooseCorrectAnswer(true);
                cocos2d::experimental::AudioEngine::setFinishCallback(sound_id, [=](int au_id, std::string path) {
                    checkAllQAItem(tag);
                    scheduleSoundGuidingGame();
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                        this->scheduleOnce([this](float dt) {
                            this->onCloseGameVmonkey();
                            }, 0.5, "finish_game");
                        });
                    });
                }, 1.0, "delay_audio_right");

        }
        else {
            auto d = answer_content->_data;
            std::string audio_answer;
            for (auto a : d.answers) {
                if (a.text == d.answer) {
                    audio_answer = a.audio_path;
                }
            }
            list_item[tag]->setWasChooseCorrectAnswer(true);
            cocos2d::experimental::AudioEngine::stopAll();
            int sound_id = cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_win.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(sound_id, [=](int au_id, std::string path) {
                this->onGameOver();
                });
        }
    }
}

void MJGameQA::setInputConfigGame() {
    _numques = (int)_current_set.questions.size();
}

void MJGameQA::onDoneGame(Ref* sender) {
    this->hideArrowGuideDone();
    this->unschedule("sound_guide");
    CCLOG("on Done Game");

    if (itemcurrent_index == _numques) return;
    if (!game_play_enable) {
        this->pauseGame();
        return;
    }
    list_answercontent[itemcurrent_index]->setVisible(false);
    list_item[itemcurrent_index]->setStateItem();
    img_btdone->setVisible(true);
    itemcurrent_index = _numques;


    int count_true = 0;
    for (int i = 0; i < _numques; i++) {
        if (list_answercontent[i]->isCheckAnswer) {
            count_true++;
        }
        list_answercontent[i]->stopAllAudio();
        list_item[i]->setCheckTrueFalseItem(list_answercontent[i]->isCheckAnswer);
    }

    if (count_true == _numques) {
        alert_true->setVisible(true);
        btnext->setVisible(true);
        btnext->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.25), ScaleTo::create(0.5, 0.75), NULL)));
        alert_false->setVisible(false);
        cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_win.mp3");
    }
    else
    {
        alert_true->setVisible(false);
        alert_false->setVisible(true);
        cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_lose.mp3");
    }

    results_lb->setVisible(true);
    if (!isVM) {
        results_lb->setString(StringUtils::format("You have %d out of %d right answers.", count_true, _numques));
    }
    else {
        results_lb->setString(StringUtils::format("Bạn có %d trên %d đáp án đúng.", count_true, _numques));
    }

}

bool MJGameQA::isEnableTouchItem()
{
    return _enableTouchItem;
}

void MJGameQA::onFinishGame() {
    this->unschedule("sound_guide");

    auto text = Text::create("", "fonts/monkeystories_medium.ttf", 80);
    text->setPosition(this->getContentSize() / 2);
    text->setColor(Color3B::BLACK);
    this->addChild(text, 9000000);

    text->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.5f, 1.0f), DelayTime::create(2.0f), CallFunc::create([=] {
        if (CONFIG_MANAGER->isVietnameseStories()) {
            this->onCloseGameVmonkey();
        }
        else {
            this->onGameOver();
        }
        }), NULL));
}

mj::GameDataSet<mj::QADataSet> MJGameQA::loadData() {
    mj::GameDataSet<mj::QADataSet> data;
    mj::QADataSet set;
    set.questions = {
        {{"Of what descent is Bill?", "", "sounds/qa/soundtest.mp3"},
            {
                {"African", "", "sounds/qa/soundtest.mp3"},
                {"Vietnamese", "", "sounds/qa/soundtest.mp3"},
                {"Africa", "", "sounds/qa/soundtest.mp3"},
            },
            "African",
        },

        {{"what descent is Clara of?", "", "sounds/qa/soundtest.mp3"},
            {
                {"French", "", "sounds/qa/soundtest.mp3"},
                {"Indian", "", "sounds/qa/soundtest.mp3"},
                {"Irish", "", "sounds/qa/soundtest.mp3"},
                {"Brasil", "", "sounds/qa/soundtest.mp3"},
            },
            "Indian",
        },

        {{"Who is of Vietnamese descent?", "", "sounds/qa/soundtest.mp3"},
            {
                {"John", "", "sounds/qa/soundtest.mp3"},
                {"Min", "", "sounds/qa/soundtest.mp3"},
                {"Minh", "", "sounds/qa/soundtest.mp3"},
            },
            "Minh",
        },
    };

    data.sets.push(set);

    return data;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameQA::onLocateClickCallback(const std::string& callback_name) {
    if (callback_name == "onNext") {
        return CC_CALLBACK_1(MJGameQA::onNext, this);
    }

    return nullptr;
}

void MJGameQA::onRefresh(cocos2d::Ref* sender) {
    //    auto data = this->loadData();
    //    this->removeFromParent();
    //    Director::getInstance()->getRunningScene()->addChild(MJGameBase::createGame(data));

}

void MJGameQA::checkAllQAItem(int tag, bool nextQues)
{
    int checkedItem_count = 0;
    for (int i = 0; i < list_item.size(); i++ ) {
        if (list_item[i]->getWasChooseAnswer() || list_answercontent[i]->isReadingDone()) {
            checkedItem_count++;
        }
    }
    if (checkedItem_count == list_item.size()) {
        _allQuesWasChoosed = true;
    }

    if (_allQuesWasChoosed) {
        for (int i = 0; i < list_item.size(); i++) {
            if (!list_item[i]->getWasChooseCorrectAnswer()) {
                //onChooseQuesItem(list_item[i]);
                list_item[i]->showArrowGuide(false, true);
                break;
            }           
        }
    }
    if (nextQues) {
        if (checkedItem_count < list_item.size()) {
            this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
                if ((tag + 1) >= list_item.size()) return;
                onChooseQuesItem(list_item[tag + 1]);
                }), nullptr));
        }
    }
    
}

void MJGameQA::checkCountclickedQues()
{
    int checkedItem_count = 0;
    for (auto i : list_item) {
        if (i->getWasChooseAnswer()) {
            checkedItem_count++;
        }
    }
    if (checkedItem_count == list_item.size()) {
        _allQuesWasChoosed = true;
    }
}

void MJGameQA::setColorforAll(std::string color, int tag)
{
    if (tag >= list_item.size()) return;
    if (color == "red") {
        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_red")->setVisible(true);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_red")->setVisible(true);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_red")->setVisible(true);

        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_green")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_green")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_green")->setVisible(false);
        list_item[tag]->setVisibleItem_QARed("red");
        list_answercontent[tag]->setRedColorWrongAnswer(true);
    }
    else if (color == "green") {
        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_red")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_red")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_red")->setVisible(false);

        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_green")->setVisible(true);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_green")->setVisible(true);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_green")->setVisible(true);
        list_item[tag]->setVisibleItem_QARed("green");
        list_answercontent[tag]->setRedColorWrongAnswer(false);
        list_answercontent[tag]->setBlueColorCorrectAnswer();
    }
    else {
        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_red")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_red")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_red")->setVisible(false);

        this->getChildByName<cocos2d::ui::ImageView*>("icon_bg_green")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("bg_green")->setVisible(false);
        this->getChildByName<cocos2d::ui::ImageView*>("tick_title_green")->setVisible(false);
        list_item[tag]->setVisibleItem_QARed("origin");
        list_answercontent[tag]->setRedColorWrongAnswer(false);
    }
}

void MJGameQA::onNext(cocos2d::Ref* senfer) {
    if (CONFIG_MANAGER->isVietnameseStories()) {
        this->onCloseGameVmonkey();
    }
    else {
        this->onGameOver();
    }
}

void MJGameQA::showArrowGuideDone() {
    if (submit_arrowguide_showed) {
        return;
    }
    submit_arrowguide_showed = true;
    auto arrow_guide = cocos2d::ui::ImageView::create("games/arrow_guide.png");
    arrow_guide->setAnchorPoint(Vec2(0.5, 0));
    arrow_guide->setPosition(btdone->getPosition() + Vec2(btdone->getContentSize().width / 2, btdone->getContentSize().height + 10));
    this->addChild(arrow_guide, 10000);
    auto duration = 0.3f;
    arrow_guide->runAction(Repeat::create(Sequence::create(MoveBy::create(duration, Vec2(0, 10)), MoveBy::create(duration, Vec2(0, -10)), NULL), 100000));
    arrow_guide->setName("arrow_guide");
}

void MJGameQA::hideArrowGuideDone() {
    this->removeChildByName("arrow_guide");
}

bool MJGameQA::is_auto_submit = false;
