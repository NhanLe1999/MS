//
//  MSExploreView_Lessons.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/29/18.
//

#ifndef MSExploreView_Lessons_hpp
#define MSExploreView_Lessons_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"
#include "MSGradeBoxDelegate.h"
#include "MSGradeManager.h"
#include "MSCollectionLessonItemDelegate.h"

class MSComplexCollection;

class MSExploreView_Lessons : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::GradeBox, public ms::delegate::Collection, public ms::delegate::CollectionLessonItem{
public:
    CREATE_FUNC(MSExploreView_Lessons);
    static MSExploreView_Lessons * createView(std::string name, std::vector<mj::model::LessonInfo> lessons);
    void runAnimationAppear();
    void runAnimationDisappear();
    
    void setContentSize(const cocos2d::Size & size) override;
    void enableEditMode(std::string str = "favorite.edit");
    void onExit() override;
    void setName(std::string name);
    
private:
    void onEnter() override;
    void didLoadFromCSB(std::string name, std::vector<mj::model::LessonInfo> lessons);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    void onEdit(cocos2d::Ref * sender);
    
    void reloadData();

    void onLearnLesson(mj::model::LessonInfo lesson) override;
    void onTryToLearnLockedLessons(mj::model::LessonInfo lesson) override;

    std::string _name;
    std::vector<mj::model::LessonInfo> _lessons;
    std::vector<mj::model::LessonInfo> _filtered_lessons;
    
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    void removeCellAtPosition(int index);
    
    std::vector<cocos2d::ui::Widget *> _ui_source;
    MSComplexCollection * _collection = nullptr;
    
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    void onReceivedEventGradeChanged(int grade_id);
    
    bool _is_editing = false;
    std::string _saved_title_edit;

    ms::GradeManager::Scope m_scope;

};

CREATE_READER(MSExploreView_Lessons);

#endif /* MSExploreView_Lessons_hpp */
