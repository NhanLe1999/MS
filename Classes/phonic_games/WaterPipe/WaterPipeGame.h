#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "cocostudio/CocoStudio.h"
#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "Pipe.h"
#include "Blank.h"
#include "Choice.h"
#include "PipeReader.h"
//#include "BlankReader.h"
#include "ChoiceReader.h"
#include "DataParser.h"
#include "QuestionInfo.h"
#include "MyAudioPlayer.h"
#include "GAFWrapper.h"
#include "LongPipe.h"
#include "MJMainGBase.h"

class WaterPipe : public MJMainGBase, public MyClassDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    WaterPipe();
    virtual ~WaterPipe();
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(WaterPipe);
	

protected:
	int level;
	int numWrong;
	Pipe* questionPipe;
	cocos2d::Vec2 questionPipeOriginalPosition;
	QuestionInfo * question;
	cocos2d::Node* choiceNode;
	cocos2d::Vec2 choiceNodeOriginalPosition;
	cocos2d::Vector <Choice*> choices;
	cocos2d::Vector <Blank*> blanks;
	cocos2d::ui::Layout* pipeLayout;	
	
	DataParser * dataParser = new DataParser;	
	
	audioPlayer* audio;

	void createDataParser();
	void createLevel(const int &level);
	void makeChoices();
	void makeChoiceMoves();

	void triggerStart();

	void onEnter() override;	
    void onExit() override;
	
	void runRightAnimation(Choice* movingChoice, Blank* _blank);
	void runWrongAnimation(Choice* movingChoice, Blank* _blank);

	void showQuestion();
	float getRealPositionX(cocos2d::ui::ImageView* Long);
	float getRealPositionY(cocos2d::ui::ImageView* Long);

	void madeWrong(Choice* movingChoice, Blank* _blank);
	void madeRight(Choice* movingChoice, Blank* _blank);
	virtual void clickedd(Choice* movingChoice);
	virtual void cancled(Choice* movingChoice);
	Blank* inRectt(Choice* movingChoice);

	void triggerGameOver();

	bool increaseLevel();

	void introduction();

private:
    bool speakerEnabled = false;
	float itemsScaleX = 0.6f;
	float itemsScaleY = 0.28f;
	float firstLength;
	int rightItems;
	int _blankIndex = 0;
	int _partIndex = 0;
	int _numberAge = 0;
	int _indexCorrectAnswer = 0;
	float _timeDelay = 0;
	bool _isClickObject = false;
	bool _isPlayAudioCorrect = true;
	std::vector<cocos2d::Vector<Sprite*>> _part = {};
	std::vector <cocos2d::Vec2> _listPostBlank = {};
	std::vector<int> _valueIndexOfListPostBlank = {};
	int _indexBlank = -1;
	int _valueIndex = -1;
	cocos2d::Vec2 _postPart = cocos2d::Vec2(0, 0);
	Vector<FiniteTimeAction*> _actions;
	std::vector<math::resource::Sync_Text> _listSyncText = {};

	void changeRed(Blank* _blank);
	void changeWhite(Blank* _blank);

	GAFWrapper * gaf;

	cocos2d::Node* rootNode;

	cocos2d::Vector<cocos2d::ui::Text*> textVec;

	void syncText(std::vector<Choice*> listChoice = {});
	void syncText(std::vector<cocos2d::Vector<Sprite*> > parts, std::vector<std::string> audios, int part_index = 0, int blank_index = 0, std::vector<Choice*> listChoice = {});
    
    void inactiveMode(bool enabled,float delay = 12.0f);
	void handSuggetionGame(float timeDlay = 12.0f);
	void syncText2(std::vector<cocos2d::Vector<Sprite*> > parts, std::vector<std::string> audios, int part_index, int blank_index, std::vector<Choice*> listChoice = {});
	std::vector<Choice*> _listChoice = {};
	std::vector<Choice*> sortChoice(std::vector<Choice*> _listChoice);

};

#endif // __HELLOWORLD_SCENE_H__
