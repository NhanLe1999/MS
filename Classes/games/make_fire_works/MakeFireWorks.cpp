#include "MakeFireWorks.h"
#include "AspectR.h"
#include <audio/include/AudioEngine.h>
#include "HSAudioEngine.h"

#include "StoryConfigManager.h"

USING_NS_CC;

using namespace makefireworks;
using namespace rapidjson;

GamePlay* GamePlay::createGame(std::vector<math::resource::Image*> images)
{
    GamePlay* obj = new GamePlay();
    if (obj->init())
    {
        obj->autorelease();
        obj->initOptions(images);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}


void GamePlay::initOptions(std::vector<math::resource::Image*> images)
{
    json_file = "config.json";

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(Size(visibleSize.width, visibleSize.height));

    //assign
    _allLevels = loadLevelData(images);

    //background///////////
    auto bgSprite = Sprite::create("games/make_fireworks/bg.png");
    bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgSprite->setPosition(AspectR::getMidX() - bgSprite->getContentSize().width, AspectR::getMidY());
    //this->addChild(bgSprite);
    auto bgSprite2 = Sprite::create("games/make_fireworks/bg.png");
    bgSprite2->setFlippedX(true);
    bgSprite2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgSprite2->setPosition(bgSprite2->getContentSize().width * 1.5f - 2, AspectR::getMidY());
    bgSprite->addChild(bgSprite2);
    // set touch for bg
    auto bgLayout = Layout::create();
    bgLayout->addChild(bgSprite);
    bgLayout->setContentSize(Size(visibleSize.width, visibleSize.height));
    bgLayout->setTouchEnabled(true);
    bgLayout->addClickEventListener([=](Ref* sender) {
        CCLOG("clicked background");
        this->sheduleSoundGuiding();
        });
    this->addChild(bgLayout);


    ///////////////////////

    // music background
    AUDIO_ENGINE->playBackgroundMusic("sounds/make_fireworks/ui/bg.mp3", 0.8f);


    //actor load
    const string path = "gaf/make_fireworks/silly_chemist_animation/Silly_Chemist_animation.gaf";
    GAFAsset* asset = GAFAsset::create(path);
    _actor = asset->createObject();
    _actor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _actor->setPosition(Vec2(AspectR::getMidX() - AspectR::getWidth(512), AspectR::getMidY()));
    _actor->setSequenceDelegate(CC_CALLBACK_2(GamePlay::onFinishSequence, this));
    this->addChild(_actor);

    //start actor idle animation
    _actor->start();
    _actor->playSequence("idle", true);

    //move anim at start up
    bgSprite->runAction(MoveTo::create(0.5, Vec2(AspectR::getMidX() - bgSprite->getContentSize().width / 2, AspectR::getMidY())));
    _actor->runAction(MoveTo::create(0.5, Vec2(AspectR::getMidX(), AspectR::getMidY())));

    //top white question panel
    _question = ImageView::create("games/make_fireworks/qpanel.png");
    _question->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _question->setPosition(Vec2(AspectR::getMidX(), AspectR::getHeight(1500)));
    _question->setCascadeOpacityEnabled(true);
    _question->setTouchEnabled(true);
    _question->addClickEventListener([=](Ref* sender) { 
        if (!canTouch)
            return;
        AUDIO_ENGINE->stopAllSound();
        AUDIO_ENGINE->playSound(_allLevels[_currentIndex].quesAudio, false, ([=] {
            sheduleSoundGuiding();
            }));
        });
    this->addChild(_question);

    auto questionCaption = Label::createWithTTF("An elephant is bigger than a mouse.", "fonts/Linotte Regular.ttf", AspectR::getFontSize(33));
    questionCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    questionCaption->setTextColor(Color4B::BLACK);
    questionCaption->setName("caption");
    questionCaption->setPosition(Vec2(_question->getContentSize().width / 2, AspectR::getHeight(20)));
    questionCaption->setDimensions(_question->getContentSize().width - AspectR::getWidth(40), AspectR::getHeight(150));
    questionCaption->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _question->addChild(questionCaption);
    //----------------------------

    //Left answer
    _leftAnswer = Layout::create();
    _leftAnswer->setContentSize(Size(200, AspectR::getHeight(200)));
    _leftAnswer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _leftAnswer->setPosition(Vec2((visibleSize.width / 2) - 260, AspectR::getHeight(165)));
    _leftAnswer->setScale(0);
    _leftAnswer->addTouchEventListener(CC_CALLBACK_2(GamePlay::onAnswerClick, this));
    _leftAnswer->setTouchEnabled(true);
    _leftAnswer->setName("correct_left");//important for anim
    _leftAnswer->setCascadeOpacityEnabled(true);
    this->addChild(_leftAnswer);

    auto leftCaption = Label::createWithTTF(_allLevels[_currentIndex].left, "fonts/Linotte Bold.ttf", AspectR::getFontSize(36));
    leftCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftCaption->setTextColor(Color4B::WHITE);
    leftCaption->setName("caption");
    leftCaption->setPosition(Vec2(_leftAnswer->getContentSize().width / 2, _leftAnswer->getContentSize().height / 2));
    _leftAnswer->addChild(leftCaption);
    //----------------------------

    //right answer
    _rightAnswer = Layout::create();
    _rightAnswer->setContentSize(Size(200, AspectR::getHeight(200)));
    _rightAnswer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _rightAnswer->setPosition(Vec2((visibleSize.width / 2) + 262, AspectR::getHeight(165)));
    _rightAnswer->setScale(0);
    _rightAnswer->setTouchEnabled(true);
    _rightAnswer->addTouchEventListener(CC_CALLBACK_2(GamePlay::onAnswerClick, this));
    _rightAnswer->setName("correct_right");//important for anim
    _rightAnswer->setCascadeOpacityEnabled(true);
    this->addChild(_rightAnswer);

    auto rightCaption = Label::createWithTTF(_allLevels[_currentIndex].right, "fonts/Linotte Bold.ttf", AspectR::getFontSize(36));
    rightCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightCaption->setTextColor(Color4B::WHITE);
    rightCaption->setName("caption");
    rightCaption->setPosition(Vec2(_rightAnswer->getContentSize().width / 2, _rightAnswer->getContentSize().height / 2));
    _rightAnswer->addChild(rightCaption);
    //----------------------------

    // create speak
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    _speaker->setTouchEnabled(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender)
        {
            AUDIO_ENGINE->stopAllSound();        
            AUDIO_ENGINE->playSound(_allLevels[_currentIndex].quesAudio, false, ([=] {
                sheduleSoundGuiding();
             }));
        });
    this->addChild(_speaker);


}

