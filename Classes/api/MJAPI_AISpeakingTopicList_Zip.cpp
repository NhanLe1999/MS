//
//  MJAPIAISpeakingTopicListZip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_AISpeakingTopicList_Zip.h"
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
#include"APStoreErrorAPI.h"

namespace mj {
    namespace api {
        using namespace ap;
        void MJAPIAISpeakingTopicListZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new MJAPIAISpeakingTopicListZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        MJAPIAISpeakingTopicListZip::MJAPIAISpeakingTopicListZip() {
            int api_version_ai_speaking_local = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_local, 0);
            int parentIDCategoriesAISpeaking = 9366;
            _request = new ap::api::Base(ap::api::PlatformType::EDU_LESSON, "/api/v1/categories/list",
                { 
                    {"lang_id", "1"}, 
                    {"version", std::to_string(api_version_ai_speaking_local)},
                    {"parent_id", std::to_string(parentIDCategoriesAISpeaking)},
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool MJAPIAISpeakingTopicListZip::onReceivedRawData(int tag, const std::string& data)
        {
            auto unzip_json = mj::helper::unzip_json(data);
            if (!unzip_json) {
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            CCLOG("MJAPIAISpeakingTopicListZip::onReceivedRawData, response: %s", unzip_json);

            rapidjson::Document doc;
            doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);

            if (doc.HasParseError()) {
                doc.GetParseError();
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            if (!doc.IsObject()) {
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            auto map_response = doc.GetObject();
            std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingCategoryInfo = mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingTopicData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update, map_response);
            MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_local, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_local, -1)));
            onDownloadIconCategorys(aISpeakingCategoryInfo);
            if (_callback) {
                _callback(true);
            }
            return true;
        }

        void MJAPIAISpeakingTopicListZip::onDownloadIconCategorys(std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingCategoryInfos)
        {
            if (aISpeakingCategoryInfos.size() <= 0)
                return;
            std::vector <mj::api::ThumbDownloadInfo*> thumbDownloadInfos;
            std::vector <std::string> download_url_thumbs;
            for (mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo : aISpeakingCategoryInfos)
            {
                bool thumbExits = IsTopicThumbExits(aISpeakingCategoryInfo.iconInfo);
                if (!thumbExits)
                {
                    download_url_thumbs.push_back(aISpeakingCategoryInfo.icon_zip_url);
                }

            }
            if (download_url_thumbs.size()>0)
            {
                for (std::string download_url_thumb : download_url_thumbs)
                {
                    if (mj::helper::isValidURL(download_url_thumb)) {
                        mj::api::ThumbDownloadInfo* thumbDownloadInfo = new mj::api::ThumbDownloadInfo{};
                        thumbDownloadInfo->downloadUrl = download_url_thumb;
                        thumbDownloadInfo->countRetryDownload = 0;
                        thumbDownloadInfos.push_back(thumbDownloadInfo);
                    }
                }
//                mj::api::MJAPI_Download_MultiFile::call(thumbDownloadInfos, [=](bool isSuccess, int, int, int) {
//
//                    },nullptr, RemoteImageView::Type::AI_SPEAKING);
            }
        }

        void MJAPIAISpeakingTopicListZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            delete this;
        }

        void MJAPIAISpeakingTopicListZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/game/list-game", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void MJAPIAISpeakingTopicListZip::send() {
            if (_request) {
                _request->send();
            }
        }
        bool MJAPIAISpeakingTopicListZip::IsTopicThumbExits(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo)
        {
            std::string	folder_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/";
            std::vector<std::string> thumbpaths;
            thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.icon_thumb));
            thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.selected_circle_icon));
            thumbpaths.push_back(folder_image + mj::helper::getFilenameFromURL(aISpeakingCategoryIconInfo.progress_bg_icon));
            for (std::string path : thumbpaths)
            {
                if (!FileUtils::getInstance()->isFileExist(path))
                    return(false);

            }
            return(true);

        }
    }
}
