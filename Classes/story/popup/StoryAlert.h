//
//  StoryAlert.h
//  TalkingPet
//
//  Created by To Ngoc Linh on 3/22/15.
//
//

#ifndef __StoryAlert__
#define __StoryAlert__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryAlertDelegate {
public:
    virtual void onAlertOK(cocos2d::Ref * sender) {};
    virtual void onAlertClose(cocos2d::Ref * sender) {};
    virtual void onAlertCancel(cocos2d::Ref * sender) {};
};

class StoryAlert : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryAlert);

    static StoryAlert * showAlert(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt = false, int fontSize = 28*(CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f), StoryAlertDelegate * delegate = nullptr, int tag = 0,std::string alertName = "", std::string rickText = "", int fontSizeButton = 0);
    static StoryAlert * showAlert2Button(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt = false, int fontSize = 28*(CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f), StoryAlertDelegate * delegate = nullptr, int tag = 0, std::string alertCSBPath = "", std::string rickText = "");
    void setContent(std::string message, std::string label_ok, std::string label_cancel, bool isOneBt = false, int fontSize = 28*(CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f), bool isTowButton = false, std::string rickText = "");
    void setDelegate(StoryAlertDelegate * delegate);
    void show(Scene *scene = nullptr, int z_order = INT_MAX - 1);
    void setStoryAlertOneButton();
    void setNetworkDownContent(bool isPortrait = false, bool isOneBt = false, bool isCloseBt = true);
    void setAlertConfirmRate(std::string message, std::string label_ok, std::string label_cancel, std::string label_guide);
    void setUpdatePopupContent(bool is_required);
    bool isRateConfirm = false;
    
    inline void setOkButtonCallback(std::function<void()>i_okButtonCallback) { m_okButtonCallback = i_okButtonCallback; }
    inline void setCacelButtonCallback(std::function<void()>i_cacelButtonCallback) { m_cancelButtonCallback = i_cacelButtonCallback; }
    inline void setCloseButtonCallback(std::function<void()>i_closeButtonCallback) { m_CloseButtonCallback = i_closeButtonCallback; }
	void onOK(cocos2d::Ref * sender);
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void didLoadFromCSB();
    void onEnter();
    StoryAlertDelegate * delegate = nullptr;
    void onCancel(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    void onTapOutside(cocos2d::Ref * sender);
    void onGuideRate(cocos2d::Ref * sender);
    bool isRunning = true;
    bool isNetworkDownAlert = false;
    Label *label_message;
    Text *label_yes, *label_no;

    float _fontSize = 0;
    bool _isTowButton = false;

    std::string replaceString(std::string subject, const std::string search,
                              const std::string replace);
    
    void chanegFontsBasedOnLanguage();
    
	std::function<void()> m_okButtonCallback = nullptr;
	std::function<void()> m_cancelButtonCallback = nullptr;
	std::function<void()> m_CloseButtonCallback = nullptr;

    CC_SYNTHESIZE(std::string, _timeline, TimelineName);
};

CREATE_READER_CLASS(StoryAlertReader, StoryAlert);

#endif /* defined(__Spring_Ninja__StoryAlert__) */