//void GamePlay::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::Director::getInstance()->getWinSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

vector<LevelData> GamePlay::loadLevelData(std::vector<math::resource::Image*> images)
{
    Document document;
    std::string str = FileUtils::getInstance()->getStringFromFile(json_file);
    document.Parse(str.c_str());

    vector<LevelData> totalLevels;

    //overall array
    auto levelArray = document["data"].GetArray();
    for (auto i = 0; i < levelArray.Size(); i++) {
        auto level = LevelData();
        auto info = math::resource::ResHelper::getImage(images, levelArray[i]["question_info"].GetString());
        level.ques = info->getText();
        level.quesAudio = info->getAudio();
        level.left = levelArray[i]["left_ans"].GetString();
        level.right = levelArray[i]["right_ans"].GetString();
        level.correct = levelArray[i]["true_ans"].GetString();
        totalLevels.push_back(level);
    }
    //mj::helper::shuffle(totalLevels);
    return totalLevels;
}

void GamePlay::showQuestion() {
    _question->setOpacity(255);
    _question->setPositionY(AspectR::getHeight(1500));
    _question->setTouchEnabled(false);
    //set Question text
    ((Label*)_question->getChildByName("caption"))->setString(_allLevels[_currentIndex].ques);

    _question->runAction(Sequence::create(MoveTo::create(1, Vec2(AspectR::getMidX(), AspectR::getHeight(810))), nullptr));
}

