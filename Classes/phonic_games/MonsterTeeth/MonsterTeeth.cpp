//
//  MonsterTeeth.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/10/18.
//

#include "MonsterTeeth.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
USING_NS_CC;
INITIALIZE_READER(MonsterTeeth);
#define speed_opacity 15

std::once_flag MonsterTeeth_reader;
#define CSB_NAME "csb/game/MonsterTeeth/MonsterTeeth.csb"
#define _game_path "json/monsterteeth/"
//#define _audio_path "sample_game/monsterteeth/"

MonsterTeeth* MonsterTeeth::createGame(std::string json_file, bool isAutoPlay) {
    std::call_once(MonsterTeeth_reader, [] {
        REGISTER_CSB_READER(MonsterTeeth);
        REGISTER_CSB_READER(MonsterTeeth_Item);
        });

    auto p = static_cast<MonsterTeeth*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MonsterTeeth::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    _gamemode = document.FindMember("game_mode")->value.GetInt();
    rapidjson::Value& data = document.FindMember("data")->value;

    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    for (int i = 0; i < data.Size(); i++) {

        rapidjson::Value& json_item = data[i].FindMember("game")->value;

        auto img_id = json_item.FindMember("text")->value.GetString();
        auto p_img = math::resource::ResHelper::getImage(images, img_id);
        mj::PHONIC_WORD item;
        item.text = p_img->getText("name_1");
        item.audio_path = p_img->getAudios("name_1").front();
        words.push_back(item);
    }

    screen_size = Director::getInstance()->getVisibleSize();
    _isAutoPlay = isAutoPlay;

    loadFrame();
}

bool MonsterTeeth::isPhone() {
    if (Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_IPHONE) {
        return true;
    }
    else return false;
}

//game config
void MonsterTeeth::gameConfig() {
    if (isPhone()) {
        _circle_pos = Vec2((bg_size.width + screen_size.width) * 0.48, screen_size.height * 0.35);
        _table_pos = Vec2(bg_size.width / 2 + screen_size.width * 0.95, screen_size.height * 0.5);
        _khay_pos = Vec2(bg_size.width / 2 + screen_size.width * 0.95, screen_size.height * 0.6);
        _tool_rotate = 90;
        _tool_scale = 0.8;
        _table_rotate = -90;
        _monster_scale = 1.2;

    }
    else {
        _circle_pos = Vec2((bg_size.width + screen_size.width) * 0.5, screen_size.height * 0.45);
        _table_pos = Vec2(bg_size.width / 2, screen_size.height * 0.15);
        _khay_pos = Vec2((bg_size.width + screen_size.width) * 0.5, screen_size.height * 0.12);
    }
}

