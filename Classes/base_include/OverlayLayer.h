//
//  LessonBase.h
//  Demo
//
//  Created by Ha Anh Man on 3/9/16.
//
//

#ifndef __OverlayLayer__LessonBase__
#define __OverlayLayer__LessonBase__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
class OverlayLayer : public LayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(OverlayLayer);
    cocos2d::Size sceneSize;
    float scaleXY = 0;
    cocos2d::ui::Layout *panel = NULL;
};

#endif /* defined(__OverlayLayer__LessonBase__) */
