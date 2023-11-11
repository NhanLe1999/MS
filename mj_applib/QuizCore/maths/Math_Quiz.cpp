
//
//  Math_Quiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#include "Math_Quiz.h"
#include "TestingModeUtils.h"

#include "CalendarBase.h"

#pragma mark - Contructor
Math_Quiz * Math_Quiz::createQuiz() {
    auto p = new Math_Quiz();
    if (p && p->_initQuiz()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Math_Quiz::_initQuiz() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    setBackGroundQuiz("themeui/bg.jpg");

    this->setContentSize(quiz_size);
    
    /*---- Layout dùng để di 'move' object ----*/
    _panel_move=Layout::create();
    _panel_move->setContentSize(quiz_size);
    _panel_move->setLayoutType(Layout::Type::ABSOLUTE);
    _panel_move->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _panel_move->setBackGroundColor(cocos2d::Color3B::BLACK);
    _panel_move->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _panel_move->setPosition(quiz_size/2);
    _panel_move->setOpacity(0);
    _panel_move->setSwallowTouches(true);
    this->addChild(_panel_move, 10);
    
    /*---- Layout chính: chứa phần nội dung quiz ----*/
    _panel=Layout::create();
    _panel->setContentSize(quiz_size);
    _panel->setLayoutType(Layout::Type::ABSOLUTE);
    _panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _panel->setPosition(quiz_size/2);
    _panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _panel->setBackGroundColor(cocos2d::Color3B::BLACK);
    _panel->setOpacity(0);
    _panel->setSwallowTouches(true);
    this->addChild(_panel, 2);
    
    /*---- Layout phải: chứa phần nội dung tương tác ----*/
    _panel_right=Layout::create();
    _panel_right->setContentSize(quiz_size);
    _panel_right->setLayoutType(Layout::Type::ABSOLUTE);
    _panel_right->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
    _panel_right->setPosition(cocos2d::Vec2(quiz_size.width, quiz_size.height/2));
    _panel_right->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _panel_right->setBackGroundColor(cocos2d::Color3B::BLUE);
    _panel_right->setOpacity(0);
    this->addChild(_panel_right, 3);
    
    /*---- Layout dưới: chứa phần nội dung tương tác ----*/
    _panel_bot=Layout::create();
    _panel_bot->setContentSize(quiz_size);
    _panel_bot->setLayoutType(Layout::Type::ABSOLUTE);
    _panel_bot->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
    _panel_bot->setPosition(cocos2d::Vec2(quiz_size.width/2, 0));
    _panel_bot->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _panel_bot->setBackGroundColor(cocos2d::Color3B::RED);
    _panel_bot->setOpacity(0);
    this->addChild(_panel_bot, 3);
    
    /*---- Layout ngăn tương tác: không cho tương tác lên màn hình quiz ----*/
    _panel_disable=Layout::create();
    _panel_disable->setContentSize(quiz_size);
    _panel_disable->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _panel_disable->setPosition(quiz_size/2);
    _panel_disable->setVisible(false);
    this->addChild(_panel_disable, INT_MAX-1);
    
    /*---- Text info: thông tin ngắn gọn về quiz ----*/
    _debug_text=cocos2d::ui::Text::create();
    _debug_text->setFontSize(18);
    _debug_text->setTextColor(cocos2d::Color4B::RED);
    _debug_text->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    _debug_text->setPosition(cocos2d::Vec2(10, 5));
    this->addChild(_debug_text, 9);
    
    return true;
}

void Math_Quiz::initQuiz(mj::model::Game game) {
    _game = game;
    _game_path = math_statistic->getGamePath(_game);
    if (testing::TestManager::getInstance()->getTestMode() == testing::activity) {
        image_prefix = _game_path;
    }
    setVoiceId(3);
}

void Math_Quiz::onStart() {
    math_statistic->setCurrentGame(_game);
    std::string pop_out = cocos2d::StringUtils::format("movenumber/pop_out_%d.mp3", cocos2d::random(1, 2));
    audio_helper->play_sequence(new math::SequenceAudio(), pop_out, pop_out, "lalala.mp3");

    showQuestionBox();
}

void Math_Quiz::startQuestion() {}

void Math_Quiz::onExplanation(bool correct) {}

void Math_Quiz::onComplete() {
    audio_helper->play_sequence(new math::SequenceAudio(), "howmany/pop_out10.mp3", "howmany/pop_out10.mp3");
    hideQuestionBox();
}

void Math_Quiz::autoTesting() {}

void Math_Quiz::onGuideStarted() {}

void Math_Quiz::onGuideDone() {}

void Math_Quiz::setCompleteCallback(math::callback::onCompleteQuiz callback) {
    _complete_callback=callback;
}

void Math_Quiz::startGuide() {
    GuideCenter->startGuide(CC_CALLBACK_0(Math_Quiz::onGuideStarted, this), CC_CALLBACK_0(Math_Quiz::onGuideDone, this));
}

#pragma mark - Game info
mj::model::Game Math_Quiz::getGame() {
    return _game;
}

void Math_Quiz::setDebugText(std::string text) {
    _debug_text->setString(text);
}

#pragma mark - Control
void Math_Quiz::quizBegan(float delay, bool show) {
    enableControlQuiz();
    disableControlQuiz();
    show_question = show;
    scheduleOnce([=](float) {
        //audio transition IN
        std::string pop_out = cocos2d::StringUtils::format("movenumber/pop_out_%d.mp3", cocos2d::random(1, 2));
        audio_helper->play_sequence(new math::SequenceAudio(), pop_out, pop_out, "lalala.mp3");
        math_statistic->setCurrentGame(_game);
        showQuestionBox();
    }, delay, "quiz_began");
}

void Math_Quiz::quizEnded(float delay, bool show_question) {
    GuideCenter->stopGuide();
    scheduleOnce([=](float){
        audio_helper->play_sequence(new math::SequenceAudio(), "howmany/pop_out10.mp3", "howmany/pop_out10.mp3");
    }, delay, "quiz_ended");
    hideQuestionBox(delay);
}

void Math_Quiz::onShow() {}

float Math_Quiz::onHide(bool cleanup) {
    float time = 0;
    if (display_type == math::common::has_right_bar) {
        time = onHideRightBar();
    } else if (display_type == math::common::has_bottom_bar) {
        time = onHideBottomBar();
    } else if (display_type == math::common::has_color_pad_right || display_type == math::common::has_color_pad_bottom) {
        time = hideColorsPad();
    }
    
    scheduleOnce([=](float){
        this->removeFromParentAndCleanup(cleanup);
    }, time, "quiz_hide");
    return time;
}

void Math_Quiz::disableControlQuiz(float time) {
    unschedule("disable_touch_quiz");
    _panel_disable->setVisible(true);
    this->getEventDispatcher()->setEnabled(false);
    if (time>0) {
        scheduleOnce([=](float dt){
            this->getEventDispatcher()->setEnabled(true);
            _panel_disable->setVisible(false);
        }, time, "disable_touch_quiz");
    }
}

void Math_Quiz::enableControlQuiz() {
    unschedule("disable_touch_quiz");
    this->getEventDispatcher()->setEnabled(true);
    _panel_disable->setVisible(false);
}

void Math_Quiz::setVoiceId(int vid) {
    voice_id = vid;
}

void Math_Quiz::showQuestionBox(float dt) {
    auto panel_size = _panel->getContentSize();
    _question->onStart(dt);
}

void Math_Quiz::hideQuestionBox(float dt) {
    float delay = dt+0.5;
    if (display_type == math::common::has_right_bar) {
        delay = onHideRightBar(dt);
    } else if (display_type == math::common::has_bottom_bar) {
        delay = onHideBottomBar(dt);
    } else if (display_type == math::common::has_color_pad_right || display_type == math::common::has_color_pad_bottom) {
        delay = hideColorsPad(dt);
    }
    _question->onHide(delay - 0.5);
    scheduleOnce([=](float){
        if (this->checkStopTrialWhenComplete()) {
            _complete_callback(true);
        }
    }, delay+0.5, "complete_math_quiz");
}

float Math_Quiz::showConratulation(float dt) {
    return 1;
}

#pragma mark - Question
void Math_Quiz::parseQuestion (bool loop, int loop_number) {
    _question=mpool->getQuestion();
    _question->setEnable([=](){
        this->startQuestion();
    });
    _question->setAudioCallback(CC_CALLBACK_1(Math_Quiz::speakQuestion, this));
    _question->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height - 20));
    this->addChild(_question, INT_MAX-2);
    
    auto question_list = math::question::Parse::parseQuestion(_game_path, image_prefix, loop, loop_number);
    for (math::question::Question question_data : question_list) {
        _question->pushData(question_data);
    }
}

