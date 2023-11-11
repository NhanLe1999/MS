#ifndef F2PCommon_hpp
#define F2PCommon_hpp
#include "IAPManager.h"
#include "APProfile.h"
namespace ms {
    namespace f2p {
        enum class FreeContentOption {
            MONO_ITEM_ONE_DAY_ONLY_STORY = 0,
            MONO_ITEM_ONE_DAY_RADOM = 1,
            MULTILE_ITEM_ONE_DAY = 2,
            TEN_ITEM_FREE = 3,
            DEFAULT =4,
        };
        enum class MergeUserType {
            FREE_USER = 0,
            PAID_USER = 1,
            MERGED_USER = 2,
        };
        struct F2PAccountInfoForMerge {
            int _userId = 0;
            std::string _token = "";
            std::vector<InAppPackageModel> _inAppPackageModel = {};
            std::vector<ap::Profile>_profileList = {};
            int _maxProfile = 3;
            ap::UserInfo _userInfo = ap::UserInfo();
            F2PAccountInfoForMerge() {};
            F2PAccountInfoForMerge(std::string userId , std::string token) :  _token(token){
                _inAppPackageModel = {};
                _profileList = {};
                _userInfo = ap::UserInfo();
                _maxProfile = 3;
                try {
                    // string -> integer
                    _userId = std::stoi(userId);
                }
                catch (...) {
                    // error management
                    _userId = 0;
                }
            }
            F2PAccountInfoForMerge(int userId, std::string token) : _userId(userId),_token(token) {
                _inAppPackageModel = {};
                _profileList = {};
                _userInfo = ap::UserInfo();
                _maxProfile = 3;
            }
        };
    };
    namespace MsTypes {
        enum class DocumentsType {
            STORY,
            CATEGORY,
            AUDIOBOOK_CATEGORY,
            AUDIOBOOK,
            LESSON,
            PHONICS,
            READING_COMPREHENTION,
            AI_SPEAKING
        };
    };

}
#endif // defined(F2PCommon_hpp)
