//
//  MSTabMaterial.h
//  MJStory
//
//  Created by Hieu Nguyen on 11/16/20.
//

#ifndef MSTabMaterial_h
#define MSTabMaterial_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "APPopupBase.h"
#include "MSMaterialTopBar.h"
#include "MSMaterialEnterEmail.h"
#include "StoryDataModel.h"
#include "MSTabFavourite.h"

#define msmat_name_tab_material             "name_tab_material"

#define msmat_name_page_worksheet           "msmat_name_page_worksheet"

#define MSMAT_FONT_FILE_PATH                (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf")

#define MSMAT_FONT_FILE_PATH_BOLD           (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Bold.ttf")

#define MSMAT_GET_TEXT(__KEY__)             LANGUAGE_MANAGER->getDisplayTextByKey(__KEY__)

#define MSMAT_MATERIAL_TYPE_STR(__TYPE__)   __TYPE__ == mj::model::MaterialInfo::Type::STORY ? "story" : "phonics"

#define MSMAT_MAX_NUMBER_OF_MATERIALS       MSTabMaterial::getMaxNumberOfSelectedMaterials() // số tài liệu tối đa người dùng được chọn

#define MSMAT_MAX_RECENT_SEARCHES           5 // số từ khoá tìm kiếm tối đa được lưu

#define MSMAT_SHOW_BUTTON_QR                0 // hiển thị button Quét QR ở bên dưới

#define MSMAT_DEBUG_ALWAYS_BTN_EMAIL        0 // DEBUG: luôn luôn hiển thị button Email ở tab Material

enum
{
    kMSTabMaterialZOrderContent = 2000,
    kMSTabMaterialZOrderTopBar  = 2001,
    kMSTabMaterialZOrderFullContent = 2002,
};

class MSHome;

class MSTabMaterial : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public MSMaterialTopBarDelegate, public MSMaterialEnterEmailDelegate
{
public:
    CREATE_FUNC(MSTabMaterial);
    
    static MSTabMaterial * createTab();
    
    static int getMaxNumberOfSelectedMaterials();
    
    void setContentSize(const cocos2d::Size &size) override;
    
    void setMSHome(MSHome *home) {m_home = home;};
    
    // nhảy vào WS stories/phonics kèm tích item luôn
    void onSelectStoriesWithItems(const std::string selected_ids, int grade_id = 0, cocos2d::Ref * sender = nullptr);
    void onSelectPhonicsWithItems(const std::string selected_ids, int grade_id = 0, cocos2d::Ref * sender = nullptr);
    
    // MSMaterialEnterEmailDelegate
    void onMaterialEmailUpdated() override;
    mj::model::MaterialInfo::Type fromMaterialScreen() override {return mj::model::MaterialInfo::Type::NONE;};
    void setMSTabFavourite(MSTabFavourite* mSTabFavourite) {
        if (mSTabFavourite)
            m_tabFavourite = mSTabFavourite;
    };
    MSTabFavourite* getMSTabFavourite() {
        return m_tabFavourite;
    };
private:
    MSHome *            m_home = nullptr;       // assign var
    MSMaterialTopBar *  m_topBar = nullptr;     // top bar
    cocos2d::Node *     m_content = nullptr;    // content
    MSTabFavourite* m_tabFavourite = nullptr;
    void changeDisplayLanguage() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    bool didLoadFromCSB() override;
    
    void pushCleverTapEvent_screenChooseType(const std::string& str_type);
    
    // MSMaterialTopBarDelegate
    void onMaterialTopBarWorksheet(cocos2d::Ref * sender) override;
    void onMaterialTopBarSearch(cocos2d::Ref * sender) override;
    void onMaterialTopBarLibrary(cocos2d::Ref * sender) override;
    void onMaterialTopBarScanQR(cocos2d::Ref * sender) override;
    
    // buttons
    void onSelectStories(cocos2d::Ref * sender);
    void onSelectPhonics(cocos2d::Ref * sender);
    void onQR(cocos2d::Ref * sender);
    void onEmail(cocos2d::Ref * sender);
};

CREATE_READER(MSTabMaterial);

#endif /* MSTabMaterial_h */
