//
//  MSTabLessons.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#include "MSTabLessons.h"
#include "MSCollectionLessonItem.h"
#include "MSComplexCollection.h"
#include "StoryDataModel.h"
#include "MSActivityPreview.h"
#include "MSLessonPreview.h"
#include "MJDefinitions.h"
#include "MSLessonManager.h"
#include "MSActivityEnd.h"
//#include "MSActivityWrapper.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "MSEventLogger.h"
#include "StoryAlert.h"
#include "MMFirebaseDatabase.h"
#include "HelperManager.h"
#include "MSSidebar.h"
#include "APProfileManager.h"

#define MSTAB_LESSON_SCENCE_NAME "MSTabLessons_scence"
#define MSTAB_LESSON_VIEW_NAME "MSTabLessons_view"

/*
 // * 1. chưa tải, bấm tải -> hiện button học
 // * 2. bấm học -> vào popup giới thiệu activity (đọc, không có play, tự động mất) -> vào học.
 // * 3. Popup giới thiệu (bỏ lesson, phóng to activity, đọc activity 1, learn vocab...)
 // * 4. Nếu đang học dở, sẽ vào trang activities
 // * 5. Bấm vào từng activity, sẽ tương tự khi ấn nút học
 // * 6. Chuyển từ activity->activity, hiện nút play
 // * 7. Xong bài học, quay lại trang bài học
 * 8. Close
 */


USING_NS_CC;
INITIALIZE_READER(MSTabLessons);

std::once_flag ms_tab_lessons_reader;

