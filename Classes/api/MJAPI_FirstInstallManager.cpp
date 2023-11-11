#include "MJAPI_FirstInstallManager.h"
#include "MJAPI_Audiobook_Zip.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "MJStoryInfoModel.h"
#include "StoryFreeManager.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>
#include "MJAPI_DecodeObjectJsonInfo.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include"CleverTapManager.h"
#include "MJ_PageLoadResource.h"
#include "MsNewRequest.h"
namespace mj {
    namespace api {
        void MJAPIFirstInstallManager::call(std::function<void(bool)> callback) {
            auto refresh_call = new MJAPIFirstInstallManager();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        MJAPIFirstInstallManager::MJAPIFirstInstallManager(){
            if(ms::newApi::Request::getInstance().isNewRequest())
           {
               _downloader = std::shared_ptr<cocos2d::network::Downloader>(new cocos2d::network::Downloader({6, 30,".tmp"}));
               _downloader->setOnTaskError([this](const cocos2d::network::DownloadTask& task, int error_code, int error_internal_code, const std::string& error_string) {
                   mj::helper::pushEventCrashNetWork(task.requestURL, "error", "dowload", error_code, 0);
               });

               _downloader->onTaskProgress = [this](const cocos2d::network::DownloadTask& task,
                                                    int64_t bytesReceived,
                                                    int64_t totalBytesReceived,
                                                    int64_t totalBytesExpected) {
               };

               _downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task) {

                   auto zipfile = task.storagePath;
                   mj::helper::unzipAsync(zipfile, cocos2d::FileUtils::getInstance()->getWritablePath(), [this, zipfile](bool success) {
                       if (success) {
                           cocos2d::FileUtils::getInstance()->removeFile(zipfile);
                           std::string jsonAppinfo = FileUtils::getInstance()->getStringFromFile("index.json");
                           onRequestRaw(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install, jsonAppinfo);
                       }
                       else {

                       }
                   });
               };
            }else{
                _request = new mj::Request(mj::Request::ActionType::PATH_INSTALL_ZIP);
                _request->setUrl(MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, ""));
                _request->setDelegate(this);
            }
        }

