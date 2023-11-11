//
//  MSTabAudiobooks.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#ifndef MSTabAudiobooks_hpp
#define MSTabAudiobooks_hpp

#include "cocostudiomacro2.h"
#include "MSCollectionDelegate.h"
#include "MSGradeBoxDelegate.h"
#include "MSGradeManager.h"

#define UNDEFINED_GRADE_ID -113

class MSSimpleCollection;
class MSHome;

class MSTabAudiobooks : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::Collection, public ms::delegate::GradeBox {
public:
    CREATE_FUNC(MSTabAudiobooks);
    static MSTabAudiobooks * createTab(MSHome * home = nullptr);
    void setContentSize(const cocos2d::Size & size) override;
    void updateRecently();
    void clearUnusedContents();
    void reloadData(const int i_gradeID = UNDEFINED_GRADE_ID);
private:
    void onEnter() override;
    void didLoadFromCSB(MSHome * home);
    
    void onSelectMore(MSSimpleCollection * sender) override;
    void onPlayAll(MSSimpleCollection * sender) override;
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    
    cocos2d::ui::ListView * _content;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onSearch(cocos2d::Ref * sender);
    
    void onSelectGradeBox() override {};
    void onDeselectGradeBox(int grade_id) override;
    
    void onReceivedEventGradeChanged(int grade_id);
    
    MSHome *             _mshome = nullptr;
    MSSimpleCollection * _recent_collection = nullptr;
    MSSimpleCollection * _favourite_collection = nullptr;
    MSSimpleCollection * _categories_collection = nullptr;

    ms::GradeManager::Scope m_scope;
    int     _current_grade_id = -1 ;
	bool m_isReloadDataFunCalled = false;
};

CREATE_READER(MSTabAudiobooks);

#endif /* MSTabAudiobooks_hpp */
