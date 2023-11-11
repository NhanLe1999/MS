#ifndef ShootingStar_Item_h
#define ShootingStar_Item_h

#include "GAFWrapper.h"

USING_NS_CC;

class ShootingStar_Item :public cocos2d::ui::Layout {
public:
	ShootingStar_Item(std::string str, std::string star_gaf, bool tail);

	cocos2d::ui::Text* getText();
	GAFWrapper* getTail();
	GAFWrapper* getStar();
	cocos2d::ui::Layout* getLayout();
private:
	cocos2d::ui::Layout* layout;
	GAFWrapper* star;
	GAFWrapper* star_tail;
	cocos2d::ui::Text* text;

};

#endif // !ShootingStar_Item_h
