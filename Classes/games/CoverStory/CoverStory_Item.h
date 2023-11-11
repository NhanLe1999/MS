//
//  CoverStory_Item.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef CoverStory_Item_hpp
#define CoverStory_Item_hpp

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
USING_NS_CC;
using namespace ui;

class CoverStory_ItemDelegate{
public:
    virtual void onClickedDelegate(Ref* sender) {};
};

class CoverStory_Item : public cocos2d::ui::Layout {
    
public:
    CREATE_FUNC(CoverStory_Item);
    static CoverStory_Item* createItem(std::string text);
    void setDelegate(CoverStory_ItemDelegate* delegate);
    void playEffect(int is_right);
    
private:
    void didLoadFromCSB(std::string word);
    void onEnter() override;
    void onClickedAnswer(Ref* sender);
    
    CoverStory_ItemDelegate* _delegate;
    bool _is_right = false;
    std::string font_name = "fonts/Montserrat-Regular.ttf";
    int font_sz = 30;
};

CREATE_READER(CoverStory_Item)

#endif /* CoverStory_Item_hpp */
