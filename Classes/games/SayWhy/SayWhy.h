#ifndef MJ_GAME_SAY_WHY_H
#define MJ_GAME_SAY_WHY_H

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "SayWhyAnswer.h"
#include "SayWhyBook.h"

class SayWhy : public MJMainGBase, public SayWhyBookDelegate {
public:
	SayWhy();
	static SayWhy* getInstance();
	virtual bool init() override;
	static SayWhy* createGame(std::string json_path);
	void generateData();
	void onEnter() override;
	void onExit() override;
	void onExitTransitionDidStart() override;
	void onClickedAnswer(cocos2d::Ref* sender) override;
	void radio();
	void stopRadio();
protected:
	void loadFrames();
	void initBook();
	void showIntroGames();
	void startGame();
	void onChooseRight(SayWhyAnswer* answer);
	void onChooseWrong(SayWhyAnswer* answer);
	void onNextQuestion();
	void onFinishGame() override;
	void lockTouch(bool is_lock);
	void showTapGuideGame(Vec2 pos, int loop = 100, float delay_time = 10.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	CC_SYNTHESIZE(bool, isReading, Reading);
private:
	static SayWhy* _sayWhyInstance;
	cocos2d::ui::Layout* layout_root;
	GAFWrapper* m_detective;
	std::string m_config_path;
	SayWhyBook* m_sw_book;
	std::vector<SayWhyBook::SayWhyBookInfo> list_game_info;
	Button* _speaker;
	int number_turnplay;
	int _idAudio = -1;
	int _idRadio = -1;
	void initSampleGameInfo();
};

#endif // !MJ_GAME_SAY_WHY_H