Scene* MSTabLessons::createScene(ms::GradeManager::Scope scope, int force_cat_id, int force_les_id)
{
    auto scene = Scene::create();

    if (scene)
    {
        scene->setName(MSTAB_LESSON_SCENCE_NAME);
        auto view = MSTabLessons::createTab(scope);
        view->setName(MSTAB_LESSON_VIEW_NAME);
        view->setContentSize(Director::getInstance()->getVisibleSize());
        scene->addChild(view, INT_MAX - 1);
        if (force_cat_id > 0 && force_les_id > 0) {
            if (scope == ms::GradeManager::Scope::LESSONS)
            {
                view->loadCustomGradeWithID(force_cat_id);
            }
            view->openLessonForce(force_cat_id, force_les_id);
        }
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

MSTabLessons * MSTabLessons::createTab(ms::GradeManager::Scope scope)
{
    mj::helper::SetFpsByNumberFrames(8);
    std::call_once(ms_tab_lessons_reader, []
    {
        REGISTER_CSB_READER(MSTabLessons);
    });

    auto csb_name = mj::helper::getCSBName("csb/new_ui/Lessons.csb");
    
    auto tab = reinterpret_cast<MSTabLessons *>(CSLoader::createNode(csb_name));
    
    if (tab)
    {
        tab->didLoadFromCSB(scope);
        return tab;
    }
    
    CC_SAFE_DELETE(tab);
    return nullptr;
}

static std::pair<int, int> s_gradeIDsCache = { -1,-1 };

int MSTabLessons::countCompletedLessons() {
    
    std::vector<int> listIdOfLessonsLearned = PROFILE_MANAGER.getAllLessonLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
    int count = 0;

    for (int i = 0; i < _lessons.size(); i++) {
        if (_lessons[i].is_learned) {
            count++;
        }
        else
        {
            if (checkLessonLearned(_lessons[i].lesson_id, listIdOfLessonsLearned))
            {
                count++;
            }
        }
    }
    return count;
}

void MSTabLessons::didLoadFromCSB(ms::GradeManager::Scope scope)
{
    if (CONFIG_MANAGER->isVietnameseStories()){
        if (utils::findChild(this, "gradient_top")){
            utils::findChild(this, "gradient_top")->setVisible(false);
        }
        if (utils::findChild(this, "gradient_bottom")){
            utils::findChild(this, "gradient_bottom")->setVisible(false);
        }
        
        utils::findChild<Layout*>(this, "navi")->setBackGroundColor(Color3B(107,113,250));
        ms::GradeManager::getInstance().setCurrentGradeId(764);
    }

    auto layout_block_touch = ui::Layout::create();
    layout_block_touch->setContentSize(Director::getInstance()->getVisibleSize());
    layout_block_touch->setTouchEnabled(true);
    layout_block_touch->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    layout_block_touch->setPosition(Point::ZERO);
    layout_block_touch->setName("layout_block_touch");
    this->addChild(layout_block_touch, INT_MAX);

    this->setClippingEnabled(false);

	m_scope = scope;
	ms::GradeManager::getInstance().setScope(m_scope);
	MJDEFAULT->setBoolForKey(key_is_phonic_lesson, m_scope == ms::GradeManager::Scope::LESSONS_PHONIC);

	if (m_scope == ms::GradeManager::Scope::LESSONS_PHONIC)
	{
		s_gradeIDsCache.second = ms::GradeManager::getInstance().getCurrentGradeId();
	}
	else if (m_scope == ms::GradeManager::Scope::LESSONS)
	{
		s_gradeIDsCache.first = ms::GradeManager::getInstance().getCurrentGradeId();
	}

    auto gradebox_placeholder = utils::findChild(this, "gradebox");
    _gradeBox = MSGradeBox::createBox(true, m_scope == ms::GradeManager::Scope::LESSONS_PHONIC? s_gradeIDsCache.second: s_gradeIDsCache.first);
    _gradeBox->setPosition(gradebox_placeholder->getPosition());
    _gradeBox->setAnchorPoint(gradebox_placeholder->getAnchorPoint());
    _gradeBox->setDelegate(this);
    _gradeBox->setName("gradebox");
    gradebox_placeholder->getParent()->addChild(_gradeBox, 1);
    //// < Dinh Duc - Search Button
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    m_search_button = ui::Button::create("mjstory/new_ui/icon_searchbar_search.png", "", "");
    m_search_button->setAnchorPoint(gradebox_placeholder->getAnchorPoint());
    m_search_button->setPosition(_gradeBox->getPosition() + Vec2(_gradeBox->getContentSize().width, 0));
    gradebox_placeholder->getParent()->addChild(m_search_button, 2);
    m_search_button->addClickEventListener(CC_CALLBACK_1(MSTabLessons::onSelectSearch,this));
    //// >
    #endif
    gradebox_placeholder->removeFromParent();
    
    m_gradeID   = ms::GradeManager::getInstance().getCurrentGradeId();
    if (m_scope == ms::GradeManager::Scope::LESSONS && m_gradeID > 400) {    
        m_gradeID = s_gradeIDsCache.first == -1 ? 84 : s_gradeIDsCache.first;
        if (m_gradeID > 400)
            m_gradeID = 84;
    }
    _lessons    = ms::LessonManager::getInstance().getAllLessonsByGrade(m_gradeID);
    /*
    * _lesons được lấy theo current gradeID.
    * currentGradeID được lấy theo key cho từng trường hợp: AUDIOBOOKS, LESSON, ...
    * key được xác định theo biến _scope. 
    * Có trường hợp là khi thay đổi grade ở gradebox, thì gọi event grade_change
    * event "grade change" gọi rất nhiều listener ở trong các tab khác.
    * Tab cuối cùng gọi grade change là tab Favorite ( theo mấy lần debug).
    * Trong listener của tab Favourite, _scope được set là LESSONS.
    * Cho nên khi thay đổi grade thì _scope luôn là LESSONS.
    * 
    * Vào trong LESSON_PHONCIS chọn một cấp độ bất kì lần 1, _scope được gán là LESSON.
    * Chọn tiếp một cấp độ bất kì lần 2, hàm setCurrentGradeId ghi vào key của LESSON giá trị
    * của PHONICS hiện tại.
    * Sau đó quay lại tab LESSON SELECT, chọn LESSON, các bài học được hiển thị theo grade 
    *  của PHONICS.
    * Để xử lí trường hợp này, ta chỉ gán cho currentGradeID của LESSON các giá trị nhỏ hơn 400
    *  (Do ID cùa các grade của lesson được fix sẵn là nhỏ hơn 400  (Xem GradeManager)).
    * 
    */
    
    auto lastLessonId = ms::LessonManager::getInstance().getLessonShouldLearn(m_gradeID);
    auto it = std::find_if(_lessons.rbegin(), _lessons.rend(), [this, lastLessonId](mj::model::LessonInfo l) {
        // nếu là MS và Lessons-Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicCategory(m_gradeID)) {
            return l.lesson_id == lastLessonId;
        // các trường hợp khác
        } else {
            return l.state == mj::model::LessonInfo::State::UNLOCKED || l.state == mj::model::LessonInfo::State::READY;
        }
    });
    
    if (it != _lessons.rend())
    {
        _saved_current_row = (int)std::distance(it, _lessons.rend()) - 1;
        if (_saved_current_row == _lessons.size() - 1){
            if ((_lessons[_lessons.size()-1]).is_learned){
                _saved_current_row = (int)_lessons.size();
            }
        }
    }else{
        _saved_current_row = (int)_lessons.size();
    }
    
    _collection = MSComplexCollection::createCollection(this);
    _collection->setDelegate(this);
    utils::findChild(this, "collection")->addChild(_collection);
    
    ui::Helper::doLayout(this);
    this->reloadData();
    
    auto rich = ui::RichText::create();
    rich->pushBackElement(ui::RichElementImage::create(0, Color3B::WHITE, 255, "mjstory/new_ui/image_lesson_check.png"));
    int learnedLessons = countCompletedLessons();
    rich->pushBackElement(ui::RichElementText::create(1, Color3B::BLACK, 255,
                        StringUtils::format("  %d/%d %s", learnedLessons, (int)_lessons.size(),
                        LANGUAGE_MANAGER->getDisplayTextByKey("lessons.completed").c_str()),
                        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  "fonts/leelawdb.ttf": "fonts/Montserrat-Regular.ttf", 20));
    auto navi = utils::findChild(this, "navi");
    auto sz = navi->getContentSize();
    navi->addChild(rich);
    rich->setPosition(Point(navi->getContentSize().width / 2, navi->getContentSize().height / 2));
    rich->setAnchorPoint(Point(0.5, 0.5));
    rich->setName("completed");
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }
        //auto grade_id = reinterpret_cast<int *>(e->getUserData());
        //this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_reload_lesson_state, [this](EventCustom * e)
    {
        this->reloadLessons();
    }), this);
    
    auto back_btn = utils::findChild<Button*>(this, "btnBack");
    back_btn->addClickEventListener(CC_CALLBACK_1(MSTabLessons::onBack, this));
    
    this->scheduleOnce([=](float)
    {
           utils::findChild<Layout*>(this, "layout_block_touch")->removeFromParent();
    }, 1.0f, "remove_block_touch");


    auto click_exit_lesson = utils::findChild<Button*>(this, "button_exit_lesson");
    click_exit_lesson->addClickEventListener(CC_CALLBACK_1(MSTabLessons::onBack, this));
    
    // events...
    auto event_should_close_listener = EventListenerCustom::create(key_event_search_page_close, [=](EventCustom *e) {
        this->removeFromParent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_should_close_listener, this);
}

