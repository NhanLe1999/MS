//
//  MSActivityWait.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#ifndef MSActivityWait_hpp
#define MSActivityWait_hpp

#include "cocostudiomacro2.h"

class MSActivityWait : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MSActivityWait);
    static MSActivityWait * createView();
    
private:
    void didLoadFromCSB();
    void onEnter();
};

CREATE_READER(MSActivityWait);

#endif /* MSActivityWait_hpp */
