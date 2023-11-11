//
//  StoryPageVC.cpp
//  ViettelPost
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryPageVC.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "JsonDump.h"
#include "TruyenTranhPage.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryRatePopUp.h"
#include "StoryEndingPage.h"
#include "MJ_AnimationCache.h"
#include "StoryDataModel.h"
#include "StoryTheEndPage.h"
#include "GAFWrapper.h"
#include "MJAPIRateApp.h"
#include "MJAPISendDownload.h"
#include "PoolManager.h"
#include "StoryInfoCover.h"
#include "ActionFlipPage.h"
#include "MJFirebaseListener.h"
#include "manager/RatingManager.h"
#include "manager/CleverTapManager.h"
#include "RemoteImageView.h"
#include "APProfileManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../proj.win32/Win32DeviceInfo.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MacCppBridge.h"
#endif
#include "MSBookPage.h"

#include "MJBaseActivity.h"
#include "MSActivityWrapper.h"
#include "MJHelper.h"
#include "APDatabase.h"
#include "HelperManager.h"
#include "MSScreenRecorder.h"
#include "F2PManager.h"
#include "MSCacheManager.h"

INITIALIZE_READER_INSTANCE(StoryPageVCReader);

StoryPageVC* StoryPageVC::_storyVC;
StoryPageVC* StoryPageVC::getInstance()
{
    return _storyVC;
}

#define ACTION_PAGE_TURN_TAG 7392873

USING_NS_CC;
using namespace ui;

StoryPageVC::StoryPageVC() {

    _delegate = nullptr;
}

StoryPageVC::~StoryPageVC(){

    pushStoryTrackerDataToServer();
}

Mat4 rect_transform;

void StoryPageVC::transformPageByAngle(Node * n, double angle) {
    if (!n) return;
    auto transform = Mat4::IDENTITY;
    transform.rotateY(angle);

    auto scale_x = (M_PI / 3 + angle) / (M_PI / 3);
    transform.scale(scale_x, 1, 1);
    
    n->setNodeToParentTransform(transform);
    auto bb = n->getBoundingBox();
}

float StoryPageVC::transformPageTurn(Node * n, double distance) {
    auto ratio = (distance - 50) / (Director::getInstance()->getVisibleSize().width - 400);
    if (ratio > 1) {
        ratio = 1;
    }
    if (ratio < 0 && distance < 50) {
        ratio = 0;
    }
    auto angle = - M_PI / 3 * ratio;
    
    transformPageByAngle(n, angle);
    return angle;
}

void StoryPageVC::initPageTurnLayout() {
    auto touch_layout = ui::Layout::create();
    touch_layout->setTouchEnabled(true);
    touch_layout->setContentSize(Director::getInstance()->getVisibleSize());
    this->addChild(touch_layout, 50);
    touch_layout->setSwallowTouches(false);
    touch_layout->setPropagateTouchEvents(true);
    touch_layout->setName("page_turn_touch_layout");
    touch_layout->addTouchEventListener([this](Ref * sender, ui::Widget::TouchEventType event) {
        auto wg = reinterpret_cast<ui::Widget *>(sender);
        if (event == ui::Widget::TouchEventType::MOVED) {
            //            CCLOG("touching...");
            auto distance = wg->getTouchMovePosition().x - wg->getTouchBeganPosition().x;
            if (distance < -50) {
                auto imp = dynamic_cast<MSPageImp *>(_current_page);
                if (imp->hasNextBoxText()) {
                    return;
                }
                if (_is_moving_prev) {
                    return;
                } else {
                    _is_moving_next = true;
                    if (_current_page) {
                        _current_page_angle = transformPageTurn(_current_page, fabs(distance));
                        transformPageTurn(_grad_next, fabs(distance));
                    }
                }
            } else if (distance > 50) {
                auto imp = dynamic_cast<MSPageImp *>(_current_page);
                if (imp->hasPrevBoxText()) {
                    return;
                }
                if (_is_moving_next) {
                    return;
                } else {
                    _is_moving_prev = true;
                    if (distance <= Director::getInstance()->getVisibleSize().width - 400) {
                        distance = Director::getInstance()->getVisibleSize().width - 400 - distance;
                        if (_prev_page) {
                            _prev_page_angle = transformPageTurn(_prev_page, fabs(distance));
                            transformPageTurn(_grad_prev, fabs(distance));
                        }
                    }
                }
            }
        } else if (event == ui::Widget::TouchEventType::ENDED || event == ui::Widget::TouchEventType::CANCELED) {
            if (!(_is_moving_prev || _is_moving_next)) {
                return;
            }
            //            _is_moving_prev = false;
            //            _is_moving_next = false;
            if (_current_page) {
                auto action = Sequence::create(ActionFloat::create(0.5, _current_page_angle, 0, [this](float v) {
                    transformPageByAngle(_current_page, v);
                    _current_page_angle = v;
                    transformPageByAngle(_grad_next, v);
                }), CallFunc::create([this] {
                    _is_moving_next = false;
                    _is_moving_prev = false;
                }), nullptr);
                action->setTag(ACTION_PAGE_TURN_TAG);
                _current_page->runAction(action);
            }
            if (_prev_page) {
                auto action = Sequence::create(ActionFloat::create(0.5, _prev_page_angle, - M_PI / 3, [this](float v) {
                    transformPageByAngle(_prev_page, v);
                    _prev_page_angle = v;
                    transformPageByAngle(_grad_prev, v);
                }), CallFunc::create([this] {
                    _is_moving_prev = false;
                    _is_moving_next = false;
                }), nullptr);
                action->setTag(ACTION_PAGE_TURN_TAG);
                _prev_page->runAction(action);
            }
        }
    });
}

void StoryPageVC::didLoadFromCSB() {
    _storyVC = this;
    this->setName("story_pageVC");

    initPageTurnLayout();
    _timeStartReadStory =  MJDEFAULT->getIntegerForKey("key_time_start_learn_lesson_story");
    MJDEFAULT->deleteValueForKey(key_sum_time_user_exit_app);
    MJDEFAULT->deleteValueForKey(key_time_user_exit_app);

    auto back_layout = ui::Layout::create();
    back_layout->setContentSize(Director::getInstance()->getVisibleSize());
    back_layout->setBackGroundColor(Color3B::WHITE);
    back_layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->addChild(back_layout, -1);
    
    layout_block_touch = ui::Layout::create();
    layout_block_touch->setContentSize(Director::getInstance()->getVisibleSize());
    layout_block_touch->setTouchEnabled(false);
    layout_block_touch->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    layout_block_touch->setPosition(Point::ZERO);
    this->addChild(layout_block_touch, INT_MAX-1);
    
    if(!CONFIG_MANAGER->isVietnameseStories()){
        auto logo = ui::ImageView::create("mjstory/new_ui/image_appdrawer_stories_logo_new.png");
        logo->setAnchorPoint(Point::ANCHOR_MIDDLE);
        logo->setPosition(Director::getInstance()->getVisibleSize() / 2);
        logo->setOpacity(0x55);
        back_layout->addChild(logo);
    }
    
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    createCoverPage();
    btclose = this->getChildByName<Button *>("btclose");
    btclose->setLocalZOrder(100000);
    if (XHSLib->getSizeScreen().width*1.0/XHSLib->getSizeScreen().height >= 2.1) {
        btclose->setPosition(btclose->getPosition() + Vec2(-50, 0));
    }
    classes_name = "storypagevc";
    isloadallpage = false;
    
    auto path_check = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.plist", _storyInfo.storyid, XHSLib->lang_id, 1));
    if (FileUtils::getInstance()->isFileExist(path_check)) {
        is_story_plist = true;
    }
    if (isloadallpage){
        if (is_story_plist) {
            for (int i = 0; i < XHSLib->pageNumAll; i++) {
                cocos2d::ui::Layout *page;
                page = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, i+1);
                this->addChild(page);
                page->setVisible(false);
                page->removeFromParent();
            }
        }
    }
    if (is_sample_read){
        pagecheck = 0;
        check_free_can_read = true;
        CCLOG("per_free = %f", FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed());
    }else {
        if (is_activity_play) {
            check_free_can_read = checkLessonFreeCanRead();
            if (check_free_can_read) {
                pagecheck = 0;
            }else pagecheck = 3;
        }else {
            check_free_can_read = checkFreeCanRead();
            if (check_free_can_read) {
                auto sotrangdadoc = FREE_STORY_MANAGER->getStoryNumpageRecentRead(XHSLib->_id);
                auto percent_can_read = FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed(ACTION_STORY);
                auto so_trang_con_lai = percent_can_read/100.0f*XHSLib->pageNumAll;
                int bonus_trang = 0;
                if (so_trang_con_lai - (int)so_trang_con_lai > 0) {
                    bonus_trang = 1;
                }
                pagecheck = (int) so_trang_con_lai + bonus_trang + sotrangdadoc;
                if (pagecheck <= 3) {
                    pagecheck = 3;
                }else{
                    if (pagecheck >= XHSLib->pageNumAll) {
                        pagecheck = 0;
                    }
                }
            }else {
                pagecheck = 3;
            }
        }
    }
    
    
    XHSLib->enable_keyboard = true;
    XHSLib->show_popup_subscribe = false;
}
#pragma mark - onEnter
void StoryPageVC::onEnter() {
    mj::helper::SetFpsByNumberFrames(10);
	CCLOG("XHSLib->story_play_type: %d ", XHSLib->story_play_type);
   
	MJDEFAULT->setStringForKey("cl_read_recommended_story", "null");
	//init for tracking at index 0

    m_currentTime = cocos2d::utils::getTimeInMilliseconds();

    int pageID = 0;

    if(pageID >= 0 && pageID < m_StoryTracker.m_storyInfors.size() && !m_StoryTracker.m_storyInfors.empty())
    {
        m_StoryTracker.m_storyInfors.at(pageID).m_repeatCount += 1;
    }

    Scene::onEnter();
    
    _time_start_reading = int(utils::getTimeInMilliseconds() / 1000);
    _page_read = 0;
    
    MJDEFAULT->setBoolForKey(key_current_guide_showed, false);
    didLoadFromCSB();
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsScene("story_read_scene", "story_read_page");
    
    _map_event_analytics.clear();

    if (_map_event_analytics.count("story_name") > 0) {
        _map_event_analytics["story_name"] = StringUtils::format("%d_%s", _storyInfo._id, _storyInfo.name.c_str()) ;
    }

    if (_map_event_analytics.count("page_index") > 0) {
        _map_event_analytics["page_index"] = StringUtils::format("%d/%d", XHSLib->pageIndex, _storyInfo.total_page);
    }

    MS_LOGGER.logEvent("StoryPageVC_onEnter",
    {
                {"story_name", cocos2d::Value(_map_event_analytics["story_name"].asString().c_str())},
                {"page_index", cocos2d::Value(_map_event_analytics["page_index"].asString().c_str())},
                {"story_info_id", cocos2d::Value(_storyInfo._id)},
                {"story_info_name", cocos2d::Value(_storyInfo.name.c_str())},
                {"story_info_total_page", cocos2d::Value(_storyInfo.total_page)},
     });

    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_read_story, "story_id", _storyInfo._id);
    
    auto list_category = StoryDataModel::getInstance()->getListCategoryWithStoryId(XHSLib->_id);
    for (int i = 0; i < (int)list_category.size(); i++) {
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_read_category+StringUtils::toString(list_category[i]), "", "");
    }

    cocos2d::ui::Layout *page = nullptr;

    if (is_story_plist){
        page = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex);
    }else {
        page = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex);
    }
    
    _current_page = page;
    auto imp = dynamic_cast<MSPageImp*>(_current_page);
    if (imp) {
        imp->enableInteraction();
        imp->setNeedRefreshed(true);
        _current_page->scheduleOnce([imp](float dt) {
            if (imp) {

                MS_LOGGER.logEvent("call_startPlaying_01", {});

                imp->startPlaying();
            }
        }, 0.25f, "delay_play_page");
    }
    this->addChild(page, 3);
    page->setTag(100+XHSLib->pageIndex);
    page->setName(StringUtils::format("page_%d", 100 + XHSLib->pageIndex));
    
    _prev_page = nullptr;
    _grad_next = ui::Layout::create();
    _grad_next->setContentSize(Director::getInstance()->getVisibleSize());
    auto grad = LayerGradient::create(Color4B(mj::helper::colorFromHex(0x0), 0xdd), Color4B(0, 0, 0, 0), Vec2(1, 0));
    if (grad)
    {
        grad->setContentSize(Size(300, Director::getInstance()->getVisibleSize().height));
        //    _grad_next->setName("pageturn_gradient");
        grad->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, 0));
        _grad_next->addChild(grad);
        this->addChild(_grad_next, 3);

        _grad_prev = ui::Layout::create();
        _grad_prev->setContentSize(Director::getInstance()->getVisibleSize());
        grad = LayerGradient::create(Color4B(mj::helper::colorFromHex(0x0), 0xdd), Color4B(0, 0, 0, 0), Vec2(1, 0));
        grad->setContentSize(Size(300, Director::getInstance()->getVisibleSize().height));
        //    _grad_prev->setName("pageturn_gradient");
        grad->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, 0));
        _grad_prev->addChild(grad);
    }

    if (_grad_prev)
    {
        this->addChild(_grad_prev, 4);
        transformPageByAngle(_grad_prev, -M_PI / 3);
    }

    if(auto currentPage = dynamic_cast<MSPageImp*>(_current_page))
    {
        int pageID = currentPage->getPageIndex() - 1;

        if(pageID >= 0 && pageID < m_StoryTracker.m_storyInfors.size() && !m_StoryTracker.m_storyInfors.empty())
        {
            m_StoryTracker.m_storyInfors.at(pageID).m_standardTime = currentPage->getStandardTime();
        }
    }

    if (MJDEFAULT->getBoolForKey("debugkey_skip_story", false))
    {
        Director::getInstance()->getRunningScene()->stopAllActions();
        Director::getInstance()->getRunningScene()->unscheduleAllCallbacks();
        Director::getInstance()->getRunningScene()->unscheduleAllSelectors();
        AUDIO_ENGINE->stopAllAudio();
        AUDIO_ENGINE->stopAllEffects();
        AUDIO_ENGINE->stopAllSound();
        cocos2d::experimental::AudioEngine::stopAll();
        _checkStartGame = true;
        XHSLib->pageIndex = XHSLib->pageNumAll;
        return;
    }
    MS_LOGGER.logEvent("COMPLETE_StoryPageVC_onEnter",{});
}

