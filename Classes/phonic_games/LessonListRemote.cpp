#include "LessonListRemote.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "ui/CocosGUI.h"
#include "sqlite3.h"
#include "MJMainGBase.h"
#include "activity/GameList.h"
#include "StoryData.h"
#include <string>
using namespace cocos2d::network;
using namespace rapidjson;
#define BASE_URL "http://api.daybehoc.com"
#define APP_ID 52
#define DEVICE_ID 123

bool MJLessonsListRemote::init()
{
	if (!Scene::init())
		return false;

	initGUI();
	fetchGameInfoFromAPI();
	return true;
}

void MJLessonsListRemote::initGUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto margin = 50.0f;
	auto columnMargin = 20.0f;
	auto listViewSize = cocos2d::Size(
		(visibleSize.width - 2 * (margin + columnMargin)) / 3,
		visibleSize.height - 2 * margin
	);

	auto tab_lesson = MPTabLessons::createTab();
	tab_lesson->setDelegate(this);
	this->addChild(tab_lesson, 1, "tab_lesson");

	//back button
	auto btn_close = ui::Button::create("phonics_btnext.png");
	btn_close->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	btn_close->setPosition(Vec2(visibleSize) - Point(50, 50));
	addChild(btn_close,1);
	btn_close->addClickEventListener([=](Ref* ref) {
//        auto scene = GameListSelect::create();
//        Director::getInstance()->replaceScene(scene);
        this->removeFromParent();
	});

	m_progress = cocos2d::ui::Text::create("", "fonts/monkeystories_medium.ttf", 30);
	m_progress->setPosition(cocos2d::Vec2(visibleSize.width / 2, 25));
	addChild(m_progress,5);
}

void MJLessonsListRemote::downloadFile(std::string url, std::function<void(bool, std::string)> callback, bool disableCache)
{
	auto writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
	writablePath += "cached/zip";
	auto lastSlash = url.find_last_of('/') + 1;
	writablePath += url.substr(lastSlash);

	// cached
	if (cocos2d::FileUtils::getInstance()->isFileExist(writablePath) && disableCache == false) {
		if (callback)
			callback(true, writablePath);
		return;
	}

	auto downloader = new cocos2d::network::Downloader();
	downloader->onTaskProgress = ([=](const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected) {
		//downloading progress
		std::ostringstream s;
		s << (totalBytesReceived * 10000 / totalBytesExpected) / 100.0f;
		m_progress->setString(std::string("Downloaded ") + s.str() + " %");
	});

	downloader->onFileTaskSuccess = ([=](const cocos2d::network::DownloadTask& task) {
		//file downloaded, do what you need next
		auto a = 5;
		if (callback)
			callback(true, writablePath);
	});

	downloader->onTaskError = ([=](const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr) {
		//file downloading error
		auto b = 5;
		CCLOG("%s", errorStr.c_str());
		if (callback)
			callback(false, "");
	});

	downloader->createDownloadFileTask(url, writablePath);
}

void MJLessonsListRemote::downloadAndPlay(mj::model::ActInfo act_info)
{
	// download resources
	if (m_downloading)
		return;
	m_downloading = true;
	auto gameIt = m_games.find(act_info.sub_id);
	if (gameIt != m_games.end())
		act_info.resource_zip = gameIt->second->resource_zip;
	//act_info.resource_zip = mj::helper::getUrlFromDomain(act_info.resource_zip, mj::UrlType::MSURL_GAME);
	downloadFile(act_info.resource_zip, [=](bool isSuccess, std::string filePath) {
		if (!isSuccess) {
			m_downloading = false;
			return;
		}
		auto writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
		auto basePath = writablePath + "extracted/0/";
		FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + "extracted/0/" + (StoryData::getInstance()->isHdr ? "hdr/" : "hd/"), true);
		auto zipFile = new cocos2d::ZipFile(filePath);
		auto fileName = zipFile->getFirstFilename();
		while (fileName.length() > 0) {
			if (fileName.find_last_of('/') + 1 == fileName.length() ||
				fileName.find_last_of('.') == std::string::npos) {
				fileName = zipFile->getNextFilename();
				continue;
			}
			ssize_t size = 0;
			auto fileData = zipFile->getFileData(fileName, &size);
			if (!size)
				continue;

			cocos2d::Data dataWrapper;
			dataWrapper.copy(fileData, size);
			delete fileData;
			createFile(basePath + fileName);
			cocos2d::FileUtils::getInstance()->writeDataToFile(dataWrapper, basePath + fileName);
			fileName = zipFile->getNextFilename();
		}

		// remove zip file 
		delete zipFile;
		//cocos2d::FileUtils::getInstance()->removeFile(filePath);

		// download config
		downloadFile(act_info.data_zip, [act_info, this, basePath](bool isSuccess, std::string filePath) {
			FileUtils::getInstance()->createDirectory(basePath);
			FileUtils::getInstance()->createDirectory(basePath + "json");
			FileUtils::getInstance()->createDirectory(basePath + "json/" + cocos2d::StringUtils::toString(act_info.activity_id));
			mj::helper::unzip(filePath, basePath + "json/" + cocos2d::StringUtils::toString(act_info.activity_id) + "/");
			cocos2d::FileUtils::getInstance()->removeFile(filePath);
			m_downloading = false;

			// start game
			startGame(act_info);
		}, true);
	});
}

