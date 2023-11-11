/////////////////////////////////////////////////////////////
////  MSTabAISpeaking_cpp
////  Implementation of the Class MSTabAISpeaking_cpp
////  Created on:      21-04-2022
////  Original author: TruongBX
/////////////////////////////////////////////////////////////
//
//#include "MSTabAISpeaking.h"
//#include "MSCollectionAiSpeakingLessonItem.h"
//#include "MSComplexCollection.h"
//#include "StoryDataModel.h"
//#include "MSActivityPreview.h"
//#include "MSLessonPreview.h"
//#include "MJDefinitions.h"
//#include "MSLessonAISpeakingManager.h"
//#include "MSActivityEnd.h"
////#include "MSActivityWrapper.h"
//#include "MJPlatformConfig.h"
//#include "StoryLanguageManager.h"
//#include "MSEventLogger.h"
//#include "StoryAlert.h"
//#include "MMFirebaseDatabase.h"
//#include "HelperManager.h"
//#include "MSSidebar.h"
//#include "APProfileManager.h"
//#include "AISpeakingFakeCallScreen.h"
//#include "AISpeakingRelearnSplashScreen.h"
//
//#define category_view_limit 7
//#define category_view_duration 3
///*
// // * 1. chưa tải, bấm tải -> hiện button học
// // * 2. bấm học -> vào popup giới thiệu activity (đọc, không có play, tự động mất) -> vào học.
// // * 3. Popup giới thiệu (bỏ lesson, phóng to activity, đọc activity 1, learn vocab...)
// // * 4. Nếu đang học dở, sẽ vào trang activities
// // * 5. Bấm vào từng activity, sẽ tương tự khi ấn nút học
// // * 6. Chuyển từ activity->activity, hiện nút play
// // * 7. Xong bài học, quay lại trang bài học
// * 8. Close
// */
//
//
//USING_NS_CC;
//INITIALIZE_READER(MSTabAISpeaking);
//
//std::once_flag ms_tab_ai_speaking_reader;
//
//MSTabAISpeaking * MSTabAISpeaking::createTab(MSHome* home, ms::GradeManager::Scope scope)
//{
//    std::call_once(ms_tab_ai_speaking_reader, []
//    {
//        REGISTER_CSB_READER(MSTabAISpeaking);
//    });
//
//    auto csb_name = mj::helper::getCSBName("csb/new_ui/AISpeaking/MSTabAISpeakings.csb");
//
//    auto tab = reinterpret_cast<MSTabAISpeaking *>(CSLoader::createNode(csb_name));
//
//    if (tab)
//    {
//        tab->didLoadFromCSB(home, scope);
//        return tab;
//    }
//
//    CC_SAFE_DELETE(tab);
//    return nullptr;
//}
//
//static std::pair<int, int> s_gradeIDsCache = { -1,-1 };
//
//int MSTabAISpeaking::countCompletedLessons() {
//
//    std::vector<int> listIdOfLessonsLearned = getLessonLearnedfromProfile();
//    int count = 0;
//
//    for (auto aiSpeakingLessons : _aiSpeakingLessonsAllGrades) {
//        for (int i = 0; i < aiSpeakingLessons.size(); i++) {
//            if (aiSpeakingLessons[i].is_learned) {
//                count++;
//            }
//            else
//            {
//                if (checkLessonLearned(aiSpeakingLessons[i].lesson_id, listIdOfLessonsLearned))
//                {
//                    count++;
//                }
//            }
//        }
//    }
//
//    return count;
//}
//
//std::vector <std::vector<mj::model::AISpeakingLessonInfo>> MSTabAISpeaking::getAiSpeakingLessonsAllGradesByCategory()
//{
//    if (_aISpeakingCategoryInfos.size() <= 0)
//    {
//        _aISpeakingCategoryInfos = StoryDataModel::getInstance()->getAllAiSpeakingTopic();
//        std::sort(_aISpeakingCategoryInfos.begin(), _aISpeakingCategoryInfos.end(), [](mj::model::AISpeakingCategoryInfo& a, mj::model::AISpeakingCategoryInfo& b) {
//            return a.order_by < b.order_by;
//            });;
//    }
//
//    if (_gradleIDs.size() <= 0)
//    {
//        for (auto aISpeakingCategoryInfo : _aISpeakingCategoryInfos) {
//            _gradleIDs.push_back(aISpeakingCategoryInfo.topic_id);
//        }
//    }
//
//    if (_aiSpeakingLessonsAllGrades.size()<=0) {
//        for (int i = 0; i<_gradleIDs.size(); i++)
//        {
//            int gradeId = _gradleIDs.at(i);
//            std::vector<mj::model::AISpeakingLessonInfo> aISpeakingLessonInfo = ms::LessonAISpeakingManager::getInstance().getAllLessonsByGrade(gradeId);
//            if(aISpeakingLessonInfo.size()<=0)
//            {
//                _gradleIDs.erase(_gradleIDs.begin()+i);
//                i--;
//                continue;
//
//            }
//            std::sort(aISpeakingLessonInfo.begin(), aISpeakingLessonInfo.end(), [](mj::model::AISpeakingLessonInfo& a, mj::model::AISpeakingLessonInfo& b) {
//                return a.lesson_order < b.lesson_order;
//                });;
//
//            // handle update lesson state from ProfileDB and firebase
//            aISpeakingLessonInfo = ms::LessonAISpeakingManager::getInstance().getAllLessonAISpeakingStateFromDB(aISpeakingLessonInfo);
//
//            _aiSpeakingLessonsAllGrades.push_back(aISpeakingLessonInfo);
//        }
//
//    }
//    return _aiSpeakingLessonsAllGrades;
//
//}
//
//void MSTabAISpeaking::InitialListViewTopicCollection(std::vector<std::vector<mj::model::AISpeakingLessonInfo>> aiSpeakingLessonsAllGrades)
//{
//
//    ClearView();
//    if(MSCollectionAiSpeakingLessonItem::_isDownloadingCommonVideo == true)
//    {
//        return;
//    }
//    if (!_panel_1)
//    {
//        _panel_1 = utils::findChild<Layout*>(this, "Panel_1_0");
//    }
//
//    if (!_listViewTopics) {
//        _listViewTopics = utils::findChild<ListView*>(this, "speakingTopicList");
//        _listViewTopics->setScrollBarEnabled(false);
//        _listViewTopics->setTouchEnabled(true);
//        _listViewTopics->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(MSTabAISpeaking::selectedTopicItemEventScrollView, this));
//    }
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//
//            if (aiSpeakingLessonsAllGrades.size() > category_view_limit)
//            {
//                listViewStartPostion = 0;
//
//                for (int i = listViewStartPostion; i < category_view_limit; i++)
//                {
//                    auto aiSpeakingLessonsAllGrade = aiSpeakingLessonsAllGrades.at(i);
//                    if (aiSpeakingLessonsAllGrade.size() <= 0)
//                        continue;
//                    mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo = StoryDataModel::getInstance()->getAiSpeakingTopicById(aiSpeakingLessonsAllGrade.at(0).category_id);
//                    AISpeakingTopic* aISpeakingTopic = creatListViewLessonList(aiSpeakingLessonsAllGrade, aISpeakingCategoryInfo);
//                    this->scheduleOnce([=](float)
//                        {
//                            //_listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                        }, 0.3f + DELAY_CREAT_LESSON* aISpeakingTopic->getLessonSize(), "addLesssonToListView");
//                    //_listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                    _listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                }
//            }
//            else
//            {
//                listViewStartPostion = 0;
//                for (auto aiSpeakingLessonsAllGrade : aiSpeakingLessonsAllGrades) {
//                    if (aiSpeakingLessonsAllGrade.size() <= 0)
//                        continue;
//                    mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo = StoryDataModel::getInstance()->getAiSpeakingTopicById(aiSpeakingLessonsAllGrade.at(0).category_id);
//                    AISpeakingTopic* aISpeakingTopic = creatListViewLessonList(aiSpeakingLessonsAllGrade, aISpeakingCategoryInfo);
//                    this->scheduleOnce([=](float)
//                        {
//                            //_listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                        }, 0.3f + DELAY_CREAT_LESSON * aISpeakingTopic->getLessonSize(), "addLesssonToListView");
//                     _listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                }
//            }
//            _listViewTopics->forceDoLayout();
//        });
//
//}
//
//
//AISpeakingTopic* MSTabAISpeaking::creatListViewLessonList(std::vector<mj::model::AISpeakingLessonInfo> _aiSpeakingLessons, mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo)
//{
//
//
//    if (CONFIG_MANAGER->isAppActive()){
//        _isLockTopic = false;
//    }
//    else
//    {
//        _isLockTopic = false;
//      if(_countForUserExpectedInAllTopic>=2)
//          _isLockTopic = true;
//    }
//    AISpeakingTopic* aISpeakingTopic = AISpeakingTopic::createTab(aISpeakingCategoryInfo, _aiSpeakingLessons, getLessonLearnedfromProfile(), _isLockTopic);
//    aISpeakingTopic->setContentSize(Size(this->getContentSize().width, this->getContentSize().height / 2.2));
//    aISpeakingTopic->setDelegate(this);
//
//    _countForUserExpectedInAllTopic++;
//    return aISpeakingTopic;
//}
//
//void MSTabAISpeaking::UpdateListViewCollection(std::vector<std::vector<mj::model::AISpeakingLessonInfo>> _aiSpeakingLessonsAllGrades, int oldListViewIndex, int newListViewIndex)
//{
//
//}
//
//void MSTabAISpeaking::UpdateDataView(int index, int durationChangeIndex)
//{
//
//}
//
//
//void MSTabAISpeaking::ClearView()
//{
//    if(MSCollectionAiSpeakingLessonItem::_isDownloadingCommonVideo == true)
//    {
//        if(_listViewTopics)
//            _listViewTopics->requestDoLayout();
//        return;
//
//    }
//    if(_listViewTopics)
//        _listViewTopics->removeAllChildren();
//    _collectionsLayout.clear();
//    _collections.clear();
//}
//
//void MSTabAISpeaking::clearUnusedContents()
//{
//
//    ClearView();
//}
//
//void MSTabAISpeaking::selectedTopicItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
//{
//    if (category_view_limit >= _aiSpeakingLessonsAllGrades.size())
//        return;
//    if (_listViewTopics->getItems().size() <= 0)
//        return;
//    _listViewSize = _listViewTopics->getItems().size();
//    //ListView* listView = static_cast<ListView*>(pSender);
//    if (_listViewSize < 7)
//        return;
//
//    switch (type) {
//    case ui::ScrollView::EventType::SCROLL_TO_RIGHT:
//        {
//            AISpeakingTopic* aISpeakingTopic =  static_cast<AISpeakingTopic*>(_listViewTopics->getItem(_listViewTopics->getCurSelectedIndex()));
//            aISpeakingTopic->mouseScroll("", aISpeakingTopic->getListViewLesson(), 50.0f, 1.0f, true);
//            break;
//        }
//    case ui::ScrollView::EventType::SCROLL_TO_LEFT:
//        {
//            AISpeakingTopic* aISpeakingTopic =  static_cast<AISpeakingTopic*>(_listViewTopics->getItem(_listViewTopics->getCurSelectedIndex()));
//            aISpeakingTopic->mouseScroll("", aISpeakingTopic->getListViewLesson(), -50.0f, 1.0f, true);
//            break;
//        }
//    case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
//    case ui::ScrollView::EventType::BOUNCE_BOTTOM:
//    case ui::ScrollView::EventType::SCROLLING_ENDED:
//    {
//        selectIndex = _listViewTopics->getCurSelectedIndex();
//        if(selectIndex < _listViewSize - 4)
//            break;
//        if (_aiSpeakingLessonsAllGrades.size() > _listViewSize + category_view_duration)
//        {
//
//            int listViewSize  = _listViewSize++;
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//                std::vector<AISpeakingTopic*> aISpeakingTopics;
//                for (int i = 0; i <= category_view_duration; i++)
//                {
//
//                    int indexItemAdd = _listViewSize;
//                    if (indexItemAdd >= _aiSpeakingLessonsAllGrades.size())
//                    {
//                        break;
//                    }
//                    else {
//                        CCLOG("_aiSpeakingLessonsAllGrades size = %d , indexItemAdd = %d", _aiSpeakingLessonsAllGrades.size(), indexItemAdd);
//                        auto aiSpeakingLessonsAllGrade = _aiSpeakingLessonsAllGrades.at(indexItemAdd);
//                        if (aiSpeakingLessonsAllGrade.size() > 0)
//                        {
//                            mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo = StoryDataModel::getInstance()->getAiSpeakingTopicById(aiSpeakingLessonsAllGrade.at(0).category_id);
//                            AISpeakingTopic* lastAISpeakingTopicInListView =  static_cast<AISpeakingTopic*>(_listViewTopics->getItem(_listViewTopics->getItems().size()-1));
//                            _listViewSize++;
//                            if(lastAISpeakingTopicInListView->getTopicId() == aiSpeakingLessonsAllGrade.at(0).category_id)
//                            {
//                                continue;
//                            }
//                            AISpeakingTopic* aISpeakingTopic = creatListViewLessonList(aiSpeakingLessonsAllGrade, aISpeakingCategoryInfo);
//                            aISpeakingTopics.push_back(aISpeakingTopic);
//                        }
//                    }
//
//                }
//                for (AISpeakingTopic* aISpeakingTopic: aISpeakingTopics)
//                {
//                    aISpeakingTopic->setVisible(false);
//                    _listViewTopics->pushBackCustomItem(aISpeakingTopic);
//                    aISpeakingTopic->runAction(Sequence::create(DelayTime::create(0.05f),
//                        CallFunc::create([=] {
//                            aISpeakingTopic->setVisible(true);
//                            }),
//                        NULL));;
//                }
//                int per = selectIndex *100 / (listViewSize + category_view_duration);
//                _listViewTopics->jumpToPercentVertical(per);
//
//                });
//        }
//
//        break;
//    }
//
//    default:
//        break;
//    }
//
//}
//
//void MSTabAISpeaking::didLoadFromCSB(MSHome* home, ms::GradeManager::Scope scope)
//{
//    _mshome = home;
//
//    if (CONFIG_MANAGER->isVietnameseStories()){
//        ms::GradeManager::getInstance().setCurrentGradeId(764);
//    }
//
//    auto layout_block_touch = ui::Layout::create();
//    layout_block_touch->setContentSize(Director::getInstance()->getVisibleSize());
//    layout_block_touch->setTouchEnabled(true);
//    layout_block_touch->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
//    layout_block_touch->setPosition(Point::ZERO);
//    layout_block_touch->setName("layout_block_touch");
//    this->addChild(layout_block_touch, INT_MAX);
//
//    this->setClippingEnabled(false);
//
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_reload_lesson_state, [this](EventCustom * e)
//    {
//       // this->reloadLessons();
//    }), this);
//
//    this->scheduleOnce([=](float)
//    {
//           utils::findChild<Layout*>(this, "layout_block_touch")->removeFromParent();
//    }, 1.0f, "remove_block_touch");
//}
//
//bool MSTabAISpeaking::checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned)
//{
//    for(int i = 0; i < listIdOfLessonsLearned.size(); i ++)
//    {
//        if (id == listIdOfLessonsLearned[i])
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//vector<int> MSTabAISpeaking::getLessonLearnedfromProfile()
//{
//    return _lessonIdsLearnedFromProfile;
//}
//
//void MSTabAISpeaking::setLessonLearedfromProfile(vector<int> lessonLeared)
//{
//     _lessonIdsLearnedFromProfile = lessonLeared;
//}
//
//int MSTabAISpeaking::getCurrentSelectTopicIndex()
//{
//    return _listViewTopics->getCurSelectedIndex();
//}
//
//int MSTabAISpeaking::getCurrentSelectLessonInTopicIndex()
//{
//    AISpeakingTopic* aISpeakingTopic =  static_cast<AISpeakingTopic*>(_listViewTopics->getItem(_listViewTopics->getCurSelectedIndex()));
//    int index = aISpeakingTopic->getListViewLesson()->getCurSelectedIndex();
//    return index;
//}
//
//
//void MSTabAISpeaking::onEnter()
//{
//    CRASH_AUTO_LOG;
//    Layout::onEnter();
//}
//
//void MSTabAISpeaking::onExit(){
//    Layout::onExit();
//}
//
//void MSTabAISpeaking::onContinueLearn(mj::model::ActInfo info) {
//
//    if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
//        tmp->removeFromParent();
//    }
//
//    if (auto wrapper = MSActivityWrapper::createWithInfo(info)) {
//        wrapper->setName("wrapper");
//        Director::getInstance()->getRunningScene()->addChild(wrapper);
//        wrapper->setVisible(false);
//    }
//
//};
//
//
//void MSTabAISpeaking::setContentSize(const Size & size) {
//    ui::Layout::setContentSize(size);
//    ui::Helper::doLayout(this);
//
//
//    auto collection_layout = utils::findChild(this, "collectionLayout");
//    if (collection_layout) {
//        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));
//    }
//
//    if (_listViewTopics) {
//        _listViewTopics->setContentSize(Size(size.width - 100, _listViewTopics->getContentSize().height));
//    }
//}
//
//
//#define MSTabAISpeaking_tag_open_lesson_from_qr    2051
//
//void MSTabAISpeaking::onReloadAISpeakingPage()
//{
//    if (CONFIG_MANAGER->isAutoPlay()) {
//        _callbackAutoPlay = [this]() {
//
//                //học bài học tiếp theo trong grade
//                auto curentLesson = ms::LessonAISpeakingManager::getInstance().getCurrentLesson();
//        };
//    }
//    setUnlockTopic();
//    // -- phải reload lessons và lesson items ở onEnter này vì nếu act cuối là story thì scene cũ chứa tabLessons sẽ được gọi lại
//    _aiSpeakingLessonsAllGrades = getAiSpeakingLessonsAllGradesByCategory();
//    setLessonLearedfromProfile(PROFILE_MANAGER.getAllLessonAISpeakingLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0)));
//    InitialListViewTopicCollection(_aiSpeakingLessonsAllGrades);
//    creatAiSpeakingFolders();
//}
//
//void MSTabAISpeaking::showPermissionPopup()
//{
//
//    if (!checkPermissionCameraAndAudio())
//    {
//        MSAISpeakingPermissionPopup* mSAISpeakingPermissionPopup = MSAISpeakingPermissionPopup::show(this, this, INT_MAX - 1);
//    }
//
//
//}
//void MSTabAISpeaking::requestPermissionCameraAndAudio()
//{
//    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)){
//#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
//        bool firstCheckPermission = MJDEFAULT->getBoolForKey(FIRST_CHECK_AISPEKING_PERMISSION, true);
//        CameraHelper::GetInstance()->authorization(firstCheckPermission);
//        MJDEFAULT->setBoolForKey(FIRST_CHECK_AISPEKING_PERMISSION, false);
//#endif
//    }
//
//}
//bool MSTabAISpeaking::checkPermissionCameraAndAudio()
//{
//if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
//#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
//    return CameraHelper::GetInstance()->getAuthorizationStatus();
//#endif
//    return true;
//}
//
//void MSTabAISpeaking::onAccept(cocos2d::Ref* sender)
//{
//    pushEventAIspeakingToCleaverTap(true);
//    requestPermissionCameraAndAudio();
//    this->scheduleOnce([=](float dt) {
//        HSAudioEngine::resetAudioEngine();
//    }, 0.1f, "resetAudioAfterGrantPermission");
//
//}
//
//void MSTabAISpeaking::onClose(cocos2d::Ref* sender)
//{
//    pushEventAIspeakingToCleaverTap(false);
//}
//
//void MSTabAISpeaking::onSavelessonLearnAfterLearned(mj::model::AISpeakingLessonInfo lessonInfo)
//{
//    bool isLearned = checkLessonLearned(lessonInfo.lesson_id, getLessonLearnedfromProfile());
////    if(isLearned)
////        return;
//    ms::LessonAISpeakingManager::getInstance().onSavelessonLearnAfterLearned(lessonInfo);
//    onUpdateTopicHaveLessonIteamLearned(lessonInfo);
//    updateLessonLeared(lessonInfo.lesson_id);
//}
//
//void MSTabAISpeaking::onAlertOK(cocos2d::Ref * sender)
//{
//    auto alert = (StoryAlert *)sender;
//    if (alert->getTag() == MSTabAISpeaking_tag_open_lesson_from_qr)
//    {
//    }
//}
//
//void MSTabAISpeaking::onAlertCancel(cocos2d::Ref * sender)
//{
//    // do nothing
//}
//
//
//void MSTabAISpeaking::onUpdateTopicHaveLessonIteamLearned(mj::model::AISpeakingLessonInfo lessonInfo)
//{
//    for (auto items : _listViewTopics->getItems()) {
//        AISpeakingTopic* topicList = static_cast<AISpeakingTopic*>(items);
//        if (topicList->getCategoryInfo().topic_id == lessonInfo.category_id)
//        {
//            int index = 0;
//            for (auto item : topicList->getListViewLesson()->getItems())
//            {
//                MSCollectionAiSpeakingLessonItem * lessonItemview = static_cast<MSCollectionAiSpeakingLessonItem*>(item);
//                if (lessonItemview->getLessonAISpeakingInfo().lesson_id == lessonInfo.lesson_id)
//                {
//                    lessonItemview->setLessonStateToView(mj::model::AISpeakingLessonInfo::State::LEARNED);
//                    topicList->setNumberLessonLearned(topicList->getNumberLessonLearned() + 1);
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//                    //return;
//#endif
//                    if (index < topicList->getListViewLesson()->getItems().size() - 1)
//                    {
//                        MSCollectionAiSpeakingLessonItem* itemNext = static_cast<MSCollectionAiSpeakingLessonItem*>(topicList->getListViewLesson()->getItem(index + 1));
//                        if(itemNext->getStateInfo()!= mj::model::AISpeakingLessonInfo::State::READY &&
//                           itemNext->getStateInfo() != mj::model::AISpeakingLessonInfo::State::LEARNED)
//                            itemNext->setLessonStateToView(mj::model::AISpeakingLessonInfo::State::UNLOCKED);
//                        if (index >= 2 && APPURCHASE_MANAGER->getTypeUser() == IAP::user::TYPE_USERS::USER_EXPRIED)
//                        {
//                            itemNext->setLessonStateToView(mj::model::AISpeakingLessonInfo::State::LOCKED);
//                        }
//                    }
//                }
//                index++;
//            }
//        }
//    }
//
//}
//
//bool MSTabAISpeaking::creatAiSpeakingFolders()
//{
//    bool resultTotal = false;
//    std::vector<std::string> aiSpeakingPath;
//    aiSpeakingPath.push_back(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING);
//    aiSpeakingPath.push_back(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING);
//    aiSpeakingPath.push_back(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING);
//    aiSpeakingPath.push_back(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/");
//    aiSpeakingPath.push_back(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING+ "audio/");
//
//    for (std::string path : aiSpeakingPath)
//    {
//        bool result_creatFile = false;
//        if (!FileUtils::getInstance()->isDirectoryExist(path))
//        {
//            result_creatFile = FileUtils::getInstance()->createDirectory(path);
//            if (!result_creatFile)
//                CCLOG("Create folder false: %s", path.c_str());
//            else
//            {
//                CCLOG("Create folder successful: %s", path.c_str());
//            }
//
//        }
//        resultTotal = resultTotal & result_creatFile;
//    }
//    return resultTotal;
//}
//
//void MSTabAISpeaking::onLearnLesson(mj::model::AISpeakingLessonInfo lesson)
//{
//    if (CONFIG_MANAGER->isVietnameseStories()) {
//
//        return;
//    }
//    HSAudioEngine::resetAudioEngine();
//    HSAudioEngine::getInstance();
//    mj::helper::showWaitView();
//
//    MSActivityWrapper::preSetupBeforeStartGame();
//
//    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
//
//    if (!activities.empty()) {
//
//        auto activityInfo = activities.front();
//
//        auto game_info = StoryDataModel::getInstance()->getGameById(activityInfo.sub_id);
//
//        std::vector<std::pair<std::string, std::string>> unzip_data;
//
//        activityInfo.zip_urls.erase(unique(activityInfo.zip_urls.begin(), activityInfo.zip_urls.end()), activityInfo.zip_urls.end());
//
//        for (auto i = 0; i < activityInfo.zip_urls.size(); ++i)
//        {
//            auto path = FileUtils::getInstance()->getWritablePath() + "cached/zip/" + mj::helper::getFilenameFromURL(activityInfo.zip_urls[i]);
//            auto directory = FileUtils::getInstance()->getWritablePath() + "extracted/" + StringUtils::toString(i / 2) + "/";
//            unzip_data.push_back(std::make_pair(path, directory));
//        }
//
//        mj::helper::unzipAsync(unzip_data, [=](bool success) {
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//
//                if(success){
//                    if (unzip_data.size() > 0)
//
//                    {
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//                        bool isLearned = checkLessonLearned(lesson.lesson_id, getLessonLearnedfromProfile());
//                        //if(MJDEFAULT->getBoolForKey(AI_SPEAKING_FIRST_TIME,true))
//                        if(!isLearned)
//                        {
//
//                            auto screen = AISpeakingFakeCallScreen::createView(AISpeakingFakeCallScreen::AISpeakingFakeCallScreenType::START_CALLING_SCREEN, lesson, {}, this);
//                            Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
//                            MJDEFAULT->setBoolForKey(key_is_relearn_talking_lesson, false);
//                        }
//                        else
//                        {
//                            auto screen = AISpeakingRelearnSplashScreen::createView(AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::LISTEN_AND_REPEAT,lesson, this);
//                            Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
//                            MJDEFAULT->setBoolForKey(key_is_relearn_talking_lesson, true);
//                        }
//
//                        mj::helper::deleteWaitView();
//#endif
//                    }
//
//                }
//            });
//       });
//    }
//    else
//    {
//        mj::helper::deleteWaitView();
//    }
//}
//
//void MSTabAISpeaking::onActivityEnd(Ref * sender, ms::delegate::Activity::EndReason reason) {
//
//    CRASH_AUTO_LOG;
//
//}
//bool MSTabAISpeaking::onDeleteData()
//{
//    return false;
//}
//
//void MSTabAISpeaking::updateLessonLeared(int lessonLeared) {
//    if (std::find(_lessonIdsLearnedFromProfile.begin(), _lessonIdsLearnedFromProfile.end(),lessonLeared) ==_lessonIdsLearnedFromProfile.end())
//        _lessonIdsLearnedFromProfile.push_back(lessonLeared);
//}
//
//void MSTabAISpeaking::showSubscribePopup(){
//
//    CRASH_AUTO_LOG;
//    subscirbe_popup = this->pauseGame(true, StorySubscribePopup::SUB_POPUP_TYPE::SUB_AI_SPEAKING_LESSON);
//
//}
//
//void MSTabAISpeaking::onSubscribeOk() {
//    CRASH_AUTO_LOG;
//    MJDEFAULT->setStringForKey(key_source_subscribe, "unlock_talk");
//    StorySubscribePopupDelegate::onSubscribeOk();
//    XHSLib->isEndingPageClick = false;
//    XHSLib->fontNameStory = "";
//    XHSLib->isPopupSubscribeNow = true;
//    MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, true);
//    if(subscirbe_popup)
//        subscirbe_popup->removeFromParent();
//}
//
//void MSTabAISpeaking::onSubscribeCancel() {
//    CRASH_AUTO_LOG;
//
//    StorySubscribePopupDelegate::onSubscribeCancel();
//    if(subscirbe_popup)
//        subscirbe_popup->removeFromParent();
//}
//
//void MSTabAISpeaking::onSubscribeFree() {
//
//    StorySubscribePopupDelegate::onSubscribeFree();
//
//    XHSLib->isEndingPageClick = false;
//    XHSLib->fontNameStory = "";
//    XHSLib->isPopupSubscribeNow = true;
//    MJDEFAULT->setBoolForKey(key_event_go_to_free_page, true);
//    if(subscirbe_popup)
//        subscirbe_popup->removeFromParent();
//}
//
//void MSTabAISpeaking::setUnlockTopic() {
//    _isLockTopic = false;
//    _countForUserExpectedInAllTopic = 0;
//}
//
//void MSTabAISpeaking::onVerticalScrollView(int index)
//{
//
//}
//void MSTabAISpeaking::pushEventAIspeakingToCleaverTap(bool allow)
//{
//
//    CleverTapManager::GetInstance()->pushEvent("alk_share_video_popup", {
//                        {"allow",cocos2d::Value(allow)}
//    , });
//}
//
//
////// >
