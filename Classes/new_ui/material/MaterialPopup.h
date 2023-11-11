#pragma once

#include "cocostudiomacro2.h"

class MaterialPopupDelegate {
public:
    virtual void onBackWarningClose1() {};
};

class MaterialPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    enum Type {
        BACK_WARNING
    };
    CREATE_FUNC(MaterialPopup);
    static MaterialPopup* createPopup(Type type, MaterialPopupDelegate* delegate,int numberMaterialSelected);
    void showpopup(Type type);

private:
    MaterialPopupDelegate* m_materialPopupDelegate = nullptr;
    Type m_type;
    static std::string getCsbFile(Type type);
    void didLoadFromCSB(Type type,MaterialPopupDelegate* PopupDelegate,int numberMaterialSelected);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    void onBackWarningClose(cocos2d::Ref* ref);
    void onBackWarningContinue(cocos2d::Ref* ref);
    int m_numberMaterialSelected=0;
    
    bool m_isRunning = false;    // đang chạy hiệu ứng show/hide popup
};

CREATE_READER(MaterialPopup);

