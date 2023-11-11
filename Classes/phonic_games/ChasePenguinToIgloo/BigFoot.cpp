#include "BigFoot.h"
#include "Penguin.h"
#include "HSAudioEngine.h"
#include <cmath>

#define BASE_ATTACH_ANGLE (M_PI * 3/2)
#define BIG_FOOT_RADIUS 130.0f
#define MAX_ATTACHES_NUMBER 10
#define MAX_ATTACHES_ANGLE 5
#define ANGLE_BETWEEN_PENGUINS (M_PI/MAX_ATTACHES_ANGLE)

BigFoot * BigFoot::createGAF(const std::string & gaf_name)
{
	auto wrapper = new BigFoot();
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

void BigFoot::onChangeMoveDirection(MoveDirection direction)
{
	switch (direction) {
	case NONE:
	case IDLE:
		playSequence("idle", true);
		AUDIO_ENGINE->stopSound("sounds/chasepenguintoigloo/front_bigfoot.mp3");
		AUDIO_ENGINE->stopSound("sounds/chasepenguintoigloo/side_bigfoot.mp3");
		break;
	case UP:
		// skip this case, no move up animation
		m_destinationAngle = -M_PI / 2;
		m_offset2 = cocos2d::Vec2(0, -70);
		break;
	case DOWNLEFT:
	case DOWNRIGHT:
		m_destinationAngle = M_PI/2;
		m_offset2 = cocos2d::Vec2(0, -70);
		playSequence("front", true);
		AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/front_bigfoot.mp3", true);
		AUDIO_ENGINE->stopSound("sounds/chasepenguintoigloo/side_bigfoot.mp3");
		break;
	case LEFT:
	case RIGHT:
		m_destinationAngle = M_PI / 2;
		playSequence("side", true);
		AUDIO_ENGINE->stopSound("sounds/chasepenguintoigloo/front_bigfoot.mp3");
		AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/side_bigfoot.mp3", true);
		break;
	}

	if (direction == LEFT) {
		m_destinationAngle = M_PI / 2;
		//m_offset2 = cocos2d::Vec2(70, 0);
	}
	else if (direction == RIGHT) {
		m_destinationAngle = M_PI / 2;
		//m_offset2 = cocos2d::Vec2(-70, 0);
	}

	setFlippedX(direction == LEFT || direction == DOWNLEFT);
	refreshAttachesPosition();
}

void BigFoot::attach(Penguin * penguin)
{
	if (m_slots.size() >= MAX_ATTACHES_NUMBER)
		return;
	auto it = m_slots.begin();
	// calculate angle
	auto anglePath = penguin->getPosition() - getPosition();
	auto angle = anglePath.getAngle() + M_PI * 2;
	while (it != m_slots.end() && it->desiredAngle < angle)
	{
		it++;
	}
	Slot slot;
	slot.pPenguin = penguin;
	slot.desiredAngle = (int) ( anglePath.getAngle() / ANGLE_BETWEEN_PENGUINS ) * ANGLE_BETWEEN_PENGUINS ;
	penguin->setAttached(true);
	//it->desiredAngle = anglePath.getAngle();
	m_slots.insert(it, slot);
	refreshAttachesPosition();
}

void BigFoot::detachAll()
{
	for (auto it = m_slots.begin(); it != m_slots.end();) {
		it->pPenguin->setAttached(false);
		it = m_slots.erase(it);
	}
}

void BigFoot::setSafeArea(cocos2d::Rect & rect)
{
	m_safeArea = rect;
}

cocos2d::Vec2 BigFoot::getDestinationPosition()
{
	return m_destinationPoint;
}

void BigFoot::setPosition(const cocos2d::Vec2 & pos)
{
	MovingAnimal::setPosition(pos);
	refreshAttachesPosition();
}

void BigFoot::update(float delta)
{
	MovingAnimal::update(delta);
	if (m_destinationAngle > m_baseAngle) {
		m_baseAngle += m_rotationSpeed * delta;
		m_baseAngle = std::min(m_baseAngle, m_destinationAngle);
	}
	else {
		m_baseAngle -= m_rotationSpeed * delta;
		m_baseAngle = std::max(m_baseAngle, m_destinationAngle);
	}

	if (m_baseAngle > 2 * M_PI) {
		m_baseAngle -= 2 * M_PI;
		m_destinationAngle -= 2 * M_PI;
	}
}

std::vector<BigFoot::Slot> BigFoot::getSlots()
{
	return m_slots;
}

BigFoot::BigFoot() :
	m_baseAngle(BASE_ATTACH_ANGLE),
	m_destinationAngle(BASE_ATTACH_ANGLE),
	m_rotationSpeed(M_PI),
	m_offset2(0, -70)
{
}

void BigFoot::refreshAttachesPosition()
{
	if (m_slots.size() == 0)
		return;
	auto counter = 0;
	//if (m_direction == IDLE || m_direction == NONE) {
	//	auto baseAngle = M_PI * 3 / 2;
	//	std::vector<Slot> sortZorderArr(m_slots);
	//	auto offsetAngle = M_PI * 2 / m_slots.size();
	//	for (auto it = m_slots.begin(); it != m_slots.end(); it++) {
	//		it->desiredAngle = baseAngle + counter * offsetAngle;
	//		auto offset = cocos2d::Vec2(
	//			BIG_FOOT_RADIUS * cosf(it->desiredAngle),
	//			BIG_FOOT_RADIUS * sinf(it->desiredAngle) * 0.8f - 35.0f// Y-scale
	//		);
	//		it->pPenguin->setDestinationPoint(getPosition() + offset);
	//		counter++;
	//	}

	//	std::sort(sortZorderArr.begin(), sortZorderArr.end(), [](Slot l, Slot r)->bool {
	//		return l.pPenguin->getDestinationPoint().y > r.pPenguin->getDestinationPoint().y;
	//	});
	//	counter = -10;
	//	for (auto it = sortZorderArr.begin(); it != sortZorderArr.end(); it++) {
	//		if (counter < 0 && it->pPenguin->getDestinationPoint().y < getPosition().y - 35.0f)
	//			counter = 1;
	//		it->pPenguin->setLocalZOrder(counter++);
	//	}
	//	return;
	//}
	auto baseAngle1 = m_baseAngle - (std::min(MAX_ATTACHES_ANGLE, (int)m_slots.size()) - 1) * ANGLE_BETWEEN_PENGUINS /2;
	auto baseAngle2 = m_baseAngle - (std::min(MAX_ATTACHES_ANGLE, (int)(m_slots.size() - MAX_ATTACHES_ANGLE)) - 1) * ANGLE_BETWEEN_PENGUINS / 2;

	auto centerPoint1 = getPosition();
	auto centerPoint2 = getPosition() + m_offset2;
	if (centerPoint1.x < centerPoint2.x || centerPoint1.y > centerPoint2.y) {
		auto temp = centerPoint1;
		centerPoint1 = centerPoint2;
		centerPoint2 = temp;
	}
	auto radius = BIG_FOOT_RADIUS;
	if (m_baseAngle != m_destinationAngle)
		radius += 30;
	std::map<float, int> angle_count;
	for (auto it = m_slots.begin(); it != m_slots.end(); it++) {
		auto cp = centerPoint1;
		/*if (counter / MAX_ATTACHES_ANGLE) {
			it->desiredAngle = baseAngle2 + (counter % MAX_ATTACHES_ANGLE) * ANGLE_BETWEEN_PENGUINS;
			cp = centerPoint2;
		}
		else
			it->desiredAngle = baseAngle1 + (counter % MAX_ATTACHES_ANGLE) * ANGLE_BETWEEN_PENGUINS;
*/
		int ac = 0;
		auto aci = angle_count.find(it->desiredAngle);
		if (aci != angle_count.end()) {
			ac = aci->second + 1;
			if (ac < MAX_ATTACHES_ANGLE / 2) {
				aci->second += 1;
			}
			else {

				for (int i = 1;;++i) {
					it->desiredAngle -= ANGLE_BETWEEN_PENGUINS;
					auto acit = angle_count.find(it->desiredAngle);
					if (acit == angle_count.end()) {
						ac = 0;
						angle_count.insert({ it->desiredAngle,0 });
						break;
					}
				/*	else if (acit->second < 1) {
						ac = acit->second + 1;
						acit->second += 1;
						break;
					}*/
				}
			}
			
		}
		else {
			angle_count.insert({ it->desiredAngle,0 });
		}
		radius = BIG_FOOT_RADIUS + ac * 35;
		auto offset = cocos2d::Vec2(
			radius * cosf(it->desiredAngle),
			radius * sinf(it->desiredAngle)
		);
		/*if (counter / MAX_ATTACHES_ANGLE)
			offset += m_offset2;*/
		it->pPenguin->setDestinationPoint(cp + offset);
		//it->pPenguin->setLocalZOrder((counter / MAX_ATTACHES_ANGLE) + 1);
		counter++;
	}

	std::vector<Slot> sortZorderArr(m_slots);
	std::sort(sortZorderArr.begin(), sortZorderArr.end(), [](Slot l, Slot r)->bool {
		return l.pPenguin->getDestinationPoint().y > r.pPenguin->getDestinationPoint().y;
	});
	counter = -10;
	for (auto it = sortZorderArr.begin(); it != sortZorderArr.end(); it++) {
		if (counter < 0 && it->pPenguin->getDestinationPoint().y < getPosition().y - 35.0f)
			counter = 1;
		it->pPenguin->setLocalZOrder(counter++);
	}

}