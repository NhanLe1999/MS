//
// Created by To Ngoc Linh on 9/6/17.
//

#ifndef MJSTORY_MJGRADEINFOCONTAINER_H
#define MJSTORY_MJGRADEINFOCONTAINER_H

#include "cocostudiomacro2.h"
#include "MJGradeInfoTab.h"

class MJGradeInfoContainer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGradeInfoTabListener {
public:
    CREATE_FUNC(MJGradeInfoContainer);
    static MJGradeInfoContainer * createView();
    void selectGrade(int grade_id);
	std::string getCurrentGrade() { return (m_currentTab != nullptr ? m_currentTab->getGradeInfo().name : "") ; }//for tracking 
private:
    void onEnter() override;
    void didLoadFromCSB();
    void onSelectTab(MJGradeInfoTab * selected_tab, mj::model::GradeInfo info) override;

    std::vector<mj::model::LevelInfo> _levels;
	MJGradeInfoTab* m_currentTab = nullptr;//for tracking
	
	CC_SYNTHESIZE(cocos2d::Node*, m_parent, ParentDelegate);

};

CREATE_READER(MJGradeInfoContainer);

#endif //MJSTORY_MJGRADEINFOCONTAINER_H
