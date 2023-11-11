//
//  MJLicenseDatabase.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#include "MJLicenseDatabase.h"
#include "MJDefault.h"

namespace license {
    Storage & Storage::getInstance() {
        static Storage shared_storage;
        return shared_storage;
    }
    
    Storage::Storage() {
        
    }
    
    std::string Storage::getEffectiveLicense() {
        return MJDEFAULT->getStringForKey("license_key", "");
    }
    
    int Storage::getTimeExpiry() {
        return MJDEFAULT->getIntegerForKey("key_sub_time_expire", 0);
    }
}