void MSTabLessons::onEnter() {
    if (CONFIG_MANAGER->isAutoPlay()) {
        _callbackAutoPlay = [this]() {
                //trường hợp đã kết thúc bài học cuối cùng của grade
            auto x = ms::GradeManager::getInstance().getCurrentGradeId();
                if (m_gradeID != ms::GradeManager::getInstance().getCurrentGradeId()) {
                    m_gradeID = ms::GradeManager::getInstance().getCurrentGradeId();
                    //chọn grade tiếp theo ở gradeBox
                    _gradeBox->onReceivedEventGradeChanged(m_gradeID);
                    //Reload lại data sau khi đổi grade ở gradeBox
                    onReceivedEventGradeChanged(m_gradeID);
                    Director::getInstance()->getRunningScene()->scheduleOnce([=](float) {
                        auto curentLesson = ms::LessonManager::getInstance().getCurrentLesson();
                        onLearnLesson(curentLesson);
                        }, 3.0f, "continue_play_and_change_grade");
                }
                else {
                    //học bài học tiếp theo trong grade
                    auto curentLesson = ms::LessonManager::getInstance().getCurrentLesson();
                    if (curentLesson.lesson_id != 0) {
                        onLearnLesson(ms::LessonManager::getInstance().getNextLesson(curentLesson));
                    }
                }
        };
    }

    
    MS_LOGGER.logEvent("view_lesson", {
        {"lesson_type",cocos2d::Value(m_scope == ms::GradeManager::Scope::LESSONS ? "STORIES" : "PHONICS")}
    });
	ms::GradeManager::getInstance().setScope(m_scope);
	MJDEFAULT->setBoolForKey(key_is_phonic_lesson, m_scope == ms::GradeManager::Scope::LESSONS_PHONIC);

	auto reload_state = EventListenerCustom::create("reload_all_lesson_items", [=](EventCustom * e) {
		reloadData();
	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(reload_state, this);
    
    // -- phải reload lessons và lesson items ở onEnter này vì nếu act cuối là story thì scene cũ chứa tabLessons sẽ được gọi lại
    this->reloadLessons();
    this->reloadData();
    // -- end --

    ui::Layout::onEnter();
    runAnimationAppear();
}

void MSTabLessons::reloadLessons(int gradeID) {
    
    if(gradeID!=-1) {
        
        m_gradeID =gradeID;
    }
    
    _lessons = ms::LessonManager::getInstance().getAllLessonsByGrade(m_gradeID);
    
    auto lastLessonId = ms::LessonManager::getInstance().getLessonShouldLearn(m_gradeID);
    auto it = std::find_if(_lessons.rbegin(), _lessons.rend(), [this, lastLessonId](mj::model::LessonInfo l) {
        // nếu là MS và Lessons-Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicCategory(m_gradeID)) {
            return l.lesson_id == lastLessonId;
            // các trường hợp khác
        } else {
            return l.state == mj::model::LessonInfo::State::UNLOCKED || l.state == mj::model::LessonInfo::State::READY;
        }
    });
    
    if (it != _lessons.rend())
    {
        _saved_current_row = (int)std::distance(it, _lessons.rend()) - 1;
        if (_saved_current_row == _lessons.size() - 1){
            if ((_lessons[_lessons.size()-1]).is_learned){
                _saved_current_row = (int)_lessons.size();
            }
        }
    }else{
        _saved_current_row = (int)_lessons.size();
    }
    
    int countLessonLearned = countCompletedLessons();
    auto rich = utils::findChild<ui::RichText *>(this, "completed");
    rich->removeElement(1);
    rich->pushBackElement(ui::RichElementText::create(1, Color3B::BLACK, 255, StringUtils::format("  %d/%d %s", countLessonLearned, (int)_lessons.size(), LANGUAGE_MANAGER->getDisplayTextByKey("lessons.completed").c_str()), (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf"), 20));
    
    if (CONFIG_MANAGER->isAutoPlay()) {
        Director::getInstance()->getRunningScene()->scheduleOnce([=](float) {
            if (_callbackAutoPlay) {
                _callbackAutoPlay();
           }
        }, 10.0f, "continue_play");
    }

}

void MSTabLessons::runAnimationAppear() {
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.3f));
    
    if(!CONFIG_MANAGER->isVietnameseStories())
    {
        if (MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false))
        {
            //this->runAction(Sequence::create(FadeIn::create(0.0f), CallFunc::create([this] {
            //   // HelperManager::GetInstance()->addSidebarBlack();
            //    }), nullptr));
        }
        else
        {
           // deleteSidebarBlack();
            this->runAction(Sequence::create(FadeIn::create(0.5f), CallFunc::create([this] {
                onboarding();
            }), nullptr));
        }
    }
}

void MSTabLessons::onboarding() {

    if (MJDEFAULT->getBoolForKey(key_onboarding_lessons, false) == false)
    {
        auto onboard = CSLoader::getInstance()->createNode("csb/new_ui/intro/Onboard2.csb");
        if (auto onBoardIU = utils::findChild<ui::Widget*>(onboard, "arrow_v")) {
            auto stencil = ui::Scale9Sprite::create("account/grey9path.png");
            stencil->setScale9Enabled(true);
            stencil->setContentSize({ 250,50 });
            stencil->setAnchorPoint(Point(1, 0.5));
            stencil->setPosition(cocos2d::Vec2(getGradePosition().x /*+ getGradeSize().width*/, getGradePosition().y));

            ClippingNode* clipper = ClippingNode::create();
            clipper->setStencil(stencil);
            clipper->setAlphaThreshold(0.8f);
            clipper->setInverted(true);
            this->addChild(clipper, 10);

            auto layerColor = LayerColor::create(Color4B::BLACK);
            layerColor->setIgnoreAnchorPointForPosition(false);
            layerColor->setOpacity(0.75f * 255.0f);
            layerColor->setAnchorPoint(Vec2(0.5f, 0.5f));
            layerColor->setContentSize(Director::getInstance()->getVisibleSize());
            layerColor->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize() / 2);
            clipper->addChild(layerColor);

            auto lockButton = Button::create();
            lockButton->setScale9Enabled(true);
            lockButton->setIgnoreAnchorPointForPosition(false);
            lockButton->setAnchorPoint(Vec2(0.5f, 0.5f));
            lockButton->setPosition(layerColor->getContentSize() / 2);
            lockButton->setContentSize(Director::getInstance()->getVisibleSize());
            layerColor->addChild(lockButton, -1);

            auto guide = onBoardIU->clone();
            guide->setScale(0.8f);
            guide->setName("update_popup");
            this->addChild(guide, 10);

            auto pos = getGradePosition();
            guide->setPosition(pos + Vec2(-getGradeSize().width * 0.5f, -getGradeSize().height * 0.5f));
            
            // save firebase
            MJDEFAULT->setBoolForKey(key_onboarding_lessons, true);
            mm::RealtimeDatabase::getInstance().updateUserSettings();

            if (auto closeButton = utils::findChild<ui::Button*>(guide, "button_next"))
            {
                closeButton->setCascadeColorEnabled(true);

                closeButton->addClickEventListener([=](Ref*)
                {
                    clipper->removeFromParent();
                    guide->removeFromParent();

                    if (auto scene = Director::getInstance()->getRunningScene())
                    {
                        auto check_node = scene->getChildByName("update_popup");

                        if (check_node)
                        {
                            check_node->removeFromParent();
                        }
                    }
                });
            }

            if (auto bg = utils::findChild(guide, "background"))
            {
                bg->setPosition(Vec2(bg->getPosition().x, bg->getPosition().y - 70));
            }

            if (auto labelNext = utils::findChild<ui::Text*>(guide, "label_next"))
            {
                labelNext->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Bold.ttf");

                labelNext->setColor(Color3B::BLACK);
                labelNext->setString(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.finish"));
            }


            if (auto title = utils::findChild<ui::Text*>(guide, "title"))
            {
                title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Bold.ttf");

                title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("new_onboard_title"));
            }

            if (auto optout = utils::findChild(guide, "optout"))
            {
                optout->removeFromParent();
            }
        }
    }
}

