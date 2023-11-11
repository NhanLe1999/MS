//
//  MSForgotLicensePopup.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#ifndef MSForgotLicensePopup_hpp
#define MSForgotLicensePopup_hpp

#include "cocostudiomacro2.h"

namespace mj { namespace api {
    class ForgotLicense;
}}

class MSForgotLicensePopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSForgotLicensePopup);
    static MSForgotLicensePopup * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    
    void onClose(cocos2d::Ref * sender);
    void onOK(cocos2d::Ref * sender);
    
    void startLoading();
    void stopLoading();
    
    mj::api::ForgotLicense * _request = nullptr;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSForgotLicensePopup);

#endif /* MSForgotLicensePopup_hpp */