void StoryPageVC::onEnterTransitionDidFinish() {

	MS_LOGGER.logEvent("StoryPageVC_onEnterTransitionDidFinish", {});

	Scene::onEnterTransitionDidFinish();

	auto replace_page = EventListenerCustom::create("mjstory.replace_page", [=](EventCustom* event_custom) {
		
        int* edata = (int*)event_custom->getUserData();

        if (edata != nullptr) {
            int data = *edata;
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                this->onReplacePage(data);
            });
        }
		
        delete edata;
	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(replace_page, this);

	cocos2d::ui::Layout* page_2 = nullptr;

	if (is_story_plist) {
		page_2 = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex + 1);
	}
	else {
		page_2 = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex + 1);
	}

	if (page_2) {
		this->addChild(page_2, 1);
		_next_page = page_2;
	}
}

#pragma mark - onExit
void StoryPageVC::onExitTransitionDidStart(){
    CRASH_AUTO_LOG;
    if(!m_disableCancelRecordAction)
    {
         ms::recorder::cancelRecord();
    }
   
    Scene::onExitTransitionDidStart();

    releaseRequest();

    AUDIO_ENGINE->stopAllEffects();
}

void StoryPageVC::onExit()
{
    CRASH_AUTO_LOG;
    mj::helper::purgeTextureData();
    Scene::onExit();
    mj::helper::SetFpsByNumberFrames(6);
    ms::EventLogger::getInstance().logEvent("StoryPageVC::onExit",{});

    if(this->getEventDispatcher()->hasEventListener("mjstory.replace_page")) {
        
        this->getEventDispatcher()->removeCustomEventListeners("mjstory.replace_page");
    }
    
}

#pragma mark - REPLACE_PAGE
void StoryPageVC::onReplacePage(int index_replace) {

    ms::EventLogger::getInstance().logEvent("onReplacePage", {{"index_replace", cocos2d::Value(index_replace) }});
    
    if (_current_page) _current_page->stopActionByTag(ACTION_PAGE_TURN_TAG);
    if (_prev_page) _prev_page->stopActionByTag(ACTION_PAGE_TURN_TAG);
    if (_next_page) _next_page->stopActionByTag(ACTION_PAGE_TURN_TAG);

    /*calculate total time on pages for trackinga
      old page*/
    auto const k_totalTimeOnPage =  (cocos2d::utils::getTimeInMilliseconds() - m_currentTime)/1000.0;

    m_currentTime = cocos2d::utils::getTimeInMilliseconds();

    if(auto currentPage = dynamic_cast<MSPageImp*>(_current_page))
    {
        int pageID = currentPage->getPageIndex() - 1;

        if(pageID >= 0 && pageID < m_StoryTracker.m_storyInfors.size() && !m_StoryTracker.m_storyInfors.empty())
        {
            m_StoryTracker.m_storyInfors.at(pageID).m_totalTimeOnPage += k_totalTimeOnPage;
            m_StoryTracker.m_storyInfors.at(pageID).m_imgClickedCount += currentPage->getIMGClickedNum();
            m_StoryTracker.m_storyInfors.at(pageID).m_gafClickedCount += currentPage->getGAFClickedNum();
        }
    }

    HSAudioEngine::getInstance()->stopAllEffects();

    if (XHSLib->pageIndex >= 2) {
        MJDEFAULT->setBoolForKey(key_current_guide_showed, true);
        auto count = MJDEFAULT->getIntegerForKey(key_count_guide_story, 0);
        MJDEFAULT->setIntegerForKey(key_count_guide_story, count + 1);
    }
    
    _page_read = MAX(_page_read, XHSLib->pageIndex - 1);
    CCLOG("XHSLib->pageIndex = %d", XHSLib->pageIndex);
    
    if (index_replace == 2 && XHSLib->pageIndex == 0) {
        return;
    }
    
    auto index_endpage = XHSLib->pageNumAll;
    if (CONFIG_MANAGER->isTestGame()) {
        _next_page->removeFromParent();
        index_endpage = 1;
    }
    
    if (XHSLib->pageIndex > index_endpage) {//END_PAGE_ XHSLib->pageNumAll
        if (_next_page && _next_page->getParent()){
            _next_page->removeFromParent();
        }
        displayEndingPage();
        if (!CONFIG_MANAGER->isAppActive() && !is_activity_play && !is_sample_read) {
            FREE_STORY_MANAGER->setPercentActionFreePlayed(100);
        }
        return;
    }
    
    //check story can read
    if (is_activity_play) {
        if (!checkLessonPageStoryCanRead(index_replace)) return;
    }else {
        if (!checkStoryCanRead(index_replace)) return;
    }
    
    _map_event_analytics["page_index"] = StringUtils::format("%d/%d", XHSLib->pageIndex, _storyInfo.total_page);
    
    ActionInterval *move1 = nullptr, *move2 = nullptr;
    auto tag_current = 0;
    switch (index_replace) {
        case 1:
            tag_current = XHSLib->pageIndex -1;
            break;

        case 2:
            tag_current = XHSLib->pageIndex + 1;
            break;

        case 3:
            tag_current = XHSLib->pageIndex;
            break;
    }
    cocos2d::ui::Layout *page_current = nullptr;
    cocos2d::ui::Layout *page_new = nullptr;

    if (is_story_plist){
        if (index_replace == 1) {
            page_new = dynamic_cast<ui::Layout *>(_next_page);
        } else if (index_replace == 2) {
            page_new = dynamic_cast<ui::Layout *>(_prev_page);
        } else {
            page_new = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex);
        }
    }else {
        if (index_replace == 1) {
            page_new = dynamic_cast<ui::Layout *>(_next_page);
        } else if (index_replace == 2) {
            page_new = dynamic_cast<ui::Layout *>(_prev_page);
        } else {
            page_new = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex);
        }
    }
    
    if (_current_page)
    {
        page_current = dynamic_cast<ui::Layout*>(_current_page);
    }
    
    if (index_replace == 1) {
        if (_prev_page) {
            _prev_page->removeFromParent();
            _prev_page = nullptr;
        }
        if (_current_page && page_new)
        {
            _prev_page = _current_page;
            _prev_page->setLocalZOrder(4);
            _current_page = page_new;
            _current_page->setLocalZOrder(3);
            _next_page = nullptr;
        }
    } else if (index_replace == 2) {
        if (_next_page) {
            _next_page->removeFromParent();
            _next_page = nullptr;
        }
        if (_current_page && page_new)
        {
            _next_page = _current_page;
            _next_page->setLocalZOrder(1);
            _current_page = page_new;
            _current_page->setLocalZOrder(3);
            _prev_page = nullptr;
        }
    } else {
        if (page_new)
        {
            _current_page = page_new;
            this->addChild(page_new, 3);
        }
    }
    
    auto imp = dynamic_cast<MSPageImp *>(_current_page);
    if (imp) {
        imp->setNeedRefreshed(true);
        _current_page->scheduleOnce([imp](float dt) {
            imp->enableInteraction();
        }, 0.25, "delay_enable_interaction_page");
    }

    /*  for tracking data
        new page*/
    if(auto currentPage = dynamic_cast<MSPageImp*>(_current_page))
    {
        int pageID = currentPage->getPageIndex() - 1;

        if(pageID >= 0 && pageID < m_StoryTracker.m_storyInfors.size() && !m_StoryTracker.m_storyInfors.empty())
        {
            m_StoryTracker.m_storyInfors.at(pageID).m_repeatCount  += 1;
            m_StoryTracker.m_storyInfors.at(pageID).m_standardTime += currentPage->getStandardTime();
        }
    }

    auto overlay = Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setTouchEnabled(true);
    overlay->setName("overlay");
    this->addChild(overlay, 100);

    auto duration = .25f;
    auto posW = 0.0f, posH = 0.0f;
    auto page_fx = true;
    move1 = move2 = nullptr;
    
    ActionInterval * move1_grad = nullptr;
    ActionInterval * move2_grad = nullptr;
    switch (index_replace) {
        case 1://next
            move1 = ActionFloat::create(duration, _current_page_angle, -M_PI / 3, [this](float v) {
                transformPageByAngle(_prev_page, v);
                _prev_page_angle = v;
            });
            
            move1_grad = ActionFloat::create(duration, _current_page_angle, -M_PI / 3, [this](float v) {
                transformPageByAngle(_grad_next, v);
            });
            
            break;
        case 2://back
            move2 = ActionFloat::create(duration, _prev_page_angle, 0, [this](float v) {
                transformPageByAngle(_current_page, v);
                _current_page_angle = v;
            });
            
            move2_grad = ActionFloat::create(duration, _prev_page_angle, 0, [this](float v) {
                transformPageByAngle(_grad_prev, v);
            });
            
            break;
        case 3://refresh
            duration = 0.35f;
            page_fx = false;
            move2 = MoveBy::create(duration, Vec2(0, -XHSLib->getSizeScreen().height));
            move1 = MoveBy::create(duration, Vec2(0, -XHSLib->getSizeScreen().height));
            posW = 0;
            posH = XHSLib->getSizeScreen().height;
            break;
        default:
            break;
    }
    if (_menucontrol) {
        _menucontrol->resetStateMenu();
    }
    if (page_new)
    {
        page_new->setPosition(Vec2(posW, posH));
    }

    auto action_callback_end_page_turn = CallFunc::create([=]() {
        _current_page_angle = 0.f;
        auto imp = dynamic_cast<MSPageImp *>(_current_page);
        if (imp) {
            imp->enableInteraction();
            _current_page->scheduleOnce([imp](float dt) {
                if (imp) {

                    MS_LOGGER.logEvent("call_startPlaying_02", {});

                    imp->startPlaying();
                }
            }, 0.25, "delay_play_page");
        }
        
        imp = dynamic_cast<MSPageImp *>(_next_page);
        if (imp) {
            imp->disableInteraction();
        }
        
        imp = dynamic_cast<MSPageImp *>(_prev_page);
        if (imp) {
            imp->disableInteraction();
        }
        
        if (page_new)
        {
            page_new->setTag(100 + XHSLib->pageIndex);
            page_new->setName(StringUtils::format("page_%d", 100 + XHSLib->pageIndex));
            page_new->setLocalZOrder(3);
        }
//        this->getChildByName("overlay")->removeFromParent();
        this->scheduleOnce([this](float dt) {
            this->removeChildByName("overlay");
        }, 0.5, "delay_remove_overlay");
        _is_moving_next = false;
        _is_moving_prev = false;

        if (index_replace == 1 && !isloadallpage) {
//            this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
                if (XHSLib->pageIndex <= XHSLib->pageNumAll) {
                    cocos2d::ui::Layout *page_2 = nullptr;
                    
                    if (is_story_plist){
                        page_2 = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                    }else {
                        page_2 = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                    }

                    if (page_2) {
                        this->addChild(page_2, 1);
                        _next_page = page_2;
                    }

                    if (_prev_page) {

                        auto im = dynamic_cast<MSPageImp *>(_prev_page);
                        
                        if (im)
                        {
                            if (!im->isNeedRefreshed()) {
                                return;
                            }
                        }
                        
                        auto temp = _prev_page;

                        if (is_story_plist) {
                            _prev_page = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex-1);
                        } else {
                            _prev_page = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex-1);
                        }
                        if (_prev_page)
                        {
                            this->addChild(_prev_page, 4);
                            _prev_page_angle = -M_PI / 3;
                            transformPageByAngle(_prev_page, _prev_page_angle);
                        }
                        
                        temp->removeFromParent();
                    }
                }
        } else if (index_replace == 2 && !isloadallpage) {
                if (XHSLib->pageIndex > 1) {

					cocos2d::ui::Layout* page_2 = nullptr;
					if (is_story_plist) {
						page_2 = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex - 1);

					}
					else {
						page_2 = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex - 1);
					}

					if (page_2)
					{
						this->addChild(page_2, 4);
						_prev_page = page_2;
					}

                    _prev_page_angle = - M_PI / 3;
                    transformPageByAngle(_prev_page, _prev_page_angle);
                    
                    if (_next_page) {
                        auto im = dynamic_cast<MSPageImp *>(_next_page);
                        if (im)
                        {
                            if (!im->isNeedRefreshed()) {
                                return;
                            }
                        }

                        auto temp = _next_page;
                        if (is_story_plist) {
                            _next_page = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                        } else {
                            _next_page = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                        }
                        if (_next_page)
                        {
                            this->addChild(_next_page, 1);
                        }
                        temp->removeFromParent();
                    }
                } else {
                    _prev_page = nullptr;
                    if (_next_page) {
                        auto im = dynamic_cast<MSPageImp *>(_next_page);
                        if (im)
                        {
                            if (!im->isNeedRefreshed()) {
                                return;
                            }
                        }
                        auto temp = _next_page;

                        if (is_story_plist) {
                            _next_page = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                        } else {
                            _next_page = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                        }
                        if (_next_page)
                        {
                            this->addChild(_next_page, 1);
                        }
                        temp->removeFromParent();
                    }
                }
        }
    });
    
    if (move1_grad) this->runAction(move1_grad);
    if (move2_grad) this->runAction(move2_grad);

    if (index_replace == 1) {
        if (page_current && move1){
            page_current->setLocalZOrder(4);
            page_current->runAction(Sequence::create(move1, action_callback_end_page_turn, nullptr));
        }
        cocos2d::experimental::AudioEngine::play2d("sounds/page_turn.mp3");
        return;
    }

    if (index_replace == 2) {
        if (page_new && move2) {
            page_new->setLocalZOrder(4);
            page_new->runAction(Sequence::create(move2, action_callback_end_page_turn, NULL));
        }
        cocos2d::experimental::AudioEngine::play2d("sounds/page_turn.mp3");
        return;
    }

    auto check_opengl_win32 = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        check_opengl_win32 = true;
    }