void MonsterTeeth::loadFrame() {
    if (CONFIG_MANAGER->isGameVMonkey()) {
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    _bg = utils::findChild<ImageView*>(root_layout, "background");
    bg_size = _bg->getContentSize();

    gameConfig();
    //set positions
    utils::findChild(root_layout, "circle")->setPosition(_circle_pos);
    utils::findChild(root_layout, "table")->setPosition(_table_pos);
    utils::findChild(root_layout, "table")->setRotation(_table_rotate);
    utils::findChild(root_layout, "khay")->setPosition(_khay_pos);
    utils::findChild(root_layout, "khay")->setRotation(_tool_rotate);
    utils::findChild(root_layout, "khay")->setScale(_tool_scale);

    auto monster_pos = Vec2(_circle_pos.x, _circle_pos.y + 100);
    //create monster
    GAFWrapper* monster = GAFWrapper::createGAF("gaf/monsterteeth/monster/monster.gaf");
    _bg->addChild(monster);
    monster->setPosition(monster_pos);
    monster->play(true);
    monster->setName("monster");
    monster->setScale(_monster_scale);

    //load mouth
    if (_gamemode == 0) {
        _mouth = utils::findChild<ImageView*>(root_layout, "mouth_1");
    }
    else {
        _mouth = utils::findChild<ImageView*>(root_layout, "mouth_2");
    }
    _mouth->retain();
    _mouth->removeFromParentAndCleanup(false);
    monster->addChild(_mouth);
    _mouth->setPosition(Vec2(monster->getContentSize().width / 2, monster->getContentSize().height * 0.54));
    _mouth->release();

    //brush
    _brush = utils::findChild<ImageView*>(root_layout, "brush");
    //tool pos
    Size khay_sz = utils::findChild(root_layout, "khay")->getContentSize();
    //    float _tool_pos_y = _khay_pos.y + bg_size.height*0.5;
    if (isPhone()) {
        _brush_pos = Vec2(_khay_pos.x - khay_sz.width * _tool_scale * 0.05, _khay_pos.y + _brush->getContentSize().width * 0.4);
        _water_pos = Vec2(_khay_pos.x - khay_sz.width * _tool_scale * 0.05, _khay_pos.y - _brush->getContentSize().width * 0.4);
    }
    else {
        _brush_pos = Vec2(_khay_pos.x - khay_sz.width / 5, _khay_pos.y + _brush->getContentSize().height / 2);
        _water_pos = Vec2(_khay_pos.x + khay_sz.width / 5, _khay_pos.y + _brush->getContentSize().height / 2);
    }
    _brush->addTouchEventListener(CC_CALLBACK_2(MonsterTeeth::onTouchedItem, this));
    _brush->setPosition(_brush_pos);
    _brush->setRotation(_tool_rotate);
    _brush->setScale(_tool_scale);
    _brush->setTouchEnabled(false);

    //create bubble for brush
    GAFWrapper* bubble = GAFWrapper::createGAF("gaf/monsterteeth/bubble/bubble.gaf");
    bubble->setRotation(90);
    _brush->addChild(bubble, -1);
    bubble->setPosition(utils::findChild(_brush, "gaf_pos")->getPosition());
    bubble->setName("gaf");
    bubble->setVisible(false);
    utils::findChild(_brush, "gaf_pos")->setLocalZOrder(-1);
    _brush->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, _tool_scale + 0.05), ScaleTo::create(1.0f, _tool_scale))));

    //create water for spray
    _water = utils::findChild<ImageView*>(root_layout, "water");
    _water->addTouchEventListener(CC_CALLBACK_2(MonsterTeeth::onTouchedItem, this));
    _water->setPosition(_water_pos);
    _water->setRotation(_tool_rotate);
    _water->setScale(_tool_scale);
    _water->setTouchEnabled(false);

    GAFWrapper* water_gaf = GAFWrapper::createGAF("gaf/monsterteeth/water/water.gaf");
    _water->addChild(water_gaf, -1);
    water_gaf->setPosition(utils::findChild(_water, "gaf_pos")->getPosition());
    water_gaf->setName("gaf");
    water_gaf->setVisible(false);
    _water->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, _tool_scale + 0.05), ScaleTo::create(1.0f, _tool_scale))));

    //set positions
    utils::findChild(this, "khay")->setLocalZOrder(10);
    _brush->setLocalZOrder(12);
    _water->setLocalZOrder(12);

    origin_pos[0] = _brush->getPosition();
    origin_pos[1] = _water->getPosition();
    showGame();
}

