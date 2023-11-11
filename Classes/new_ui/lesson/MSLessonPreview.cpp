//
//  MSLessonPreview.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/14/17.
//

#include "MSLessonPreview.h"
#include "MSActivityPreview.h"
#include "StoryDataModel.h"
#include "MSActivityWrapper.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSLessonManager.h"
#include "MJDefinitions.h"
#include "StoryAlert.h"
#include "AudioEngine.h"
#include "MSActivityEnd.h"
#include "GAFWrapper.h"
#include "manager/CleverTapManager.h"
#include "MJPlatformConfig.h"
#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "HelperManager.h"
#include "MSGradeBox.h"
#include "APProfileManager.h"
#include "MMFirebaseDatabase.h"
#include "MJBaseActivity.h"
#include "Common.h"
#include "F2PManager.h"
USING_NS_CC;
INITIALIZE_READER(MSLessonPreview);

int MSLessonPreview::_activity_lock_audio_id = -1;

std::once_flag ms_lesson_preview_reader;

MSLessonPreview* MSLessonPreview::_msLesson;
MSLessonPreview* MSLessonPreview::getInstance()
{
    return _msLesson;
}
Scene* MSLessonPreview::createScene(mj::model::LessonInfo lesson)
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = MSLessonPreview::createView(lesson);
        view->_isPushSence = true;
        auto myScreenContentSize = Director::getInstance()->getVisibleSize();
        scene->setContentSize(myScreenContentSize);
        view->setContentSize(scene->getContentSize());
        view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        view->setContentSize(scene->getContentSize());
        view->setPosition(Vec2(myScreenContentSize.width / 2, myScreenContentSize.height / 2));
        ui::Helper::doLayout(view);
        view->setName("lesson_preview");
        scene->addChild(view);
        
        if (lesson.lesson_id == 541 || lesson.lesson_id == 539) { // fix cứng bài học phonic đầu tiên option 10 bài(lesson_id = 541) và 7 bài(lesson_id = 539) lần đầu vào sẽ được highligh học luôn bài học

            auto a_id = ms::LessonManager::getInstance().getActivityShouldLearn(lesson.category_id);

            std::string lessonLearned = MJDEFAULT->getStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_f2p)));

            if ((a_id == 0 || a_id == -1) && lessonLearned.length() == 0) {
                mj::model::ActInfo temp;
                temp.activity_id = -1;
                temp.lesson_id = lesson.lesson_id;

                auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
                auto popup = MSActivityEnd::createPopup(activities[0], false);
                popup->setDelegate(view);
                popup->setName("popup_start");
                scene->addChild(popup, 1000);
            }
        }
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}
MSLessonPreview * MSLessonPreview::createView(mj::model::LessonInfo info)
{
    std::call_once(ms_lesson_preview_reader, []
    {
        REGISTER_CSB_READER(MSLessonPreview);
    });
    
    auto node = reinterpret_cast<MSLessonPreview *>(CSLoader::createNode("new_ui/lesson/LessonPreview.csb"));

    if (node)
    {
        node->didLoadFromCSB(info);
        _msLesson = node;
        return node;
    }
    
    CC_SAFE_DELETE(node);

    return nullptr;
}


MSLessonPreview::~MSLessonPreview()
{
	pushCleverTapTrackingData();

    // show the sidebar
    if (auto sidebar = cocos2d::utils::findChild(Director::getInstance()->getRunningScene(), "msSideBar")) {
        sidebar->setVisible(true);
    }

    if(F2P_MANAGER->isFreeContentPage())
        _gradeIdPhonic = PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_f2p);
    MJDEFAULT->setStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, _gradeIdPhonic), _textNextAct);
    if (numberLearnAct > -1)
    {
        mm::RealtimeDatabase::getInstance().updateActivityLearnOfLessonPhonic(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _gradeIdPhonic, numberLearnAct);
    }

    if (_isPushSence && Director::getInstance()->getRunningScene())
    {
        Director::getInstance()->popScene();
        _isPushSence = false;
    }
}

