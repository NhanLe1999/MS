//
//  MSTrialPopup.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/7/17.
//

#ifndef MSTrialPopup_hpp
#define MSTrialPopup_hpp

#include "cocostudiomacro2.h"

class MSTrialPopup : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MSTrialPopup);
    static MSTrialPopup * createPopup();
    
private:
    void didLoadFromCSB();
};

CREATE_READER(MSTrialPopup);

#endif /* MSTrialPopup_hpp */
