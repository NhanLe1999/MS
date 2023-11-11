//
//  StoryInfoCover.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryInfoCover.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "StoryDataModel.h"
#include "StoryEndingPage.h"
#include "TruyenTranhPage.h"
#include "MJ_AnimationCache.h"
#include "MJ_PageLoadResource.h"
#include "NativeBridge.h"
#include "StoryLevelInfo.h"
#include "StoryLevelInfo_Guide.h"
#include "StoryPageVC.h"
#include "MJDefinitions.h"
#include "MJFirebaseListener.h"
#include "MJAPIGetLinkDownload.h"
#include "MJAPISendDownload.h"
#include "MJDebugDownloadView.h"
#include "RemoteImageView.h"
#include "MJBaseActivity.h"
#include "MSBookPage.h"
#include "MSEventLogger.h"
#include "MSServerManager.h"
#include "MSCollectionStoryItem.h"
#include "MSScreenRecorder.h"
#include "StoryRatePopUp.h"
#include "MJStoryInfo.h"
#include "APProfileManager.h"
#include "MJPlatformConfig.h"
#include "MJAPIStoryDetails.h"
#include "MsNewDownloadFile.h"
#include "MSCacheManager.h"

#define tag_get_link_download_story 10
#define tag_network_not_available   100

#define kTimeDelayLevelInfoGuide 2.0f
INITIALIZE_READER_INSTANCE(StoryInfoCoverReader);

USING_NS_CC;
using namespace ui;
std::once_flag ms_storyInfoCover;

Scene* StoryInfoCover::createScene(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from, Size& senceSize)
{
	auto scene = Scene::create();

	scene->setContentSize(senceSize);
	if (scene)
	{
		scene->setName("StoryInfoCoverSence");
		auto view = StoryInfoCover::createView(info, isPortrait, type, from);
		view->setName("StoryInfoCoverPage");
		scene->addChild(view);
		view->InitDisplay();
		view->_usingSence = true;
	}
	else
	{
		CC_SAFE_DELETE(scene);
	}

	return scene;
}

StoryInfoCover* StoryInfoCover::createView(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from)
{
	std::call_once(ms_storyInfoCover, []
		{
			REGISTER_CSB_READER(StoryInfoCover);
		});

	auto storyinfocover = mj::helper::getCSBName("csb/StoryInfoCover.csb");//StoryInfoCover.csb

	auto view = reinterpret_cast<StoryInfoCover*>(CSLoader::createNode(storyinfocover));

	if (view)
	{
		view->InitObjects(info, isPortrait, type, from);
		return view;
	}

	CC_SAFE_DELETE(view);
	return nullptr;
}
void StoryInfoCover::initWithData(StoryInfoObj info, bool isPortrait, int type, const mj::FROMSTORY &from)
{
	CRASH_AUTO_LOG;

	_delegate = nullptr;
	_storyInfo = info;
	_isPortrait = isPortrait;
	_type = type;
    _from = from;
}

Rect StoryInfoCover::getThumbBoundingBox()
{
	CRASH_AUTO_LOG;

	auto thumb = utils::findChild<ui::Layout*>(this, "thumb");
	auto bb = thumb->getBoundingBox();
	auto p = thumb->getPosition();
	p = thumb->getParent()->convertToWorldSpace(p);
	bb.origin = thumb->getParent()->convertToWorldSpace(bb.origin);

	return bb;
}

void StoryInfoCover::estimate() {

	CRASH_AUTO_LOG;

	per_Story = story_size / (story_size + act_size) * 100;
	per_VMQuizz = 100 - per_Story;
}
void StoryInfoCover::InitDisplay() {

	CRASH_AUTO_LOG;
	if (_info == nullptr)
		return;
	this->setVisible(true);
	auto thumb = utils::findChild(this, "thumb");
	thumb->setVisible(false);

	auto cover = utils::findChild(this, "content");
	cover->setOpacity(0);
	cover->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.5), NULL));

	auto bb = _info->getBoundingBox();
	if (_info->getParent()) {
		bb.origin = _info->getParent()->convertToWorldSpace(bb.origin);
	}
	bb.origin = this->convertToNodeSpace(bb.origin);

	auto new_item = MSCollectionStoryItem::createItem(_info->getStoryInfo());
	new_item->disableNewRead();
	new_item->setAnchorPoint(thumb->getAnchorPoint());
	thumb->getParent()->addChild(new_item);
	new_item->setPosition(thumb->getPosition());

	//auto story_written = utils::findChild<ui::Text*>(this, "story_written");
	//auto story_written_point = story_written->getPosition();
	//auto p = thumb->getPosition();
	//p.x = story_written_point.x - story_written->getContentSize().width/2;
	//p = p + thumb->getContentSize() / 2;
	//p = thumb->getParent()->convertToWorldSpace(p);
	//p = this->convertToNodeSpace(p);

	//	//            new_item->runAction(EaseBackOut::create(ScaleTo::create(1, 1.25)));
	//new_item->runAction(EaseExponentialOut::create(MoveTo::create(0.5, p)));
	new_item->runAction(Sequence::create(DelayTime::create(0.0), EaseBackOut::create(ScaleTo::create(1, 1.25)), NULL));

	this->setOpacity(0);
	this->runAction(FadeIn::create(0.25));

	Size size_img_book = Director::getInstance()->getRunningScene()->getBoundingBox().size;
	auto background = utils::findChild(this, "img_book");
	background->setContentSize(Director::getInstance()->getVisibleSize());
	background->setOpacity(0);
	background->runAction(Sequence::create(DelayTime::create(0.25), ResizeTo::create(0.25, size_img_book + Size(10, 10)), nullptr));
	background->runAction(Sequence::create(DelayTime::create(0.25), FadeIn::create(0.25), nullptr));
}
void StoryInfoCover::InitObjects(MSCollectionStoryItem* mSCollectionStoryItem, bool isPortrait, int type, const mj::FROMSTORY& from) {

	CRASH_AUTO_LOG;
	_info = mSCollectionStoryItem;
	_type = type;
	if (_info == nullptr)
		return;
	auto info = _info->getStoryInfo();
	StoryInfoObj obj;

	obj.name = info.name;
	obj.mostpopular = info.total_words;
	obj.readinglevel = info.levels[0];
	obj.languageid = info.lang_id;
	obj.storyid = info.story_id;
	obj.newest = info.unique_id;
	obj.total_page = info.total_page;
	obj.word_count = info.total_words;
	obj.lexile = info.lexile;
	obj._id = info.unique_id;
	obj._storyid = info.story_id;
	obj._version = info.version;
	obj.downloaded = false;
	obj.thumbnail = info.thumbnail_url;
	obj.written = info.author;
	obj.design = info.designer;
	obj.image = info.thumbnail_url;
	this->initWithData(obj, false, 1, _from);
	this->setName(mjhelper_name_storyinfocover);
}
void StoryInfoCover::didLoadFromCSB(MSCollectionStoryItem* info, bool isPortrait, int type, const mj::FROMSTORY& from)
{
	auto listLinkUrl = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::STORY);
	_isNewDownload = ms::NewDownload::getInstance().isNewDownload();
	_maxRetryCount = listLinkUrl.size();
	ms::NewDownload::getInstance().clearListDownLoadStory();

	MJDEFAULT->deleteValueForKey(key_error_url_stories);
    MJDEFAULT->setIntegerForKey("key_time_start_learn_lesson_story", 0);
	MJDEFAULT->setStringForKey(key_live_in_screen, "story_list");
	MJDEFAULT->deleteValueForKey("key_type_of_learning_voice");
    _readNameStroy= true;
	CRASH_AUTO_LOG;

	auto listview = utils::findChild<ui::ListView*>(this, "listview");
	listview->setScrollBarEnabled(false);
	listview->setTouchEnabled(false);
	attemp_download = 2;
	this->setContentSize(DIRECTOR->getVisibleSize());
	ui::Helper::doLayout(this);

	std::string csb_path;
	if (_type == Home_Type)
	{
		csb_path = _isPortrait ? "csb/StoryInfoCover_Portrait.csb" : "csb/StoryInfoCover.csb";
	}
	else
	{
		csb_path = "csb/StoryInfoCover.csb";
		utils::findChild(this, "btclose")->setVisible(false);
	}

	panel_bg = this->getChildByName<Layout*>("panel_bg");
	panel_bg->addTouchEventListener(CC_CALLBACK_2(StoryInfoCover::onPanelLayoutTouchEvent, this));

	string plistFile = StringUtils::format("plist/storycover_lang_content.plist");
	auto maptemp = FileUtils::getInstance()->getValueMapFromFile(plistFile);
	auto _str = JsonDump::getInstance()->MapToString(maptemp);

	CCLOG("abc 124 = %s", _str.c_str());

	if (!maptemp[StringUtils::format("%d", _storyInfo.languageid)].isNull())
	{
		_mapLangContent = maptemp[StringUtils::format("%d", _storyInfo.languageid)].asValueMap();
	}
	else
	{
		_mapLangContent = maptemp["1"].asValueMap();
	}

	_mapLangContent = maptemp[StringUtils::toString(LANGUAGE_MANAGER->getDisplayLangId())].asValueMap();

	img_book = this->getChildByName<ImageView*>("img_book");
	panel_read = utils::findChild<Layout*>(this, "panel_read");
	panel_download = utils::findChild<Layout*>(this, "panel_download");
	panel_progress = utils::findChild<Layout*>(this, "panel_progress");
	text_btdownload = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_btdownload"));
	text_downloadguide = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_downloadguide"));
	text_status = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_status"));
	text_percent = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_percent"));
	text_readtome = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_readtome"));
	text_autoplay = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_autoplay"));
	text_autoitmyseft = static_cast<Text*>(ui::Helper::seekWidgetByName(this, "text_readitmyseft"));

	text_downloadguide->setString("Enjoy this story!");

	LANGUAGE_MANAGER->setFontByLang({
		utils::findChild<ui::Text*>(this, "story_written"),
		utils::findChild<ui::Text*>(this, "story_difficulty"),
		utils::findChild<ui::Text*>(this, "story_illustrated"),
		utils::findChild<ui::Text*>(this, "text_readtome"),
		utils::findChild<ui::Text*>(this, "text_readitmyseft"),
		utils::findChild<ui::Text*>(this, "text_autoplay"),
		utils::findChild<ui::Text*>(this, "text_readnarration"),
		utils::findChild<ui::Text*>(this, "text_narration_normal"),
		utils::findChild<ui::Text*>(this, "text_narration_mute"),
		utils::findChild<ui::Text*>(this, "text_btdownload"),
		utils::findChild<ui::Text*>(this, "text_status"),
        utils::findChild<ui::Text*>(this, "text_recording"),
		}, StoryLanguageManager::TextType::BOLD);
    LANGUAGE_MANAGER->setFontByLang({
        utils::findChild<ui::Text*>(this, "text_recording"),
        }, StoryLanguageManager::TextType::REGULAR);
	//get story image url
	auto story_link = StoryDataModel::getInstance()->getLinkDownloadStory(_storyInfo._id);
	auto activities = StoryDataModel::getInstance()->getActivitiesOfStory(_storyInfo._id);
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false) && !MJDEFAULT->getBoolForKey("debugkey_skip_story", false))
	{
		activities.clear();
	}
