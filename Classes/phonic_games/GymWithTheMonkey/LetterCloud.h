#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
class CloudSelectListener {
public:
	virtual void onSelectLetter(std::string letter) = 0;
};

class LetterCloud : public cocos2d::Node
{
public:
	virtual bool init();
	void setLetter(std::string letter,bool skipAnimation = false);
	void setTextureColor(int textureColor);
	std::string getLetter();
	cocos2d::Label * getLabel();
	void setListener(CloudSelectListener*);
	void playAnimation(bool correct,std::function<void()> callback = nullptr);
	CREATE_FUNC(LetterCloud);
	void fitValueTextSize();
    std::string m_audio = "";
	cocos2d::ui::Button* cloudBtn;
	cocos2d::ui::Button* m_button;
	void setClickAbleStatus(bool isStautus);
protected:
    
	CloudSelectListener* m_listener;
	std::string m_currentLetter;
	cocos2d::Label* m_label;
	int m_textureColor;
private:
};
