#pragma once

#include "MovingAnimal.h"

class Penguin;

class BigFoot : public MovingAnimal
{
public:
	struct Slot {
		float desiredAngle = 0;
		Penguin* pPenguin = nullptr;
	};
	static BigFoot* createGAF(const std::string &gaf_name);
	virtual void onChangeMoveDirection(MoveDirection direction) override;
	void attach(Penguin* penguin);
	void detachAll();
	void setSafeArea(cocos2d::Rect& rect);
	cocos2d::Vec2 getDestinationPosition();
	virtual void setPosition(const cocos2d::Vec2 &pos) override;
	void update(float delta) override;
	std::vector<Slot> getSlots();

protected:
	BigFoot();
	void refreshAttachesPosition();
private:
	std::vector<Slot> m_slots;
	cocos2d::Rect m_safeArea;
	float m_baseAngle;
	float m_destinationAngle;
	float m_rotationSpeed;
	cocos2d::Vec2 m_offset2;
};