cocos2d::Vec2 MSTabLessons::getGradePosition() {
    auto panel = utils::findChild(this, "navi");
    if (auto gradeBox = utils::findChild(this, "gradebox"))
    {
        auto pos = gradeBox->getParent()->convertToWorldSpace(gradeBox->getPosition());

        return Vec2(pos.x, Director::getInstance()->getVisibleSize().height - panel->getContentSize().height * 0.5f);//_searchbar->getParent()->convertToWorldSpace(_searchbar->getPosition());
    }

    return cocos2d::Vec2();
}

cocos2d::Size MSTabLessons::getGradeSize() {
    auto  panel = utils::findChild(this, "navi");
    if (auto gradeBox = utils::findChild(panel, "gradebox")) {
        return gradeBox->getContentSize();
    }

    return cocos2d::Size();
}

void MSTabLessons::runAnimationDisappear() {
    this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
//        auto home = MSHome::createScene();
//        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
}

int MSTabLessons::numberOfCells() {
    return (int)_lessons.size();
}

int MSTabLessons::numberOfColumns() {
    return (int) ((collectionSize().width + 10) / (modelSize().width + 10));
}

cocos2d::ui::Widget * MSTabLessons::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    auto index = row * numberOfColumns() + col;
    if (index < 0) return nullptr;
    if (_ui_source[index]) {
    } else {
        auto lesson_info = _lessons[index];
        auto lesson = MSCollectionLessonItem::createItem(lesson_info, checkLessonLearned(lesson_info.lesson_id, listIdOfLessonsLearned));
        lesson->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        lesson->setDelegate(this);
        _ui_source[index] = lesson;
        
        if (m_scope == ms::GradeManager::Scope::LESSONS_PHONIC)
        {
            if (lesson_info.lesson_id == ms::LessonManager::getInstance().getLastLessonIdOfCategory(lesson_info.category_id))
            {
                lesson->setLastLesson();
            }
        }
    }
    
    return _ui_source[index];
}

