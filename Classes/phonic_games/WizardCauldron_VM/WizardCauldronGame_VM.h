#pragma once

#include "MJMainGBase.h"
#include "WordFrame_VM.h"
#include "GameData.h"

namespace WizardCauldron_VM {
	class TextNode_VM;
}

class WizardCauldronGame_VM : public MJMainGBase
{
public:
	static WizardCauldronGame_VM * createGame(std::string json_file, bool isAutoPlay);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	CREATE_FUNC(WizardCauldronGame_VM);
protected:
	WizardCauldronGame_VM();
	void cacheTexture();
	void initBackground();
	void initCauldron();
	void initStones();
	void initWordFrame_VM();
	void onTouchTextNode_VM(WizardCauldron_VM::TextNode_VM*);
	bool onReleaseTextNode_VM(WizardCauldron_VM::TextNode_VM*);
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
private:
	std::string m_configPath;
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_bgNode;
	GAFWrapper* m_cauldron;
	GAFWrapper* m_wizard;
	cocos2d::Node* m_clickBlockLayer;
	std::vector<mj::GameObject> m_correctLetters;
	std::vector<mj::GameObject> m_answers;
    mj::GameObject m_text_khong_dau;
    mj::GameObject m_text_dau;
    int pos_dau;
    bool co_dau;
	int m_letterIndex;
	mj::GameObject m_correctString;
	mj::GameObject m_correctImage;
	cocos2d::Node* m_correctSprite;
	cocos2d::Node* m_answersNode;
	cocos2d::Node* m_thinkingNode;
	cocos2d::Node* m_bgSprite;
	WizardCauldron_VM::WordFrame_VM* m_WordFrame_VM;
	gaf::GAFAsset* m_effectAsset;
	bool m_guidePlayed;
	int m_gameIndex;
	int m_consecutiveWrong;
    std::string question_vi = "";
};
