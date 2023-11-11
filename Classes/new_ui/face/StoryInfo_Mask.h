//
//  StoryInfo_Mask.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/14/19.
//

#ifndef StoryInfo_Mask_hpp
#define StoryInfo_Mask_hpp

#include "cocostudiomacro2.h"

class StoryInfo_Mask : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(StoryInfo_Mask);
    static StoryInfo_Mask * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    void onBack(cocos2d::Ref * sender);
    void onDone(cocos2d::Ref * sender);
    void onTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void onZoomIn(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void onZoomOut(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    
    cocos2d::Sprite * _sprite;
    cocos2d::Point _original_position;
    cocos2d::Rect _mask_bounding_box;
    
    void adjustFaceInBox();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string & name) override;
    
    float _scale = 1;
};

CREATE_READER(StoryInfo_Mask);

#endif /* StoryInfo_Mask_hpp */
