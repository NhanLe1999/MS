//
//  MSTabLessons.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSTabLessons_hpp
#define MSTabLessons_hpp

#include "cocostudiomacro2.h"
#include "MSCollectionDataSource.h"
#include "MJStoryInfoModel.h"
#include "MSCollectionDelegate.h"
#include "MSComplexCollection.h"
#include "MSCollectionLessonItemDelegate.h"
#include "MSGradeBoxDelegate.h"
#include "MSPreActivity.h"
#include "MSGradeManager.h"
#include "StoryConfigManager.h"
#include "StoryAlert.h"
#include "MSGradeManager.h"
#include "MSActivityWrapper.h"
#include "MSGradeBox.h"
//// < Dinh Duc - Lesson Search Bar
#include "MSSearchbar.h"
#include "APPopupBase.h"
//// >

class MSTabLessons : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::Collection, public ms::delegate::CollectionLessonItem, public ms::delegate::GradeBox, public ms::delegate::PreActivity, public StoryAlertDelegate  {
public:
 
    CREATE_FUNC(MSTabLessons);
    static Scene* createScene(ms::GradeManager::Scope scope,int force_cat_id, int force_les_id);
    static MSTabLessons * createTab(ms::GradeManager::Scope scope);
    void setContentSize(const cocos2d::Size & size) override;
    void removeLessonReviewLayout();
    
    void loadCustomGradeWithID(int gradeID);
    
    void openLessonForce(int gradeId, int lessonId);
private:
    void didLoadFromCSB(ms::GradeManager::Scope scope);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void runAnimationAppear();
    void runAnimationDisappear();
    void deleteSidebarBlack();
    void changeDisplayLanguage() override;
    
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    bool shouldScrollWhenReload() override;
    double percentScrollWhenReload() override;
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void openLessonFromQR();

    std::function<void()> _callbackAutoPlay = nullptr;
    
    std::vector<cocos2d::ui::Widget *> _ui_source;
    
    std::vector<mj::model::LessonInfo> _lessons;
    
	MSComplexCollection * _collection = nullptr;
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    void onLearnLesson(mj::model::LessonInfo lesson) override;
    void onTryToLearnLockedLessons(mj::model::LessonInfo lesson) override;
    
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    
    void onReceivedEventGradeChanged(int grade_id);
    
    void reloadData();
    
    void reloadLessons(int gradeID = -1);
    
    void onContinueLearn(mj::model::ActInfo) override;
    void onBackToLesson();
    int countCompletedLessons();
    
    void onBack(Ref * sender);
    int _saved_current_row = 0;

    void onboarding();
    bool checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned);
    cocos2d::Vec2 getGradePosition();
    cocos2d::Size getGradeSize();
private:
    MSGradeBox* _gradeBox = nullptr;
	ms::GradeManager::Scope m_scope;
	bool m_isReloadDataCalledFromOnEnterFucntion = false;
	int m_gradeID = -1;
    int m_lesson_order_from_qr = -1; // thứ tự Lesson trong _ui_source cần được mở từ QR

    //// < Dinh Duc - Lesson Search Bar
    //MSSearchbar* m_searchbar = nullptr;
    void onSelectSearch(cocos2d::Ref* sender);
    ui::Button* m_search_button;
    int dem = 0;
    Vec2 x;
    cocos2d::Size k;

    //// >
};

CREATE_READER(MSTabLessons);

#endif /* MSTabLessons_hpp */
