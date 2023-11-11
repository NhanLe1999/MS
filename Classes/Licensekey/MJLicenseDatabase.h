//
//  MJLicenseDatabase.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#ifndef MJLicenseDatabase_hpp
#define MJLicenseDatabase_hpp

#include <stdio.h>
#include <string>

namespace license {
    class Storage {
    public:
        static Storage & getInstance();
        
        std::string getEffectiveLicense();
        int getTimeExpiry();
        
    protected:
        explicit Storage();
        
    private:
        Storage(Storage const &);
        void operator = (Storage const &);
    };
}

#endif /* MJLicenseDatabase_hpp */
