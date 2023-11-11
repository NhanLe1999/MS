
#include "GameJumpingMonkey_Item_VMonkey.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameJumpingMonkeyVMonkey_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/jumping_monkey/GameJumpingMonkey_Item.csb"

MJGameJumpingMonkeyVMonkey_Item * MJGameJumpingMonkeyVMonkey_Item::createGame(ItemInfo item_info, int type) {
    auto p = static_cast<MJGameJumpingMonkeyVMonkey_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, type);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingMonkeyVMonkey_Item::didLoadFromCSB(ItemInfo item_info, int type) {
    _item_info = item_info;
    type_item = type;
    if (type_item == TEXT_TYPE) {
        auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "lb_item");
        cocos2d::utils::findChild(this, "img_item")->setVisible(false);
        text->setString(item_info.text);
		while (text->getContentSize().width > 247){
			text->setFontSize(text->getFontSize() - 1);
		}
    }else{
        auto img = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "img_item");
        cocos2d::utils::findChild(this, "lb_item")->setVisible(false);
        img->loadTexture(item_info.img_path);
        
    }
}

void MJGameJumpingMonkeyVMonkey_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameJumpingMonkeyVMonkey_Item::onWrightAnswer(){
    CCLOG("onWrightAnswer");
    auto duration = 0.1f;
    is_click_enable = false;
    auto layout_wrong = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_wrong");
    layout_wrong->setVisible(true);
    layout_wrong->setBackGroundColor(Color3B::GREEN);
   

    auto height = 20;
    this->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), MoveBy::create(duration, Vec2(0, height)), MoveBy::create(duration, Vec2(0, -height)), CallFunc::create([=]{
        layout_wrong->setVisible(false);
    }), NULL));
    
    auto bg_item = this->getChildByName("bg_item");
    ImageView* img_tick;
    if (mj::helper::isRetina()){
        img_tick = ui::ImageView::create("gaf/jumping_monkey/tick/tick_hdr.png");

    }else{
      img_tick = ui::ImageView::create("gaf/jumping_monkey/tick/tick_hd.png");
    }
    
   
    bg_item->addChild(img_tick, 10000);
    img_tick->setPosition(bg_item->getContentSize()/2);
    img_tick->setScale(0);
    img_tick->runAction(Sequence::create(ScaleTo::create(0.25,1.0),ScaleTo::create(0.15,0.5),MoveTo::create(0.15,Vec2(70,70)),NULL));
    
}

void MJGameJumpingMonkeyVMonkey_Item::onWrongAnswer(){
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

void MJGameJumpingMonkeyVMonkey_Item::setDelegate(MJGameJumpingMonkeyVMonkey_ItemDelegate *_delegate){
    this->delegate = _delegate;
}


void MJGameJumpingMonkeyVMonkey_Item::onChoose(cocos2d::Ref *sender){
    if (!is_click_enable) return;
    if (delegate) {
        delegate->onClickItem(this, type_item);
    }
}

ui::Widget::ccWidgetClickCallback MJGameJumpingMonkeyVMonkey_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onChoose") {
        return CC_CALLBACK_1(MJGameJumpingMonkeyVMonkey_Item::onChoose, this);
    }
    return nullptr;
}
