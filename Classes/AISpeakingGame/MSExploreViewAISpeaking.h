/////////////////////////////////////////////////////////////
////  MSExploreViewAISpeaking_hpp
////  Implementation of the Class MSExploreViewAISpeaking_hpp
////  Created on:      21-04-2022
////  Original author: TruongBX
/////////////////////////////////////////////////////////////
//
//#ifndef MSExploreViewAISpeaking_hpp
//#define MSExploreViewAISpeaking_hpp
//
//#include "cocostudiomacro2.h"
//#include "StoryDataModel.h"
//#include "MSCollectionDataSource.h"
//#include "MSCollectionDelegate.h"
//#include "MSGradeBoxDelegate.h"
//#include "MSGradeManager.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#include "MSAISpeakingPermissionPopup.h"
//
//class MSComplexCollection;
//
//class MSExploreViewAISpeaking : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol,
//    public ms::delegate::CollectionDataSource, public ms::delegate::GradeBox, public ms::delegate::Collection,
//    public ms::delegate::CollectionAISpeakingLessonItem{
//public:
//    CREATE_FUNC(MSExploreViewAISpeaking);
//    static MSExploreViewAISpeaking * createView(std::string name, std::vector<mj::model::AISpeakingLessonInfo> lessons);
//    void runAnimationAppear();
//    void runAnimationDisappear();
//
//    void setContentSize(const cocos2d::Size & size) override;
//    void enableEditMode(std::string str = "favorite.edit");
//    void onExit() override;
//    void setName(std::string name);
//    void showPermissionPopup() override;
//
//private:
//    void onEnter() override;
//    void didLoadFromCSB(std::string name, std::vector<mj::model::AISpeakingLessonInfo> lessons);
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
//    void onBack(cocos2d::Ref * sender);
//    void onEdit(cocos2d::Ref * sender);
//
//    void reloadData();
//
//    void onLearnLesson(mj::model::AISpeakingLessonInfo lesson) override;
//    void onTryToLearnLockedLessons(mj::model::AISpeakingLessonInfo lesson) override;
//
//    std::string _name;
//    std::vector<mj::model::AISpeakingLessonInfo> _aiSpeakingLessons;
//    std::vector<mj::model::AISpeakingLessonInfo> _filtered_lessons;
//
//    int numberOfColumns() override;
//    int numberOfCells() override;
//    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
//    cocos2d::Size modelSize() override;
//    cocos2d::Size collectionSize() override;
//    void offLoadCellAt(int index) override;
//    void removeCellAtPosition(int index);
//
//    std::vector<cocos2d::ui::Widget *> _ui_source;
//    MSComplexCollection * _collection = nullptr;
//
//    void onSelectGradeBox() override;
//    void onDeselectGradeBox(int grade_id) override;
//
//    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
//    void onReceivedEventGradeChanged(int grade_id);
//
//    bool _is_editing = false;
//    std::string _saved_title_edit;
//
//    ms::GradeManager::Scope m_scope;
//
//};
//
//CREATE_READER(MSExploreViewAISpeaking);
//
//#endif /* MSExploreViewAISpeaking_hpp */
