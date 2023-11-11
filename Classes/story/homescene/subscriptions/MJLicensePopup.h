//
//  MJLicensePopup.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/1/17.
//
//

#ifndef MJLicensePopup_hpp
#define MJLicensePopup_hpp

#include "CocoStudioMacro.h"
#include "MJRequest.h"
#include "MJHelper.h"

class MJLicensePopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public mj::RequestDelegate {
public:
    CREATE_FUNC(MJLicensePopup);
    static MJLicensePopup * createPopup();

private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name);

    void onBack(cocos2d::Ref * sender);
    void onNext(cocos2d::Ref * sender);

    mj::Request * _request = nullptr;

    void onRequestSuccess(int tag, rapidjson::Value data);
    void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data);
};

CREATE_READER_CLASS(MJLicensePopupReader, MJLicensePopup);

#endif /* MJLicensePopup_hpp */
