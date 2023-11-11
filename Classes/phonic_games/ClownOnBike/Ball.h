#include "GAFWrapper.h"
#include <string>
#include "BallDelegate.h"
#include "HSAudioEngine.h"
USING_NS_CC;
using namespace std;

class Ball : public GAFWrapper
{
public:
	static Ball* Init(string gaf, bool left, float width, string word);
	void setAudio(string wordAudio);
	void Warning();
	void Boom();
	void InitPosition();
	void FlyAway();
	void setText(string word);
	void HandTap();
	BallDelegate* delegate;
	bool Left;
	int color;
	void Wrong();
	bool boomed;
	bool phrase2;
	void setDelegate(BallDelegate* delegate1);
	bool touchable;
	bool flag = true;
private:
	string wordAudio;
	string word;
	cocos2d::ui::Text* textField;
	GAFWrapper* hand;
	GAFWrapper* firework;
	GAFWrapper* effec;
	cocos2d::ui::Layout* layout;
	void addClick();
};
