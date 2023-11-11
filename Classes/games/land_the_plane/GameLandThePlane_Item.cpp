
#include "GameLandThePlane_Item.h"
#include "AudioEngine.h"
INITIALIZE_READER(MJGameLandThePlane_Item);

USING_NS_CC;

MJGameLandThePlane_Item * MJGameLandThePlane_Item::createFrame(mj::GameObject object, int type, int font_size) {
    auto p = static_cast<MJGameLandThePlane_Item *>(cocos2d::CSLoader::createNode("csb/game/landtheplane/GameLandThePlane_Item.csb"));
    if (p) {
        p->didLoadFromCSB(object, type, font_size);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameLandThePlane_Item::didLoadFromCSB(mj::GameObject object, int type, int font_size) {
    _data = object;
    _type = type;
    std::string font_name = "fonts/monkeystories_ultra.ttf";
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    auto _text = this->getChildByName<Text *>("text");
    _text->setVisible(false);
    
    text = Label::createWithTTF("", font_name, font_size);
    text->enableOutline(Color4B::WHITE, 2);
    text->setTextColor(Color4B::BLACK);
    this->addChild(text);
    text->setPosition(this->getContentSize()/2);
    ////text->setWidth(220); Fixed width -> new line
    text->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    text->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    text->setString(object.text);
    text->setLocalZOrder(1000);
    
    
    auto img = getImgWith(StringUtils::format("games/landtheplane/landtheplane_land%d.png", type), 0);
    
    img->setScale(1.4, 1.2); //// scale island up for long words
    this->addChild(img);
//    type = id_land
    
}


ImageView* MJGameLandThePlane_Item::getImgWith(string path_img, int type){//create ICON
    auto img = ImageView::create(path_img);
    img->setPosition(this->getContentSize()/2);
    img->setTouchEnabled(true);
    img->addClickEventListener(CC_CALLBACK_1(MJGameLandThePlane_Item::onChooseWord, this));
    return img;
}

void MJGameLandThePlane_Item::addCloudWhenWrongAnswer(){
    isTouchEnable = false;
    if (cloud) return;
    cloud = GAFWrapper::createGAF("gaf/may/may.gaf");
    cloud->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cloud->setPosition(Vec2(this->getContentSize().width*0.55, this->getContentSize().height*0.4));
    this->addChild(cloud, 10000);
    cloud->playSequence("ac2", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        cloud->playSequence("ac3", true);
    });
    cocos2d::experimental::AudioEngine::play2d("sounds/landtheplane/landtheplane_cloud.mp3");
}

std::string MJGameLandThePlane_Item::getObjectName() {
    return _data.text;
}

void MJGameLandThePlane_Item::setDelegate(MJGameLandThePlane_ItemDelegate * delegate) {
    this->delegate = delegate;
}

void MJGameLandThePlane_Item::onChooseWord(Ref *sender){//delegate when click item.
    if (!isTouchEnable) return;
    if (delegate) {
        delegate->onClickLand(this);
    }
}

ui::Widget::ccWidgetClickCallback MJGameLandThePlane_Item::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(MJGameLandThePlane_Item, onChooseWord),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}


