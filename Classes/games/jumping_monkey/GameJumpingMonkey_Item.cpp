
#include "GameJumpingMonkey_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameJumpingMonkey_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/jumping_monkey/GameJumpingMonkey_Item.csb"

MJGameJumpingMonkey_Item * MJGameJumpingMonkey_Item::createGame(ItemInfo item_info, int type) {
    auto p = static_cast<MJGameJumpingMonkey_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, type);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingMonkey_Item::didLoadFromCSB(ItemInfo item_info, int type) {
    _item_info = item_info;
    type_item = type;
    if (type_item == TEXT_TYPE) {
        auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "lb_item");
        cocos2d::utils::findChild(this, "img_item")->setVisible(false);
        text->setString(item_info.text);
        while (text->getContentSize().width > 0.8*296.0f) {
            text->setFontSize(text->getFontSize() - 1);
        }
    }else{
        auto img = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "img_item");
        cocos2d::utils::findChild(this, "lb_item")->setVisible(false);
        img->loadTexture(item_info.img_path);
        
    }
}

void MJGameJumpingMonkey_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameJumpingMonkey_Item::onRightAnswer(){
    CCLOG("onWrightAnswer");
    auto duration = 0.1f;
    is_click_enable = false;
    auto layout_wrong = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_wrong");
    layout_wrong->setVisible(true);
    layout_wrong->setBackGroundColor(Color3B::GREEN);
    //AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_correct.mp3");

    auto height = 20;
    this->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), CallFunc::create([=]{
        layout_wrong->setVisible(false);
    }), NULL));
    if (type_item == IMG_TYPE) {
        /*auto bg_item = this->getChildByName("bg_item");
        auto tick = GAFWrapper::createGAF("gaf/jumping_monkey/tick/tick.gaf");
        bg_item->addChild(tick, 10000);
        tick->setPosition(bg_item->getContentSize() / 2);
        tick->play(false);*/
    }
    else
    {
        auto text = cocos2d::utils::findChild<cocos2d::ui::Text*>(this, "lb_item");
        if (text) {
            auto preScaleFactor = text->getScale();
            text->runAction(Sequence::create(
                DelayTime::create(1.0f),
                CallFunc::create([=]() {
                    text->setTextColor(Color4B::RED);
                }),
                ScaleTo::create(0.2f, 1.3f),
                DelayTime::create(0.5f),
                CallFunc::create([=]() {
                    text->setTextColor(Color4B::BLACK);
                }),         
                ScaleTo::create(0.2f, preScaleFactor)
                ,NULL));
        }
 
    }
}

void MJGameJumpingMonkey_Item::onWrongAnswer(){
    CCLOG("onWrongAnswer");
    auto duration = 0.1f;
    auto height = 20;
    
    auto layout_wrong = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_wrong");
    layout_wrong->setVisible(true);
    layout_wrong->setBackGroundColor(Color3B::RED);
    AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_wrong.mp3");
    
    this->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), CallFunc::create([=]{
        layout_wrong->setVisible(false);
        is_click_enable = true;
    }), NULL));
    
}

void MJGameJumpingMonkey_Item::setDelegate(MJGameJumpingMonkey_ItemDelegate *_delegate){
    this->delegate = _delegate;
}


void MJGameJumpingMonkey_Item::onChoose(cocos2d::Ref *sender){
    if (!is_click_enable) return;
    if (delegate) {
        delegate->onClickItem(this, type_item);
    }
}

ui::Widget::ccWidgetClickCallback MJGameJumpingMonkey_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onChoose") {
        return CC_CALLBACK_1(MJGameJumpingMonkey_Item::onChoose, this);
    }
    return nullptr;
}
