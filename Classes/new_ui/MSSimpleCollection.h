//
//  MSSimpleCollection.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef MSSimpleCollection_hpp
#define MSSimpleCollection_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

namespace ms {
    namespace delegate {
        class Collection;
    }
}

class MSSimpleCollection : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    enum CollectionType {
        STORY,
		CATEGORY,
		AUDIOBOOK_CATEGORY,
        AUDIOBOOK,
        LESSON,
        AI_SPEAKING
    };
    
    enum CollectionOrder {
        POPULAR,
        NEW,
		MORAL_STORIE,
		WORLD_OF_TALE,
		FAMOUS_FABLE,
		POEM,
        OTHER
    };
    
    CREATE_FUNC(MSSimpleCollection);
    static MSSimpleCollection * createCollection(std::string title, std::vector<cocos2d::ui::Widget *> items);
    
	static MSSimpleCollection * createCategoriesCollection();
	static MSSimpleCollection * createAudiobookCategoriesCollection();
    static MSSimpleCollection * createPopularStoriesCollection(int grade_id);
    static MSSimpleCollection * createNewestStoriesCollection(int grade_id);
    static MSSimpleCollection * createRecentStoriesCollection(int grade_id);
    static MSSimpleCollection * createFavouriteStoriesCollection(int grade_id);
    static MSSimpleCollection * createFavouriteAudiobooksCollection(int grade_id);
    static MSSimpleCollection * createDownloadedAudiobooksCollection(int grade_id);
    static MSSimpleCollection * createDownloadedStoriesCollection(int grade_id);
    static MSSimpleCollection * createChaptersCollection(int grade_id);
    
    static MSSimpleCollection * createPopularAudiobooksCollection(int grade_id);
    static MSSimpleCollection * createNewestAudiobooksCollection(int grade_id);
    
    static MSSimpleCollection * createDownloadedLessonsCollection(cocos2d::Node* i_node = nullptr);
    static MSSimpleCollection* createDownloadedAISpeakingLessonsCollection(cocos2d::Node* i_node = nullptr);

    void setContentSize(const cocos2d::Size & size) override;
    void setLeftPadding(float padding);
    void setChildMargin(float margin);
    void processClickEvent(cocos2d::Point touch_position);
    void changeTextDisplayLang(std::string title);
    
    std::string getTitle();
    void addItems(std::vector<cocos2d::ui::Widget *> items);
    
    void disableMore();
    void enablePlaylist();
    void onMoreDefault();
    
    const cocos2d::Vector<cocos2d::ui::Widget *>& getCollectionItems() {return _list->getItems();};
    
private:
    void didLoadFromCSB(std::string title, std::vector<cocos2d::ui::Widget *> items);
    void onSelectItem(cocos2d::Ref * sender, cocos2d::ui::ListView::EventType event);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onMore(cocos2d::Ref * sender);
    void onPlayAll(cocos2d::Ref * sender);
    
    cocos2d::ui::ListView * _list = nullptr;
    cocos2d::ui::Text * _label_title;
    
    std::vector<cocos2d::ui::Widget *> _items;
    
    CC_SYNTHESIZE(std::vector<mj::model::StoryInfo>, _stories, Stories);
    CC_SYNTHESIZE(std::vector<mj::model::AudiobookInfo>, _audiobooks, Audiobooks);
	CC_SYNTHESIZE(std::vector<mj::model::CategoryInfo>, _categories, Categories);
	CC_SYNTHESIZE(std::vector<mj::model::AudiobookSeriesInfo>, _audiobookCategories, AudiobookCategories);
    CC_SYNTHESIZE(std::vector<mj::model::LessonInfo>, _lessons, Lessons);
    CC_SYNTHESIZE(std::vector<mj::model::AISpeakingLessonInfo>, _aiSpeakingLessons, AISpekingLessons);

    std::string _title;
    long long _time_touch_start;
    cocos2d::Vec2 _position_touch_start;
    
    CC_SYNTHESIZE(ms::delegate::Collection *, _delegate, Delegate);
    CC_SYNTHESIZE(CollectionType, _collection_type, CollectionType);
    CC_SYNTHESIZE(CollectionOrder, _collection_order, CollectionOrder);
    CC_SYNTHESIZE(std::string, _series_name, SeriesName);
};

CREATE_READER(MSSimpleCollection);

#endif /* MSSimpleCollection_hpp */
