#pragma once

#include "MJMainGBase.h"
#include "WordFrame.h"
#include "GameData.h"

namespace WizardCauldron {
	class TextNode;
}

class WizardCauldronGame : public MJMainGBase
{
public:
	static WizardCauldronGame * createGame(std::string json_file, bool isAutoPlay);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	CREATE_FUNC(WizardCauldronGame);
protected:
	WizardCauldronGame();
	void cacheTexture();
	void initBackground();
	void initCauldron();
	void initStones();
	void initWordFrame();
	void onTouchTextNode(WizardCauldron::TextNode*);
	bool onReleaseTextNode(WizardCauldron::TextNode*);
	void getGameData();
	void raiseClickBlockLayer();
	void destroyClickBlockLayer();
	void playShowTextScript(mj::GameObject text, Color4B textColor, std::function<void()> callback = nullptr);
	bool checkWinCondition();
	void playEndingScript();
	void initThinkingNode();
	void startGame();
	void nextGame();
	void playAmbientSounds();
	void stopAmbientSounds();
	void playGuide();
	void playSynctext(std::vector<Label*> nodes, std::function<void()> callback = nullptr);
	void playGuideScript();
	void moveTheCorrectAnswer(WizardCauldron::TextNode* node, Vec2 destinationPos, Vec2 path);
	void HintsForTheGame();
	void runActionHand();
private:
	bool _hideText = false;
	std::string m_configPath;
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_bgNode;
	GAFWrapper* m_cauldron;
	GAFWrapper* m_wizard;
	cocos2d::Node* m_clickBlockLayer;
	std::vector<mj::GameObject> m_correctLetters;
	std::vector<mj::GameObject> m_answers;
	int m_letterIndex;
	mj::GameObject m_correctString;
	mj::GameObject m_correctImage;
	cocos2d::Node* m_correctSprite;
	cocos2d::Node* m_answersNode;
	cocos2d::Node* m_thinkingNode;
	cocos2d::Node* m_bgSprite;
	WizardCauldron::WordFrame* m_wordFrame;
	gaf::GAFAsset* m_effectAsset;
	bool m_guidePlayed;
	int m_gameIndex;
	int m_consecutiveWrong;
	int _numberAge = 0;
	bool _isInteractWithTheGame = false;
	std::string _auioIntroGme = "";
	std::vector<std::pair<std::string, cocos2d::Vec2>> _listPositionTextNode = {};
};
