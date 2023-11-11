//
//  StoryParentLanguages_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/14/17.
//
//

#ifndef StoryParentLanguages_Landscape_hpp
#define StoryParentLanguages_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"
#include "MMFirebaseDatabase.h"
class StoryParentLanguages_Landscape : public StoryParentSubPage_Landscape {
public:
    CREATE_FUNC(StoryParentLanguages_Landscape);
    static StoryParentLanguages_Landscape * createPage();
    void onShow();
private:
	~StoryParentLanguages_Landscape();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;

    void onSelectLanguage(cocos2d::Ref * sender);
    void didLoadFromCSB() override;
    int current_lang = 1,beforeLanguageID = 1;
    void onBack(Ref *sender);
    void changeDisplayLanguage();
    bool isBack = false;

	void pushCleverTapEvent();
};

CREATE_READER_CLASS(StoryParentLanguages_LandscapeReader, StoryParentLanguages_Landscape);

#endif /* StoryParentLanguages_Landscape_hpp */
