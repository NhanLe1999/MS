//
//  GroupPromotion.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/8/19.
//

#ifndef GroupPromotion_hpp
#define GroupPromotion_hpp

#include "cocostudiomacro2.h"

class GroupPromotion : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    static bool shouldShow();
    static void attempToShow(cocos2d::Node * node = nullptr, std::function<void()> callback = nullptr);
    static GroupPromotion * createView();
    CREATE_FUNC(GroupPromotion);
private:
    void onEnter() override;
    void onExit() override;
    void didLoadFromCSB();
    void onClose(cocos2d::Ref * sender);
    void onJoin(cocos2d::Ref * sender);
    
    std::function<void()> _callback;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
};

CREATE_READER(GroupPromotion);

#endif /* GroupPromotion_hpp */