#endif

    if (!check_opengl_win32 && page_new && move2) {//bình thường
        page_new->runAction(Sequence::create(move2, CallFunc::create([=]{
            if (page_current){
                page_current->removeFromParent();
            }
            
            auto imp = dynamic_cast<MSPageImp *>(_current_page);
            if (imp) {
                imp->enableInteraction();
                _current_page->scheduleOnce([imp](float dt) {
                    if (imp) {

                        MS_LOGGER.logEvent("call_startPlaying_03", {});

                        imp->startPlaying();
                    }
                }, 0.25, "delay_play_page");
            }
            
            page_new->setTag(100 + XHSLib->pageIndex);
            page_new->setName(StringUtils::format("page_%d", 100 + XHSLib->pageIndex));
            page_new->setLocalZOrder(3);
            this->getChildByName("overlay")->removeFromParent();
        }), NULL));
		
        if (page_current&& move1)
        {
            page_current->runAction(move1);
        }
    }
    else{//bị opengl_win32
        if (overlay)
        {
            overlay->setBackGroundColor(Color3B::WHITE);
            overlay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            overlay->setOpacity(0);
            if (page_new)
            {
                page_new->setVisible(false);
                page_new->setPosition(Vec2(0, 0));
            }
            overlay->runAction(Sequence::create(FadeIn::create(0.3f), DelayTime::create(0.05f), CallFunc::create([=] {
                if (page_current) {
                    page_current->removeFromParent();
                }
                if (page_new)
                {
                    page_new->setTag(100 + XHSLib->pageIndex);
                    page_new->setName(StringUtils::format("page_%d", 100 + XHSLib->pageIndex));
                    page_new->setLocalZOrder(3);
                    page_new->setVisible(true);
                }
                }), FadeOut::create(0.3f), CallFunc::create([=] {
                    this->getChildByName("overlay")->removeFromParent();
                    }), NULL));
        }
    }

    mj::helper::purgeTextureData();

}

bool StoryPageVC::checkStoryCanRead(int index_replace){
    
    CRASH_AUTO_LOG;
    
    subscribe_type = (int) CONFIG_MANAGER->isAppActive();
    if (subscribe_type) {
        sentStoryIdDownloadSuccess(DOWNLOAD_SUCCESS);
        return true;
    }

    bool checkStoryIsTutoring = StoryDataModel::getInstance()->CheckStoryIsTutoring(_storyInfo._id);

    if (checkStoryIsTutoring)
    {
        return true;
    }

    auto lesson_ids = ms::LessonManager::getInstance().getListLessonFree();
    for (auto items : lesson_ids)
    {
        if (_storyInfo.newest == items)
        {
            return true;
        }
    }

    if (XHSLib->pageIndex < pagecheck) {
        if (_free_play_type != FREE_TIME_TYPE) {
            sentStoryIdDownloadSuccess(DOWNLOAD_SUCCESS);
        }
        if (_free_play_type != 0 && !is_sample_read && !is_activity_play) {
            FREE_STORY_MANAGER->setStoryIDRecentRead(XHSLib->_id, XHSLib->pageIndex, XHSLib->pageNumAll, ACTION_STORY);
        }
        return true;
    };
    
    if (check_free_can_read) {
        if ((pagecheck == 0 && XHSLib->pageIndex == XHSLib->pageNumAll) || pagecheck == XHSLib->pageIndex) {
            FREE_STORY_MANAGER->clearNumPageFreeRead();
            switch (_free_play_type) {
                case FREE_TIME_TYPE:
                    FREE_STORY_MANAGER->setActionIDFreeTimePlayed(XHSLib->_id);
                    sentStoryIdDownloadSuccess(FREE_TIME_USE);
                    FREE_STORY_MANAGER->map_story_recent.clear();
                    break;
                case FREE_OFDAY_TYPE:
                    FREE_STORY_MANAGER->setReadFreeStory(XHSLib->_id);
                    FREE_STORY_MANAGER->map_story_recent.clear();
                    break;
                default:
                    break;
            }
        }
        
        if ((pagecheck == 0 || pagecheck == XHSLib->pageIndex) && !is_activity_play) {
            //đánh dấu truyện này đã đọc nhé!.
            if (!is_sample_read){
                FREE_STORY_MANAGER->setStoryIDRecentRead(XHSLib->_id, XHSLib->pageIndex, XHSLib->pageNumAll, ACTION_STORY);
            }
            return true;
        }
    }
    
    if (index_replace == 1) {
        XHSLib->pageIndex--;
    }
    
    if (index_replace == 2) {
        XHSLib->pageIndex++;
    }
    showSubscribePopup();
    return false;
}

bool StoryPageVC::checkFreeCanRead(){
    subscribe_type = (int) CONFIG_MANAGER->isAppActive();

    if (F2P_MANAGER->isFreeContentPage())
        return true;

    if (subscribe_type) {
        sentStoryIdDownloadSuccess(DOWNLOAD_SUCCESS);
        return true;
    }

    auto percent_played = FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed(ACTION_STORY);
    if (percent_played <= 0.0f) {
        return false;
    }
    
    if (!is_sample_read && !is_activity_play) {
        FREE_STORY_MANAGER->setStoryIDRecentRead(XHSLib->_id, XHSLib->pageIndex, XHSLib->pageNumAll, ACTION_STORY);
    }
    
    if (FREE_STORY_MANAGER->checkFreeTimeAvailable()) {//thời gian 7 ngày đầu.
        _free_play_type = FREE_TIME_TYPE;
        return true;
    }else {//đã hết thời gian 7 ngày đầu.
        if (FREE_STORY_MANAGER->checkFreeOfDayStoryCanRead(XHSLib->_id)) {
            _free_play_type = FREE_OFDAY_TYPE;
            return true;
        }else {
            return false;
        }
    }
    return false;
}

bool StoryPageVC::checkLessonFreeCanRead(){
    subscribe_type = (int) CONFIG_MANAGER->isAppActive();
    if (F2P_MANAGER->isFreeContentPage())
        return true;

    if (subscribe_type) {
        return true;
    }
    if (FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed(ACTION_LESSON) > 0.0f) {
        return true;
    }
    return false;
}

bool StoryPageVC::checkLessonPageStoryCanRead(int page_index_replace){
    
    MS_LOGGER.logEvent("checkLessonPageStoryCanRead",  {
        {"page_index", cocos2d::Value(page_index_replace)},
    });
    
    if (check_free_can_read) {
        return true;
    }
    
    if (XHSLib->pageIndex < pagecheck) {
        sentStoryIdDownloadSuccess(DOWNLOAD_SUCCESS);
        return true;
    };

    if (_isReadStories)
    {
        return true;
    }
    
    if (page_index_replace == 1) {
        XHSLib->pageIndex--;
    }
    
    if (page_index_replace == 2) {
        XHSLib->pageIndex++;
    }
    showSubscribePopup();
    return false;
}

#pragma mark - MENU_CONTROL
void StoryPageVC::displaymenucontrol(bool isNotHide){
    AUDIO_ENGINE->playEffect("slide-rock.mp3");
    if (!_menucontrol) {
        _menucontrol = static_cast<PageMenuControl *>(CSLoader::createNode("csb/PageMenuControl.csb"));
        _menucontrol->setAnchorPoint(Vec2(0.5, 0.5));
        this->addChild(_menucontrol, 100002);
        CCLOG("abc = %f", _menucontrol->getContentSize().width);
        _menucontrol->setPosition(Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height +150));
        _menucontrol->setDelegate(this);
    }
    if (isNotHide) {
        isCloseClicked = true;
        btclose->runAction(FadeOut::create(0.25f));
        auto to_pos = Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height - _menucontrol->getContentSize().height/2);
        _menucontrol->runAction(Sequence::create(MoveTo::create(0.5f, to_pos), NULL));
    }else{
        setShowHideMenuControl(true);
    }
}

void StoryPageVC::setShowHideMenuControl(bool isShow){
    if (menucontrol_isrunning) return;
    menucontrol_isrunning = true;
    Vec2 from_pos, to_pos;
    if (isShow){
        _menucontrol->setClickEnable(true);
        to_pos = Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height - _menucontrol->getContentSize().height/2);
    }else{
        to_pos = Vec2(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height + 150);
    }
    
    auto overlay = _menucontrol->getChildByName("overlay");
    if (isShow) {
        overlay->setOpacity(0);
        overlay->runAction(Sequence::create(CallFuncN::create([](Node * n) {
            n->setVisible(true);
        }),  FadeIn::create(0.5), nullptr));
    } else {
        overlay->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) {
            n->setVisible(false);
        }), nullptr));
    }
    
    _menucontrol->runAction(Sequence::create(DelayTime::create(0.15), MoveTo::create(0.5f, to_pos), CallFunc::create([=]{
        _menucontrol->setPosition(to_pos);
        menucontrol_isrunning = false;
        if (isShow) {
            _menucontrol->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([=]{
                setShowHideMenuControl(false);
            }), NULL));
        }else {
            btclose->runAction(Sequence::create(FadeTo::create(0.2f, 0x88), CallFunc::create([=]{
                isCloseClicked = false;
            }), NULL));
        }
    }), NULL));
}

void StoryPageVC::onClose(cocos2d::Ref *sender){
    CRASH_AUTO_LOG;
    if (isCloseClicked) return;
    isCloseClicked = true;
    btclose->runAction(FadeOut::create(0.2f));
    displaymenucontrol();
}

#pragma mark - RATE_APP
void StoryPageVC::showRatePopup(){
    auto rate_popup = StoryRatePopUp::createPage();
    this->addChild(rate_popup, 19999999);
    rate_popup->onShowHidePopUp(true);
}

void StoryPageVC::showRatePopupBuy(){
    auto rate_popup_buy = StoryRatePopUp::createPage(3);
    this->addChild(rate_popup_buy, 1999999999);
    rate_popup_buy->onShowHidePopUp(true);
}

void StoryPageVC::showRateConfirm(){
//    showAlertWithContent(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_content"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_ok"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_cancel"), 100, false, 35);
    
    showAlertHaiLongApp(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_content"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_ok"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_cancel"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_guiderate"), 100);

}

void StoryPageVC::showSubscribePopup(){
    
    CRASH_AUTO_LOG;
    
    XHSLib->show_popup_subscribe = true;
    
    auto subscirbe_popup = StorySubscribePopup::createSubscribePopup(XHSLib->_id, StorySubscribePopup::SUB_STORY);
    this->addChild(subscirbe_popup, 1999999999);
    subscirbe_popup->setDelegate(this);
    subscirbe_popup->onShowHidePopUp(true);

}
#pragma mark - AlertDelegate
void StoryPageVC::onSubscribeOk(){

    CRASH_AUTO_LOG;
    
    XHSLib->isEndingPageClick = false;
    XHSLib->fontNameStory = "";
    XHSLib->isPopupSubscribeNow = true;
    MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, true);
    DIRECTOR->popScene([](Scene * scene) {
        return TransitionSlideInL::create(0.25, scene);
    });
    _delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FREE_ACTION) : (void) nullptr;
    Director::getInstance()->getScheduler()->schedule([](float) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
        //delay and dispatch event directly
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_subscribe_page);
    }, Application::getInstance(), 0, 0, 0.5, false, "delay_event_after_popscene");
}
void StoryPageVC::onSubscribeCancel(){
    
    CRASH_AUTO_LOG;
    
    XHSLib->isEndingPageClick = false;
    XHSLib->fontNameStory = "";
    mj::AnimationCache::endSession();
    _delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FREE_ACTION) : (void) nullptr;
    DIRECTOR->popScene([](Scene * scene) {
        return TransitionSlideInL::create(0.5, scene);
    });
}

void StoryPageVC::onSubscribeFree(){
    
    CRASH_AUTO_LOG;
    
    XHSLib->isEndingPageClick = false;
    XHSLib->fontNameStory = "";
    XHSLib->isPopupSubscribeNow = true;
    DIRECTOR->popScene([](Scene * scene) {
        return TransitionSlideInL::create(0.25, scene);
    });
    _delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FREE_ACTION) : (void) nullptr;
    MJDEFAULT->setBoolForKey(key_event_go_to_free_page, true);
    Director::getInstance()->getScheduler()->schedule([](float) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
    }, Application::getInstance(), 0, 0, 0.5, false, "delay_event_after_popscene_free");
}

void StoryPageVC::onAlertOK(cocos2d::Ref * sender){
    auto _tag = (int)static_cast<StoryAlert *>(sender)->getTag();
    switch (_tag) {
        case 100:
            MJDEFAULT->setBoolForKey(key_rate_app_won, true);
            if (!CONFIG_MANAGER->isAppActive()) {
                FREE_STORY_MANAGER->setRateTimeExpire(time_bonus_rate);
            }
            break;
        case 101:
        {
            XHSLib->isEndingPageClick = false;
            XHSLib->fontNameStory = "";
            XHSLib->isPopupSubscribeNow = true;
            DIRECTOR->popScene([](Scene * scene) {
                return TransitionSlideInL::create(0.25, scene);
            });
            _delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FREE_ACTION) : (void) nullptr;
            Director::getInstance()->getScheduler()->schedule([](float) {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
            }, Application::getInstance(), 0, 0, 0.5, false, "delay_event_after_popscene");
        }
            break;
        case 103:
            MJDEFAULT->setBoolForKey(key_hailong_app, true);
            showRatePopup();
            break;
        default:
            break;
    }
}

void StoryPageVC::onAlertCancel(cocos2d::Ref * sender){
    auto _tag = (int)static_cast<StoryAlert *>(sender)->getTag();
    switch (_tag) {
        case 100:
            NativeBridge::rateAppMJStories();

            break;
        case 101:
            XHSLib->isEndingPageClick = false;
            XHSLib->fontNameStory = "";
            mj::AnimationCache::endSession();
            _delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FREE_ACTION) : (void) nullptr;
            DIRECTOR->popScene([](Scene * scene) {
                return TransitionSlideInL::create(0.5, scene);
            });

            break;
        case 103:
            MJDEFAULT->setBoolForKey(key_hailong_app, true);
            break;
        default:
            break;
    }

}

void StoryPageVC::showNetworkDownWhenRate(){
    showAlertNetworkDown(102, false, true);
}

#pragma mark - RequestAPI
void StoryPageVC::sentStoryIdDownloadSuccess(int type){
    
    CRASH_AUTO_LOG;
    
    if (!NativeBridge::networkAvaiable()) return;
    if (isSentStoryDownloadSuccess) return;
    isSentStoryDownloadSuccess = true;
}

