//
//  MSStoryGuide.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/3/18.
//

#ifndef MSStoryGuide_hpp
#define MSStoryGuide_hpp

#include "cocostudiomacro2.h"

class MSStoryGuide : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MSStoryGuide);
    static MSStoryGuide * createGuide();
    void startTimer();
    
private:
    void didLoadFromCSB();
};

CREATE_READER(MSStoryGuide);

#endif /* MSStoryGuide_hpp */
