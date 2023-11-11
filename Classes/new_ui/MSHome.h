//
//  MSHome.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSHome_hpp
#define MSHome_hpp

#include "cocostudiomacro2.h"
#include "MSSidebarDelegate.h"
#include "MJUpdateManager.h"
#include "MSTabLessonSelect.h"
#include "MSTabMaterial.h"
#include "StoryDataModel.h"
#include "intro/MSOnboardDelegate.h"
//#include "MSTabAISpeaking.h"
#define MSHOME_NAME                             "home_view"
#define mshome_name_storyparentpage_landscape   "ms_storyparentpage_landscape"
#define mshome_name_tabsubscribe                "ms_tabsubscribe"
#define mshome_name_account_userinfo            "mshome_name_account_userinfo"
#define mshome_name_choose_profile_view         "mshome_name_choose_profile_view"
#define mshome_name_appdrawer                   "mshome_name_appdrawer"

/**
 * Hệ thống Z-Order của MSHome, addChild vào MSHome thì chọn hoặc thêm giá trị ở đây, không dùng bừa bãi
 */
enum {
    kMSHomeZOrder_sideBar       = 10,           // sidebar
    kMSHomeZOrder_contentFull   = 11,           // nội dung full màn hình
    kMSHomeZOrder_parentSetting = 20,           // màn hình Setting/Account/License... của bố mẹ
    kMSHomeZOrder_popupUpdate   = 2000999999,   // popup Update
};

//#include "MSEventPopup.h"
class MSSearchbar;
class MSSidebar;
//class MSTabLessonSelect;
class MSTabLessons;
class MSTabStories;
class MSTabAudiobooks;
class MSTabFree;
class MSTabFavourite;
class MSTabMaterial;

//main interface of application

class MSHome : public cocos2d::ui::Layout, cocostudio::WidgetCallBackHandlerProtocol, public ms::SidebarDelegate, public ms::MSTabLessonSelectDelegate, public ms::MSOnboardDelegate
{

public://public functions

    CREATE_FUNC(MSHome);
    
    /**
     * creation function
     */
    static MSHome * createView(cocos2d::Scene *i_homeScene);
    ~MSHome();
    /**
     * create interface in a scene
     */
    static cocos2d::Scene * createScene();
    
    void openLesson(int gradeId, int lessonId,ms::LessonTab tab);
    void openStory(const mj::model::StoryInfo& story_info);
    void clearLayouts();
    void ScanSubFolderImageCache();
    void addLayoutChooseProfile(bool createNewProfileShowed = false);
    void goToPromotionLayout();
    void goToPopularStories();
    void goToStoriesTab(bool showGrades = false);
    void goToStoriesTabWithCategory(int catId);
    void goToLessonsTab();
    void goToAudiobooksTab(bool showGrades = false);
    void goToQRCodeTab();
    void goToMaterialTab(mj::model::MaterialInfo::Type type = mj::model::MaterialInfo::Type::NONE, int grade_id = -1, const std::string& selected_ids = "");
    void goToSubscribeLayout();
    void goToSettingsLayout();
    void goToAccountProfileDetailLayout();
    void goToAccountLayout(bool showCalculator = false);
    CC_DEPRECATED_ATTRIBUTE void goToParentAccountProfilePage(); // đã có account v3 mới, hàm này giờ ko dùng đến nữa
    void goToLicensePage();
    void goToParent();
    void processQRCode(std::string url = "");
    void onSelectedMaterial(mj::model::MaterialInfo::Type type, const std::string& selected_ids = ""); // màn hình Học liệu Stories hoặc Lessons
    void goToSignInForF2PMerge();
    void btnClickedEffect(ui::Button* btn);

private://private functions
    void deleteSidebarBlack();
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExit() override;
    
    void onShowFreeLessonIcon();
    void onMoveFreeButton(cocos2d::ui::Button* btn);
    void didLoadFromCSB(cocos2d::Scene *i_homeScene);
    
    /**
     * sidebar selection callback
     * @param tab
     */
    void onSelectedTab(ms::HomeTab tab, ms::MovingDirection) override;
    
    /**
     * sidebar select language callback
     * @param is_show - show or hide languages selection
     * @param lang_code - selected language code
     */
    void onSelectedLanguage(bool is_show, std::string lang_code) override;
    
    void onTest(Ref * sender);
    
    void transitionBetweenTab(cocos2d::ui::Layout * prev, cocos2d::ui::Layout * curr, ms::MovingDirection d);

    void onboarding();

    void onPushEventForWinMac();

    void onSelectedLesson(ms::LessonTab tab) override {
        onSelectedLesson(tab, -1, -1);
    };
    
    // mở trang Lessons: Comprehension hoặc Phonics, nếu tham số cat_id > 0 và les_id > 0 thì mở luôn bài học
    void onSelectedLesson(ms::LessonTab tab, int force_cat_id, int force_les_id);

    void onFinishOnboard() override;

    void hideAllTabs();
    
    void intCheatMenu();
    void onTouchFreeLessonIcon(cocos2d::Ref* sender);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void changeIconAppForIos();
    void revertIconAPPForIos();
#endif

    

private://variables

    cocos2d::ui::Layout*    m_prevTab               = nullptr;
    MSTabLessonSelect*      m_tabLessonSelect       = nullptr;
    MSTabLessons*           m_comprehensionLesson   = nullptr;
    MSTabStories*           m_tabStories            = nullptr;
   // MSTabAISpeaking*        _tabAISpeaking = nullptr;
    MSTabMaterial *         m_tabMaterial           = nullptr; // phần Học liệu
    MSTabAudiobooks*        m_tabAudiobooks         = nullptr;
    MSTabFree*              m_tabFree               = nullptr;
    MSTabFavourite*         m_tabFavourite          = nullptr;
    ms::HomeTab             m_currentTab;
    MSSidebar *             m_sidebar               = nullptr;
    bool                    m_haveExited            = false;
    bool                    m_isLicenseShow         = false;
    MJUpdateManager*        mjupdate_manager        = nullptr;
    ParticleSystemQuad*     m_clickEffect           = nullptr;
    bool                    _clickFreeLessonButton  = false;
    cocos2d::Vec2           _startTouchPoint        = Vec2(0, 0);
    cocos2d::ui::ImageView* _freeLessonContentText  = nullptr;
};

CREATE_READER(MSHome);

#endif /* MSHome_hpp */