void StoryPageVC::callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest){
    CCLOG("map_response = %s", JsonDump::getInstance()->MapToString(map_response).c_str());
    if (tagRequest == kTag_API_sentDownloadSuccess ) {
        auto status = map_response["status"].asString();
        if (status == "success") {
            if (!map_response["data"].isNull() && map_response["data"].getType() == Value::Type::MAP) {
                auto data = map_response["data"].asValueMap();
                auto type = data["type"].asInt();
                CCLOG("callback success type = %d", type);
                if (type == DOWNLOAD_SUCCESS) {//downloadsuccess_type
                    StoryDataModel::getInstance()->addStorySentDownloadSuccess(XHSLib->_id, XHSLib->story_id, type);
                }else {//type = FREE_TIME_USE : free_time_read
                    FREE_STORY_MANAGER->clearFreeTimeListStoryRead();
                }
            }
        }
    }
    
    if (tagRequest == kTag_API_rated) {
        auto status = map_response["status"].asString();
        if (status == "success") {
            CCLOG("rated app");
        }
    }
}

#pragma mark - COVER_PAGE
void StoryPageVC::setInfoStory(StoryInfoObj info_story, bool activity_play_mode, bool sample_mode, bool isReadStories, mj::FROMSTORY from){
    _from = from;
    _storyInfo = info_story;
    _isReadStories = isReadStories;
    is_activity_play = activity_play_mode;
    is_sample_read = sample_mode;
	
	m_StoryTracker.m_commonStoryID	= info_story._id;
    m_StoryTracker.m_storyID        = info_story._storyid;
    m_StoryTracker.m_storyName      = info_story.name;
    m_StoryTracker.m_from           = MJDEFAULT->getStringForKey("STORY_START_FROM","unknow");
    m_StoryTracker.m_totalPage      = info_story.total_page;
    m_StoryTracker.m_userAge        = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id,-1)).age; //MJDEFAULT->getIntegerForKey(key_current_profile_id,-1);
    m_StoryTracker.m_storyAuthor    = info_story.written;
    m_StoryTracker.m_storyDesigner  = info_story.design;
    m_StoryTracker.m_storyLevel     = info_story.readinglevel;
	m_StoryTracker.m_storyTopic		= "UNKNOW";

	if (m_StoryTracker.m_from == "LESSION")
	{
		m_StoryTracker.m_lessonID	= MJDEFAULT->getIntegerForKey("clevertap_lesson_id", -1); //for tracking only.
		m_StoryTracker.m_lessonName = MJDEFAULT->getStringForKey("clevertap_lesson_name","NONE");//for tracking only.
	}

    for (int i = 0; i < m_StoryTracker.m_totalPage; ++i)
    {
        m_StoryTracker.m_storyInfors.push_back(ms::StoryInfoTracker(i+1,0.0f,0.0f,0,0,0));
    }

    m_finishStoryTracker.m_storyID        = info_story._storyid;
    m_finishStoryTracker.m_storyName      = info_story.name;

}

StoryInfoObj StoryPageVC::getInfoStory(){
    return _storyInfo;
}

void StoryPageVC::createCoverPage(){
    return;
    std::string csb_path = "csb/StoryInfoCover.csb";
    auto storyinfocover = static_cast<StoryInfoCover *>(CSLoader::createNode(csb_path));
    storyinfocover->initWithData(_storyInfo, false, storyinfocover->Story_Type);
    storyinfocover->setName("content");
//    storyinfocover->setPositionX(-XHSLib->getSizeScreen().width);
    storyinfocover->setName("storyinfocover");
    utils::findChild(storyinfocover, "panel_read")->setVisible(true);
    _prev_page_angle = -M_PI / 3;
    this->addChild(storyinfocover, 4);
    transformPageByAngle(storyinfocover, -M_PI / 3);
}

void StoryPageVC::backToCoverInfoStory(){
    cocos2d::experimental::AudioEngine::play2d("sounds/page_turn.mp3");
    XHSLib->pageIndex = 0;
    auto duration = 0.5f;
 
    cocos2d::ui::Layout *page_current;
    if (is_story_plist){
        page_current = this->getChildByName<TruyenTranhPage *>(StringUtils::format("page_%d", 100+1));

    }else {
        page_current = this->getChildByName<MSBookPage *>(StringUtils::format("page_%d", 100+1));

    }

    auto overlay = Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setTouchEnabled(true);
    overlay->setName("overlay");
    this->addChild(overlay, 100);

    auto page_cover = this->getChildByName("storyinfocover");
    if (!page_cover) {
        createCoverPage();
        page_cover = this->getChildByName("storyinfocover");
    }
    page_cover->setLocalZOrder(4);
    page_cover->setVisible(true);
    
    _prev_page = nullptr;
    _next_page = _current_page;
    _current_page = page_cover;
    
    auto move2 = ActionFloat::create(duration, _prev_page_angle, 0, [this](float v) {
        transformPageByAngle(_current_page, v);
        _current_page_angle = v;
    });
    
    if (page_cover){
        XHSLib->isEndingPageClick = true;
        page_cover->runAction(Sequence::create(move2, CallFunc::create([this, overlay] {
            _current_page->setLocalZOrder(3);
            _next_page->setLocalZOrder(1);
            overlay->removeFromParent();
        }), nullptr));
    }
}

void StoryPageVC::nextPageFromCoverInfStory(){

    CRASH_AUTO_LOG;

    XHSLib->isEndingPageClick = false;
    cocos2d::experimental::AudioEngine::play2d("sounds/page_turn.mp3");
    auto duration = 0.5f;
    XHSLib->pageIndex = 1;

    cocos2d::Node * page_new = nullptr;
    if (is_story_plist){
        page_new = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex);
    }else {
        page_new = _next_page;
    }

    if (page_new) {
        page_new->setTag(100 + 1);
        page_new->setName(StringUtils::format("page_%d", 101));
    }
   
    auto overlay = Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setTouchEnabled(true);
    overlay->setName("overlay");
    this->addChild(overlay, 100);
    
    auto move1 = ActionFloat::create(duration, _current_page_angle, -M_PI / 3, [this](float v) {
        transformPageByAngle(_prev_page, v);
        _prev_page_angle = v;
    });
    
    auto page_cover = this->getChildByName("storyinfocover");
    
    _prev_page = page_cover;
    _current_page = _next_page;
    _next_page = nullptr;
    
    if (page_cover){
        page_cover->setLocalZOrder(4);
        page_cover->runAction(Sequence::create(move1, CallFunc::create([this, page_cover, page_new, overlay] {
            overlay->removeFromParent();
            page_cover->setLocalZOrder(1);

            if (page_new) {
                page_new->setLocalZOrder(3);
                page_new->setVisible(true);
            }
            
            if (XHSLib->pageIndex < XHSLib->pageNumAll) {
                cocos2d::ui::Layout *page_2 = nullptr;
                if (is_story_plist){
                    page_2 = TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                }else {
                    page_2 = MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex+1);
                }
                this->addChild(page_2, 1);
                _next_page = page_2;
            }
        }), nullptr));
    }
}

#pragma mark - ENDING_PAGE
void StoryPageVC::displayEndingPage(){

    CRASH_AUTO_LOG;

    /*only push when ending story !*/
    pushFinishStoryTrackerDataToServer();

    auto const k_totalTimeOnPage =  (cocos2d::utils::getTimeInMilliseconds() - m_currentTime)/1000.0;

    m_currentTime = cocos2d::utils::getTimeInMilliseconds();

    /*page at index m_StoryTracker.m_storyInfors.size() - 1.*/
    if(auto currentPage = dynamic_cast<MSPageImp*>(_current_page))
    {
        int pageID = currentPage->getPageIndex() -1;

        if(pageID >= 0 && pageID < m_StoryTracker.m_storyInfors.size() && !m_StoryTracker.m_storyInfors.empty())
        {
            m_StoryTracker.m_storyInfors.at(pageID).m_totalTimeOnPage += k_totalTimeOnPage;
        }
    }

    XHSLib->enable_keyboard = false;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("story.disable.keyboard");
    btclose->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([=]{
        btclose->setVisible(false);
    }), NULL));

    switch (getTypeEndingPage()) {
        case YOUHAVELEARN_GAME_ENDSTORY:
            //you have learn -> game
            runAirplaneEndStory(ENDPAGE_TYPE);
            break;
        case YOUHAVELEARN_ONLY_ENDSTORY:
            //you have learn -> recommended stories
            runAirplaneEndStory(ENDPAGE_TYPE, false);
            break;
        case GAME_ONLY_ENDSTORY:
            //game
            runAirplaneEndStory(NOYOUHAVELEARN_TYPE);
            break;
        case NULL_ENDSTORY:
            //recommended stories
            if (is_activity_play) {//activity_play_mode
                runAirplaneEndActivity();
            }else runAirplaneEndStory(NOYOUHAVELEARN_TYPE, false);
            
            if (ms::recorder::isRecording()) {
                ms::recorder::stopRecord();
            }
            
            break;
            
        default:
            break;
    }

    pushStoryTrackerDataToServer();

	RatingManager::GetInstance()->showRatingPopup();

    return;
}

int StoryPageVC::getTypeEndingPage() {
    
    CRASH_AUTO_LOG;

    if(StoryDataModel::getInstance()->getActivitiesOfStory(_storyInfo._id).size() != 0 && _storyInfo.typeStory == mj::model::ActInfo::Type::STORY){
        return 1;
    }
    
    return 0;
}

/**
* Display activity story page timeline
*/
void StoryPageVC::runTransitionEndActivity()
{
    CRASH_AUTO_LOG;
    _ageUser = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id, -1)).age;


    Node* node = this;
    node = Director::getInstance()->getRunningScene();



    node->runAction(Sequence::create(CallFunc::create([&] {
        this->removeChildByName("page_turn_touch_layout");
        layout_block_touch->setTouchEnabled(false);
        }), DelayTime::create(0.75f), CallFunc::create([&] {
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false) && !MJDEFAULT->getBoolForKey("debugkey_skip_story", false))
            {
                DIRECTOR->popScene([](Scene* scene) {
                    
                    scene->scheduleOnce([=](float dt) {
                        ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlay();
                        }, 1.0f, "delay_continue_story");

                    return TransitionSlideInL::create(0.5, scene);
                    });
            }
            else {
//#endif
                clickButtonClose();
                auto layerAct = CSLoader::createNode("csb/new_ui/lesson/ActivityEnd_Story.csb");
                layerAct->setAnchorPoint(Point(0.5, 0.5));
                layerAct->setPosition(Director::getInstance()->getRunningScene()->getBoundingBox().size / 2);
                Director::getInstance()->getRunningScene()->addChild(layerAct, 10000);
                if (_checkClickButtonPlay)
                {
                    _totalTimeLearnActvity = _totalTimeLearnActvity + (int(utils::getTimeInMilliseconds() / 1000) - _timeStartActivity);
                }
                _checkClickButtonPlay = false;
                _checkCloseInteractiveQuestion = false;
                addLayerActivityEnd(layerAct);
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            }
//#endif
            
            }), nullptr
     ));
}

void StoryPageVC::runAirplaneEndActivity(){
    
    CRASH_AUTO_LOG;

    this->removeChildByName("page_turn_touch_layout");
    
    if (_current_page) {
        auto action = Sequence::create(ActionFloat::create(0.25, _current_page_angle, - M_PI / 3, [this](float v) {
            transformPageByAngle(_current_page, v);
            _current_page_angle = v;
        }), nullptr);
        _current_page->runAction(action);
        
        auto action_grad = ActionFloat::create(0.25, _current_page_angle, - M_PI / 3, [this](float v) {
            transformPageByAngle(_grad_next, v);
        });
        _grad_next->runAction(action_grad);
    }
    
    auto layout_block = ui::Layout::create();
    layout_block->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layout_block->setContentSize(XHSLib->getSizeScreen());
    layout_block->setTouchEnabled(true);
    layout_block->setBackGroundColor(Color3B::WHITE);
    layout_block->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_block->setOpacity(0);
    this->addChild(layout_block, 99);
    
    auto layout_touch = ui::Layout::create();
//    layout_touch->setContentSize(XHSLib->getSizeScreen());
    layout_touch->setTouchEnabled(true);
    layout_touch->setBackGroundColor(Color3B::WHITE);
    layout_touch->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_touch->setOpacity(230);
    layout_touch->setContentSize(Size(XHSLib->getSizeScreen().width + linerun_end_x+100, XHSLib->getSizeScreen().height));
    
    auto layout_black = ui::Layout::create();
    layout_black->setBackGroundColor(Color3B::BLACK);
    layout_black->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_black->setOpacity(100);
    layout_black->setContentSize(layout_touch->getContentSize());
    layout_touch->addChild(layout_black);
    layout_black->setAnchorPoint(Vec2(0,0));
    
    auto pos_y = XHSLib->getSizeScreen().height/2;
    auto line_run = ImageView::create("mjstory/theendpage/theendpage_linerun.png");
    line_run->setPosition(Vec2(-900, pos_y));
    this->addChild(line_run, 19999999);
    line_run->setName("line_run");
    
    auto airplane = GAFWrapper::createGAF(HSLib::anims[0]);
    auto flag = GAFWrapper::createGAF(HSLib::anims[1]);
    line_run->addChild(flag, 10);
//    flag->setPosition(Vec2(linerun_start_x,line_run->getContentSize().height*0.55));
    flag->setPosition(Vec2(linerun_start_x,line_run->getContentSize().height*0.47));
    flag->play(true);
//    airplane->setPosition(Vec2(500,line_run->getContentSize().height*0.58));
    airplane->setPosition(Vec2(200,line_run->getContentSize().height*(0.81-0.08)));
    
    if (CONFIG_MANAGER->isVietnameseStories()) flag->setVisible(false);
    auto clone_linerun = line_run->clone();
    line_run->addChild(clone_linerun, 100);
    clone_linerun->setPosition(line_run->getContentSize()/2);
    
    line_run->addChild(layout_touch, 1);
    layout_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    layout_touch->setPosition(line_run->getContentSize()/2);
    
    line_run->addChild(airplane, 11);
    airplane->play(true);

	if ("lesson" == MJDEFAULT->getStringForKey("STORY_START_FROM", "unknow") && MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false) == true)
	{
		AUDIO_ENGINE->playEffect("theendpage/endingpage_airplane.mp3");
		line_run->runAction(Sequence::create(MoveTo::create(linerun_time*0.1f, Vec2(0, pos_y)), 
			MoveTo::create(linerun_time*0.5f, Vec2(XHSLib->getSizeScreen().width*0.8f, pos_y)), 
			CallFunc::create([=] {
			AUDIO_ENGINE->playEffect("theendpage/endingpage_yeahmusic.mp3");}),
			MoveTo::create(linerun_time*0.2, Vec2(XHSLib->getSizeScreen().width + linerun_end_x, pos_y)),
			CallFunc::create([=] {
			_delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FINISHED) : (void) nullptr;
			DIRECTOR->popScene([](Scene * scene) {
				return scene;
			});
		}), nullptr));
	}
	else
	{
		AUDIO_ENGINE->playEffect("theendpage/endingpage_airplane.mp3");
		line_run->runAction(Sequence::create(MoveTo::create(linerun_time * 0.1, Vec2(0, pos_y)), CallFunc::create([=] {

		}), MoveTo::create(linerun_time * 0.6, Vec2(XHSLib->getSizeScreen().width * 0.8f, pos_y)), CallFunc::create([=] {
			AUDIO_ENGINE->playEffect("theendpage/endingpage_yeahmusic.mp3");
		}), MoveBy::create(1.5f, Vec2(100, 0)), CallFunc::create([=] {

		}), MoveTo::create(linerun_time * 0.2, Vec2(XHSLib->getSizeScreen().width + linerun_end_x, pos_y)), DelayTime::create(1.0f),
			CallFunc::create([=] {
			_delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FINISHED) : (void) nullptr;
			DIRECTOR->popScene([](Scene* scene) {
				return TransitionSlideInL::create(0.5, scene);
			});
		}), NULL));
	}
}