void MSLessonPreview::didLoadFromCSB(mj::model::LessonInfo info)
{
    _currentProfileId = MJDEFAULT->getIntegerForKey(key_current_profile_id);
    MJDEFAULT->setStringForKey(key_name_last_learn_activity, info.name);
    CleverTapManager::GetInstance()->pushProfileByID(MJDEFAULT->getIntegerForKey(key_current_profile_id),CleverTapManager::PUSH_TYPE::LOGIN,nullptr, true);
	m_curretActivityinfo.activity_id = -100;
    is_phonic_lesson = ms::LessonManager::getInstance().isPhonicLesson(info);
	m_currentTime = mj::helper::getTime();

    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height - 120));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender) {
        this->onBack(nullptr);
    });
    this->addChild(fake_layout);
    
    _info = info;
    _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
    this->setClippingEnabled(false);
    
    changeTextDisplayLang();
    
    utils::findChild<ui::ListView *>(this, "listview")->setScrollBarEnabled(false);
    
    auto activity_play_listener = EventListenerCustom::create("play_activity", [=](EventCustom *e) {
        auto act_info = reinterpret_cast<mj::model::ActInfo *>(e->getUserData());
        auto popup = MSActivityEnd::createPopup(* act_info, false);
        popup->setDelegate(this);
        popup->setName("popup_start");
        Director::getInstance()->getRunningScene()->addChild(popup, 1000);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(activity_play_listener, this);
    
    auto activity_play_locked_listener = EventListenerCustom::create("play_activity_locked", [=](EventCustom *e) {
        if (_activity_lock_audio_id == -1) {
            _activity_lock_audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/ui/activity_lock.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(_activity_lock_audio_id, [](int, std::string) {
                MSLessonPreview::_activity_lock_audio_id = -1;
            });
        }
        
        int saved_index = -1;
        for (auto i = 0; i < (int) _activities.size(); ++i) {
            auto a = _activities[i];
            if (a.state == mj::model::ActInfo::READY) {
                saved_index = i;
            }
        }
        
        auto listview = utils::findChild<ui::ListView *>(this, "listview");
        listview->scrollToItem(saved_index, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        
        auto item = utils::findChild(this, "button_continue");
        if (item->getChildByName("gaf")) {
            return;
        }
        auto gaf = GAFWrapper::createGAF("gaf/hand_tap/hand_tap.gaf");
        gaf->setName("gaf");
        gaf->setPosition(Point(item->getContentSize() / 2) + Point(20, -20));
        item->addChild(gaf, INT_MAX - 1);
        gaf->setOpacity(0);
        gaf->runAction(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node * n) {
            reinterpret_cast<GAFWrapper *>(n)->play(true);
        }), DelayTime::create(1.25), FadeOut::create(0.25), RemoveSelf::create(), NULL));
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(activity_play_locked_listener, this);
    
    auto reload_state = EventListenerCustom::create(key_reload_lesson_state, [=](EventCustom * e) {
        this->reloadState();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(reload_state, this);
    
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
}

void MSLessonPreview::onExit()
{
    cocos2d::ui::Layout::onExit();

    mj::helper::purgeTextureData();
}

void MSLessonPreview::reloadState()
{
    mj::helper::SetFpsByNumberFrames(10);
    if (_activities.empty())
    {
        return;
    }
    
    mj::model::ActInfo next_activity;
    
    auto saved_state = mj::model::ActInfo::LEARNED;
    
    // Nếu là MS và Lessons-Comprehension
    if (!CONFIG_MANAGER->isVietnameseStories() && !is_phonic_lesson)
    {
        next_activity = ms::LessonManager::getInstance().getActivityById(ms::LessonManager::getInstance().getActivityShouldLearn_4notPhonics(_info.category_id, _info.lesson_id));
    }
    // Các trường hợp còn lại
    else
    {
        auto lesson_should_learn = ms::LessonManager::getInstance().getLessonShouldLearn(_info.category_id);
        
        _gradeIdPhonic = PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_phonic);
        if (F2P_MANAGER->isFreeContentPage())
        {
            _gradeIdPhonic = PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_f2p);
        }

        if (lesson_should_learn != _info.lesson_id && !F2P_MANAGER->isFreeContentPage())
        {
            next_activity.activity_id = -1;
        }
        else
        {
            next_activity = ms::LessonManager::getInstance().getActivityById(ms::LessonManager::getInstance().getActivityShouldLearn(_info.category_id));
            if (F2P_MANAGER->isFreeContentPage() && next_activity.lesson_id!= _gradeIdPhonic)
            {
                next_activity.lesson_id = _info.lesson_id;
                next_activity.activity_id = -1;
            }
            if (next_activity.activity_id == -1)
            {
                next_activity = _activities[0];
            }
        }

        // Fix khách mất lộ trình khi học act bên Phonic
        std::string lessonLearned = MJDEFAULT->getStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, _gradeIdPhonic), "");


        int count = 0;

        if (lessonLearned.length() > 0)
        {
            count = std::stoi(lessonLearned);
            next_activity.activity_id = _activities[count].activity_id; // indexText;
            MJDEFAULT->deleteValueForKey(StringUtils::format(key_number_activity_of_grade_phonic, _gradeIdPhonic));
        }

    }
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    bool is_first_push_items = listview->getItems().size() ? false : true; // nếu listview chưa có item nào thì đây đây là lần đầu tiên gọi reloadState và push item
    listview->removeAllItems();
    
    auto index_in_lesson = 0;

    if (!_activities.empty()) {

        for (int i = 0; i < _activities.size(); i++)
        {
            _activities[i].zip_urls.erase(unique(_activities[i].zip_urls.begin(), _activities[i].zip_urls.end()), _activities[i].zip_urls.end());
        }
    }

    if (F2P_MANAGER->isFreeContentPage() && _activities.size()>0) {
        if (_info.state != mj::model::LessonInfo::State::LEARNED)
        {
            _info.state = mj::model::LessonInfo::State::READY;
        }
    }
    
    for (auto i = 0; i < _activities.size(); ++i) {
        
        if ((!CONFIG_MANAGER->isVietnameseStories() && !is_phonic_lesson && _info.is_learned) || (F2P_MANAGER->isFreeContentPage() &&_info.is_learned)) {
            _activities[i].state = mj::model::ActInfo::State::LEARNED;
        } else if (i == 0 && !CONFIG_MANAGER->isVietnameseStories() && !is_phonic_lesson) {
            // activity đầu tiên của Lesson-Comprehension luôn luôn được mở
            _activities[i].state = mj::model::ActInfo::State::READY;
            if (_activities[i].activity_id == next_activity.activity_id) {
                saved_state = mj::model::ActInfo::State::LOCKED;
                index_in_lesson = i;
            }
        } else if (_info.state == mj::model::LessonInfo::State::LEARNED) {
            _activities[i].state = mj::model::ActInfo::State::LEARNED;
        } else if (_info.state == mj::model::LessonInfo::State::READY) {
            if (_activities[i].activity_id == next_activity.activity_id) {
                _activities[i].state = mj::model::ActInfo::State::READY;
                numberLearnAct = i;
                _textNextAct = std::to_string(i);

                saved_state = mj::model::ActInfo::State::LOCKED;
                index_in_lesson = i;
            } else {
                _activities[i].state = saved_state;
            }
        }
        listview->pushBackCustomItem(MSActivityPreview::createView(_activities[i], false, false));
    }

    if (numberLearnAct == _activities.size() - 1)
    {
        int countActivityLearn = 0;
        for (auto i = 0; i < _activities.size(); ++i) {
            if (_activities[i].state == mj::model::LessonInfo::State::LEARNED)
            {
                countActivityLearn++;
            }
        }
        if (countActivityLearn == _activities.size())
        {
            numberLearnAct = 0;
            _textNextAct = std::to_string(0);
        }
    }
    
	m_curretActivityinfo = next_activity;

    listview->jumpToItem(index_in_lesson, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);

    number_activity_learned = number_activity_learned + 1;
    
    // dịch listview của các activity về bên phải một chút, để tránh bị tai thỏ iPhone đè lên
    if (is_first_push_items) {
        listview->setLeftPadding(MJ_PLATFORM_CONFIG.getNotchHeight());
    }
}

