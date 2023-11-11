//
//  StoryParentLicense_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/12/17.
//
//

#ifndef StoryParentLicense_Landscape_hpp
#define StoryParentLicense_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"
#include "MJPay.h"
#include "StoryAlert.h"
#include "LicenseInfoModel.h"
#include "MJFullLicenseView.h"
#include "MJOverLicenseView.h"

namespace license { struct Package; }

class StoryParentLicense_Landscape: public StoryParentSubPage_Landscape, public mj::ActiveLicenseDelegate, public StoryAlertDelegate,  public MJFullLicenseViewDelegate, public MJOverLicenseViewDelegate {
public:
    CREATE_FUNC(StoryParentLicense_Landscape);
    static StoryParentLicense_Landscape * createPage();
    void onShow();
    void onOK(cocos2d::Ref * sender);
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
//    cocos2d::ui::Widget::ccWidgetEventCallback onLocateEventCallback(const std::string &callback_name) override;
    void onBack(Ref *sender);
    void changeDisplayLanguage();
    bool isBack = false;
//    cocos2d::ui::TextField *textfield_license;

//    ImageView *background;
//    void onTextfieldChanged(cocos2d::Ref *pSender, int event);
//    Vec2 pos_bottom, pos_top;
protected:
    void didLoadFromCSB() override;
    void onEnter() override;

    void onDeactivate(cocos2d::Ref * sender);
    void onForgotLicense(cocos2d::Ref * sender);
    void onSubScribe(cocos2d::Ref * sender);
    void onActiveLicenseSuccess(const std::string & license, const int & time_expire, const std::string & message, rapidjson::Value data) override;
    void onActiveLicenseFailed(const std::string & license, const int & error_code, const std::string & error, rapidjson::Value data) override;
              
    void startLoading();
    void endLoading();
    
        
    mj::Request * _request = nullptr;
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    void updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success = false);
    std::string phone_info, mail_info, total_active;
    std::vector<license::LicenseDeviceInfo> _list_device_info;
    bool is_vn;
    void onFullLicenseViewFreeUse() override;
    void onFullLicenseonRemoveDevice(license::Package) override;
    void showOverLicenseView(std::string message);
    void onOverLicenseViewFreeUse() override;
    void onOverLicenseViewListDevice() override;
    void showGuideRemoveLicense(license::Package license);
    std::string str_licence;
    
    std::string _entered_license;
    std::string _phone_info, _mail_info, _total_active;
};

CREATE_READER_CLASS(StoryParentLicense_LandscapeReader, StoryParentLicense_Landscape);

#endif /* StoryParentLicense_Landscape_hpp */