void Math_Quiz::parseQuestionWithLoops(std::vector<int> loops) {
    _question=mpool->getQuestion();
    _question->setEnable([=](){
        this->startQuestion();
    });
    _question->setAudioCallback(CC_CALLBACK_1(Math_Quiz::speakQuestion, this));
    _question->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height -20));
    this->addChild(_question, INT_MAX-2);
    
    auto question_list = math::question::Parse::parseQuestionWithLoops(_game_path, image_prefix, loops);
    for (math::question::Question question_data : question_list) {
        _question->pushData(question_data);
    }
}

Math_Question* Math_Quiz::getQuestion() {
    return _question;
}

void Math_Quiz::nextQuestion(float delay) {
    scheduleOnce([=](float){
        stopSpeakQuestion();
        if (checkStopTrial()) {
            this->unschedule("hideQuestion");
            if (!_question->goNext()) {
                onComplete();
            }
        }
    }, delay, "key next question");
}

void Math_Quiz::goToQuestionAtIndex(int index, float delay) {
    scheduleOnce([=](float){
        if (checkStopTrial()) {
            this->unschedule("hideQuestion");
            if (!_question->goToQuestionAtIndex(index)) {
                onComplete();
            }
        }
    }, delay, "key go question at index");
}

#pragma mark - Audio
void Math_Quiz::onPreloadSeqDone(std::string key) {}

void Math_Quiz::onPlaySeqDone(std::string key) {
    if (key == "spreak_question") {
        isSpeaking=false;
        GuideCenter->setGuideEnabled();
        if (!show_question) {
            scheduleOnce([=](float){
                _question->hideQuestion();
            }, 0.5, "hideQuestion");
        } else {
            _question->enableTouchQuestion();
        }
    }
}

void Math_Quiz::speakQuestion(bool stop)
{
    if (stop) {
        stopSpeakQuestion();
    }
    //Audio controller
    if (isSpeaking) {
        return;
    }
    
    GuideCenter->setGuideDisabled();
    auto voices_list = _question->getQuestionVoices(voice_id);
    
    speak_question = new math::SequenceAudio();
    speak_question->audios = voices_list;
    speak_question->name = "spreak_question";
    speak_question->func = CC_CALLBACK_1(Math_Quiz::onPlaySeqDone, this);
    
    audio_helper->play_sequence(speak_question);
    isSpeaking=true;
}

void Math_Quiz::stopSpeakQuestion()
{
    if (isSpeaking) {
        isSpeaking = false;
        if (speak_question != nullptr) {
            audio_helper->stop_sequence(speak_question);
            delete speak_question;
            speak_question = nullptr;
        }
    }
}

void Math_Quiz::playObjectAudio(Math_Object *object, std::string key) {
    audio_helper->play_sequence(object->getImageSource()->getAudios(key), nullptr, "");
}

#pragma mark - UI Display
void Math_Quiz::setBackGroundQuiz(std::string path) {
    if (_bg_quiz) {
        _bg_quiz->removeFromParentAndCleanup(true);
        _bg_quiz = nullptr;
    }
    _bg_quiz = cocos2d::ui::ImageView::create(getUIPath(path));
    _bg_quiz->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _bg_quiz->setPosition(quiz_size/2);
    if (_bg_quiz->getContentSize().width < quiz_size.width) {
        _bg_quiz->setScale9Enabled(true);
        _bg_quiz->setCapInsets(cocos2d::Rect(1, 1, _bg_quiz->getVirtualRendererSize().width-2, _bg_quiz->getVirtualRendererSize().height-2));
        _bg_quiz->setContentSize(quiz_size);
    }
    this->addChild(_bg_quiz, 0);
}

void Math_Quiz::addChildNode(Node *object, int zorder) {
    _panel->addChild(object, zorder);
    
    if (object->getName() == "Math_Object") {
        Math_Object* mo = static_cast<Math_Object*>(object);
        mo->setSuperNode(_panel_move);
    } else if (object->getName() == "Math_Group") {
        Math_Group* mg = static_cast<Math_Group*>(object);
        mg->setSuperNode(_panel_move);
    }
}

void Math_Quiz::addChildNodeToRightPanel(Node *object, int zorder) {
    _panel_right->addChild(object, zorder);
    
    if (object->getName() == "Math_Object") {
        Math_Object* mo = static_cast<Math_Object*>(object);
        mo->setSuperNode(_panel_move);
    } else if (object->getName() == "Math_Group") {
        Math_Group* mg = static_cast<Math_Group*>(object);
        mg->setSuperNode(_panel_move);
    }
}

void Math_Quiz::addChildNodeToBottomPanel(Node *object, int zorder) {
    _panel_bot->addChild(object, zorder);
    
    if (object->getName() == "Math_Object") {
        Math_Object* mo = static_cast<Math_Object*>(object);
        mo->setSuperNode(_panel_move);
    } else if (object->getName() == "Math_Group") {
        Math_Group* mg = static_cast<Math_Group*>(object);
        mg->setSuperNode(_panel_move);
    }
}

