#include "../external/json/document.h"
#include "../external/json/rapidjson.h"
#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "cocostudiomacro2.h"
#include "Fruit.h"
#include "Word.h"
#include <vector>
#include "FruitQuestion.h"
#include "HSAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

enum class FruitGameState
{
	Ready,
	Phase1,
	Phase2
};

class FruitJumpRope : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public WordDelegate {
public:
	CREATE_FUNC(FruitJumpRope);
	static FruitJumpRope * createGame(std::string jsonfile);
	void update(float dt) override;
private:
	FruitGameState gameState;
	void informationGame();
	void didLoadFromCSB(std::string json_file);
	void WordClick(Ref* ref) override;
	void onEnter() override;
    void onExit() override;
	void triggerReady();
	void triggerStart();
	void phase2();
	void clearSence();
	void playEffectStars();

	std::vector<cocos2d::Vec2> randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item);
	void triggerWin();
	void placeFruit(int position, Fruit* fruit);
	Word * word;
	Button* _buttonWord;
	Word * p2Word[3];
	void phun(float rad);
	void setUpTouchHandling();
	bool touchable;
	bool left;
	Fruit* fruit;
	bool justJump;
	int count;
	int tapped;
	int timeCount;
	int preFruit;
	int point;
	int rightAnswer;
	int level;
	vector<string> fruitList;
	FruitQuestion* question;
	vector<FruitQuestion*> questions;
	vector<Fruit*> fruits;

	cocos2d::ui::Layout * root;
	void showTutorial();
    void inActiveMode();
    Vec2 word_pos;
};

CREATE_READER(FruitJumpRope)