bool MSTabLessons::checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned)
{
    for(int i = 0; i < listIdOfLessonsLearned.size(); i ++)
    {
        if (id == listIdOfLessonsLearned[i])
        {
            return true;
        }
    }
    return false;
}

cocos2d::Size MSTabLessons::modelSize() {
    return Size(190, 340) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

cocos2d::Size MSTabLessons::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

bool MSTabLessons::shouldScrollWhenReload() {
    return true;
}

double MSTabLessons::percentScrollWhenReload() {
    auto total_row = ((int)_lessons.size() - 1) / numberOfColumns() + 1;
    auto row = (_saved_current_row - 1) / numberOfColumns() + 1;
    auto percent = (row - 1) * 100.f / (total_row - 1);
    return percent;
}

void MSTabLessons::offLoadCellAt(int index) {
//    if (index < 0 || index >= _ui_source.size()) {
//        return;
//    }
//    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
//    _ui_source[index] = nullptr;
}

void MSTabLessons::onSelectCollectionItem(ui::Widget * item) {
    CCLOG("Selected!!");
}

void MSTabLessons::onLearnLesson(mj::model::LessonInfo lesson) {
    if (CONFIG_MANAGER->isAutoPlay()) {
        Director::getInstance()->getRunningScene()->unschedule("continue_play");
    }
	MS_LOGGER.logEventStartLearning(m_scope == ms::GradeManager::Scope::LESSONS ? "STORIES" : "PHONICS",lesson);
    
	if (auto gradebox = utils::findChild(this, "gradebox"))
	{
		if (auto gradeName = utils::findChild<ui::Text*>(gradebox, "name_current"))
		{
            auto _gradeName = gradeName->getString();
            if (_gradeName.find("Cấp độ 1") != std::string::npos ||
                _gradeName.find("Level 1") != std::string::npos ||
                _gradeName.find("ระดับ 1") != std::string::npos ||
                _gradeName.find("Pre K") != std::string::npos)
            {
                MJDEFAULT->setStringForKey("GRADE_NAME", "Pre K");
            }
            if (_gradeName.find("Cấp độ 2") != std::string::npos ||
                _gradeName.find("Level 2") != std::string::npos ||
                _gradeName.find("ระดับ 2") != std::string::npos ||
                _gradeName.find("Kindergarten") != std::string::npos)
            {
                MJDEFAULT->setStringForKey("GRADE_NAME", "Kindergarten");
            }
            if (_gradeName.find("Cấp độ 3") != std::string::npos ||
                _gradeName.find("Level 3") != std::string::npos ||
                _gradeName.find("ระดับ 3") != std::string::npos ||
                _gradeName.find("Grade 1") != std::string::npos)
            {
                MJDEFAULT->setStringForKey("GRADE_NAME", "Grade 1");
            }
		}
	}
    
    
    if (!CONFIG_MANAGER->isVietnameseStories() || MJDEFAULT->getBoolForKey("debugkey_all_lessons",false)){
        auto preview = MSLessonPreview::createScene(lesson);
	
		if (!ms::LessonManager::getInstance().isPhonicLesson(lesson)) {

			auto a_id = ms::LessonManager::getInstance().getActivityShouldLearn_4notPhonics(lesson.category_id, lesson.lesson_id);

			auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);

			if (!activities.empty()) {

				if (a_id == activities.front().activity_id || a_id == -1) {
					mj::model::ActInfo temp;
					temp.activity_id = -1;
					temp.lesson_id = lesson.lesson_id;

					auto popup = MSActivityEnd::createPopup(activities.front(), false);
					popup->setDelegate(this);
					popup->setName("popup_start");
                    preview->addChild(popup, 1000);
				}
			}
		}
		else
		{
			auto l_id = ms::LessonManager::getInstance().getLessonShouldLearn(lesson.category_id);

			if (l_id == lesson.lesson_id) {

				auto a_id = ms::LessonManager::getInstance().getActivityShouldLearn(lesson.category_id);
                std::string lessonLearned = MJDEFAULT->getStringForKey(StringUtils::format(key_number_activity_of_grade_phonic, PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_phonic)));

                if ((a_id == 0 || a_id == -1) && lessonLearned.length() == 0) {
					mj::model::ActInfo temp;
					temp.activity_id = -1;
					temp.lesson_id = lesson.lesson_id;

					auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
					auto popup = MSActivityEnd::createPopup(activities[0], false);
					popup->setDelegate(this);
					popup->setName("popup_start");
                    preview->addChild(popup, 1000);
				}
			}
		}
        if (preview)
        {
            preview->setContentSize(this->getContentSize());
            //ui::Helper::doLayout(preview);
            preview->setName("lesson_preview");
            Director::getInstance()->pushScene(preview);
            preview->setOpacity(0);
            preview->runAction(FadeIn::create(0.25));
            MJDEFAULT->setIntegerForKey(key_time_start_learn_lesson, int(utils::getTimeInMilliseconds() / 1000));
        }

    }else{
        auto a_id = ms::LessonManager::getInstance().getActivityShouldLearn(lesson.category_id);
        auto next_activity = StoryDataModel::getInstance()->getActivityById(a_id);
        if (a_id == 0 || a_id == -1 || next_activity.lesson_id != lesson.lesson_id) {
            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
            auto popup = MSActivityEnd::createPopup(activities[0], false);
            popup->setDelegate(this);
            popup->setName("popup_start");
            Director::getInstance()->getRunningScene()->addChild(popup, 1000);
        }else{
            mj::model::ActInfo temp;
            auto _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
            auto lesson_should_learn = ms::LessonManager::getInstance().getLessonShouldLearn(lesson.category_id);
            if (lesson_should_learn != lesson.lesson_id)
            {
                next_activity.activity_id = -1;
            }
            else
            {
                next_activity = ms::LessonManager::getInstance().getActivityById(ms::LessonManager::getInstance().getActivityShouldLearn(lesson.category_id));
                
                if (next_activity.activity_id == -1)
                {
                    next_activity = _activities[0];
                }
            }
            
            auto saved_state = mj::model::ActInfo::LEARNED;
            
            auto index_in_lesson = 0;
            
            for (auto i = 0; i < _activities.size(); ++i) {
                if (lesson.state == mj::model::LessonInfo::State::LEARNED) {
                    _activities[i].state = mj::model::ActInfo::State::LEARNED;
                } else if (lesson.state == mj::model::LessonInfo::State::READY) {
                    if (_activities[i].activity_id == next_activity.activity_id) {
                        _activities[i].state = mj::model::ActInfo::State::READY;
                        saved_state = mj::model::ActInfo::State::LOCKED;
                        index_in_lesson = i;
                    } else {
                        _activities[i].state = saved_state;
                    }
                }
            }
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
            
            auto popup = MSActivityEnd::createPopup(temp, false);
            popup->setDelegate(this);
            popup->setName("popup_start");
            Director::getInstance()->getRunningScene()->addChild(popup, 1000);
        }
    }
}

