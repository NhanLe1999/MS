//
//  MSTabFavourite.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/26/17.
//

#ifndef MSTabFavourite_hpp
#define MSTabFavourite_hpp

#include "cocostudiomacro2.h"
#include "MSCollectionDelegate.h"
#include "MSCollectionLessonItemDelegate.h"
#//include "MSCollectionAISpeakingLessonItemDelegate.h"

class MSSimpleCollection;

class MSTabFavourite : public cocos2d::ui::Layout, public ms::delegate::Collection,
public ms::delegate::CollectionLessonItem,
public cocostudio::WidgetCallBackHandlerProtocol {
public:
    static bool is_tab_downloaded;
    CREATE_FUNC(MSTabFavourite);
    static MSTabFavourite * createTab();
    void setContentSize(const cocos2d::Size & size) override;

private:
    void didLoadFromCSB();
    void onEnter() override;
    
    void reloadData();
    void reloadData_main();
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    void onSelectMore(MSSimpleCollection * collection) override;
    void onDownloaded(cocos2d::Ref * sender);
    void onFavourite(cocos2d::Ref * sender);
    void onLearnLesson(mj::model::LessonInfo lesson) override;
    void onTryToLearnLockedLessons(mj::model::LessonInfo lesson) override;
    
    MSSimpleCollection * _favourite_stories = nullptr;
    MSSimpleCollection * _favourite_audiobooks = nullptr;
    MSSimpleCollection * _downloaded_stories = nullptr;
    MSSimpleCollection * _downloaded_audiobooks = nullptr;
    MSSimpleCollection * _downloaded_lessons = nullptr;
    MSSimpleCollection* _downloaded_ai_speaking_lessons = nullptr;

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    cocos2d::ui::Layout * _button_favourite, * _button_downloaded;
};

CREATE_READER(MSTabFavourite);

#endif /* MSTabFavourite_hpp */