//#endif

	story_size = StoryDataModel::getInstance()->getFileSize(story_link);
	act_size = getStorySize(activities);
	auto size = story_size + act_size;
	estimate();

	m_isHaveInteractiveQuestions = (!activities.empty());

	utils::findChild<Text*>(this, "text_filesize")->setString(StringUtils::format("%.2f MB", size));

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		utils::findChild<Text*>(this, "text_filesize")->setVisible(false);
	}

	text_status->setString(_mapLangContent["pleasewait"].asString().c_str());
	text_readtome->setString(replaceString(_mapLangContent["readtome"].asString(), "*", "\n"));
	text_autoplay->setString(replaceString(_mapLangContent["autoplay"].asString(), "*", "\n"));
	text_autoitmyseft->setString(replaceString(_mapLangContent["readitmyself"].asString(), "*", "\n"));

	auto _withMax = 190;
	if (text_readtome->getContentSize().width > _withMax) {
		text_readtome->setScale(_withMax / text_readtome->getContentSize().width);
	}

	if (text_autoplay->getContentSize().width > _withMax) {
		text_autoplay->setScale(_withMax / text_autoplay->getContentSize().width);
	}

	if (text_autoitmyseft->getContentSize().width > _withMax) {
		text_autoitmyseft->setScale(_withMax / text_autoitmyseft->getContentSize().width);
	}

	auto extracted = mj::helper::extractSeries(_storyInfo.name);

	namestory_label = utils::findChild<ui::Text*>(this, "story_title");
	namestory_label->setString(extracted.first);
	namestory_label->setColor(Color3B::BLACK);
	namestory_label->setTextColor(Color4B::WHITE);
	if (namestory_label->getContentSize().width > 1000) {
		namestory_label->ignoreContentAdaptWithSize(false);
		namestory_label->setTextAreaSize(Size(700, 0));
		namestory_label->setContentSize(namestory_label->getVirtualRendererSize());
	}

	auto subtitle = utils::findChild<ui::Text*>(this, "story_subtitle");
	subtitle->setString(extracted.second);

	auto text = utils::findChild<ui::Text*>(this, "text_btdownload");
	text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.download"));

	utils::findChild<ui::Text*>(this, "text_readnarration")->setString(_mapLangContent["record"].asString());
	utils::findChild<ui::Text*>(this, "text_narration_normal")->setString(_mapLangContent["record_normal"].asString());
	utils::findChild<ui::Text*>(this, "text_narration_mute")->setString(_mapLangContent["record_mute"].asString());

	lbwritten = utils::findChild<ui::Text*>(this, "story_written");
	lbwritten->setString(_mapLangContent["written"].asString());

	detail_label = utils::findChild<ui::Text*>(this, "story_author");
	std::vector<int> check_written;
	if (LANGUAGE_MANAGER->getCurrentLangId() == LANGUAGE_VN_CODE)
	{
		check_written = NativeBridge::findStringOnString(_storyInfo.written, "(dịch)", false);
	}

	if (check_written.empty())
	{
		detail_label->setString(_storyInfo.written);
	}
	else
	{
		lbwritten->setString(_mapLangContent["translation"].asString());
		auto translation = mj::helper::replace_string(_storyInfo.written, "(dịch)", "");
		detail_label->setString(translation);
	}

	auto author_string = detail_label->getString();
	auto is_rewrite = !NativeBridge::findStringOnString(author_string, "(vietlai)", false).empty();
	if (is_rewrite) {
		author_string = mj::helper::replace_string(author_string, "(vietlai)", "");
		detail_label->setString(author_string);
		lbwritten->setString(LANGUAGE_MANAGER->getDisplayTextByKey("author.rewrite"));
	}

	lbillus = utils::findChild<ui::Text*>(this, "story_illustrated");
	lbillus->setString(_mapLangContent["design"].asString());

	design_label = utils::findChild<ui::Text*>(this, "story_designer");
	design_label->setString(_storyInfo.design);

	utils::findChild<ui::Text*>(this, "story_difficulty")->setString(_mapLangContent["level"].asString());
	utils::findChild<ui::Text*>(this, "story_level")->setString(StoryDataModel::getInstance()->getLevelInfoById(_storyInfo.readinglevel, _storyInfo.languageid).name);
	//    189x251
	auto path_image = _storyInfo.image;

	CCLOG("abc 123 = %s", _storyInfo.image.c_str());

	if (path_image != "")
	{
		auto pos = path_image.find_last_of('/');
		path_image = path_image.substr(pos);
		path_image = FileUtils::getInstance()->getWritablePath() + image_cache_path + path_image;
	}
	else
	{
		path_image = string("mjstory/home/mjsh_stories_thumb_1.png");
	}
	XHSLib->storythumb = path_image;
	auto imgAvatar = utils::findChild<ui::Layout*>(this, "thumb");
	auto cover = MSCollectionStoryItem::createItem(StoryDataModel::getInstance()->getStoryById(_storyInfo._id));
	cover->setScale(1.25);
	cover->disableNewRead();
	imgAvatar->addChild(cover);

	auto level_info = XHSLib->getNameLevelWithID(_storyInfo.readinglevel);

	auto level = utils::findChild<ui::Text*>(this, "lb_level");
	level->setString(StringUtils::format("%s %s", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "thumb.story.level").c_str(), level_info.c_str()));

	auto bb_illustrate = lbillus->getBoundingBox();
	auto bb_written = lbwritten->getBoundingBox();
	auto bb_author = detail_label->getBoundingBox();
	auto bb_designer = design_label->getBoundingBox();
	auto bb_thumb = imgAvatar->getBoundingBox();

	auto bb_layout = bb_thumb.unionWithRect(bb_illustrate);
	bb_layout = bb_layout.unionWithRect(bb_written);
	bb_layout = bb_layout.unionWithRect(bb_author);
	bb_layout = bb_layout.unionWithRect(bb_designer);

	auto panel_detail = utils::findChild(this, "panel_detail");
	panel_detail->setContentSize(bb_layout.size - Size(0, 30));
	ui::Helper::doLayout(panel_detail);

	auto size_imgbook = img_book->getContentSize();

	auto imglevel = static_cast<ImageView*>(ui::Helper::seekWidgetByName(this, "imglevel"));
	if (imglevel) {
		if (!_isPortrait) {
			imglevel->loadTexture(StringUtils::format("mjstory/storyinfo_level1.png"));
		}
		imglevel->getChildByName<Text*>("levellb")->setString(XHSLib->getNameLevelWithID(_storyInfo.readinglevel));
	}

	progress = utils::findChild<ui::LoadingBar*>(this, "loading_bar");
	progress->setPercent(0);
	text_percent->setLocalZOrder(2);
	text_percent->setString("");

	XHSLib->story_id = _storyInfo.storyid;
	XHSLib->lang_id = _storyInfo.languageid;

	isStoryDownloaded = StoryDataModel::getInstance()->getStoryIsDownloadedWith(_storyInfo._id, _storyInfo.storyid, _storyInfo.languageid);


	auto check_download_again = false;
	if (check_download_again || CONFIG_MANAGER->isTestStory()) {
		if (isStoryDownloaded && _type != Story_Type) {
			auto _text = Text::create("download again", "fonts/monkeystories_medium.ttf", 40);
			this->addChild(_text, 10000);
			_text->setTouchEnabled(true);
			_text->addClickEventListener(CC_CALLBACK_1(StoryInfoCover::onDownload, this));
			if (_isPortrait) {
				_text->setAnchorPoint(Vec2(0, 1));
				_text->setPosition(Vec2(10, 10));
				_text->setRotation(-90);

			}
			else {
				_text->setAnchorPoint(Vec2(0, 0));
				_text->setPosition(Vec2(10, 10));
			}
		}
	}

	if (_type != Story_Type) {
		if (isStoryDownloaded) {
			panel_read->setVisible(true);
			startSyncText();
		}
		else {
			panel_download->setVisible(true);
		}
	}

	ui::Helper::doLayout(this);
	listview->doLayout();
	listview->jumpToTop();

	addFaceMask();

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.edit.face", [this](EventCustom*) {
		this->addFaceMask();
	}), this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (!ms::recorder::isSupported()) {
		utils::findChild(this, "btreadnarration")->setVisible(false);
		utils::findChild(this, "bt_narration_normal")->setVisible(false);
		utils::findChild(this, "bt_narration_mute")->setVisible(false);
		utils::findChild(this, "btreaditmyself")->setVisible(true);
	}
