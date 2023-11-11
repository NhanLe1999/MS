#include "MJMainGBase.h"
#include "SpacePipe.h"
#include "SpacePipeSlot.h"
#include "MJHelper.h"

namespace mj {
	struct BAPhonic {
		GameObject text_normal;
		GameObject text_slow;
		std::vector<GameObject> text_part;
		std::vector<GameObject> text_part_slow;
	};
}

class SpaceStationGame : public MJMainGBase {
public:
	static SpaceStationGame* createGame(std::string json_file, bool isAutoPlay);
	CREATE_FUNC(SpaceStationGame);
	virtual bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float dt) override;
	bool onTouchBegan(Touch* touch, Event* unusedEvent) override;
	void onTouchMoved(Touch* touch, Event* unusedEvent) override;
	void onTouchEnded(Touch* touch, Event* unusedEvent) override;
	void onDropPipe(SpacePipe* pipe);
	cocos2d::ui::ImageView* re_Fueling_Spacecraft;
	void guideGame();
	void stopGuideGame();
	Button* speaker;
protected:
	SpaceStationGame();
	void initBackground();
	void initStars();
	void initObjects();
	void getGameData();
	void startGame();
	void endGame();
	void initPipes();
	void playStartingScript();
	void playEndingScript();
	void detachAndReveal(std::function<void()> callback = nullptr);
	void readSentence(std::function<void()> callback = nullptr);
	bool isEnableMovingScreen();
	std::string m_configPath;
	void inActiveMode();
	bool flag;
private:
	cocos2d::Node* m_bgNode;
	cocos2d::Node* m_starNode;
	cocos2d::Node* m_planetNode;
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_leftLight;
	cocos2d::Node* m_rightLight;
	cocos2d::Node* m_smoke;
	cocos2d::Node* m_astronaut;
	cocos2d::Node* m_energy;
	std::vector<SpacePipe*> m_pipes;
	std::vector<SpacePipeSlot*> m_pipeSlots;
	int m_numberOfDetaches;
	GAFWrapper* m_ship;
	cocos2d::Vec2 m_lastTouchLocation;

	std::vector<mj::GameObject> m_words;
	mj::GameObject m_sentence;
	std::string m_audioPath;

	mj::BAPhonic sentence;
	cocos2d::ui::Layout* text_panel;

	int index_sentence = 0;
	int m_game_index = 0;
	bool m_clickable;
	bool m_speakable;
	float _age_check = 3.5;
	int i = 0;

	std::vector<int> m_detachIndex;
	std::vector<math::resource::Image*> m_images;
};