void MSLessonPreview::onBack(Ref * sender)
{    
    mj::helper::playButtonFXClose();

    this->removeFromParent();

   // Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
}

void MSLessonPreview::onPlay(Ref * sender)
{
    mj::helper::playButtonFX();
    mj::model::ActInfo temp;
    
    // Nếu là MS và Lesson-Comprehension thì lấy act READY xa nhất
    if (!CONFIG_MANAGER->isVietnameseStories() && !is_phonic_lesson)
    {
        temp = _activities[0];
        
        for (auto i=_activities.size()-1; i>0; i--)
        {
            auto acttt = _activities.at(i);
            if (acttt.state == mj::model::ActInfo::State::READY)
            {
                temp = acttt;
            }
        }
    }
    else
    {
        auto it = std::find_if(_activities.begin(), _activities.end(), [](mj::model::ActInfo a) {
            return a.state == mj::model::ActInfo::State::READY;
        });
        
        if (it != std::end(_activities))
        {
            temp = * it;
        }
        else
        {
            temp = _activities[0];
        }
    }
    
    auto popup = MSActivityEnd::createPopup(temp, false);
    popup->setDelegate(this);
    popup->setName("popup_start");
    Director::getInstance()->getRunningScene()->addChild(popup, 1000);
}

void MSLessonPreview::onBackToLesson()
{
    if (auto popupStart =utils::findChild(Director::getInstance()->getRunningScene(), "popup_start")) {
        popupStart->removeFromParent();
    }

    if (auto wrapper = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        wrapper->removeFromParent();
    }
}

