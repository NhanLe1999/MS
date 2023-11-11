#pragma once

#include "cocostudiomacro2.h"
#include "APProfile.h"
#include "MsProfileItem.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "MSIntro.h"
#include "CleverTapManager.h"
#include "HelperManager.h"
#include "Merge2Scene.h"
#include "Merge1Scene.h"
#include "MS_F2P_API_MergeAccount.h"
#include "APProfileCard.h"
#include "MSMergeSuccess.h"
#include "APProfilesListAPI.h"
#include "F2PManager.h"
#include "ShowNewAlert.h"

class Merge2Scene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::MSCollectionProfileItemDelegate
{
public:

    CREATE_FUNC(Merge2Scene);

    static cocos2d::Scene* createScene(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges);

    static Merge2Scene* createView(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges);//1.

private:

    void didLoadFromCSB(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges);//2.

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

    void onBtnBackToMerge1Clicked(cocos2d::Ref* sender);

    void onBtnSubmitClicked(cocos2d::Ref* sender);

    void onSelectedProfile(int profileId) override;
    void onDeleteSelectedProfile(int profileId) override;

    void switchToMergeSuccess();
    
    void callMergAgain();

    void updatePage();
    void selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
    void onExit() override;

    MsProfileItem* initProfileItem(ListView* listview, ap::Profile profileInfo);
    void initListViewProfile(ListView* listview, std::vector<ap::Profile> profile);

    void showProductPackages();
    void onDisableDeleteProFile();
    std::string getPackageName(std::string productId);

private:
    std::vector<ap::Profile> _listProfile;
    std::vector<ap::Profile> _listProfileForView;
    std::vector<int> _listProfileToDelete;
    cocos2d::ui::ListView* _profileListView = nullptr;
    bool _isNeedDelete = false;
    vector<ms::f2p::F2PAccountInfoForMerge> _f2PAccountInfoForMerges;
    CC_SYNTHESIZE(bool, _mergeSuccessStatus, MergeSuccessStatus);
    CC_SYNTHESIZE(int, _maxProfile, MaxProfile);

};

CREATE_READER(Merge2Scene);

