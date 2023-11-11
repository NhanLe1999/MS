//
//  MSFeaturedStories.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef MSFeaturedStories_hpp
#define MSFeaturedStories_hpp

#include "cocostudiomacro2.h"

class MSFeaturedStories : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSFeaturedStories);
    static MSFeaturedStories * createView();
    void setContentSize(const cocos2d::Size & size) override;
    
private:
    void didLoadFromCSB();
    void updateFeatured(float dt);
    
    void moveForward();
    void moveBackward();
    
    std::vector<cocos2d::Node *> _nodes;
    cocos2d::ui::Layout * _pages;
    
    cocos2d::ui::Button * _bt_forward, * _bt_backward;
    cocos2d::ui::ListView * _indicators;
    
    int _current_idx;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    
    void onForward(cocos2d::Ref * sender);
    void onBackward(cocos2d::Ref * sender);
};

CREATE_READER(MSFeaturedStories);
#endif /* MSFeaturedStories_hpp */
