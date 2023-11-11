//
//  MSIntro.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/27/18.
//

#ifndef MSIntro_hpp
#define MSIntro_hpp

#include "cocostudiomacro2.h"

class MSIntro : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSIntro);
    static MSIntro * createIntro();
    static cocos2d::Scene * createScene();
    
    void reloadIntro();
    
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string & name) override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

    void onTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touch);
    void onStart(cocos2d::Ref * sender);
    void onSelectItem(std::string item_name);
    
    enum IntroScreen {
        SPLASH = 0,
        STORIES = 1,
        LESSONS = 2,
        AI_SPEAKING = 3, 
        AUDIOBOOKS = 4,
        IN_STORIES = 5,
        MATERIAL = 6,
    };
    
    IntroScreen _current_screen;
    
    void onNextScreen();
    void onPrevScreen();
    
    void onShowDevice();
    void onHideDevice();
    
    void showSample();
    void hideSample();
    
    void changeText(bool from_left);
    
    void showHand();
};

CREATE_READER(MSIntro);

#endif /* MSIntro_hpp */