void MSLessonPreview::onContinueLearn(mj::model::ActInfo info)
{
    if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        tmp->removeFromParent();
    }
    info.startFrom = mj::model::ActInfo::START_FROM::START_LESOON;
    if (auto wrapper = MSActivityWrapper::createWithInfo(info)) {
        wrapper->setName("wrapper");
        Director::getInstance()->getRunningScene()->addChild(wrapper, INT_MAX - 1);
        wrapper->setVisible(false);
    }
}

ui::Widget::ccWidgetClickCallback MSLessonPreview::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSLessonPreview, onBack),
        CLICK_MAP(MSLessonPreview, onPlay),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSLessonPreview::onEnter()
{
    _lessonID = _info.lesson_id;
    ui::Layout::onEnter();
    reloadState();

    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    auto lesson_end_listener = EventListenerCustom::create(key_lesson_ended, [this](EventCustom *)
    {

        this->removeFromParent();
        
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lesson_end_listener, this);
}

void MSLessonPreview::changeTextDisplayLang()
{
    if(auto title = utils::findChild<ui::Text*>(this, "title"))
    {
        
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ? title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-SemiBold.ttf");
        
  
		title->setString(StringUtils::format("%s %d: %s", LANGUAGE_MANAGER->getDisplayTextByKey("lesson.item.title").c_str(),_info.lesson_order, _info.name.c_str()));
    }

    if( auto activitiesStatusText =  utils::findChild<ui::Text *>(this, "activities_status"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  activitiesStatusText->setFontName("fonts/leelawdb.ttf") : activitiesStatusText->setFontName("fonts/Montserrat-Regular.ttf");

        activitiesStatusText->setString(StringUtils::format("%d/%d %s", 0, (int)_activities.size(), LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.lessonpreview.actistatus").c_str()));
    }
    
    if(auto textStart =  utils::findChild<ui::Text *>(this, "text_start"))
    {
        /*
        * Nút bấm vào để học.
        * Nếu học đến cuối bài học, nút này là "Replay"
        * Nếu học chưa đến cuối, nút này là "Continue"
        */
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  textStart->setFontName("fonts/leelawdb.ttf") : textStart->setFontName("fonts/Montserrat-ExtraBold.ttf");

        if (_info.state == mj::model::LessonInfo::State::LEARNED) {
            textStart->setString(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "key.reply").c_str()));
        }
        else {
            textStart->setString(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.lessonpreview.start").c_str()));
        }
        
        if(auto parent = dynamic_cast<Button*>(textStart->getParent()))
        {
            if(parent->getContentSize().width <textStart->getContentSize().width)
            {
                parent->setScale9Enabled(true);
                parent->setContentSize({textStart->getContentSize().width*1.05f,parent->getContentSize().height});//105%
            }
        }
    }
    
    auto title_text = utils::findChild<ui::Text *>(this, "title");
    
    auto title = title_text->getString();
    
    auto story_name = title.substr(title.find(':') + 1);
    
    auto length = StringUtils::getCharacterCountInUTF8String(title) - StringUtils::getCharacterCountInUTF8String(story_name);
    
    for (auto i = 0; i < length; ++i)
    {
        auto letter = title_text->getLetter(i);
        letter ? letter->setColor(Color3B::RED) : (void) nullptr;
    }
    
    auto title_bb = title_text->getBoundingBox();
    title_bb.origin = title_text->getParent()->convertToWorldSpace(title_bb.origin);
    
    auto last_x = title_bb.origin.x + title_bb.size.width + 50;
    auto temp_point = Vec2(last_x, 0);
    
//    auto dot = utils::findChild(this, "dot");
//    temp_point = dot->getParent()->convertToNodeSpace(temp_point);
//    temp_point.x = std::min(temp_point.x, 600.f);
//    dot->setPosition(Point(temp_point.x, dot->getPositionY()));
//
//    auto line = utils::findChild(this, "line");
//    line->setPosition(dot->getPosition());
}

void MSLessonPreview::pushCleverTapTrackingData()
{
    // Vmonkey không nhảy vào đây, event "learn_lesson" được push ở bằng hàm MSActivityWrapper::pushCleverTapTrackingData_Vmonkey
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (numberLearnAct > -1)
    {
        mm::RealtimeDatabase::getInstance().updateActivityLearnOfLessonPhonic(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _gradeIdPhonic, numberLearnAct);
    }
#endif
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        return;
    }
    
    try
    {
        int currentActivityID = m_curretActivityinfo.activity_id;
        
        if (currentActivityID != -1)
        {
            auto pos = std::find_if(_activities.begin(), _activities.end(), [=](mj::model::ActInfo acInfo) {
                return acInfo.activity_id == m_curretActivityinfo.activity_id;
            });

            currentActivityID = std::distance(_activities.begin(), pos) + 1;
        };

        currentActivityID = (currentActivityID == -1 ? _activities.size() : currentActivityID);
        
        // thay thế các ký tự ',' sang '_'
        std::string str_lesson_name = _info.name;
        mj::helper::replace_string_all(str_lesson_name, ",", "_");
        std::string nameActivity = m_curretActivityinfo.name;

        int time_learn_lesson = int(utils::getTimeInMilliseconds() / 1000) - MJDEFAULT->getIntegerForKey(key_time_start_learn_lesson);
        float completedLesson = ((float)number_activity_learned / (float)_activities.size()) * 100.f;
        if (completedLesson > 100)
        {
            completedLesson = 100;
        }
        CleverTapManager::GetInstance()->pushEvent("learn_lesson", {
            {"lesson_id",cocos2d::Value(_info.lesson_id)},
            {"lesson_name",cocos2d::Value(str_lesson_name)},
            {"course_type",cocos2d::Value(MJDEFAULT->getBoolForKey(key_is_phonic_lesson) ? "lesson_phonics " : "lesson_comprehension")},
            {"grade_id",cocos2d::Value(_info.category_id)},
            {"grade_name",cocos2d::Value(MJDEFAULT->getStringForKey("GRADE_NAME", "unknow"))},
            {"read_from",cocos2d::Value(m_playFrom)},
            {"completed",cocos2d::Value((int)completedLesson)},
            {"number_activities",cocos2d::Value((int)_activities.size())},
            {"time_spent",cocos2d::Value(time_learn_lesson)},
            {"stop_at_activity",cocos2d::Value(currentActivityID)},
            {"time_purchased", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_purchased",0)))},
            {"time_activated",cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_activated",0)))},
            {"time_expired", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_expire",0))) },
            {"stop_at_activity_name", cocos2d::Value(nameActivity)},
            });
    }
    catch (const std::exception&)
    {

    }
}
