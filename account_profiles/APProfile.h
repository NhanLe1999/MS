//
//  APProfile.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/25/19.
//

#ifndef APProfile_hpp
#define APProfile_hpp

#include <string>

namespace ap {
    class Profile {
    public:
        enum Gender {
            MALE = 1,
            FEMALE = 2
        };
        
        int profile_id = 0;
        std::string name;
        Gender gender = MALE;
        int age = 0;
        std::string description;
		std::string avatar;
        int getGenderCode() const {
            if (gender == FEMALE) {
                return 2;
            }
            
            return 1;
        }
    };

	struct UserInfo
	{
		int _id;
		std::string _name;
		std::string _mail;
		std::string _phone;
		std::string _address;
		std::string _config_item = "";
		std::string _userName = "";//the identity for login.
	};
}

#endif /* APProfile_hpp */
