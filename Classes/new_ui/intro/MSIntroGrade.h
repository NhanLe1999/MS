//
//  MSIntroGrade.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#ifndef MSIntroGrade_hpp
#define MSIntroGrade_hpp

#include "cocostudiomacro2.h"

class MSIntroGrade : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSIntroGrade);
    static cocos2d::Scene * createScene();
    static MSIntroGrade * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    std::vector<cocos2d::ui::Button *> _buttons;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onSelect(cocos2d::Ref * sender);
    void onNext(cocos2d::Ref * sender);
    
    void checkButton(cocos2d::ui::Button * bt);
    void uncheckButton(cocos2d::ui::Button * bt);
    
    void showNextButton();
};

CREATE_READER(MSIntroGrade);

#endif /* MSIntroGrade_hpp */
