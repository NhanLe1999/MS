
#ifndef GearsForCar_Gear_HPP
#define GearsForCar_Gear_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#include "ui/UIVideoPlayer.h"
using namespace cocos2d::experimental::ui;
#endif
USING_NS_CC;
using namespace ui;

class GearsForCar_Gear : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(GearsForCar_Gear);
    static GearsForCar_Gear * createItem();
    
    void setSlot(bool isEmpty);
    void setCorrect();
    void setCallBack(const std::function<void()> afunc);
    
    bool isEmpty;
private:
    void onEnter() override;
	void didLoadFromCSB();
    
    cocos2d::Size screen_size;
    cocos2d::ui::ImageView *gear;
    GAFWrapper *flower;
    std::function<void()> callBackFunc=nullptr;
};

CREATE_READER(GearsForCar_Gear);

#endif