/*------ Slide Bar RIGHT ------*/
void Math_Quiz::onShowRightBar(float delay, float width, math::callback::onAnswerClicked callback_done) {
    display_type =  math::common::has_right_bar;
    right_bar_rect = cocos2d::Rect(0, 0, quiz_size.width * width, quiz_size.height);
    _panel_right->setContentSize(cocos2d::Size(quiz_size.width * width, quiz_size.height));
    
    auto position = cocos2d::Vec2((1-width) * quiz_size.width, quiz_size.height/2);
    _panel_right->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position))));

    //background
    _bg_panel = cocos2d::ui::ImageView::create(getUIPath("themeui/rightbg.png"));
    _bg_panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
    _bg_panel->setScale9Enabled(true);
    auto render_size = _bg_panel->getVirtualRendererSize();
    _bg_panel->setCapInsets(cocos2d::Rect(render_size.width*0.5, render_size.height*0.5, render_size.width*0.2, render_size.height*0.2));
    _bg_panel->setContentSize(cocos2d::Size(quiz_size.width * width + 100, quiz_size.height));
    _bg_panel->setPosition(cocos2d::Vec2(0, quiz_size.height/2));
    addChildNodeToRightPanel(_bg_panel);
    
    //sub frame
    _bg_panel_sub = cocos2d::ui::ImageView::create(getUIPath("themeui/box.png"));
    _bg_panel_sub->setScale9Enabled(true);
    auto render_size_sub = _bg_panel_sub->getVirtualRendererSize();
    _bg_panel_sub->setCapInsets(cocos2d::Rect(render_size_sub.width*0.4, render_size_sub.height*0.4, render_size_sub.width*0.2, render_size_sub.height*0.2));
    _bg_panel_sub->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
    _bg_panel_sub->setContentSize(cocos2d::Size(quiz_size.width * width - 50, quiz_size.height - 50));
    _bg_panel_sub->setPosition(cocos2d::Vec2(25, quiz_size.height/2));
    addChildNodeToRightPanel(_bg_panel_sub);
    
    if (callback_done != nullptr) {
        _bg_panel_sub->setContentSize(cocos2d::Size(quiz_size.width * width - 50, quiz_size.height - 170));
        _bg_panel_sub->setPosition(cocos2d::Vec2(25, quiz_size.height /2 + 55));

        done = mpool->getAnswer();
        done->setEnable("", math::common::done, callback_done, getAnswerSize(1));
        done->setIcon(getTextByTextGroup("Done"));
        done->setTitleWithIcon("name_1");
        done->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        done->setPosition(cocos2d::Vec2( quiz_size.width * width / 2, 75));
        done->onShowInParent(_panel_right, 2, 0.5+delay, math::animation::SlideUp);
    }
}

float Math_Quiz::onHideRightBar(float delay) {
    display_type =  math::common::normal;
    scheduleOnce([=](float){
        float wait_done = 0;
        if (done != nullptr) {
            done->setDisable(0, math::animation::SlideUp);
            wait_done = 0.5;
        }
        auto position = cocos2d::Vec2(quiz_size.width, quiz_size.height/2);
        _panel_right->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(wait_done), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))));
    }, delay, "hide right bar");
    
    if (done != nullptr) {
        delay += 0.5;
    }
    return delay + 0.75;
}

/*------ Slide Bar BOTTOM ------*/
void Math_Quiz::onShowBottomBar(float delay, float height, bool sub_frame, math::callback::onAnswerClicked callback_done, math::callback::onAnswerClicked callback_trash, bool done_center) {
    display_type =  math::common::has_bottom_bar;
    bot_bar_rect = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height * height);
    _panel_bot->setContentSize(cocos2d::Size(quiz_size.width, quiz_size.height * height));
    
    auto position = cocos2d::Vec2(quiz_size.width/2, bot_bar_rect.size.height);
    _panel_bot->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position))));

    //background bar
    _bg_panel = cocos2d::ui::ImageView::create(getUIPath("themeui/botbg.png"));
    _bg_panel->setScale9Enabled(true);
    auto render_size = _bg_panel->getVirtualRendererSize();
    _bg_panel->setCapInsets(cocos2d::Rect(render_size.width*0.45, render_size.height*0.8, render_size.width*0.1, render_size.height*0.1));
    _bg_panel->setContentSize(cocos2d::Size(quiz_size.width, quiz_size.height * height + 50));
    _bg_panel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _bg_panel->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height * height/2 - 25));
    addChildNodeToBottomPanel(_bg_panel);
    
    //sub frame
    float delta = 0;
    if (sub_frame) {
        delta = 10;
        _bg_panel_sub = cocos2d::ui::ImageView::create(getUIPath("themeui/box.png"));
        _bg_panel_sub->setScale9Enabled(true);
        auto render_size_sub = _bg_panel_sub->getVirtualRendererSize();
        _bg_panel_sub->setCapInsets(cocos2d::Rect(render_size_sub.width*0.4, render_size_sub.height*0.4, render_size_sub.width*0.2, render_size_sub.height*0.2));
        _bg_panel_sub->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        
        _bg_panel_sub->setContentSize(cocos2d::Size(quiz_size.width - 50, quiz_size.height * height + 50));
        _bg_panel_sub->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height * height / 2 - 45));
        if (callback_done != nullptr) {
            _bg_panel_sub->setContentSize(cocos2d::Size(quiz_size.width - 300, quiz_size.height * height + 50));
            _bg_panel_sub->setPosition(cocos2d::Vec2(quiz_size.width/2 - 125, quiz_size.height * height /2 - 45));
        }
        if (callback_trash != nullptr) {
            _bg_panel_sub->setContentSize(cocos2d::Size(quiz_size.width - 450, quiz_size.height * height + 50));
            _bg_panel_sub->setPosition(cocos2d::Vec2(quiz_size.width/2 - 50, quiz_size.height * height/2 - 45));
        }
        addChildNodeToBottomPanel(_bg_panel_sub);
        
        if (callback_done != nullptr) {
            done = mpool->getAnswer();
            done->setEnable("", math::common::done, callback_done, getAnswerSize(2));
            done->setIcon(getTextByTextGroup("Done"));
            done->setTitleWithIcon("name_1");
            done->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            done->setPosition(cocos2d::Vec2(quiz_size.width - 150, quiz_size.height*height*0.5-delta));
            done->onShowInParent(_panel_bot, 2, 0.5+delay, math::animation::SlideUp);
        }
        
        if (callback_trash != nullptr) {
            auto img_trash = math::resource::ResHelper::createImage(getUIPath("common/trash.png"));
            trash = mpool->getAnswer();
            trash->setEnable("", math::common::done, callback_trash, getAnswerSize(2));
            trash->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            trash->setCustomAnswer(img_trash, cocos2d::Size(100, 100), 10);
            trash->setPosition(cocos2d::Vec2(100,quiz_size.height*height*0.5 - delta));
            trash->onShowInParent(_panel_bot, 2, 0.5+delay, math::animation::SlideUp);
        }
    } else {
        if (callback_done != nullptr) {
            done = mpool->getAnswer();
            done->setEnable("", math::common::done, callback_done, getAnswerSize(2));
            done->setIcon(getTextByTextGroup("Done"));
            done->setTitleWithIcon("name_1");
            done->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            if (done_center) {
                done->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*height*0.5-delta));
            } else {
                done->setPosition(cocos2d::Vec2(quiz_size.width - 150, quiz_size.height*height*0.5-delta));
            }
            done->onShowInParent(_panel_bot, 2, 0.5+delay, math::animation::SlideUp);
        }
    }
}

