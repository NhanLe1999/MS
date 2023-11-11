//
//  MSActivityPreview.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/14/17.
//

#ifndef MSActivityPreview_hpp
#define MSActivityPreview_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MSActivityPreview : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSActivityPreview);
    static MSActivityPreview * createView(mj::model::ActInfo info, bool is_first = false, bool is_last = false);
    
private:
    void didLoadFromCSB(mj::model::ActInfo info, bool is_first, bool is_last);
    
    mj::model::ActInfo _info;
    void changeTextDisplayLang();
    void onEnter() override;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void onSelectActivity(cocos2d::Ref * sender);
    
//    std::string getGameName(int game_id);
};

CREATE_READER(MSActivityPreview);

#endif /* MSActivityPreview_hpp */