void MonsterTeeth::showGame() {

    //reset
    word = words[0];
    words.erase(words.begin());
    tooth_item.clear();
    number_clean = 0;
    count = 0;

    //create item
    int mouth_sz = (int)_mouth->getChildren().size();
    for (int i = 0; i < mouth_sz; i++) {

        auto teeth = _mouth->getChildByName("tooth_" + cocos2d::StringUtils::toString(i));
        Vec2 teeth_pos = teeth->getPosition();

        int rand_dirty_img = random(1, 4);

        MonsterTeeth_Item* item = MonsterTeeth_Item::createItem(word.text, rand_dirty_img);
        item->setClean(false);
        item->setAnchorPoint(Point(0.5, 0.5));
        item->setScale(1.1);
        _mouth->addChild(item);
        item->setPosition(teeth_pos);
        item->setName("item");

        tooth_item.push_back(item);
    }

    //show game
    this->runAction(Sequence::create(CallFunc::create([=] {
        auto show_up = AUDIO_ENGINE->play("sounds/monsterteeth/show_up.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(show_up, [=](int id, std::string path) {
            if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
            {
                AUDIO_ENGINE->play("sounds/monsterteeth/question_vn.mp3");
            }
            else {
                AUDIO_ENGINE->play("sounds/monsterteeth/question.mp3");
            }

            });
        _bg->runAction(MoveBy::create(1.0f, Vec2(-screen_size.width, 0)));
        }), DelayTime::create(1.0), CallFunc::create([=] {
            AUDIO_ENGINE->play("sounds/monsterteeth/text_showed.mp3");
            for (auto child : tooth_item) {
                utils::findChild(child, "text")->runAction(FadeIn::create(1.5f));
            }
            if (count_game != 0) {
                _brush->setTouchEnabled(true);
                _water->setTouchEnabled(true);
            }
            }), NULL));
}

void MonsterTeeth::onEnter() {

    MJMainGBase::onEnter();

    playBackgroundMusic("sounds/monsterteeth/music.mp3");

    this->setContentSize(screen_size);

    this->runAction(Sequence::create(DelayTime::create(2.5), CallFunc::create([=] {
        this->showGuideGame();
        inActiveMode(12.0f);
        }), NULL));

    _onNextStageCallback = [=]() {

        onNextStage();

    };
}

void MonsterTeeth::onExit() {

    MJMainGBase::onExit();

    _onNextStageCallback = nullptr;

}

void MonsterTeeth::onTouchedItem(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    Layout* tool = (Layout*)sender;

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        activeMode();
        //CCLOG("began");
        AUDIO_ENGINE->play("sounds/monsterteeth/choose.mp3");
        tool->stopAllActions();
        tool->runAction(Spawn::create(RotateTo::create(0.2f, 45, 45), ScaleTo::create(0.2f, _tool_scale + 0.2), NULL));
        tool->setLocalZOrder(100);

        tool_gaf = utils::findChild<GAFWrapper*>(tool, "gaf");

        if (tool->getName() == "brush") {
            tool_index = 0;
        }
        else {
            tool_index = 1;
        }

        tool_sound = "sounds/monsterteeth/" + tool->getName() + ".mp3";
        last_pos = tool->getPosition();
        break;

    case cocos2d::ui::Widget::TouchEventType::MOVED:
        //            CCLOG("moved");
        unschedule("stop_tool_sound");

        tool->setPosition(_bg->convertToNodeSpace(((Layout*)sender)->getTouchMovePosition()));
        current_pos = tool->getPosition();

        for (int i = 0; i < tooth_item.size(); i++) {
            //neu cham vao rang
            if (utils::findChild(tool, "gaf_pos")->getBoundingBox().containsPoint(tool->convertToNodeSpace(tooth_item[i]->getParent()->convertToWorldSpace(tooth_item[i]->getPosition())))) {

                //                     AUDIO_ENGINE->playSound(tool_sound, true);

                AUDIO_ENGINE->playSound(tool_sound, true);


                CCLOG("tool sound id: %i", tool_sound_id);
                if (tool_index == 0 && current_pos.distance(last_pos) < 10) {
                    //ban chai
                    tool_sound_isOn = false;
                }
                else {
                    //voi nuoc
                    tool_sound_isOn = true;
                    last_pos = current_pos;
                }

                tool_gaf->setVisible(true);
                tool_gaf->play(true);

                if (tooth_item[i]->getClean() == false && tool_sound_isOn) {
                    if (tooth_item[i]->getOpacity() > 80) {
                        tooth_item[i]->runAction(FadeTo::create(0.2f, tooth_item[i]->getOpacity() - delta_opacity));
                    }
                    else {
                        tool_sound_isOn = false;
                        tooth_item[i]->setVisible(false);
                        playEffect(_mouth, tooth_item[i]->getPosition(), i);
                        tooth_item[i]->setClean(true);
                        number_clean++;
                    }
                }
                break;
            }
            else {
                //k cham vao rang
                tool_sound_isOn = false;
                tool_gaf->setVisible(false);
            }
        }

        //stop tool sound effect
        if (tool_index == 1) {
            if (!tool_sound_isOn) {
                //                    AUDIO_ENGINE->stopSound(tool_sound);
                AUDIO_ENGINE->stopSound(tool_sound);

            }
        }
        else {
            scheduleOnce([=](float) {
                //                        AUDIO_ENGINE->stopSound(tool_sound);
                AUDIO_ENGINE->stopSound(tool_sound);
                }, 0.15, "stop_tool_sound");
        }

        //check next stage
        if (number_clean == tooth_item.size()) {
            tool->setTouchEnabled(false);
            onTouchedItem(tool, ui::Widget::TouchEventType::ENDED);
        }

        break;

    default:
        //            AUDIO_ENGINE->stopSound(tool_sound);

        AUDIO_ENGINE->stopSound(tool_sound);

        tool->runAction(Spawn::create(ScaleTo::create(0.2f, _tool_scale), RotateTo::create(0.2f, _tool_rotate, _tool_rotate), EaseBackOut::create(MoveTo::create(0.3f, origin_pos[tool_index])), NULL));
        tool->setLocalZOrder(12);
        tool->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, _tool_scale + 0.05), ScaleTo::create(1.0f, _tool_scale))));
        tool_gaf->setVisible(false);
        tool_gaf->pauseAnimation();

        if (number_clean == tooth_item.size()) {

            this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {

                if (_onNextStageCallback) {

                    _onNextStageCallback();
                }

                }), NULL));
        }
        else {
            inActiveMode(10.0f);
        }
        break;
    }
}