float Math_Quiz::onHideBottomBar(float delay) {
    display_type =  math::common::normal;
    scheduleOnce([=](float){
        float wait_done = 0;
        if (done != nullptr) {
            done->setDisable(0, math::animation::SlideUp);
            wait_done = 0.5;
        }
        if (trash != nullptr) {
            trash->setDisable(0, math::animation::SlideUp);
            wait_done = 0.5;
        }
        auto position = cocos2d::Vec2(quiz_size.width/2, 0);
        _panel_bot->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(wait_done), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))));
    }, delay, "hide bottom bar");
    
    if (done != nullptr || trash != nullptr) {
        delay += 0.5;
    }
    return delay + 0.75;
}

/*------ Slide Bar RIGHT or BOTTOM - COLOR ------*/
float Math_Quiz::showColorsPad (std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done, float delay, bool is_right) {
    float bar_size = 250.0;
    
    _color_pens.clear();
    
    float pen_width = 300;
    for (std::string color : colors) {
        auto imgs = getColor(color);
        if (imgs != nullptr) {
            auto pen = mpool->getObject();
            pen->setEnable(0, math::common::touch, imgs, nullptr);
            pen->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            pen->setObjectSize(cocos2d::Size(300, 89));
            auto render_size = pen->getImage()->getVirtualRendererSize();
            pen->getImage()->setScale9Enabled(true);
            pen->getImage()->setCapInsets(cocos2d::Rect(render_size.width*0.5, 1, render_size.width*0.1, render_size.height-2));
            _color_pens.push_back(pen);
            
            if (is_right) {
                float bar_width = 250.0/quiz_size.width;
                bar_width = bar_width < 0.2 ? 0.2 : bar_width;
                bar_size = bar_width * quiz_size.width;
                pen_width = bar_size + 50;
                
                pen->setObjectSize(cocos2d::Size(pen_width, 89));
                
                addChildNodeToRightPanel(pen, 3);
                pen->setTouchCallback([=](Node* node) {
                    Math_Object* selected = static_cast<Math_Object*>(node);
                    float start_position_x = bar_size;
                    float target_position_x = bar_size * 0.5 + 40;// (bar_size + 50) *  - 70;// 180;
                    for (Math_Object* pen : _color_pens) {
                        if (abs(pen->getPosition().x - target_position_x) < 10) {
                            if (pen->isSame(selected)) {
                                return;
                            }
                            pen->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(start_position_x, pen->getPosition().y))));
                        }
                    }
                    auto current_pos= selected->getPosition();
                    selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(target_position_x, current_pos.y))));
                    _color_selected->setVisible(true);
                    _color_selected->setPosition(cocos2d::Vec2(current_pos.x, current_pos.y+5));
                    _color_selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(target_position_x, current_pos.y+5))));
                    if (callback != nullptr) {
                        callback(node);
                    }
                });
            } else {
                bar_size = quiz_size.height * 0.3;
                addChildNodeToBottomPanel(pen, 3);
                pen->setTouchCallback([=](Node* node) {
                    this->disableControlQuiz(0.3);
                    Math_Object* selected = static_cast<Math_Object*>(node);
                    audio_helper->play("colorusekey/click3.mp3");
                    float start_position_y = quiz_size.height*0.3-250;
                    float target_position_y = quiz_size.height*0.3-180;
                    for (Math_Object* pen : _color_pens) {
                        if (abs(pen->getPosition().y - target_position_y) < 10) {
                            if (pen->isSame(selected)) {
                                return;
                            }
                            pen->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(pen->getPosition().x, start_position_y))));
                        }
                    }
                    auto current_pos= selected->getPosition();
                    selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(current_pos.x, target_position_y))));
                    _color_selected->setVisible(true);
                    _color_selected->setPosition(cocos2d::Vec2(current_pos.x + 5, current_pos.y));
                    _color_selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(current_pos.x + 5, target_position_y))));
                    if (callback != nullptr) {
                        callback(node);
                    }
                });
            }
        } else {
            CCLOG("Math Error: Missing color '%s'", color.c_str());
        }
    }
    if (_color_pens.empty()) {
        return 0;
    }
    math::func::shuffle(_color_pens);
    
    //ui pad color
    _color_selected = cocos2d::ui::ImageView::create(getUIPath("themeui/penselect.png"));
    _color_selected->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _color_selected->setScale9Enabled(true);
    _color_selected->setCapInsets(cocos2d::Rect(_color_selected->getVirtualRendererSize().width*0.4, 1, _color_selected->getVirtualRendererSize().width*0.2, _color_selected->getVirtualRendererSize().height-2));
    _color_selected->setContentSize(cocos2d::Size(pen_width+50, 130));
    _color_selected->setPosition(cocos2d::Vec2::ZERO);
    _color_selected->setVisible(false);

    if (is_right) {
        addChildNodeToRightPanel(_color_selected, 2);

        float bar_width = 250.0/quiz_size.width;
        bar_width = bar_width < 0.2 ? 0.2 : bar_width;
        onShowRightBar(delay, bar_width, callback_done);
        display_type =  math::common::has_color_pad_right;
        
        _bg_panel_sub->loadTexture(getUIPath("themeui/colorpad.png"));
        auto render_size_sub = _bg_panel_sub->getVirtualRendererSize();
        _bg_panel_sub->setCapInsets(cocos2d::Rect(render_size_sub.width*0.4, render_size_sub.height*0.4, render_size_sub.width*0.2, render_size_sub.height*0.2));
        auto sub_size = _bg_panel_sub->getContentSize();
        _bg_panel_sub->setContentSize(cocos2d::Size(sub_size.width + 100, sub_size.height));
        
        auto pen_size = _color_pens.front()->getContentSize();
        float start_position_x = bar_size;
        
        float delta = 20;
        while ((sub_size.height - (int)_color_pens.size() * (pen_size.height + delta) - delta * 2) < 0) {
            delta -= 5;
        }
        float start_position_y = ((callback_done == nullptr) ? (quiz_size.height * 0.5) : (quiz_size.height - 25 - sub_size.height/2)) + ((int)_color_pens.size() * (pen_size.height + delta) - delta)/2 - pen_size.height/2;
        
        int i = 0;
        for (Math_Object* pen : _color_pens) {
            pen->setPosition(cocos2d::Vec2(start_position_x, start_position_y));
            start_position_y -= (pen_size.height + delta);
            pen->onShow(0.4 + delay + 0.1 * ((int)_color_pens.size()- (i++)), math::animation::SlideLeft, right_bar_rect.size.width*.7);
        }
    } else {
        addChildNodeToBottomPanel(_color_selected, 2);

        float bar_height = 0.3;
        onShowBottomBar(delay, bar_height, true, callback_done);
        display_type =  math::common::has_color_pad_bottom;
        
        _bg_panel_sub->loadTexture(getUIPath("themeui/colorpad.png"));
        auto render_size_sub = _bg_panel_sub->getVirtualRendererSize();
        _bg_panel_sub->setCapInsets(cocos2d::Rect(render_size_sub.width*0.4, render_size_sub.height*0.4, render_size_sub.width*0.2, render_size_sub.height*0.2));
        auto sub_size = _bg_panel_sub->getContentSize();
        _bg_panel_sub->setContentSize(cocos2d::Size(sub_size.height, sub_size.width));
        _bg_panel_sub->setRotation(90);
        _color_selected->setRotation(90);
        
        auto pen_size = _color_pens.front()->getContentSize();
        float start_position_y = quiz_size.height*bar_height-250;
        float delta = 40;
        while ((sub_size.width - (int)_color_pens.size() * (pen_size.height + delta) - delta * 2) < 0) {
            delta -= 5;
        }
        float start_position_x = ((callback_done == nullptr) ? (quiz_size.width*0.5) : (25+sub_size.width/2)) - ((int)_color_pens.size() * (pen_size.height + delta) - delta)/2 + pen_size.height/2;
        
        int i = 0;
        for (Math_Object* pen : _color_pens) {
            pen->setContentSize(cocos2d::Size(pen_size.height, pen_size.width));
            pen->getImage()->setPosition(pen->getContentSize()/2);
            pen->getImage()->setRotation(90);
            pen->setPosition(cocos2d::Vec2(start_position_x, start_position_y));
            start_position_x += (pen_size.height + delta);
            pen->onShow(0.4 + delay + 0.1 * (i++), math::animation::SlideUp, right_bar_rect.size.width*.7);
        }
    }
    
    return bar_size;
}

