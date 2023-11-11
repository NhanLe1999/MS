#pragma once
#include "cocos2d.h"
#include "GameData.h"

USING_NS_CC;
class SpacePipeSlot : public cocos2d::Node{
public:
	CREATE_FUNC(SpacePipeSlot);
	bool init() override;
	void setWord(mj::GameObject word);
	mj::GameObject getWord();
protected:
	SpacePipeSlot();
private:
	mj::GameObject m_word;
};