//
//  MSTabFree.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/22/17.
//

#ifndef MSTabFree_hpp
#define MSTabFree_hpp

#include "cocostudiomacro2.h"
#include "MSPreActivity.h"
#include "MSCollectionLessonItemDelegate.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"

#include "MSGradeBoxDelegate.h"

class MSTabFree : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionLessonItem, public ms::delegate::PreActivity, public ms::delegate::GradeBox  {
public:
    CREATE_FUNC(MSTabFree);
    static MSTabFree * createTab();
    void setContentSize(const cocos2d::Size & size) override;
    
private:
    void didLoadFromCSB();
    void changeTextDisplayLang();
    void onEnter() override;
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name);
    void onLearnLesson(mj::model::LessonInfo lesson) override;
    void onContinueLearn(mj::model::ActInfo) override;
    
    void processClickEvent(cocos2d::Point touch_position, ui::ListView* list);
    
    long long _time_touch_start;
    
    void onDeselectGradeBox(int grade_id) override;
    void onSelectGradeBox() override {};
    
    void onBackToLesson();
};

CREATE_READER(MSTabFree);

#endif /* MSTabFree_hpp */
