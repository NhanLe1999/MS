//
//  MSCollectionLessonItem.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/13/17.
//

#include "MSCollectionLessonItem.h"
#include "RemoteImageView.h"
#include "MJAPILessonDetails.h"
#include "MSLessonPreview.h"
#include "MSCollectionLessonItemDelegate.h"
#include "MSLessonManager.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "StoryAlert.h"
#include "AudioEngine.h"
#include "MSLessonManager.h"
#include "GAFWrapper.h"
#include "MSGradeManager.h"
#include "MSTabLessons.h"
#include "MMFirebaseDatabase.h"
#include "APProfileManager.h"
#include "MSDownloader.h"
#include "MsNewDownloadFile.h"
#include "NativeBridge.h"

USING_NS_CC;
INITIALIZE_READER(MSCollectionLessonItem);

#define tag_popup_question_download 1112
#define tag_popup_download_failed   1113

int MSCollectionLessonItem::_lesson_locked_audio_id = -1;

std::once_flag ms_collection_lesson_item;
cocos2d::Data ms_collection_lesson_node_data;
MSCollectionLessonItem* MSCollectionLessonItem::createItem(mj::model::LessonInfo item, bool isLessonLearn) {
    std::call_once(ms_collection_lesson_item, [] {
        REGISTER_CSB_READER(MSCollectionLessonItem);
     //   ms_collection_lesson_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/LessonItem.csb");
       ms_collection_lesson_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/LessonItem_0.csb");
    });
    
    auto node = reinterpret_cast<MSCollectionLessonItem *>(CSLoader::createNode(ms_collection_lesson_node_data));
    if (node) {
        node->didLoadFromCSB(item, isLessonLearn);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSCollectionLessonItem::didLoadFromCSB(mj::model::LessonInfo item, bool isLessonLearn)
{
    _info = item;
    _isLessonLearned = isLessonLearn;
   // _isLessonReady = ms::LessonManager::getInstance().isLessonReady(_info);
    if(auto buttonBeginDownload = utils::findChild(this, "button_begin_download"))
    {
        buttonBeginDownload->setVisible(false);
    }
    utils::findChild(this, "Image_1")->setLocalZOrder(INT_MAX-1);
    utils::findChild(this, "icon_new")->setLocalZOrder(INT_MAX - 1);
    utils::findChild(this, "icon_check")->setLocalZOrder(INT_MAX - 1);
    utils::findChild<ui::Text *>(this, "placeholder")->setString(_info.name);
    changeTextDisplayLang();
    
    auto remoteview = RemoteImageView::createImage(_info.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
        auto image = this->getChildByName("image");
        if (is_success) {
            image->setVisible(false);
        }
        
        auto thumb = this->getChildByName("thumb");
        thumb ? thumb->setContentSize(image->getContentSize()) : (void) nullptr;
        if (thumb) {
            auto img = thumb->getChildByName<ui::ImageView *>("r_image");
            if (img) {
                if (_info.state == mj::model::LessonInfo::LOCKED) {
                    mj::helper::grayscaleImageView(img);
                } else {
                    mj::helper::grayscaleImageView(img, false);
                }
            }
        }
    }, RemoteImageView::Type::STORIES);
    
    auto image = this->getChildByName("image");
    remoteview->setPosition(image->getPosition());
    remoteview->setAnchorPoint(image->getAnchorPoint());
   // remoteview->setName("thumb");
    remoteview->setContentSize(image->getContentSize());
    this->addChild(remoteview);
    
    setLessonState(_info.state);
    
    auto button_delete = ui::Button::create("mjstory/new_ui/button_delete_lesson.png");
    button_delete->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    button_delete->setPosition(Point(190, 250));
    button_delete->addClickEventListener([=](Ref *) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_delete_lesson", &_info);
    });
    button_delete->setVisible(false);
    button_delete->setOpacity(0);
    button_delete->setName("button_delete");
    this->addChild(button_delete, 3);
    _infoActDownload.activity_id = -1;
}

