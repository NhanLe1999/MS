//
//  MSTabStories.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSTabStories_hpp
#define MSTabStories_hpp

#include "cocostudiomacro2.h"
#include "MSSearchbarDelegate.h"
#include "MSCollectionDelegate.h"
#include "MSGradeManager.h"

class MSSearchbar;
class MSFeaturedStories;
class MSSimpleCollection;
class MSHome;

class MSTabStories : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::Searchbar, public ms::delegate::Collection {
public:
    CREATE_FUNC(MSTabStories);
    static MSTabStories * createTab(MSHome * home = nullptr);
    void goToPopularStories(); // hiển thị Popular Stories
    void goToCategory(int catId); // hiển thị trang Chủ đề truyện nào đó
    void setContentSize(const cocos2d::Size & size) override;
    void clearUnusedContents();
    cocos2d::Vec2 getGradePosition();
    cocos2d::Size getGradeSize();
    mj::model::LevelInfo getLevelInfo(int levelId, int langId);
private:
    void didLoadFromCSB(MSHome * home);
    void onEnter() override;
    
    void updateSearchbarPosition(float dt);

    void setLevelInfoIOfStories(int levelId, int langId);
    
    MSHome *                _mshome = nullptr; // assign var
	MSSearchbar *           _searchbar = nullptr;
    cocos2d::ui::ListView * _content = nullptr;
    MSSimpleCollection *    _categories_collection = nullptr, *_popular_stories_collection = nullptr, * _new_stories_collection = nullptr, * _recent_stories_collection = nullptr, * _favourite_stories = nullptr;

    void onSelectGrade() override;
    void onDeselectGrade(int grade_id) override;
    void onSelectSearch() override;
    void reloadData();
    
    int             _current_grade_id;
    long long       _time_touch_start;
    cocos2d::Vec2   _position_touch_start;
    
    void propagateClickEventToCollection(cocos2d::Point position);
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    void onSelectMore(MSSimpleCollection * collection) override;
    void onReceivedEventGradeChanged(int grade_id);
    void changeTextDisplayLang();
    MSFeaturedStories* _featured = nullptr;
	ms::GradeManager::Scope m_scope;
    cocos2d::ui::Widget * m_searchbarHook = nullptr;
    std::map<std::string, mj::model::LevelInfo> _levelInfoOfStoryItem;
};

CREATE_READER(MSTabStories);

#endif /* MSTabStories_hpp */