#endif
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
	{
		if (auto btnBackStory = utils::findChild(this, "btn_go_back"))
		{
			btnBackStory->setVisible(true);
		}

		if (auto btnzSkipStory = utils::findChild(this, "btn_skip"))
		{
			btnzSkipStory->setVisible(true);
		}

		if (panel_download->isVisible())
		{
			onDownload(this);
		}
		else {
			this->scheduleOnce([=](float dt) {
				m_isHaveInteractiveQuestions = false;
				auto btnAutoPlay = cocos2d::utils::findChild<ui::Button*>(this, "btautoplay");
				if (btnAutoPlay) {
					onPlayStory(btnAutoPlay);
				}
				}, 0.5f, "delay_auto_play");
		}
	}
//#endif
}

void StoryInfoCover::addFaceMask() {

	CRASH_AUTO_LOG;

	auto face_mask = utils::findChild<ui::ImageView*>(this, "face_mask");
	if (!face_mask) {
		return;
	}

	FileUtils::getInstance()->isFileExist("face.png", [this, face_mask](bool is_exist) {
		if (is_exist) {
			Director::getInstance()->getTextureCache()->reloadTexture("face.png");
			face_mask->loadTexture("face.png");
			face_mask->setOpacity(0xff);
		}
	});
}

#pragma mark - onEnter
void StoryInfoCover::onEnter() {
	//TODO:bg music
	CRASH_AUTO_LOG;
	mj::helper::SetFpsByNumberFrames(6);
	AUDIO_ENGINE->stopBackgroundMusic();
	Layout::onEnter();

	ms::EventLogger::getInstance().logEvent(ms::analytics::kEventViewStoryInfo, {
		{ms::analytics::kPropertyStoryId, cocos2d::Value(_storyInfo._id)},
		{ms::analytics::kPropertyStoryName, cocos2d::Value(_storyInfo.name)},
		{ms::analytics::kPropertyStoryLanguage, cocos2d::Value(_storyInfo.languageid == 1 ? "en" : "vi")},
		});

	didLoadFromCSB(_info, _isPortrait, _type, _from);
    setCallbackOnPlayStory();
    setCallbackonNarrationNormal();
    setCallbackInNarrationNormal();
	XHSLib->level_current = _storyInfo.readinglevel;
	XHSLib->_id = _storyInfo._id;
	NativeBridge::setCrashlyticsKeyValue(key_crashlytic, StringUtils::format("storyid%d_langid%d_pageid%d", _storyInfo._storyid, _storyInfo.languageid, 0));
	CCLOG("version = %d", _storyInfo._version);

	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode key_code, Event* event) {
		switch (key_code) {
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
			this->onKeyboardPage(1);
			break;
		case EventKeyboard::KeyCode::KEY_BACK:
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			auto level_info = this->getChildByName("level_info");
			if (!level_info) {
				this->onClose(nullptr);
			}
#endif
		}
		break;
		default:
			break;
		};
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	if (ms::recorder::isRecording())
	{
		utils::findChild(this, "panel_read")->setVisible(false);
		auto text_recording = LANGUAGE_MANAGER->getDisplayTextByKey("record.swipe");
		utils::findChild<ui::Text*>(this, "text_recording")->setVisible(true);
		utils::findChild<ui::Text*>(this, "text_recording")->setString(text_recording);
		utils::findChild<ui::Text*>(this, "text_recording")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf" : "fonts/Montserrat-SemiBoldItalic.ttf");
	}

	/*if (CONFIG_MANAGER->isAutoPlay()) {
			auto btnAutoPlay = cocos2d::utils::findChild<ui::Button*>(this, "btautoplay");
			if (btnAutoPlay) {
				onPlayStory(btnAutoPlay);
			}
	}*/
}

void StoryInfoCover::onEnterTransitionDidFinish() {

	CRASH_AUTO_LOG;

	Layout::onEnterTransitionDidFinish();

	if (XHSLib->pop_scene_index > 0 && !XHSLib->isEndingPageClick && _type == Home_Type) {
		return;
	}
}

#pragma mark - onExit
void StoryInfoCover::onExitTransitionDidStart() {

	CRASH_AUTO_LOG;

    _callbackOnPlayStory = nullptr;
    _callbackonNarrationNormal = nullptr;
    _callbackInNarrationNormal = nullptr;
	Layout::onExitTransitionDidStart();
	LTZipHelper::getInstance().setDelegate(nullptr);
	cocos2d::experimental::AudioEngine::stopAll();

	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->stopAllEffects();
	AUDIO_ENGINE->stopAllSound();
    mj::helper::purgeTextureData();
}

void StoryInfoCover::onExit() {

	CRASH_AUTO_LOG;
	mj::helper::SetFpsByNumberFrames(6);
	ui::Layout::onExit();
	LTZipHelper::getInstance().stopDownload();
	LTZipHelper::getInstance().setDelegate(nullptr);
	_getlink_api ? _getlink_api->setCallback(nullptr) : (void) nullptr;
	_delegate = nullptr;
    _callback = nullptr;
    _callbackOnPlayStory = nullptr;
    _callbackonNarrationNormal = nullptr;
    _callbackInNarrationNormal = nullptr;
    
    if(_storyAlert) {
        
        _storyAlert->setDelegate(nullptr);
    }

	cocos2d::experimental::AudioEngine::stopAll();

	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->stopAllEffects();
	AUDIO_ENGINE->stopAllSound();
	AsyncTaskPool::getInstance()->stopTasks(cocos2d::AsyncTaskPool::TaskType::TASK_UNZIP);
	//if (_usingSence)
	//{
	//	DIRECTOR->popScene([](Scene* scene) {
	//		return TransitionSlideInL::create(0.25, scene);
	//		});
	//}
}

void StoryInfoCover::onAlertOK(cocos2d::Ref* sender) {

	CRASH_AUTO_LOG;

	if (auto alert = dynamic_cast<StoryAlert*>(sender)) {
		if (alert->getTag() == 11) {
			getLinkDownloadStory(true);
		}
	}
}

void StoryInfoCover::onAlertClose(cocos2d::Ref* sender) {

	CRASH_AUTO_LOG;

	if (auto alert = dynamic_cast<StoryAlert*>(sender))
	{
		int tag = alert->getTag();
		if (tag == tag_get_link_download_story) { 
			attemp_download = 2;
			panel_read->setVisible(false);
			panel_progress->setVisible(false);
			panel_download->setVisible(true);
			return;
		}
	}
}

void StoryInfoCover::onAlertCancel(cocos2d::Ref* sender) {

	CRASH_AUTO_LOG;

	if (auto alert = dynamic_cast<StoryAlert*>(sender))
	{
		int tag = alert->getTag();
		if (tag == tag_get_link_download_story) { /* retry download */
			attemp_download = 2;
			_valueLinkError = { -1, "" };
			_countClickRetry = -1;
			_listUrlDownloadError.clear();
			onDownload(this);
			return;
		}
		else if (tag == tag_network_not_available) { /* retry download */
			this->onDownload(nullptr);
			return;
		}
	}
}

std::string StoryInfoCover::replaceString(std::string subject, const std::string search, const std::string replace) {

	CRASH_AUTO_LOG;

	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}

#pragma mark - BUTTON
void StoryInfoCover::onClose(cocos2d::Ref* sender) {

	CRASH_AUTO_LOG;

	AUDIO_ENGINE->stopAllEffects();

	if (MJDEFAULT->getBoolForKey(key_background_music)) {
		if (CONFIG_MANAGER->isVietnameseStories()) {
			AUDIO_ENGINE->playBackgroundMusic("sounds/bg_music.mp3");
		}
	}

	if(!_listEvent.empty())
	{
		for(int i = 0; i < _listEvent.size(); i++)
		{
			Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_listEvent[i]);
		}
	}
	_listEvent.clear();

    if(auto button  = utils::findChild<ui::Button*>(this, "btdownload")) {
        
        button->setEnabled(false);
    }

	ms::recorder::cancelRecord();

	auto background = utils::findChild(this, "panel_bg");
	background->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=] {
		if (_delegate) {
			_delegate ? _delegate->onActivityEnd(this, ms::delegate::Activity::EndReason::INTERRUPTED) : (void) nullptr;
		}
		else {
			this->removeFromParent();
		}
	}), nullptr));

	ms::NewDownload::getInstance().clearListDownLoadStory();

	for (auto c : this->getChildren()) {

		if (c->getName() == "panel_bg") {
			continue;
		}

		c->runAction(EaseExponentialIn::create(MoveBy::create(0.5f, Vec2(-1000, 0))));
		c->runAction(FadeOut::create(0.5f));
	}
	if (_usingSence)
	{
		DIRECTOR->popScene([](Scene* scene) {
			return TransitionSlideInL::create(0.25, scene);
			});
	}
}

void StoryInfoCover::startDownload() {

	CRASH_AUTO_LOG;
	
	_is_downloading = true;
	attemp_download = 2;
	getLinkDownloadStory(false);
}

