#pragma once

#include "GAFWrapper.h"

class Goat : public GAFWrapper
{
public:
	static Goat * createGAF(const std::string &gaf_name);
	void jump(float height,std::function<void()> landCallback = nullptr);
	void setTimeScale(float timeScale);
	virtual void update(float dt) override;
	virtual void onEnter() override;
	virtual void onExit() override;
protected:
	Goat();
	void setSequence(std::string sequence);
	bool m_isJumping;
	float m_baseVerticalPos;
	float m_velocity;
	float m_gravity;
	float m_timeScale;
	std::string m_sequence;
	std::function<void()> m_landCallback;
private:
};