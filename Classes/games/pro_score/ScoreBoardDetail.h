#pragma once
#ifndef __MK_SCORE_BOARD_H__
#define __MK_SCORE_BOARD_H__
#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

class ScoreBoardDetailDelegate {
public:
	virtual void replayAudio() = 0;
};
class ScoreBoardDetail : public ui::Layout {

public:
	
	CREATE_FUNC(ScoreBoardDetail);
	
	static ScoreBoardDetail* createScoreBoardDetail(int score,std::string sentence, std::vector<std::pair<std::string, float>> wordSore);

	ScoreBoardDetailDelegate* getDelegate() {
		return _delegate;
	};

	void setDelegate(ScoreBoardDetailDelegate* dlgate) {
		_delegate = dlgate;
	}

private:

	void prepareData(int score, std::string sentence, std::vector<std::pair<std::string, float>> wordSore);
	void loadUI();
	void createElement();
	void alignElement(std::vector<Layout*> elements);
	Size calculateTotalSize(Layout* headLeft, std::vector<Layout*> elements);
	std::string scoreToIconPath(int score);
	void drawLines();
	void coloringImage();
	void arrangeElement();
	Color3B scoreToColor(int score);
	std::string scoreToColorString(int score);
	std::string scoreToMessage(int score);
	void createTextElements();
	RichText* createColoringText();
	void fitToScreen();
	void disableEarButton(Button* earButton);

	int _score;
	std::string _sentence;
	std::vector<std::pair<std::string, float>> _wordScore;
	ImageView* _background;
	float _fontSize = 60;
	std::vector<Layout*> _rightElements;
	Layout* _headLeft;
	std::vector<Text*> _listTextElements;
	ui::RichText* _fullText;

	ScoreBoardDetailDelegate* _delegate = nullptr;

};
#endif
