//
//  MJLicensePackage.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#ifndef MJLicensePackage_hpp
#define MJLicensePackage_hpp

#include <stdio.h>
#include <string>

namespace license {
    struct Package {
        std::string license = "";
        std::string name = "";
        std::string mail = "";
        std::string phone = "";
        std::string product_id = "";
        int time_enter = -1;
        int time_effective = -1;
        int time_expiry = -1;
        int total_active = 0;
        int current_active = 0;
        
        static std::string getSQLStructure();
        static std::string getSQLValues();
    };
}

#endif /* MJLicensePackage_hpp */