void StoryInfoCover::doDownload() {
	CRASH_AUTO_LOG;

	per_Story = 100;
	per_VMQuizz = 0;
	current_percent = 0;

	//[Feature_Begin]: Interactive Questions

	CCLOG("%s____linkne", _valueLinkError.second.c_str());
    
    if(keyTypeError.length() == 0)
    {
        keyTypeError = MJDEFAULT->getStringForKey(key_error_url_stories, "None");
    }

	if (_valueDomainDownloadStories.empty())
	{
		_valueDomainDownloadStories = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::STORY);
	}

	if (_valueDomainDownloadAudioBookAndActivity.empty())
	{
		_valueDomainDownloadAudioBookAndActivity = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
	}

	auto acts = StoryDataModel::getInstance()->getActivitiesOfStory(_storyInfo._id, _valueLinkError, _countClickRetry, _valueDomainDownloadStories, _valueDomainDownloadAudioBookAndActivity, keyTypeError);
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false) && !MJDEFAULT->getBoolForKey("debugkey_skip_story", false))
	{
		acts.clear();
	}
//#endif
	_valueLinkError = { -1, "" };
	activities.clear();

	for (auto ac : acts)
	{
		if (!ms::LessonManager::getInstance().isActivityReady(ac))
		{
			activities.push_back(ac);
		}
	}

	if (!activities.empty())
	{
		per_Story = 50;
		per_VMQuizz = 50;
		m_isHaveInteractiveQuestions = true;
	}
	//[Feature_End]: Interactive Questions
	this->unschedule(schedule_selector(StoryInfoCover::displayLevelInfoGuide));

	if (!NativeBridge::networkAvaiable())
	{
        _storyAlert = showAlertNetworkDown(tag_network_not_available, _isPortrait, true);
		return;
	}

	auto check_debug_download_view = false;

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	check_debug_download_view = true;
#else
	check_debug_download_view = false;
#endif
	check_debug_download_view = false;

	if (check_debug_download_view)
	{
		auto view = MJDebugDownloadView::createView();
		this->addChild(view);
	}
	else
	{
		startDownload();
	}
}

void StoryInfoCover::onDownload(cocos2d::Ref* sender) {

    if(mj::helper::showPopupWarningFullDisk())
    {
        return;
    }
    
	ms::utils::ThumbQueue::getInstance().unDownloadImage();
	ms::utils::Downloader::getInstance().unEnqueue();
    ms::NewDownload::getInstance().clearListDownLoadStory(true);

	CRASH_AUTO_LOG;

	if (_is_downloading) {
		return;
	}
    
    if(auto button  = utils::findChild<ui::Button*>(this, "btdownload")) {
        
        button->setEnabled(false);
    }
	
	_is_downloading = true;

	panel_read->setVisible(false);
	panel_progress->setVisible(true);

    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        auto load_action = Sequence::create(CallFuncN::create([](Node* n) {
            if (auto node = dynamic_cast<ui::Text*>(n)) {
                node->setString(".");
            }
        }), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
            if (auto node = dynamic_cast<ui::Text*>(n)) {
                node->setString("..");
            }
        }), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
            if (auto node = dynamic_cast<ui::Text*>(n)) {
                node->setString("...");
            }
        }), DelayTime::create(0.25), nullptr);

        if(text_percent && text_percent->getNumberOfRunningActions() > 0)
        {
            text_percent->stopAllActions();
            text_percent->runAction(RepeatForever::create(load_action));
        }
    });

	text_percent->stopAllActions();
	doDownload();

}

void StoryInfoCover::onReadWithNarration(Ref* sender)
{
	CRASH_AUTO_LOG;

	CCLOG("StoryInfoCover::onReadWithNarration");

	if (!ms::recorder::isSupported()) {
        _storyAlert = StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("recorder.warning"), "OK", "");
		return;
	}

	CCLOG("narration...");
	auto panel = utils::findChild(this, "panel_read");
	for (auto c : panel->getChildren()) {
		auto c_name = c->getName();
		if (c_name == "btautoplay" || c_name == "btreadtome" || c_name == "btreaditmyself") {
			c->runAction(Sequence::create(FadeOut::create(0.25), Hide::create(), NULL));
		}

		if (c_name == "bt_narration_normal") {
			c->setOpacity(0x00);
			c->runAction(Sequence::create(DelayTime::create(0.15), Show::create(), Spawn::create(FadeIn::create(0.25), MoveTo::create(0.5, Point(100, 150)), NULL), NULL));
		}

		if (c_name == "bt_narration_mute") {
			c->setOpacity(0x00);
			c->runAction(Sequence::create(DelayTime::create(0.15), Show::create(), Spawn::create(FadeIn::create(0.25), MoveTo::create(0.5, Point(300, 150)), NULL), NULL));
		}

		if (c_name == "btreadnarration") {
			c->runAction(Sequence::create(DelayTime::create(0.15), Show::create(), MoveTo::create(0.5, Point(300, 150)), NULL));
		}
	}
}

void StoryInfoCover::onNarrationNormal(cocos2d::Ref* sender)
{
	MJDEFAULT->setStringForKey("key_type_of_learning_voice", "record_your_voice_narration");
	CRASH_AUTO_LOG;
	CCLOG("StoryInfoCover::onNarrationNormal");
    
    if(_callbackonNarrationNormal)
    {
        _callbackonNarrationNormal();
    }
}

void StoryInfoCover::setCallbackonNarrationNormal()
{
    _callbackonNarrationNormal = [this] () {
        ms::recorder::startRecord([this] {
            if(_callbackInNarrationNormal)
            {
                _callbackInNarrationNormal();
            }
        });
    };
}

void StoryInfoCover::setCallbackInNarrationNormal()
{
    _callbackInNarrationNormal = [this] () {
        ms::recorder::disableMicro();
        auto bt_read_to_me = utils::findChild(this, "btreadtome");
        onPlayStory(bt_read_to_me);
        if(auto tr = utils::findChild<ui::Text*>(this, "text_recording"))
        {
            tr->setVisible(true);
            auto text_recording = LANGUAGE_MANAGER->getDisplayTextByKey("record.recording");
            tr->setString(text_recording);
            tr->setPosition(Point(tr->getParent()->getContentSize().width / 2 - tr->getContentSize().width / 2, tr->getPositionY()));
            auto load_action = Sequence::create(CallFuncN::create([text_recording](Node* n) {
                reinterpret_cast<ui::Text*>(n)->setString(text_recording);
            }), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
                reinterpret_cast<ui::Text*>(n)->setString(text_recording + " .");
            }), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
                reinterpret_cast<ui::Text*>(n)->setString(text_recording + " . .");
            }), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
                reinterpret_cast<ui::Text*>(n)->setString(text_recording + " . . .");
            }), DelayTime::create(0.4), NULL);
            tr->runAction(RepeatForever::create(load_action));
        }
    };
}

void StoryInfoCover::onNarrationMute(cocos2d::Ref* sender)
{
	CRASH_AUTO_LOG;

	CCLOG("StoryInfoCover::onNarrationMute");
	MJDEFAULT->setStringForKey("key_type_of_learning_voice", "record_your_voice_mute");
	ms::recorder::startRecord([this] {
		panel_read->setVisible(false);
		isReadDoneTitle = true;
		if(auto tr = utils::findChild<ui::Text*>(this, "text_recording"))
		{
			tr->setVisible(true);
			auto text_recording = LANGUAGE_MANAGER->getDisplayTextByKey("record.swipe");
            tr->setString(text_recording);
			tr->setPosition(Point(tr->getParent()->getContentSize().width / 2 - tr->getContentSize().width / 2, tr->getPositionY()));
			auto load_action = Sequence::create(CallFuncN::create([text_recording](Node* n) {
				reinterpret_cast<ui::Text*>(n)->setString(text_recording);
			}), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
				reinterpret_cast<ui::Text*>(n)->setString(text_recording + " .");
			}), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
				reinterpret_cast<ui::Text*>(n)->setString(text_recording + " . .");
			}), DelayTime::create(0.4), CallFuncN::create([text_recording](Node* n) {
				reinterpret_cast<ui::Text*>(n)->setString(text_recording + " . . .");
			}), DelayTime::create(0.4), NULL);
			tr->runAction(RepeatForever::create(load_action));
		}
	});
}

#include "MSAudioDelay.h"

void StoryInfoCover::setCallbackOnPlayStory()
{
    _callbackOnPlayStory =[this] () {
        mj::AnimationCache::startSession();
        this->loadAllPageStories();
        showPleaseWait(false);
        if ((int)timeline.size() == 0) {
            this->onStartPlayStory();
        }
        else {
            if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
                AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(_audio_title).c_str());
                ValueMap map = timeline[0].asValueMap();
                auto delay0 = map["ts"].asInt() / 1000.0f;
                this->scheduleOnce([=](float dt) {
                    highlightTextAction();
                }, delay0, StringUtils::format("start_highlight"));
            }
            else {
                this->onStartPlayStory();
            }
        }
    };
}

