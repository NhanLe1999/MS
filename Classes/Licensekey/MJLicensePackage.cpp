//
//  MJLicensePackage.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#include "MJLicensePackage.h"

namespace license {
    std::string Package::getSQLValues() {
        return "(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    }
    
    std::string Package::getSQLStructure() {
        return "(license, product_id, name, mail, phone, date_enter, date_effect, date_expire, total_active, current_active)";
    }
}
