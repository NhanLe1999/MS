#ifndef Ufo_Sheep_h
#define Ufo_Sheep_h

#include "GAFWrapper.h"

USING_NS_CC;

class Ufo_Sheep :public cocos2d::ui::Layout {
public:
	Ufo_Sheep(std::string text, GAFWrapper* sheep);
	cocos2d::ui::Layout* getLayout();
	GAFWrapper* getSheep();
	cocos2d::ui::Text* getText();
	void setAudio(std::string audio);
	std::string getAudio();
	
private:
	cocos2d::ui::Layout* layout;
	cocos2d::ui::Text* text;
	GAFWrapper* sheep;
	std::string audio;
};

#endif 