float Math_Quiz::hideColorsPad (float delay) {
    _color_selected->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::FadeOut::create(0.2), cocos2d::CallFunc::create([=](){
        _color_selected->removeFromParentAndCleanup(true);
    }), NULL));
    
    //Hide button
    if (display_type == math::common::has_color_pad_right) {
        //Hide pen
        float start_position_x = 260;
        float target_position_x = 180;
        int i=0;
        for (Math_Object* pen : _color_pens) {
            pen->setLocked(true);
            if (abs(pen->getPosition().x - target_position_x) < 10) {
                pen->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::EaseSineIn::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(start_position_x, pen->getPosition().y))), cocos2d::DelayTime::create(i*0.1), cocos2d::CallFunc::create([=](){
                    pen->onHide(0, math::animation::SlideLeft, right_bar_rect.size.width*.55);
                }), NULL));
            } else {
                pen->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.3+ delay + i*0.1), cocos2d::CallFunc::create([=](){
                    pen->onHide(0, math::animation::SlideLeft, right_bar_rect.size.width*.55);
                })));
            }
            i++;
        }
        
        //Hide panel
        scheduleOnce([=](float){
            for (cocos2d::Node* child : ui_elements) {
                child->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::FadeOut::create(0.2), cocos2d::CallFunc::create([=](){
                    child->removeFromParentAndCleanup(true);
                }), NULL));
            }
            float wait_done = 0;
            if (done != nullptr) {
                done->setDisable();
                wait_done = 0.5;
            }
            auto position = cocos2d::Vec2(quiz_size.width, quiz_size.height/2);
            _panel_right->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(wait_done), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))));
        }, delay + 0.8 + ((int)_color_pens.size()-1) * 0.1, "hide right bar");
    } else {
        //Hide pen
        float start_position_y = quiz_size.height*0.3-250;
        float target_position_y = quiz_size.height*0.3-180;
        int i=0;
        for (Math_Object* pen : _color_pens) {
            pen->setLocked(true);
            if (abs(pen->getPosition().y - target_position_y) < 10) {
                pen->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::EaseSineIn::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(pen->getPosition().x, start_position_y))), cocos2d::DelayTime::create(i*0.1), cocos2d::CallFunc::create([=](){
                    pen->onHide(0, math::animation::SlideUp, right_bar_rect.size.width*.55);
                }), NULL));
            } else {
                pen->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.3+ delay + i*0.1), cocos2d::CallFunc::create([=](){
                    pen->onHide(0, math::animation::SlideUp, right_bar_rect.size.width*.55);
                })));
            }
            i++;
        }
        
        //Hide panel
        scheduleOnce([=](float){
            for (cocos2d::Node* child : ui_elements) {
                child->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::FadeOut::create(0.2), cocos2d::CallFunc::create([=](){
                    child->removeFromParentAndCleanup(true);
                }), NULL));
            }
            float wait_done = 0;
            if (done != nullptr) {
                done->setDisable();
                wait_done = 0.5;
            }
            auto position = cocos2d::Vec2(quiz_size.width/2, 0);
            _panel_bot->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(wait_done), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))));
        }, delay + 0.8 + ((int)_color_pens.size()-1) * 0.1, "hide bottom bar");
    }
    
    display_type =  math::common::normal;
    return delay + 1.3 + ((int)_color_pens.size()-1) * 0.1;
}

#pragma mark Special images
//==== COLORS =====
cocos2d::Color4B Math_Quiz::getColor4B(std::string color, bool white) {
    auto m_color = math::func::lowerString(color);
    
    if (!white && m_color == "white") {
        return cocos2d::Color4B(0, 0, 0, 255);
    }
    
    if (m_color == "red")       return cocos2d::Color4B(255, 51,  0,   255);
    if (m_color == "yellow")    return cocos2d::Color4B(255, 221, 59,  255);
    if (m_color == "grey")      return cocos2d::Color4B(169, 169, 169, 255);
    if (m_color == "blue")      return cocos2d::Color4B(86,  154, 239, 255);
    if (m_color == "brown")     return cocos2d::Color4B(149, 75,  23,  255);
    if (m_color == "green")     return cocos2d::Color4B(107, 214, 95,  255);
    if (m_color == "orange")    return cocos2d::Color4B(255, 102, 0,   255);
    if (m_color == "pink")      return cocos2d::Color4B(253, 106, 188, 255);
    if (m_color == "purple")    return cocos2d::Color4B(149, 89,  237, 255);
    if (m_color == "white")     return cocos2d::Color4B(255, 255, 255, 255);
    return cocos2d::Color4B(0, 0, 0, 255);
}

cocos2d::Color3B Math_Quiz::getColor3B(std::string color, bool white) {
    auto color4b = getColor4B(color, white);
    return cocos2d::Color3B(color4b.r, color4b.g, color4b.b);
}

