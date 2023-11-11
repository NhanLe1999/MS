
#pragma once
#include "BaseSingleton.h"
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "APPhoneCodePopup.h"
#define SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE 5
using namespace cocos2d;
using namespace cocos2d::ui;

#define F2P_MANAGER F2PManager::GetInstance()

class F2PManager :public BaseSingleton<F2PManager>
{
public:
    void setKeyGotoF2PGetStarted(bool value);
    bool getKeyGotoF2PGetStarted();
    
    void setKeyGotoF2PSplash(bool value);
    bool getKeyGotoF2PSplash();
    bool isNeedGotoCheat();
    void setKeyF2P_Paid(bool value);
    bool getKeyF2P_Paid();
    void deleteKeyF2P_Paid();
    void setKeyF2P_Free(bool value);
    bool getKeyF2P_Free();
    void deletekeyF2P_Free();

    void setSkipUser(bool value);

    bool isSkipUser();
    
    void setMergingAccount(bool value);

    bool isMergingAccount();
    
    void setMegreSkipAccount(bool value);

    bool isMegredAccount();

    void setMegredAccount(bool value);

    bool isMegreSkipAccount();
    
    void clearDataFreeToPaid();
    
    void setFirstLoginSkip(bool value);

	bool isFirstLoginSkip();

    void setUserIdMerged(int userId = 0);

    int getUserIdMerged();

    void setIsFreeContentPage(bool isFreeContentPage);

    bool isFreeContentPage();

    void saveMergedUserInfo(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges);

    void setProfileMergeMaxProfile(int maxProfile);
    
    int getProfileMergeMaxProfile();

    void setProfileMergeCurrentProfile(int maxProfile);

    int getProfileMergeCurrentProfile();
    bool isCanOpenMergeProfilePage();
    bool isCanMergeProfile();
    void setCanMergeProfileKey(bool isNeedMerge);
    void f2PSaveUserInfo(bool isOldInfo = true, bool isLogOut = false);

    void pushEventTrackingDoneItemFree(int id);

    std::vector <std::string> f2PGetUserInfo(bool isOldInfo = true);

private:
    
	
};


