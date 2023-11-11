#include "MovingAnimal.h"

MovingAnimal * MovingAnimal::createGAF(const std::string & gaf_name)
{
	auto wrapper = new MovingAnimal();
	if (wrapper && wrapper->init(gaf_name)) {
		wrapper->autorelease();
	}
	else {
		CC_SAFE_DELETE(wrapper);
		wrapper = nullptr;
	}

	return wrapper;
}

void MovingAnimal::setDestinationPoint(cocos2d::Vec2 point, std::function<void()> callback)
{
	if (m_desiredSpeed <= 0) {
		setMoveDirection(IDLE);
		return;
	}
	if (point == m_destinationPoint)
		return;

	auto movePath = point - getPosition();
	auto angle = movePath.getAngle();
	if (-M_PI_4 < angle && angle <= M_PI_2) {
		setMoveDirection(MoveDirection::RIGHT);
	}
	else if (angle >= M_PI_2 || (angle >= -M_PI && angle <= -3 * M_PI_4)) {
		setMoveDirection(MoveDirection::LEFT);
	}
	else if (angle >= -3 * M_PI_4 && angle <= -M_PI_2) {
		setMoveDirection(MoveDirection::DOWNLEFT);
	}
	else if (angle >= -M_PI_2 && angle <= -M_PI_4) {
		setMoveDirection(MoveDirection::DOWNRIGHT);
	}
	else {
		setMoveDirection(MoveDirection::RIGHT);
	}
	m_destinationPoint = point;
	m_reachDestinationCallback = callback;
}

cocos2d::Vec2 MovingAnimal::getDestinationPoint()
{
	return m_destinationPoint;
}

void MovingAnimal::setPosition(const cocos2d::Vec2 & pos)
{
	GAFWrapper::setPosition(pos);
	if (pos == m_destinationPoint && m_reachDestinationCallback) {
		auto callback = m_reachDestinationCallback;
		m_reachDestinationCallback = nullptr;
		callback();
	}
}

void MovingAnimal::setDesiredSpeed(float speed)
{
	m_desiredSpeed = speed;
}

void MovingAnimal::update(float delta)
{
	GAFWrapper::update(delta);
	if (m_destinationPoint.length() == 0)
		return;
	auto movePath = m_destinationPoint - getPosition();
	auto moveDistance = delta * m_desiredSpeed;
	auto offset = movePath.getNormalized() *sqrtf(moveDistance);
	auto destination = offset.length() < movePath.length() ? (offset + getPosition()) : (movePath + getPosition());
	if (getPosition() == m_destinationPoint)
		setMoveDirection(IDLE);
	setPosition(destination);
}

void MovingAnimal::onEnter()
{
	GAFWrapper::onEnter();
	scheduleUpdate();
}

void MovingAnimal::onExit()
{
	GAFWrapper::onExit();
	unscheduleUpdate();
}

MovingAnimal::MovingAnimal() :
	m_desiredSpeed(0),
	m_direction(MoveDirection::IDLE),
	m_destinationPoint(0, 0),
	m_reachDestinationCallback(nullptr)
{
}

bool MovingAnimal::init(const std::string & gaf_name)
{
	bool result = GAFWrapper::init(gaf_name);
	if (!result)
		return false;
	return true;
}

void MovingAnimal::onChangeMoveDirection(MoveDirection direction)
{
}

void MovingAnimal::setMoveDirection(MoveDirection direction)
{
	if (direction != m_direction) {
		m_direction = direction;
		onChangeMoveDirection(direction);
	}
}
