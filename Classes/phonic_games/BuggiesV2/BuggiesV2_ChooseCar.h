//
//  MJ_BuggiesV2_ChooseCar.h
//

#ifndef BuggiesV2_ChooseCar_h
#define BuggiesV2_ChooseCar_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
class BuggiesV2_ChooseCarDelegate {
public:
    virtual void onChooseCar(std::string car_index, int map_index) {};
};

class BuggiesV2_ChooseCar : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    
    enum {
        MAP_1,
        MAP_2,
    };
    
    CREATE_FUNC(BuggiesV2_ChooseCar);
    static BuggiesV2_ChooseCar *createView(int type);
    void setDelegate(BuggiesV2_ChooseCarDelegate *delegate);
	void onSelectedCar(cocos2d::Ref * sender);
	void onClickedNext(cocos2d::Ref * sender);
	void onClickedBack(cocos2d::Ref * sender);
	void onClickedLetsGo(cocos2d::Ref * sender);
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(int type);
    void onEnter() override;
    BuggiesV2_ChooseCarDelegate *_delegate = nullptr;
    std::string car_index;
    int map_index;
    cocos2d::ui::ImageView* _imgGameName = nullptr;
    //void onChooseCar(cocos2d::Ref *sender);
};
CREATE_READER(BuggiesV2_ChooseCar);
#endif /* MJ_BuggiesV2_ChooseCar_h */
