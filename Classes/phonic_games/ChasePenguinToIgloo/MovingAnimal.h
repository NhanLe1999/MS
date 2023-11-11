#pragma once

#include "GAFWrapper.h"

#define ANIMAL_MOVE_SPEED 1800

class MovingAnimal : public GAFWrapper
{
public:
	static MovingAnimal* createGAF(const std::string &gaf_name);
	void setDestinationPoint(cocos2d::Vec2 point, std::function<void()> callback = nullptr);
	cocos2d::Vec2 getDestinationPoint();
	virtual void setPosition(const cocos2d::Vec2 &pos) override;
	void setDesiredSpeed(float speed);
	virtual void update(float delta) override;
	virtual void onEnter() override;
	virtual void onExit() override;
protected:
	enum MoveDirection
	{
		UP,
		DOWNLEFT,
		DOWNRIGHT,
		LEFT,
		RIGHT,
		IDLE,
		NONE
	};
 	bool init(const std::string & gaf_name) override;
	virtual void onChangeMoveDirection(MoveDirection direction);
	MovingAnimal();
	float m_desiredSpeed;
	MoveDirection m_direction;
	cocos2d::Vec2 m_destinationPoint;
	std::function<void()> m_reachDestinationCallback;
private:
	void setMoveDirection(MoveDirection direction);
};