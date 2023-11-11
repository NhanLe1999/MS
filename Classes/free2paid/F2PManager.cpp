#include "F2PManager.h"
#include "StoryLanguageManager.h"
#include "popup/RemindingUpdateInfoPopup.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MJCalculator.h"
#include "StoryConfigManager.h"
#include "APPurchaseManager.h"
#include "StoryDataModel.h"
#include "CleverTapManager.h"

USING_NS_CC;

void F2PManager::clearDataFreeToPaid() {
    
    MJDEFAULT->deleteValueForKey("f2p_first_login_skip");
    MJDEFAULT->deleteValueForKey("f2p_profile_merge_max_profile");
    MJDEFAULT->deleteValueForKey("f2p_profile_merge_current_profile");
    MJDEFAULT->deleteValueForKey(key_f2p_paid);
    MJDEFAULT->deleteValueForKey(key_f2p_free);
    MJDEFAULT->deleteValueForKey(key_f2p_account_free_info_for_merge);
    MJDEFAULT->deleteValueForKey(key_f2p_account_paid_info_for_merge);
    MJDEFAULT->deleteValueForKey("f2p_is_megre_skip_account");
    MJDEFAULT->deleteValueForKey("f2p_is_merging");
    //MJDEFAULT->deleteValueForKey("setKeyGotoF2PGetStarted");
}

void F2PManager::setKeyGotoF2PGetStarted(bool value)
{
    MJDEFAULT->setBoolForKey("setKeyGotoF2PGetStarted", value);
}

bool F2PManager::getKeyGotoF2PGetStarted()
{
    return MJDEFAULT->getBoolForKey("setKeyGotoF2PGetStarted", false);
}
void F2PManager::setKeyGotoF2PSplash(bool value)
{
    /*key này phải được giữ trong suốt quát trình cài app, không để reset cho đến khi app bị gỡ hoac xoa app data tu he dieu hanh, hoac thuc hien merge thanh cong
     */
    MJDEFAULT->setBoolForKey("KeyGotoF2PSplash", value);
}

bool F2PManager::getKeyGotoF2PSplash()
{
    return MJDEFAULT->getBoolForKey("KeyGotoF2PSplash", false);
}

bool F2PManager::isNeedGotoCheat()
{
    /* kiem tra xem user co phai lan dau tao user skip khong, neu da tung tao thi se vao luong chong cheat, neu la lan dau tao thi se vao luong f2p
     key KeyGotoF2PSplash chỉ set true khi lan dau vao vao page Splash sau khi dang nhap, key nay = false, nghĩa là user  chưa vào f2p bao giờ. Lúc này có 2 trường hợp.
     TH1: User chua vào tới splash và chưa gỡ app: khi đó key isFirstLoginSkip = true và key KeyGotoF2PSplash = false
     TH2: User da vào tới splash và chưa gỡ app: khi đó key isFirstLoginSkip = true và key KeyGotoF2PSplash = true
     TH3: user đã vào/ chua vao toi splash  -> gỡ app ra -> cài lại: Khi đó isFirstLoginSkip = false và KeyGotoF2PSplash = false
     TH4: user đã merge hoac mua xong, can ket thuc luong f2p: Khi đó isFirstLoginSkip = false và KeyGotoF2PSplash = false
     key này phải được giữ trong suốt quát trình cài app, không để  reset cho đến khi app bị gỡ
     key isKeyFirstLoginSkip = true chi khi user chua bao gio call loginskip,
     */
    bool isKeyGotoF2PSplash = getKeyGotoF2PSplash();
    bool isKeyFirstLoginSkip = isFirstLoginSkip();
    bool isUserSkip = F2P_MANAGER->isSkipUser();
    bool isMergedAccount = F2P_MANAGER->isMegredAccount();
    if(isKeyFirstLoginSkip) // truong hop lan dau tien vao
        return false;
    if(isMergedAccount)// neu da tung merge account f2p thanh cong thi vao luong chong cheat
        return true;
    else if(!isKeyFirstLoginSkip && isKeyGotoF2PSplash) // truong hop khong go app
    {
        if(!isUserSkip)  // user khong phai la skip, thi vao luong chong cheat
            return true;
        else
            return false;
    }
    else if(!isKeyFirstLoginSkip && !isKeyGotoF2PSplash) // truong hop go app
        return true;
    else  // tat ca truong hop con lai deu vao cheat
        return true;
    
}
/*
#define key_f2p_paid "key_f2p_paid"
#define key_f2p_free "key_f2p_paid"
*/
void F2PManager::setKeyF2P_Paid(bool value)
{
    /* set true neu user vao f2p va mua*/
    /* set false neu nguoc lai*/
    MJDEFAULT->setBoolForKey(key_f2p_paid, value);
}