        void MJAPIFirstInstallManager::send() {
            if(ms::newApi::Request::getInstance().isNewRequest())
            {
                std::string url = MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, "");
                std::string download_id = mj::helper::getFilenameFromURL(url);
                std::string storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + download_id;
                auto task = _downloader->createDownloadFileTask(url, storage_path, download_id);
            }else{
                if (_request) {
                    _request->send();
                }
            }
        }

        int MJAPIFirstInstallManager::onRequestRaw(int tag, std::string data) {
            CCLOG("MJAPIFirstInstallManager::onRequestRaw");
            unsigned char * unzip_json;
            if(!ms::newApi::Request::getInstance().isNewRequest())
            {
                unzip_json = mj::helper::unzip_json(data);
                if (!unzip_json) {
                    if (_callback) {
                        mj::helper::pushEventCrashNetWork(MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, ""), "error_format", "dowload", 0, 0);
                        _callback(false);
                    }
                    return -1;
                }
            }

            if(data.empty())
            {
                if(_callback)
                {
                    mj::helper::pushEventCrashNetWork(MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, ""), "error_format", "dowload", 0, 0);
                    _callback(false);
                }
                return -1;
            }
      
            rapidjson::Document *doc = new rapidjson::Document();
            if(ms::newApi::Request::getInstance().isNewRequest())
            {
                doc->Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>(data.c_str());
            }else{
                doc->Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);
            }
            
            if (doc->HasParseError() || !doc->IsObject()) {
                if (_callback) {
                    mj::helper::pushEventCrashNetWork(MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, ""), "error_format", "dowload", 0, 0);
                    _callback(false);
                }
                return -1;
            }

            if (mj::helper::showPopupWarningFullDisk())
            {
                if (_callback) {
                    _callback(true);
                }
                return 1;
            }


            std::string imageHdr = mj::helper::isRetina() ? "/hdr/" : "/hd/";

            auto listDataWorkSheet = StoryDataModel::getInstance()->getAllStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId(), 2);

            if (!listDataWorkSheet.empty())
            {
                if (listDataWorkSheet[0].thumbnail_url.find(imageHdr) == std::string::npos)
                {
                    mj::PageLoadResource::getInstance().deleteCacheAllStory();
                    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
                    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
                    FileUtils::getInstance()->purgeCachedEntries();
                }
            }

            DecodeAppInfo(tag, doc);
            IntinialStoryInfo(tag, doc);
            IntinialAudioInfo(tag, doc);
            IntinialWorksheetInfo(tag, doc);
            IntinialGameListInfo(tag, doc);
            IntinialLessonListInfo(tag, doc);
            IntinialListActivitiyInfo(tag, doc);
            IntinialCategoriesInfo(tag, doc);
            // IntinialAISpeakingInfo(tag, doc);
            IntinialProductListInfo(tag, doc);
            IntinialLangguageListInfo(tag, doc);
            IntinialAppProblemsInfo(tag, doc);
            GetListThumbZip(tag, doc);
            MJDEFAULT->setBoolForKey(key_check_is_first_install_is_finished, true);
            if (_callback) {
                _callback(true);
            }

            return 1;
        }

        void MJAPIFirstInstallManager::onRequestSuccess(int tag, rapidjson::Value data) {
            delete this;
        }

        void MJAPIFirstInstallManager::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                mj::helper::pushEventCrashNetWork(MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, ""), "message", "dowload", error_code, 0);
                _callback(false);
            }
            delete this;
        }
        void MJAPIFirstInstallManager::DecodeAppInfo(int tag, rapidjson::Document* document)
        {
            int resultCheckVersion = mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAppInfoData(document);
            if (resultCheckVersion == 1)
                call(_callback);
        }
        void MJAPIFirstInstallManager::IntinialStoryInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "story", Object)) {
                auto object = map_response["story"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install, object);
            }
        }
        void MJAPIFirstInstallManager::IntinialAISpeakingInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "ai_speaking", Object)) {
                auto object = map_response["ai_speaking"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install, object);
            }
        }
        void MJAPIFirstInstallManager::IntinialAudioInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "audio", Object)) {
                auto object = map_response["audio"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAudioData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install, object);
            }
        }
        void MJAPIFirstInstallManager::IntinialWorksheetInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "worksheet", Object)) {
                auto object = map_response["worksheet"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeWorksheetData(tag, object);
            }
        }
        void MJAPIFirstInstallManager::IntinialGameListInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "game_list", Object)) {
                auto gameListResponse = map_response["game_list"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(tag, gameListResponse);
            }
        }
        void MJAPIFirstInstallManager::IntinialLessonListInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "lesson_list", Object)) {
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLessonListData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install, map_response["lesson_list"].GetObject());
            }
        }
        void MJAPIFirstInstallManager::IntinialListActivitiyInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "list_activity", Array)) {
                auto object = map_response["list_activity"].GetArray();
                if (object.Size() > 0)
                    mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryLessonInteractiveData(tag, object);
            }
        }
        void MJAPIFirstInstallManager::IntinialCategoriesInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "categories_list", Object)) {
                auto object = map_response["categories_list"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeCategoriesInfoData(tag, object);

            }
        }
        void MJAPIFirstInstallManager::IntinialProductListInfo(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "product_list", Object)) {
                auto object = map_response["product_list"].GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeProductListInfoData(tag, object);

            }
        }
        void MJAPIFirstInstallManager::IntinialLangguageListInfo(int tag, rapidjson::Document* document)
        {
        }
        void MJAPIFirstInstallManager::IntinialAppProblemsInfo(int tag, rapidjson::Document* document)
        {
        }
        void MJAPIFirstInstallManager::GetListThumbZip(int tag, rapidjson::Document* document)
        {
            auto map_response = document->GetObject();
            if (JSON_SAFE_CHECK(map_response, "download_thumb_story", String)) {
                auto object = map_response["download_thumb_story"].GetString();
                if (object)
                    MJDEFAULT->setStringForKey(THUMB_STORY_URL_KEY, object);
            }
            if (JSON_SAFE_CHECK(map_response, "download_thumb_audio", String)) {
                auto object = map_response["download_thumb_audio"].GetString();
                if (object)
                    MJDEFAULT->setStringForKey(THUMB_AUDIO_URL_KEY, object);
            }
            if (JSON_SAFE_CHECK(map_response, "download_thumb_lesson", String)) {
                auto object = map_response["download_thumb_lesson"].GetString();
                if (object)
                    MJDEFAULT->setStringForKey(THUMB_LESSON_URL_KEY, object);
            }
            if (JSON_SAFE_CHECK(map_response, "download_thumb_other", String)) {
                auto object = map_response["download_thumb_other"].GetString();
                if (object)
                    MJDEFAULT->setStringForKey(THUMB_OTHER_URL_KEY, object);
            }
            if (JSON_SAFE_CHECK(map_response, "download_thumb_ai_speaking", String)) {
                auto object = map_response["download_thumb_ai_speaking"].GetString();
                if (object)
                    MJDEFAULT->setStringForKey(THUMB_AI_SPEAKING_URL_KEY, object);
            }

        }
    }
}
