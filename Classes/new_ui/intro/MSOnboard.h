//
//  MSOnboard.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/1/18.
//

#ifndef MSOnboard_hpp
#define MSOnboard_hpp

#include "cocostudiomacro2.h"

struct RichTextInfo {
    std::string text;
    bool is_bold;
    int size;
};

struct OnboardInfo {
    std::vector<RichTextInfo> texts;
    std::string title;
    std::string plain_text;
    cocos2d::Point anchor;
    cocos2d::Point position;
    cocos2d::Rect highlight;
};

namespace ms {
    class MSOnboardDelegate;
};

class MSOnboard : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSOnboard);
    static MSOnboard * createSingleBoard(OnboardInfo info);
    static MSOnboard * createBoard(int step);
    void setHighlightRect(cocos2d::Rect r);
    
private:
    void didLoadFromCSB(int step);
    void didLoadFromCSB(OnboardInfo info);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void onNext(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    
    int _step;
    
    std::vector<OnboardInfo> _onboard_info;
    CC_SYNTHESIZE(ms::MSOnboardDelegate*, _delegate, Delegate);
};

CREATE_READER(MSOnboard);

#endif /* MSOnboard_hpp */
