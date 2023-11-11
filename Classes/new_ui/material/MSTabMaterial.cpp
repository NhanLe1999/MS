//
//  MSTabMaterial.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 11/16/20.
//

#include "MSTabMaterial.h"
#include "MSHome.h"
#include "MSTabFavourite.h"
#include "MJCalculator.h"
#include "APDatabase.h"
#include "MJWorksheet.h"
#include "MSSidebar.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(MSTabMaterial);

std::once_flag mat_tab_material_reader_flag;

#pragma mark - Initialize
int MSTabMaterial::getMaxNumberOfSelectedMaterials()
{
    if (CONFIG_MANAGER->isAppActive() && !MJDEFAULT->getBoolForKey("check_is_free", true)) {
        return MJDEFAULT->getIntegerForKey(key_material_max_items_sent, 1);
    }
    
    return MJDEFAULT->getIntegerForKey(key_material_max_items_sent_free, 1);
}

MSTabMaterial * MSTabMaterial::createTab()
{
    std::call_once(mat_tab_material_reader_flag, []
    {
        REGISTER_CSB_READER(MSTabMaterial);
    });
    
    auto csb_name = "csb/new_ui/material/MaterialMain.csb";
    auto p = static_cast<MSTabMaterial *>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (p && p->didLoadFromCSB())
    {
        return p;
    }
    
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void MSTabMaterial::setContentSize(const cocos2d::Size &size)
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        auto btn_stories = cocos2d::utils::findChild(this, "btn_stories");
        btn_qr->setPosition(Point(btn_stories->getPosition().x - 144, btn_qr->getPosition().y));
    }
    
    if (auto btn_email = cocos2d::utils::findChild(this, "btn_email"))
    {
        auto btn_phonics = cocos2d::utils::findChild(this, "btn_phonics");
        btn_email->setPosition(Point(btn_phonics->getPosition().x - 144, btn_email->getPosition().y));
    }
}

bool MSTabMaterial::didLoadFromCSB()
{
    // content
    m_content = Node::create();
    m_content->setPosition(Point::ZERO);
    this->addChild(m_content, kMSTabMaterialZOrderContent);
    
    this->setName(msmat_name_tab_material);
    
    // top bar
    m_topBar = MSMaterialTopBar::createBar(this);
    m_topBar->setActiveTab(MSMaterialTopBar::ActiveTab::WORKSHEET);
    m_topBar->setPosition(Point(0, Director::getInstance()->getVisibleSize().height - 68));
    this->addChild(m_topBar, kMSTabMaterialZOrderTopBar);
    
    // check email exists
    this->onMaterialEmailUpdated();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || !MSMAT_SHOW_BUTTON_QR
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        btn_qr->setVisible(false);
    }
#endif
    
    // language
    this->changeDisplayLanguage();
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeDisplayLanguage();
    }), this);
    
    return true;
}

