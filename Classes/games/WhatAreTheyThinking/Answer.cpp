#include "Answer.h"

#define img_red "hd/games/WhatAreTheyThinking/textboxes/watt_r_tbox6.png"
#define img_green "hd/games/WhatAreTheyThinking/textboxes/watt_g_tbox6.png"
#define img_white "hd/games/WhatAreTheyThinking/textboxes/watt_tbox6.png"

using namespace cocos2d;

Answer::Answer(std::string string) {
	cocos2d::ui::Text* text = cocos2d::ui::Text::create();
	text->setText(string);
	this->text = text;
	text->setTextAreaSize(cocos2d::Size(300, 200));
	//text->setContentSize(cocos2d::Size(200, 200));
	text->setFontSize(35);
	text->setTextColor(cocos2d::Color4B::BLACK);
	text->setTextHorizontalAlignment(cocos2d::CCTextAlignment::CENTER);
	text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);

	layout = cocos2d::ui::Layout::create();	
	layout->setContentSize(cocos2d::Size(300, 200));
	layout->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	layout->setBackGroundImage(img_white);

	this->addChild(layout);
	this->addChild(text);
}

cocos2d::ui::Layout * Answer::getLayout() {
	return layout;
}

cocos2d::ui::Text * Answer::getText() {
	return text;
}

void Answer::onClickWrong() {
	this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
		this->getLayout()->setBackGroundImage(img_red);
		this->runAction(EaseBackOut::create(MoveBy::create(0.05, Point(10, 0))));
		//this->runAction(Repeat::create(Sequence::create(DelayTime::create(0), MoveBy::create(0.1, Vec2(10, 0)), MoveBy::create(0.1, Vec2(-10, 0)), MoveBy::create(0.1, Vec2(10, 0)), NULL), 2));
	}), DelayTime::create(0.05), CallFunc::create([=] {
		this->runAction(EaseBackOut::create(MoveBy::create(0.1, Point(-20, 0))));
	}), DelayTime::create(0.1), CallFunc::create([=] {
		this->runAction(EaseBackOut::create(MoveBy::create(0.05, Point(10, 0))));
	}), DelayTime::create(0.05), CallFunc::create([=] {
		this->runAction(EaseBackOut::create(MoveBy::create(0.05, Point(10, 0))));
	}), DelayTime::create(0.05), CallFunc::create([=] {
		this->runAction(EaseBackOut::create(MoveBy::create(0.1, Point(-20, 0))));
	}), DelayTime::create(0.1), CallFunc::create([=] {
		this->runAction(EaseBackOut::create(MoveBy::create(0.05, Point(10, 0))));
	}), DelayTime::create(0.1), CallFunc::create([=] {
		this->getLayout()->setBackGroundImage(img_white);
	}), NULL));
}

void Answer::onClickRight(bool state) {
	this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
		this->getLayout()->setBackGroundImage(img_green);
		this->runAction(EaseBackOut::create(ScaleTo::create(0.2, 1.2)));
	}), DelayTime::create(2), CallFunc::create([=] {
		this->getLayout()->setBackGroundImage(img_white);
		this->runAction(ScaleTo::create(1, 0));
		if (state) {
			this->runAction(RotateTo::create(1, 45));
		}
		else {
			this->runAction(RotateTo::create(1, -45));
		}
		
	}), DelayTime::create(1), CallFunc::create([=] {
		this->getLayout()->setBackGroundImage(img_white);
	}), NULL));
}

std::string Answer::getAudio() {
	return audio;
}

void Answer::setAudio(std::string audio) {
	this->audio = audio;
}