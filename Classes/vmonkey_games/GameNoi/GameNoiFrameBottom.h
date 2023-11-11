//
//  GameNoiFrameBottom.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/23/17.
//	Cloned by MinhTzy on 29/05/2019
//

#ifndef GameNoiFrameBottom_hpp
#define GameNoiFrameBottom_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class GameNoiFrameBottom : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameNoiFrameBottom);
    static GameNoiFrameBottom * createFrame(mj::GameObject object, bool is_image);

    void addConnectedPlug(cocos2d::Point position);
    std::string getName();
    void turnBulbOn();

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onBgFrame(cocos2d::Ref *sender);
    void didLoadFromCSB(mj::GameObject object, bool is_image);
    mj::GameObject _object;
    
};

CREATE_READER(GameNoiFrameBottom);
#endif /* GameNoiFrameBottom_hpp */
