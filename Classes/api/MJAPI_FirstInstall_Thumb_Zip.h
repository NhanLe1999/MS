
//
//  MJAPI_FirstInstall_Thumb_Zip.h
//  MJStory
//
//  Created by TruongBX on 06/01/2022
//

#ifndef MJAPI_FirstInstall_Thumb_Zip_hpp
#define MJAPI_FirstInstall_Thumb_Zip_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "NativeBridge.h"
#include "manager/CleverTapManager.h"
#include "APDatabase.h"
#include "HelperManager.h"
#include "MSDownloader.h"
#include "../external/json/document.h"
#include "MSEventLogger.h"
#include "cocostudiomacro2.h"
#include "StoryAlert.h"
#include "RemoteImageView.h"

namespace mj {
    namespace api {
        struct ThumbDownloadInfo
        {
            std::string downloadUrl = "";
            std::string defaultDownloadUrl = "";
            std::string backupDownloadUrl1 = "";
            std::string backupDownloadUrl2 = "";
            std::string currentVersion = key_thumb_res_version_current;
            std::string requestVersion = key_thumb_res_version_request;
            std::string keyResPath = key_thumb_res_path;
            std::string contentDownload = key_thumb_res_path;

            int countRetryDownload = 0;
        };
        class FisrtInstallThumb :public ms::utils::Downloader{
        public:
            static void call(ThumbDownloadInfo* thumbDownloadInfo, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback = nullptr);
            static void call(std::vector<ThumbDownloadInfo*>thumbDownloadInfos, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback = nullptr);
            static void call(std::vector<ThumbDownloadInfo*>thumbDownloadInfos, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback = nullptr, RemoteImageView::Type type = RemoteImageView::Type::OTHER);

            void setThumbDownloadInfo(ThumbDownloadInfo* thumbDownloadInfo) {
                    _thumbDownloadInfo = thumbDownloadInfo;
            };
            ThumbDownloadInfo* getThumbDownloadInfo() { return _thumbDownloadInfo; };

            void setThumbDownloadInfoList(std::vector<ThumbDownloadInfo*> thumbDownloadInfos) {
                _thumbDownloadInfos = thumbDownloadInfos;
            };
            std::vector<ThumbDownloadInfo*> getThumbDownloadInfoList() { return _thumbDownloadInfos; };
            void InsertThumbDownloadInfo(ThumbDownloadInfo* thumbDownloadInfo) {
                _thumbDownloadInfos.push_back(thumbDownloadInfo);
            };

        private:
            void send();
            void onDownload(ThumbDownloadInfo* thumbDownloadInfo);
            void onDownloadAndUnzipDone();
            void onUnzipError(int error_code, std::string error_string);
            void onDownloadError(int error_code, std::string error_string);
            void showNetworkError(std::string message);
            void onSuccess(const std::string& srcUrl, const std::string& storagePath, const std::string& customId) override;
            void onError(const cocos2d::network::DownloadTask& task,
                int errorCode,
                int errorCodeInternal,
                const std::string& errorStr, bool isPushEvent = false) override;
            void onProgress(double total, double downloaded, const std::string& url, const std::string& customId) override;
            void handlerCalBackSuccess();
            FisrtInstallThumb(std::string keyResPath);
            FisrtInstallThumb(ThumbDownloadInfo* thumbDownloadInfo);
            FisrtInstallThumb(std::vector<ThumbDownloadInfo*> thumbDownloadInfos);

            void onDownloadMultiZipFile();
            void InitialDownloader();
            void onUnzipFile(std::string storagePath, ThumbDownloadInfo* thumbDownloadInfo);
            void RetryEnqueue(ThumbDownloadInfo* thumbDownloadInfo);

            std::function<void(bool, int, int, int)> _callback;
            std::function<void(float)> _callback_progress;
            std::shared_ptr<cocos2d::network::Downloader> _downloader;
            ThumbDownloadInfo* _thumbDownloadInfo;
            std::vector<ThumbDownloadInfo*>_thumbDownloadInfos;
            int countForGotoCallBack = 0;
            int _totolDownloadFile = 1;
            float _percentOneDownload = 0, _percentTotoal = 0;
            std::map<std::string, int> _retry_count;
            RemoteImageView::Type _type;
            int _countDownLoad = 0;
            int _oldPercent = 0;
            int _countDownloadSuceess = 0;

        };

    }

}
#endif /* MJAPI_FirstInstall_Thumb_Zip */