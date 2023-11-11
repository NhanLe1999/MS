//
//  MSExplorerView_Categories.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/21/17.
//

#ifndef MSExplorerView_Categories_hpp
#define MSExplorerView_Categories_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"
#include "MSGradeBoxDelegate.h"

class MSComplexCollection;

class MSExploreView_Categories : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::Collection {
public:
    CREATE_FUNC(MSExploreView_Categories);
    static MSExploreView_Categories * createView(int i_gradeID);
    void runAnimationAppear();
    void runAnimationDisappear();
    void setContentSize(const cocos2d::Size & size) override;

private:
    void onEnter() override;
    void didLoadFromCSB(int i_gradeID);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    
    void reloadData();

    std::vector<mj::model::CategoryInfo> _categories;

    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;

    std::vector<cocos2d::ui::Widget *> _ui_source;
    MSComplexCollection * _collection = nullptr;

    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;

    int _current_grade_id;

};

CREATE_READER(MSExploreView_Categories);

#endif /* MSExplorerView_Categories_hpp */

