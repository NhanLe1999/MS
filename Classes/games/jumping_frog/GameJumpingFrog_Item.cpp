
#include "GameJumpingFrog_Item.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
#include "HSAudioEngine.h"
INITIALIZE_READER(MJGameJumpingFrog_Item);

USING_NS_CC;

#define CSB_NAME "csb/game/jumpingfrog/GameJumpingFrog_Item.csb"

MJGameJumpingFrog_Item * MJGameJumpingFrog_Item::createFrame(mj::GameObject object, int type, int font_size) {
    auto p = static_cast<MJGameJumpingFrog_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(object, type, font_size);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingFrog_Item::didLoadFromCSB(mj::GameObject object, int type, int font_size) {
    _data = object;
    _type = type;

    auto _text = this->getChildByName<Text *>("text");
    _text->setVisible(false);
    std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/monkeystories_medium.ttf";
    text = Label::createWithTTF("", font_name, font_size);
    text->enableOutline(Color4B::WHITE, 2);
    text->setTextColor(Color4B::BLACK);
    this->addChild(text);
    text->setPosition(this->getContentSize()/2);
    //text->setWidth(200);
    text->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    text->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    text->setString(object.text);
    text->setLocalZOrder(1001);

    auto bb = text->getBoundingBox();
    if(bb.size.width > 200)
    {
        auto config = text->getTTFConfig();
        config.fontSize = font_size * (200 / bb.size.width);
        text->setTTFConfig(config);
    }

    item_gaf = GAFWrapper::createGAF(StringUtils::format("gaf/beo%d/beo%d.gaf", type, type));
    item_gaf->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item_gaf->setPosition(Vec2(this->getContentSize().width*0.5, this->getContentSize().height*0.5));
    this->addChild(item_gaf, 1000);
    item_gaf->playSequence("idle", true);
//    type = id_land

}


ImageView* MJGameJumpingFrog_Item::getImgWith(string path_img, int type){//create ICON
    auto img = ImageView::create(path_img);
    img->setPosition(this->getContentSize()/2);
    img->setTouchEnabled(true);
    img->addClickEventListener(CC_CALLBACK_1(MJGameJumpingFrog_Item::onChooseWord, this));
    return img;
}

void MJGameJumpingFrog_Item::whenWrongAnswer(){
    AUDIO_ENGINE->stopSound(_data.audio_path);
    AUDIO_ENGINE->playSound(_data.audio_path,false);
    isTouchEnable = false;
    text->setVisible(false);
    this->runAction(Spawn::create(Sequence::create(ScaleTo::create(0.25f, 0.35f), NULL), FadeTo::create(0.25f, 120), NULL));
}

void MJGameJumpingFrog_Item::whentrueAnswer(){
    if (item_gaf) {
        item_gaf->playSequence("deepwater", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            item_gaf->playSequence("idle", true);
        });
    }

    text->runAction(Sequence::create(ScaleTo::create(0.25f, 0.85f), DelayTime::create(0.3f), ScaleTo::create(0.275f, 1.0f), NULL));
}


void MJGameJumpingFrog_Item::whenClickTrue(){
    item_gaf->runAction(Sequence::create(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL));
}

std::string MJGameJumpingFrog_Item::getObjectName() {
    return _data.text;
}

void MJGameJumpingFrog_Item::setDelegate(MJGameJumpingFrog_ItemDelegate * delegate) {
    this->delegate = delegate;
}

void MJGameJumpingFrog_Item::onChooseWord(Ref *sender){//delegate when click item.
    if (!isTouchEnable) return;
    if (delegate) {
        delegate->onClickItem(this);
    }
}

ui::Widget::ccWidgetClickCallback MJGameJumpingFrog_Item::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameJumpingFrog_Item, onChooseWord),
    };

    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}
