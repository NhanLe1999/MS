//
//  GameLazerFrameBottom.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/23/17.
//
//

#ifndef GameLazerFrameBottom_hpp
#define GameLazerFrameBottom_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameLazerFrameBottom : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameLazerFrameBottom);
    static MJGameLazerFrameBottom * createFrame(mj::GameObject object, bool is_image, bool isTextLength = false);

    void addConnectedPlug(cocos2d::Point position);
    std::string getName();
    void turnBulbOn();
    void setAudio(std::string s){
        _audio_path = s;
    }
    std::string getAudio(){
        return _audio_path;
    }
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onBgFrame(cocos2d::Ref *sender);
    void didLoadFromCSB(mj::GameObject object, bool is_image, bool isTextLength = false);
    mj::GameObject _object;
    std::string _audio_path;
    bool _isPlayAudio = true;
    
};

CREATE_READER(MJGameLazerFrameBottom);
#endif /* GameLazerFrameBottom_hpp */