void MSCollectionLessonItem::onEnter()
{
    cocos2d::ui::Layout::onEnter();
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        this->changeTextDisplayLang();
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    auto reload_state = EventListenerCustom::create(key_reload_lesson_state, [=](EventCustom * e) {
        auto newest_state = ms::LessonManager::getInstance().getLessonById(_info.lesson_id);
        if (this->_info.state != newest_state.state) {
            this->_info = newest_state;
            this->setLessonState(newest_state.state);
        }
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(reload_state, this);
}

void MSCollectionLessonItem::addFirstActivityProgressListener() {
    if (_activities.empty())
    {
        _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
    }
    if (_activities.size() <= 0)
        return;
    if(_infoActDownload.activity_id == -1)
    {
        _infoActDownload = ms::LessonManager::getInstance().getActivityBeginDownload(_activities, ms::LessonManager::getInstance().isPhonicLesson(_info), _info.lesson_id, _info.category_id);
    }

    auto first_activity = _infoActDownload.activity_id != -1 ? _infoActDownload : _activities[0];
    auto listener_first_activity_download = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventActivityProgress(first_activity), [=](EventCustom * e) {
        auto progress = reinterpret_cast<double *>(e->getUserData());

        if(*progress > _precent)
        {
            _precent = *progress;
        }

        utils::findChild<ui::LoadingBar *>(this, "bar")->setPercent(_precent);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_first_activity_download, this);
    
    auto listener_lesson_download_ready = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventLessonReady(_info), [this](EventCustom *) {
        this->onLessonReady();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_ready, this);
}


void MSCollectionLessonItem::setLessonState(mj::model::LessonInfo::State state) {
    
    bool isButtonRelearnVisible = false;
    CCLOG("%d___%s", _info.lesson_id, std::to_string(state).c_str());
    
    if(auto button = utils::findChild(this, "button_relearn"))
    {
        isButtonRelearnVisible = button->isVisible();
    }
    // if MS && Lessons-Comprehension && state is locked --> unlock lesson
    if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(_info) && state == mj::model::LessonInfo::State::LOCKED) {
        state = mj::model::LessonInfo::State::UNLOCKED;
    }
    
 //   bool isPhonic = MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false);
    bool isPhonic = false;
    
    if (state == mj::model::LessonInfo::State::UNLOCKED) {
        if (ms::LessonManager::getInstance().isLessonReady(_info)) {
            state = mj::model::LessonInfo::State::READY;
        }
    }

    if (_info.state == mj::model::LessonInfo::State::LEARNED) {
        utils::findChild(this, "icon_check")->setVisible(true);
        _info.state = state;
    } else {
        _info.state = state;
        utils::findChild(this, "icon_check")->setVisible(_info.state == mj::model::LessonInfo::LEARNED);
        if (_info.state == mj::model::LessonInfo::State::UNLOCKED) {
            if ((utils::getTimeInMilliseconds() / 1000) - _info.time_published < 3600 * 24 * 7 * 2) {
                utils::findChild(this, "icon_new")->setVisible(true);
            } else {
                utils::findChild(this, "icon_new")->setVisible(false);
            }
        }
    }
    
    utils::findChild(this, "mask")->setVisible(_info.state != mj::model::LessonInfo::READY && _info.state != mj::model::LessonInfo::UNLOCKED);
    utils::findChild(this, "mask_active")->setVisible(_info.state == mj::model::LessonInfo::UNLOCKED);
    
    auto is_last = ms::LessonManager::getInstance().getLessonShouldLearn(_info.category_id) == _info.lesson_id;

    if (is_last) {
        if (auto hand_gaf = cocos2d::utils::findChild(Director::getInstance()->getRunningScene(), "gaf_xxx_yyy")) {
            hand_gaf->removeFromParent();
        }
        
        if (MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false))
        {
            if (_info.state == mj::model::LessonInfo::READY || _info.state == mj::model::LessonInfo::UNLOCKED) {

                utils::findChild<ui::Text*>(this, "name")->setOpacity(255);
            }
            auto gaf = GAFWrapper::createGAF("gaf/eff_swf_full/eff_swf_full.gaf");
            gaf->setName("gaf_xxx_yyy");
            gaf->setAnchorPoint(Point(0.5, 0.5));
            gaf->setScale(1);
            gaf->setPosition(Point(200, -40));
            this->addChild(gaf, INT_MAX - 1);
            gaf->setOpacity(0);
            gaf->runAction(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node* n) {
                reinterpret_cast<GAFWrapper*>(n)->play(true);
                }), DelayTime::create(1.25), NULL));

            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_play_locked_lesson, [this](EventCustom* e) {

                }), this);
        }
        else if(!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(_info))
        {
              // thêm bàn tay
                auto gaf = GAFWrapper::createGAF("gaf/hand_tap_2/hand_tap_2.gaf");
                gaf->setName("gaf_xxx_yyy");
                gaf->setAnchorPoint(Point(0.5, 0.5));
                gaf->setScale(1);
                gaf->setPosition(Point(200, -40));
                this->addChild(gaf, INT_MAX - 1);
                gaf->setOpacity(0);
                gaf->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), CallFuncN::create([gaf](Node* n) {
					gaf->play(true);
                })));
                std::string key = StringUtils::format(key_last_lesson_x, _info.category_id);
                int profileID = ap::ProfileManager::getInstance().getCurrentProfileId();
                mm::RealtimeDatabase::getInstance().updateProfileKeysByKeyAndValue(profileID, key, _info.lesson_id);
        }

        // code cũ
        utils::findChild(this, "icon_new")->setVisible(false);
       this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_play_locked_lesson, [this](EventCustom * e) {
           if (this->getChildByName("gaf_xxx_yyy")) {
               return;
           }
           auto gaf = GAFWrapper::createGAF("gaf/hand_tap/hand_tap.gaf");
           gaf->setName("gaf_xxx_yyy");
           gaf->setPosition(Point(150, 25));
           this->addChild(gaf, INT_MAX - 1);
           gaf->setOpacity(0);
           gaf->runAction(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node * n) {
               reinterpret_cast<GAFWrapper *>(n)->play(true);
           }), DelayTime::create(1.25), FadeOut::create(0.25), RemoveSelf::create(), NULL));
       }), this);

       utils::findChild(this, "mask_active")->setVisible(false);
       utils::findChild(this, "mask")->setVisible(false);
            
    } else {
        
        if (auto gaf = this->getChildByName("gaf_xxx_yyy")) {
            gaf->removeFromParent();

        }

		if (auto maskActive = utils::findChild(this, "mask_active")){
			maskActive->setVisible(_info.state == mj::model::LessonInfo::UNLOCKED || _info.state == mj::model::LessonInfo::READY);
		}

		if (auto maskActive = utils::findChild(this, "mask")) {
			maskActive->setVisible(_info.state != mj::model::LessonInfo::READY && _info.state != mj::model::LessonInfo::UNLOCKED);
		}
    }
    
	if (auto lockIcon = utils::findChild(this, "icon_lock"))
	{
		lockIcon->setVisible(_info.state == mj::model::LessonInfo::LOCKED);
		lockIcon->setOpacity(100);
	}
    if (_isLessonLearned)
    {
        _info.state = mj::model::LessonInfo::LEARNED;
    }
    utils::findChild(this, "button_locked")->setVisible(_info.state == mj::model::LessonInfo::LOCKED);
    utils::findChild(this, "button_relearn")->setVisible(_info.state == mj::model::LessonInfo::LEARNED);
    utils::findChild(this, "button_learn")->setVisible(_info.state == mj::model::LessonInfo::READY);
    utils::findChild(this, "download_progress")->setVisible(_info.state == mj::model::LessonInfo::UNLOCKED);
    if (_info.state == mj::model::LessonInfo::LEARNED && isPhonic)
    {
        utils::findChild(this, "button_relearn")->setOpacity(utils::findChild(this, "button_relearn")->getOpacity() * 0.7);
    }
    if (CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild(this, "icon_new")->setVisible(false);
    }
    auto name = utils::findChild<ui::Text *>(this, "name");
    if (_info.state == mj::model::LessonInfo::LOCKED) {
        name->setTextColor(Color4B(65, 65, 65, 255));
    } else {
            name->setTextColor(Color4B::BLACK);
    }
    
    auto thumb = utils::findChild(this, "thumb");
    if (thumb) {
        auto img = thumb->getChildByName<ui::ImageView *>("r_image");
        if (img) {
            if (state == mj::model::LessonInfo::LOCKED || _info.state == mj::model::LessonInfo::LEARNED) {
                mj::helper::grayscaleImageView(img);
                img->setOpacity(100);
            } else {
                mj::helper::grayscaleImageView(img, false);
                img->setOpacity(255);
            }
        }
    }
    

    if (state == mj::model::LessonInfo::LOCKED) {
        utils::findChild(this, "mask_active")->setVisible(true);
        utils::findChild(this, "mask")->setVisible(false);
        utils::findChild<ui::Text*>(this, "name")->setTextColor(Color4B(mj::helper::colorFromHex(0xcccccc)));
    }
    else
    {
        utils::findChild(this, "mask_active")->setVisible(false);
        utils::findChild(this, "mask")->setVisible(true);
    }
}

