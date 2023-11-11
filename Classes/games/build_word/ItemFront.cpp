#include "ItemFront.h"
//#include "SimpleAudioEngine.h"
#include "SoundM.h"

USING_NS_CC;

ItemFront::ItemFront(){};
ItemFront::~ItemFront(){};

ItemFront* ItemFront::create(string letter, int type, Size size)
{
    ItemFront* obj = new ItemFront();
    if(obj->init())
    {
        obj->autorelease();
        obj->initOptions(letter, type, size);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void ItemFront::initOptions(string letter, int type, Size size)
{
    //draw back image
    _image = ImageView::create("games/build_word/b" + StringUtils::toString(type) + ".png");
    _image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_image);

    //determine scale according to tray
    float scaleWidthFactor = (1/_image->getContentSize().width) * size.width;
    float scaleHeightFactor = (1/_image->getContentSize().height) * size.height;

    CCLOG("pre --> width = %f, height = %f", scaleWidthFactor, scaleHeightFactor);

    //change scale of image
    //_image->setScale(scaleWidthFactor);

    this->setContentSize(_image->getContentSize());

    _image->setPosition(Vec2( this->getContentSize().width/2, this->getContentSize().height/2 ));

    //letter label
    _caption = Label::createWithTTF(letter, "fonts/monkeystories_medium.ttf", 100 /*AspectR::getFontSize(100)*/);
    _caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _caption->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    _caption->setTextColor(Color4B::WHITE);
    this->addChild(_caption);
}

/*void ItemFront::playLetterSound(float delay) {
    _caption->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=](){
        SoundM::playLetter(_caption->getString());
    }),TintTo::create(0.1,Color3B::RED),DelayTime::create(0.1),TintTo::create(0.1,Color3B::WHITE), nullptr));
}*/



void ItemFront::fadeOut() {
    _image->runAction(FadeOut::create(0.3));
    _caption->runAction(FadeOut::create(0.3));
    hided = true;
    /*_caption->runAction(Sequence::create(FadeOut::create(0.3), CallFunc::create([=](){
        setVisible(false);
    }), nullptr));*/
}

Label *ItemFront::caption() {
    return _caption;
}
