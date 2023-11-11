#pragma once

#include "cocos2d.h"
#include "cocostudiomacro2.h"
//#include "ms_game/MSActInfoModel.h"
#include "MPTabGame.h"
#include "MPTabLessons.h"
#include "MJBaseActivity.h"

class MJLessonsListRemoteDelegate {
public:
	virtual void onClickCloseBtn() {};
	
};
class MJLessonsListRemote : public cocos2d::Scene, public  MPTabGamesDelegate, public MPTabLessonsDelegate, public ms::delegate::Activity
{
public:
	CREATE_FUNC(MJLessonsListRemote);
	bool init() override;
	void initGUI();

	void downloadFile(std::string url, std::function<void(bool, std::string)> callback, bool disableCache = false);
	void downloadAndPlay(mj::model::ActInfo act_info);
	void startGame(mj::model::ActInfo ai);
	void switchToRemotePaths();
	void clearCaches();
	void clearGameCache();
    void disableButton();
    void enableButton();
	virtual void onExit() override;
	void fetchGameInfoFromAPI(std::function<void(bool)> callback = nullptr);
	MJLessonsListRemoteDelegate * _delegate;
	void onClickedActiviyDelegate(mj::model::ActInfo act_info) override;
	void onClickedLessonDelegate(mj::model::LessonInfo lesson) override;
     void onActivityEnd(cocos2d::Ref * sender, EndReason reason = FINISHED) override;
protected:
	MJLessonsListRemote();
	std::map<int, mj::model::GameInfo*> m_games;
    cocos2d::ui::Text* m_progress;
	int m_currentCategory;
	int m_currentGame;
	bool m_downloading;
    MPTabGames * _tab_game;
    mj::model::ActInfo _current_act;
private:
	void createFile(std::string path);
	void createDirectory(std::string path);
};
