//
//  MSMaterialTopBar.h
//  MJStory
//
//  Created by Hieu Nguyen on 11/18/20.
//

#ifndef MSMaterialTopBar_h
#define MSMaterialTopBar_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "APPopupBase.h"

class MSMaterialTopBarDelegate
{
public:
    virtual void onMaterialTopBarWorksheet(cocos2d::Ref * sender) {};
    virtual void onMaterialTopBarSearch(cocos2d::Ref * sender) {};
    virtual void onMaterialTopBarLibrary(cocos2d::Ref * sender) {};
    virtual void onMaterialTopBarScanQR(cocos2d::Ref * sender) {};
};

class MSMaterialTopBar : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    enum class ActiveTab
    {
        WORKSHEET,
        SEARCH,
        LIBRARY,
        SCAN_QR,
    };
    
    CREATE_FUNC(MSMaterialTopBar);
    static MSMaterialTopBar * createBar(MSMaterialTopBarDelegate *delegate);
    
    void setContentSize(const cocos2d::Size &size) override;
    
    void setActiveTab(MSMaterialTopBar::ActiveTab tab);
    
    MSMaterialTopBar::ActiveTab getActiveTab() {return m_activeTab;};
    
    void changeDisplayLanguage() override;
    
private:
    MSMaterialTopBarDelegate *      m_delegate;
    MSMaterialTopBar::ActiveTab     m_activeTab;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    
    bool didLoadFromCSB(MSMaterialTopBarDelegate *delegate);
    
    void onWorksheet(cocos2d::Ref * sender);
    void onSearch(cocos2d::Ref * sender);
    void onLibrary(cocos2d::Ref * sender);
    void onScanQR(cocos2d::Ref * sender);
};

CREATE_READER(MSMaterialTopBar);

#endif /* MSMaterialTopBar_h */
