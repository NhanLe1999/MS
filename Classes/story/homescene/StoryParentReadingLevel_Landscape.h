//
//  StoryParentReadingLevel_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/15/17.
//
//

#ifndef StoryParentReadingLevel_Landscape_hpp
#define StoryParentReadingLevel_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"
#include "MJGradeInfoContainer.h"

class StoryParentReadingLevel_Landscape : public StoryParentSubPage_Landscape {
public:
    CREATE_FUNC(StoryParentReadingLevel_Landscape);
    static StoryParentReadingLevel_Landscape * createPage();
	void onSelectTab(MJGradeInfoTab * selected_tab, mj::model::GradeInfo info);
private:
	~StoryParentReadingLevel_Landscape();

    void didLoadFromCSB() override;
    void onEnter() override;

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onBack(cocos2d::Ref * sender);

	MJGradeInfoContainer* grade_container;
	void pushCleverTapEvents();

	CC_SYNTHESIZE(bool, m_isPushEventEnable, PushEventEnable);
};

CREATE_READER_CLASS(StoryParentReadingLevel_LandscapeReader, StoryParentReadingLevel_Landscape);

#endif /* StoryParentReadingLevel_Landscape_hpp */
