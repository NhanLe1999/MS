//
//  MJWarningRemoveView.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJWarningRemoveView_hpp
#define MJWarningRemoveView_hpp

#include "cocostudiomacro2.h"
#include "LicenseInfoModel.h"
#include "MJLicenseAlert.h"
using namespace cocos2d;
using namespace ui;

class MJWarningRemoveViewDelegate {
public:
    virtual void onRemoveDeviceLicenseSuccess() {};
};

class MJWarningRemoveView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJWarningRemoveView);
    static MJWarningRemoveView * createView(license::LicenseDeviceInfo device_info, std::string license_key, int current_device_id);
    void setDelegate(MJWarningRemoveViewDelegate *delegate);
private:
    void didLoadFromCSB(license::LicenseDeviceInfo device_info, std::string license_key, int current_device_id);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJWarningRemoveViewDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    void onOk(cocos2d::Ref *sender);
    license::LicenseDeviceInfo device_info;
    std::string license_key;
    int current_device_id;
    void onShow();
    void onHide();
    bool is_showed = false;
    
};

CREATE_READER(MJWarningRemoveView);

#endif