cocos2d::Color4F Math_Quiz::getColor4F(std::string color, bool white) {
    auto color4b = getColor4B(color, white);
    return cocos2d::Color4F(color4b.r/255.0, color4b.g/255.0, color4b.b/255.0, 1);
}

std::vector<math::resource::Image*> Math_Quiz::getColors() {
    if (_colors_img.empty()) {
        _colors_img = math::resource::ResHelper::getImages("game/colors.json");
        for (int i=0; i<_colors_img.size(); i++) {
            _colors_img[i]->path = getUIPath(_colors_img[i]->path);
        }
    }
    return _colors_img;
}

math::resource::Image* Math_Quiz::getColor(std::string name) {
    auto color_list = getColors();
    for (math::resource::Image* img : color_list) {
        if (img->getText() == name) {
            return img;
        }
    }
    return nullptr;
}

std::vector<math::resource::Image*> Math_Quiz::getNumbers(int from, int to, math::lang::MathLanguage lang)  {
    std::vector<math::resource::Image*> numbers;
    for (int i=from; i<=to; i++) {
        numbers.push_back(math::number::getNumber(i,lang));
    }
    return numbers;
}

std::vector<math::resource::Image*> Math_Quiz::getOrdinals(int from, int to)  {
    std::vector<math::resource::Image*> numbers;
    for (int i=from; i<=to; i++) {
        numbers.push_back(math::ordinal::getOrdinal(i));
    }
    return numbers;
}

std::vector<math::resource::Image*> Math_Quiz::getOperators() {
    if (_operators_img.empty()) {
        _operators_img = math::resource::ResHelper::getImages("game/operators.json");
        
        for (math::resource::Image* img : _operators_img) {
            img->path = getUIPath(img->path);
        }
    }
    return _operators_img;
}

math::resource::Image* Math_Quiz::getOperator(std::string name) {
    auto img_list = getOperators();
    for (math::resource::Image* img : img_list) {
        if (img->getText() == name) {
            return img;
        }
    }
    return nullptr;
}

math::resource::Image* Math_Quiz::groupBG(std::string color) {
    auto m_color = math::func::lowerString(color);
    return math::resource::ResHelper::createImage(getUIPath("common/groupbg/"+m_color+".png"));
}

math::resource::Image* Math_Quiz::objectBG() {
    return math::resource::ResHelper::createImage(getUIPath("common/drag.png"));
}

math::resource::Image* Math_Quiz::slotBG() {
    return math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
}

math::resource::Image* Math_Quiz::getTextByTextGroup(std::string text_group) {
    auto img = math::resource::ResHelper::createImage("");
    math::resource::Property prop_name;
    prop_name.type = math::resource::Property::PROP;
    prop_name.key = "name_1";
    math::resource::Text name_text;
    name_text.text = text_group;
    prop_name.value = name_text;
    
    math::resource::PropertiesSet prop_set;
    prop_set.lang = m_lang->getLanguage();
    prop_set.properties.push_back(prop_name);
    img->properties.push_back(prop_set);
    return img;
}

std::vector<math::resource::Image*> Math_Quiz::getPeopleNames(int number) {
    auto people_list = math::resource::ResHelper::getImages("game/people.json", "", m_theme->getThemePath());
    return getImageList(people_list, number, "id");
}

std::vector<math::resource::Image*> Math_Quiz::getDatesOfWeek() {
    return math::calendar::Calendar::getDatesOfWeek(7);
}

math::resource::Image* Math_Quiz::getDateOfWeek(int date) {
    if (date > 6) {
        date = 6;
    }
    return math::calendar::Calendar::getDatesOfWeek(date).front();
}

std::vector<math::resource::Image*> Math_Quiz::getMonthsOfYear() {
    return math::calendar::Calendar::getMonthsOfYear(12);
}

math::resource::Image* Math_Quiz::getMonthOfYear(int month) {
    if (month>11) {
        month=11;
    }
    return math::calendar::Calendar::getMonthsOfYear(month).front();
}

std::vector<math::resource::Image*> Math_Quiz::getSeasonsOfYear() {
    return math::calendar::Calendar::getSeasonsOfYear(4);
}

math::resource::Image* Math_Quiz::getSeasonOfYear(int season) {
    if (season>3) {
        season = 3;
    }
    return math::calendar::Calendar::getSeasonsOfYear(season).front();
}


#pragma mark - filter image
std::vector<math::resource::Image*> Math_Quiz::getImageList(std::string json) {
     auto all_images = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%s%s.json", _game_path.c_str(), json.c_str()), "", image_prefix);
    return all_images;
}

std::vector<math::resource::Image*> Math_Quiz::getImageList(std::string json, int number, std::string different, std::string same, cocos2d::Value same_value) {
    
    auto all_images = getImageList(json);
    return getImageList(all_images, number, different, same, same_value);
}

std::vector<math::resource::Image*> Math_Quiz::getImageList(std::vector<math::resource::Image*> images,  int number, std::string different, std::string same, cocos2d::Value same_value) {
    
    auto all_images = math::resource::ResHelper::filterImages(images, same, same_value);
    std::vector<math::resource::Image*> list;
    if (all_images.size() < number) {
        for (int i=0; i<number; i++) {
            list.push_back(images[0]);
        }
        return list;
    }
    
    if (different == same) {
        different = "none";
    }
    
    if (different == "none") {
        auto image = math::func::getRandomNode(all_images);
        for (int i=0; i<number; i++) {
            list.push_back(image);
        }
    } else if (different == "id") {
        math::func::shuffle(all_images);
        for (int i=0; i<number; i++) {
            list.push_back(all_images[i]);
        }
    } else {
        if (different == "name") {
            different = "name_1";
        }
        //check filter is prop
        bool filter_prop = true;
        for (math::resource::Property prop : all_images.front()->comparisions) {
            if (prop.key == different) {
                filter_prop = false;
                break;
            }
        }
        
        if (filter_prop) {
            std::vector<std::string>choose_imgs;
            bool add_img;
            do {
                add_img=true;
                std::string m_kind=math::func::getRandomNode(all_images)->getText(different);
                for (std::string kind: choose_imgs) {
                    if (kind.compare(m_kind)==0) {
                        add_img=false;
                        break;
                    }
                }
                if (add_img) {
                    choose_imgs.push_back(m_kind);
                }
            } while (choose_imgs.size()<number);
            
            for (int i=0; i<number; i++) {
                std::string kind=choose_imgs[i];
                while (true) {
                    auto image=math::func::getRandomNode(all_images);
                    if (image->getText(different) == kind) {
                        list.push_back(image);
                        break;
                    }
                }
            }
        } else {
            std::vector<int>choose_imgs;
            bool add_img;
            do {
                add_img = true;
                int m_kind = math::func::getRandomNode(all_images)->getComparisionValue(different);
                for (int kind: choose_imgs) {
                    if (kind == m_kind) {
                        add_img=false;
                        break;
                    }
                }
                if (add_img) {
                    choose_imgs.push_back(m_kind);
                }
            } while (choose_imgs.size()<number);
            
            for (int i=0; i<number; i++) {
                int kind = choose_imgs[i];
                while (true) {
                    auto image=math::func::getRandomNode(all_images);
                    if (image->getComparisionValue(different) == kind) {
                        list.push_back(image);
                        break;
                    }
                }
            }
        }
    }
    return list;
}

