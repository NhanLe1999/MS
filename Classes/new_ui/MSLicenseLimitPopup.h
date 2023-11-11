//
//  MSLicenseLimitPopup.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#ifndef MSLicenseLimitPopup_hpp
#define MSLicenseLimitPopup_hpp

#include "cocostudiomacro2.h"

class MSLicenseLimitPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSLicenseLimitPopup);
    static MSLicenseLimitPopup * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    
    void onOK(cocos2d::Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSLicenseLimitPopup);

#endif /* MSLicenseLimitPopup_hpp */