void MSTabLessons::onContinueLearn(mj::model::ActInfo info) {

    if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        tmp->removeFromParent();
    }

    info.startFrom = mj::model::ActInfo::START_FROM::START_LESOON;
    if (auto wrapper = MSActivityWrapper::createWithInfo(info)) {
        wrapper->setName("wrapper");
        Director::getInstance()->getRunningScene()->addChild(wrapper);
        wrapper->setVisible(false);
    }
 
};

void MSTabLessons::onBackToLesson() {
    Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
}

void MSTabLessons::onBack(Ref * sender){
    mj::helper::SetFpsByNumberFrames(20);
    mj::helper::playButtonFXClose();
    runAnimationDisappear();
}

void MSTabLessons::onSelectGradeBox() {
}

void MSTabLessons::onDeselectGradeBox(int grade_id) {

}

void MSTabLessons::reloadData() {
    CCLOG("reload tab lessons");
    _ui_source.clear();
    _ui_source.resize(_lessons.size(), nullptr);
                      
    _collection->reloadData();
}

void MSTabLessons::onReceivedEventGradeChanged(int grade_id) 
{
    _lessons = ms::LessonManager::getInstance().getAllLessonsByGrade(grade_id);

	if (m_scope == ms::GradeManager::Scope::LESSONS_PHONIC)
	{
		s_gradeIDsCache.second = grade_id;
	}
	else
	{
		s_gradeIDsCache.first = grade_id;
	}

	m_gradeID = grade_id;
    
    auto lastLessonId = ms::LessonManager::getInstance().getLessonShouldLearn(m_gradeID);
    auto it = std::find_if(_lessons.rbegin(), _lessons.rend(), [this, lastLessonId](mj::model::LessonInfo l) {
        // nếu là MS và Lessons-Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicCategory(m_gradeID)) {
            return l.lesson_id == lastLessonId;
        // các trường hợp khác
        } else {
            return l.state == mj::model::LessonInfo::State::UNLOCKED || l.state == mj::model::LessonInfo::State::READY;
        }
    });
    
    if (it != _lessons.rend())
    {
        _saved_current_row = (int)std::distance(it, _lessons.rend()) - 1;
        if (_saved_current_row == _lessons.size() - 1){
            if ((_lessons[_lessons.size()-1]).is_learned){
                _saved_current_row = (int)_lessons.size();
            }
        }
    }else{
        _saved_current_row = (int)_lessons.size();
    }
    
    int countLessonLearned = ms::LessonManager::getInstance().isPhonicCategory(m_gradeID) ? _saved_current_row : countCompletedLessons();
    auto rich = utils::findChild<ui::RichText *>(this, "completed");
    rich->removeElement(1);
    rich->pushBackElement(ui::RichElementText::create(1, Color3B::BLACK, 255, StringUtils::format("  %d/%d %s", countLessonLearned, (int)_lessons.size(),
		LANGUAGE_MANAGER->getDisplayTextByKey("lessons.completed").c_str()), 
		(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Regular.ttf", 20));

    ui::Helper::doLayout(this);
    
    reloadData();
}

void MSTabLessons::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    auto line = utils::findChild(this, "line");
    auto navi = utils::findChild(this, "navi");
    if (navi) {
        navi->setContentSize(Size(size.width, navi->getContentSize().height));
        auto rich = navi->getChildByName("completed");
        if (rich) {
            rich->setPosition(Point(size.width / 2, navi->getContentSize().height / 2));
            rich->setColor(Color3B::RED/*(39, 175, 229)*/);
//            auto back = navi->getChildByName("btnBack");
//            rich->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
//            rich->setPosition(Point(/*back->getPosition().x + back->getContentSize().width/2 + */25, navi->getContentSize().height/2));
        }
    }

    if (line) {
        line->setContentSize(Size(1200, line->getContentSize().height));
        auto rich = navi->getChildByName("completed");
        if (rich) {
            auto p = rich->getBoundingBox().origin;
            p = rich->getParent()->convertToWorldSpace(p);
            p = line->getParent()->convertToNodeSpace(p);
            line->setContentSize(Size(p.x - line->getPositionX(), line->getContentSize().height));
        }
    }
    
    if (auto gradebox = utils::findChild(this, "gradebox")) {
        gradebox->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        gradebox->setPosition(Point(navi->getBoundingBox().size.width * 0.98, gradebox->getPositionY()));
    }

    auto collection_layout = utils::findChild(this, "collection");
    if (collection_layout) {
        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));
    }
    
    if (_collection) {
        _collection->setContentSize(Size(size.width - 100, _collection->getContentSize().height));

		// because when users exit reading story scene, the scene manager will pop the current scene
		// and THE TOP SCENE ON STACK (HomeScene) will call onEnter function for all children
		// actually, have some codes, we only want to excute once!
		// so we need to use the flag m_isReloadDataCalled 

		if (!m_isReloadDataCalledFromOnEnterFucntion)
		{
			m_isReloadDataCalledFromOnEnterFucntion = true;
            reloadData();
		}
    }
}

