#include "Ufo_sheep.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
Ufo_Sheep::Ufo_Sheep(std::string string, GAFWrapper* sheep) {
	cocos2d::ui::Text* text = cocos2d::ui::Text::create();
	text->setText(string);
	this->text = text;
	this->sheep = sheep;

	text->setFontSize(80);
	text->setFontName(CONFIG_MANAGER->isVietnameseStories()? PHONIC_MANAGER->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF");
	text->setTextColor(cocos2d::Color4B::WHITE);
	text->enableOutline(Color4B::BLACK, 5);
	text->setPosition(cocos2d::Vec2(-20, 0));

	layout = cocos2d::ui::Layout::create();
	this->addChild(layout);
	this->addChild(sheep);
	this->addChild(text);

	layout->setContentSize(cocos2d::Size(200, 200));

	layout->setContentSize(cocos2d::Size(200, 200));
	layout->setAnchorPoint(Vec2(0.5, 0.5));
    
    this->autorelease();
}

cocos2d::ui::Layout * Ufo_Sheep::getLayout() {
	return layout;
}

GAFWrapper * Ufo_Sheep::getSheep() {
	return sheep;
}

cocos2d::ui::Text* Ufo_Sheep::getText() {
	return text;
}

void Ufo_Sheep::setAudio(std::string audio) {
	this->audio = audio;
}

std::string Ufo_Sheep::getAudio() {
	return this->audio;
}