void StoryPageVC::runAirplaneEndStory(int type, bool isHaveGame) {
    
    CRASH_AUTO_LOG;

    //AddEventTracking 
    ms::f2p::FreeContentOption freeContentOption = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option);
    if (freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM 
        || freeContentOption == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY
        || freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY)
    {
        F2P_MANAGER->pushEventTrackingDoneItemFree(_storyInfo._id);
    }

    AUDIO_ENGINE->stopSound("theendpage/endingpage_leaf.mp3");
    this->removeChildByName("page_turn_touch_layout");
    if (_current_page) {
        auto action = Sequence::create(ActionFloat::create(0.25, _current_page_angle, - M_PI / 3, [this](float v) {
            transformPageByAngle(_current_page, v);
            _current_page_angle = v;
        }), nullptr);
        _current_page->runAction(action);
        
        auto action_grad = ActionFloat::create(0.25, _current_page_angle, - M_PI / 3, [this](float v) {
            transformPageByAngle(_grad_next, v);
        });
        _grad_next->runAction(action_grad);
    }
    
    auto layout_touch = ui::Layout::create();
    layout_touch->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layout_touch->setContentSize(XHSLib->getSizeScreen());
    layout_touch->setTouchEnabled(true);
    layout_touch->setBackGroundColor(Color3B::WHITE);
    layout_touch->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_touch->setOpacity(0);
    this->addChild(layout_touch, 99);
    
    
    auto pos_y = XHSLib->getSizeScreen().height/2;
    auto line_run = ImageView::create("mjstory/theendpage/theendpage_linerun.png");
    line_run->setPosition(Vec2(-900, pos_y));
    this->addChild(line_run, 19999999);
    line_run->setName("line_run");
    
    auto airplane = GAFWrapper::createGAF(HSLib::anims[0]);
    
    if (type != ENDGAME_TYPE)
    {
        if(isHaveGame)
        {
            layout_touch->setLocalZOrder(10002);
            airplane->setPosition(Vec2(-220,line_run->getContentSize().height*(0.81-0.08)));
        }
        else
        {
            auto flag = GAFWrapper::createGAF(HSLib::anims[1]);
            line_run->addChild(flag);
            flag->setPosition(Vec2(linerun_start_x,line_run->getContentSize().height*0.47));
            flag->play(true);
            airplane->setPosition(Vec2(200,line_run->getContentSize().height*(0.81-0.08)));
            
            auto clone_linerun = line_run->clone();
            line_run->addChild(clone_linerun, 100);
            clone_linerun->setPosition(line_run->getContentSize()/2);
        }
    }
    else
    {
        layout_touch->setLocalZOrder(10002);
        airplane->setPosition(Vec2(-220,line_run->getContentSize().height*(0.81-0.08)));
    }
    
    line_run->addChild(airplane);
    airplane->play(true);
    
    if (type == NOYOUHAVELEARN_TYPE)
    {
        isHaveGame ? showGame(GAME_ONLY_ENDSTORY):showRecomentStories(5.0f);
    }
    else
    {
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsScene("story_read_scene", "youhavelearn_page");
        addTheEndPage(type, isHaveGame);
    }
    
    layout_touch->runAction(Sequence::create(FadeTo::create(0.5f, 220), CallFunc::create([=]{
        AUDIO_ENGINE->playEffect("theendpage/endingpage_airplane.mp3");
        if (type != ENDGAME_TYPE) {
            line_run->runAction(Sequence::create(MoveTo::create(linerun_time*0.6, Vec2(XHSLib->getSizeScreen().width*0.8f, pos_y)), CallFunc::create([=]{
                if (type != NOYOUHAVELEARN_TYPE) {
                    AUDIO_ENGINE->playEffect("theendpage/endingpage_yeahmusic.mp3");
                }
            }),MoveBy::create(1.0f, Vec2(100, 0)),MoveTo::create(linerun_time*0.2, Vec2(XHSLib->getSizeScreen().width + linerun_end_x, pos_y)), CallFunc::create([=]{
                layout_touch->removeFromParent();
                line_run->removeFromParent();
            }), NULL));
        }else {
            line_run->runAction(Sequence::create(MoveTo::create(linerun_time*0.65, Vec2(XHSLib->getSizeScreen().width + linerun_end_x, pos_y)), CallFunc::create([=]{
                layout_touch->removeFromParent();
                line_run->removeFromParent();
                auto game = this->getChildByName("game_game");
                if (game) {
                    game->removeFromParent();
                }
            }), NULL));
        }
    }), NULL));
}

void StoryPageVC::addTheEndPage(int type, bool isHaveGame) {
    auto the_end_page = StoryTheEndPage::createPage(type, isHaveGame);
    this->addChild(the_end_page, 100);
    if (type == ENDGAME_TYPE) {
        the_end_page->setLocalZOrder(10003);
    }
}

void StoryPageVC::pushEventInteractiveQuestion()
{
    if (_checkClickButtonPlay)
    {
        _totalTimeLearnActvity = _totalTimeLearnActvity + (int(utils::getTimeInMilliseconds() / 1000) - _timeStartActivity);
    }
    _stopActivity = act_index + 1;
    if (_stopActivity > _totalActivity)
    {
        _stopActivity = _totalActivity;
    }
    if (!_checkStartGame)
    {
        _timeReadStory = int(utils::getTimeInMilliseconds() / 1000) - _timeStartReadStory;
    }
    _checkEndStory = true;
    _checkPushEvent = false;
    float completeActivity = ((float)_countNumberClickPlayNow / (float)_totalActivity) * 100.f;

    if(activities.size()<=0)
        return;
    
    if (act_index < activities.size()) {
        
        auto game = StoryDataModel::getInstance()->getGameById(activities[act_index].sub_id);
        _gameNameExit = game.name;
    }
    else
    {
        if(act_index<1 || activities.size()< act_index - 1)
            return;
        auto game = StoryDataModel::getInstance()->getGameById(activities[act_index - 1].sub_id);
        _gameNameExit = game.name;
    }

    try
    {
        std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingInteractiveQuestion =
        {
         {"story_id", cocos2d::Value(_storyInfo.storyid)},
        {"story_name", cocos2d::Value(_storyInfo.name)},
        {"stop_at_activity", cocos2d::Value(_stopActivity)},
        {"total_activity", cocos2d::Value(_totalActivity)},
        {"count_skip", cocos2d::Value(_countNumberClickSkip)},
        {"count_playnow", cocos2d::Value(_countNumberClickPlayNow)},
        {"completed", cocos2d::Value((int)completeActivity)},
        {"story_grade", cocos2d::Value(name_grade)},
        {"duration", cocos2d::Value(_totalTimeLearnActvity)},
        {"profile_age", cocos2d::Value(_ageUser)},
        {"stop_at_activity_name", cocos2d::Value(_gameNameExit)},
        {"activities_played", cocos2d::Value(_gameNamePlay)},
        {"activities_skipped", cocos2d::Value(_gameNameSkip)}
        };

        CleverTapManager::GetInstance()->pushEvent("Interactive_question", eventTrackingInteractiveQuestion);
    }
    catch (const char* i_errorCode)
    {
        CCLOG("Error: cannot push story tracking data to server !");
    }
}

void StoryPageVC::showRecomentStories(float duration_delay){
    mm::RealtimeDatabase::getInstance().pushDataFirestore(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), XHSLib->_id);
    if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
    {
        this->unschedule("delay_time_replace_scene");
        this->scheduleOnce([=](float dt) {
            DIRECTOR->popScene([](Scene* scene) {

                scene->scheduleOnce([=](float dt) {
                    ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlay();
                    }, 1.0f, "delay_continue_story");

                return TransitionSlideInL::create(0.5, scene);
                });
            }, 1.0f, "delay_time_replace_scene");
      
        return;
    }
    if (_totalActivity > 0 && _checkPushEvent)
    {
        pushEventInteractiveQuestion();
        pushFireBaseInteractiveQuestionActivitySkipped();
    }
    pushCleverTapTrackingData();
    _page_read = XHSLib->pageNumAll;
    
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsScene("story_read_scene", "recomment_story_page");
    
    if(_from == mj::FROMSTORY::FREE_TWO_PAID || F2P_MANAGER->isFreeContentPage())
    {
        if (_totalActivity > 0 && utils::findChild(Director::getInstance()->getRunningScene(), "activity_end_story"))
        {
            this->removeChildByName("activity_end_story");
        }
        this->scheduleOnce([=] (float dt) {
            onClickBtClose();
            DIRECTOR->popScene([](Scene * scene) {
                return TransitionSlideInL::create(1.0f, scene);
            });
        }, _totalActivity > 0 ? 0 : 4.5f, "delay_end_f2p");
        
        return;
    }
    
    recomment_page = StoryEndingPage::createPage();
    recomment_page ->setName("recomment_page");
    this->addChild(recomment_page, 10000);
    if (duration_delay > 0) {
        recomment_page->setContentSize(cocos2d::Size(0, 0));
        this->schedule(schedule_selector(StoryPageVC::updateShowRecommentPage));
    }else {
        recomment_page->runAnimShow();
    }
}

void StoryPageVC::updateShowRecommentPage(float dt){

    if (!game_linerun) {
        game_linerun = cocos2d::utils::findChild(this, "line_run");
    }
    auto pos_x = game_linerun->getPositionX();
    if (pos_x <= 0) return;
    if (pos_x >= XHSLib->getSizeScreen().width) {
        this->unschedule(schedule_selector(StoryPageVC::updateShowRecommentPage));
        recomment_page->setContentSize(cocos2d::Size(XHSLib->getSizeScreen().width, XHSLib->getSizeScreen().height));
        recomment_page->runAnimShow();
        game_linerun = nullptr;
        recomment_page = nullptr;
    }else {
        recomment_page->setContentSize(cocos2d::Size(pos_x, XHSLib->getSizeScreen().height));
    }
}

#pragma mark - GAME
void StoryPageVC::showGame(int type){
//    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsScene("story_read_scene", "game_page");
    if (type == YOUHAVELEARN_GAME_ENDSTORY) {
        AUDIO_ENGINE->playEffect("theendpage/endingpage_leaf.mp3");
        layout_block_touch->setTouchEnabled(true);
        auto leaf_trans = GAFWrapper::createGAF("gaf/Leaf_trans/Leaf_trans.gaf");
        leaf_trans->setScale(1.5);
        this->addChild(leaf_trans, 10010);
        leaf_trans->setPosition(Vec2(XHSLib->getSizeScreen().width/2+10, XHSLib->getSizeScreen().height/2));
        leaf_trans->play(false, [=] (gaf::GAFObject *obj){
            this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
                leaf_trans->removeFromParent();
                layout_block_touch->setTouchEnabled(false);
            }), NULL));
        });
        
        game_page = mj::game::createGameFromFile(StringUtils::format("%d_%d_game.plist", XHSLib->story_id, XHSLib->lang_id));
        
        auto game_type = mj::TITLE_GAME_DEFAULT.at(game_page->getGameInfo().game_id);
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventPlayGameInStory, {
            {ms::analytics::kPropertyStoryId, cocos2d::Value(_storyInfo.storyid)},
            {ms::analytics::kPropertyStoryName, cocos2d::Value(_storyInfo.name)},
            {ms::analytics::kPropertyGameType, cocos2d::Value(game_type)}
        });
        
        game_page->setName("game_game");
        game_page->retain();
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
            this->addChild(game_page, 5);
            AUDIO_ENGINE->playEffect("theendpage/endingpage_leaf.mp3");
            game_page->release();
        }), NULL));

    }else if (type == GAME_ONLY_ENDSTORY){
        layout_game = Layout::create();
        layout_game->setName("game_game");
         this->addChild(layout_game, 10000);
        layout_game->setContentSize(cocos2d::Size(0, 0));
        this->schedule(schedule_selector( StoryPageVC::updateShowGame));
    }
}

void StoryPageVC::startGame(int story_id, int act_order){
//    utils::findChild<Layout*>(this, "game_game")->setTouchEnabled(false) ;

    _timeReadStory = int(utils::getTimeInMilliseconds() / 1000) - _timeStartReadStory;


    if (auto gameName = utils::findChild<Layout*>(this, "game_game"))
    {
        gameName->removeAllChildren();
    }
    activities = StoryDataModel::getInstance()->getActivitiesOfStory(story_id);
    
    for(auto &ac:activities) {
        
        ac.storyName = m_StoryTracker.m_storyName;
    }
    
    _totalActivity = activities.size();
    name_grade = StoryDataModel::getInstance()->getLevelInfoById(_storyInfo.readinglevel, _storyInfo.languageid).name;

    runTransitionEndActivity();
}