void MSTabMaterial::changeDisplayLanguage()
{
    // title, desc
    if (auto title = cocos2d::utils::findChild<ui::Text *>(this, "title")) {
        title->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        title->setString(MSMAT_GET_TEXT("key.material.main.title1"));
    }
    
    if (auto desc = cocos2d::utils::findChild<ui::Text *>(this, "desc")) {
        desc->setFontName(MSMAT_FONT_FILE_PATH);
        desc->setString(MSMAT_GET_TEXT("key.material.main.title2"));
    }
    
    // button: QR
    if (auto btn_qr = cocos2d::utils::findChild(this, "btn_qr"))
    {
        const char * name_of_button = "tab_mat_button_qr";
        btn_qr->removeChildByName(name_of_button);
        
        /*
         -- tiếng Việt --
         <font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>Quét mã QR</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> trên học liệu của bạn để mở truyện hoặc bài học tương ứng trên APP</font>
         
         -- tiếng Anh --
         <font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>Scan the QR code</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> in your worksheet to access the corresponding stories or lessons on app</font>
         
         -- tiếng Thái --
         <font color='#27AFE5' size='19' face='fonts/leelawdb.ttf'>สแกนรหัส QR</font><font color='#000000' size='19' face='fonts/leelawdb.ttf'> บนเอกสารของคุณเพื่อเปิดเรื่องราวหรือบทเรียนที่เกี่ยวข้องบนแอพ</font>
         
         -- tiếng Indo --
         <font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>Pindai kode QR</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> pada materi Anda yang ingin dibuka cerita atau pelajaran terkait di aplikasi</font>
         */
        
        if (auto richText = ui::RichText::createWithXML(MSMAT_GET_TEXT("key.material.button.scan.qr")))
        {
            auto text_1 = (ui::Text *)btn_qr->getChildByName("text_1");
            
            richText->setName(name_of_button);
            richText->setHorizontalAlignment(RichText::HorizontalAlignment::LEFT);
            richText->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            richText->setPosition(Point(text_1->getPosition().x + 8, btn_qr->getContentSize().height/2));
            richText->setTouchEnabled(true);
            richText->addClickEventListener([=](cocos2d::Ref * ref){
                this->onQR(ref);
            });
            btn_qr->addChild(richText);
        }
    }
    
    // button: Email
    if (auto btn_email = cocos2d::utils::findChild(this, "btn_email"))
    {
        const char * name_of_button = "tab_mat_button_qr";
        btn_email->removeChildByName(name_of_button);
        
        /*
         -- tiếng Việt --
         <font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>Điền địa chỉ email</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> để nhận học liệu</font>
         
         -- tiếng Anh --
         <font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'>Please </font><font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>enter your email</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> to receive the worksheets</font>
         
         -- tiếng Thái --
         <font color='#000000' size='19' face='fonts/leelawdb.ttf'>ป้อน</font><font color='#27AFE5' size='19' face='fonts/leelawdb.ttf'>ที่อยู่อีเมล</font><font color='#000000' size='19' face='fonts/leelawdb.ttf'>ของคุณเพื่อรับเอกสาร</font>
         
         -- tiếng Indo --
         <font color='#27AFE5' size='18' face='fonts/Montserrat-Medium.ttf'>Masukkan email</font><font color='#000000' size='18' face='fonts/Montserrat-Medium.ttf'> Anda untuk menerima materi</font>
         */
        if (auto richText = ui::RichText::createWithXML(MSMAT_GET_TEXT("key.material.button.enter.email")))
        {
            auto text_1 = (ui::Text *)btn_email->getChildByName("text_1");
            
            richText->setName(name_of_button);
            richText->setHorizontalAlignment(RichText::HorizontalAlignment::LEFT);
            richText->ignoreContentAdaptWithSize(false);
            richText->setContentSize(Size(280, 76));
            richText->setAnchorPoint(text_1->getAnchorPoint());
            richText->setPosition(text_1->getPosition() + Point(8, 13));
            richText->setTouchEnabled(true);
            richText->addClickEventListener([=](cocos2d::Ref * ref){
                this->onEmail(ref);
            });
            btn_email->addChild(richText);
        }
    }
    
    // top bar
    m_topBar->changeDisplayLanguage();
}

ui::Widget::ccWidgetClickCallback MSTabMaterial::onLocateClickCallback(const std::string & callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSTabMaterial, onSelectStories),
        CLICK_MAP(MSTabMaterial, onSelectPhonics),
        CLICK_MAP(MSTabMaterial, onQR),
        CLICK_MAP(MSTabMaterial, onEmail),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSTabMaterial::onMaterialEmailUpdated()
{
#if MSMAT_DEBUG_ALWAYS_BTN_EMAIL
    return;
#endif
    
    if (auto btn_email = cocos2d::utils::findChild(this, "btn_email"))
    {
#if !MSMAT_DEBUG_ALWAYS_BTN_EMAIL
        auto user_info = ap::Database::getInstance().getUserInfo();
        auto email_material = MJDEFAULT->getStringForKey(StringUtils::format(key_email_material_x, user_info._id), "");
        btn_email->setVisible( (email_material.length() || user_info._mail.length()) ? false : true);
#endif
    }
}

void MSTabMaterial::pushCleverTapEvent_screenChooseType(const std::string& str_type)
{
    try
    {
        CleverTapManager::GetInstance()->pushEvent("worksheet_screen_choose_type",
        {
            {"type", cocos2d::Value(str_type)},
        });
    }
    catch (const std::exception&)
    {
        // nothing here
    }
}

#pragma mark - Handle with Buttons
void MSTabMaterial::onSelectStoriesWithItems(const std::string selected_ids, int grade_id, cocos2d::Ref * sender)
{
    this->onSelectStories(sender);
    
    if (auto page_worksheet = utils::findChild<MJWorksheet *>(Director::getInstance()->getRunningScene(), msmat_name_page_worksheet)) {
        page_worksheet->tickWorksheetItems(selected_ids, grade_id);
    }
}

