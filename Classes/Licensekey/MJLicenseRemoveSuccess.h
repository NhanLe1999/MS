//
//  MJLicenseRemoveSuccess.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJLicenseRemoveSuccess_hpp
#define MJLicenseRemoveSuccess_hpp

#include "cocostudiomacro2.h"
#include "LicenseInfoModel.h"
using namespace cocos2d;
using namespace ui;

class MJLicenseRemoveSuccessDelegate {
public:
    virtual void onAlertRemoveSuccessConfirm() {};
};

class MJLicenseRemoveSuccess : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJLicenseRemoveSuccess);
    static MJLicenseRemoveSuccess * createView(license::LicenseDeviceInfo device_info, int success_type);
    void setDelegate(MJLicenseRemoveSuccessDelegate *delegate);
    
    enum SUCCESS_TYPE {
        REMOVE_CURRENT,
        REMOVE_OTHER,
        ACTIVE_LICENSE
    };
private:
    void didLoadFromCSB(license::LicenseDeviceInfo device_info, int success_type);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJLicenseRemoveSuccessDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onOk(cocos2d::Ref *sender);
    void onShow();
    void onHide();
    bool is_showed = false;
    int success_type;
    license::LicenseDeviceInfo device_info;
};

CREATE_READER(MJLicenseRemoveSuccess);

#endif
