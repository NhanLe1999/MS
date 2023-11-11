#pragma once
#include "FrogSentenceGame.h"
#include <unordered_set>

class SentenceGroup : public cocos2d::Node
{
public:
	CREATE_FUNC(SentenceGroup);
	virtual bool init() override;
	void showSentence(float initialDelay,const std::vector<WordSync> &words, std::string audio,
		std::function<void()> callback = nullptr, float callbackDelay = 0.2f);
	void showSentence(const std::vector<WordSync> &words, std::string audio,
		std::function<void()> callback = nullptr,float callbackDelay = 0.1f);
	void initLabels(const std::vector<WordSync> &words);
	void repeatSentence(const std::vector<WordSync> &words, const std::vector<WordSync>::iterator it,
		std::function<void(int auId, std::string auPath)> callback = nullptr, std::vector<math::resource::Sync_Text> listTextHight = {}, std::string m_audioPath = "",
		std::vector<TextLeaf*> m_leaves = {}, cocos2d::Color4B initilSyncTextColor = cocos2d::Color4B::WHITE, cocos2d::Color4B startSyncTextColor = cocos2d::Color4B::WHITE, cocos2d::Color4B endSyncTextColor = cocos2d::Color4B::RED);
	
protected:
	SentenceGroup();
	void flashLabel(Text* label);
    void setFontSz(const std::vector<WordSync>& words);
private:
	unordered_set<Text*> m_label_used;
	std::function<void()> m_completeCallback;
	bool m_dirtySentence;
	std::vector<Text*> m_labels;
    float font_sz = 60.0f;
};
