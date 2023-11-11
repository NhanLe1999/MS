//
//  MSTabLessonSelect.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 3/25/19.
//

#include "MSTabLessonSelect.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "MMFirebaseDatabase.h"
#include "MSTabLessons.h"
#include "TILLabelAnimated.h"
USING_NS_CC;
INITIALIZE_READER(MSTabLessonSelect);
std::once_flag ms_tab_lesson_select_reader;

MSTabLessonSelect::~MSTabLessonSelect()
{
    if(MJDEFAULT->getBoolForKey(key_onboarding_part_2) && !MJDEFAULT->getBoolForKey(key_onboarding_part_3, false))
    {
        MJDEFAULT->setBoolForKey(key_onboarding_part_3, true);
    }
}
	
MSTabLessonSelect * MSTabLessonSelect::createTab()
{
    std::call_once(ms_tab_lesson_select_reader, []
    {
        REGISTER_CSB_READER(MSTabLessonSelect);
    });
    
    auto csb_name = "csb/new_ui/LessonSelect.csb";
    
    auto tab = reinterpret_cast<MSTabLessonSelect *>(CSLoader::createNode(csb_name));
    
    if (tab)
    {
        tab->didLoadFromCSB();
        return tab;
    }
    
    CC_SAFE_DELETE(tab);
    
    return nullptr;
}

void MSTabLessonSelect::didLoadFromCSB()
{
    onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    
    // add new icon on phonics
    auto button_phonics = utils::findChild(this, "btn_mp");
    
//    auto new_icon = ui::ImageView::create("mjstory/new_icon.png");
//    new_icon->setScale(Director::getInstance()->getVisibleSize().width/Director::getInstance()->getVisibleSize().height > (4.f/3.f)? 1.5f:1.0f);
//    new_icon->setPosition(Vec2(button_phonics->getContentSize()) + Vec2(-25,-25));
//    button_phonics->addChild(new_icon);
}

void MSTabLessonSelect::setContentSize(const cocos2d::Size &size)
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
}

void MSTabLessonSelect::showOnboard()
{

}

void MSTabLessonSelect::onEnter()
{
    ui::Layout::onEnter();
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        this->onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
}

void MSTabLessonSelect::onAlertOK(cocos2d::Ref *sender)
{
    MJDEFAULT->setBoolForKey(key_onboarding_part_3, true);
    
    mm::RealtimeDatabase::getInstance().updateUserSettings();
}

void MSTabLessonSelect::onAlertCancel(cocos2d::Ref *sender)
{
    MJDEFAULT->setBoolForKey(key_onboarding_part_3, true);
    
    mm::RealtimeDatabase::getInstance().updateUserSettings();
}

void MSTabLessonSelect::onChangeDisplayLanguage(int lang_id){

	utils::findChild<Sprite*>(this, "mp_title")->setVisible(true);

    if(lang_id == LANGUAGE_VN_CODE)
    {
        utils::findChild(this, "title_th")->setVisible(false);

        utils::findChild<Sprite*>(this, "title_vi")->setVisible(true);
        utils::findChild<Sprite*>(this, "ms_title_vi")->setVisible(true);
        // utils::findChild<Sprite*>(this, "mp_title_vi")->setVisible(true);
        
        utils::findChild<Sprite*>(this, "title_img")->setVisible(false);
        utils::findChild<Sprite*>(this, "ms_title")->setVisible(false);
        // utils::findChild<Sprite*>(this, "mp_title")->setVisible(false);
    }
    else  if(lang_id == LANGUAGE_THAI_CODE)
    {
        utils::findChild<Sprite*>(this, "title_img")->setVisible(false);
        utils::findChild<Sprite*>(this, "title_vi")->setVisible(false);
        
        utils::findChild(this, "title_th")->setVisible(true);

        utils::findChild<Sprite*>(this, "ms_title")->setVisible(true);
        utils::findChild<Sprite*>(this, "ms_title_vi")->setVisible(false);
    }
    else  if(lang_id == LANGUAGE_ID_CODE)
    {
        utils::findChild<Sprite*>(this, "title_img")->setVisible(false);
        utils::findChild<Sprite*>(this, "title_vi")->setVisible(false);
        
        utils::findChild(this, "title_th")->setVisible(true);
        utils::findChild<Text*>(this, "title_th")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.choose.a.course"));
        utils::findChild<Sprite*>(this, "ms_title")->setVisible(true);
        utils::findChild<Sprite*>(this, "ms_title_vi")->setVisible(false);
    }
    else
    {
        utils::findChild(this, "title_th")->setVisible(false);

        utils::findChild<Sprite*>(this, "title_img")->setVisible(true);
        utils::findChild<Sprite*>(this, "ms_title")->setVisible(true);
        //utils::findChild<Sprite*>(this, "mp_title")->setVisible(true);
        
        utils::findChild<Sprite*>(this, "title_vi")->setVisible(false);
        utils::findChild<Sprite*>(this, "ms_title_vi")->setVisible(false);
        // utils::findChild<Sprite*>(this, "mp_title_vi")->setVisible(false);
    }
    
    // hieunt: ẩn mấy text ở button chọn Phonics và Đọc-hiểu đi
    if (!CONFIG_MANAGER->isVietnameseStories()) {
        utils::findChild<Sprite*>(this, "ms_title")->setVisible(false);
        utils::findChild<Sprite*>(this, "ms_title_vi")->setVisible(false);
        utils::findChild<Sprite*>(this, "mp_title")->setVisible(false);
        utils::findChild<Sprite*>(this, "mp_title_vi")->setVisible(false);
    }
}

void MSTabLessonSelect::onMSSelect(cocos2d::Ref * sender)
{
    CCLOG("ms select");
     _delegate ? _delegate->onSelectedLesson(ms::LessonTab::STORIES_LESSON) : (void) nullptr;
}

void MSTabLessonSelect::onMPSelect(cocos2d::Ref * sender)
{
    CCLOG("mp select");
    _delegate ? _delegate->onSelectedLesson(ms::LessonTab::PHONICS_LESSON) : (void) nullptr;
}

ui::Widget::ccWidgetClickCallback MSTabLessonSelect::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(MSTabLessonSelect, onMSSelect),
        CLICK_MAP(MSTabLessonSelect, onMPSelect),
    };
    
    if (function_map.count(callBackName) > 0)
    {
        return function_map.at(callBackName);
    }
    
    return nullptr;
}
