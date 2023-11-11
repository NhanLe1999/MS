//
//  LicenseInfoModel.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef __LicenseInfoModel__
#define __LicenseInfoModel__
#define LICENSE_INFO_MODEL license::LicenseInfoModel::getInstance()

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
#include <stdio.h>
#include <vector>
#include <string>
#include "../external/json/document.h"
#include "MJLicensePackage.h"

namespace license {
    
    struct LicenseDeviceInfo {
        int device_id = 0;
        std::string device_name = "";
        int device_status = 0;
        int active_date = 0;
        int deactive_date = 0;
        
        LicenseDeviceInfo() : device_id(0), device_name(""), device_status(0), active_date(0), deactive_date(0) {}
        LicenseDeviceInfo(rapidjson::Value d);
        LicenseDeviceInfo(cocos2d::ValueMap data);
        static std::string getSQLStructure();
        std::string getSQLValues();
    };
    
    class LicenseInfoModel {
    public:
        static LicenseInfoModel * getInstance();
        static LicenseInfoModel * __instance;
        void removeDeviceIDFromListLicenseDevice(int device_id_remove);
        bool isDisplayLanguageVN();
        void restartApp();
        void deleteActiveApp();
        void forceDismissKeyboard();
        void onSubscribe();
        void onFreeUse();
        void updateLicenseDeviceInfo(std::vector<license::LicenseDeviceInfo> list_licensedeviceinfo);
        std::vector<license::LicenseDeviceInfo> getLicenseDeviceInfo();
        int getDeviceID();
        void setInfoVerify(std::string phone, std::string mail);
        std::string phone_info, mail_info;
        
        Package getCurrentLicense();
        Package getLicenseByLicenseKey(const std::string & license_key);
    private:
        
    };
}

#endif