void GamePlay::onAnswerClick(Ref* sender, ui::Widget::TouchEventType event) {
    if (!canTouch) return;
    if (event == Widget::TouchEventType::ENDED) {
        auto layout = (Layout*)sender;
        auto direction = layout->getName(); //important for anim
        auto caption = ((Label*)layout->getChildByName("caption"));
        string answer = caption->getString();
        //button press animation
        caption->setColor(Color3B::RED);
        layout->setScale(0);
        layout->runAction(Sequence::create(EaseBounceInOut::create(ScaleTo::create(0.5, 1)), CallFunc::create([=]() {
            caption->setColor(Color3B::WHITE);
            }), nullptr));

        AUDIO_ENGINE->stopAllSound();
        if (answer == _allLevels[_currentIndex].correct) {
            this->unschedule("soundGuiding");
            _speaker->setTouchEnabled(false);
            _question->runAction(FadeOut::create(0.3));
            _leftAnswer->runAction(FadeOut::create(0.3));
            _rightAnswer->runAction(FadeOut::create(0.3));
            _actor->playSequence(direction, false);
            cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/correct_new.mp3");
        }
        else {
            _actor->playSequence("wrong", false);
            cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/ui/wrong.mp3");
            this->sheduleSoundGuiding();
        }

        canTouch = false;

    }
}

#include "StoryPageVC.h"
void GamePlay::onFinishSequence(GAFObject* object, const std::string& sequenceName) {
    if (sequenceName == "wrong") {
        _actor->playSequence("idle", true);
        canTouch = true;
    }
    else if (sequenceName == "correct_left" || sequenceName == "correct_right") {

        //update to next level
        _currentIndex++;

        if (_currentIndex >= (int)_allLevels.size()) {
            //<<<<GAME IS OVER>>>>
            _actor->setVisible(false);
            this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
                _actor->removeFromParent();
                //_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
                if (CONFIG_MANAGER->isVietnameseStories()) {
                    this->onCloseGameVmonkey();
                }
                else {
                    commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/make_fireworks/Bachoc_standalone/Bachoc_standalone.gaf", true);
                }
                }), NULL));
        }
        else {
            _actor->playSequence("idle", true);

            showNextLevel();
        }
    }
}

void GamePlay::showNextLevel() {
    //show top question panel
    showQuestion();

    //reset left panel
    ((Label*)_leftAnswer->getChildByName("caption"))->setString(_allLevels[_currentIndex].left);

    //reset right panel
    ((Label*)_rightAnswer->getChildByName("caption"))->setString(_allLevels[_currentIndex].right);

    this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=]() {
        //play question sound
        if (!game_play_enable && _currentIndex >= 1) {
            this->pauseGame();
            return;
        }

        if (audio_question_id != 0) {
            AUDIO_ENGINE->stopEffect(audio_question_id);
        }

        _leftAnswer->setTouchEnabled(false);
        _rightAnswer->setTouchEnabled(false);
        if (_speaker) _speaker->setTouchEnabled(false);
        this->unschedule("soundGuiding");

        if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
            audio_question_id = AUDIO_ENGINE->playSound(_allLevels[_currentIndex].quesAudio, false, [=]() {
                AUDIO_ENGINE->_listIDAudio.push_back(audio_question_id);
                _question->setTouchEnabled(true);
                AUDIO_ENGINE->playSound("sounds/make_fireworks/dung.mp3", false, [=]() {
                    _leftAnswer->setTouchEnabled(true);
                    AUDIO_ENGINE->playSound("sounds/make_fireworks/sai.mp3", false, [=]() {
                        _rightAnswer->setTouchEnabled(true);
                        _speaker->setTouchEnabled(true);
                        });
                    _rightAnswer->runAction(FadeIn::create(0.25));
                    });
                _leftAnswer->runAction(FadeIn::create(0.25));
                this->sheduleSoundGuiding();

                });
        }
        else
        {
            audio_question_id = AUDIO_ENGINE->playSound(_allLevels[_currentIndex].quesAudio, false, [=]() {
                if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/make_fireworks/Is it true or false.mp3")) {
                    AUDIO_ENGINE->playSound("sounds/make_fireworks/Is it true or false.mp3", false, ([=] {
                        _speaker->setTouchEnabled(true);
                        _question->setTouchEnabled(true);
                        _leftAnswer->setTouchEnabled(true);
                        _rightAnswer->setTouchEnabled(true);
                        _leftAnswer->runAction(FadeIn::create(0.25));
                        _rightAnswer->runAction(FadeIn::create(0.25));
                        this->sheduleSoundGuiding();
                        }));
                    }
                });
        }

        auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_question_id);
        auto layoutSequence = Sequence::create(DelayTime::create(duration), CallFunc::create([=]() {
            _leftAnswer->runAction(FadeTo::create(0.3, 150));
            _rightAnswer->runAction(FadeTo::create(0.3, 150));
            canTouch = true;
            }), nullptr);

        this->runAction(layoutSequence);
        }), nullptr));
}