void MSTabMaterial::onSelectPhonicsWithItems(const std::string selected_ids, int grade_id, cocos2d::Ref * sender)
{
    this->onSelectPhonics(sender);
    
    if (auto page_worksheet = utils::findChild<MJWorksheet *>(Director::getInstance()->getRunningScene(), msmat_name_page_worksheet)) {
        page_worksheet->tickWorksheetItems(selected_ids, grade_id);
    }
}

void MSTabMaterial::onSelectStories(cocos2d::Ref * sender)
{
    if (m_home)
    {
        m_home->onSelectedMaterial(mj::model::MaterialInfo::Type::STORY);
        this->pushCleverTapEvent_screenChooseType("story");
    }
}

void MSTabMaterial::onSelectPhonics(cocos2d::Ref * sender)
{
    if (m_home)
    {
        m_home->onSelectedMaterial(mj::model::MaterialInfo::Type::PHONICS);
        this->pushCleverTapEvent_screenChooseType("phonics");
    }
}

void MSTabMaterial::onQR(cocos2d::Ref * sender)
{
    if (auto sidebar = cocos2d::utils::findChild<MSSidebar *>(Director::getInstance()->getRunningScene(), "msSideBar"))
    {
        sidebar->onQRForce();
        this->pushCleverTapEvent_screenChooseType("qr_code");
    }
}

void MSTabMaterial::onEmail(cocos2d::Ref * sender)
{
    MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {
        auto popup = MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type::ENTER_EMAIL, this);
        popup->showPopup();
        Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX-1);
        this->pushCleverTapEvent_screenChooseType("setting_email");
    });
}

#pragma mark - MSMaterialTopBarDelegate
void MSTabMaterial::onMaterialTopBarWorksheet(cocos2d::Ref * sender)
{
    mj::helper::playButtonFX();
    mj::helper::SetFpsByNumberFrames(20);
    if (m_topBar->getActiveTab() != MSMaterialTopBar::ActiveTab::WORKSHEET)
    {
        m_topBar->setActiveTab(MSMaterialTopBar::ActiveTab::WORKSHEET);
        if (m_tabFavourite)
        {
            m_tabFavourite->setVisible(false);
            m_tabFavourite->setOpacity(0);
        }
        //m_content->removeAllChildren();
        if (auto layer = cocos2d::utils::findChild(this, "Panel_1_0"))
        {
            layer->setOpacity(0);
            layer->runAction(FadeIn::create(0.25f));
        }
    }
}

void MSTabMaterial::onMaterialTopBarSearch(cocos2d::Ref * sender)
{
    if (m_topBar->getActiveTab() != MSMaterialTopBar::ActiveTab::SEARCH)
    {
        m_topBar->setActiveTab(MSMaterialTopBar::ActiveTab::SEARCH);
        m_content->removeAllChildren();
    }
}

void MSTabMaterial::onMaterialTopBarLibrary(cocos2d::Ref * sender)
{
    mj::helper::SetFpsByNumberFrames(10);
    if (m_topBar->getActiveTab() != MSMaterialTopBar::ActiveTab::LIBRARY)
    {
        m_topBar->setActiveTab(MSMaterialTopBar::ActiveTab::LIBRARY);
        
        if (!m_tabFavourite)
        {
            //m_content->removeAllChildren();
            m_tabFavourite = MSTabFavourite::createTab();
            m_tabFavourite->setContentSize(this->getContentSize());

            if (auto fav_topBar = cocos2d::utils::findChild(m_tabFavourite, "Panel_2"))
            {
                fav_topBar->setVisible(false);
            }

            if (auto fav_favourite = cocos2d::utils::findChild(m_tabFavourite, "content_favourite"))
            {
                fav_favourite->setVisible(false);
            }
            m_content->addChild(m_tabFavourite);

        }
        m_tabFavourite->setVisible(true);
        m_tabFavourite->setOpacity(255);

    }
}

void MSTabMaterial::onMaterialTopBarScanQR(cocos2d::Ref * sender)
{
    if (m_topBar->getActiveTab() != MSMaterialTopBar::ActiveTab::SCAN_QR)
    {
        this->onQR(sender);
    }
}