#include "MJBaseActivity.h"
//#include "Math_Libs.h"
void MJLessonsListRemote::startGame(mj::model::ActInfo ai)
{
    disableButton();
	auto basePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "extracted/0/";
	switchToRemotePaths();
	auto configPath = basePath + "json/" + cocos2d::StringUtils::toString(ai.activity_id) + "/config.json";

	auto search_path = FileUtils::getInstance()->getSearchPaths();
	search_path.insert(search_path.begin(), basePath);

	search_path.insert(search_path.begin(), basePath + "json/" + cocos2d::StringUtils::toString(ai.activity_id));
	FileUtils::getInstance()->setSearchPaths(search_path);

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	FileUtils::getInstance()->purgeCachedEntries();

	ms::BaseActivity * game = ms::BaseActivity::createActivity(ai, images);
	if (game) {
		game->setOnExitCallback([] {
			auto search_path = FileUtils::getInstance()->getSearchPaths();
			search_path.erase(search_path.begin());
			FileUtils::getInstance()->setSearchPaths(search_path);
		});
        game->setActivityDelegate(this);
		this->addChild(game, 1000);
		return;
	}

	MJMainGBase* gameView = MJMainGBase::createActivity(ai, configPath, images);

	if (gameView) {
		gameView->setConfigImages(images);
        gameView->setActivityDelegate(this);
		addChild(gameView,1000);
	}
    
    
}

void MJLessonsListRemote::switchToRemotePaths()
{
	auto basePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "extracted/0/";

	//    cocos2d::FileUtils::getInstance()->setDefaultResourceRootPath(basePath);

	auto searchPaths = cocos2d::FileUtils::getInstance()->getSearchPaths();
	/* for (auto &it : searchPaths) {
		 if (it.find_first_of(basePath) == std::string::npos) {
			 it = basePath + it;
		 }
	 }*/
}

