#include "ShootingStar_Item.h"

#define STAR_TAIL "gaf/ShootingStar/stars_tail1/stars_tail1.gaf"

using namespace cocos2d;

ShootingStar_Item::ShootingStar_Item(std::string str, std::string star_gaf, bool tail) {
	star = GAFWrapper::createGAF(star_gaf);

	layout = cocos2d::ui::Layout::create();
	layout->setContentSize(cocos2d::Size(200, 200));
	layout->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	star_tail = GAFWrapper::createGAF(STAR_TAIL);
	star_tail->setRotation(-30);
	star_tail->setAnchorPoint(cocos2d::Vec2(0, 0));
	star_tail->setPosition(cocos2d::Vec2(0, -80));

	cocos2d::ui::Text* text = cocos2d::ui::Text::create();
	text->setText(str);
	this->text = text;
	text->setFontSize(80);
	text->setFontName("fonts/KGWHATTHETEACHERWANTS.TTF");
	text->setTextColor(cocos2d::Color4B::WHITE);
	text->enableOutline(Color4B::BLACK, 3);
	text->setPosition(cocos2d::Vec2(0, -50));

	if (!tail) {
		star_tail->setRotation(-90);
		star_tail->setPosition(cocos2d::Vec2(40, -50));
	}

	this->addChild(star_tail);
	this->addChild(star);
	this->addChild(text);
	this->addChild(layout);
	
	star_tail->play(true);
    
    this->autorelease();
}

cocos2d::ui::Text* ShootingStar_Item::getText() {
	return text;
}

GAFWrapper* ShootingStar_Item::getTail() {
	return star_tail;
}

GAFWrapper* ShootingStar_Item::getStar() {
	return star;
}

cocos2d::ui::Layout* ShootingStar_Item::getLayout() {
	return layout;
}
