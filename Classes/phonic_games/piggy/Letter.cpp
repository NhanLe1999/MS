//
//  Letter.cpp
//  Piggy-mobile
//
//  Created by Kiet Le on 1/5/18.
//

#include "Letter.h"

Letter::Letter():
Label()
{
}

Letter::~Letter(){
}

Letter* Letter::createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, const Size& dimensions /* = Size::ZERO */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    auto ret = new (std::nothrow) Letter();
    
    if (ret && ret->initWithTTF(text, fontFile, fontSize, dimensions, hAlignment, vAlignment))
    {
        ret->initOption();
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Letter::initOption(){
    auto cSize = getContentSize();
	float offset = 0.2f * cSize.width;
    _rightPoint = Vec2(cSize.width - offset, cSize.height/2 - 60);
    _leftPoint = Vec2(offset, cSize.height/2 - 60);
   
    setTextColor(Color4B::BLACK);
    enableOutline(Color4B::WHITE,2);
}

bool Letter::isLetterIntersectRect(const cocos2d::Rect &rect){
    //letter on left of Pig
    //if ( getPositionX() <= rect.getMidX() ){
    //    return rect.containsPoint( convertToWorldSpace(_leftPoint) );
    //}
    ////letter on right of Pig
    //else{
    //    return rect.containsPoint( convertToWorldSpace(_rightPoint) );
    //}
	auto left_point = convertToWorldSpace(_leftPoint);
	auto right_point = convertToWorldSpace(_rightPoint);
	auto cSize = getContentSize();
	return rect.intersectsRect(Rect(left_point,Size(right_point.x - left_point.x,cSize.height)));
}

void Letter::addToBasket(Sprite * _pig, const int & zOrder){
    //letter on left side
    float deltaX = .0f;
    if (getPositionX() <= _pig->getPositionX()){
        //CCLOG( "%f", convertToWorldSpace(_leftPoint).x - _pig->getPositionX() );
        if ( _pig->getPositionX() - convertToWorldSpace(_leftPoint).x > 90 ){
            deltaX = _pig->getPositionX() - convertToWorldSpace(_leftPoint).x - 90;
            deltaX += random(.0f, 15.0f);
        }
    }else{
        if( convertToWorldSpace(_rightPoint).x - _pig->getPositionX() > 90 ){
            deltaX = -(convertToWorldSpace(_rightPoint).x - _pig->getPositionX() - 90);
            deltaX -= random(.0f, 15.0f);
        }
    }
    
    auto posOfBasket = Vec2(_pig->getContentSize().width/2 ,
                            _pig->getContentSize().height - 40/*- 208*/);
    auto oldWorPos = this->convertToWorldSpaceAR(Vec2(0, 0));
    auto newLocPos = _pig->convertToNodeSpaceAR(oldWorPos);
    
    this->removeFromParentAndCleanup(false);
    this->setPosition(Vec2(newLocPos.x + posOfBasket.x, posOfBasket.y));
    _pig->addChild(this, zOrder);
    
    stopAllActions();
    runAction(JumpBy::create(.3f, Vec2(deltaX, 0), 30, 1));
}

void Letter::jumpOutBasket(){
    stopAllActions();
    int direction = rand() % 2 == 0 ? 1: -1;
    runAction( Sequence::create(JumpBy::create(0.8f, Vec2(250 * direction, -400), 280, 1),
                                RemoveSelf::create(), NULL) );
}

void Letter::fitValueTextSize(cocos2d::Size box)
{
	auto size = box;
	auto label_size = this->getContentSize();
	auto ttf_config = this->getTTFConfig();
	while (label_size.width > size.width * 0.75 && label_size.height > size.height * 0.75) {
		ttf_config.fontSize -= 2;
		this->setTTFConfig(ttf_config);
		label_size = this->getContentSize();
	}
	while (label_size.width < size.width * 0.75 && label_size.height < size.height * 0.75) {
		ttf_config.fontSize -= 2;
		this->setTTFConfig(ttf_config);
		label_size = this->getContentSize();
	}
}
