#ifndef MS_POPULAR_SEARCH_ITEM
#define MS_POPULAR_SEARCH_ITEM

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"


class MJPopularSearchItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(MJPopularSearchItem);

	static MJPopularSearchItem* createItem(mj::model::MaterialPopularInfo info);

	static MJPopularSearchItem* createItem(mj::model::PopularSearchAudiobookInfo info);

	static MJPopularSearchItem* createItem(mj::model::PopularSearchStoryInfo info);

	std::string getKeyword();

private:

	void onEnter() override;

	void didLoadFromCSB(mj::model::MaterialPopularInfo info);
	void didLoadFromCSB(mj::model::PopularSearchAudiobookInfo info);
	void didLoadFromCSB(mj::model::PopularSearchStoryInfo info);
    
    void setNewFooter(const std::string& texture);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    
	void onSelectPopularSearch(cocos2d::Ref* sender);

	mj::model::MaterialPopularInfo _info;
	mj::model::PopularSearchAudiobookInfo _audiobookInfo;
	mj::model::PopularSearchStoryInfo _storyInfo;

	cocos2d::ui::ImageView* _thumbnail;
	cocos2d::ui::ImageView* _footer;
	cocos2d::ui::Text* _keyword;
};

CREATE_READER(MJPopularSearchItem);

#endif
