//
// Created by To Ngoc Linh on 8/23/17.
//

#ifndef MJSTORY_MJCALCULATOR_H
#define MJSTORY_MJCALCULATOR_H

#include "cocostudiomacro2.h"

class MJCalculator : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJCalculator);
    static MJCalculator * createCalculator();
    static void showCalculator(cocos2d::Node * blocked_node, std::function<void()> callback = nullptr, std::function<void()> close_callback = nullptr);

private:
    void didLoadFromCSB();
    void generateFormula();
    void onNumPad(cocos2d::Ref * sender);
    void onClear(cocos2d::Ref * sender);
    void onOK(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);

    cocos2d::ui::Text * _formula;
    int _input_counter;
    std::string _saved_answer;
    
    std::function<void()> _callback;
    std::function<void()> _close_callback;
public:
    ccWidgetClickCallback onLocateClickCallback(const std::string &callBackName) override;
};

CREATE_READER(MJCalculator);

#endif //MJSTORY_MJCALCULATOR_H
