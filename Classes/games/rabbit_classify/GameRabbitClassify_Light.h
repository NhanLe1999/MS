
#ifndef Rabbit_classify_light_h
#define Rabbit_classify_light_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

class GameRabbitClassify_Light : public cocos2d::Node {
public:
    CREATE_FUNC(GameRabbitClassify_Light);
	bool isOn = false;
	bool init();
    static GameRabbitClassify_Light * createItem();
    void onLightOn();
};

CREATE_READER(GameRabbitClassify_Light);

#endif