void GamePlay::sheduleSoundGuiding()
{
    CCLOG("schedule sound guild");
    this->unschedule("soundGuiding");
    this->schedule([=](float)
        {
            if (cocos2d::experimental::AudioEngine::getPlayingAudioCount() == 1)
            {
                if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/make_fireworks/Is it true or false.mp3")) {
                    int sound_TF_id = AUDIO_ENGINE->playSound("sounds/make_fireworks/Is it true or false.mp3", false, ([=] {
                        sheduleSoundGuiding();
                    }));
                }
            }
        }, 10, 100, 10, "soundGuiding");
}

#include "StoryConfigManager.h"

void GamePlay::onEnter()
{
    ms::BaseActivity::onEnter();
    if (MJDEFAULT->getBoolForKey(key_background_music)) {
        //cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/ui/bg.mp3", true, 1);
        AUDIO_ENGINE->playBackgroundMusic("sounds/make_fireworks/ui/bg.mp3");
    }

    //left, right answer and question panel anim
    auto layoutSequence = Sequence::create(DelayTime::create(8.5f), CallFunc::create([=]() {
        _leftAnswer->runAction(EaseBounceInOut::create(ScaleTo::create(1, 1)));
        AUDIO_ENGINE->playEffect("sounds/make_fireworks/ui/bounce.mp3");
        }), DelayTime::create(1), CallFunc::create([=]() {
            _rightAnswer->runAction(EaseBounceInOut::create(ScaleTo::create(1, 1)));
            AUDIO_ENGINE->playEffect("sounds/make_fireworks/ui/bounce.mp3");
            }), DelayTime::create(3.2), CallFunc::create([=]() {
                //showNextLevel();
                }), nullptr);

    this->runAction(layoutSequence);

    if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
        auto audio_intro = cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/vi_intro/intro_vi.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_intro, [=](int au_id, std::string path) {
            this->scheduleOnce([=](float dt) {
                showNextLevel();
                }, 0.3f, "next_level");
            });
    }
    else {
        //Play intro instruction of gameplay
        auto blackLayout = LayerColor::create(Color4B(0, 0, 0, 200));
        this->addChild(blackLayout, 10000);

        auto imgGameName = cocos2d::ui::ImageView::create("games/make_fireworks/makefirework_title.png");
        blackLayout->addChild(imgGameName, 10000);
        imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);

        auto audio_pre = cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_pre, [=](int au_id, std::string path) {
            imgGameName->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.7f)
                , CallFunc::create([=]() {

                    }), RemoveSelf::create(true), nullptr));
            blackLayout->runAction(Sequence::createWithTwoActions(DelayTime::create(1.2f), RemoveSelf::create(true)));
            auto soundSequence = Sequence::create(DelayTime::create(1.2f), CallFunc::create([=]() {
                auto audio_intro = cocos2d::experimental::AudioEngine::play2d("sounds/make_fireworks/intro/intro.mp3");
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_intro, [=](int au_id, std::string path) {
                    this->scheduleOnce([=](float dt) {
                        showNextLevel();
                        }, 0.3f, "next_level");

                    });
                //CCLOG("doc_intro");
                }), nullptr);
            this->runAction(soundSequence);
            });
    }

}
