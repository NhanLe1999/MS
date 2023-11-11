/////////////////////////////////////////////////////////////
////  AISpeakingTopic.cpp
////  Implementation of the Class AISpeakingTopic.cpp
////  Created on:      21-04-2022
////  Original author: TruongBX
/////////////////////////////////////////////////////////////
//
//#include "AISpeakingTopic.h"
//#include "MJDefinitions.h"
//#include "MSTabAISpeaking.h"
//#include "StoryLanguageManager.h"
//#include "StoryDataModel.h"
//#include "MSHome.h"
//#include "platform/CCPlatformConfig.h"
//#include "MSActivityEnd.h"
//#include "APPurchaseManager.h"
//
//INITIALIZE_READER(AISpeakingTopic);
//
///*
// * 1. New, Popular
// * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
// */
//
//std::once_flag ms_ai_speaking_topic_reader;
//
//AISpeakingTopic::AISpeakingTopic(){
//
//}
//
//AISpeakingTopic::~AISpeakingTopic(){
//
//}
//
//AISpeakingTopic* AISpeakingTopic::createTab(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo, std::vector<mj::model::AISpeakingLessonInfo> aiSpeakingLessons, std::vector < int >lessonIdsLearnedFromProfile, bool isLockTopic)
//{
//	std::call_once(ms_ai_speaking_topic_reader, []
//		{
//			REGISTER_CSB_READER(AISpeakingTopic);
//		});
//
//	auto csbName = mj::helper::getCSBName("csb/new_ui/AISpeaking/AISpeakingOneTopic.csb");
//
//	auto node = reinterpret_cast<AISpeakingTopic*>(CSLoader::createNode(csbName));
//
//	if (node)
//	{
//		node->didLoadFromCSB(aISpeakingCategoryInfo, aiSpeakingLessons, lessonIdsLearnedFromProfile, isLockTopic);
//		return node;
//	}
//
//	CC_SAFE_DELETE(node);
//	return nullptr;
//}
//
//void AISpeakingTopic::showPermissionPopup()
//{
//	_delegate ? _delegate->showPermissionPopup() : (void) nullptr;
//}
//
//void AISpeakingTopic::onUpdateTopicProccess()
//{
//	SetInfoViewTopic(_aISpeakingCategoryInfo);
//}
//
//void AISpeakingTopic::didLoadFromCSB(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo, std::vector<mj::model::AISpeakingLessonInfo> aiSpeakingLessons, std::vector < int >lessonIdsLearnedFromProfile, bool isLockTopic)
//{
//	_aISpeakingCategoryInfo = aISpeakingCategoryInfo;
//	_aiSpeakingLessons = aiSpeakingLessons;
//
//	_scheduleName = cocos2d::StringUtils::format("waitingDownloadIcon_%d", getCategoryInfo().topic_id);
//    setLockTopic(isLockTopic);
//	auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom*) {
//		this->changeTextDisplayLang();
//		});
//	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
//	setLessonLearedfromProfile(lessonIdsLearnedFromProfile);
//	SetInfoViewTopic(aISpeakingCategoryInfo);
//}
//
//
//int AISpeakingTopic::getLearnedProccessing(){
//
//	return _learnProccessing;
//}
//
//
//std::vector<int> AISpeakingTopic::getSpeakingIDList(){
//
//	return  aiSpeakingItemsIDList;
//}
//
//
//std::string AISpeakingTopic::getThumbPath(){
//
//	return  topicThumbPath;
//}
//
//
//std::string AISpeakingTopic::getTopicColorCode(){
//
//	return  topicColorCode;
//}
//
//
//int AISpeakingTopic::getTopicId(){
//
//	return alSpeakingTopicId;
//}
//
//
//std::string AISpeakingTopic::getTopicName(){
//
//	return  aiSpeakingTopicName;
//}
//
//
//int AISpeakingTopic::getTopicOrder(){
//
//	return aiSpeakingTopicOrder;
//}
//
//
//void AISpeakingTopic::setLearnedProccessing(int learnedProccessing){
//	this->_learnProccessing = learnedProccessing;
//	setUpdateTopicProcess("", learnedProccessing);
//}
//
//
//void AISpeakingTopic::setSpeakingIDList(std::vector<int> speakingIDList){
//	this->aiSpeakingItemsIDList = speakingIDList;
//}
//
//
//void AISpeakingTopic::setThumbPath(std::string thumbPath){
//	this->topicThumbPath = thumbPath;
//}
//
//
//void AISpeakingTopic::setTopicColorCode(std::string topicColorCode){
//	this->topicColorCode = topicColorCode;
//}
//
//
//void AISpeakingTopic::setTopicId(int topicId){
//	this->alSpeakingTopicId = topicId;
//}
//
//
//void AISpeakingTopic::setTopicName(std::string topicName){
//	this->aiSpeakingTopicName = topicName;
//}
//
//
//void AISpeakingTopic::setTopicOrder(int topicOrder){
//	this->aiSpeakingTopicOrder = topicOrder;
//}
//
//void AISpeakingTopic::setNumberLessonLearned(int numberLessonLearned)
//{
//	_numberLessonLearned = numberLessonLearned;
//	float topicProccess = float(_numberLessonLearned * 100 / getLessonSize());
//	setLearnedProccessing(topicProccess);
//    if(_numberLessonLearned +1 == _aiSpeakingLessons.size())
//        setIsTopicLearned(true);
//}
//
//int AISpeakingTopic::getNumberLessonLearned()
//{
//	return _numberLessonLearned;
//}
//
//void AISpeakingTopic::clearUnusedContents()
//{
//}
//
//void AISpeakingTopic::SetInfoViewTopic(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo)
//{
//	_panel_avata = utils::findChild<ui::Layout*>(this, "panel_avata");
//	_cirlceProccessLayout = utils::findChild<ui::Layout*>(this, "cirlceProccessLayout");
//	_avataIconLayout = utils::findChild<ui::Layout*>(this, "avataIconLayout");
//	if (_panel_avata)
//	{
//		_panel_avata->setScale(0.8);
//	}
//	Layout* collectionLayout = utils::findChild<Layout*>(this, "collection");
//	collectionLayout->setOpacity(0);
//	collectionLayout->setVisible(false);
//	_title = utils::findChild<ui::Text*>(this, "title");
//	if (_title)
//	{
//		_title->setString(aISpeakingCategoryInfo.topic_name);
//		_title->setTextVerticalAlignment(TextVAlignment::CENTER);
//
//	}
//	setUpListView();
//	onDowloadAvata(aISpeakingCategoryInfo.iconInfo);
//
//}
//
//void AISpeakingTopic::setAvataTopicInfo(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo)
//{
//    if(_isExitTopic)
//        return;
//
//	if (!_selectedCircleImage && _cirlceProccessLayout && _panel_avata)
//	{
//		_selectedCircleImage = RemoteImageView::createImage(aISpeakingCategoryIconInfo.selected_circle_icon, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::AI_SPEAKING);
//		_cirlceProccessLayout->getParent()->addChild(_selectedCircleImage);
//		_selectedCircleImage->setContentSize(_cirlceProccessLayout->getContentSize() * 0.8);
//		_selectedCircleImage->setPosition(_cirlceProccessLayout->getPosition());
//		_selectedCircleImage->setZOrder(_cirlceProccessLayout->getZOrder()+1);
//		_selectedCircleImage->setVisible(true);
//	}
//	if (!_avataIconImg && _panel_avata && _cirlceProccessLayout)
//	{
//		_avataIconImg = RemoteImageView::createImage(aISpeakingCategoryIconInfo.icon_thumb, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::AI_SPEAKING);
//		_avataIconLayout->getParent()->addChild(_avataIconImg);
//		_avataIconImg->setContentSize(_cirlceProccessLayout->getContentSize() * 0.8);
//		_avataIconImg->setPosition(_avataIconLayout->getPosition());
//		_avataIconImg->setZOrder(_cirlceProccessLayout->getZOrder());
//		_avataIconImg->setVisible(true);
//	}
//	cirlceProccessImageName = aISpeakingCategoryIconInfo.selected_circle_icon;
//	setUpdateTopicProcess(aISpeakingCategoryIconInfo.selected_circle_icon, getLearnedProccessing());
//}
//
//void AISpeakingTopic::setUpListView()
//{
//	_speakingItemsList = utils::findChild<ui::ListView*>(this, "speakingLessonList");
//	_speakingItemsList->setTouchEnabled(false);
//	_speakingItemsList->setPropagateTouchEvents(true);
//	_speakingItemsList->setSwallowTouches(false);
//	_speakingItemsList->setScrollBarEnabled(false);
//	Layout* collectionLayout = utils::findChild<Layout*>(this, "collection");
//	ListView* listViewLesson = this->getListViewLesson();
//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//		int learnedLessonNumber = 0;
//		mj::model::AISpeakingLessonInfo::State stateOfPreLesson = mj::model::AISpeakingLessonInfo::LEARNED;
//		if (getLockTopic())
//			stateOfPreLesson = mj::model::AISpeakingLessonInfo::LOCKED;
//		int index = 0;
//		float scaleSize = 0, scaleSizeHeight = 0, scaleSizeWidth = 0;
//		// only fille only 3 lesson item in with and fill 2 item in height
//		int sizeTopic = this->getContentSize().width;
//		int newLessonWidth = (this->getContentSize().width - listViewLesson->getItemsMargin() * 3- 80) / 3;
//		int newLessonHeight = this->getContentSize().height * 0.7;
//		int margin = 0, oldLessonWidth = 0;
//		for (auto aiSpeakingLesson : _aiSpeakingLessons) {
//			bool isLearned = checkLessonLearned(aiSpeakingLesson.lesson_id, getLessonLearnedfromProfile());
//			if (isLearned)
//				learnedLessonNumber++;
//			aiSpeakingLesson._stateOfPreLesson = stateOfPreLesson;
//			aiSpeakingLesson.is_learned = isLearned;
//			auto lesson = MSCollectionAiSpeakingLessonItem::createItem(aiSpeakingLesson, aiSpeakingLesson.is_learned);
//			if(oldLessonWidth == 0)
//				oldLessonWidth = lesson->getContentSize().width;
//			stateOfPreLesson = lesson->getStateInfo();
//
//            // handler lock/ unclock lesson of Topic
//            if(getLockTopic())
//            {
//                stateOfPreLesson = mj::model::AISpeakingLessonInfo::LOCKED;
//            }
//            else
//            {
//                if(CONFIG_MANAGER->isAppActive())
//                {
//                    stateOfPreLesson = mj::model::AISpeakingLessonInfo::LOCKED;
//                    if(isLearned)
//                        stateOfPreLesson = mj::model::AISpeakingLessonInfo::LEARNED;
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//    if (MJDEFAULT->getBoolForKey("mjdk_full_features", false))
//    {
//        stateOfPreLesson = mj::model::AISpeakingLessonInfo::LEARNED;
//    }
//
//#endif
//
//                }
//                else
//                {
//                    stateOfPreLesson = mj::model::AISpeakingLessonInfo::LEARNED;
//                    if(_countForUserExpectedInOneTopic >= 0)
//                        stateOfPreLesson = mj::model::AISpeakingLessonInfo::LOCKED;
//                }
//            }
//			if (scaleSizeHeight == 0)
//			{
//				scaleSizeHeight = newLessonHeight / lesson->getContentSize().height;
//			}
//			if (scaleSizeWidth == 0)
//			{
//				scaleSizeWidth = newLessonWidth / lesson->getContentSize().width;
//			}
//
//			if (scaleSize == 0)
//			{
//				scaleSize = scaleSizeHeight < scaleSizeWidth ? scaleSizeHeight : scaleSizeWidth;
//			}
//			lesson->setScale(scaleSize);
//
//			//lesson->onResize(Size(newLessonWidth, newLessonHeight));
//			//int lessonWith = lesson->getContentSize().width;
//			lesson->setTouchEnabled(true);
//			lesson->setDelegate(this->getDelegate());
//			this->getListViewLesson()->pushBackCustomItem(lesson);
//			index++;
//			_countForUserExpectedInOneTopic++;
//		}
//		if (margin == 0)
//		{
//			margin = (this->getContentSize().width - oldLessonWidth * scaleSize * 3) / 3;
//		}
//		setNumberLessonLearned(learnedLessonNumber);
//		listViewLesson->setItemsMargin(margin);
//		listViewLesson->setLeftPadding(margin / 2);
//		listViewLesson->forceDoLayout();
//		});
//}
//void AISpeakingTopic::selectedTopicItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
//{
//	switch (type) {
//	case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
//	{
//		_delegate ? _delegate->onVerticalScrollView(1) : (void) nullptr;
//
//		break;
//	}
//	case ui::ScrollView::EventType::SCROLL_TO_TOP:
//	{
//		_delegate ? _delegate->onVerticalScrollView(-1) : (void) nullptr;
//		break;
//	}
//
//	default:
//		break;
//	}
//
//}
//
//vector<int> AISpeakingTopic::getLessonLearnedfromProfile()
//{
//	return _lessonIdsLearnedFromProfile;
//}
//
//void AISpeakingTopic::setLessonLearedfromProfile(vector<int> lessonLeared)
//{
//	 _lessonIdsLearnedFromProfile = lessonLeared;
//}
//
//void AISpeakingTopic::onExit(){
//    _isExitTopic = true;
//	this->unschedule(_scheduleName);
//    Layout::onExit();
//}
//void AISpeakingTopic::onDowloadAvata(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo)
//{
//	bool thumbsExits = IsTopicThumbExits(aISpeakingCategoryIconInfo);
//	if (!thumbsExits)
//	{
//		std::vector <mj::api::ThumbDownloadInfo*> thumbDownloadInfos;
//		std::vector <std::string> download_url_thumbs;
//		download_url_thumbs.push_back(_aISpeakingCategoryInfo.icon_zip_url);
//		for (std::string download_url_thumb : download_url_thumbs)
//		{
//			if (mj::helper::isValidURL(download_url_thumb)) {
//				mj::api::ThumbDownloadInfo* thumbDownloadInfo = new mj::api::ThumbDownloadInfo{};
//				thumbDownloadInfo->downloadUrl = download_url_thumb;
//				thumbDownloadInfo->countRetryDownload = 0;
//				thumbDownloadInfos.push_back(thumbDownloadInfo);
//			}
//		}
//
//		mj::api::FisrtInstallThumb::call(thumbDownloadInfos, [=](bool isSuccess, int, int, int) {
//			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//            if(isSuccess)
//				_isDownloadTopicIconSuceesss = true;
//				});
//			}, nullptr, RemoteImageView::Type::AI_SPEAKING);
//
//		this->schedule([=](float) {
//			if (_isDownloadTopicIconSuceesss && !_isExitTopic)
//			{
//				setAvataTopicInfo(aISpeakingCategoryIconInfo);
//				this->unschedule(_scheduleName);
//			}
//
//
//			}, 1.0f, CC_REPEAT_FOREVER, 0.0f, _scheduleName);
//	}
//	else
//	{
//		setAvataTopicInfo(aISpeakingCategoryIconInfo);
//	}
//
//}
//void AISpeakingTopic::onSelectCollectionItem(cocos2d::ui::Widget* item)
//{
//}
//
//void AISpeakingTopic::reloadData()
//{
//}
//
//void AISpeakingTopic::changeTextDisplayLang()
//{
//}
//
//void AISpeakingTopic::setTopicState()
//{
//}
//
//void AISpeakingTopic::setUpdateTopicProcess(std::string cirlceProccess, int percent = 0)
//{
//    if(_isExitTopic)
//        return;
//
//	if(cirlceProccess!= "")
//		cirlceProccessImageName = mj::helper::getFilenameFromURL(cirlceProccess);
//
//	std::string	_cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/" + cirlceProccessImageName;
//
//	if (!FileUtils::getInstance()->isFileExist(_cached_image))
//		return;
//	if (!_cirlceProccessLayout)
//	{
//		return;
//	}
//	if (_progcess == nullptr)
//	{
//		auto bar = Sprite::create(_cached_image);
//        if(!bar){
//            _progcess = nullptr;
//            return;
//        }
//		bar->setContentSize(Size(_cirlceProccessLayout->getContentSize()));
//		_progcess = ProgressTimer::create(bar);
//		_progcess->setType(ProgressTimer::Type::RADIAL);
//		_progcess->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		_progcess->setPosition(_cirlceProccessLayout->getContentSize() / 2);
//		_progcess->setVisible(true);
//		_progcess->setName("ProgressTimer");
//		_cirlceProccessLayout->addChild(_progcess);
//	}
//    if(percent>0 && percent<=100)
//        _progcess->setPercentage(percent);
//    else if(percent>100)
//    {
//        _progcess->setPercentage(100);
//
//    }
//    else
//        _progcess->setPercentage(0);
//
//}
//
//cocos2d::Size AISpeakingTopic::collectionSize()
//{
//	return _speakingItemsList->getContentSize();
//}
//
//void AISpeakingTopic::offLoadCellAt(int index)
//{
//}
//
//void AISpeakingTopic::onLearnLesson(mj::model::AISpeakingLessonInfo lesson) {
//	_delegate ? _delegate->onLearnLesson(lesson) : (void) nullptr;
//}
//
//bool AISpeakingTopic::IsTopicThumbExits(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo)
//{
//	std::string	folder_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/";
//	std::vector<std::string> thumbpaths;
//	thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.icon_thumb));
//	thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.selected_circle_icon));
//	thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.progress_bg_icon));
//	for (std::string path:thumbpaths)
//	{
//		if (!FileUtils::getInstance()->isFileExist(path))
//			return(false);
//
//	}
//	return(true);
//
//}
//
//bool AISpeakingTopic::checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned)
//{
//	for (int i = 0; i < listIdOfLessonsLearned.size(); i++)
//	{
//		if (id == listIdOfLessonsLearned[i])
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void AISpeakingTopic::mouseScroll(std::string listviewName, ListView* listView, float percent, float timeInSec, bool attenuated)
//{
//    ListView* list;
//    if(listviewName!="")
//        list = utils::findChild<ListView*>(this, listviewName);
//    else
//    {
//        list = listView;
//    }
//    if (!list)
//        return;
//
//        int _percent = list->getScrolledPercentHorizontal();
//        _percent += percent;
//        list->scrollToPercentHorizontal(_percent, timeInSec, attenuated);
//
//}
