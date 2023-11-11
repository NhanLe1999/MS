//
//  MSInstallPromotion.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/21/19.
//

#ifndef MSInstallPromotion_hpp
#define MSInstallPromotion_hpp

#include "cocostudiomacro2.h"

class MSInstallPromotion : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSInstallPromotion);
    static MSInstallPromotion * createView(bool is_popup = false);
    static bool shouldShowPopup();
    static bool shouldShowNotification();
private:
    void didLoadFromCSB(bool is_popup);
    void onEnter() override;
    void onExit() override;
    void onClose(cocos2d::Ref * sender);
    void onReceive(cocos2d::Ref * sender);
    
    bool _is_popup = false;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSInstallPromotion);

#endif /* MSInstallPromotion_hpp */