void StoryPageVC::updateShowGame(float dt){
    if (!game_linerun) {
        game_linerun = cocos2d::utils::findChild(this, "line_run");
    }
    
    auto game_layer = utils::findChild<Layout*>(this, "game_game");
    
    auto pos_x = game_linerun->getPositionX();
    if (pos_x <= 0) return;
    if (pos_x >= XHSLib->getSizeScreen().width) {
        this->unschedule(schedule_selector(StoryPageVC::updateShowGame));
        game_layer->setContentSize(cocos2d::Size(XHSLib->getSizeScreen().width, XHSLib->getSizeScreen().height));
//        game_layer->setTouchEnabled(true);
//        game_layer = nullptr;
//        game_page->setContentSize(cocos2d::Size(XHSLib->getSizeScreen().width, XHSLib->getSizeScreen().height));
//        game_page = nullptr;
        game_linerun = nullptr;
        
//        auto bg_img = ImageView::create("playVGame.jpg");
//        game_layer->addChild(bg_img);
//        bg_img->setOpacity(0);
//        bg_img->setPosition(game_layer->getContentSize()/2);
//
//        auto skip_btn = Button::create("btn_img.png");
//        skip_btn->setTitleText("Bỏ qua");
//        skip_btn->setTitleFontSize(38);
//        skip_btn->setTitleColor(Color3B(75, 43, 43));
//        skip_btn->setOpacity(0);
//        game_layer->addChild(skip_btn);
//        skip_btn->setPosition(Vec2( layout_game->getContentSize().width*0.7, game_layer->getContentSize().height*0.5));
//        skip_btn->addClickEventListener([=](Ref* sender){
//            AUDIO_ENGINE->play("sounds/click_soft.mp3");
//            runAirplaneEndStory(ENDGAME_TYPE, false);
//        });
//
//        auto play_btn = Button::create("btn_img.png");
//        play_btn->setTitleText("Chơi trò chơi");
//        play_btn->setTitleFontSize(38);
//        play_btn->setTitleColor(Color3B(75, 43, 43));
//        skip_btn->setOpacity(0);
//        game_layer->addChild(play_btn);
//        play_btn->setPosition(Vec2( layout_game->getContentSize().width*0.3, game_layer->getContentSize().height*0.5));
//        play_btn->addClickEventListener([=](Ref* sender){
//            AUDIO_ENGINE->play("sounds/click_soft.mp3");
//            startGame(_storyInfo._id , act_index);
//        });
        
        this->scheduleOnce([=](float){
//            bg_img->runAction(FadeIn::create(0.4));
//            skip_btn->runAction(FadeIn::create(0.4));
//            play_btn->runAction(FadeIn::create(0.4));
            _checkStartGame = true;
            startGame(_storyInfo._id , act_index);
        }, 0.2f, "showInGame");
    }else {
//        if(game_page){
//            game_page->setContentSize(cocos2d::Size(pos_x, XHSLib->getSizeScreen().height));
//        }
//
//        if(layout_game){
//            layout_game->setContentSize(cocos2d::Size(pos_x, XHSLib->getSizeScreen().height));
//        }
        game_layer->setContentSize(cocos2d::Size(pos_x, XHSLib->getSizeScreen().height));
    }
}

void StoryPageVC::showGameOver(bool is_interrupted){
    AUDIO_ENGINE->stopAllEffects();
    CCLOG("Finished");
    
    if (FileUtils::getInstance()->isDirectoryExist (FileUtils::getInstance()->getWritablePath() + "extracted/") ) {
        FileUtils::getInstance()->removeDirectory( FileUtils::getInstance()->getWritablePath() + "extracted/");
        auto searchpath = FileUtils::getInstance()->getSearchPaths();
        CCLOG("Remove directory: %s", searchpath.begin()->c_str());
        searchpath.erase(searchpath.begin());
        CCLOG("Remove directory: %s", searchpath.begin()->c_str());
        searchpath.erase(searchpath.begin());
        FileUtils::getInstance()->setSearchPaths(searchpath);
        
    }
    
    if(is_interrupted){
        runAirplaneEndStory(NOYOUHAVELEARN_TYPE, false);
        return;
    }
    
    act_index++;
    if(act_index < activities.size()){
        runTransitionEndActivity();
    }else{
        if (_checkClickButtonPlay)
        {
            _totalTimeLearnActvity = _totalTimeLearnActvity + int(utils::getTimeInMilliseconds() / 1000) - _timeStartActivity;
        }
        _checkClickButtonPlay = false;
        endActivity();
    }
}

void StoryPageVC::showMonkeyPush(){
    if (isRunningMove) return;
    isRunningMove = true;
    
    auto the_end_page = StoryTheEndPage::createPage(ENDPAGE_TYPE, true);
    this->addChild(the_end_page, 2);
    mj::AnimationCache::endSession();
    
    auto layout_touch = ui::Layout::create();
    layout_touch->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layout_touch->setContentSize(XHSLib->getSizeScreen());
    layout_touch->setTouchEnabled(true);
    this->addChild(layout_touch, 4);
    
    auto monkey_push = GAFWrapper::createGAF("gaf/monkey_push/monkey_push.gaf");
    layout_touch->addChild(monkey_push);
    monkey_push->playSequence("push", true);
    monkey_push->setPosition(Vec2(835 + XHSLib->getSizeScreen().width, 247));
    auto move2 = MoveBy::create(2.0f/1024*XHSLib->getSizeScreen().width, Vec2(-XHSLib->getSizeScreen().width, 0));
    monkey_push->runAction(Sequence::create(move2, CallFunc::create([=]{
        monkey_push->runAction(MoveBy::create(1.0f, Vec2(250, 0)));
        
        monkey_push->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
            AUDIO_ENGINE->stopEffect(audio_monkey_foot);
            AUDIO_ENGINE->playEffect("theendpage/theendpage_handwipping.mp3");
        }), DelayTime::create(1.25f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("theendpage/theendpage_monkeyrun.mp3");
        }),NULL));
        
        the_end_page->runAnimShow();
        monkey_push->playSequence("stepback", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            layout_touch->removeFromParent();
        });
    }),NULL));
    audio_monkey_foot = AUDIO_ENGINE->playEffect("theendpage/theendpage_monkeyfoot.mp3", true);
}

#pragma mark - PAUSE_PLAY_BUTTON
void StoryPageVC::onClickBtPlayPause(bool is_play){
    
    ms::EventLogger::getInstance().logEvent("StoryPageVC::onClickBtPlayPause", {{"is_play", cocos2d::Value(is_play?"true":"false") }});
    
    if (is_play) {//refresh page
        onReplacePage(3);
        _menucontrol->resetStateMenu();
        menucontrol_isrunning = false;
        isCloseClicked = true;
        btclose->stopAllActions();
        _menucontrol->stopAllActions();
        _menucontrol->setClickEnable(false);
        setShowHideMenuControl(false);
        
    }else {//pause page
        if (is_story_plist) {
//            auto page_story = this->getChildByTag<TruyenTranhPage *>(100+XHSLib->pageIndex);
            auto page_story = this->getChildByName<TruyenTranhPage *>(StringUtils::format("page_%d", 100+XHSLib->pageIndex));

            if (page_story) {
                page_story->onPauseAllAnimationAndSound();
            }
        }else {
//            auto page_story = this->getChildByTag<MSBookPage *>(100+XHSLib->pageIndex);
            auto page_story = this->getChildByName<MSBookPage *>(StringUtils::format("page_%d", 100+XHSLib->pageIndex));

            if (page_story) {
                page_story->onPauseAllAnimationAndSound();
            }
        }
    }
}

void StoryPageVC::onClickBtClose() {
    CRASH_AUTO_LOG;
    _checkClickButtonCloseOfStories = true;

    ms::f2p::FreeContentOption freeContentOption = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option);
    if (freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM
        || freeContentOption == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY
        || freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY)
    {
        F2P_MANAGER->pushEventTrackingDoneItemFree(_storyInfo._id);
    }

    if (!_checkEndStory)
    {
        _timeReadStory = int(utils::getTimeInMilliseconds() / 1000) - _timeStartReadStory;
        _numberPage = XHSLib->pageIndex;
    }
    else
    {
        _numberPage = XHSLib->pageNumAll;
    }
    if (_checkPushEventLearnStroy)
    {
        pushCleverTapTrackingData();
    }
    auto reason = (XHSLib->pageIndex < XHSLib->pageNumAll) ? ms::delegate::Activity::EndReason::INTERRUPTED : ms::delegate::Activity::EndReason::FINISHED;
    _delegate ? _delegate->onActivityEnd(this, reason) : (void) nullptr;
}

void StoryPageVC::onClickBtFavorite(bool i_isFavorited) {

    m_finishStoryTracker.m_favorited = i_isFavorited;

}

void StoryPageVC::sendEventAnalyticsWhenCloseStory(){
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsParametersEvent(analytics_event_close_story, _map_event_analytics);
    
    if (XHSLib->pageIndex < XHSLib->pageNumAll) {
        auto list_category = StoryDataModel::getInstance()->getListCategoryWithStoryId(XHSLib->_id);
        for (int i = 0; i < (int)list_category.size(); i++) {
            mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_close_category+StringUtils::toString(list_category[i]), "", "");
        }
    }
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryPageVC::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryPageVC, onClose),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void StoryPageVC::setDisableCancelRecordAction(bool i_input)
{
    m_disableCancelRecordAction = i_input;
}

void StoryPageVC::pushStoryTrackerDataToServer()
{
    if(!m_hasPendingTrackingEvents)
    {
        return;
    }

    m_hasPendingTrackingEvents = false;

    auto convertDataToJsonFun = [](ms::StoryInfoTracker i_obj) -> std::string {

        const char * k_format = "\"%d\":[%.1f,%.1f,%d,%d,%d]";

        return cocos2d::StringUtils::format(k_format,
                i_obj.m_currentPage,
                i_obj.m_totalTimeOnPage,
                i_obj.m_standardTime,
                i_obj.m_repeatCount,
                i_obj.m_imgClickedCount,
                i_obj.m_gafClickedCount);
    };

    if(!m_StoryTracker.m_storyInfors.empty())
    {
        std::vector<std::string> storyPagesInfoJsonFormatList;

        std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =  {
            {msAnalyticsKStoryID, cocos2d::Value(m_StoryTracker.m_storyID)},
            {msAnalyticsKStoryName, cocos2d::Value(m_StoryTracker.m_storyName)},
            {msAnalyticsKStoryFrom,cocos2d::Value(m_StoryTracker.m_from)},
            {msAnalyticsKStoryTotalPage, cocos2d::Value(m_StoryTracker.m_totalPage)},
            {msAnalyticsKProfileAge, cocos2d::Value(m_StoryTracker.m_userAge)},
            {msAnalyticsKStoryAuthor,cocos2d::Value( m_StoryTracker.m_storyAuthor)},
            {msAnalyticsKStoryDesigner, cocos2d::Value(m_StoryTracker.m_storyDesigner)},
            {msAnalyticsKStoryLevel, cocos2d::Value(m_StoryTracker.m_storyLevel)},
            {msAnalyticsKStoryTopic, cocos2d::Value(m_StoryTracker.m_storyTopic)},
        };

        const int pageInfoNum       = static_cast<int>(std::ceil(m_StoryTracker.m_storyInfors.size()/3.f));
        const int K_PAGE_NUM_MAX    = 3;

        int id = 0;

        //If we have 7 pages -> pageInfoNum = ceil(7/K_PAGE_NUM_MAX) = 3 block.
        //example block01: {"1":[4013.6,9.6,2,0,0],"2":[2.9,4.4,1,0,0],"3":[2.1,8.7,1,0,0]}
        //example block02: {"4":[1.6,4.7,1,0,0],"5":[3.1,6.1,1,0,0],"6":[2.5,5.7,1,0,0]}
        //example block03: {"7":[1.6,4.7,1,0,0]}
        //
        for (int blockID = 0; blockID < pageInfoNum ; ++blockID)
        {
            std::string storyJsonFormat = "{";

            for (int i = 0; i < K_PAGE_NUM_MAX; ++i)
            {
                auto pageInfo = m_StoryTracker.m_storyInfors.at(id); id++;

                if( blockID == pageInfoNum -1 && id  >= m_StoryTracker.m_storyInfors.size())
                {
                    storyJsonFormat += ( convertDataToJsonFun(pageInfo) +"}" );

                    break;
                }

                storyJsonFormat += ( convertDataToJsonFun(pageInfo) + ( i == K_PAGE_NUM_MAX - 1 ? "}" : ",") );

            }

            storyPagesInfoJsonFormatList.push_back(storyJsonFormat);

            //CCLOG("XXX|storyJsonFormat:%s",storyJsonFormat.c_str());
        }

        for (int i = 0; i < storyPagesInfoJsonFormatList.size() ; ++i)
        {
            eventTrackingProperties.push_back({StringUtils::format("pageInfo%d",i+1),cocos2d::Value(storyPagesInfoJsonFormatList.at(i))});
        }

        //we need to use try catch to avoid crash
        try
        {
            ms::EventLogger::getInstance().logEvent(ms::analytics::kStoryLearningProperties,eventTrackingProperties);
        }
        catch (const char * i_errorCode)
        {
            CCLOG("Error: cannot push story tracking data to server !");
        }
    }
}

void StoryPageVC::pushFinishStoryTrackerDataToServer()
{
    //we need to use try catch to avoid crash
    try
    {
        CCLOG("XXX|pushFinishStoryTrackerDataToServer: %d %s %d",m_finishStoryTracker.m_storyID,m_finishStoryTracker.m_storyName.c_str(),m_finishStoryTracker.m_favorited);

        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishStory, {
                {ms::analytics::kPropertyStoryId, cocos2d::Value(m_finishStoryTracker.m_storyID)},
                {ms::analytics::kPropertyStoryName, cocos2d::Value(m_finishStoryTracker.m_storyName)},
                {ms::analytics::kPropertyStoryLanguage, cocos2d::Value(_storyInfo.languageid == 1 ? "en" : "vi")},
                {ms::analytics::kPropertyStoryFavorite,cocos2d::Value((int)m_finishStoryTracker.m_favorited)},
        });
    }
    catch (const char * i_errorCode)
    {
        CCLOG("Error: cannot push story tracking data to server !");
    }

}

