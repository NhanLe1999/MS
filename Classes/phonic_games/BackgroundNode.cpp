#include "BackgroundNode.h"

BackgroundNode * BackgroundNode::createBackground(std::string path, float moveSpeed, float scale)
{
	auto result = BackgroundNode::create();
	result->initBackground(path, scale);
	result->setMovingSpeed(moveSpeed);
	return result;
}

BackgroundNode * BackgroundNode::createBackground(std::string path1, std::string path2, float moveSpeed, float scale)
{
	auto result = BackgroundNode::create();
	result->initBackground(path1, path2, scale);
	result->setMovingSpeed(moveSpeed);
	return result;
}

void BackgroundNode::onEnter()
{
	Node::onEnter();
	//scheduleUpdate();
	schedule(schedule_selector(BackgroundNode::updateBackground), 0);
	
}

void BackgroundNode::onExit()
{
	Node::onExit();
	unschedule(schedule_selector(BackgroundNode::updateBackground));
	//unscheduleUpdate();
}

void BackgroundNode::updateBackground(float delta)
{
	auto moveSpeed = m_moveSpeed * m_speedScale;
	static auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (auto it = m_backgrounds.begin(); it != m_backgrounds.end(); it++) {
		auto bg = *it;
		bg->setPosition(bg->getPosition() + cocos2d::Vec2(-moveSpeed * delta, 0)) ;
		if (m_moveSpeed > 0 && bg->getPosition().x < -bg->getContentSize().width * bg->getScale() / 2)
			bg->setPosition(bg->getPosition() + cocos2d::Vec2(bg->getContentSize().width * bg->getScale() - 1.5, 0) * 2);
		if (m_moveSpeed < 0 && bg->getPosition().x -bg->getContentSize().width * bg->getScale() / 2 > visibleSize.width)
			bg->setPosition(bg->getPosition() - cocos2d::Vec2(bg->getContentSize().width * bg->getScale() - 1.5, 0) * 2);
	}
}

void BackgroundNode::setSpeedScale(float scale)
{
	m_speedScale = scale;
}

void BackgroundNode::setMovingSpeed(float speed)
{
	m_moveSpeed = speed;
}

float BackgroundNode::getMovingSpeed()
{
	return m_speedScale * m_moveSpeed;
}

void BackgroundNode::setBackgroundAnchor(BackgroundAnchor anchor)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (auto it = m_backgrounds.begin(); it != m_backgrounds.end(); it++) {
		auto bg = (*it);
		auto pos = bg->getPosition();
		switch (anchor)
		{
		case TOP:
			pos.y = visibleSize.height;
			bg->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
			break;
		case MIDDLE:
			pos.y = visibleSize.height / 2;
			bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
			break;
		case BOTTOM:
			pos.y = 0;
			bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
			break;
		default:
			break;
		}
		bg->setPosition(pos);
	}
}

BackgroundNode::BackgroundNode()
	:m_moveSpeed(0),
	m_speedScale(1)
{
}

void BackgroundNode::initBackground(std::string path, float scale)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (auto i = 0; i < 2; i++) {
		auto bg = cocos2d::Sprite::create(path);
		if (!bg)
			return;
		if (scale == -1)
			scale = std::max((visibleSize.width + 3) / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
		bg->setScale(scale);
		bg->setPosition(cocos2d::Vec2(visibleSize / 2) + i * cocos2d::Vec2(bg->getContentSize().width * bg->getScale() - 1.5, 0));
		addChild(bg);
		m_backgrounds.push_back(bg);
	}
}

void BackgroundNode::initBackground(std::string path1, std::string path2, float scale)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (auto i = 0; i < 2; i++) {
		auto bg1 = cocos2d::Sprite::create(path1);
		auto bg2 = cocos2d::Sprite::create(path2);
		auto bg = cocos2d::Node::create();
		bg->setContentSize(cocos2d::Size(bg1->getContentSize().width + bg2->getContentSize().width, bg1->getContentSize().height));
		bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		bg1->setPosition(bg1->getContentSize() / 2);
		bg2->setPosition(cocos2d::Vec2(bg1->getContentSize().width,0) + bg2->getContentSize() / 2);
		bg->addChild(bg1);
		bg->addChild(bg2);
		if (!bg)
			return;
		if (scale == -1)
			scale = std::max((visibleSize.width + 3) / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
		bg->setScale(scale);
		bg->setPosition(cocos2d::Vec2(visibleSize / 2) + i * cocos2d::Vec2(bg->getContentSize().width * bg->getScale() - 1.5, 0));
		addChild(bg);
		m_backgrounds.push_back(bg);
	}
}
