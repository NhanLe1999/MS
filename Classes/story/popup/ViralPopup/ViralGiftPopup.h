//
//  ViralGiftPopup.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/30/18.
//

#ifndef ViralGiftPopup_hpp
#define ViralGiftPopup_hpp

#include "cocostudiomacro2.h"
USING_NS_CC;
using namespace ui;

class ViralGiftPopup: public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(ViralGiftPopup);
    static ViralGiftPopup* createPopup();
    static void show();
private:
    void didLoadFromCSB();
    void onOK(Ref* sender);
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    
    
};
CREATE_READER(ViralGiftPopup);
#endif /* ViralGiftPopup_hpp */
