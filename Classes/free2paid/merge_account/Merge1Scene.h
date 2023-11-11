
/**
* @author DH
* @created 06/29/2022
*/
/*
* input dau vao, la thong tin useId, tokenID cuar 2 loai account: account free va acount cu da mua
* call cac api: 
    1. api ProfileList cho ca 2 account
    2. get thong tin goi mua cua account da thanh toan bang APPURCHASE_MANAGER->getPurchasedPackages()
    3. click vao lien ket, call api merge, response tra ve:
        1. neu false thi can merge profile --> chuyen sang man merge profile
        2. neu thanh cong, khong can merge profile duoi app, chuyen sang man success
* 
*/
#ifndef Merge1Scene_hpp
#define Merge1Scene_hpp

#include "cocostudiomacro2.h"
#include "APProfile.h"
#include "MsProfileItem.h"
#include "StoryAlert.h"
#include "GettingUserInfoManager.h"
#include "RatingManager.h"
class Merge1Scene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::MSCollectionProfileItemDelegate, public StoryAlertDelegate
{
public:

    CREATE_FUNC(Merge1Scene);
    static cocos2d::Scene* createScene();

    static Merge1Scene* createView();

    

    void onSelectedProfile(int profileId) ;
    void onDeleteSelectedProfile(int profileId);

   
private:
    Merge1Scene() {};
    ~Merge1Scene();
    void didLoadFromCSB();//2.
    void loadAuthenInfoForCallAPI();

    void handlerForFreeAccount(const std::function<void(bool)> callback);
    void handlerForFreeAccountCallback(bool success);
    void handlerForPaidAccount(const std::function<void(bool)> callback);
    void handlerForPaidAccountCallback(bool success);

    void CallAPILoadUpdate();
    void APILoadUpdateCallBack(bool isSucess, std::string msg);

    void CallAPIProfileList();
    void APIProfileListCallBack(bool isSucess, std::string msg);

    void onSignOut(cocos2d::Ref* sender);

    void CallAPIMergeProfile();
    void APIMergeProfileCallBack(bool isSucess, std::string msg, int errorCode);
    void onGoToMerge2Page();

    void loadProfileInfos();
    //void Get
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

    void onBtnLinkClicked(cocos2d::Ref* sender);

    void showWaitViewCustom();

    void deleteWaitViewCustom();

    void selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type);

    void onBtnSkipClicked(cocos2d::Ref* sender);
    MsProfileItem* initProfileItem(ListView* listview, ap::Profile profileInfo);
    void initListViewProfile(ListView* listview, std::vector<ap::Profile> profile);
    void showProductPackages();
    std::string getPackageName(std::string productId);
    void onExit() override;

    void showAlertMergeLiftTimeAccountFalse();

    ms::f2p::F2PAccountInfoForMerge _f2PNewAccountInfoForMerge , _f2POldAccountInfoForMerge ;

    std::function<void(bool)>_handlerForFreeAccountCallback = nullptr, _handlerForPaidAccountCallback = nullptr;
    
private:
    cocos2d::ui::ListView* _profilePaidAccountListView = nullptr, * _profileFreeAccountListView = nullptr;
    CC_SYNTHESIZE(int, _userIdAPICalling, UserIdAPICalling);
    CC_SYNTHESIZE(int, _maxProfile, MaxProfile);
    bool mergeLifeTimeError = false;

};

CREATE_READER(Merge1Scene);

#endif
