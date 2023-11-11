//
//  MSIntroLanguage.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#ifndef MSIntroLanguage_hpp
#define MSIntroLanguage_hpp

#include "cocostudiomacro2.h"

class MSIntroLanguage : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSIntroLanguage);
    static MSIntroLanguage * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onSelect(cocos2d::Ref * sender);
    void onNext(cocos2d::Ref * sender);
    
    std::vector<cocos2d::ui::Layout *> _panels;
};

CREATE_READER(MSIntroLanguage);

#endif /* MSIntroLanguage_hpp */