bool F2PManager::getKeyF2P_Paid()
{
    bool isPaid = false;
    std::vector<std::string> skipUserInfoFormerge = f2PGetUserInfo(true);
    if(skipUserInfoFormerge.size()>=SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE)
    {
        monkey::iap::PRODUCT_PACKAGES newUserProductPackage = monkey::iap::PRODUCT_PACKAGES::FREE;
        int newPackage = std::stoi(skipUserInfoFormerge.at(2));
        bool skipUserIsSkip = skipUserInfoFormerge.at(3) != "0" ? true : false;
        newUserProductPackage = (monkey::iap::PRODUCT_PACKAGES)newPackage;
        if(skipUserIsSkip && newUserProductPackage!= monkey::iap::PRODUCT_PACKAGES::FREE)
            isPaid = true;
    }
    
    return isPaid || MJDEFAULT->getBoolForKey(key_f2p_paid, false);
}

void F2PManager::deleteKeyF2P_Paid()
{
    MJDEFAULT->deleteValueForKey(key_f2p_paid);
}

void F2PManager::setKeyF2P_Free(bool value)
{
    /* set true neu user vao f2p va chua qua man merge */
    /* set false neu nguoc lai*/
    MJDEFAULT->setBoolForKey(key_f2p_free, value);

}

bool F2PManager::getKeyF2P_Free()
{
    return MJDEFAULT->getBoolForKey(key_f2p_free, false);
}

void F2PManager::deletekeyF2P_Free()
{
    MJDEFAULT->deleteValueForKey(key_f2p_free);
}

void F2PManager::setSkipUser(bool value) {
    
    MJDEFAULT->setBoolForKey("f2p_is_skip_user",value);
}

bool F2PManager::isSkipUser() {
    return APPURCHASE_MANAGER->isSkipUser();
}
void F2PManager::setFirstLoginSkip(bool value) {
    
    MJDEFAULT->setBoolForKey("f2p_first_login_skip",value);
}

bool F2PManager::isFirstLoginSkip() {

    return MJDEFAULT->getBoolForKey("f2p_first_login_skip", false);
}

bool F2PManager::isMergingAccount() {
    
    return MJDEFAULT->getBoolForKey("f2p_is_merging",false);
}

void F2PManager::setMergingAccount(bool value) {
    
    MJDEFAULT->setBoolForKey("f2p_is_merging",value);
}

bool F2PManager::isMegreSkipAccount() {
    
    return MJDEFAULT->getBoolForKey("f2p_is_megre_skip_account",false);
}

void F2PManager::setMegreSkipAccount(bool value) {
    
    MJDEFAULT->setBoolForKey("f2p_is_megre_skip_account",value);
}

bool F2PManager::isMegredAccount() {

    return MJDEFAULT->getBoolForKey("f2p_is_megred_account", false);
}

void F2PManager::setMegredAccount(bool value) {

    // key danh dau da trai qua merge account skip, se khong vao lai luong megre nua neu da merge
    MJDEFAULT->setBoolForKey("f2p_is_megred_account", value);
}

void F2PManager::setUserIdMerged(int userId)
{
    
        MJDEFAULT->setIntegerForKey(key_userId_merged, userId);
}

int F2PManager::getUserIdMerged()
{
    return MJDEFAULT->getIntegerForKey(key_userId_merged, 0);
}
void F2PManager::setIsFreeContentPage(bool isFreeContentPage)
{

    MJDEFAULT->setBoolForKey(key_on_free_content_page, isFreeContentPage);
}

bool F2PManager::isFreeContentPage()
{
    return MJDEFAULT->getBoolForKey(key_on_free_content_page, false);
}
void F2PManager::saveMergedUserInfo(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges)
{
    int userIdMerged = getUserIdMerged();
    for (auto f2PAccountInfo : f2PAccountInfoForMerges) {
        if (f2PAccountInfo._userId == userIdMerged) // neu userId khong trung voi useId duoc save trong Database, can save lai va call loadUpdate
        {
            Database::getInstance().saveIdentity(std::to_string(userIdMerged), f2PAccountInfo._token);
            APPURCHASE_MANAGER->reset();
            APPURCHASE_MANAGER->setPurchasedPackage(f2PAccountInfo._inAppPackageModel);
            break;
        }
    }
}

void F2PManager::setProfileMergeMaxProfile(int maxProfile) {
    
    MJDEFAULT->setIntegerForKey("f2p_profile_merge_max_profile",maxProfile);
}

