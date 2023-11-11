//
//  MSTabFree.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/22/17.
//

#include "MSTabFree.h"
#include "StoryDataModel.h"
#include "MSCollectionStoryItem.h"
#include "MSCollectionAudiobookItem.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSLessonManager.h"
#include "MSCollectionLessonItem.h"
#include "MSGradeBox.h"

#include "MSActivityEnd.h"
#include "MSActivityWrapper.h"
#include "MSLessonPreview.h"
#include "StoryLanguageManager.h"

#include "MSGradeManager.h"
#include "StoryConfigManager.h"
#include "MSHome.h"

USING_NS_CC;
INITIALIZE_READER(MSTabFree);

std::once_flag ms_tab_free_reader;
MSTabFree * MSTabFree::createTab() {
    std::call_once(ms_tab_free_reader, [] {
        REGISTER_CSB_READER(MSTabFree);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Free.csb");
    auto node = reinterpret_cast<MSTabFree *>(CSLoader::createNode(csb_name));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSTabFree::didLoadFromCSB() {
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
    this->setClippingEnabled(false);
    
    auto stories = StoryDataModel::getInstance()->getFreeStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    auto audiobooks = StoryDataModel::getInstance()->getFreeAudiobooksByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview_free_stories");
    listview->setScrollBarEnabled(false);
    listview->setItemsMargin(25);
    
    std::vector<ui::Widget *> ui_items;

    mj::helper::shuffle(stories);
    mj::helper::shuffle(audiobooks);
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        
        if(stories.size() == 0){
            listview->setVisible(false);
            utils::findChild<ui::Text *>(this, "stories.update.text")->setVisible(true);
        }else{
            for(int i = 0; i < stories.size(); i++){
                auto story_item = MSCollectionStoryItem::createItem(stories[i]);
                ui_items.push_back(story_item);
            }
        }
        
    }else{
        for (int i = 0; i < (int)stories.size(); i++) {
            ui_items.push_back(MSCollectionStoryItem::createItem(stories[i]));
        }
        
        for (int i = 0; i < (int)audiobooks.size(); i++) {
            ui_items.push_back(MSCollectionAudiobookItem::createThumb(audiobooks[i]));
        }
    }
    
    auto w =0;
    for (int i = 0; i < (int)ui_items.size(); i++) {
        listview->pushBackCustomItem(ui_items[i]);
        if (i <= 4)
        w += ui_items[i]->getContentSize().width + listview->getItemsMargin();
    }
    
    if (CONFIG_MANAGER->isVietnameseStories()) {
        listview->setContentSize(Size(w + listview->getItemsMargin()*2.5, listview->getContentSize().height));
        listview->setPosition(Point(listview->getParent()->getContentSize().width / 2, listview->getPositionY()));
        listview->setScrollBarEnabled(true);
        CCLOG("list pos: %f, %f", listview->getPositionX(), listview->getPositionY());
    }else{
        w -= listview->getItemsMargin();
        if (w < 810) {
            listview->setLayoutComponentEnabled(false);
            listview->setContentSize(Size(w, listview->getContentSize().height));
        }
        listview->setPosition(Point(listview->getParent()->getContentSize().width / 2, listview->getPositionY()));
    }
   

    listview->addTouchEventListener([=](Ref * sender, ui::Widget::TouchEventType event) {
        switch (event) {
            case ui::Widget::TouchEventType::BEGAN:
                _time_touch_start = utils::getTimeInMilliseconds();
                break;

            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED: {
                auto current_time = utils::getTimeInMilliseconds();
                auto current_position = listview->getTouchEndPosition();
                auto start_position = listview->getTouchBeganPosition();
                if (current_position.fuzzyEquals(start_position, 5) && current_time - _time_touch_start < 500) {
                    this->processClickEvent(start_position, listview);
                }
                break;
            }

            default:
                break;
        }
    });


    auto listview_lesson = utils::findChild<ui::ListView *>(this, "listview_free_lessons");
    listview_lesson->setScrollBarEnabled(false);
     auto width = 0.f;
    if(CONFIG_MANAGER->isVietnameseStories()){
        
        if(audiobooks.size() < 2){
            listview_lesson->setVisible(false);
            utils::findChild<ui::Text *>(this, "audios.update.text")->setVisible(true);
        }else{
            for (int i = 0; i < 2; i++) {
                auto b = audiobooks[i];
                auto audio_item = MSCollectionAudiobookItem::createThumb(b);
                listview_lesson->pushBackCustomItem(audio_item);
                width += audio_item->getContentSize().width + listview_lesson->getItemsMargin();
            }
        }
        
        listview_lesson->addTouchEventListener([=](Ref * sender, ui::Widget::TouchEventType event) {
            switch (event) {
                case ui::Widget::TouchEventType::BEGAN:
                    _time_touch_start = utils::getTimeInMilliseconds();
                    break;
                    
                case ui::Widget::TouchEventType::ENDED:
                case ui::Widget::TouchEventType::CANCELED: {
                    auto current_time = utils::getTimeInMilliseconds();
                    auto current_position = listview_lesson->getTouchEndPosition();
                    auto start_position = listview_lesson->getTouchBeganPosition();
                    if (current_position.fuzzyEquals(start_position, 5) && current_time - _time_touch_start < 500) {
                        this->processClickEvent(start_position, listview_lesson);
                    }
                    break;
                }
                    
                default:
                    break;
            }
        });

    }else{
        
        int currentGrade = ms::GradeManager::getInstance().getCurrentGradeId();
        
        ms::LessonManager::getInstance().setCurrentLessonFreeForCategory(currentGrade);
        
        auto lesson_ids = ms::LessonManager::getInstance().getFreeLessonId(currentGrade);

        //ms::LessonManager::getInstance().setListLessonFree(lesson_ids);
       
        for (auto i = 0; i < lesson_ids.size(); ++i) {
            auto lesson_info = ms::LessonManager::getInstance().getLessonById(lesson_ids[i]);
            
            if (lesson_info.lesson_id <= 0) {
                continue;;
            }
            
            if (i != 0) lesson_info.state = mj::model::LessonInfo::State::LOCKED;
            auto lesson_item = MSCollectionLessonItem::createItem(lesson_info);
            lesson_item->setDelegate(this);
            listview_lesson->pushBackCustomItem(lesson_item);
            width += lesson_item->getContentSize().width + listview_lesson->getItemsMargin();
        }
    }
    width -= listview_lesson->getItemsMargin();
    
    listview_lesson->setContentSize(Size(width, listview_lesson->getContentSize().height));
    listview_lesson->setPosition(Point(listview_lesson->getParent()->getContentSize().width / 2, listview_lesson->getPositionY()));
    
    if(!CONFIG_MANAGER->isVietnameseStories()){
        auto placeholder = utils::findChild(this, "gradebox_placeholder");
        const int k_currentGradeID = ms::GradeManager::getInstance().getCurrentGradeId();
        auto gradebox = MSGradeBox::createBox(false,k_currentGradeID);
        gradebox->setDelegate(this);
        gradebox->setPosition(placeholder->getPosition());
        gradebox->setAnchorPoint(placeholder->getAnchorPoint());
        placeholder->getParent()->addChild(gradebox);
        placeholder->removeFromParent();
    }
    
    changeTextDisplayLang();
}

#include "../story/popup/ViralPopup/ViralPopup.h"
void MSTabFree::onEnter() {

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
    ui::Layout::onEnter();

    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        
    }else{
        if (!CONFIG_MANAGER->isAppActive() && CONFIG_MANAGER->isCountryVN()) {
            auto show_viral = MJDEFAULT->getIntegerForKey(key_show_viral);
            if (MJDEFAULT->getIntegerForKey(key_show_viral) == 2 ){
                //TODO
                //viral popup
                this->scheduleOnce([](float dt) {
//                    ViralPopup::show();
                }, 0.5, "viral popup");
                MJDEFAULT->setIntegerForKey(key_show_viral, 0);
            }else{
                show_viral+=1;
                MJDEFAULT->setIntegerForKey(key_show_viral, show_viral);
            }
        }
    }
}

void MSTabFree::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview_free_stories");
    auto listview_lesson = utils::findChild<ui::ListView *>(this, "listview_free_lessons");
    if (listview) {
        listview->setPosition(Point(listview->getParent()->getContentSize().width / 2, listview->getPositionY()));
        if(!CONFIG_MANAGER->isVietnameseStories()){
            listview->setContentSize(Size(this->getContentSize().width, listview->getContentSize().height));
        }
        listview->setLeftPadding(50);
        listview->setRightPadding(50);
        
        // set lại position
        float contentWidth = this->getContentSize().width;
        float w = listview->getLeftPadding() + listview->getRightPadding() - listview->getItemsMargin();
        for (auto child : listview->getChildren()) {
            w += child->getContentSize().width + listview->getItemsMargin();
        }
        if (w < contentWidth) {
            listview->setPosition(Point( listview->getParent()->getContentSize().width/2 + (contentWidth - w)/2, listview->getPosition().y ));
        }
    }
    if (listview_lesson)
        listview_lesson->setPosition(Point(listview_lesson->getParent()->getContentSize().width / 2, listview_lesson->getPositionY()));
}