void MSCollectionLessonItem::onExit()
{
    this->unschedule("checkDownloadState");
    cocos2d::ui::Layout::onExit();

    if (_storyAlert) {
    
        _storyAlert->setDelegate(nullptr);
    }
}

void MSCollectionLessonItem::onLearn(Ref * sender) {

	mj::helper::playButtonFX();

	if (!ms::LessonManager::getInstance().isLessonReady(_info)) {
		onDownload(nullptr);
		return;
	}
    ms::LessonManager::getInstance().setInforCurrenLesson(_info);
    _delegate ? _delegate->onLearnLesson(_info) : (void) nullptr;
}

void MSCollectionLessonItem::onRelearn(Ref *sender) {
    mj::helper::playButtonFX();
    if (CONFIG_MANAGER->isAppActive()) {
        //preview
        if (ms::LessonManager::getInstance().isLessonReady(_info)) {
            this->getEventDispatcher()->removeCustomEventListeners("event_lesson_auto_open_from_qr");
            onLearn(nullptr);
            return;
        }
        onDownload(nullptr);
    } else {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.feature.free"), "OK", "");
    }
}

void MSCollectionLessonItem::onLocked(cocos2d::Ref *sender) {
    mj::helper::playButtonFX();
    
    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.popup.prior"), "OK", "");
    }
    
    if (_lesson_locked_audio_id == -1)
    {
        auto path = CONFIG_MANAGER->isVietnameseStories()?"sounds/ui/lesson_guide_vi.mp3":"sounds/ui/lesson_lock.mp3";
        _lesson_locked_audio_id = cocos2d::experimental::AudioEngine::play2d(path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_lesson_locked_audio_id, [](int, std::string) {
            MSCollectionLessonItem::_lesson_locked_audio_id = -1;
        });
    }
    _delegate ? _delegate->onTryToLearnLockedLessons(_info) : (void) nullptr;
}

