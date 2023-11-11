#pragma once

#include "MJMainGBase.h"
#include "Mole.h"
#include "GameData.h"

class WhackAMoleGame : public MJMainGBase, WhackAMole::HitMoleListener
{
public:
	static WhackAMoleGame * createGame(std::string json_file, bool isAutoPlay);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float delta) override;
	virtual void onMoleHit(float time,void* mole);
	virtual void onMoleHide() override;
	void startGame();
	CREATE_FUNC(WhackAMoleGame);
protected:
	WhackAMoleGame();
	std::string m_configPath;
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_endingNode;
	float m_nextMoleUpRemaining;
	float m_moleUpInterval;
	int m_hitTimes;
	mj::GameObject m_letter;
	std::vector<WhackAMole::Mole*> m_moles;
    std::vector<mj::GameObject> letters;
	std::string _audioGuiding= "";
	std::string _audioIntro = "";
	bool m_preventReinit;
    int m_max_game = 0;
    int m_count_hit = 0;
	cocos2d::ui::Button* _speaker;
	bool _isSpeaking = false;
//    bool is_firstTime;
	int _audioSpeaker = -1 ;
	int _audioInactiveMode = -1;
	bool _guideShowed = true; // de mac dinh la true vi thay doi yeu cau khong hien thi guiding nua, neu can hien thi guiding thi set = false 
    

private:
	bool initBackground();
	bool initMoles();
	void initIntro(std::function<void()> finishCallback = nullptr);
	void getGameData();
    void inActiveMode();
    void showHand();
    void hideHand();
	void onSpeakerClicked(cocos2d::Ref* sender);
};