void StoryPageVC::pushCleverTapTrackingData()
{
    vector <std::string> textReadFrom = {};
    //we need to use try catch to avoid crash
	try
	{
        _totalAct = StoryDataModel::getInstance()->getActivitiesOfStory(_storyInfo._id).size();

        std::string readFrom = "";
        readFrom = MJDEFAULT->getStringForKey(key_read_from);
        if (readFrom.find(".") != -1)
        {
            textReadFrom = mj::helper::split(readFrom, '.');
            if (textReadFrom.size() >= 2)
            {
                readFrom = textReadFrom[1];
                MJDEFAULT->setStringForKey(key_read_from, textReadFrom[0]);
            }
        }

		auto listCategory = StoryDataModel::getInstance()->getListCategoryWithStoryId(m_StoryTracker.m_commonStoryID);

		int catalogID = (listCategory.size() > 0 ? listCategory.front() : -1);

		std::string catalogName = "";

		auto catalogInfo = StoryDataModel::getInstance()->getCategoryByID(catalogID);

		if (catalogInfo.count("content"))
		{
			catalogName = catalogInfo.at("content").asString();
		}

		auto gradeName = StoryDataModel::getInstance()->getLevelInfoById(_storyInfo.readinglevel, _storyInfo.languageid).name;

		int currentPageID = -1;

		auto currentPage = dynamic_cast<MSPageImp*>(_current_page);
		{
			currentPageID = currentPage->getPageIndex();
		}
        _numberPage = _checkEndStory ? m_StoryTracker.m_totalPage : currentPageID;
        float xComplete = (float)(_numberPage) / (float)(m_StoryTracker.m_totalPage) * 100.f;
        if (xComplete > 100)
        {
            xComplete = 100;
        }

        int totalTurnPreviousPage = MJDEFAULT->getIntegerForKey(key_turn_previous_page, 0);

        _timeReadStory = _timeReadStory - MJDEFAULT->getIntegerForKey(key_sum_time_user_exit_app, 0);
        if (_timeReadStory > 1800 || _timeReadStory <= 0)
        {
            _timeReadStory = 1800;
        }
        std::string typeOfLearning = MJDEFAULT->getStringForKey("key_type_of_learning_voice");
        MJDEFAULT->deleteValueForKey("key_type_of_learning_voice");

        if(typeOfLearning == "")
        {
            typeOfLearning = XHSLib->story_play_type==1?"read_for_me":(XHSLib->story_play_type == 2 ?"Auto_play":"readitmyself");
        }

        MJDEFAULT->deleteValueForKey(key_sum_time_user_exit_app);
        MJDEFAULT->deleteValueForKey(key_time_user_exit_app);

		std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =
		{
		  {"item_id", cocos2d::Value(m_StoryTracker.m_storyID)},
		  {"item_name", cocos2d::Value(m_StoryTracker.m_storyName)},
		 // {"lesson_id", cocos2d::Value(m_StoryTracker.m_lessonID)},
		 // {"lesson_name", cocos2d::Value(m_StoryTracker.m_lessonName)},
		  {"category_id", cocos2d::Value(catalogID)},
		  {"category_name", cocos2d::Value(catalogName)},
		  {"read_from",cocos2d::Value(readFrom)},
		  {"total_page", cocos2d::Value(m_StoryTracker.m_totalPage)},
		  {"age", cocos2d::Value(m_StoryTracker.m_userAge)},
		  {"story_author",cocos2d::Value(m_StoryTracker.m_storyAuthor)},
		  {"story_designer", cocos2d::Value(m_StoryTracker.m_storyDesigner)},
		  {"grade_id", cocos2d::Value(m_StoryTracker.m_storyLevel)},
		  {"grade_name", cocos2d::Value(gradeName)},
		  //{"topic", cocos2d::Value(m_StoryTracker.m_storyTopic)},
		  {"completed", cocos2d::Value((int)(xComplete))},
		  {"close_at", cocos2d::Value(currentPageID+1)},
		  {"type_of_learning", cocos2d::Value(typeOfLearning)},
		  {"read_recommended_story", cocos2d::Value(MJDEFAULT->getStringForKey("cl_read_recommended_story","null"))},
          {"interactive_question", cocos2d::Value(_totalAct == 0 ? "No" : "Yes")},
          {"stop_at_page", cocos2d::Value(_numberPage)},
          {"time_spent", cocos2d::Value(int(_timeReadStory))},
          {"turn_previous_page", cocos2d::Value(int(totalTurnPreviousPage))},
            //những event push time bên dưới, để clevertap nhận phải giữ nguyên định dạng
            //và code ở native để push, vui lòng đừng sửa nhé -> nếu convert bằng C++ đúng định dạng ngày giờ hiển thị đc trên clevertap
            // nhưng nó không nhận được => sẽ ko tạo được campaign => chỉ xử lý native.
          {"time_purchased", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_purchased",0)))},
          {"time_activated",cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_activated",0)))},
          {"time_expired", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_expire",0))) }
		};

        if (_totalAct != 0)
        {
            std::pair<std::string, cocos2d::Value> valueCloseInteractiveQuestion;
            valueCloseInteractiveQuestion.first = "skip_interactive_question";
            valueCloseInteractiveQuestion.second = _checkCloseInteractiveQuestion ? "yes" : "no";

            eventTrackingProperties.push_back(valueCloseInteractiveQuestion);
        }

		CleverTapManager::GetInstance()->pushEvent("learn_story", eventTrackingProperties);
        _checkPushEventLearnStroy = false;
        if (_checkClickButtonCloseOfStories)
        {
            std::string readFromTheSecond = MJDEFAULT->getStringForKey("key_read_from_the_second");
            if (readFromTheSecond.find('.') != -1)
            {
                auto listReadFrom = mj::helper::split(readFromTheSecond, '.');
                if (listReadFrom.size() > 0)
                {
                    MJDEFAULT->setStringForKey(key_read_from, listReadFrom[0]);
                }
                MJDEFAULT->deleteValueForKey("key_read_from_the_second");
            }
            _checkClickButtonCloseOfStories = false;
        }
	}
	catch (const char * i_errorCode)
	{
		CCLOG("Error: cannot push story tracking data to server !");
	}
}

void StoryPageVC::addLayerActivityEnd(Node* layerAct)
{
    AUDIO_ENGINE->stopAllAudio();
    auto game = StoryDataModel::getInstance()->getGameById(activities[act_index].sub_id);
    auto image_thurm = utils::findChild<ImageView*>(layerAct, "Image_2");
    auto remoteview = RemoteImageView::createImage(game.thumbnail_url, [=](RemoteImageView* img, bool is_success) {
        img ? img->setContentSize(image_thurm->getContentSize()) : (void) nullptr;
        }, RemoteImageView::Type::OTHER, true);

    //name activity and number activity
    utils::findChild<ui::Text*>(layerAct, "activity_name")->setString(mj::helper::extractActivityName((activities[act_index].name)));
	
	std::string activtyName = LANGUAGE_MANAGER->getDisplayTextByKey("lesson.actipreview.acti") + " %d";

	if (!CONFIG_MANAGER->isVietnameseStories())
	{
		activtyName = "Activity %d";
	}

    utils::findChild<ui::Text*>(layerAct, "activity_order")->setString(StringUtils::format(activtyName.c_str(), activities[act_index].order + 1));
    remoteview->setPosition(image_thurm->getContentSize() / 2);
   // remoteview->setName("image_thurm");
    remoteview->setAnchorPoint(Point(0.5, 0.5));
    image_thurm->addChild(remoteview);

    auto listview_bar = utils::findChild<ListView*>(layerAct, "listview");

    auto progress_csb = utils::findChild<ImageView*>(layerAct, "progress");
    auto img_normal_csb = utils::findChild<ImageView*>(layerAct, "img_normal");
    auto img_current_csb = utils::findChild<ImageView*>(layerAct, "img_current");
    auto img_checked_csb = utils::findChild<ImageView*>(layerAct, "img_checked");

    //add progress
    for (int i = 0; i < activities.size(); i++)
    {
        if (auto text_oder = img_current_csb->getChildByName<Text*>("order"))
        {
            text_oder->setString(StringUtils::toString(i + 1));
        }
        if (auto text_oder = img_normal_csb->getChildByName<Text*>("order"))
        {
            text_oder->setString(StringUtils::toString(i + 1));
        }

        if (i < act_index)
        {
            listview_bar->pushBackCustomItem(img_checked_csb->clone());
        }
        else
        {
            if (i == act_index && act_index == 0)
            {
                listview_bar->pushBackCustomItem(img_normal_csb->clone());
            }
            else
            {
                listview_bar->pushBackCustomItem(img_current_csb->clone());
            }
        }
        if (i < activities.size() - 1)
        {
            listview_bar->pushBackCustomItem(progress_csb->clone());
        }
    }

    if (act_index != 0)
    {
        this->runAction(Sequence::create(CallFunc::create([=] {
            if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
            {
                button_play->setOpacity(0.8 * 255.f);
                button_play->setTouchEnabled(false);
            }
            if (auto btn_skip = utils::findChild<Button*>(layerAct, "button_skip"))
            {
                btn_skip->setOpacity(255 * 0.8);
                btn_skip->setTouchEnabled(false);
            }
            }), DelayTime::create(1.5f), CallFunc::create([=] {
                    if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
                    {
                        button_play->setOpacity(255);
                        button_play->setTouchEnabled(true);
                    }
                    if (auto btn_skip = utils::findChild<Button*>(layerAct, "button_skip"))
                    {
                        btn_skip->setOpacity(255);
                        btn_skip->setTouchEnabled(true);
                    }
                    listview_bar->insertCustomItem(img_normal_csb->clone(), 2*act_index);
                    if (auto item = listview_bar->getItem(2*act_index + 1))
                    {
                        item->removeFromParent();
                        listview_bar->getItems().erase(2*act_index + 1);
                    }

                    if (auto text_oder = listview_bar->getItem(2*act_index)->getChildByName<Text*>("order"))
                    {
                        text_oder->setString(StringUtils::toString(act_index + 1));
                    }
            }), nullptr));
    }

    float x_size_listview_bar = 0.0;
    for (int i = 0; i < listview_bar->getItems().size(); i++)
    {
        if (auto item1 = listview_bar->getItem(i))
        {
            if (auto name_bar = item1->getChildByName<ui::LoadingBar*>("bar"))
            {
                if (i < act_index * 2 - 1)
                {
                    name_bar->setPercent(100);
                }
                else
                {
                    name_bar->setPercent(0);
                }
                if (i ==  act_index * 2 - 1)
                {
                    name_bar->runAction(Sequence::create(DelayTime::create(1.5), EaseQuinticActionOut::create(ActionFloat::create(1, 0, 100, [name_bar](float v) {
                        name_bar->setPercent(v);
                        })), nullptr));
                }
                
            }
            x_size_listview_bar = x_size_listview_bar + item1->getBoundingBox().size.width / 2;
        }

    }

    float x_listview = listview_bar->getPosition().x;
    listview_bar->setPosition(Vec2(listview_bar->getPosition().x + listview_bar->getBoundingBox().size.width / 2 - x_size_listview_bar, listview_bar->getPosition().y));
    listview_bar->setAnchorPoint(Point(0.5, 0.5));
    listview_bar->setScrollBarAutoHideEnabled(false);
    listview_bar->setTouchEnabled(false);

    auto arrow = utils::findChild<ImageView*>(layerAct, "arrow");

    float x_arrow = listview_bar->getPosition().x - listview_bar->getBoundingBox().size.width / 2;
    float x_moveto_arrow = x_arrow;
    if (act_index == 0)
    {
        if (auto item1 = listview_bar->getItem(act_index))
        {
            x_arrow = x_arrow + item1->getBoundingBox().size.width / 2;
        }
    }
    else
    {
        for (int i = 0; i < listview_bar->getItems().size(); i++)
        {
            if (i > act_index * 2) break;
            if (i == act_index * 2)
            {
                if (auto item1 = listview_bar->getItem(i))
                {
                    x_arrow = x_arrow + item1->getBoundingBox().size.width / 2;
                }
            }
            else
            {
                if (auto item1 = listview_bar->getItem(i))
                {
                    x_arrow = x_arrow + item1->getBoundingBox().size.width;
                }
            }
        }
    }


    for (int i = 0; i < listview_bar->getItems().size(); i++)
    {
        if (i > (act_index - 1) * 2) break;
        if (i == (act_index - 1) * 2)
        {
            if (auto item1 = listview_bar->getItem(i))
            {
                x_moveto_arrow = x_moveto_arrow + item1->getBoundingBox().size.width / 2;
            }
        }
        else
        {
            if (auto item1 = listview_bar->getItem(i))
            {
                x_moveto_arrow = x_moveto_arrow + item1->getBoundingBox().size.width;
            }
        }
    }
    if (act_index == 0)
    {

        arrow->setPosition(Vec2(x_arrow, listview_bar->getPosition().y - 35));
    }
    else
    {
        arrow->setPosition(Vec2(x_moveto_arrow, listview_bar->getPosition().y - 35));
    }
    arrow->runAction(Sequence::create(DelayTime::create(1.5), EaseQuinticActionOut::create(MoveTo::create(1, Point(x_arrow, listview_bar->getPosition().y - 35))), nullptr));
   
    //read audio activity
    
    this->runAction(Sequence::create(CallFunc::create([=] {
        if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
        {
            button_play->setOpacity(0.8 * 255.f);
            button_play->setTouchEnabled(false);
        }
        if (auto btn_skip = utils::findChild<Button*>(layerAct, "button_skip"))
        {
            btn_skip->setOpacity(255 * 0.8);
            btn_skip->setTouchEnabled(false);
        }
        readAudioOfActivity();
        }), DelayTime::create(1.0f), CallFunc::create([=] {
            if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
            {
                button_play->setOpacity(255.f);
                button_play->setTouchEnabled(true);
            }
            if (auto btn_skip = utils::findChild<Button*>(layerAct, "button_skip"))
            {
                btn_skip->setOpacity(255.f);
                btn_skip->setTouchEnabled(true);
            }
            clickButtonSkip(layerAct, arrow, listview_bar, img_checked_csb, image_thurm, img_normal_csb);
            //click button Play
            clickButtonPlay(layerAct, listview_bar);
            }), NULL));
    //add and click button Skip
}

void StoryPageVC::clickButtonPlay(Node* layerAct, cocos2d::ui::ListView* listview_bar)
{
	if (auto playButton = utils::findChild<Button*>(layerAct, "button_play"))
	{
        if(CONFIG_MANAGER->isVietnameseStories())
        {
            if (auto playText = dynamic_cast<ui::Text*>(playButton->getChildByName("Text_1"))) {
                
                playText->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lesson.lessonpreview.start"));
                LANGUAGE_MANAGER->setFontByLang(playText, StoryLanguageManager::TextType::BOLD);
                
                if (playText->getContentSize().width > playButton->getContentSize().width)
                {
                    playButton->setContentSize({ playText->getContentSize().width*1.05f,  playButton->getContentSize().height });
                    playText->setPositionX(playButton->getContentSize().width / 2.f);
                }
            }
        }

        playButton->addClickEventListener([=](Ref* sender) {
            // [Thêm mới event] Interactive_question bổ sung thêm properties
            if (_countClickButtonPlayAndSkip < activities.size()) {
                auto game = StoryDataModel::getInstance()->getGameById(activities[_countClickButtonPlayAndSkip].sub_id);
                _countClickButtonPlayAndSkip++;
                _gameNamePlay = _gameNamePlay + game.name + ", ";
            }
            //

            if (auto buttonClosePopupChooseActivity = utils::findChild<cocos2d::ui::Button*>(this, "button_close_popup_activity"))
            {
                buttonClosePopupChooseActivity->setTouchEnabled(false);
                buttonClosePopupChooseActivity->setEnabled(false);
            }

            if (auto closeLayout = utils::findChild<cocos2d::ui::ImageView*>(this, "close_layout_story"))
            {
                closeLayout->stopAllActions();
            }
            this->unschedule("back_to_top");
            if (auto img_nor = listview_bar->getItem(act_index * 2))
            {
                if (img_nor->getName() == "img_normal")
                {
                    _checkClickButtonPlay = true;
                    PROFILE_MANAGER.updateStorieActivity(MJDEFAULT->getIntegerForKey(key_current_profile_id), XHSLib->_id);
                    mm::RealtimeDatabase::getInstance().updateStoriesActivity(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id));
                    _timeStartActivity = int(utils::getTimeInMilliseconds() / 1000);
                    _countNumberClickPlayNow++;
                    if (auto closeButton = utils::findChild(Director::getInstance()->getRunningScene(), "close_layout_story"))
                    {
                        closeButton->removeFromParent();
                    }
                    layerAct->removeFromParent();
                    this->removeChildByName("act_layout");
                    activities[act_index].startFrom = mj::model::ActInfo::START_FROM::START_STORY;
                    auto wrapper = MSActivityWrapper::createWithInfo(activities[act_index]);
                    wrapper->setStartFrom(MSActivityWrapper::START_FROM::END_STORY);
                    wrapper->setName("act_layout");
                    this->addChild(wrapper);
                }
            }
            });
	}

}

void StoryPageVC::clickButtonSkip(Node* layerAct, ImageView* arrow, ListView * listview_bar, ui::ImageView* img_checked_csb, ui::ImageView * image_thurm, ui::ImageView * img_normal_csb)
{
    auto btn_skip = utils::findChild<Button*>(layerAct, "button_skip");
    auto text_skip = utils::findChild<Text*>(layerAct, "Text_Skip");
    
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        text_skip->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.skip"));
        LANGUAGE_MANAGER->setFontByLang(text_skip,StoryLanguageManager::TextType::BOLD);
    }

    
    btn_skip->setTouchEnabled(true);
    btn_skip->addClickEventListener([=](Ref* sender) {
        // [Thêm mới event] Interactive_question bổ sung thêm properties
        if (_countClickButtonPlayAndSkip < activities.size()) {
            auto game = StoryDataModel::getInstance()->getGameById(activities[_countClickButtonPlayAndSkip].sub_id);
            _gameNameSkip = _gameNameSkip + game.name + ", ";
            _countClickButtonPlayAndSkip++;
        }
        //
        
        _countNumberClickSkip++;
        int count_index_size = act_index;
        count_index_size++;
        if (count_index_size >= activities.size())
        {
            btn_skip->setTouchEnabled(false);
        }

		if (auto imageThurm = layerAct->getChildByName("image_thurm"))
		{
			imageThurm->removeFromParent();
		}

        //act được ấn skip sẽ được tích xanh
        listview_bar->insertCustomItem(img_checked_csb->clone(), act_index * 2);
        if (auto item = listview_bar->getItem(act_index * 2 + 1))
        {
            item->removeFromParent();
            listview_bar->getItems().erase(act_index * 2 + 1);
        }
        
        if (count_index_size < activities.size())
        {
            act_index++;
            //Đọc số thứ tự của Act
            readAudioOfActivity();
            if (auto remove_img_thurm = utils::findChild(layerAct, "image_thurm"))
            {
                remove_img_thurm->removeFromParent();
            }
            //Thay đổi ảnh của act khi skip
            auto game = StoryDataModel::getInstance()->getGameById(activities[act_index].sub_id);
            auto remoteview = RemoteImageView::createImage(game.thumbnail_url, [=](RemoteImageView* img, bool is_success) {
                img ? img->setContentSize(image_thurm->getContentSize()) : (void) nullptr;
                }, RemoteImageView::Type::OTHER, true);

			std::string activtyName = LANGUAGE_MANAGER->getDisplayTextByKey("lesson.actipreview.acti") + " %d";

			if (!CONFIG_MANAGER->isVietnameseStories())
			{
				activtyName = "Activity %d";
			}

            utils::findChild<ui::Text*>(layerAct, "activity_name")->setString(mj::helper::extractActivityName((activities[act_index].name)));

			if (auto activtyOrder = utils::findChild<ui::Text*>(layerAct, "activity_order"))
			{
				activtyOrder->setString(StringUtils::format(activtyName.c_str(), activities[act_index].order + 1));
			}

            remoteview->setPosition(image_thurm->getContentSize() / 2);
           // remoteview->setName("image_thurm");
            remoteview->setAnchorPoint(Point(0.5, 0.5));
            image_thurm->addChild(remoteview);

            this->runAction(Sequence::create(CallFunc::create([&] {
                btn_skip->setOpacity(255 * 0.8);
                btn_skip->setTouchEnabled(false);
                if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
                {
                    button_play->setOpacity(0.8 * 255.f);
                    button_play->setTouchEnabled(false);
                }
                }), CallFunc::create([&] {
                if (auto item1 = listview_bar->getItem(act_index * 2 - 1))
                {
                    if (auto name_bar = item1->getChildByName<ui::LoadingBar*>("bar"))
                    {
                        name_bar->runAction(Sequence::create(DelayTime::create(0.25), EaseQuinticActionOut::create(ActionFloat::create(1, 0, 100, [name_bar](float v) {
                            name_bar->setPercent(v);
                            })), nullptr));
                    }
                }
                }),DelayTime::create(1.0f), CallFunc::create([&] {
                    listview_bar->insertCustomItem(img_normal_csb->clone(), act_index * 2);
                    if (auto item = listview_bar->getItem(act_index * 2 + 1))
                    {
                        item->removeFromParent();
                        listview_bar->getItems().erase(act_index * 2 + 1);
                    }

                    if (auto text_oder = listview_bar->getItem(act_index * 2)->getChildByName<Text*>("order"))
                    {
                        text_oder->setString(StringUtils::toString(act_index + 1));
                    }
                    }), nullptr));
            
            //progress
            float x_arrow = arrow->getPosition().x;
            if (auto item = listview_bar->getItem((act_index - 1) * 2))
            {
                x_arrow = x_arrow + item->getBoundingBox().size.width / 2;
            }
            if (auto item = listview_bar->getItem(act_index  * 2))
            {
                x_arrow = x_arrow + item->getBoundingBox().size.width / 2;
            }
            if (auto item = listview_bar->getItem(act_index * 2 - 1))
            {
                x_arrow = x_arrow + item->getBoundingBox().size.width ;
            }
            arrow->runAction(Sequence::create(DelayTime::create(0.25), EaseQuinticActionOut::create(MoveTo::create(1, Point(x_arrow, listview_bar->getPosition().y - 35))),
                 CallFunc::create([&] {
                    btn_skip->setOpacity(255);
                    btn_skip->setTouchEnabled(true);
                    if (auto button_play = utils::findChild<Button*>(layerAct, "button_play"))
                    {
                        button_play->setOpacity(255.f);
                        button_play->setTouchEnabled(true);
                    }
                    }), nullptr));
        }
        else
        {
            if (act_index >= activities.size())
            {
                act_index--;
            }
            //Xóa button close khi click skip
            if (auto close_button = utils::findChild(Director::getInstance()->getRunningScene(), "close_layout_story"))
            {
                close_button->removeFromParent();
            }
           // về màn hình kết thúc truyện
            endActivity();
        }
        });
}

void StoryPageVC::clickButtonClose()
{
    CRASH_AUTO_LOG;
    //Tạo button close
    auto closeLayout = ImageView::create("mjstory/close_game.png");
    closeLayout->setName("close_layout_story");
    this->addChild(closeLayout, 1000000000);
    closeLayout->setPosition(cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height + 50));
    closeLayout->setAnchorPoint(cocos2d::Point::ANCHOR_TOP_RIGHT);

    auto lblClose = Label::createWithTTF("Close?", "fonts/Montserrat-SemiBold.ttf", 16);
    lblClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lblClose->setPosition(Vec2(closeLayout->getContentSize().width / 2, -10));
    lblClose->setTextColor(Color4B(Color3B(38, 176, 230)));
    lblClose->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    closeLayout->addChild(lblClose);
    lblClose->setVisible(false);

    auto button = cocos2d::ui::Button::create("mjstory/papgemenucontrol_btclose.png");
    button->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    button->setPositionX(closeLayout->getContentSize().width / 2);
    button->setPositionY(10);
    button->setLocalZOrder(-1);
    button->setOpacity(0);
    button->setScale(1.5f);
    button->setName("button_close_popup_activity");
    button->addClickEventListener([=](Ref* sender) {
        if (act_index == 0 && !_checkCloseInteractiveQuestion)
        {
            _checkCloseInteractiveQuestion = true;
        }
        if (!close_button_data.onMove){
            if (close_button_data.onBot){
                //this->onGameOver(true);
                close_button_data.onMove = true;
                endActivity();
                
                if (auto closeLayout_story = utils::findChild(Director::getInstance()->getRunningScene(), "close_layout_story"))
                {
                    if(closeLayout_story->getNumberOfRunningActions() > 0)
                    {
                        closeLayout_story->stopAllActions();
                    }
                    closeLayout_story->removeFromParent();
                }
          
                this->unschedule("back_to_top");
            }
            else{
                close_button_data.onMove = true;
                closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height))
                                                        , CallFunc::create([=](){
                    lblClose->setVisible(true);
                    close_button_data.onBot = true;
                    close_button_data.onMove = false;
                    this->scheduleOnce([=](float dt){
                        close_button_data.onMove = true;
                        close_button_data.onBot = false;
                        closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height + 50))
                                                                , CallFunc::create([=](){
                            close_button_data.onMove = false;
                            lblClose->setVisible(false);
                        }), nullptr));
                    }, 2.0f, "back_to_top");
                }), nullptr));
            }
        }
    }
    );
    closeLayout->addChild(button, 1000000000);
}