std::vector<math::resource::Image*> Math_Quiz::getImageListNumber(std::string json, std::vector<math::resource::Image*> images, std::vector<int> numbers, std::string prop_key) {
    
    auto all_images = getImageList(json);
    return getImageListNumber(all_images, images, numbers, prop_key);
}

std::vector<math::resource::Image*> Math_Quiz::getImageListNumber(std::vector<math::resource::Image*> all_images, std::vector<math::resource::Image*> images, std::vector<int> number_list, std::string prop_key) {
    
    int number = (int)number_list.size();
    std::vector<math::resource::Image*> results;
    
    if (prop_key == "none" || prop_key == "id" || prop_key == "size" || prop_key == "") {
        for (int i=0; i<number; i++) {
            auto img = images[i];
            for (int i2=0; i2<number_list[i]; i2++) {
                results.push_back(img);
            }
        }
    } else {
        if (prop_key == "name") {
            prop_key = "name_1";
        }
        //check filter is prop
        bool filter_prop = true;
        for (math::resource::Property prop : all_images.front()->comparisions) {
            if (prop.key == prop_key) {
                filter_prop = false;
                break;
            }
        }
        
        if (filter_prop) {
            for (int i=0; i<number; i++) {
                std::vector<math::resource::Image*> same_images;
                std::string key = images[i]->getText(prop_key);

                for (math::resource::Image* img : all_images) {
                    if (img->getText(prop_key) == key && img->code != images[i]->code) {
                        same_images.push_back(img);
                    }
                }
                
                int number_image = number_list[i];
                results.push_back(images[i]);
                
                if (same_images.size() < number_image) {
                    if (same_images.empty()) {
                        for (int i2 = 0; i2 < number_image-1; i2++) {
                            results.push_back(results.back());
                        }
                    } else {
                        for (int i2 = 0; i2 < number_image-1; i2++) {
                            results.push_back(math::func::getRandomNode(same_images));
                        }
                    }
                } else {
                    math::func::shuffle(same_images);
                    for (int i2 = 0; i2 < number_image-1; i2++) {
                        results.push_back(same_images[i2]);
                    }
                }
                
            }
        } else {
            for (int i=0; i<number; i++) {
                std::vector<math::resource::Image*> same_images;
                int key = images[i]->getComparisionValue(prop_key);
                
                for (math::resource::Image* img : all_images) {
                    if (img->getComparisionValue(prop_key) == key && img->code != images[i]->code) {
                        same_images.push_back(img);
                    }
                }
                
                int number_image = number_list[i];
                results.push_back(images[i]);
                
                if (same_images.size() < number_image) {
                    if (same_images.empty()) {
                        for (int i2 = 0; i2 < number_image-1; i2++) {
                            results.push_back(results.back());
                        }
                    } else {
                        for (int i2 = 0; i2 < number_image-1; i2++) {
                            results.push_back(math::func::getRandomNode(same_images));
                        }
                    }
                } else {
                    math::func::shuffle(same_images);
                    for (int i2 = 0; i2 < number_image-1; i2++) {
                        results.push_back(same_images[i2]);
                    }
                }
            }
        }
    }
    
    return results;
}


#pragma mark - Function support
math::comparision::Key Math_Quiz::getComparisionKey (std::string key)
{
    if (key.compare("id")==0) {
        return math::comparision::exactly;
    }
    if (key.compare("name")==0) {
        return math::comparision::name;
    }
    if (key.compare("kind")==0) {
        return math::comparision::kind;
    }
    if (key.compare("family")==0) {
        return math::comparision::family;
    }
    if (key.compare("color")==0) {
        return math::comparision::color;
    }
    if (key.compare("like")==0) {
        return math::comparision::like;
    }
    if (key.compare("size")==0) {
        return math::comparision::size;
    }
    if (key.compare("position")==0) {
        return math::comparision::position;
    }
    if (key.compare("number")==0) {
        return math::comparision::number;
    }
    if (key.compare("height")==0) {
        return math::comparision::height;
    }
    if (key.compare("width")==0) {
        return math::comparision::width;
    }
    if (key.compare("weight")==0) {
        return math::comparision::weight;
    }
    if (key.compare("length")==0) {
        return math::comparision::length;
    }
    if (key.compare("hold")==0) {
        return math::comparision::hold;
    }
    if (key.compare("speed")==0) {
        return math::comparision::speed;
    }
    if (key.compare("time")==0) {
        return math::comparision::time;
    }
    if (key.compare("distance")==0) {
        return math::comparision::distance;
    }
    return math::comparision::exactly;
}

cocos2d::Size Math_Quiz::getAnswerSize(int size) {
    
    switch (m_theme->getTheme()) {
        case math::theme::default_theme:
        {
            switch (size) {
                case 1:
                    return cocos2d::Size(200, 100);
                    break;
                case 2:
                    return cocos2d::Size(225, 100);
                    break;
                default:
                    return cocos2d::Size(100+50*size, 100);
                    break;
            }
        }
            break;
        default:
        {
            
        }
            break;
    }
    
    return cocos2d::Size(200, 100);
}

void Math_Quiz::fixAnswerSize(float max_width, float min_width) {
    if (!_answers.empty()) {
        min_width = min_width < 100 ? 100 : min_width;
        float max_w = 0;
        for (Math_Answer* answer : _answers) {
            float answer_w = answer->getRenderSize();
            if (answer_w > max_w) {
                max_w = answer_w;
            }
        }
        
        if (max_w <= min_width) {
            for (Math_Answer* answer : _answers) {
                answer->setAnswerSize(cocos2d::Size(min_width, answer->getContentSize().height));
            }
        } else {
            if (max_width == 0) {
                for (Math_Answer* answer : _answers) {
                    answer->setAnswerSize(cocos2d::Size(max_w, answer->getContentSize().height));
                }
            } else {
                if (max_width < max_w) {
                    float text_size = _answers.front()->getValueText()->getFontSize();
                    for (Math_Answer* answer : _answers) {
                        answer->getValueText()->setFontSize(text_size-2);
                    }
                    fixAnswerSize(max_width, min_width);
                } else {
                    for (Math_Answer* answer : _answers) {
                        answer->setAnswerSize(cocos2d::Size(max_w, answer->getContentSize().height));
                    }
                }
            }
        }
    }
}

std::vector<int> Math_Quiz::randomArrayIndexes(int number, int size) {
    std::vector<int> datas;
    for (int i=0; i<size; i++) {
        datas.push_back(i);
    }
    math::func::shuffle(datas);
    if (number == size) {
        return datas;
    }
    
    std::vector<int> results;
    for (int i = 0; i<number; i++) {
        results.push_back(datas[i]);
    }
    return results;
}


