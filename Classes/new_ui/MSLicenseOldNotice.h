//
//  MSLicenseOldNotice.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/31/18.
//

#ifndef MSLicenseOldNotice_hpp
#define MSLicenseOldNotice_hpp

#include "cocostudiomacro2.h"

namespace mj { namespace api {
    class ChangeLicense;
}}

class MSLicenseOldNotice : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSLicenseOldNotice);
    static MSLicenseOldNotice * createView(std::string mail, std::string phone, std::string license);
    
private:
    void didLoadFromCSB(std::string mail, std::string phone, std::string license);
    void onEnter() override;
    
    void onClose(cocos2d::Ref * sender);
    void onMail(cocos2d::Ref * sender);
    void onSMS(cocos2d::Ref * sender);
    
    void showWait();
    void hideWait();
    
    std::string _license;
    
    mj::api::ChangeLicense * _api;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSLicenseOldNotice);

#endif /* MSLicenseOldNotice_hpp */
