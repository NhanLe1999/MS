//
//  MJPayGuide.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 3/29/17.
//
//

#ifndef MJPayGuide_hpp
#define MJPayGuide_hpp

#include <stdio.h>
#include "CocoStudioMacro.h"
#include "HSBaseLayout.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class MJPayGuide : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(MJPayGuide);
    static MJPayGuide * createPage();
    void onShowHidePopUp(bool isShow);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;

    void initPage();
    void onClose(cocos2d::Ref*sender);
};

CREATE_READER_CLASS(MJPayGuideReader, MJPayGuide);

#endif /* MJPayGuide_hpp */
