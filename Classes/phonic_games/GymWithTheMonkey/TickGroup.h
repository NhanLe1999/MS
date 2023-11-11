#pragma once

#include "cocos2d.h"
#include "GAFWrapper.h"

class TickGroup : public cocos2d::Node
{
public:
    bool init() override;
	bool pushTick();
	void resetTicks();
	int getTickCount();
	int getNumberOfTicks();
	bool isFull();
//    static TickGroup* create(int number);
    CREATE_FUNC(TickGroup);
    bool setNumber(int number);

protected:
	int m_tickCount;
	std::vector<GAFWrapper*> m_ticks;
	std::vector<GAFWrapper*> m_faceticks;
	int m_numberOfTicks;

private:
};
