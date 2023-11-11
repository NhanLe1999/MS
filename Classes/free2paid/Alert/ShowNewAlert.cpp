#include "StoryLanguageManager.h"
#include "ShowNewAlert.h"
#include "MJHelper.h"
#include "MKDefinitions.h"
USING_NS_CC;
INITIALIZE_READER(ShowNewAlert);

std::once_flag ms_show_new_alert;


ShowNewAlert * ShowNewAlert::createAlert(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText, cocos2d::Node* parent, bool isOneButton, bool isBigbg)
{
    mj::helper::deleteWaitView();
    auto alert = showAlert(title, textMessage, textOk, textCancel, isRickText, isOneButton, isBigbg);

    if (!parent)
    {
        parent = Director::getInstance()->getRunningScene();
    }

    alert->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    alert->setName("newAlert_MS");
    alert->setPosition(parent->getContentSize() / 2);

    parent->addChild(alert, INT_MAX - 1);

    return alert;
}


ShowNewAlert* ShowNewAlert::showAlert(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText, bool isOneButton, bool isBigbg) {
    std::call_once(ms_show_new_alert, [] {
        REGISTER_CSB_READER(ShowNewAlert);

        });
    auto csb_file = std::string(isBigbg ? "free2paid/alert/newAlertBig.csb" : "free2paid/alert/newAlert.csb");
    auto node = static_cast<ShowNewAlert*>(CSLoader::createNodeWithVisibleSize(csb_file));
    if (node) {
        node->didLoadFromCSB(title, textMessage, textOk, textCancel, isRickText, isOneButton);
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}


void ShowNewAlert::didLoadFromCSB(std::string title, std::string textMessage, std::string textOk, std::string textCancel, bool isRickText, bool isOneButton)
{
    mj::helper::autoScaleRootLayout(1.3, this, "popup_bg");
    _textMessage = utils::findChild<cocos2d::ui::Text*>(this, "lb_message");
    
    MK_SAFELY_SET_TEXT(this, "lb_message", textMessage);

    MK_SAFELY_SET_TEXT(this, "lb_title", title);

    MK_SAFELY_SET_TEXT(this, "lb_ok", textOk);
    
    MK_SAFELY_SET_TEXT(this, "lb_ok_one", textOk);

    MK_SAFELY_SET_TEXT(this, "lb_cance", textCancel);
    
    //truyền true nếu muốn chỉ hiện thị button Ok
    if (isOneButton)
    {
        if (auto buttonCancel = cocos2d::utils::findChild<ui::Button*>(this, "button_cancel"))
        {
            buttonCancel->setVisible(false);
        }
        
        if (auto buttonOk = cocos2d::utils::findChild<ui::Button*>(this, "button_ok"))
        {
            buttonOk->setVisible(false);
        }
        
        if (auto buttonOk = cocos2d::utils::findChild<ui::Button*>(this, "button_ok_one"))
        {
            if (auto lableOkOne = cocos2d::utils::findChild<ui::Text*>(this, "lb_ok_one")) {
                resizeButtonByText(buttonOk, lableOkOne);
            }
            buttonOk->setVisible(true);
        }
    }
    

    if (auto text = utils::findChild<Text*>(this, "lb_ok"))
    {
        _fontSizeButton = text->getFontSize();
    }

    resizeText("button_ok", "lb_ok");
    resizeText("button_cancel", "lb_cance");

    if (auto text = utils::findChild<Text*>(this, "lb_ok"))
    {
        text->setFontSize(_fontSizeButton);
    }
    if (auto text = utils::findChild<Text*>(this, "lb_cance"))
    {
        text->setFontSize(_fontSizeButton);
    }


    if (isRickText && _textMessage)
    {
        auto alert_layout = utils::findChild(this, "popup_bg");
        if (alert_layout)
        {
            _textMessage->setVisible(false);
            auto notiText = cocos2d::ui::RichText::createWithXML(textMessage);
            notiText->ignoreContentAdaptWithSize(false);
            notiText->setContentSize(_textMessage->getContentSize());
            notiText->setAnchorPoint(_textMessage->getAnchorPoint());
            notiText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
            notiText->setPosition(_textMessage->getPosition());
            alert_layout->addChild(notiText);
        }
    }
}


void ShowNewAlert::onEnter() {

    ui::Layout::onEnter();

}

ShowNewAlert::~ShowNewAlert()
{
   
}

void ShowNewAlert::onClose(cocos2d::Ref* sender)
{
    if (_closeCallback)
    {
        _closeCallback();
    }

    this->removeFromParent();
}

void ShowNewAlert::onCancel(cocos2d::Ref* sender)
{
    if (_cancelCallback)
    {
        _cancelCallback();
    }
    this->removeFromParent();
}

void ShowNewAlert::resizeText(std::string parent, std::string lableName)
{
    if (auto parentNode = utils::findChild<Button*>(this, parent))
    {
        if (auto lableText = utils::findChild<Text*>(this, lableName))
        {
            cocos2d::ui::Text* textClone = cocos2d::ui::Text::create(lableText->getString(), lableText->getFontName(), lableText->getFontSize());
            
            while ((parentNode->getContentSize().width - textClone->getContentSize().width) < 20)
            {
                textClone->setFontSize(textClone->getFontSize() - 1);
            }

            if (textClone->getFontSize() < _fontSizeButton)
            {
                _fontSizeButton = textClone->getFontSize();
            }
        }
    }
}

void ShowNewAlert::onOk(cocos2d::Ref* sender)
{
    if (_okCallback)
    {
        _okCallback();
    }
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback ShowNewAlert::onLocateClickCallback(const std::string& callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
            CLICK_MAP(ShowNewAlert, onClose),
            CLICK_MAP(ShowNewAlert, onOk),
            CLICK_MAP(ShowNewAlert, onCancel),
    };

    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    return nullptr;
}

void ShowNewAlert::onExit()
{
    ui::Layout::onExit();
}

void ShowNewAlert::setFontSizeTextMessage(float fontSize)
{
    if(_textMessage)
    {
        _textMessage->setFontSize(fontSize);
    }
}

void ShowNewAlert::setSizeTextMessage(cocos2d::Size size)
{
    if(auto message =  utils::findChild<cocos2d::ui::Text*>(this, "lb_message"))
    {
        message->setContentSize(size);
    }
}

void ShowNewAlert::resizeButtonByText(cocos2d::ui::Button* button, cocos2d::ui::Text* text)
{
    if (text->getContentSize().width >= button->getContentSize().width - 100)
    {
        button->setContentSize(Size(button->getContentSize().width + 100, button->getContentSize().height));
        text->setPosition(Vec2(button->getContentSize().width * 0.5, button->getContentSize().height * 0.61));
    }
}

void ShowNewAlert::setFontSizeTextTitle(float fontSize)
{
    if(auto titleText = utils::findChild<ui::Text*>(this, "lb_title"))
    {
        titleText->setFontSize(fontSize);
    }
}

void ShowNewAlert::setSizeTextTitle(cocos2d::Size size)
{
    if(auto title =  utils::findChild<cocos2d::ui::Text*>(this, "lb_title"))
    {
        title->setContentSize(size);
    }
}

// hàm này dùng để dịch text message lên hoặc xuống 1 đoạn x
// nếu là rickText thì truyền text message = "" vào createAlert 
void ShowNewAlert::MoveTextMessage(std::string textMessage, bool isRickText, float X)
{
    _textMessage = utils::findChild<cocos2d::ui::Text*>(this, "lb_message");

    if (_textMessage)
    {
        if (isRickText)
        {
            auto alert_layout = utils::findChild(this, "popup_bg");
            if (alert_layout)
            {
                auto notiText = cocos2d::ui::RichText::createWithXML(textMessage);
                notiText->ignoreContentAdaptWithSize(false);
                notiText->setContentSize(_textMessage->getContentSize());
                notiText->setAnchorPoint(_textMessage->getAnchorPoint());
                notiText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
                notiText->setPosition(_textMessage->getPosition() + Vec2(0, X));
                alert_layout->addChild(notiText);
            }
        }
        else
        {
            _textMessage->setPosition(_textMessage->getPosition() + Vec2(0, X));
        }
    }
}
