#ifndef Answer_h
#define Answer_h

#include "GAFWrapper.h"

USING_NS_CC;

class Answer :public cocos2d::ui::Layout {
public:
	Answer(std::string string);
	cocos2d::ui::Layout* getLayout();
	cocos2d::ui::Text* getText();
	std::string getAudio();
	void setAudio(std::string audio);
	
	void onClickRight(bool state);
	void onClickWrong();
	
private:
	cocos2d::ui::Layout* layout;
	cocos2d::ui::Text* text;
	std::string audio;
};

#endif // !Answer_h