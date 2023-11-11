//
//  Firefly_Item.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/3/18.
//

#ifndef Firefly_Item_hpp
#define Firefly_Item_hpp

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;

class Firefly_ItemDelegate{
public:
    virtual void onTouchedDelegate(Ref* sender, ui::Widget::TouchEventType type) {};
};


class Firefly_Item : public cocos2d::ui::Layout {
  
public:
    CREATE_FUNC(Firefly_Item);
    static Firefly_Item* createItem(std::string word, int fly_id, int word_id, gaf::GAFAsset* asset);
    void setDelegate(Firefly_ItemDelegate* delegate);
    void setMoving(bool isMove);
    bool isMoving();
	void setDisable(bool a) {
		is_disable = a;
	}
	bool isDisable() {
		return is_disable;
	}
private:
    void didLoadFromCSB(std::string word, int fly_id, int word_id, gaf::GAFAsset* asset);
    void onEnter() override;
    
    void onTouchedItem(Ref* sender, ui::Widget::TouchEventType type);

	bool is_disable = false;
	
	Firefly_ItemDelegate* _delegate;
    bool _moving = false;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 50;
};

CREATE_READER(Firefly_Item)
#endif /* Firefly_Item_hpp */