void StoryInfoCover::onPlayStory(cocos2d::Ref* sender)
{
    if(_readNameStroy)
    {
        if (!isStoryAvailable()){
            mj::model::ActInfo info;
            auto storyName = _storyInfo.name;
            info.name = storyName;
            auto errorPopup = UnzipErrorPopup::createPopup(UnzipErrorPopup::PopupType::STORY, info, nullptr);
            this->addChild(errorPopup);

		return;
	}
	MJDEFAULT->setStringForKey(key_name_last_learn_activity, _storyInfo.name);
	CleverTapManager::GetInstance()->pushProfileByID(MJDEFAULT->getIntegerForKey(key_current_profile_id),CleverTapManager::PUSH_TYPE::LOGIN,nullptr, true);
    MJDEFAULT->setIntegerForKey("key_time_start_learn_lesson_story", int(utils::getTimeInMilliseconds() / 1000));
	CRASH_AUTO_LOG;
	CCLOG("StoryInfoCover::onPlayStory");

	this->unschedule(schedule_selector(StoryInfoCover::displayLevelInfoGuide));
	auto level_guide = this->getChildByName("level_guide");
	if (level_guide) {
		level_guide->removeFromParent();
	}

	auto tag = static_cast<Button*>(sender)->getTag();
	if (tag == 4) {
		onReadWithNarration(nullptr);
	}
	else {
		reinterpret_cast<ui::Button*>(sender)->setEnabled(false);
	}

	XHSLib->story_play_type = tag - 30;

	mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_playstory_type, analytics_event_playstory_type, XHSLib->story_play_type);

	ms::EventLogger::getInstance().logEvent(ms::analytics::kEventReadStory, {
		{ms::analytics::kPropertyStoryId, cocos2d::Value(_storyInfo.storyid)},
		{ms::analytics::kPropertyStoryName, cocos2d::Value(_storyInfo.name)},
		{ms::analytics::kPropertyStoryLanguage, cocos2d::Value(_storyInfo.languageid == 1 ? "en" : "vi")},
		{ms::analytics::kPropertyStoryReadingType, cocos2d::Value(XHSLib->story_play_type)}
		}, true);

	if (MJDEFAULT->getStringForKey("STORY_START_FROM", "unknow") == "story_search_tab")
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("push_clevertap_event_for_story_tab");
	}

	XHSLib->pageNumAll = _storyInfo.total_page;
	XHSLib->pageIndex = 1;
	panel_read->setVisible(false);

        showPleaseWait(true, _isPortrait);
        mj::helper::AudioDelay::preload(_storyInfo.storyid, _storyInfo.languageid, [this] {
           if(_callbackOnPlayStory)
           {
               _callbackOnPlayStory();
           }
        });
        _readNameStroy = false;
    }
}

void StoryInfoCover::startSyncText() {
	CRASH_AUTO_LOG;
	auto _filepath = FileUtils::getInstance()->getWritablePath() + cocos2d::StringUtils::format("mjstory/%d_%d_%d.plist", XHSLib->story_id, XHSLib->lang_id, 0);
	if (FileUtils::getInstance()->isFileExist(_filepath)) {
		_mapT0 = FileUtils::getInstance()->getValueMapFromFile(_filepath);
		if (!_mapT0["title"].isNull()) {
			auto _mapTitle = _mapT0["title"].asValueMap();
			cocos2d::Value::Type vType = _mapTitle["sync_data"].getType();
			if (!_mapTitle["sync_data"].isNull() && vType == Value::Type::VECTOR) {
				timeline = _mapTitle["sync_data"].asValueVector();
				//            timeline = JsonDump::getInstance()->dumpValueVectorFromString(_string);
			}
			_audio_title = _mapTitle["audio"].asString();
			_audio_written = _mapT0["writentby"].asValueMap()["audio"].asString();
			_audio_design = _mapT0["illustratedby"].asValueMap()["audio"].asString();


			if (!_mapT0["writentby"].asValueMap()["duration"].isNull()) {
				_time_delay_written = _mapT0["writentby"].asValueMap()["duration"].asFloat() / 1000.0;
			}

			if (!_mapT0["illustratedby"].asValueMap()["duration"].isNull()) {
				_time_delay_design = _mapT0["illustratedby"].asValueMap()["duration"].asFloat() / 1000.0;
			}
		}
	}
	XHSLib->fontNameStory = _mapT0["fontname"].asString();
}

void StoryInfoCover::highlightWrittenDesign() {
    
    ms::EventLogger::getInstance().logEvent("StoryInfoCover::highlightWrittenDesign", { });
    
	CRASH_AUTO_LOG;
    
	auto _start_design = CallFunc::create([=] {
		design_label->setTextColor(Color4B(highlight_color));
		lbillus->setTextColor(Color4B(highlight_color));
		if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
			AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(_audio_design).c_str());
		}
	});

	auto _end_design = CallFunc::create([=] {
		design_label->setTextColor(Color4B(normal_color));
		lbillus->setTextColor(Color4B(normal_color));
		this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([=] {
			this->onStartPlayStory();
		}), NULL));
	});

	design_label->runAction(Sequence::create(DelayTime::create(_time_delay_written + 2 * 0.2f), _start_design, DelayTime::create(_time_delay_design), _end_design, NULL));

	auto _start_written = CallFunc::create([=] {
		detail_label->setTextColor(Color4B(highlight_color));
		lbwritten->setTextColor(Color4B(highlight_color));
		if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
			AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(_audio_written).c_str());
		}
	});

	auto _end_written = CallFunc::create([=] {
		detail_label->setTextColor(Color4B(normal_color));
		lbwritten->setTextColor(Color4B(normal_color));
	});

	detail_label->runAction(Sequence::create(DelayTime::create(0.2f), _start_written, DelayTime::create(_time_delay_written), _end_written, NULL));
}

void StoryInfoCover::highlightTextAction() {
	//    CCLOG("sync_index: %i", sync_index);
	CRASH_AUTO_LOG;
	this->unscheduleAllCallbacks();
	if ((timeline.size()) == sync_index) {
		namestory_label->setColor(normal_color);
		sync_index = 0;
		timeline.clear();
		//highlight written
		isReadDoneTitle = true;
		highlightWrittenDesign();
		return;
	}

	namestory_label->setColor(normal_color);
	ValueMap map = timeline[sync_index++].asValueMap();
	iStart = map["ts"].asInt();
	iEnd = map["te"].asInt();

	for (int i = iStart; i <= iEnd; i++) {
		auto letter = namestory_label->getLetter(i);
		if (letter != NULL) {
			letter->setColor(Color3B::RED);
		}
	}

	auto duration = (map["e"].asFloat() - map["s"].asFloat()) / 1000.0f;
	this->scheduleOnce([=](float dt) {
		highlightTextAction();
	}, duration, StringUtils::format("sync_index_%i", sync_index));
}

#pragma mark - Download_Action
void StoryInfoCover::downloadAction(string url) {
	CRASH_AUTO_LOG;
	CCLOG("link_story = %s", url.c_str());
    _urlStrory = url;

	//trường hợp đã có zip
	auto fname = mj::helper::getFilenameFromURL(url);
	auto fdirectory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
	auto fpath = fdirectory + fname;

	if (is_story_has_plist0) {
		mj::PageLoadResource::getInstance().deleteCacheOfStory(_storyInfo._id, fpath);
	}

	if (FileUtils::getInstance()->isFileExist(fpath))
	{
		this->getEventDispatcher()->removeCustomEventListeners(fpath + "_unzip_done");
		this->getEventDispatcher()->removeCustomEventListeners(fpath + "_unzip_error");
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(fpath + "_unzip_done", [this](EventCustom* e) {

			(m_isHaveInteractiveQuestions) ? this->downloadResourceQuizzVmonkey() : this->onDownloadAndUnzipDone();

		}), this);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(fpath + "_unzip_error", [this](EventCustom* e) {
			this->onUnzipError(-1, "");
		}), this);

		mj::helper::unzipAsync(fpath, fdirectory, [=](bool success) {
			try
			{
				if (success) {
					pushDownloadStoryEvent(true, true, url);
					FileUtils::getInstance()->removeFile(fpath);
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_done");
				}
				else {
					// Trường hợp download về được nhưng unzip bị lỗi
                    _countRetryDownloadUrl++;
                    if(_countRetryDownloadUrl < 2)
                    {
                        FileUtils::getInstance()->removeFile(fpath);
                        ms::NewDownload::getInstance().clearListDownLoadStory();
                        downloadAction(_urlStrory);
                        return;
                    }
                    else{
                        _countRetryDownloadUrl = 0;
                    }
                    
					pushDownloadStoryEvent(true, false, url);
					MJDEFAULT->setStringForKey(key_link_url_download, url);
					MS_LOGGER.logEvent("Unzip Error", {});
					MS_LOGGER.logEvent("unzip_failed", { 
						{"story_url",cocos2d::Value(url)},
						{"ip_address",cocos2d::Value(MJDEFAULT->getStringForKey("key_ip_adress", ""))}
						});
					std::string userID = MJDEFAULT->getStringForKey(key_user_id);
					std::string deviceID = MJ_PLATFORM_CONFIG.getDeviceId();
					std::string urlDownload = url;
					std::string storyID = std::to_string(_storyInfo._id);
					std::string key = "story_unzip_error";
					std::string value = "UserID = " + userID + "\nDeviceID = " + deviceID + "\nStoryID = " + storyID + "\nUrlDownload = " + urlDownload + "\n";
					NativeBridge::setCrashlyticsKeyValue(key, value);
					mj::model::ActInfo info;
					auto storyName = _storyInfo.name;
					info.name = storyName;
					auto errorPopup = UnzipErrorPopup::createPopup(UnzipErrorPopup::PopupType::STORY, info, nullptr);
					this->addChild(errorPopup);
					//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_error");
				}
			}
			catch (const std::exception&)
			{

			}
		});
		return;
	}

	ms::utils::DownloadUnit unit;

	auto it = std::find_if(_listUtlDownloadSucce.begin(), _listUtlDownloadSucce.end(), [=](std::string a) {
			return url == a;
			});

	url = url;
	unit.url = url;
	unit.download_id = mj::helper::getFilenameFromURL(url);
	unit.forderPath = FileUtils::getInstance()->getWritablePath() + "mjstory/";
	unit.storage_path = unit.forderPath + unit.download_id;
	unit.success_event = "download_story_success" + unit.download_id;
	unit.failed_event = "download_story_failed" + unit.download_id;
	unit.progress_event = "download_story_progress" + unit.download_id;
	unit.typeUrl = StoryDataModel::TYPEURL::STORY;

	_listEvent.push_back(unit.success_event);
	_listEvent.push_back(unit.failed_event);
	_listEvent.push_back(unit.progress_event);

	this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
	this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
	this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);

	auto filename = mj::helper::getFilenameFromURL(url);
	auto directory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
	auto full_path = directory + filename;

	if ((it != _listUtlDownloadSucce.end()))
	{
		(m_isHaveInteractiveQuestions) ? this->downloadResourceQuizzVmonkey() : this->onDownloadAndUnzipDone();
		return;
	}
	if(_isNewDownload)
	{
		ms::NewDownload::getInstance().initDownload(unit, "storyList", "url_story");
	}

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [=](EventCustom* e) {
		MJDEFAULT->deleteValueForKey(key_error_url_stories);
		mj::helper::unzipAsync(full_path, directory, [=](bool success) {
			try
			{
				if (success) {
					pushDownloadStoryEvent(true, true, url);
					FileUtils::getInstance()->removeFile(fpath);
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(full_path + "_unzip_done");
				}
				else {
					// Download success but unzip error

                    _countRetryDownloadUrl++;
                    if(_countRetryDownloadUrl < 2)
                    {
                        FileUtils::getInstance()->removeFile(full_path);
                        ms::NewDownload::getInstance().clearListDownLoadStory();
                        downloadAction(_urlStrory);
                        return;
                    }
                    else{
                        _countRetryDownloadUrl = 0;
                    }
					pushDownloadStoryEvent(true, false, url);
					mj::model::ActInfo info;
					auto storyName = _storyInfo.name;
					info.name = storyName;
					auto errorPopup = UnzipErrorPopup::createPopup(UnzipErrorPopup::PopupType::STORY, info, nullptr);
					this->addChild(errorPopup);
					// Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(full_path + "_unzip_error");
				}
			}
			catch (const std::exception&)
			{

			}
		});
		auto u = (ms::utils::DownloadUnit*)e->getUserData();
		_listUtlDownloadSucce.push_back(u->url);
	}), this);

	this->getEventDispatcher()->removeCustomEventListeners(full_path + "_unzip_done");
	this->getEventDispatcher()->removeCustomEventListeners(full_path + "_unzip_error");

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(full_path + "_unzip_done", [this](EventCustom* e) {
		_countClickRetry = -1;
		_listUrlDownloadError.clear();
		auto listLinkUrl = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
		_maxRetryCount = listLinkUrl.size();

		(m_isHaveInteractiveQuestions) ? this->downloadResourceQuizzVmonkey() : this->onDownloadAndUnzipDone();

	}), this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(full_path + "_unzip_error", [this](EventCustom* e) {
		this->onUnzipError(-1, "");
	}), this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [=](EventCustom* e) {
		_countClickRetry++;

		if (_maxRetryCount <= _countClickRetry)
		{
			MJDEFAULT->setBoolForKey(key_show_pop_lost_connection, true);
		}
		if(auto info = (ms::utils::DownloadUnit*)e->getUserData())
		{
			_valueLinkError = { _storyInfo._id, info->url };
			CCLOG("Download error: %s", info->url.c_str());
			this->onDownloadError(-1, info->error_message);
		}
	}), this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.progress_event, [=](EventCustom* e) {
		auto p = reinterpret_cast<double*>(e->getUserData());
		if(p)
		{
			current_percent = (*p / 100) * per_Story;
            if(current_percent < progress->getPercent())
            {
                return;
            }
			progress->setPercent(current_percent);
			text_percent->stopAllActions();
			text_percent->setString(StringUtils::format("%0.1f %%", current_percent));
		}
	}), this);

	if(_isNewDownload)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
			ms::NewDownload::getInstance().beginDownload();
		});
	}else{
		ms::utils::Downloader::getInstance().enqueue(unit);
	}
}

