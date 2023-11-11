#pragma once

#include "MovingAnimal.h"

class Penguin : public MovingAnimal
{
public:
	static Penguin* createGAF(const std::string &gaf_name);
	virtual void onChangeMoveDirection(MoveDirection direction) override;
	void setDestinationPoint(cocos2d::Vec2 point,std::function<void()> callback = nullptr);
	virtual void setPosition(const cocos2d::Vec2 &pos) override;
	bool isAttached();
	bool isWithinBigFoot();
	void setAttached(bool isAttach);
	void setWithinBigFoot(bool within);
protected:
	Penguin();
	bool m_attached;
	bool _withinBigFoot = false;
	std::string m_currentSequence;
private:
};