
#include "GameJumpingMonkey_Food_VMonkey.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameJumpingMonkeyVMonkey_Food);

USING_NS_CC;
#define CSB_NAME "csb/game/jumping_monkey/GameJumpingMonkey_Food"

MJGameJumpingMonkeyVMonkey_Food * MJGameJumpingMonkeyVMonkey_Food::createGame(int type) {
    auto p = static_cast<MJGameJumpingMonkeyVMonkey_Food *>(cocos2d::CSLoader::createNodeWithVisibleSize(StringUtils::format("%s.csb", CSB_NAME)));
    if (p) {
        p->didLoadFromCSB(type);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingMonkeyVMonkey_Food::didLoadFromCSB(int type) {
    _type = type;
}

void MJGameJumpingMonkeyVMonkey_Food::onEnter(){
    cocos2d::ui::Layout::onEnter();
    auto root = this->getChildByName(StringUtils::format("root%d", _type));
    root->setVisible(true);
    auto light = cocos2d::utils::findChild(root, "light");
    if (light) {
        this->scheduleOnce([=](float){
            light->runAction(Repeat::create(Sequence::create(FadeTo::create(0.5f, 100), FadeIn::create(0.5f), NULL), 10000));
        }, random(1, 10)*0.1, "delay_light");
        
    }
}

void MJGameJumpingMonkeyVMonkey_Food::setDelegate(MJGameJumpingMonkeyVMonkey_FoodDelegate *_delegate){
    this->delegate = _delegate;
}

ui::Widget::ccWidgetClickCallback MJGameJumpingMonkeyVMonkey_Food::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onFood") {
        return CC_CALLBACK_1(MJGameJumpingMonkeyVMonkey_Food::onFood, this);
    }
    return nullptr;
}

void MJGameJumpingMonkeyVMonkey_Food::onFood(cocos2d::Ref *sender){
    if (!delegate) return;
    if (_type == 1 || _type == 4 || _type == 7 || _type == 10) {
        if (isRunAni) return;
        isRunAni = true;
        delegate->onClickFood(this, true);
        AUDIO_ENGINE->playEffect("sounds/jumpingmonkey/jumpingmonkey_correct.mp3");
        this->runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, -20)), MoveBy::create(0.1f, Vec2(0, 30)), MoveBy::create(0.075f, Vec2(0, -20)), MoveBy::create(0.075f, Vec2(0, 10)), CallFunc::create([=]{
        }), NULL));

    }else {
        if (isRunAni) return;
        isRunAni = true;
        delegate->onClickFood(this, false);
        AUDIO_ENGINE->playEffect("sounds/jumpingmonkey/jumpingmonkey_wrong.mp3");
        this->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, -20)), MoveBy::create(0.2f, Vec2(0, 30)), MoveBy::create(0.15f, Vec2(0, -20)), MoveBy::create(0.15f, Vec2(0, 10)), CallFunc::create([=]{
            isRunAni = false;
        }), NULL));
    }
}