int F2PManager::getProfileMergeMaxProfile() {
    int macProfile = MJDEFAULT->getIntegerForKey("f2p_profile_merge_max_profile", 0);
    if (macProfile <= 0)
        macProfile = 3;
    return macProfile;
}

void F2PManager::setProfileMergeCurrentProfile(int currentProfile) {
    
    MJDEFAULT->setBoolForKey("f2p_profile_merge_current_profile",currentProfile);
}

int F2PManager::getProfileMergeCurrentProfile() {
    
    return MJDEFAULT->getIntegerForKey("f2p_profile_merge_current_profile",0);

}

bool F2PManager::isCanOpenMergeProfilePage()
{
    bool isCheck = false;
    std::vector<std::string> skipUserInfoFormerge = f2PGetUserInfo(true);
    std::vector<std::string> oldUserInfoFormerge = f2PGetUserInfo(false);
    
    if (skipUserInfoFormerge.size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE || oldUserInfoFormerge.size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE)
        return false;
    if(F2P_MANAGER->isMergingAccount())
        return true;
    if(skipUserInfoFormerge.at(0) == oldUserInfoFormerge.at(0)) // khong merge cung 1 account
         return false;
    if (F2P_MANAGER->isMegredAccount())
        return false;// neu da  tung merge, thi se khong merge nua
    // khong merge neu ca 2 acocunt deu da co tai khoan (account co sdt va pass)
    bool skipUserHaveAccount = skipUserInfoFormerge.at(4) != "None" ? true : false;
    bool oldUserHaveAccount = oldUserInfoFormerge.at(4) != "None" ? true : false;
    
    if(skipUserHaveAccount && oldUserHaveAccount)
    {
        return false;
    }
    
    // khong merge neu 1 trong 2 acocunt ko phai la skip user
    bool skipUserIsSkip = skipUserInfoFormerge.at(3) != "0" ? true : false;
    bool oldUserIsSkip = oldUserInfoFormerge.at(3) != "0" ? true : false;
    //newUserIsSkip = true;// dum dieu kien de vao luong merge
    if(!skipUserIsSkip && !oldUserIsSkip)
        isCheck = false;
    else
    {
        isCheck = true;

    }

    return isCheck;
}

