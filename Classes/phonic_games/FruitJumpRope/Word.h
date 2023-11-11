#include <string>
#include "cocostudiomacro2.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "WordDelegate.h"
#include "PhonicConfig.h"

USING_NS_CC;
using namespace std;

class Word : public cocos2d::Node{
public:
	CREATE_FUNC(Word);

	static Word * Init(string word);
	void setDelegate(WordDelegate* delegate1);
	WordDelegate* delegate;
	bool touchable;
	string text;
	void setText(string word);
	void wrong();
	void correct();
	void changeColor(Color3B color);
	void setAudio(string wordAudio);
	void showTB(int state);
	void fixFit();
    string wordAudio;
	void addClick();
	cocos2d::Label* getTextField() {
		return textField;
	}
private:
	
	cocos2d::Label* textField;
	cocos2d::ui::Layout* layout;
	
};

CREATE_READER(Word)
