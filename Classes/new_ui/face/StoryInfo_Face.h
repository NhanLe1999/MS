//
//  StoryInfo_Face.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/11/19.
//

#ifndef StoryInfo_Face_hpp
#define StoryInfo_Face_hpp

#include "cocostudiomacro2.h"

class StoryInfo_Face : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(StoryInfo_Face);
    static StoryInfo_Face * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    
    void addFaceMask();
    
    void onBack(cocos2d::Ref * sender);
    void onCamera(cocos2d::Ref * sender);
    void onPhoto(cocos2d::Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(StoryInfo_Face);

#endif /* StoryInfo_Face_hpp */