void MSCollectionLessonItem::onDownload(Ref * sender) {
    if(!NativeBridge::networkAvaiable())
    {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        _storyAlert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
        _storyAlert->setNetworkDownContent(false, true);
        _storyAlert->show();
        _storyAlert->setTag(tag_popup_download_failed);
        _storyAlert->setDelegate(nullptr);
        return;
    }
    
    if(mj::helper::showPopupWarningFullDisk())
    {
        return;
    }

    if (auto bar = utils::findChild<ui::LoadingBar*>(this, "bar_0")) {
        bar->setPercent(100);
    }
    
    MJDEFAULT->setStringForKey(key_live_in_screen, "lesson");
    ms::utils::ThumbQueue::getInstance().unDownloadImage();
    ms::utils::Downloader::getInstance().unEnqueue();
    
    mj::helper::playButtonFX();
    utils::findChild(this, "download_progress")->setVisible(true);
    utils::findChild(this, "download_progress")->setLocalZOrder(1);
    
    if (_activities.empty())
    {
        _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
    }

    /*
    * Add listener for progressbar here
    */
    if (_activities.empty()) {
        auto listener_details = EventListenerCustom::create(StringUtils::format("%s_%d", key_event_got_lesson_details, _info.lesson_id), [=](EventCustom * e) {
            this->addFirstActivityProgressListener();
        });
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_details, this);
    } else {
        this->addFirstActivityProgressListener();
    }
    /*
    * Listener for error while downloading
    */
    auto listener_lesson_download_error = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventLessonError(_info), [this](EventCustom *) {
        this->onLessonError();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_error, this);
    
    if (ms::LessonManager::getInstance().isLessonReady(_info)) {
        onLessonReady();
    } else {
        _is_downloading = true;
        if(_activities.empty()){
            auto info = _info;
         
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    
                        //hiện pop up hỏi download
                        if (_activities.empty())
                        {
                            _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
                        }
                        auto size = getLessonSize(_activities);
                        int fontSize = 28.f * (CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f);
                        const std::string& text_ok = CONFIG_MANAGER->isVietnameseStories() ? LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button") : "OK";
                        auto alert = StoryAlert::showAlert(StringUtils::format("%s%0.2fMB.\n%s", LANGUAGE_MANAGER->getDisplayTextByKey("download.content.size").c_str(), size, LANGUAGE_MANAGER->getDisplayTextByKey("download.confirm").c_str()), text_ok, "", false, fontSize, this);
                        alert->setTag(tag_popup_question_download);
                   
                });
            
        }else{
            //hiện pop up hỏi download
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

                    if (_isSkipPopDowload)
                    {
                        mj::helper::showWaitView();
                        ms::LessonManager::getInstance().prepareLesson(_info);
                        _isSkipPopDowload = false;
                    }
                    else {

                        //download và chuẩn bị bài học

                        if (!MJDEFAULT->getBoolForKey(key_is_submit, false))
                        {
                            ms::NewDownload::getInstance().clearListDownLoadStory();                      ms::LessonManager::getInstance().resetListUrlError();
                            ms::LessonManager::getInstance().setInforCurrenLesson(_info);
                           ms::LessonManager::getInstance().prepareLesson(_info);
                       }else{

                           int nu = cocos2d::random(1, 4);

                           float size = float(nu) + nu / 3.0f;

                           int fontSize = 28.f * (CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f);
                           const std::string& text_ok = CONFIG_MANAGER->isVietnameseStories() ? LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button") : "OK";
                           auto alert = StoryAlert::showAlert(StringUtils::format("%s %0.2fMB.\n%s", LANGUAGE_MANAGER->getDisplayTextByKey("download.content.size").c_str(), size, LANGUAGE_MANAGER->getDisplayTextByKey("download.confirm").c_str()), text_ok, "", false, fontSize, this);
                           alert->setTag(tag_popup_question_download);
                        }
                    }
                });
        }
    }
}

