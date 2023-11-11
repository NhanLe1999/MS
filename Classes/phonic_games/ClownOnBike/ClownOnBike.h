#include "../external/json/document.h"
#include "../external/json/rapidjson.h"
#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "cocostudiomacro2.h"
#include "Ball.h"
#include <vector>
#include "Question.h"

USING_NS_CC;
using namespace ui;



class ClownOnBike : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public BallDelegate {
public: 
	enum class GameState
	{
		Ready,
		Playing,
		Lose,
		Ending
	};

	enum class ClownState
	{
		Idle,
		Left,
		LeftIdle,
		LeftFall,
		FallToLeft,
		Right,
		RightIdle,
		RightFall,
		FallToRight
	};
	CREATE_FUNC(ClownOnBike);
	static ClownOnBike * createGame(std::string jsonfile);
	void update(float dt) override;

private:
	void didLoadFromCSB(std::string json_file);
	void BallClick(Ref* ref) override;
	void onEnter() override;
    void onExit() override;
	void triggerReady();
	void triggerPlay();
	void triggerLose();
	void triggerEnd();
	void transition();
	void changeClownState();
	void putBallToLast(int i, bool left);
	void guideGame();
	void stopGuideGame();
	void generateQuestion();
	void warning();
	void changeBallSpeed(bool left, int DSpeed);
	void itemOverLay();
	void removeItemOverLay();
	Ball* ball1;
	Ball* ball2;
	Ball* ball3;
	GAFWrapper* rope;
	GAFWrapper* clown;
	GAFWrapper* handTap;
	ClownState clownState;
	GameState gameState;
	Node* leftBallNode;
	Node* rightBallNode;
	vector<Ball*> leftBalls;
	vector<Ball*> rightBalls;
	void presentClown();
	void phrase2();
	void Balance(bool toIdle);
	float timeCount;
	int recentBall;
	int topLeftBall;
	int topRightBall;
	int leftSpeed;
	int rightSpeed;
	bool ballShowing;
	bool clickable;
	bool upLeft;
	int point;
	int currentBall;
	int currentLevel;
	int rightAnswer;
	float bgPosX;
	float bgPosY;
	cocos2d::ui::Layout* leftLayout;
	cocos2d::ui::Layout* rightLayout;
	Question* question;
	vector<Question*> questions;
	Sprite* backgound_game;
	Sprite* backgound;
	bool flag = true;
};

CREATE_READER(ClownOnBike)
