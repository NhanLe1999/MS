//
//  MJLicenseActiveSuccess.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 11/29/18.
//

#ifndef MJLicenseActiveSuccess_hpp
#define MJLicenseActiveSuccess_hpp

#include "cocostudiomacro2.h"

class MJLicenseActiveSuccess : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJLicenseActiveSuccess);
    static MJLicenseActiveSuccess * createView(std::string message, std::function<void()> exit_callback = nullptr);
    
private:
    void didLoadFromCSB(std::string message, std::function<void()> exit_callback);
    void onEnter() override;
    
    void onClose(cocos2d::Ref * sender);
    void onManage(cocos2d::Ref * sender);
    void onJoin(cocos2d::Ref * sender);
    
    std::function<void()> _exit_callback = nullptr;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MJLicenseActiveSuccess);

#endif /* MJLicenseActiveSuccess_hpp */