void MSCollectionLessonItem::onAlertOK(cocos2d::Ref *sender) {
    if (auto alert = dynamic_cast<StoryAlert *>(sender))
    {
        switch (alert->getTag())
        {
            case tag_popup_question_download:
            {
                //download và chuẩn bị bài học
                ms::NewDownload::getInstance().clearListDownLoadStory();
                ms::LessonManager::getInstance().resetListUrlError();
                ms::LessonManager::getInstance().setInforCurrenLesson(_info);
                ms::LessonManager::getInstance().prepareLesson(_info);
            }
                break;
                
            default:
                break;
        }
    }
}

void MSCollectionLessonItem::onAlertClose(cocos2d::Ref* sender)
{
    if (auto alert = dynamic_cast<StoryAlert*>(sender))
    {
        if (alert->getTag() == tag_popup_download_failed)
        {
            ms::LessonManager::getInstance().resetIndexDownload();
            ms::LessonManager::getInstance().resetListUrlError();
            _is_downloading = false;
        }
    }
}

void MSCollectionLessonItem::onDownloadWithoutAlert()
{
    if (!NativeBridge::networkAvaiable())
    {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        _storyAlert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
        _storyAlert->setNetworkDownContent(false, true);
        _storyAlert->show();
        _storyAlert->setTag(tag_popup_download_failed);
        _storyAlert->setDelegate(nullptr);
        return;
    }

    if (mj::helper::showPopupWarningFullDisk())
    {
        return;
    }

    MJDEFAULT->setStringForKey(key_live_in_screen, "lesson");
    ms::utils::ThumbQueue::getInstance().unDownloadImage();
    ms::utils::Downloader::getInstance().unEnqueue();

    mj::helper::playButtonFX();
    utils::findChild(this, "download_progress")->setVisible(true);
    utils::findChild(this, "download_progress")->setLocalZOrder(1);

    if (_activities.empty())
    {
        _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
    }

    /*
    * Add listener for progressbar here
    */
    if (_activities.empty()) {
        auto listener_details = EventListenerCustom::create(StringUtils::format("%s_%d", key_event_got_lesson_details, _info.lesson_id), [=](EventCustom* e) {
            this->addFirstActivityProgressListener();
            });

        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_details, this);
    }
    else {
        this->addFirstActivityProgressListener();
    }

    /*
    * Listener for error while downloading
    */
    auto listener_lesson_download_error = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventLessonError(_info), [this](EventCustom*) {
        this->onLessonError();
        });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_error, this);

    ms::NewDownload::getInstance().clearListDownLoadStory();
    ms::LessonManager::getInstance().resetListUrlError();
    ms::LessonManager::getInstance().setInforCurrenLesson(_info);
    ms::LessonManager::getInstance().prepareLesson(_info);
}

