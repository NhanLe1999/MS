//
//  StoryParentDeleteCache_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/10/17.
//

#ifndef StoryParentDeleteCache_Landscape_hpp
#define StoryParentDeleteCache_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"

class StoryParentDeleteCache_Landscape : public StoryParentSubPage_Landscape {
public:
    CREATE_FUNC(StoryParentDeleteCache_Landscape);
    static StoryParentDeleteCache_Landscape * createPage();

private:
	~StoryParentDeleteCache_Landscape();
    void didLoadFromCSB() override;
    void startQueryFolderSize();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onOK(cocos2d::Ref *sender);
    void onBack(cocos2d::Ref * sender);
    void onDeleteStory(cocos2d::Ref * sender);
    void onDeleteAudiobook(cocos2d::Ref * sender);
	void pushCleverTapTrackingData();

private:

	bool _is_back = false;
    
    double _stories_size, _audiobooks_size, _cached_size;

	std::string m_deletingProperty = "none";

public:

    void onShow();
};

CREATE_READER_CLASS(StoryParentDeleteCache_LandscapeReader, StoryParentDeleteCache_Landscape);
#endif /* StoryParentDeleteCache_Landscape_hpp */