void MonsterTeeth::update(float dt) {
    last_pos = current_pos;
}

void MonsterTeeth::playEffect(Node* holder, Vec2 pos, int i) {

    std::string sound;
    Color4B color;
    Color4B outline_color = Color4B::WHITE;
    int scale = 2;
    auto i_text = word.text;
    if (i == -1) {
        sound = "sounds/monsterteeth/win.mp3";
        color = Color4B::BLACK;
        scale = 4;
    }
    else {
        sound = "sounds/monsterteeth/correct.mp3";
        color = utils::findChild<Text*>(tooth_item[i], "text")->getTextColor();
        if (color == Color4B(253, 253, 253, 255)) {
            outline_color = Color4B::GRAY;
        };
        i_text = tooth_item[i]->getWord();
    }
    int ting_id = cocos2d::experimental::AudioEngine::play2d(sound);
    cocos2d::experimental::AudioEngine::setVolume(ting_id, 0.3);


    Text* text = Text::create(i_text, font_name, font_sz);
    text->setTextColor(color);
    text->enableOutline(outline_color, 4);
    text->setOpacity(0);
    text->setScale(0);
    holder->addChild(text, 100);
    text->setPosition(pos);
    text->setGlobalZOrder(100);
    text->runAction(Spawn::create(FadeIn::create(0.5), ScaleTo::create(0.5, scale), CallFunc::create([=] {
        int word_id = AUDIO_ENGINE->play(word.audio_path);
        if (i != -1) {
            cocos2d::experimental::AudioEngine::setFinishCallback(word_id, [=](int id, string path) {
                AUDIO_ENGINE->play("sounds/monsterteeth/monster.mp3");
                });
        }
        }), NULL));

    GAFWrapper* phao_hoa = GAFWrapper::createGAF("gaf/monsterteeth/effec/effec.gaf");
    holder->addChild(phao_hoa, 1);
    phao_hoa->setPosition(pos);
    phao_hoa->play(false, [=](gaf::GAFObject* obj) {
        text->removeFromParent();
        phao_hoa->removeFromParent();
        });

    //monster effect
    utils::findChild<GAFWrapper*>(this, "monster")->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.2f, Vec2(0, 10))), EaseBackOut::create(MoveBy::create(0.2f, Vec2(0, -10))), NULL));
}

