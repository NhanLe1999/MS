#include "Item.h"

#define img_carriage "games/whathappenednext/whn_carriage_light.png"
#define img_carriage_red "games/whathappenednext/whn_carriage_red.png"

using namespace cocos2d;

Item::Item(std::string string) {
	cocos2d::ui::Text* text = cocos2d::ui::Text::create(string, font_name, font_sz);
	text->setString(string);
	this->text = text;
	text->setTextAreaSize(cocos2d::Size(300, 200));
	text->setTextColor(cocos2d::Color4B::BLACK);
	text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);

	layout = cocos2d::ui::Layout::create();	
	layout->setContentSize(cocos2d::Size(300, 200));
	layout->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	layout->setBackGroundImage(img_carriage);
    
    auto fx_img = ui::ImageView::create(img_carriage_red);
    layout->addChild(fx_img);
    fx_img->setPosition(layout->getContentSize()/2);
    fx_img->setOpacity(0);
    fx_img->setName("fx_img");

	this->addChild(layout);
	this->addChild(text);
    
    this->autorelease();
}

cocos2d::ui::Layout * Item::getLayout() {
	return layout;
}

void Item::setText(std::string text) {
	this->text->setString(text);
}

cocos2d::ui::Text * Item::getText() {
	return text;
}

void Item::setAudio(std::string audio) {
	this->audio = audio;
}

std::string Item::getAudio() {
	return this->audio;
}

void Item::setTouchEnable(bool enable) {
	this->getLayout()->setTouchEnabled(enable);
}

void Item::wrongItem() {
    auto fx = utils::findChild(layout, "fx_img");
    fx->runAction(Sequence::create(FadeTo::create(0.1, 180), FadeOut::create(0.1), FadeTo::create(0.1, 180), FadeOut::create(0.1), NULL));
//    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=] (){
//        fx->runAction(FadeTo::create(0.1, 180));
//    }), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
//        fx->runAction(FadeOut::create(0.1));
//    }), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
//        fx->runAction(FadeTo::create(0.2, 180));
//    }), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
//        fx->runAction(FadeOut::create(0.1));
//    }), NULL));
}

void Item::rightItem() {

}

void Item::setStartPos(cocos2d::Vec2 pos) {
	this->start_pos = pos;
}

void Item::backToStart() {
	this->runAction(EaseBackOut::create(MoveTo::create(0.5, start_pos)));
}

void Item::setIsRight(bool is_right) {
	this->is_right = is_right;
}

bool Item::isRight() {
	return is_right;
}
