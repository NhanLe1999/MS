//
//  GameBuildBridge_Mask.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/6/18.
//

#ifndef GameBuildBridge_Mask_hpp
#define GameBuildBridge_Mask_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameBuildBridge_Mask : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(GameBuildBridge_Mask);
    static GameBuildBridge_Mask * createMask();

    void focus(double duration);
    void unFocus(double duration);
private:
    bool init() override;
    
    cocos2d::LayerRadialGradient * _mask;
    double _mask_size;
};

#endif /* GameBuildBridge_Mask_hpp */
