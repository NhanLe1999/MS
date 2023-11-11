//
//  Coop.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#ifndef Coop_hpp
#define Coop_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
class MJCoop : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJCoop);
    static MJCoop * createCoop(mj::GameObject word, int slot);
    inline std::string getWord() { return _word.text; }
    void putChicken();
    bool isFull();
    
private:
    void didLoadFromCSB(mj::GameObject word, int slot);

    mj::GameObject _word;
    std::vector<cocos2d::ui::ImageView *> _doors;
};

CREATE_READER(MJCoop);

#endif /* Coop_hpp */
