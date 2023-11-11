
#include "SkiingRacing_Map.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(SkiingRacing_Map);

#define CSB_NAME "csb/game/SkiingRacing/SkiingRacing_Map.csb"
//#define img_path "games/SkiingRacing/"

SkiingRacing_Map * SkiingRacing_Map::createMap(std::string name_character_1, std::string name_character_2, std::string name_main_character) {
	
	auto p = static_cast<SkiingRacing_Map *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(name_character_1, name_character_2,name_main_character);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}




void SkiingRacing_Map::didLoadFromCSB(std::string name_character_1, std::string name_character_2, std::string name_main_character)
{
	GAFWrapper* gaf_start = GAFWrapper::createGAF("gaf/skiingracing/Rc_point/Rc_point.gaf");
	gaf_start->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_start->setPosition(utils::findChild<Node*>(this, "Node_start")->getPosition());
	this->addChild(gaf_start, 2);
	gaf_start->playSequence("blue", false);
	GAFWrapper* gaf_end = GAFWrapper::createGAF("gaf/skiingracing/Rc_point/Rc_point.gaf");
	gaf_end->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_end->setPosition(utils::findChild<Node*>(this, "Node_end")->getPosition());
	this->addChild(gaf_end, 2);
	gaf_end->playSequence("red", false);
	main_charater_arrow = utils::findChild<Layout*>(this, name_main_character);
	origin = main_charater_arrow->getPositionX();
	charater_1_arrow = utils::findChild<Layout*>(this, name_character_1);
	charater_2_arrow = utils::findChild<Layout*>(this, name_character_2);
	charater_1_arrow->setZOrder(10);
	charater_2_arrow->setZOrder(11);
	main_charater_arrow->setZOrder(12);
	long_map = utils::findChild<Node*>(this, "Node_end")->getPosition().x - utils::findChild<Node*>(this, "Node_start")->getPosition().x;
}
void SkiingRacing_Map::update_character1(float rate) {
	if (rate <= 1.) {
		charater_1_arrow->setPositionX(origin + rate * long_map);
	}
	
}
void SkiingRacing_Map::update_character2(float rate) {
	if (rate <= 1.) charater_2_arrow->setPositionX(origin + rate * long_map);
}
void SkiingRacing_Map::update_main_character(float rate) {
	if (rate <= 1.) main_charater_arrow->setPositionX(origin + rate * long_map);
}
void SkiingRacing_Map::playEffect(bool correct) {

	if (correct) {
		//right

	}
	else {
		//wrong

	}
}

