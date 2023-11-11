//
//  PayInit.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/2/19.
//

#ifndef PayInit_hpp
#define PayInit_hpp

#include "cocostudiomacro2.h"

class PayInit : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(PayInit);
    static PayInit * createView(std::function<void()> pay_callback);
    
private:
    void didLoadFromCSB(std::function<void()> pay_callback);
    void onEnter() override;
    void onExit() override;
    void onCall(cocos2d::Ref * sender);
    void onView(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    void onBuy(cocos2d::Ref * sender);
    
    std::function<void()> _callback;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(PayInit);

#endif /* PayInit_hpp */
