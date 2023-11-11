
#ifndef ShowNewAlert_hpp
#define ShowNewAlert_hpp

#include "cocostudiomacro2.h"

class ShowNewAlert : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(ShowNewAlert);

    static ShowNewAlert* createAlert(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText = false, cocos2d::Node* parent = nullptr, bool isOneButton = false, bool isBigbg = false);

    static ShowNewAlert* showAlert(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText = false, bool isOneButton = false, bool isBigbg = false);

    void setOnOkCallback(std::function<void()> callback){_okCallback = callback;}
    void setOnCancelCallback(std::function<void()> callback) { _cancelCallback = callback; }
    void setOnCloseCallback(std::function<void()> callback) { _closeCallback = callback; }
    
    void setFontSizeTextMessage(float fontSize);
    void setSizeTextMessage(cocos2d::Size size);

    void resizeButtonByText(cocos2d::ui::Button *button, cocos2d::ui::Text* text);

    void MoveTextMessage(std::string textMessage, bool isRickText, float X);
    
    void setFontSizeTextTitle(float fontSize);
    void setSizeTextTitle(cocos2d::Size size);

    void onExit() override;

    void onOk(cocos2d::Ref* sender);

private:
    ~ShowNewAlert();
    void didLoadFromCSB(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText = false, bool isOneButton = false);
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    void onClose(cocos2d::Ref* sender);
    void onCancel(cocos2d::Ref* sender);
    void resizeText(std::string parent, std::string lableName);
    int _fontSizeButton = 0;
    cocos2d::Size _sizeText = cocos2d::Size(0, 0);

    std::function<void()> _okCallback = nullptr;
    std::function<void()> _cancelCallback = nullptr;
    std::function<void()> _closeCallback = nullptr;

    cocos2d::ui::Text* _textMessage = nullptr;

};

CREATE_READER(ShowNewAlert);

#endif /* ShowNewAlert_hpp */