void StoryPageVC::readAudioOfActivity()
{
	if (!CONFIG_MANAGER->isVietnameseStories()) {

		auto read_activity = cocos2d::experimental::AudioEngine::play2d("sounds/activities_name/activity.mp3");
		cocos2d::experimental::AudioEngine::setFinishCallback(read_activity, [=](int, std::string) {
			auto activity_number = StringUtils::format("sounds/activities_name/%d.mp3", act_index + 1);
			auto read_number = cocos2d::experimental::AudioEngine::play2d(activity_number);
			cocos2d::experimental::AudioEngine::setFinishCallback(read_number, [=](int, std::string) {

				auto name = mj::helper::getActivitySound(activities[act_index].name);
				auto read_name = cocos2d::experimental::AudioEngine::play2d(name);

				cocos2d::experimental::AudioEngine::setFinishCallback(read_name, [=](int, std::string) {

				});

			});
		});
	}
}

void StoryPageVC::endActivity()
{
    if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
    {
        DIRECTOR->popScene([](Scene* scene) {

            scene->scheduleOnce([=](float dt) {
                ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlay();
                }, 1.0f, "delay_continue_story");

            return TransitionSlideInL::create(0.5, scene);
            });
        return;
    }

    if (auto close_button = utils::findChild(Director::getInstance()->getRunningScene(), "close_layout_story"))
    {
        close_button->removeFromParent();
    }
    AUDIO_ENGINE->playSound("theendpage/endingpage_leaf.mp3");
    layout_block_touch->setTouchEnabled(true);
    auto leaf_trans = GAFWrapper::createGAF("gaf/Leaf_trans/Leaf_trans.gaf");
    leaf_trans->setScale(1.5);
    this->addChild(leaf_trans, 1000);
    leaf_trans->setPosition(Vec2(XHSLib->getSizeScreen().width / 2 + 10, XHSLib->getSizeScreen().height / 2));
    leaf_trans->play(false, [=](gaf::GAFObject* obj) {
        leaf_trans->removeFromParent();
        layout_block_touch->setTouchEnabled(false);
        });
    runAirplaneEndStory(ENDGAME_TYPE, false);
}

void StoryPageVC::setTimeUserExitApp()
{
    int timeUserExitApp = MJDEFAULT->getIntegerForKey(key_time_user_exit_app, 0) + int(utils::getTimeInMilliseconds() / 1000);
    MJDEFAULT->setIntegerForKey(key_time_user_exit_app, timeUserExitApp);
}

void StoryPageVC::pushFireBaseInteractiveQuestionActivitySkipped()
{
    //[Thêm mới event] Tạo event mới trên firebase interactive_question_activity_played, interactive_question_activity_skipped, learn_lesson_activity
    MS_LOGGER.logEvent("interactive_question_activity_skipped", {
                {"activity_name",cocos2d::Value(_gameNameSkip)},
                {"story_name",cocos2d::Value(_storyInfo.name)},
                {"story_id",cocos2d::Value(_storyInfo.storyid)}
        });
}
