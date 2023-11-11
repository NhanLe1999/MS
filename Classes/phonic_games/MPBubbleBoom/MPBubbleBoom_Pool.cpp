#include "MPBubbleBoom_Pool.h"

MPBubbleBoom_Pool * MPBubbleBoom_Pool::instance = nullptr;

MPBubbleBoom_Pool * MPBubbleBoom_Pool::getInstance()
{
	if (instance == nullptr) {
		instance = new MPBubbleBoom_Pool();
	}
	return instance;
}

MPBubbleBoom_Item * MPBubbleBoom_Pool::getItem()
{
	MPBubbleBoom_Item * item = nullptr;

	if (resources.size() > 0) {
		item = resources.back();
		resources.pop_back();
	}

	return item;
}

void MPBubbleBoom_Pool::releaseItem(MPBubbleBoom_Item * item)
{
	item->retain();
	item->resetBubble();
	resources.push_back(item);
}

MPBubbleBoom_Pool::~MPBubbleBoom_Pool()
{

}

void MPBubbleBoom_Pool::releaseAllItem()
{
	for (auto item : resources) {
		item->release();
	}
	resources.clear();
}

MPBubbleBoom_Pool::MPBubbleBoom_Pool()
{
}