void MSCollectionLessonItem::onAlertCancel(cocos2d::Ref *sender) {
    if (auto alert = dynamic_cast<StoryAlert *>(sender))
    {
        switch (alert->getTag())
        {
            case tag_popup_question_download:
            {
                //đổi trạng thái thanh download
               // utils::findChild<ui::Text *>(this, "text_download")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.item.download"));
                _is_downloading = false;
            }
                break;
                
            case tag_popup_download_failed:
            {
                // re-download
                _countClickRetry++;
                ms::LessonManager::getInstance().setCountClickRetry(_countClickRetry);
                ms::LessonManager::getInstance().resetIndexDownload();
                _isSkipPopDowload = true;
                this->onDownload(nullptr);
            }
                break;
                
            default:
                break;
        }
    }
}

float MSCollectionLessonItem::getLessonSize(std::vector<mj::model::ActInfo> acts){
    float size = 0;
    for(auto act: acts){
        size += StoryDataModel::getInstance()->getFileSize(act.data_zip);
    }
    
    return size;
}

ui::Widget::ccWidgetClickCallback MSCollectionLessonItem::onLocateClickCallback(const std::string & callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSCollectionLessonItem, onLearn),
        CLICK_MAP(MSCollectionLessonItem, onDownload),
        CLICK_MAP(MSCollectionLessonItem, onRelearn),
        CLICK_MAP(MSCollectionLessonItem, onLocked)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSCollectionLessonItem::onLessonReady()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        this->getEventDispatcher()->dispatchCustomEvent("event_lesson_auto_open_from_qr");
        
        this->getEventDispatcher()->removeCustomEventListeners(ms::LessonManager::getInstance().getKeyEventLessonError(_info));
        this->getEventDispatcher()->removeCustomEventListeners(ms::LessonManager::getInstance().getKeyEventLessonReady(_info));
        mj::model::LessonInfo::State state;
        if (this->_info.state == mj::model::LessonInfo::UNLOCKED || this->_info.state == mj::model::LessonInfo::State::LEARNED)
            state = mj::model::LessonInfo::READY;
        else
            state = _info.state;
        setLessonState(state);
        
        if (_info.state != mj::model::LessonInfo::LOCKED) {
            if (_info.is_learned) {
             //   utils::findChild(this, "button_relearn")->setVisible(true);
                utils::findChild(this, "button_learn")->setVisible(false);
                utils::findChild(this, "button_begin_download")->setVisible(false);
            } else {
                utils::findChild(this, "button_learn")->setVisible(true);
                utils::findChild(this, "button_relearn")->setVisible(false);
            }
            utils::findChild(this, "download_progress")->setVisible(false);
        }
        utils::findChild(this, "download_progress")->setVisible(false);
    
    if (_learnWhenReady)
    {
        mj::helper::deleteWaitView();
        if (ms::LessonManager::getInstance().isLessonReady(_info)) {
            onLearn(nullptr); 
        }
    }
        
        if(auto buttonBeginDownload = utils::findChild(this, "button_begin_download"))
        {
            buttonBeginDownload->setVisible(false);
        }
    });
    //utils::findChild<ui::Text *>(this, "text_download")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.item.download"));
}

