//
//  Math_Answer.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#ifndef Math_Answer_h
#define Math_Answer_h

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"
#include "Math_Libs.h"
#include "Math_Func.h"

class Math_Answer : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    
    CREATE_FUNC(Math_Answer);
    
    static Math_Answer * createAnswer();
    void setEnable(std::string title, math::common::AnswerType type, math::callback::onAnswerClicked callback, cocos2d::Size size = cocos2d::Size(200, 100));
    void setDisable(float delay = 0.5, math::animation::ObjectShowType revert_show = math::animation::SlideUp);
    bool isEnable();
    void setCustomAnswer(math::resource::Image* img, cocos2d::Size size, float font_size, cocos2d::Color4B text_color = m_text_color, float padding = 0);
    void setAnswerSize(cocos2d::Size size);
    void fitAnswerSize();
    
    math::common::AnswerType getType();
    void setCorrect(bool correct);
    bool isCorrect();
    
    void setIcon(math::resource::Image* image);
    math::resource::Image* getIcon();

    void setAnswerVisiable(bool visiable);
    void setTitleWithIcon (std::string key);
    void setTitleWithIconProp (std::string key);

    void onAnimationClick(bool correct);

    void playAnswerAudio(std::string key = "name_1", float delay = 0);
    void onShowInParent(Node* parent, int order, float delay = 0.5, math::animation::ObjectShowType show = math::animation::SlideUp);
        
    //Debugging
    void onAutoClick();
    
    //TODO: code ver 2.1
    void disableClickAnswer();
    void enableClickAnswer();
    
    //TODO: code ver 2.2: Animation
    void setFlipBG(bool value);
    void captureState();
    void restoreCaptuedState(bool animated=true);
    
    cocos2d::ui::Text* getValueText();
    
    float getRenderSize();
    
private:
    bool init() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onClicked(Ref* pSender);
    void onShow(math::animation::ObjectShowType show, float delay);
    void onHide(math::animation::ObjectShowType revert_show, float delay);
    
	cocos2d::ui::Layout*    _panel = nullptr;
	cocos2d::ui::Text*      _text = nullptr;
	cocos2d::ui::Button*    _button = nullptr;
	cocos2d::ui::ImageView* _icon = nullptr;
	cocos2d::ui::ImageView* _bg = nullptr;
	math::resource::Image*  _icon_src = nullptr;
    
    bool _enable;
    bool _correct;
    bool _clickable;
    math::common::AnswerType        _type;
    math::callback::onAnswerClicked _click_callback;
    math::animation::CapturedNode   _capture_node;
};

CREATE_READER_CLASS(Math_AnswerReader, Math_Answer);
#endif /* Math_Answer_h */
