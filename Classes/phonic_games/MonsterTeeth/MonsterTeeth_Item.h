//
//  MonsterTeeth_Item.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/10/18.
//

#ifndef MonsterTeeth_Item_hpp
#define MonsterTeeth_Item_hpp
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;

class MonsterTeeth_Item : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MonsterTeeth_Item);
    static MonsterTeeth_Item* createItem(std::string word, int index);
    void setClean(int clean);
    bool getClean();
	std::string getWord();
private:
    void didLoadFromCSB(std::string word, int index);
    void onEnter() override;

	std::string m_word;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 80;
    std::vector<Color4B> text_color {
        Color4B(96, 78, 11, 255),Color4B::RED, Color4B(253, 260, 67, 255), Color4B::BLUE
    };
    
	//Color4B(253, 253, 253, 255)
	//Color4B(206, 204, 43, 255)
    bool _clean;
    
};

CREATE_READER(MonsterTeeth_Item)

#endif /* MonsterTeeth_Item_hpp */

