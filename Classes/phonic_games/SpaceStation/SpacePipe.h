#pragma once
#include "cocos2d.h"
#include "GameData.h"

class SpacePipe : public cocos2d::Node
{
public:
	CREATE_FUNC(SpacePipe);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	void setText(mj::GameObject text);
	mj::GameObject getText();
	void attach(cocos2d::Vec2 position);
	void detach();
	void detach(cocos2d::Vec2 pos);
	void hideText();
	void revealText();
	void setReleaseCallback(std::function<void(SpacePipe*)> callback = nullptr);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
protected:
	SpacePipe();
private:
	bool m_attached;
	cocos2d::Sprite* m_sprite;
    cocos2d::Label* m_label;
	cocos2d::Vec2 m_lastTouchPosition;
	mj::GameObject m_text;
	std::function<void(SpacePipe*)> m_releaseCallback;
};
