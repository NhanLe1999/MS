//
//  MJWaitingHUD.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/21/17.
//

#ifndef MJWaitingHUD_hpp
#define MJWaitingHUD_hpp

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"

class MJWaitingHUD : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJWaitingHUD);
    static void show(cocos2d::Node * blocked);
    static void hide(cocos2d::Node * blocked = nullptr);
    
private:
    static MJWaitingHUD * createHUD();
};

CREATE_READER_CLASS(MJWaitingHUDReader, MJWaitingHUD);

#endif /* MJWaitingHUD_hpp */
