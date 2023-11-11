//
//  MJListDeviceView.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJListDeviceView_hpp
#define MJListDeviceView_hpp

#include "cocostudiomacro2.h"
using namespace cocos2d;

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "MacCppBridge.h"
#endif
#include "MJListDeviceView_Cell.h"
#include "LicenseInfoModel.h"
#include "MJLicenseAlert.h"
#include "MJVerifyInfoView.h"
#include "MJWarningRemoveView.h"
#include "MJLicenseRemoveSuccess.h"
#include "MJLicensePackage.h"

class MJListDeviceViewDelegate {
public:
    virtual void onMJListDeviceViewNext() {};
};

class MJListDeviceView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJListDeviceView_CellDelegate, public MJLicenseAlertDelegate, public MJVerifyInfoViewDelegate, public MJWarningRemoveViewDelegate, public MJLicenseRemoveSuccessDelegate  {
public:
    CREATE_FUNC(MJListDeviceView);
    static MJListDeviceView * createView(std::vector<license::LicenseDeviceInfo> list_device_info, int deviceid_current, license::Package license, std::string total_active, bool is_active_license = false, bool show_btremove = true);
    void setDelegate(MJListDeviceViewDelegate *delegate);
    void onShow();
    void onHide();
private:
    void didLoadFromCSB(std::vector<license::LicenseDeviceInfo> list_device_info, int deviceid_current, license::Package license, std::string total_active, bool is_active_license, bool show_btremove);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJListDeviceViewDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(cocos2d::Node *parent, std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    std::vector<license::LicenseDeviceInfo> list_device_info;
    int deviceid_current = 0;
    license::Package _license;
    std::string info_user;
    std::string total_active;
    license::LicenseDeviceInfo current_device_info, choose_device_info;
    void setDisplayCurrentDevice(license::LicenseDeviceInfo device_info);
    void onDeActiveDeviceCell(license::LicenseDeviceInfo device_info) override;
    void onRemove(cocos2d::Ref *sender);
    void onMJAlertClicked(bool is_ok, int tag) override;
    void showVerifyInfoView();
    void onVerifySuccess() override;
    void showWarningRemoveView();
    void onRemoveDeviceLicenseSuccess() override;
    bool is_active_license;
    void showAlertLicenseRemoveSuccess();
    bool show_btremove;
    void activeLicenseWhenRemoveOtherDeviceSuccess();
    void onAlertRemoveSuccessConfirm() override;
    void updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success);
};

CREATE_READER(MJListDeviceView);

#endif
