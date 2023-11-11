//
//  MSIntroLicense.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/28/18.
//

#ifndef MSIntroLicense_hpp
#define MSIntroLicense_hpp

#include "cocostudiomacro2.h"
#include "MJPay.h"
#include "StoryAlert.h"
#include "LicenseInfoModel.h"
#include "MJFullLicenseView.h"
#include "MJOverLicenseView.h"

class MSIntroLicense : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public mj::ActiveLicenseDelegate, public StoryAlertDelegate, public MJFullLicenseViewDelegate, public MJOverLicenseViewDelegate {
public:
    CREATE_FUNC(MSIntroLicense);
    static MSIntroLicense * createView();
    static MSIntroLicense * showPopup();
	void onSend(cocos2d::Ref * sender);
	bool is_unverified_account = false;
private:
    void didLoadFromCSB();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    void onTrigger(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    
    void onForgot(cocos2d::Ref * sender);
    void onCancel(cocos2d::Ref *sender);
	void closePopup();
    void onActiveLicenseSuccess(const std::string & license, const int & time_expire, const std::string & message, rapidjson::Value data) override;
    void onActiveLicenseFailed(const std::string & license, const int & error_code, const std::string & error, rapidjson::Value data) override;
    
    std::string _entered_license;\
    std::string _phone_info, _mail_info, _total_active;
    
    std::vector<license::LicenseDeviceInfo> _list_device_info;
    
    CC_SYNTHESIZE(std::string, _timeline, TimelineName);
	CC_SYNTHESIZE(std::function<void(bool)> , _license_callback, OnLicenseCallback);
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void onFullLicenseViewFreeUse() override;
    void onFullLicenseonRemoveDevice(license::Package) override;
    void onFullLicenseonSubscribe() override;
    void showOverLicenseView(std::string message);
    void onOverLicenseViewFreeUse() override;
    void onOverLicenseViewListDevice() override;
    void onOverLicenseViewSubscribe() override;
    void showGuideRemoveLicense();
    
    void updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success = false);
    void showWarning(const std::string & message);
    void hideWarning();
    bool isRunning = false;
};

CREATE_READER(MSIntroLicense);

#endif /* MSIntroLicense_hpp */
