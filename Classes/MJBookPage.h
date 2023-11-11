//
//  MJBookPage.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/6/17.
//

#ifndef MJBookPage_hpp
#define MJBookPage_hpp

#include "cocostudiomacro2.h"

// a book page

class MJBookPage : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJBookPage);
    static MJBookPage * createPage(const std::string & config_file);
private:
    void onEnter() override;
    
    int box_id;
    int cursor_id;
    
    /**
     * highlight function
     */
    void highlightText(std::string text, int box, int cursor, float start_time, float duration, cocos2d::Color3B color);
    
    std::vector<std::string> texts;
};

CREATE_READER(MJBookPage);

#endif /* MJBookPage_hpp */