void MJLessonsListRemote::clearCaches()
{
	auto cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "caches/";
	if (cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
		cocos2d::FileUtils::getInstance()->removeDirectory(cachePath);
}

void MJLessonsListRemote::clearGameCache()
{
	for (auto it = m_games.begin(); it != m_games.end();) {
		delete it->second;
		it = m_games.erase(it);
	}
}

void MJLessonsListRemote::onExit()
{
	cocos2d::Scene::onExit();
	clearGameCache();
}

void MJLessonsListRemote::fetchGameInfoFromAPI(std::function<void(bool)> callback)
{
	HttpRequest* request = new HttpRequest();
	std::stringstream ss;

	ss << BASE_URL << "/api/v1/mobile/utils/appinfov2?app_id=" << APP_ID << "&device_id=" << DEVICE_ID <<
		"&os=win32&info&app_ver=1&device_type=2&lang_id=1&lang=vi-VN&lang_id=4&api_ver_audio_book=1536137456&api_ver_story=1536137456&version_game_number=0&version_lesson_number=0";

	request->setUrl(ss.str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
		if (response->getResponseCode() != 200) {
			// failed
			if (callback)
				callback(false);
			return;
		}

		Document d;
		auto responseData = response->getResponseData();

		// create zip file from response
		std::string dataBuffer = std::string(responseData->begin(), responseData->end());
		auto zipFile = cocos2d::ZipFile::createWithBuffer(dataBuffer.c_str(), dataBuffer.size());


		string fileName = zipFile->getFirstFilename();
		ssize_t size = 0;
		auto fileData = zipFile->getFileData(fileName, &size);

		std::string buffer;
		buffer.assign((const char *)fileData, size);

		if (d.Parse(buffer.c_str()).HasParseError()) {
			// failed
			if (callback)
				callback(false);
			return;
		}

		if (!(d.HasMember("game_list") && d["game_list"].IsObject())) {
			if (callback)
				callback(false);
			return;
		}

		clearGameCache();
		auto dataArray = d["game_list"]["list"].GetArray();
		for (auto i = 0; i < dataArray.Size(); i++) {
			auto info = new mj::model::GameInfo();
			info->game_id = dataArray[i]["id"].GetInt();
			info->name = dataArray[i]["name"].GetString();
			info->resource_zip = dataArray[i]["path_config"].GetString();
			info->resource_zip = info->resource_zip;
			info->thumbnail_url = dataArray[i]["path_images"].GetString();
			info->zip_size = dataArray[i]["zip_size"].GetString();
			m_games.insert(std::pair<int, mj::model::GameInfo*>(info->game_id, info));

		}
		if (callback) {
			callback(true);
			CCLOG("Load Game Info Success");
		}
	});

	HttpClient::getInstance()->send(request);
	request->release();
}

void MJLessonsListRemote::onClickedActiviyDelegate(mj::model::ActInfo act_info)
{
    _current_act = act_info;
	downloadAndPlay(act_info);
}

void MJLessonsListRemote::onClickedLessonDelegate(mj::model::LessonInfo lesson)
{
    _tab_game = MPTabGames::createTab(lesson);
	_tab_game->setDelegate(this);
	this->addChild(_tab_game, 2);
    
}

MJLessonsListRemote::MJLessonsListRemote() :
	m_currentCategory(0),
	m_downloading(false)
{
}

void MJLessonsListRemote::createFile(std::string path)
{
	if (!path.length())
		return;
	if (cocos2d::FileUtils::getInstance()->isFileExist(path) ||
		cocos2d::FileUtils::getInstance()->isDirectoryExist(path))
		return;
	std::vector<std::string> dirs;
	// split path 
	auto found = path.find_last_of("/\\");
	if (found != std::string::npos) {
		createDirectory(path.substr(0, found));
	}
	FILE* f;
#ifdef WIN32
	fopen_s(&f, path.c_str(), "w");
#else
	f = fopen(path.c_str(), "w");
#endif
	if (f)
		fclose(f);
}

void MJLessonsListRemote::createDirectory(std::string path)
{
	if (cocos2d::FileUtils::getInstance()->isDirectoryExist(path))
		return;
	auto found = path.find_first_of("/\\");
	if (found == std::string::npos) {
		cocos2d::FileUtils::getInstance()->createDirectory(path);
		return;
	}
	std::vector<std::string> dirs;
	do
	{
		found = path.find_first_of("/\\");
		std::string dir = path.substr(0, found);
		dirs.push_back(dir);
		path = path.substr(found + 1);
	} while (found != std::string::npos);

	path = "";
	for (auto it = dirs.begin(); it != dirs.end(); it++) {
		path += (*it);
#ifdef WIN32
		path += "\\";
#else
		path += "/";
#endif
		if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path))
			cocos2d::FileUtils::getInstance()->createDirectory(path);
	}
}
void MJLessonsListRemote::enableButton(){
    for (auto btn: this->getChildren()) {
        if(btn->getName() == "game_btn"){
            ((Layout*)btn)->setTouchEnabled(true);
        }
    }
}

void MJLessonsListRemote::disableButton(){
    for (auto btn: this->getChildren()) {
        if(btn->getName() == "game_btn"){
            ((Layout*)btn)->setTouchEnabled(false);
        }
    }
}

void MJLessonsListRemote::onActivityEnd(Ref* sender, EndReason reason){
    CCLOG("Finished");
	if (EndReason::FINISHED == reason) {
		auto act_info = _tab_game->getNextActivity(_current_act);
		onClickedActiviyDelegate(act_info);
	}
   
}