std::vector<cocos2d::Vec2> Math_Quiz::randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item) {
    std::vector<cocos2d::Vec2> list;
    
    float delta = 1.4;
    int count_rand = 0;
    
    for (int i=0; i<number_item; i++) {
        if (count_rand>=10) {
            count_rand=0;
            delta-=0.1;
        }
        cocos2d::Vec2 point;
        bool avail=true;
        int count=0;
        do {
            count++;
            if (count>50) {
                break;
            }
            
            avail=true;
            int rand_x = cocos2d::random(0, (int)(box_rect.size.width-item_size.width*1.2)) + (int)item_size.width*0.6;
            int rand_y = cocos2d::random(0, (int)(box_rect.size.height-item_size.height*1.2)) + (int)item_size.height*0.6;
            point=cocos2d::Vec2(rand_x+box_rect.origin.x, rand_y+box_rect.origin.y);
            
            for (cocos2d::Vec2 _point : list) {
                if (point.distance(_point)< item_size.width*delta) {
                    avail=false;
                    break;
                }
            }
        } while (!avail);
        
        if (count>50) {
            i=-1;
            list.clear();
            count_rand++;
            continue;
        } else {
            list.push_back(point);
        }
    }
    return list;
}


#pragma mark - Random show/hide
void Math_Quiz::randomShow(std::vector<cocos2d::ui::Layout*> list, float delay, math::animation::ObjectShowType show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_show;
    for (int i=0; i<list_size; i++) {
        random_show.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < list_size; i++) {
        auto name = list[i]->getName();
        if (name == "Math_Object") {
            auto object = static_cast<Math_Object*>(list[i]);
            object->onShow((random_show[i]/number_div)*0.1 + delay, show);
        } else if (name == "Math_Slot") {
            auto slot = static_cast<Math_Slot*>(list[i]);
            slot->onShow((random_show[i]/number_div)*0.1 + delay, show);
        }
    }
}

void Math_Quiz::randomHide(std::vector<cocos2d::ui::Layout*> list, float delay, math::animation::ObjectShowType revert_show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_hide;
    for (int i=0; i<list_size; i++) {
        random_hide.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < list_size; i++) {
        auto name = list[i]->getName();
        if (name == "Math_Object") {
            auto object = static_cast<Math_Object*>(list[i]);
            object->onHide((random_hide[i]/number_div)*0.1 + delay, revert_show);
        } else if (name == "Math_Slot") {
            auto slot = static_cast<Math_Slot*>(list[i]);
            slot->onHide((random_hide[i]/number_div)*0.1 + delay, revert_show);
        }
    }
}

void Math_Quiz::randomShow(std::vector<Math_Object*> list, float delay, math::animation::ObjectShowType show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_show;
    for (int i=0; i<list_size; i++) {
        random_show.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < list_size; i++) {
        list[i]->onShow((random_show[i]/number_div)*0.1 + delay, show);
    }
}

void Math_Quiz::randomHide(std::vector<Math_Object*> list, float delay, math::animation::ObjectShowType revert_show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_hide;
    for (int i=0; i<list_size; i++) {
        random_hide.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < list_size; i++) {
        list[i]->onHide((random_hide[i]/number_div)*0.1 + delay, revert_show);
    }
}

void Math_Quiz::randomShow(std::vector<Math_Slot*> list, float delay, math::animation::ObjectShowType show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_show;
    for (int i=0; i<list_size; i++) {
        random_show.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < list_size; i++) {
        list[i]->onShow((random_show[i]/number_div)*0.1 + delay, show);
    }
}

void Math_Quiz::randomHide(std::vector<Math_Slot*> list, float delay, math::animation::ObjectShowType revert_show) {
    auto list_size = static_cast<int>(list.size());
    std::vector<int> random_hide;
    for (int i=0; i<list_size; i++) {
        random_hide.push_back(i);
    }
    int number_div = (list_size+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < list_size; i++) {
        list[i]->onHide((random_hide[i]/number_div)*0.1 + delay, revert_show);
    }
}

#pragma mark - JSON Helper
int Math_Quiz::randomRangeJson(rapidjson::Value& json, std::string member) {
    return cocos2d::random(json.FindMember(member.c_str())->value.FindMember("minimum")->value.GetInt(), json.FindMember(member.c_str())->value.FindMember("maximum")->value.GetInt());    
}

cocos2d::Size Math_Quiz::getSizeJson(rapidjson::Value& json, std::string member) {
    return cocos2d::Size(json.FindMember(member.c_str())->value.FindMember("width")->value.GetInt(), json.FindMember(member.c_str())->value.FindMember("height")->value.GetInt());
}

rapidjson::Value& Math_Quiz::getJsonMember(rapidjson::Value& parent, std::string child_name) {
    if (parent.HasMember(child_name.c_str())) {
        return parent.FindMember(child_name.c_str())->value;
    }
    return parent;
}

bool Math_Quiz::getJsonBool(rapidjson::Value& parent, std::string child_name, bool _default) {
    if (parent.HasMember(child_name.c_str())) {
        return parent.FindMember(child_name.c_str())->value.GetBool();
    }
    return _default;
}

int Math_Quiz::getJsonInt(rapidjson::Value& parent, std::string child_name, int _default) {
    if (parent.HasMember(child_name.c_str())) {
        return parent.FindMember(child_name.c_str())->value.GetInt();
    }
    return _default;
}

float Math_Quiz::getJsonDouble(rapidjson::Value& parent, std::string child_name, float _default) {
    if (parent.HasMember(child_name.c_str())) {
        return parent.FindMember(child_name.c_str())->value.GetDouble();
    }
    return _default;
}

std::string Math_Quiz::getJsonString(rapidjson::Value& parent, std::string child_name, std::string _default) {
    if (parent.HasMember(child_name.c_str())) {
        return parent.FindMember(child_name.c_str())->value.GetString();
    }
    return _default;
}

#pragma mark - New theme
std::string Math_Quiz::getUIPath(std::string path) {
    return cocos2d::StringUtils::format("%s%s", m_theme->getThemePath().c_str(), path.c_str());
}

#pragma mark - Trial Manager
void Math_Quiz::setStopTrialCallback(std::function <void ()> function, bool completed_stop) {
    _stop_trial_callback = function;
    stop_trial_when_completed = completed_stop;
}

bool Math_Quiz::checkStopTrialWhenComplete () {
    if (_stop_trial_callback == nullptr) {
        return true;
    }
    
    if (stop_trial_when_completed) {
        _stop_trial_callback();
        return false;
    }
    return true;
}

bool Math_Quiz::checkStopTrial () {
    if (_stop_trial_callback == nullptr) {
        return true;
    }
    if (!stop_trial_when_completed) {
        _stop_trial_callback();
        return false;
    }
    return true;
}