void MonsterTeeth::onNextStage() {
    activeMode();
    for (auto item : tooth_item) {
        item->removeFromParent();
    }
    tooth_item.clear();
    
    _brush->setTouchEnabled(false);
    _water->setTouchEnabled(false);

    Vec2 center_pos = _mouth->getParent()->convertToWorldSpace(_mouth->getPosition());
    playEffect(root_layout, center_pos);

    if (count_game == 0) {
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
            _bg->runAction(MoveBy::create(1.0f, Vec2(screen_size.width, 0)));
            }), DelayTime::create(1.5), CallFunc::create([=] {
                count_game++;
                onNextGame();
                }), NULL));
    }
    else {
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
            onFinishGame();
            }), NULL));
    }
}

void MonsterTeeth::onNextGame() {
    if (words.size() == 0) {
        this->onFinishGame();
    }
    else {
        showGame();
        if (!_isAutoPlay) {
            inActiveMode(10.0f);
        }
    }
}

void MonsterTeeth::onFinishGame() {
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGame();
}

void MonsterTeeth::showGuideGame(std::string gaf_guide_path) {

    auto hand = GAFWrapper::createGAF(gaf_guide_path);
    hand->setRotation(-90);
    _brush->addChild(hand, 999999);
    hand->setPosition(Vec2(_brush->getContentSize().width * 0.7, _brush->getContentSize().height * 0.2));

    auto old_pos = _brush->getPosition();
    auto tooth_pos = _bg->convertToNodeSpace(tooth_item[0]->getParent()->getPosition());
    auto delta = (_brush->getContentSize().width * 0.4) / sqrt(2);
    auto brush_pos = Vec2(tooth_pos.x + delta, tooth_pos.y - delta);

    _brush->setTouchEnabled(false);
    _water->setTouchEnabled(false);

    //guide for item
    _brush->runAction(Sequence::create(RotateTo::create(0.1f, 45, 45), CallFunc::create([=] {
        hand->runAction(RotateTo::create(0.1f, -45, -45));
        utils::findChild<GAFWrapper*>(_brush, "gaf")->setVisible(true);
        utils::findChild<GAFWrapper*>(_brush, "gaf")->play(true);
        }), MoveTo::create(0.5f, brush_pos), Repeat::create(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(brush_pos.x + 30, brush_pos.y + 30)), MoveTo::create(0.5f, Vec2(brush_pos.x - 30, brush_pos.y - 30))), 2), CallFunc::create([=] {
            utils::findChild<GAFWrapper*>(_brush, "gaf")->pauseAnimation();
            utils::findChild<GAFWrapper*>(_brush, "gaf")->setVisible(false);
            _brush->runAction(EaseBackOut::create(MoveTo::create(0.5, old_pos)));
            _brush->runAction(RotateTo::create(0.1, _tool_rotate, _tool_rotate));
            hand->removeFromParent();
            _brush->setTouchEnabled(true);
            _water->setTouchEnabled(true);
            }), NULL));
}

void MonsterTeeth::inActiveMode(float dt)
{
    this->schedule([=](float) {
        if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
        {
            AUDIO_ENGINE->playSound("sounds/monsterteeth/question_vn.mp3");
        }
        else {
            AUDIO_ENGINE->playSound("sounds/monsterteeth/question.mp3");
        }
        showGuideGame();
        }, dt, "inActiveMode");
}

void MonsterTeeth::activeMode()
{
    this->unschedule("inActiveMode");
    if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
    {
        AUDIO_ENGINE->stopSound("sounds/monsterteeth/question_vn.mp3");
    }
    else {
        AUDIO_ENGINE->stopSound("sounds/monsterteeth/question.mp3");
    }

}


