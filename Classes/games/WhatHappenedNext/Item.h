#ifndef Item_h
#define Item_h

#include "GAFWrapper.h"

USING_NS_CC;

class Item :public cocos2d::ui::Layout {
public:
	Item(std::string string);
	cocos2d::ui::Layout* getLayout();
	cocos2d::ui::Text* getText();
	void setAudio(std::string audio);
	std::string getAudio();
	void setStartPos(cocos2d::Vec2 pos);
	void setText(std::string text);

	void setTouchEnable(bool enable);
	void wrongItem();
	void rightItem();

	void backToStart();
	bool isRight();
	void setIsRight(bool is_right);
	
private:
	cocos2d::ui::Layout* layout;
	cocos2d::ui::Text* text;
	std::string audio;
	cocos2d::Vec2 start_pos;
	bool is_right;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 30;
};

#endif // !Item_h
