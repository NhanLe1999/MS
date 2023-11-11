/////////////////////////////////////////////////////////////
////  MSTabAISpeaking_hpp
////  Implementation of the Class MSTabAISpeaking_hpp
////  Created on:      21-04-2022
////  Original author: TruongBX
/////////////////////////////////////////////////////////////
//
//#ifndef MSTabAISpeaking_hpp
//#define MSTabAISpeaking_hpp
//
//#include "cocostudiomacro2.h"
//#include "MSCollectionDataSource.h"
//#include "MJStoryInfoModel.h"
//#include "MSCollectionDelegate.h"
//#include "MSComplexCollection.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#include "MSGradeBoxDelegate.h"
//#include "MSPreActivity.h"
//#include "MSGradeManager.h"
//#include "StoryConfigManager.h"
//#include "StoryAlert.h"
//#include "MSGradeManager.h"
//#include "MSActivityWrapper.h"
//#include "MSGradeBox.h"
//#include "MSLessonAISpeakingManager.h"
//#include "MSSearchbar.h"
//#include "AISpeakingTopic.h"
//#include "MSAISpeakingPermissionPopup.h"
//#include "CameraHelper.h"
//#include "APPurchaseManager.h"
//#include "StorySubscribePopup.h"
//#include "MJBaseActivity.h"
//#include "HSLib.h"
//
//#define DELAY_CREAT_TOPIC 0.3
////// >
//
//class MSTabAISpeaking : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol,
//public ms::delegate::CollectionAISpeakingLessonItem, public ms::delegate::PreActivity, public StoryAlertDelegate ,public ms::delegate::Activity ,
//    public ms::delegate::AISpeakingPermissionPopupDelegate{
//public:
//
//    CREATE_FUNC(MSTabAISpeaking);
//    static MSTabAISpeaking* createTab(MSHome* home, ms::GradeManager::Scope scope = ms::GradeManager::Scope::LESSONS);
//    void setContentSize(const cocos2d::Size & size) override;
//
//    void onReloadAISpeakingPage();
//
//    void showPermissionPopup() override;
//    void requestPermissionCameraAndAudio();
//    bool checkPermissionCameraAndAudio();
//    void ClearView();
//    void clearUnusedContents();
//private:
//    void didLoadFromCSB(MSHome* home, ms::GradeManager::Scope scope = ms::GradeManager::Scope::LESSONS);
//    bool onDeleteData();
//    void onEnter()override;
//    void onExit() override;
//    void onAlertOK(cocos2d::Ref * sender) override;
//    void onAlertCancel(cocos2d::Ref * sender) override;
//    void onContinueLearn(mj::model::ActInfo) override;
//    int countCompletedLessons();
//    std::vector <std::vector<mj::model::AISpeakingLessonInfo>> getAiSpeakingLessonsAllGradesByCategory();
//    void InitialListViewTopicCollection(std::vector <std::vector<mj::model::AISpeakingLessonInfo>>_aiSpeakingLessonsAllGrades);
//    AISpeakingTopic* creatListViewLessonList(std::vector<mj::model::AISpeakingLessonInfo> _aiSpeakingLessons, mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo);
//    void UpdateListViewCollection(std::vector <std::vector<mj::model::AISpeakingLessonInfo>>_aiSpeakingLessonsAllGrades, int oldListViewIndex, int newListViewIndex);
//    void UpdateDataView(int index, int durationChangeIndex);
//    void selectedTopicItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
//
//    bool checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned);
//    vector<int>getLessonLearnedfromProfile();
//    void setLessonLearedfromProfile(vector<int> lessonLeared);
//    void updateLessonLeared(int lessonLeared);
//
//    int getCurrentSelectTopicIndex();
//    int getCurrentSelectLessonInTopicIndex();
//
//    void setGradleIDs(std::vector <int>gradleIDs) {
//        this->_gradleIDs = gradleIDs;
//    };
//    std::vector <int> getGradleIDs() {
//        return this->_gradleIDs;
//    };
//    void setAiSpeakingLessonsAllGrades(std::vector <std::vector<mj::model::AISpeakingLessonInfo>>_aiSpeakingLessonsAllGrades) {
//        this->_aiSpeakingLessonsAllGrades = _aiSpeakingLessonsAllGrades;
//    };
//    void insertAiSpeakingLessonsAllGrades(std::vector<mj::model::AISpeakingLessonInfo>_aiSpeakingLessonsAllGrade) {
//        this->_aiSpeakingLessonsAllGrades.push_back(_aiSpeakingLessonsAllGrade);
//    };
//    std::vector <std::vector<mj::model::AISpeakingLessonInfo>> getAiSpeakingLessonsAllGrades() {
//       return this->_aiSpeakingLessonsAllGrades ;
//    };
//    void setAISpeakingCategoryInfos(std::vector <mj::model::AISpeakingCategoryInfo> aISpeakingCategoryInfos) {
//         this->_aISpeakingCategoryInfos = aISpeakingCategoryInfos;
//    };
//    std::vector <mj::model::AISpeakingCategoryInfo> getAISpeakingCategoryInfos() {
//        return this->_aISpeakingCategoryInfos;
//    };
//
//    void onActivityEnd(Ref * sender, ms::delegate::Activity::EndReason reason);
//
//    std::function<void()> _callbackAutoPlay = nullptr;
//
//    std::vector <std::vector<cocos2d::ui::Widget*>> _ui_sources;
//
//    //std::vector<mj::model::AISpeakingLessonInfo> _aiSpeakingLessons;
//
//    std::vector < MSComplexCollection*> _collections;
//    int _saved_current_row = 0;
//    int _percent = 0, oldPercent = 0;
//    void onAccept(cocos2d::Ref* sender) override;
//    void onClose(cocos2d::Ref* sender) override;
//    void onSavelessonLearnAfterLearned(mj::model::AISpeakingLessonInfo lessonInfo) override;
//
//private:
//    EventListenerTouchOneByOne* _touchListener = nullptr;
//    void onUpdateTopicHaveLessonIteamLearned(mj::model::AISpeakingLessonInfo lessonInfo);
//    bool creatAiSpeakingFolders();
//    void onLearnLesson(mj::model::AISpeakingLessonInfo lesson) override;
//    void showSubscribePopup() override ;
//    void onSubscribeOk() override;
//    void onSubscribeCancel() override;
//    void onSubscribeFree() override;
//    void setUnlockTopic();
//    void pushEventAIspeakingToCleaverTap(bool allow = false);
//    void onVerticalScrollView(int index) override;
//
//    MSHome* _mshome = nullptr; // assign var
//    ui::ListView* _listViewTopics;
//    ui::ScrollView* scrollView = nullptr;
//    std::vector <std::vector<mj::model::AISpeakingLessonInfo>>_aiSpeakingLessonsAllGrades;
//    std::vector <int>_gradleIDs;
//    std::vector <mj::model::AISpeakingCategoryInfo>_aISpeakingCategoryInfos;
//    std::vector < Layout*>_collectionsLayout;
//    std::vector < int >_lessonIdsLearnedFromProfile;
//    int currentTopicIndex = 0, listViewStartPostion = 0, _listViewSize = 0;
//    Layout* _panel_1 = nullptr;
//    MSAISpeakingPermissionPopup* _popupPermission = nullptr;
//    int _countForUserExpectedInAllTopic = 0; // using to limit number of free topics in ai speaking tab
//    bool _isLockTopic = false;
//    int selectIndex = 0;
//    StorySubscribePopup *subscirbe_popup = nullptr;
//    //// >
//};
//
//CREATE_READER(MSTabAISpeaking);
//
//#endif /* MSTabAISpeaking_hpp */
