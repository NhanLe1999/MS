#pragma once

#include "cocos2d.h"

enum BackgroundAnchor {
	TOP,
	MIDDLE,
	BOTTOM
};

class BackgroundNode : public cocos2d::Node
{
public:
	static BackgroundNode* createBackground(std::string path, float moveSpeed, float scale = -1); // -1 : auto calculate scale
	static BackgroundNode* createBackground(std::string path1, std::string path2, float moveSpeed, float scale = -1); // -1 : auto calculate scale
	virtual void onEnter() override;
	virtual void onExit() override;
	void updateBackground(float delta);
	void setSpeedScale(float scale);
	void setMovingSpeed(float speed);
	float getMovingSpeed();
	void setBackgroundAnchor(BackgroundAnchor anchor);
	CREATE_FUNC(BackgroundNode);
protected:
	BackgroundNode();
	std::vector<cocos2d::Node*> m_backgrounds;
	float m_moveSpeed;
	float m_speedScale;
private:
	void initBackground(std::string path, float scale);
	void initBackground(std::string path1, std::string path2, float scale);
};