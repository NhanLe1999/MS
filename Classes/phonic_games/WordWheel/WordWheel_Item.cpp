
#include "WordWheel_Item.h"

INITIALIZE_READER(WordWheel_Item);

#define CSB_NAME "csb/game/WordWheel/WordWheel_Item.csb"

WordWheel_Item * WordWheel_Item::createItem(mj::PHONIC_INFO item_info) {
    auto p = static_cast<WordWheel_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void WordWheel_Item::didLoadFromCSB(mj::PHONIC_INFO item_info)
{
    _audio = item_info.audio_full;
    this->setAnchorPoint(Point(0.5,0.5));
     _layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "bubble_panel");
    _layout->setLocalZOrder(10000);
    _layout->addTouchEventListener(CC_CALLBACK_2(WordWheel_Item::onTouched, this));
    
    auto bubble = cocos2d::utils::findChild<ImageView*>(_layout,"bubble");
    bubble->setLocalZOrder(-10);
    
    //create text
    auto textP = ui::Text::create("", font_name, font_sz);
    textP->setString(item_info.text_phonic);
    textP->setFontSize(font_sz);
    textP->setFontName(font_name);
    textP->setTextColor(Color4B::BLACK);
    textP->setName("bubble_textP");
    textP->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textP->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    _layout->addChild(textP, -2);
    textP->setPosition(Vec2(_layout->getContentSize().width*0.5, _layout->getContentSize().height*0.45));
    textP->setOpacity(0);
    
    //create text
    auto textF = ui::Text::create("", font_name, font_sz);
    textF->setString(item_info.text_full);
    textF->setFontSize(font_sz);
    textF->setFontName(font_name);
    textF->setTextColor(Color4B::BLACK);
    textF->setName("bubble_textF");
    textF->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textF->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    _layout->addChild(textF, -2);
    textF->setPosition(Vec2(_layout->getContentSize().width*0.5, _layout->getContentSize().height*0.45));
    
    //create image
    if(item_info.image_path != ""){
        //hide text
        textF->setOpacity(0);
        //show image
        auto image = utils::findChild<ImageView*>(_layout, "image");
        image->loadTexture(item_info.image_path);
        image->setLocalZOrder(-9);
        image->setOpacity(0);
    }
}

void WordWheel_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void WordWheel_Item::onTouched(Ref* sender, ui::Widget::TouchEventType type){
    if(_delegate){
        _delegate->onTouchedDelegate(sender, type);
    }
}

void WordWheel_Item::setDelegate(WordWheel_ItemDelegate *delegate){
    _delegate = delegate;
}

void WordWheel_Item::playEffect(float angle){
    GAFWrapper* set = utils::findChild<GAFWrapper*>(_layout, "set2");
    set->setRotation(angle);
    set->playSequence("action", true);
}
