//
//  CaterpillaRacing_Item.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/16/18.
//

#ifndef CaterpillaRacing_Item_hpp
#define CaterpillaRacing_Item_hpp

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
USING_NS_CC;
using namespace ui;

namespace CaterpillaData {
    struct Game {
        std::string text;
        std::string audio_path;
        std::vector<std::pair<std::string, std::string>> images;
    };
};

class CaterpillaRacing_ItemDelegate{
    public:
    virtual void onClickedDelegate(cocos2d::Ref *sender) {};
};

class CaterpillaRacing_Item : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(CaterpillaRacing_Item);
    static CaterpillaRacing_Item* createItem(CaterpillaData::Game game, int showImage = 1);
    void setDelegate(CaterpillaRacing_ItemDelegate* delegate);
    void bubbleEffect(int delta, int i);
    std::string getAudioPath();
//    void setClean(int clean);
//    bool getClean();
private:
    void didLoadFromCSB(CaterpillaData::Game game, int txt_only);
    void onEnter() override;
//    void update() override;
    
    void onClickItem(Ref* sender);
    std::string font_name = "fonts/phonicfont.ttf";
    int font_sz = 120;
    std::vector<Color4B> text_color {
        Color4B(96, 78, 11, 255), Color4B(253, 253, 253, 255), Color4B(253, 260, 67, 255), Color4B(206, 204, 43, 255)
    };
    
    CaterpillaRacing_ItemDelegate* _delegate;
    std::string _audio;
};

CREATE_READER(CaterpillaRacing_Item)
#endif /* CaterpillaRacing_Item_hpp */