void StoryInfoCover::getLinkDownloadStory(bool is_retry) {
	CRASH_AUTO_LOG;
	auto url = StoryDataModel::getInstance()->getLinkDownloadStory(_storyInfo._id, _countClickRetry, _valueDomainDownloadStories, key_error_url_stories);
	auto load_action = Sequence::create(CallFuncN::create([](Node* n) {
		reinterpret_cast<ui::Text*>(n)->setString(".");
	}), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
		reinterpret_cast<ui::Text*>(n)->setString(". .");
	}), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
		reinterpret_cast<ui::Text*>(n)->setString(". . .");
	}), DelayTime::create(0.25), NULL);

	text_percent->stopAllActions();
	text_percent->runAction(RepeatForever::create(load_action));

	if (is_retry) {
		ms::ServerManager::getInstance().moveToNextServer();

		auto domain = MJDEFAULT->getStringForKey(key_alternative_domain_story);

		if (!domain.empty())
		{
			//replace domain of url
			url = mj::helper::replaceDomain(url, domain);
		}
	}

	attemp_download--;

	if (attemp_download < 0) {
		try
		{
			text_percent->stopAllActions();
			text_percent->setString("");
			_is_downloading = false;
			pushDownloadStoryEvent(false, false);
						
			// bỏ popup có 2 button retry và close, sử dụng popup NetworkDown
			auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
			auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
			alert->setTag(tag_get_link_download_story);
			alert->setNetworkDownContent(false, true);
			alert->show();
			alert->setDelegate(this);
                
			_storyAlert = alert;

			
			return;
		}
		catch (const std::exception&)
		{

		}
	}

	if (url.empty()) {
		_getlink_api = mj::GetLinkDownloadAPI::call(MJDEFAULT->getIntegerForKey(key_current_lang, 1), XHSLib->story_id, StoryData::getInstance()->isHdr, [=](std::string uurl) {
			_getlink_api = nullptr;
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, uurl] {
				if (!uurl.empty()) {
					downloadAction(uurl);
					MJDEFAULT->setStringForKey(key_url_story, uurl);
				}
				else {
					getLinkDownloadStory(true);
				}
			});
		});
	}
	else {
		downloadAction(url);
	}
}

void StoryInfoCover::callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest) {
	CRASH_AUTO_LOG;
	auto status = map_response["status"].asInt();
	if (status == 1 && tagRequest == kTag_API_Story) {
		auto link_download = map_response["story"].asString();
		if (link_download != "") {
			linkdownload = link_download;
			downloadAction(linkdownload);
		}
		else {
			auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display);
			auto message = LANGUAGE_MANAGER->getTextByKey(lang_id, "story.download.failure");
			auto bt_try = LANGUAGE_MANAGER->getTextByKey(lang_id, "story.download.failure.try");
			auto bt_close = LANGUAGE_MANAGER->getTextByKey(lang_id, "story.download.failure.close");
            _storyAlert  = StoryAlert::showAlert(message, bt_try, bt_close, false, 24, this, 10);
		}
	}
}

void StoryInfoCover::onDownloadAndUnzipDone()
{

	CRASH_AUTO_LOG;
	if (m_isHaveInteractiveQuestions)
	{
		progress->setPercent(100);
		text_percent->setString(StringUtils::format("%0.1f %%", 100));
	}
	_is_downloading = false;
	mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_download_story_success, "", "");

	StoryDataModel::getInstance()->downloadSuccess(_storyInfo._id, _storyInfo.storyid, _storyInfo.languageid);

	std::vector<int> valueStoryRead = StoryDataModel::getInstance()->getValueStoryReadById(_storyInfo._id);
	
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
	deleteSourceStoryWhenOver50();

	// -- end --

	isDownloaded = true;
	isStoryDownloaded = true;
	//    panel_progress->setVisible(false);
	panel_read->setVisible(true);
	panel_read->setOpacity(0);
	panel_progress->setVisible(false);
	panel_read->runAction(Sequence::create(FadeIn::create(0.5), CallFunc::create([this] {
		panel_download->setVisible(false);
		panel_progress->setVisible(false);
	}), nullptr));

	AUDIO_ENGINE->pauseBackgroundMusic();

	auto mapliststorydown = XHSLib->getValueMapForKey(kListStoryDown);
	if (mapliststorydown[StringUtils::format("%d", _storyInfo.storyid)].asInt() != 1) {
		auto numstorycandown = MJDEFAULT->getIntegerForKey(kNumStoryCanDown);
		numstorycandown -= 1;
		MJDEFAULT->setIntegerForKey(kNumStoryCanDown, numstorycandown);

		mapliststorydown[StringUtils::format("%d", _storyInfo.storyid)] = 1;
		XHSLib->setValueMapForKey(mapliststorydown, kListStoryDown);
	}
	startSyncText();

	if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
	{
		this->scheduleOnce([=](float dt) {
			m_isHaveInteractiveQuestions = false;
			auto btnAutoPlay = cocos2d::utils::findChild<ui::Button*>(this, "btautoplay");
			if (btnAutoPlay) {
				onPlayStory(btnAutoPlay);
			}
			}, 0.5f, "delay_auto_play");
	}
}

#pragma mark - Touch
void StoryInfoCover::onPanelLayoutTouchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eventType) {
	CRASH_AUTO_LOG;
	if (_type == Home_Type) {
		if (isReadDoneTitle && _type != Story_Type && eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
			auto panel = static_cast<Layout*>(pSender);
			cocos2d::Point location = panel->getTouchEndPosition();
			auto pointTouch = panel->getTouchBeganPosition();
			if ((location.x - pointTouch.x) < -200) {
				this->unscheduleAllCallbacks();
				this->stopAllActions();
				AUDIO_ENGINE->stopAllEffects();

				auto tr = utils::findChild<ui::Text*>(this, "text_recording");
				if (tr->isVisible() && tr->getString().find(LANGUAGE_MANAGER->getDisplayTextByKey("record.swipe")) != std::string::npos) {
					auto bt_read_to_me = utils::findChild(this, "btreaditmyself");
					onPlayStory(bt_read_to_me);
				}
				else {
					onStartPlayStory();
				}

				return;
			}
		}
	}
	else {
		if (Story_Type && eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
			auto panel = static_cast<Layout*>(pSender);
			cocos2d::Point location = panel->getTouchEndPosition();
			auto pointTouch = panel->getTouchBeganPosition();
			if ((location.x - pointTouch.x) < -200) {
				this->unscheduleAllCallbacks();
				this->stopAllActions();
				auto parent = (StoryPageVC*)this->getParent();
				parent->nextPageFromCoverInfStory();
				parent = nullptr;
				return;
			}
		}
	}
}

