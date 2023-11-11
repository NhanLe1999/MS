
#ifndef MP_TAB_GAMES_H__
#define MP_TAB_GAMES_H__

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "MJBaseActivity.h"

class MPTabGamesDelegate {
public:
	virtual void onClickedActiviyDelegate(mj::model::ActInfo act_info) {};
};
class MPTabGames : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MPTabGames);
    static MPTabGames * createTab(mj::model::LessonInfo info);
	mj::model::ActInfo getNextActivity(mj::model::ActInfo info);
	void setDelegate(MPTabGamesDelegate * _delegate);
protected:
	int numberOfCells();
	cocos2d::Size collectionSize();
	cocos2d::Size modelSize();
private:
	void fetchActivitiesFromAPI(std::function<void(bool)> callback = nullptr);
	mj::model::ActInfo getActivityById(int activity_id);
    void didLoadFromCSB(mj::model::LessonInfo info);
    void onEnter() override;

    std::vector<mj::model::ActInfo> _activities;
	//cocos2d::ui::ScrollView * _collection;
	MPTabGamesDelegate * _delegate;
    void reloadData();
    void onBackToLesson();
	void filterActivities();
	mj::model::LessonInfo lesson_info;
};

CREATE_READER(MPTabGames);

#endif /* MP_TAB_GAMES_H__ */
