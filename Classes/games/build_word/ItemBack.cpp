#include "ItemBack.h"
//#include "SimpleAudioEngine.h"

USING_NS_CC;

ItemBack::ItemBack(){};
ItemBack::~ItemBack(){};

ItemBack* ItemBack::create(string letter, int type, Size size)
{
    ItemBack* obj = new ItemBack();
    if(obj->init())
    {
        obj->autorelease();
        obj->initOptions(letter, type, size);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void ItemBack::initOptions(string letter, int type, Size size)
{
    //draw back image
//    auto str = "games/build_word/b" + StringUtils::toString(type) + "_back.png";

    _image = ImageView::create("games/build_word/b" + StringUtils::toString(type) + "_back.png");
    _image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_image);

    //determine scale according to tray
    float scaleWidthFactor = (1/_image->getContentSize().width) * size.width;
    float scaleHeightFactor = (1/_image->getContentSize().height) * size.height;

    CCLOG("pre --> width = %f, height = %f", scaleWidthFactor, scaleHeightFactor);

    //change scale of image
    //_image->setScale(scaleFactor);

    //CCLOG("nxt --> width = %f, height = %f", image->getContentSize().width, image->getContentSize().height);

    this->setContentSize(_image->getContentSize());

    _image->setPosition(Vec2( this->getContentSize().width/2, this->getContentSize().height/2 ));

    //letter label
    //_caption = Label::createWithTTF(letter, "fonts/Linotte_Bold.otf", AspectR::getFontSize(100));
    //_caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //_caption->setPosition(_image->getContentSize().width/2, _image->getContentSize().height/2);
    //_caption->setTextColor(Color4B::BLACK);
    //_caption->setOpacity(80);
    //_image->addChild(_caption);

}

bool ItemBack::isOverlap(Rect otherRect)
{
    if(this->getBoundingBox().containsPoint(Vec2(otherRect.getMidX(),otherRect.getMidY()))) {
        return true;
    }
    return false;
}

void ItemBack::fadeOut() {
    _image->runAction(FadeOut::create(0.3));
    //_caption->runAction(FadeOut::create(0.3));
}