void MSCollectionLessonItem::onLessonError()
{
    _is_downloading = false;
    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonManager::getInstance().getKeyEventLessonError(_info));
    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonManager::getInstance().getKeyEventLessonReady(_info));

    if (_activities.empty())
    {
        _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
    }

    if (_activities.empty()) {
    } else {
        if(_infoActDownload.activity_id == -1)
        {
            _infoActDownload = ms::LessonManager::getInstance().getActivityBeginDownload(_activities, ms::LessonManager::getInstance().isPhonicLesson(_info), _info.lesson_id, _info.category_id);
        }

        auto first_activity = _infoActDownload.activity_id != -1 ? _infoActDownload : _activities[0];
        this->getEventDispatcher()->removeCustomEventListeners(ms::LessonManager::getInstance().getKeyEventActivityProgress(_infoActDownload));
    }
    
    if (ms::LessonManager::getInstance().isLessonReady(_info)) {
        //không hiện thông báo khi lesson ready
        return;
    }
    
    if (_info.state == mj::model::LessonInfo::State::LOCKED) {
        //không hiện thông báo nếu lesson locked
        return;
    }
//    if (_info.state == mj::model::LessonInfo::State::UNLOCKED) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
            _is_downloading = false;
            auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
            _storyAlert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
            _storyAlert->setNetworkDownContent(false, true);
            _storyAlert->show();
            _storyAlert->setTag(tag_popup_download_failed);
            _storyAlert->setDelegate(this);
        });
//    }
    
   // utils::findChild<ui::Text *>(this, "text_download")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.item.download"));
    utils::findChild<ui::LoadingBar *>(this, "bar")->setPercent(0);
}

void MSCollectionLessonItem::changeTextDisplayLang()
{
    if(auto text = utils::findChild<ui::Text *>(this, "name"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-SemiBold.ttf");
        
        text->setString(StringUtils::format("%s %d", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.item.title").c_str(), _info.lesson_order));

    }
}


void MSCollectionLessonItem::setDisplayMode()
{
    utils::findChild(this, "control")->setVisible(false);
    auto name_grade = utils::findChild<ui::Text *>(this, "name_grade");
    name_grade->setVisible(true);
    name_grade->setString(ms::GradeManager::getInstance().getNameByLanguage(ms::GradeManager::getInstance().getGradeInfoById(_info.category_id).name, LANGUAGE_MANAGER->getDisplayLangId()));
    
    utils::findChild(this, "mask_active")->setVisible(true);
    utils::findChild(this, "mask")->setVisible(false);
}

void MSCollectionLessonItem::enableDeleteMode()
{
    for (auto c : this->getChildren()) {
        if (c->getName() != "button_delete")
            c->setOpacity(0x77);
    }
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setOpacity(0xff);
    button_delete->setVisible(true);
    button_delete->runAction(FadeIn::create(0.1));
}

void MSCollectionLessonItem::disableDeleteMode()
{
    for (auto c : this->getChildren()) {
        c->setOpacity(0xff);
    }
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setVisible(false);
}

void MSCollectionLessonItem::openLessonItemForce()
{
    auto listener_lesson_auto_open = EventListenerCustom::create("event_lesson_auto_open_from_qr", [this](EventCustom *) {
        onLearn(nullptr);
        this->getEventDispatcher()->removeCustomEventListeners("event_lesson_auto_open_from_qr");
    });
    
    switch (_info.state) {
        case mj::model::LessonInfo::State::READY: {
            onLearn(nullptr);
        } break;
            
        case mj::model::LessonInfo::State::LEARNED: {
            onRelearn(nullptr);
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
        } break;
            
        case mj::model::LessonInfo::State::LOCKED: {
            onLocked(nullptr);
        } break;
            
        case mj::model::LessonInfo::State::UNLOCKED: {
            onDownload(nullptr);
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
        } break;
            
        case mj::model::LessonInfo::State::PREPARING: {
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
        } break;
            
        default:
            break;
    }
}

void MSCollectionLessonItem::setLastLesson()
{
   
}

void MSCollectionLessonItem::downloadingLesson(bool learnWhenReady)
{
    if (ms::LessonManager::getInstance().isLessonReady(_info)) {
        onLearn(nullptr);
    }
    else
    {
        _isSkipPopDowload = learnWhenReady;
        _learnWhenReady = learnWhenReady;
        onDownload(nullptr);
    }


}

