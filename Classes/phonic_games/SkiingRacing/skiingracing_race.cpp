
#include "SkiingRacing_Race.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(SkiingRacing_Race);

#define CSB_NAME "csb/game/SkiingRacing/SkiingRacing_Race.csb"
//#define img_path "games/SkiingRacing/"

SkiingRacing_Race * SkiingRacing_Race::createRace() {
	
	auto p = static_cast<SkiingRacing_Race *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

SkiingRacing_Race * SkiingRacing_Race::createRace(SkiingRacingQuestion question, int numberOfQuestion) {

	auto p = static_cast<SkiingRacing_Race *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(question,numberOfQuestion);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}


void SkiingRacing_Race::didLoadFromCSB()
{
	auto bg1 = utils::findChild<Sprite*>(this, "bg_1");
	bg1->setTexture("games/skiingracing/rc_bg_without_start.png");
	auto bg2 = utils::findChild<Sprite*>(this, "bg_2");
	bg2->setTexture("games/skiingracing/rc_bg_without_start.png");

	auto bg_finish = utils::findChild<Sprite*>(this, "bg_finish");
	bg_finish->setVisible(true);
}
void SkiingRacing_Race::didLoadFromCSB(SkiingRacingQuestion question, int numberOfQuestion)
{
	this->_question = question;
	auto image_bin = utils::findChild<ImageView*>(this, "Image_bin");
	std::vector<std::string>paths = { "games/skiingracing/bin 1.png","games/skiingracing/bin 2.png","games/skiingracing/bin 3.png" };
	image_bin->loadTexture(paths[RandomHelper::random_int(0,2)]);
	image_bin->setVisible(true);

	if (numberOfQuestion != 0) {
		auto bg1 = utils::findChild<Sprite*>(this, "bg_1");
		bg1->setTexture("games/skiingracing/rc_bg_without_start.png");
		bg1->setPosition(Vec2(0, 550));
	}

	auto bg2 = utils::findChild<Sprite*>(this, "bg_2");
	bg2->setTexture("games/skiingracing/rc_bg_without_start.png");
	bg2->setPosition(Vec2(1810, 550));
}



void SkiingRacing_Race::setDelegate(SkiingRacing_RaceDelegate *delegate) {
	_delegate = delegate;
}



void SkiingRacing_Race::playEffect(bool correct) {

	if (correct) {
		//right

	}
	else {
		//wrong

	}
}

