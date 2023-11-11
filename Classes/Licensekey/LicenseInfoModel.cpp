//
//  LicenseInfoModel.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "LicenseInfoModel.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "NativeBridge.h"
#include "StoryDataModel.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "MSIntroGrade.h"
#include "MJDefinitions.h"
namespace license {
    LicenseDeviceInfo::LicenseDeviceInfo(rapidjson::Value d)  {
        assert(d.IsObject());
        auto data = d.GetObject();
        
        if (JSON_SAFE_CHECK(data, "device_id", Int)) {
            this->device_id = data["device_id"].GetInt();
        }
        
        if (JSON_SAFE_CHECK(data, "device_name", String)) {
            this->device_name = data["device_name"].GetString();
        }
        
        if (JSON_SAFE_CHECK(data, "device_status", Int)) {
            this->device_status = data["device_status"].GetInt();
        }
        
        if (JSON_SAFE_CHECK(data, "active_date", Int)) {
            this->active_date = data["active_date"].GetInt();
        }
        
        if (JSON_SAFE_CHECK(data, "deactive_date", Int)) {
            this->deactive_date = data["deactive_date"].GetInt();
        }
    }
    
    LicenseDeviceInfo::LicenseDeviceInfo(cocos2d::ValueMap data)  {
        this->device_id = data["device_id"].asInt();
        this->device_name = data["device_name"].asString();
        this->device_status = data["device_status"].asInt();
        this->active_date = data["active_date"].asInt();
        this->deactive_date = data["deactive_date"].asInt();
    }
    
    std::string LicenseDeviceInfo::getSQLStructure() {
        return "(device_id, device_name, device_status, active_date, deactive_date)";
    }
    
    std::string LicenseDeviceInfo::getSQLValues() {
        return cocos2d::StringUtils::format("(%d,\"%s\",\"%d\",%d,%d)",
                                            this->device_id,
                                            this->device_name.c_str(),
                                            this->device_status,
                                            this->active_date,
                                            this->deactive_date);
    }
    
    LicenseInfoModel * LicenseInfoModel::__instance = NULL;
    LicenseInfoModel * LicenseInfoModel::getInstance() {
        if (!__instance) {
            __instance = new LicenseInfoModel();
        }
        return __instance;
    }
    
    void LicenseInfoModel::removeDeviceIDFromListLicenseDevice(int device_id_remove){
//        DataSQL::getInstance()->removeDeviceIDFromListLicenseDevice(device_id_remove);
    }

    bool LicenseInfoModel::isDisplayLanguageVN(){
        return LANGUAGE_MANAGER->getDisplayLangId()==4;
    }
    
    void LicenseInfoModel::restartApp(){
        FileUtils::getInstance()->purgeCachedEntries();
        MJDEFAULT->setBoolForKey(key_first_api_called, true);
        Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25, MSIntroGrade::createScene()));
    }
    
    void LicenseInfoModel::deleteActiveApp(){
        MJDEFAULT->setIntegerForKey("delete_app_step", 2);
        MJDEFAULT->setIntegerForKey("key_sub_time_expire", 0);
//        DataSQL::getInstance()->deleteAllInAppPurchase();
    }
    
    void LicenseInfoModel::forceDismissKeyboard(){
        mj::PlatformConfig::getInstance().forceDismissKeyboard();
    }
    void LicenseInfoModel::onSubscribe(){
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_subscribe_page);
    }
    
    void LicenseInfoModel::onFreeUse(){
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_free_page);
    }
    
    void LicenseInfoModel::updateLicenseDeviceInfo(std::vector<license::LicenseDeviceInfo> list_licensedeviceinfo){
        StoryDataModel::getInstance()->updateLicenseDeviceInfo(list_licensedeviceinfo);
    }
    
    std::vector<license::LicenseDeviceInfo> LicenseInfoModel::getLicenseDeviceInfo(){
        return StoryDataModel::getInstance()->getLicenseDeviceInfo();
    }
    

    int LicenseInfoModel::getDeviceID(){
        return atoi(mj::PlatformConfig::getInstance().getDeviceId().c_str());
    }
    
    void LicenseInfoModel::setInfoVerify(std::string phone, std::string mail){
        phone_info = phone;
        mail_info = mail;
    }
    
    Package LicenseInfoModel::getCurrentLicense() {
        auto packages = StoryDataModel::getInstance()->getLicensePackages();
        
        if (!packages.empty()) {
            packages.erase(std::remove_if(packages.begin(), packages.end(), [](license::Package l) {
                return (l.time_expiry > 0) && (l.time_expiry < mj::helper::getTime());
            }), packages.end());
        }
        
        if (packages.empty()) {
            Package n;
            n.license = "";
            return n;
        }
        return packages.front();
    }
    
    Package LicenseInfoModel::getLicenseByLicenseKey(const std::string & license_key) {
        auto p = StoryDataModel::getInstance()->getLicenseByLicenseKey(license_key);
        return p;
    }
}