bool F2PManager::isCanMergeProfile()
{
    bool isCheck = false;
    std::vector<std::string> oldUserInfoFormerge = f2PGetUserInfo(true);
    std::vector<std::string> newUserInfoFormerge = f2PGetUserInfo(false);
    if (oldUserInfoFormerge.size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE || newUserInfoFormerge.size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE)
        return false;

    
    if(oldUserInfoFormerge.at(0) == newUserInfoFormerge.at(0)) // khong merge cung 1 account
         return false;
    // khong merge neu ca 2 acocunt deu da co tai khoan (account co sdt va pass)
    bool oldUserHaveAccount = oldUserInfoFormerge.at(4) != "None" ? true : false;
    bool newUserHaveAccount = newUserInfoFormerge.at(4) != "None" ? true : false;
    
    if(oldUserHaveAccount && newUserHaveAccount)
    {
        return false;
    }
    
    // khong merge neu 1 trong 2 acocunt ko phai la free user
    bool oldUserIsSkip = oldUserInfoFormerge.at(3) != "0" ? true : false;
    bool newUserIsSkip = newUserInfoFormerge.at(3) != "0" ? true : false;

    if(!oldUserIsSkip && !newUserIsSkip)
        return false;
    else
    {
        monkey::iap::PRODUCT_PACKAGES oldUserProductPackage = monkey::iap::PRODUCT_PACKAGES::FREE;
        monkey::iap::PRODUCT_PACKAGES newUserProductPackage = monkey::iap::PRODUCT_PACKAGES::FREE;
        try {
            // string -> integer
            int oldPackage = std::stoi(oldUserInfoFormerge.at(2));
            oldUserProductPackage = (monkey::iap::PRODUCT_PACKAGES)oldPackage;

            int newPackage = std::stoi(newUserInfoFormerge.at(2));
            newUserProductPackage = (monkey::iap::PRODUCT_PACKAGES)newPackage;
        }
        catch (...) {
            // error management
            isCheck = true;
        }

        if (oldUserProductPackage == monkey::iap::PRODUCT_PACKAGES::LIFT_TIME && newUserProductPackage != monkey::iap::PRODUCT_PACKAGES::FREE)
        {
            isCheck = false;
        }
        else if (newUserProductPackage == monkey::iap::PRODUCT_PACKAGES::LIFT_TIME && oldUserProductPackage != monkey::iap::PRODUCT_PACKAGES::FREE)
        {
            isCheck = false;
        }
        else
        {
            isCheck = true;
        }

    }

    return isCheck;
}
void F2PManager::setCanMergeProfileKey(bool isNeedMerge)
{
    // key nay dung de check xem, khi nao thi call login screen thi duoc di vao luong merge
    MJDEFAULT->setBoolForKey(key_f2p_need_merge_profiles, isNeedMerge);
}
void F2PManager::f2PSaveUserInfo(bool isOldInfo, bool isLogOut)
{
    /*
    * them chuc nang merge account
    * 1. truoc khi dang nhap account moi, luu lai useID va token cua account cu, de phuc vu cho merge account
    *
    *
    */

    /*1. */
    std::string token = ap::Database::getInstance().getAccessToken().c_str();
    ap::UserInfo  userInfo = ap::Database::getInstance().getUserInfo();
    std::vector<std::string> userInfoFormerge = {};
    std::vector<InAppPackageModel> inAppPackageModels = APPURCHASE_MANAGER->getPurchasedPackages();
    monkey::iap::PRODUCT_PACKAGES productPackage = monkey::iap::PRODUCT_PACKAGES::FREE;
    if(inAppPackageModels.size()>0)
        productPackage = IAP_MANAGER->getProductPackage(inAppPackageModels.at(0)._productId);

    if (token.empty() || userInfo._id == 0)
        return;
    userInfoFormerge.push_back(std::to_string(userInfo._id));  // 0 postion : userId
    userInfoFormerge.push_back(token); // 1 postion: token
    userInfoFormerge.push_back(std::to_string((int)productPackage)); //2 postion: productPakge

    if(F2P_MANAGER->isSkipUser())  // 3 postion
        userInfoFormerge.push_back("1");
    else
    {
        userInfoFormerge.push_back("0");
    }
    
    // chi can co sdt hoac email, facbook....
    if(!ap::Database::getInstance().getUserInfo()._phone.empty())  //4 check user da tao tai khoan voi du sdt va pass chua
        userInfoFormerge.push_back(ap::Database::getInstance().getUserInfo()._phone);
    else if(!ap::Database::getInstance().getUserInfo()._mail.empty())
        userInfoFormerge.push_back(ap::Database::getInstance().getUserInfo()._mail);
    else
        userInfoFormerge.push_back("None");

    if(F2P_MANAGER->isSkipUser())
    {
        MJDEFAULT->deleteValueForKey(key_f2p_account_free_info_for_merge);
        MJDEFAULT->deleteValueForKey(key_f2p_account_paid_info_for_merge);
        MJDEFAULT->setArrayStringForKey(key_f2p_account_free_info_for_merge, userInfoFormerge);
    }
    else
    {
        MJDEFAULT->deleteValueForKey(key_f2p_account_paid_info_for_merge);
        MJDEFAULT->setArrayStringForKey(key_f2p_account_paid_info_for_merge, userInfoFormerge);
    }
    
    if (isOldInfo)
    {
        if (isLogOut)
        {
            ap::Database::getInstance().dropDatabase();
            APPURCHASE_MANAGER->reset();
        }

    }

}

void F2PManager::pushEventTrackingDoneItemFree(int id)
{
    ms::f2p::FreeContentOption freeContentOption = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option);
    std::vector<mj::model::FreeItemInfo> freeItemInfos = StoryDataModel::getInstance()->getFreeTrialInfosByOption(freeContentOption);
    for (size_t i = 0; i < freeItemInfos.size(); i++)
    {
        if (id == freeItemInfos[i]._id)
        {
            bool optionFree = (freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM || freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
            CleverTapManager::GetInstance()->pushEvent(optionFree ? "ob_done_7_lessons"
                : "ob_done_14_lessons", {
               {"learned_lesson",cocos2d::Value(std::to_string(i+1) + (optionFree ? "_7" : "_14"))},
               {"lesson_type",cocos2d::Value(freeItemInfos[i]._type == ms::MsTypes::DocumentsType::READING_COMPREHENTION ? "lesson_comprehensive"
               : freeItemInfos[i]._type == ms::MsTypes::DocumentsType::PHONICS ? "lesson_phonic" : "story"
                   )},
               {"learning_day_package",cocos2d::Value("day_" + std::to_string(freeItemInfos[i]._day))},

                });
            break;
        }
    }
}

std::vector<std::string> F2PManager::f2PGetUserInfo(bool isSkipUser)
{
    std::vector<std::string> userInfoFormerge = {};
    if (isSkipUser)
    {
        userInfoFormerge = MJDEFAULT->getArrayStringForKey(key_f2p_account_free_info_for_merge, {});
    }
    else
    {
        userInfoFormerge = MJDEFAULT->getArrayStringForKey(key_f2p_account_paid_info_for_merge, {});
    }
    return userInfoFormerge;
}