#pragma mark - PLAY_STORY
void StoryInfoCover::onStartPlayStory()
{
	CRASH_AUTO_LOG;
	CCLOG("StoryInfoCover::onStartPlayStory");
	ms::recorder::enableMicro();
	if (XHSLib->fontNameStory == string("") || ((!FileUtils::getInstance()->isFileExist(XHSLib->fontNameStory)) && (!FileUtils::getInstance()->isFileExist(XHSLib->getFilePathStory(XHSLib->fontNameStory))))) {
		//        CCLOG("fontNameStory = %s", XHSLib->fontNameStory.c_str());
		XHSLib->getFontName();
	}
	else {
		if (FileUtils::getInstance()->isFileExist(XHSLib->getFilePathStory(XHSLib->fontNameStory))) {
			XHSLib->fontNameStory = XHSLib->getFilePathStory(XHSLib->fontNameStory);
		}
	}
	XHSLib->level_current = _storyInfo.readinglevel;
	PROFILE_MANAGER.addStoryToRecentlyRead(_storyInfo._id);
	StoryDataModel::getInstance()->setStoryRead(_storyInfo._id);
	std::vector<int> valueStoryRead = StoryDataModel::getInstance()->getValueStoryReadById(_storyInfo._id);

	if (valueStoryRead.size() == 3) {

		PROFILE_MANAGER.insertValueToTableReadStory(valueStoryRead);
		mm::RealtimeDatabase::getInstance().updateReadStories(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _storyInfo._id);
	}

	deleteSourceStoryWhenOver50();

	if (XHSLib->isEndingPageClick)
	{
		if (auto storyPageScene = static_cast<StoryPageVC*>(DIRECTOR->getRunningScene()))
		{
			storyPageScene->setDisableCancelRecordAction(ms::recorder::isRecording());
		}

		auto storypaggevc_scene = static_cast<StoryPageVC*>(CSLoader::createNode("csb/StoryPageVC.csb"));
		storypaggevc_scene->setInfoStory(_storyInfo, is_activity_play, is_sample_read, false, _from);
		MJDEFAULT->setStringForKey("cl_read_recommended_story", _storyInfo.name);
		DIRECTOR->replaceScene(storypaggevc_scene);
	}
	else
	{
		auto storypaggevc_scene = static_cast<StoryPageVC*>(CSLoader::createNode("csb/StoryPageVC.csb"));
		storypaggevc_scene->setInfoStory(_storyInfo, is_activity_play, is_sample_read, _isReadStories, _from);
		storypaggevc_scene->setDelegate(_delegate);
		MJDEFAULT->setStringForKey("cl_read_recommended_story", _storyInfo.name);
		_delegate = nullptr;
		if (_usingSence)
		{
			DIRECTOR->replaceScene(storypaggevc_scene);
		}
		else
		{
			DIRECTOR->pushScene(storypaggevc_scene);
			XHSLib->pop_scene_index++;
		}
		
	}
	this->removeFromParent();
}
void StoryInfoCover::loadAllPageStories() {
	CRASH_AUTO_LOG;
	auto path_check = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.plist", _storyInfo.storyid, XHSLib->lang_id, 1));
	if (FileUtils::getInstance()->isFileExist(path_check)) {
		for (int i = 0; i < 2; i++) {
			CCLOG("_storyInfo.total_page = %d | _storyInfo.storyid = %d", _storyInfo.total_page, _storyInfo.storyid);
			TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, i + 1);
		}
	}
	else {
		path_check = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.json", _storyInfo.storyid, XHSLib->lang_id, 1));
		if (FileUtils::getInstance()->isFileExist(path_check)) {
			for (int i = 0; i < 2; i++) {
				CCLOG("_storyInfo.total_page = %d | _storyInfo.storyid = %d", _storyInfo.total_page, _storyInfo.storyid);
				MSBookPage::createPage(XHSLib->story_id, XHSLib->lang_id, i + 1);
			}
		}
	}
}

void StoryInfoCover::deleteSourceStoryWhenOver50() {
	CRASH_AUTO_LOG;
	int max_story_over = 300;

	auto list_stories_read = StoryDataModel::getInstance()->getAllStoryRead();
	if ((int)list_stories_read.size() >= max_story_over + 1) {
		mj::PageLoadResource::getInstance().deleteCacheOfStory(list_stories_read[max_story_over].asValueMap()["id_story_lang"].asInt());
	}
}

#pragma mark - NEW DOWNLOAD
void StoryInfoCover::onDownloadError(int errorCode, std::string message) {
	CRASH_AUTO_LOG;
	_is_downloading = false;
	mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_download_story_fail, "", "");
	progress->setPercent(0);
	text_percent->setString(StringUtils::format("%d %%", 0));
	CCLOG("download error = %s", message.c_str());

	if (_maxRetryCount <= _countClickRetry)
	{
		_countClickRetry = -1;
		_listUrlDownloadError.clear();
		text_percent->stopAllActions();
		text_percent->setString("");
		_is_downloading = false;
		pushDownloadStoryEvent(false, false);

		// bỏ popup có 2 button retry và close, sử dụng popup NetworkDown
		auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
		auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
		alert->setTag(tag_get_link_download_story);
		alert->setNetworkDownContent(false, true);
		alert->show();
		alert->setDelegate(this);

		_storyAlert = alert;
	}
	else {
		onDownload(this);
	}
}

void StoryInfoCover::onUnzipError(int errorCode, std::string)
{
	CRASH_AUTO_LOG;
	_is_downloading = false;
}

void StoryInfoCover::onDownloadSuccess()
{
	CRASH_AUTO_LOG;
	_is_downloading = false;
}

void StoryInfoCover::onUnzipSuccess()
{
	CRASH_AUTO_LOG;
	(m_isHaveInteractiveQuestions) ? this->downloadResourceQuizzVmonkey() : this->onDownloadAndUnzipDone();
}

void StoryInfoCover::onProgressChange(LTZipHelper::DownloadState download_state, float percent) {
	if (download_state == LTZipHelper::DownloadState::DOWNLOADING) {
		progress->setPercent(percent);
		text_percent->setString(StringUtils::format("%0.1f %%", percent));
	}
	else if (download_state == LTZipHelper::DownloadState::UNZIPPING) {
		//        CCLOG("unzip = %f", percent);
	}
}


void StoryInfoCover::onLevelInfo(cocos2d::Ref* sender) {
	CRASH_AUTO_LOG;
	if (_type == Story_Type) return;
	this->unschedule(schedule_selector(StoryInfoCover::displayLevelInfoGuide));
	CCLOG("On Level Info");
	//    auto level_info = StoryLevelInfo::createPage();
	//    level_info->initWithData(_storyInfo, _isPortrait);
	auto info = StoryDataModel::getInstance()->getStoryById(_storyInfo._id);
	auto level_info = MJStoryInfo::createPage(info);
	level_info->setName("level_info");
	this->addChild(level_info, 29999999);
	//    level_info->onShowHidePopUp(true);
}

void StoryInfoCover::displayLevelInfoGuide(float dt) {
	CRASH_AUTO_LOG;
	if (_type == Story_Type) return;
	auto level_guide = StoryLevelInfo_Guide::createPage();
	level_guide->initWithData(_storyInfo, _isPortrait);
	this->addChild(level_guide, 29999999);
	level_guide->onShowHidePopUp(true);
	level_guide->setName("level_guide");
}

#include "../new_ui/face/StoryInfo_Face.h"
void StoryInfoCover::onAddFace(Ref* sender) {
	CRASH_AUTO_LOG;
	CCLOG("adding face...");
	auto face = StoryInfo_Face::createView();
	face->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	face->setPosition(Point(Director::getInstance()->getVisibleSize().width, 0));
	this->addChild(face);
	this->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width, 0))));
}

float StoryInfoCover::getStorySize(std::vector<mj::model::ActInfo> acts) {
	CRASH_AUTO_LOG;
	float size = 0;
	for (auto act : acts) {
		size += StoryDataModel::getInstance()->getFileSize(act.data_zip);
	}

	return size;
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryInfoCover::onLocateClickCallback(const std::string& callback_name) {
	std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(StoryInfoCover, onClose),
		CLICK_MAP(StoryInfoCover, onDownload),
		CLICK_MAP(StoryInfoCover, onPlayStory),
		CLICK_MAP(StoryInfoCover, onLevelInfo),
		CLICK_MAP(StoryInfoCover, onReadWithNarration),
		CLICK_MAP(StoryInfoCover, onNarrationNormal),
		CLICK_MAP(StoryInfoCover, onNarrationMute),
		CLICK_MAP(StoryInfoCover, onGoback),
		CLICK_MAP(StoryInfoCover, onSkip),
		CLICK_MAP(StoryInfoCover, onAddFace)
	};

	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}
	return nullptr;
}

void StoryInfoCover::onGoback(cocos2d::Ref* sender)
{
	CCLOG("1234");
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->stopAllSound();
	AUDIO_ENGINE->stopAllEffects();
	ms::NewDownload::getInstance().clearListDownLoadStory();

	_getlink_api = nullptr;
	ms::CacheManager::getInstance().onBackStory();
	this->unscheduleAllCallbacks();
	this->unscheduleAllSelectors();
	this->unscheduleUpdate();
	this->stopAllActions();
	this->removeFromParent();
}