void MSTabFree::changeTextDisplayLang()
{
    
    std::vector<cocos2d::ui::Text*> textSameFonts =
    {
        utils::findChild<ui::Text *>(this, "free.lesson.title"),
        utils::findChild<ui::Text *>(this, "free.story.title"),
        utils::findChild<ui::Text *>(this, "audios.update.text"),
        utils::findChild<ui::Text *>(this, "stories.update.text"),
    };
    
    for(auto text : textSameFonts)
    {
        if(!text)
        {
            continue;
        }
        
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }
    
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        utils::findChild<ui::Text *>(this, "free.story.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.story.vn.title"));
        utils::findChild<ui::Text *>(this, "free.lesson.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.lesson.vn.title"));
        utils::findChild<ui::Text *>(this, "stories.update.text")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("free.story.vn.sorry"));
        utils::findChild<ui::Text *>(this, "audios.update.text")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("free.story.vn.sorry"));
    }
    else
    {
      
        
        utils::findChild<ui::Text *>(this, "free.story.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.story.title"));
        utils::findChild<ui::Text *>(this, "free.lesson.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "free.lesson.title"));
    }
   
}

void MSTabFree::onLearnLesson(mj::model::LessonInfo lesson) {
    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
    //    auto current_activity = ms::LessonManager::getInstance().getLastActivityIdOfCategory(lesson.category_id);
    auto current_lesson = ms::LessonManager::getInstance().getLessonById(ms::LessonManager::getInstance().getLastLessonIdOfCategory(lesson.category_id));
    auto next_lesson = ms::LessonManager::getInstance().getNextLesson(current_lesson);
    if (next_lesson.lesson_id == lesson.lesson_id) {
        //    if (current_activity == activities[0].activity_id) {
        mj::model::ActInfo temp;
        temp.activity_id = -1;
        temp.lesson_id = lesson.lesson_id;
        
        auto popup = MSActivityEnd::createPopup(activities[0], false);
        popup->setDelegate(this);
        popup->setName("popup_start");
        Director::getInstance()->getRunningScene()->addChild(popup, 1000);
    } else {
        auto preview = MSLessonPreview::createView(lesson);
        preview->setName("lesson_preview");
        preview->setPlayFrom("tab_free");
        if (auto mshome = (MSHome *)Director::getInstance()->getRunningScene()->getChildByName(MSHOME_NAME)) {
            preview->setContentSize(mshome->getContentSize());
            ui::Helper::doLayout(preview);
            Director::getInstance()->getRunningScene()->addChild(preview);
        }
        preview->setOpacity(0);
        preview->runAction(FadeIn::create(0.25));
    }
}

