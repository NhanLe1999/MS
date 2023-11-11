//
//  APCalculator.hpp
//  MJStory
//
//  Created by ABN on 10/8/19.
//

#ifndef APCalculator_hpp
#define APCalculator_hpp

#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;
#include "cocostudiomacro2.h"

class APCalculator : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APCalculator);
    static APCalculator * createView();
    bool show_close = false;
    bool is_onboard = true;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(int lang_id);
    void generateFormula();
    void onNumPad(cocos2d::Ref * sender);
    void onClear(cocos2d::Ref * sender);
    void onOK(cocos2d::Ref * sender);
    void onTest(cocos2d::Ref* sender);
    void showErrorAlert(std::string msg);
    cocos2d::ui::Text * _formula;
    cocos2d::ui::Text * _result;
    int _input_counter = 0;
    std::string _saved_answer;
    cocos2d::ui::Layout* root_panel;

    CC_SYNTHESIZE(std::function<void()>,m_okCallback,Callback);

};

CREATE_READER(APCalculator);

#endif /* APCalculator_hpp */
