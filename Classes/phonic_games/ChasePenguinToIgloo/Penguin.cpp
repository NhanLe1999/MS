#include "Penguin.h"

Penguin * Penguin::createGAF(const std::string & gaf_name)
{
	auto wrapper = new Penguin();
	if (wrapper && wrapper->init(gaf_name)) {
		wrapper->autorelease();
		wrapper->playSequence("idle", true);
	}
	else {
		CC_SAFE_DELETE(wrapper);
		wrapper = nullptr;
	}

	return wrapper;
}

void Penguin::onChangeMoveDirection(MoveDirection direction)
{
	switch (direction)
	{
	case MovingAnimal::UP:
	case MovingAnimal::DOWNLEFT:
	case MovingAnimal::DOWNRIGHT:
	case MovingAnimal::LEFT:
	case MovingAnimal::RIGHT:
		if (m_currentSequence == "run")
			break;
		playSequence("run",true);
		m_currentSequence = "run";
		break;
	case MovingAnimal::IDLE:
	case MovingAnimal::NONE:
		if (m_currentSequence == "idle")
			break;
		playSequence("idle", true);
		m_currentSequence = "idle";
		break;
	default:
		break;
	}

	if (direction != IDLE && direction != NONE)
		setFlippedX(direction == RIGHT || direction == DOWNRIGHT);
}

void Penguin::setDestinationPoint(cocos2d::Vec2 point, std::function<void()> callback)
{
	MovingAnimal::setDestinationPoint(point);
	m_reachDestinationCallback = callback;
}

void Penguin::setPosition(const cocos2d::Vec2 & pos)
{
	MovingAnimal::setPosition(pos);
	if (pos == m_destinationPoint && m_reachDestinationCallback) {
		m_reachDestinationCallback();
		m_reachDestinationCallback = nullptr;
	}
}

bool Penguin::isAttached()
{
	return m_attached;
}

bool Penguin::isWithinBigFoot()
{
	return _withinBigFoot;
}

void Penguin::setAttached(bool isAttach)
{
	m_attached = isAttach;
	if (isAttach)
		setDesiredSpeed(7 * ANIMAL_MOVE_SPEED);
	else
		setDesiredSpeed(ANIMAL_MOVE_SPEED);
}

void Penguin::setWithinBigFoot(bool within)
{
	_withinBigFoot = within;
}

Penguin::Penguin() :
	m_attached(false),
	_withinBigFoot(false),
	m_currentSequence("")
{
}
