//
//  ViralPopup.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/28/18.
//

#ifndef ViralPopup_hpp
#define ViralPopup_hpp

#include "cocostudiomacro2.h"
#include "ViralPopup_Item.h"
#include "MJDefault.h"

USING_NS_CC;
using namespace ui ;

class ViralPopup : public cocos2d::ui::Layout, public ViralPopup_ItemDelegate, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(ViralPopup);
    static ViralPopup* createPopup(bool from_popup);
    static void show(bool from_popup = true);
private:
    void didLoadFromCSB(bool from_popup);
    void onClose(Ref* sender);
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    void onBtnShare(Ref* sender) override;
    void onSend(Ref* sender);
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    
    TextField* textField_name;

    bool is_vn = false;
    bool touch = true;
};
CREATE_READER(ViralPopup);
#endif /* ViralPopup_hpp */
