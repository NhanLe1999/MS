//
//  MJWarningActiveLicense.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/16/18.
//

#ifndef MJWarningActiveLicense_hpp
#define MJWarningActiveLicense_hpp

#include <stdio.h>

#include "cocostudiomacro2.h"

class MJWarningActiveLicense : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJWarningActiveLicense);
    static MJWarningActiveLicense * createView(const std::string & title, const std::string & message, const bool & one_button, std::function<void()> callback = nullptr);
    
private:
    void didLoadFromCSB(const std::string & title, const std::string & message, const bool & one_button, std::function<void()> callback);
    void onEnter() override;
    
    std::function<void()> _callback = nullptr;
    
    void onOK(Ref * sender);
    void onCancel(Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MJWarningActiveLicense);

#endif /* MJWarningActiveLicense_hpp */
