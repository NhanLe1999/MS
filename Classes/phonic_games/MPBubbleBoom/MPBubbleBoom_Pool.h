#ifndef MJ_GAME_MPBUBBLE_BOOM_H
#define MJ_GAME_MPBUBBLE_BOOM_H

#include "MPBubbleBoom_Item.h"

class MPBubbleBoom_Pool
{
public:
	static MPBubbleBoom_Pool * getInstance();
	MPBubbleBoom_Item * getItem();
	void releaseItem(MPBubbleBoom_Item * item);
	~MPBubbleBoom_Pool();
	void releaseAllItem();
private:

	std::vector<MPBubbleBoom_Item* > resources;
	static MPBubbleBoom_Pool * instance;
	MPBubbleBoom_Pool();
};

#endif // !MJ_GAME_MPBUBBLE_BOOM_H
