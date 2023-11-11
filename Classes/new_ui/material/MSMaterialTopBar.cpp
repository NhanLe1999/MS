//
//  MSMaterialTopBar.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 11/18/20.
//

#include "MSMaterialTopBar.h"
#include "MSTabMaterial.h"

USING_NS_CC;
INITIALIZE_READER(MSMaterialTopBar);

std::once_flag mat_material_topbar_reader_flag;

#define MaterialTopBar_Color_Active     Color3B::WHITE
#define MaterialTopBar_Color_Inactive   Color3B(39, 175, 229)

#pragma mark - Initialize
MSMaterialTopBar * MSMaterialTopBar::createBar(MSMaterialTopBarDelegate *delegate)
{
    std::call_once(mat_material_topbar_reader_flag, []
    {
        REGISTER_CSB_READER(MSMaterialTopBar);
    });
    
    auto csb_name = "csb/new_ui/material/MaterialTopBar.csb";
    auto p = static_cast<MSMaterialTopBar *>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (p && p->didLoadFromCSB(delegate))
    {
        return p;
    }
    
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void MSMaterialTopBar::setContentSize(const cocos2d::Size &size)
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
}

bool MSMaterialTopBar::didLoadFromCSB(MSMaterialTopBarDelegate *delegate)
{
    m_delegate = delegate;
    
    /*
     * Now, hide Search tab, only show Worksheet tab and Library tab
     */
    if (auto btn_search = cocos2d::utils::findChild(this, "btn_search"))
    {
        btn_search->setVisible(false);
    }
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        btn_qr->setVisible(false);
    }
#endif
#if CC_PLATFORM_HUAWEI
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        btn_qr->setVisible(false);
    }
#endif

    return true;
}

void MSMaterialTopBar::changeDisplayLanguage()
{
    // button: search
    if (auto btn_search = cocos2d::utils::findChild(this, "btn_search"))
    {
        /*
         * Now, hide Search tab, only show Worksheet tab and Library tab
         */
        btn_search->setVisible(false);
        
        auto text = (ui::Text *)btn_search->getChildByName("Text_1");
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("key.material.search"));
    }
    
    // button: library
    if (auto btn_library = cocos2d::utils::findChild(this, "btn_library"))
    {
        auto text = (ui::Text *)btn_library->getChildByName("Text_1");
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("key.material.library"));
    }
    
    // button: worksheet
    if (auto btn_worksheet = cocos2d::utils::findChild(this, "btn_worksheet"))
    {
        auto text = (ui::Text *)btn_worksheet->getChildByName("Text_1");
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("key.material.worksheet"));
    }
    
    // button: scan_qr
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        auto text = (ui::Text *)btn_qr->getChildByName("Text_1");
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("sidebar.des.qr"));
    }
}

void MSMaterialTopBar::setActiveTab(MSMaterialTopBar::ActiveTab tab)
{
    m_activeTab = tab;
    std::string active_button;
    std::vector<std::string> inactive_buttons;
    
    if (m_activeTab == MSMaterialTopBar::ActiveTab::WORKSHEET) {
        active_button = "btn_worksheet";
        inactive_buttons = {
            "btn_search",
            "btn_library",
            "btn_qr"
        };
    } else if (m_activeTab == MSMaterialTopBar::ActiveTab::SEARCH) {
        active_button = "btn_search";
        inactive_buttons = {
            "btn_worksheet",
            "btn_library",
            "btn_qr"
        };
    } else if (m_activeTab == MSMaterialTopBar::ActiveTab::LIBRARY) {
        active_button = "btn_library";
        inactive_buttons = {
            "btn_search",
            "btn_worksheet",
            "btn_qr"
        };
    } else if (m_activeTab == MSMaterialTopBar::ActiveTab::SCAN_QR) {
        active_button = "btn_qr";
        inactive_buttons = {
            "btn_search",
            "btn_worksheet",
            "btn_library"
        };
    }
    
    for (auto bbb : inactive_buttons)
    {
        if (auto button = cocos2d::utils::findChild(this, bbb))
        {
            ((ui::ImageView *)button->getChildByName("Image_2"))->setColor(MaterialTopBar_Color_Inactive);
            ((ui::Text *)button->getChildByName("Text_1"))->setTextColor(Color4B(MaterialTopBar_Color_Inactive));
        }
    }
    
    if (auto button = cocos2d::utils::findChild(this, active_button))
    {
        ((ui::ImageView *)button->getChildByName("Image_2"))->setColor(MaterialTopBar_Color_Active);
        ((ui::Text *)button->getChildByName("Text_1"))->setTextColor(Color4B(MaterialTopBar_Color_Active));
        
        if (auto background_active = cocos2d::utils::findChild(this, "bg_active"))
        {
            background_active->setPosition(button->getPosition() - Point(12, 2));
        }
    }
}

#pragma mark - Handle with buttons
ui::Widget::ccWidgetClickCallback MSMaterialTopBar::onLocateClickCallback(const std::string & callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSMaterialTopBar, onWorksheet),
        CLICK_MAP(MSMaterialTopBar, onSearch),
        CLICK_MAP(MSMaterialTopBar, onLibrary),
        CLICK_MAP(MSMaterialTopBar, onScanQR),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSMaterialTopBar::onWorksheet(cocos2d::Ref * sender)
{
    if (m_delegate)
    {
        m_delegate->onMaterialTopBarWorksheet(sender);
    }
}

void MSMaterialTopBar::onSearch(cocos2d::Ref * sender)
{
    if (m_delegate)
    {
        m_delegate->onMaterialTopBarSearch(sender);
    }
}

void MSMaterialTopBar::onLibrary(cocos2d::Ref * sender)
{
    if (m_delegate)
    {
        MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "Library");
        MJDEFAULT->setStringForKey(key_read_from, "Library");
        MJDEFAULT->deleteValueForKey("key_read_from_the_second");
        m_delegate->onMaterialTopBarLibrary(sender);
    }
}

void MSMaterialTopBar::onScanQR(cocos2d::Ref * sender)
{
    if (m_delegate)
    {
        m_delegate->onMaterialTopBarScanQR(sender);
    }
}
