//
//  ViralReceiveGift.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#ifndef ViralReceiveGift_hpp
#define ViralReceiveGift_hpp

#include "cocostudiomacro2.h"
#include "MJDefault.h"

USING_NS_CC;
using namespace ui ;

class ViralReceiveGift : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(ViralReceiveGift);
    static ViralReceiveGift* createPopup();
    static void show();
private:
    void didLoadFromCSB();
    void onClose(Ref* sender);
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    
    void onReceive(Ref* sender);
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    
    TextField* textField_phone;

    bool is_vn;
};
CREATE_READER(ViralReceiveGift);
#endif /* ViralReceiveGift_hpp */