void MSTabLessons::onTryToLearnLockedLessons(mj::model::LessonInfo lesson) {
    _collection->scrollTo(percentScrollWhenReload());
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_locked_lesson);
}

void MSTabLessons::removeLessonReviewLayout()
{
    if(auto preview = this->getChildByName("lesson_preview"))
    {
        preview->removeFromParent();
    }
}

void MSTabLessons::onExitTransitionDidStart()
{
    _callbackAutoPlay = nullptr;
    MS_LOGGER.logEvent("exit_lesson", {
        {"lesson_type",cocos2d::Value(m_scope == ms::GradeManager::Scope::LESSONS ? "STORIES" : "PHONICS")}
    });
}

#define mstablessons_tag_open_lesson_from_qr    2051

void MSTabLessons::loadCustomGradeWithID(int gradeId) {
 
    if(gradeId<=0) {
        return;
    }
    
    reloadLessons(gradeId);
    reloadData();
}
    
void MSTabLessons::openLessonForce(int gradeId, int lessonId) {
    
    if (_lessons.size() == 0)
    {
        return;
    }
    
    m_lesson_order_from_qr = -1;
    
    // nếu là MS và Comprehension
    if (!CONFIG_MANAGER->isVietnameseStories() && MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false) == false)
    {
        for (int i=0; i<_lessons.size(); i++)
        {
            auto elem = _lessons.at(i);
            
            if (elem.lesson_id == lessonId)
            {
                m_lesson_order_from_qr = i;
            }
        }
        
        if (m_lesson_order_from_qr >= 0)
        {
            this->openLessonFromQR();
        }
        return;
    }
    
    // Nếu là VM hoặc MS Phonics
    if (_lessons.size() > 1 && _lessons.at(1).state == mj::model::LessonInfo::State::LOCKED)    /* trường hợp chưa học bài học nào --> mở bài đầu tiên */
    {
        for (int i=0; i<_lessons.size(); i++)
        {
            auto elem = _lessons.at(i);
            
            if (elem.lesson_id == lessonId)
            {
                m_lesson_order_from_qr = 0;
                
                // nếu là bài đầu tiên thì mở luôn
                if (i == 0)
                {
                    this->openLessonFromQR();
                }
                // nếu là thứ 2 trở đi thì hiện popup trước
                else
                {
                    auto alert = StoryAlert::showAlert(__String::createWithFormat(LANGUAGE_MANAGER->getDisplayTextByKey("key.open.lesson.from.qr.notyet").c_str(), i+1)->getCString(), LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button"), " ");
                    alert->setTag(mstablessons_tag_open_lesson_from_qr);
                    alert->setDelegate(this);
                }
                
                break;
            }
        }
        
        return;
    }
    else                                                                                /* trường hợp thông thường */
    {
        int lastUnlockedLessonId = ms::LessonManager::getInstance().getLastLessonIdOfCategory(gradeId);
        bool isLastUnlocked = false;
        bool isTarget = false;
        
        for (int i=0; i<_lessons.size(); i++)
        {
            auto elem = _lessons.at(i);
            
            if (elem.lesson_id == lastUnlockedLessonId)
            {
                isLastUnlocked = true;
            }
            
            if (elem.lesson_id == lessonId)
            {
                isTarget = true;
            }
            
            if (isLastUnlocked && isTarget == false)
            {
                // mở bài vượt quá bài cuối cùng được mở khoá
                for (int j=i+1; j<_lessons.size(); j++)
                {
                    auto zzz = _lessons.at(j);
                    
                    if (zzz.lesson_id == lessonId)
                    {
                        m_lesson_order_from_qr = i;
                        
                        // mới hoàn thiện bài i, cần làm tiếp bài i+1, để mở khoá bài j+1
                        auto alert = StoryAlert::showAlert(__String::createWithFormat(LANGUAGE_MANAGER->getDisplayTextByKey("key.open.lesson.from.qr.overles").c_str(), i, i+1, j+1)->getCString(), LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button"), " ");
                        alert->setTag(mstablessons_tag_open_lesson_from_qr);
                        alert->setDelegate(this);
                    }
                }
                
                return;
            }
            else if (isTarget)
            {
                m_lesson_order_from_qr = i;
                break;
            }
        }
    }
    
    if (m_lesson_order_from_qr >= 0)
    {
        this->openLessonFromQR();
    }
    else
    {
        // khong ton tai bai nao
    }
}

void MSTabLessons::openLessonFromQR()
{
    if (m_lesson_order_from_qr < 0)
    {
        return;
    }
    
    /*  CHÚ Ý
     *  - không dùng được hàm cellForPosition(...) vì item được tạo trong đó sẽ được gọi trong update(dt) mà lúc mình gọi cellForPosition() thì có thể chưa addChild cho nó --> dùng _aiSpeakingLessons thay cho _ui_source
     *  - _ui_source khớp với _aiSpeakingLessons nên thứ tự LESSON_ORDER của _aiSpeakingLessons cũng chính là thứ tự trong _ui_source
     *  - _ui_source hiển thị item từ trên xuống dưới, từ trái sang phải
     */
    int TABLE_SIZE      = this->numberOfCells();
    int NUMB_OF_COLS    = this->numberOfColumns();
    int NUMB_OF_ROWS    = (TABLE_SIZE % NUMB_OF_COLS > 0 ? 1 : 0) + (TABLE_SIZE / NUMB_OF_COLS);
    int curRow          = (m_lesson_order_from_qr % NUMB_OF_COLS > 0 ? 1 : 0) + (m_lesson_order_from_qr / NUMB_OF_COLS);
    float percent       = curRow * (100.f / NUMB_OF_ROWS);
    
    _collection->scrollTo(percent);
    
    // không biết khi nào thì thằng cò hó LessonItem được tạo nên phải schedule để check
    this->schedule([this](float)
    {
        if (auto item = (MSCollectionLessonItem *)_ui_source.at(m_lesson_order_from_qr))
        {
            item->openLessonItemForce();
            this->unschedule("tab_lessons_schedule_open_lesson");
        }
    }, 1.0/60, 200, 0, "tab_lessons_schedule_open_lesson"); // update sau mỗi 1.0/60 giây, lặp 200 lần, delay time = 0
}

void MSTabLessons::onAlertOK(cocos2d::Ref * sender)
{
    auto alert = (StoryAlert *)sender;
    if (alert->getTag() == mstablessons_tag_open_lesson_from_qr)
    {
        this->openLessonFromQR();
    }
}

void MSTabLessons::onAlertCancel(cocos2d::Ref * sender)
{
    // do nothing
}

//// < Dinh Duc - Lesson Search Bar
/*
void MSTabAudiobooks::onSearch(cocos2d::Ref *sender)
{
    mj::helper::playButtonFX();
    auto view = MSSearchView_Audiobooks::createView();
    view->setContentSize(this->getContentSize());
    view->setName("MSSearchView");
    ui::Helper::doLayout(view);
    this->addChild(view);
    view->setOpacity(0);
    view->runAction(FadeIn::create(0.25));
}*/
void  MSTabLessons::onSelectSearch(cocos2d::Ref * sender)
{
    //auto search_view = MSSearchView_Lessons::createView();
    //search_view->setContentSize(this->getContentSize());
    //search_view->setName("search_view");
    auto text_field = ui::TextField::create("Text Field", "Arial", 30);
    text_field->setAnchorPoint(m_search_button->getAnchorPoint());
    text_field->setPosition(m_search_button->getPosition() + Vec2(m_search_button->getContentSize().width, 0));
    //ui::Helper::doLayout(search_view);
    //utils::findChild<>(this,)->addChild(text_field, 1);
    m_search_button->getParent()->addChild(text_field, 2);

    text_field->setTextVerticalAlignment(TextVAlignment::CENTER);
    text_field->setTextHorizontalAlignment(TextHAlignment::LEFT);
    text_field->setPlaceHolderColor(Color4B(Color3B::WHITE, 0xaa));
    text_field->setTextColor(Color4B(Color3B(0x34,0xC5,0xED)));
    text_field->setCursorEnabled(true);
    text_field->setCursorChar('|');
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    registerPressCtrlEventForTextField(text_field, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    text_field->addEventListener([=](Ref* sender, ui::TextField::EventType event) {

        if (event == ui::TextField::EventType::INSERT_TEXT || event == ui::TextField::EventType::DELETE_BACKWARD) {
            auto query = text_field->getString();
            _lessons = StoryDataModel::getInstance()->searchLessonByLanguage(StoryLanguageManager::getCurrentLangId(), query);
            
            /*_ui_source.clear();
            _collection->removeFromParentAndCleanup(true);
            
            _ui_source.resize(_aiSpeakingLessons.size(), nullptr);

            _collection = MSComplexCollection::createCollection(this);
            _collection->setDelegate(this);
            utils::findChild(this, "collection")->addChild(_collection);

            ui::Helper::doLayout(this);
            _collection->reloadData();*/

            reloadData();
        }
        });
}

void MSTabLessons::deleteSidebarBlack()
{
   /* if (auto clipperBlack = utils::findChild(Director::getInstance()->getRunningScene(), "clipper_black"))
    {
        clipperBlack->removeFromParent();
    }*/
}
void MSTabLessons::changeDisplayLanguage()
{
    
}
//// >