void MSTabFree::onContinueLearn(mj::model::ActInfo info) {

    if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        tmp->removeFromParent();
    }

    if (auto wrapper = MSActivityWrapper::createWithInfo(info)) {
        wrapper->setName("wrapper");
        Director::getInstance()->getRunningScene()->addChild(wrapper,INT_MAX-1);
    }

    Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
};

#include "MJAPIStoryDetails.h"
void MSTabFree::processClickEvent(cocos2d::Point touch_position, ui::ListView* list) {
//    auto listview = utils::findChild<ui::ListView *>(this, "listview_free_stories");
    auto children = list->getItems();
    for (auto c : children) {
        auto bb = c->getBoundingBox();
        bb.origin = c->getParent()->convertToWorldSpace(bb.origin);
        if (bb.containsPoint(touch_position)) {
            auto story_item = dynamic_cast<MSCollectionStoryItem *>(c);
            if (story_item) {
                mj::helper::playTransitionStoryCover(story_item, this, 1);
            }
            
            auto audiobook_item = dynamic_cast<MSCollectionAudiobookItem *>(c);
            if (audiobook_item) {
                mj::helper::playTransitionAudiobookCover(audiobook_item, this, 1);
            }
        }
    }
}

void MSTabFree::onDeselectGradeBox(int grade_id) {
    auto listview_lesson = utils::findChild<ui::ListView *>(this, "listview_free_lessons");
    listview_lesson->removeAllItems();
    auto lesson_ids = ms::LessonManager::getInstance().getFreeLessonId(grade_id);
    auto width = 0.f;
    for (auto i = 0; i < lesson_ids.size(); ++i) {
        auto lesson_info = ms::LessonManager::getInstance().getLessonById(lesson_ids[i]);
        
        if (lesson_info.lesson_id <= 0) {
            continue;;
        }
        
        if (i != 0) lesson_info.state = mj::model::LessonInfo::State::LOCKED;
        auto lesson_item = MSCollectionLessonItem::createItem(lesson_info);
        lesson_item->setDelegate(this);
        listview_lesson->pushBackCustomItem(lesson_item);
        width += lesson_item->getContentSize().width + listview_lesson->getItemsMargin();
    }
    width -= listview_lesson->getItemsMargin();
    
    listview_lesson->setContentSize(Size(width, listview_lesson->getContentSize().height));
    listview_lesson->setAnchorPoint(Point::ANCHOR_MIDDLE);
    listview_lesson->setPosition(Point(listview_lesson->getParent()->getContentSize().width / 2, listview_lesson->getPositionY()));
}

void MSTabFree::onBackToLesson() {
    Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
}
