#ifndef PUZZLE_H
#define PUZZLE_H
#include "cocostudiomacro2.h"
#include "PartChoice.h"
#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
#include <vector>
#include <iostream>
#include <random>

class Puzzle : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public PartChoiceDelegate {

public:
	CREATE_FUNC(Puzzle);
	static Puzzle * createGame();

	virtual void touched(PartChoice* partChoice);
	virtual void released(PartChoice* partChoice);

	void makeGame();

private:
	cocos2d::Node* rootNode;
	cocos2d::ui::ImageView* frame;
	cocos2d::ui::ImageView* frameBg;
	cocos2d::ui::Layout* imageThis;
	int level = 0, numChoices, cntCorrect;
	std::string searchFolder;
	cocos2d::Vector<PartChoice*> vecChoices;
	cocos2d::Vector<DrawNode*> vecDraws;

	cocos2d::Size originalImageContentSize;

	GAFWrapper* gaff;
	GAFWrapper* gaff1;
	GAFWrapper* gaff2;
	GAFWrapper* gaff3;
	GAFWrapper* gaff4;

	cocos2d::Vec2 centerPosition;

	cocos2d::ui::ImageView* createImage(std::string fileName);

	void initAll();
	void introduction();
	void triggerStart();
	void animateNewLevel();
	void initImages(cocos2d::Size _size);	
	std::vector<std::pair<float, float> > getPositions(cocos2d::Size _size);

	std::vector<std::pair<float, float> > getMovePositions(cocos2d::Size _size, cocos2d::Vec2 pos, cocos2d::Size _oSize);
	std::vector<std::pair<float, float> > getMoveSize(cocos2d::Size _size, cocos2d::Vec2 pos, cocos2d::Size _oSize);

	void makeLines(cocos2d::ui::ImageView* parent, cocos2d::Size targetSize);
	void makeLines(cocos2d::ui::Layout* parent, cocos2d::Size targetSize);

	cocos2d::Vector<cocos2d::FiniteTimeAction*> makeThisSize(cocos2d::Size _size1, cocos2d::Size _size2, float duration, ui::ImageView* imageThis);	
	cocos2d::Vector<cocos2d::FiniteTimeAction*> makeThisSize(cocos2d::Size _size1, cocos2d::Size _size2, float duration, PartChoice* imageThis);	

	void madeWrong(PartChoice* partChoice);
	void madeCorrect(PartChoice* partChoice);
	void runWrongAnimation(PartChoice* partChoice);
	void runCorrectAnimation(PartChoice* partChoice);
	void runEndingAnimation();

	cocos2d::Size targetSize;

};

CREATE_READER(Puzzle);

#endif