void StoryInfoCover::onSkip(cocos2d::Ref* sender)
{
	CCLOG("1234");
	_getlink_api = nullptr;
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->stopAllSound();
	AUDIO_ENGINE->stopAllEffects();
	ms::NewDownload::getInstance().clearListDownLoadStory();

	ms::CacheManager::getInstance().onSkipStory();
	this->unscheduleAllCallbacks();
	this->unscheduleAllSelectors();
	this->unscheduleUpdate();
	this->stopAllActions();
	this->removeFromParent();
}

void StoryInfoCover::onKeyboardPage(int index_keyboard) {
	CRASH_AUTO_LOG;
	switch (index_keyboard) {
	case 1://next
	{
		if (XHSLib->pageIndex >= 1) return;
		if (!XHSLib->enable_keyboard || XHSLib->show_popup_subscribe) return;
		XHSLib->enable_keyboard = false;
		if (XHSLib->pageIndex > XHSLib->pageNumAll) return;
		this->unscheduleAllCallbacks();
		this->stopAllActions();
		auto parent = (StoryPageVC*)this->getParent();
		parent->nextPageFromCoverInfStory();
		parent = nullptr;
		if (!XHSLib->enable_keyboard) {
			DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=] {
				XHSLib->enable_keyboard = true;
			}), NULL));
		}
	}
	break;
	default:
		break;
	}
}
void StoryInfoCover::downloadResourceQuizzVmonkey() {
	CRASH_AUTO_LOG;
	if (activities.size() == 0) {
		onDownloadAndUnzipDone();
		return;
	}
	this->downloadAct(activities[0], 0);

}

void StoryInfoCover::downloadAct(mj::model::ActInfo _info, int index) {

	CRASH_AUTO_LOG;

	double max = current_percent + (per_VMQuizz / (activities.size() + 1.0f));

	CCLOG("xxx|max %f :", (float)max);

	auto success_event = StringUtils::format("download_success_%d_%d", _info.lesson_id, _info.activity_id);
	auto error_event = StringUtils::format("download_error_%d_%d", _info.lesson_id, _info.activity_id);
	auto progress_event = StringUtils::format("download_progress_%d_%d", _info.lesson_id, _info.activity_id);

	auto relative_directory = std::string("cached/zip/");
	if (_info.type == mj::model::ActInfo::Type::STORY) {
		relative_directory = "mjstory/";
	}

	std::sort(_info.zip_urls.begin(), _info.zip_urls.end());
	_info.zip_urls.erase(unique(_info.zip_urls.begin(), _info.zip_urls.end()), _info.zip_urls.end());

	for (auto url : _info.zip_urls) {

		ms::utils::DownloadUnit unit;
		unit.url = url;
		unit.download_id = mj::helper::getFilenameFromURL(unit.url);
		unit.forderPath = FileUtils::getInstance()->getWritablePath() + relative_directory;
		unit.storage_path = unit.forderPath + unit.download_id;
		unit.success_event = success_event;
		unit.failed_event = error_event;
		unit.progress_event = progress_event;

		_listEvent.push_back(success_event);
		_listEvent.push_back(error_event);
		_listEvent.push_back(progress_event);

        unit.typeUrl = StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME;
		CCLOG("activity: %s", _info.name.c_str());
		CCLOG("activity url: %s", url.c_str());
		if (!FileUtils::getInstance()->isFileExist(unit.storage_path))
		{
			if(_isNewDownload)
			{
				ms::NewDownload::getInstance().initDownload(unit, "story_list", "url_story");
			}else{
				ms::utils::Downloader::getInstance().enqueue(unit);
			}
		}
	}

	CCLOG("remove listener %s", success_event.c_str());
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);

	auto event_listener = EventListenerCustom::create(success_event, [=](EventCustom* e) {

		_countClickRetry = -1;
		_listUrlDownloadError.clear();

		if (ms::LessonManager::getInstance().isActivityReady(_info)) {

			Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
			Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
			Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
			CCLOG("chot sale current %f", current_percent);
			if (activities.size() == index + 1) {
				onDownloadAndUnzipDone();
			}
			else {
				downloadAct(activities[index + 1], index + 1);
			}

		}

		auto unit = reinterpret_cast<ms::utils::DownloadUnit*>(e->getUserData());
		if(unit)
		{
			_listUtlDownloadSucce.push_back(unit->url);
			CCLOG("Download success: %s", unit->storage_path.c_str());
		}
	});

	CCLOG("add listener %s", error_event.c_str());
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(progress_event, [=](EventCustom* e) {
        auto progress1 = static_cast<double*>(e->getUserData());

        if(progress1)
		{
			if (*progress1 < 100.0)
			{
				auto perEachActity = per_VMQuizz / (activities.size());

				current_percent = per_Story + index * perEachActity + (*progress1 / 100.0) * (perEachActity);
                if(current_percent < progress->getPercent())
                {
                    return;
                }
				progress->setPercent(current_percent);
				text_percent->stopAllActions();
				text_percent->setString(StringUtils::format("%0.1f %%", current_percent));
			}
		}
    });

	event_listener = EventListenerCustom::create(progress_event, [=](EventCustom* e) {

		auto progress1 = static_cast<double*>(e->getUserData());

		if (*progress1 < 100.0)
		{
			auto perEachActity = per_VMQuizz / (activities.size());

			current_percent = per_Story + index * perEachActity + (*progress1 / 100.0) * (perEachActity);

			progress->setPercent(current_percent);
			text_percent->stopAllActions();
			text_percent->setString(StringUtils::format("%0.1f %%", current_percent));
		}

	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);
	event_listener = EventListenerCustom::create(error_event, [=](EventCustom* e) {
		Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
		Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
		Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				StringUtils::format("event_activity_error_%d", _info.activity_id));

		auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
		if (unit)
		{
			auto count = std::count(_listUrlDownloadError.begin(), _listUrlDownloadError.end(), unit->download_id);

			_countClickRetry = count++;

			_listUrlDownloadError.push_back(unit->download_id);

			if (_maxRetryCount <= _countClickRetry)
			{
				MJDEFAULT->setBoolForKey(key_show_pop_lost_connection, true);
			}

			_valueLinkError = { _info.activity_id, unit->url };
			CCLOG("Download error: %s", unit->url.c_str());
			auto u = (ms::utils::DownloadUnit*)e->getUserData();
			this->onDownloadError(-1, u->error_message);
		}
	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);

	if(_isNewDownload)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
			ms::NewDownload::getInstance().beginDownload();
		});
	}
}

void StoryInfoCover::pushDownloadStoryEvent(bool didDownloadSuccess, bool didUnzipSuccess, std::string url) {
	// Push event lên firebase crashlytics và clevertap
	try
	{
		int storyID = _storyInfo.storyid;
		std::string downloadType = MJDEFAULT->getBoolForKey(key_select_see_more_story) ? "see_more" : "direct";
		std::string errorName;
		if (!didDownloadSuccess) {
			errorName = "network_error";
		}
		else {
			errorName = (!didUnzipSuccess) ? "unzip_failed" : "none";
		}

        std::string storyName = _storyInfo.name;
        
        std::string alpBName = storyName;

		alpBName.erase(remove_if(alpBName.begin(), alpBName.end(), [](char c) {
			
			return !mj::helper::isEnglishCharacter(c) && !mj::helper::isNumber(c) && c != ' ' && c != '!' && c != '-' && c != '?';
			
		}), alpBName.end());
        
        std::string country = MJDEFAULT->getStringForKey(key_country_code,"none");
        
        std::string b64 = base64::encode(storyName);
        
        if(!MJDEFAULT->getBoolForKey("is_log_enable",false)) {
            
            //#FIXME: to fix bugs.
            ms::EventLogger::getInstance().logEvent("learn_story_download_temp", {
                {"story_name", cocos2d::Value(alpBName.empty()?"empty":alpBName)},
                {"country", cocos2d::Value(country)},
                {"story_name_b64", cocos2d::Value(alpBName.empty()?"empty":b64)}
            });
    
            ms::EventLogger::getInstance().logEvent("learn_story_download", {
                {"is_success", cocos2d::Value(didDownloadSuccess)},
                {"failed_reason", cocos2d::Value(didUnzipSuccess)},
                {"download_type", cocos2d::Value(downloadType)},
                {"story_ID", cocos2d::Value(storyID)},
                {"story_name", cocos2d::Value(alpBName.empty()?"empty":alpBName)}
            }, true);
        }

		if (MJDEFAULT->getStringForKey(key_read_from) == "Library" && didDownloadSuccess)
		{
			CleverTapManager::GetInstance()->pushEvent("redownload_item", {
				{"item_name",cocos2d::Value(alpBName.empty() ? "empty" : alpBName)},
				{"item_url",cocos2d::Value(url)},
				{"item_id",cocos2d::Value(storyID)},
				{"item_type",cocos2d::Value("story")},
				{"disk_space_available",cocos2d::Value(StringUtils::format("%0.2fG", mj::helper::getDiskSpace()))},
				{"total_downloaded_cache",cocos2d::Value(StringUtils::format("%0.2fMB", mj::helper::getTotalSizeFileDownload()))},
			});
		}
     
	}
	catch (const std::exception&)
	{

	}
}


bool StoryInfoCover::isStoryAvailable() {

	return true;
}

void onDownloadErrorQuizz(int errorCode, std::string message, int index) {


}

void StoryInfoCover::setOnprocess(double index)
{
    current_percent = (index / 100) * per_Story;
    progress->setPercent(current_percent);
    text_percent->stopAllActions();
    text_percent->setString(StringUtils::format("%0.1f %%", current_percent));
}
