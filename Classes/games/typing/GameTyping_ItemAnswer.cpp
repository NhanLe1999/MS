
#include "GameTyping_ItemAnswer.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameTyping_ItemAnswer);

USING_NS_CC;
#define CSB_NAME "csb/game/typing/GameTyping_ItemAnswer.csb"

MJGameTyping_ItemAnswer * MJGameTyping_ItemAnswer::createGame(ItemDistracting item_info) {
    auto p = static_cast<MJGameTyping_ItemAnswer *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameTyping_ItemAnswer::didLoadFromCSB(ItemDistracting item_info) {
    _item_info = item_info;
}

void MJGameTyping_ItemAnswer::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->setContentSize(cocos2d::Size(217, 132));
    ui::Helper::doLayout(this);

    text = this->getChildByName<cocos2d::ui::Text *>("text");
    text->setString(_item_info.text);
    if (CONFIG_MANAGER->isGameVMonkey()){
        auto font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
        text->setFontName(font_name);
    }
    text->setVisible(true);
    text->setOpacity(50);
    text->setTouchEnabled(true);
    auto pos = text->getPosition();
    text->setPositionY(this->getContentSize().height);
    text->runAction(Sequence::create(
        Spawn::create(FadeIn::create(0.25f), EaseBackOut::create(MoveTo::create(0.25f, pos)), NULL),
        CallFunc::create([=]{
           is_touch_enable = true;
        }),NULL));
    auto layoutTouch = utils::findChild<Layout*>(this, "layout_touch");
    layoutTouch->setTouchEnabled(true);
    text->addClickEventListener(CC_CALLBACK_1(MJGameTyping_ItemAnswer::onAnswer, this));
}

void MJGameTyping_ItemAnswer::onAnswer(cocos2d::Ref *sender){
    if (!is_touch_enable) return;
//    is_touch_enable = false;
    if (delegate) {
        delegate->onClickAnswer(this);
        CCLOG("_item_info.text = %s", _item_info.text.c_str());
    }
}


void MJGameTyping_ItemAnswer::onResultAnswer_True(){
    if (!is_touch_enable) {
        return;
    }
    is_touch_enable = false;
    auto img = this->getChildByName<cocos2d::ui::ImageView*>("Image_6");
    img->setVisible(true);
    img->loadTexture("games/typing/typing_itembg_green.png");
    text->setTextColor(Color4B::RED);
    text->runAction(Sequence::create(
        ScaleTo::create(0.15f, 1.25f),
        ScaleTo::create(0.15f, 1.0f),
        ScaleTo::create(0.15f, 1.25f),
        ScaleTo::create(0.15f, 1.0f),
        DelayTime::create(0.5f),
        CallFunc::create([=]() {
            img->setVisible(false);
            text->setTextColor(Color4B::BLACK);
            delegate->onDoneAnswer(this);
            is_touch_enable = true;
        }), NULL));
}
void MJGameTyping_ItemAnswer::onResultAnswer_False(){
    is_touch_enable = false;
    auto img = this->getChildByName<cocos2d::ui::ImageView*>("Image_6");
    img->setVisible(true);
    img->loadTexture("games/typing/typing_itembg_red.png");
    text->setTextColor(Color4B::WHITE);
    text->runAction(Sequence::create(ScaleTo::create(0.25f, 1.5f), ScaleTo::create(0.25f, 1.0f), NULL));
    text->runAction(Sequence::create(RotateTo::create(0.1f, -3), RotateTo::create(0.2f, 6), RotateTo::create(0.1f, 0),
        CallFunc::create([=]() {
        text->setTextColor(Color4B::BLACK);
        img->setVisible(false);
        is_touch_enable = true; 
    }), NULL));
   // text->setTextColor(Color4B::RED);
    CCLOG("Trả lời sai");
}


void MJGameTyping_ItemAnswer::setDelegate(MJGameTyping_ItemAnswerDelegate *_delegate){
    this->delegate = _delegate;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameTyping_ItemAnswer::onLocateClickCallback(const std::string &callback_name) {
   /* if (callback_name == "onAnswer") {
        return CC_CALLBACK_1(MJGameTyping_ItemAnswer::onAnswer, this);
    }*/
    return nullptr;
}


