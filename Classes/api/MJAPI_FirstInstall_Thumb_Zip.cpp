//
//  MJAPI_FirstInstall_Thumb_Zip.cpp
//  MJStory
//
//  Created by TruongBX on 06/01/2022.
//

#include "MJAPI_FirstInstall_Thumb_Zip.h"
#include "MsNewDownloadFile.h"
#define IMAGE_CACHE_PATH "imgcache/"
#define TIME_DOWNLOAD_THUMB 150

namespace mj {
    namespace api {
        void FisrtInstallThumb::call(ThumbDownloadInfo* thumbDownloadInfo, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            if (callback) {
                callback(true, -1, -1, -1);
            }
            return;
#endif
            auto api_call = new FisrtInstallThumb(thumbDownloadInfo);
            api_call->_callback = callback;
            api_call->_callback_progress = i_updatePercentCallback;
            api_call->onDownloadMultiZipFile();

        }

        void FisrtInstallThumb::call(std::vector<ThumbDownloadInfo*> thumbDownloadInfos, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback, RemoteImageView::Type type)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            if (callback) {
                callback(true, -1, -1, -1);
            }
            return;
#endif

            if (thumbDownloadInfos.size() <= 0)
            {
                if (callback) {
                    callback(true, -1, -1, -1);
                }
            }

            auto api_call = new FisrtInstallThumb(thumbDownloadInfos);
            api_call->_type = type;
            api_call->_callback = callback;
            api_call->_callback_progress = i_updatePercentCallback;
            api_call->onDownloadMultiZipFile();

        }

        void FisrtInstallThumb::call(std::vector<ThumbDownloadInfo*> thumbDownloadInfos, std::function<void(bool, int, int, int)> callback, std::function<void(float)> i_updatePercentCallback)
        {
            if (thumbDownloadInfos.size() <= 0)
            {
                if (callback) {
                    callback(true, -1, -1, -1);
                }
            }
            auto api_call = new FisrtInstallThumb(thumbDownloadInfos);
            api_call->_callback = callback;
            api_call->_callback_progress = i_updatePercentCallback;
            api_call->onDownloadMultiZipFile();

        }
        FisrtInstallThumb::FisrtInstallThumb(std::string keyResPath) {
            ms::utils::Downloader::init();
            //InitialDownloader();
        }

        FisrtInstallThumb::FisrtInstallThumb(ThumbDownloadInfo* thumbDownloadInfo)
        {
            ms::utils::Downloader::init();
            this->InsertThumbDownloadInfo(thumbDownloadInfo);
            //InitialDownloader();

        }

        FisrtInstallThumb::FisrtInstallThumb(std::vector<ThumbDownloadInfo*> thumbDownloadInfos)
        {
            setThumbDownloadInfoList(thumbDownloadInfos);

            //InitialDownloader();
        }

        void FisrtInstallThumb::onDownloadMultiZipFile()
        {
            _totolDownloadFile = this->getThumbDownloadInfoList().size()>0? this->getThumbDownloadInfoList().size():1;
            for (ThumbDownloadInfo* thumbDownloadInfo : this->getThumbDownloadInfoList())
            {
                onDownload(thumbDownloadInfo);
            }
            Director::getInstance()->getRunningScene()->schedule([=](float) {
                countForGotoCallBack++;
                if (countForGotoCallBack >= TIME_DOWNLOAD_THUMB)
                {
                    if (_type != RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
                    {
                        if (_callback) {
                            _callback(true, -1, -1, -1);
                            if (Director::getInstance()->getRunningScene()->isScheduled("countForGotoCallBack"))
                                Director::getInstance()->getRunningScene()->unschedule("countForGotoCallBack");
                        }
                    }
                }
                }, 1, TIME_DOWNLOAD_THUMB + 1, 1, "countForGotoCallBack");

        }

        void FisrtInstallThumb::InitialDownloader()
        {
            CCLOG("FisrtInstallThumb::InitialDownloader ");

            cocos2d::network::DownloaderHints hints = { 1,  60, ".tmp" };
          
            _downloader = std::shared_ptr<cocos2d::network::Downloader>(new cocos2d::network::Downloader(hints));
            _downloader->setOnTaskError([this](const cocos2d::network::DownloadTask& task, int error_code, int error_internal_code, const std::string& error_string) {
                auto retry_count = _retry_count.find(task.identifier) != std::end(_retry_count) ? _retry_count.at(task.identifier) : 0;
                mj::helper::pushEventCrashNetWork(task.requestURL, error_string, this->getThumbDownloadInfo()->keyResPath, error_code, retry_count);
                _retry_count[task.identifier] = retry_count + 1;
                CCLOG("cmon res error: %s", error_string.c_str());
                if (_callback) {
                    _callback(false, -1,-1,-1);
                }
                });

            _downloader->onTaskProgress = [this](const cocos2d::network::DownloadTask& task,
                int64_t bytesReceived,
                int64_t totalBytesReceived,
                int64_t totalBytesExpected) {
                    float percent = (float)totalBytesReceived / (float)totalBytesExpected;

                    CCLOG("download percentage: totalBytesReceived:%f totalBytesExpected:%f percent: %f", totalBytesReceived, totalBytesExpected, percent);

                    //if (_callback_progress)
                    //{
                    //    if (_oldPercent <= percent - 5)
                    //    {
                    //        _oldPercent = percent;
                    //        //_callback_progress(percent);

                    //    }
                    //}

            };

            _downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task) {
                std::string log = this->getThumbDownloadInfo()->keyResPath + " download success";
                CCLOG("%s", log.data());
                auto zipfile = task.storagePath;
                mj::helper::unzipAsync(zipfile, cocos2d::FileUtils::getInstance()->getWritablePath(), [this, zipfile](bool success) {
                    if (success) {
                        int request_ver = MJDEFAULT->getIntegerForKey(this->getThumbDownloadInfo()->requestVersion);
                        MJDEFAULT->setIntegerForKey(this->getThumbDownloadInfo()->currentVersion, request_ver);

                        if (_type != RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
                            cocos2d::FileUtils::getInstance()->removeFile(zipfile);


                        handlerCalBackSuccess();
                           
                    }
                    else {
                        if (_callback) {
                            _callback(false,  -1, -1, -1);
                        }
                    }
                    });

            };


        }

        void FisrtInstallThumb::onUnzipFile(std::string storagePath, ThumbDownloadInfo* thumbDownloadInfo)
        {
            CCLOG("FisrtInstallThumb::onUnzipFile ");

            this->setThumbDownloadInfo(thumbDownloadInfo);
            std::string log = thumbDownloadInfo->keyResPath + " download success";
            CCLOG("%s", log.data());
            std::string cached_image;
            if(_type == RemoteImageView::Type::AI_SPEAKING)
                cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING;
            else if (_type == RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
            {
                // if zip file is ai speaking, do not unzip file when download done
                cached_image = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING;
                handlerCalBackSuccess();
                return;
            }
            else
            {
                cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH;

            }
            auto zipfile = storagePath;
            mj::helper::unzipAsync(zipfile, cached_image, [this, zipfile](bool success) {
                if (success) {
                    int request_ver = MJDEFAULT->getIntegerForKey(this->getThumbDownloadInfo()->requestVersion);
                    MJDEFAULT->setIntegerForKey(this->getThumbDownloadInfo()->currentVersion, request_ver);

                    if (_type != RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
                        cocos2d::FileUtils::getInstance()->removeFile(zipfile);

                    handlerCalBackSuccess();
                }
                else {
                    if (_callback) {
                        _callback(false, -1, -1, -1);
                    }
                }
                });
        }

        void FisrtInstallThumb::RetryEnqueue(ThumbDownloadInfo* thumbDownloadInfo)
        {
            CCLOG("FisrtInstallThumb::RetryEnqueue ");

            ms::utils::DownloadUnit unit;
            unit.url = thumbDownloadInfo->downloadUrl;
            unit.download_id = mj::helper::getFilenameFromURL( thumbDownloadInfo->downloadUrl);
            
            if(unit.download_id == MJDEFAULT->getStringForKey(unit.download_id, ""))
            {
                _countDownloadSuceess++;
                if(_countDownloadSuceess == _totolDownloadFile)
                {
                    if (_callback) {
                        _callback(true, -1, -1, -1);
                    }
                }
                return;
            }

            if (_type == RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
            {
                unit.forderPath = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING;
                unit.storage_path = unit.forderPath + unit.download_id;
            } else
            {
                unit.forderPath = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH;
                unit.storage_path = unit.forderPath + unit.download_id;

            }
              std::string path = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING;
            if (!FileUtils::getInstance()->isDirectoryExist(path))
            {
                bool result_creatFile = FileUtils::getInstance()->createDirectory(path);
                if (!result_creatFile)
                    CCLOG("Create folder false: %s", path.c_str());
                else
                {
                    CCLOG("Create folder successful: %s", path.c_str());
                }

            }
            unit.success_event = "download " + thumbDownloadInfo->keyResPath + " success" + unit.download_id;
            unit.failed_event = "download " + thumbDownloadInfo->keyResPath + " failed" + unit.download_id;
            unit.progress_event = "download " + thumbDownloadInfo->keyResPath + " progress" + unit.download_id;
            if(ms::NewDownload::getInstance().isNewDownload())
            {
                Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.success_event, [=](EventCustom * e) {
                    auto info = (ms::utils::DownloadUnit*)e->getUserData();
                    if(info)
                    {
                        mj::helper::unzipAsync(info->storage_path, info->forderPath + unit.download_id, [=](bool success) {
                            if (success) {
                                _countDownloadSuceess++;
                                MJDEFAULT->setStringForKey(unit.download_id, unit.download_id);
                                cocos2d::FileUtils::getInstance()->removeFile(unit.storage_path);
                                if(_countDownloadSuceess == _totolDownloadFile)
                                {
                                    if (_callback) {
                                        _callback(true, -1, -1, -1);
                                    }
                                }
                            }
                            else {
                                if (_callback) {
                                    _callback(false,  -1, -1, -1);
                                }
                            }
                        });
                    }
                });

                Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.progress_event, [=](EventCustom * e) {
                    double *percent = reinterpret_cast<double*>(e->getUserData());
                    if(percent)
                    {
                        if (_callback_progress)
                        {
                            _callback_progress((float)*percent);
                        }
                    }
                });

                Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.failed_event, [=](EventCustom * e) {
                    if (_callback) {
                        _callback(false, -1,-1,-1);
                    }
                });
                ms::NewDownload::getInstance().initDownload(unit, "Onboad_ding", "THUMB_QUE_ZIP", ms::NewDownload::TYPE_LINK_URL::THUMB_ZIP);
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    ms::NewDownload::getInstance().beginDownload();
                });
                }else{
                    enqueue(unit);
                }
        }

        void FisrtInstallThumb::send() {
            int current_ver = MJDEFAULT->getIntegerForKey(this->getThumbDownloadInfo()->currentVersion, -1);
            int request_ver = MJDEFAULT->getIntegerForKey(this->getThumbDownloadInfo()->requestVersion, 0);

            //        if (true) {
            if (current_ver < request_ver) {
                auto url = MJDEFAULT->getStringForKey(this->getThumbDownloadInfo()->keyResPath, this->getThumbDownloadInfo()->defaultDownloadUrl);
                MJDEFAULT->setStringForKey(this->getThumbDownloadInfo()->downloadUrl, url);
                auto download_id = mj::helper::getFilenameFromURL(url);
                auto storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + download_id;
                auto task = _downloader->createDownloadFileTask(url, storage_path, download_id);
            }
            else {
                if (_callback) {
                    _callback(true, -1, -1, -1);
                }
                delete this;
            }
        }

        void FisrtInstallThumb::onDownload(ThumbDownloadInfo* thumbDownloadInfo) {

            CRASH_AUTO_LOG;
            CCLOG("FisrtInstallThumb::onDownload ");

            if (thumbDownloadInfo->downloadUrl == "")
                return;
            if (!NativeBridge::networkAvaiable())
            {
                auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
                auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
                alert->setNetworkDownContent(false, true);
                alert->show();
                return;
            }

            //auto url = this->getThumbDownloadInfo()->downloadUrl;
            CCLOG("link_url = %s", thumbDownloadInfo->downloadUrl.c_str());

            //trường hợp đã có zip

            auto fname = mj::helper::getFilenameFromURL(thumbDownloadInfo->downloadUrl);
            auto fdirectory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
            if (_type == RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
                fdirectory = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING;
            std::string path = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING;
            if (!FileUtils::getInstance()->isDirectoryExist(path))
            {
                bool result_creatFile = FileUtils::getInstance()->createDirectory(path);
                if (!result_creatFile)
                    CCLOG("Create folder false: %s", path.c_str());
                else
                {
                    CCLOG("Create folder successful: %s", path.c_str());
                }

            }
            auto fpath = fdirectory + fname;

            if (FileUtils::getInstance()->isFileExist(fpath)) {
                mj::helper::unzipAsync(fpath, fdirectory, [=](bool success) {
                    if (success) {
                        /*
                         * Unzip success
                         */
                        MS_LOGGER.logEvent("Unzip Success", {});
                        if (_type != RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP)
                            FileUtils::getInstance()->removeFile(fpath);
                        onDownloadAndUnzipDone();
                        //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_done");
                    }
                    else {
                        /*
                         * Unzip Error
                         * Push user_id, story_id, device_id, url_download with custom key: story_unzip_error to crashlytics.
                         */
                        MS_LOGGER.logEvent("Unzip Error", {});
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_error");
                        onUnzipError(-1, "") ;
                    }
                    });
                return;
            }
            else
            {
                CCLOG("file not exits = %s", fpath.c_str());
            }
            RetryEnqueue(thumbDownloadInfo);
        }

        void FisrtInstallThumb::onDownloadAndUnzipDone() {
            CRASH_AUTO_LOG;
//            parseData();
        }
        void FisrtInstallThumb::onUnzipError(int error_code, std::string error_string) {
            MS_LOGGER.logEvent("FisrtInstallThumb::onUnzipError", { {"Error code", cocos2d::Value(error_code)} });
            //showNetworkError(error_string);
            if (_callback) {
                _callback(false, -1, -1, -1);
            }
        }

        void FisrtInstallThumb::onDownloadError(int error_code, std::string error_string) {
            MS_LOGGER.logEvent("FisrtInstallThumb::onDownloadError", { {"Error code", cocos2d::Value(error_code)} });
           // showNetworkError(error_string);
            if (_callback) {
                _callback(false, -1, -1, -1);
            }
            }
        void FisrtInstallThumb::showNetworkError(std::string message) {
            CRASH_AUTO_LOG;
            if (message.empty()) {
                message = LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error");
            }
            auto csb_name = mj::helper::getCSBName("csb/StoryAlertPlayGame.csb");
            auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
            alert->setContent(message, "Try Again", "", false, 36);
            alert->setTimelineName("csb/StoryAlertPlayGame.csb");
            alert->show();
            //    alert->setDelegate(this);
        }
        void FisrtInstallThumb::onSuccess(const std::string& srcUrl, const std::string& storagePath, const std::string& customId)
        {
            CCLOG("FisrtInstallThumb::onSuccess ");
            _percentTotoal += _percentOneDownload;
            _oldPercent = 0;
            _percentOneDownload = 0;
            if (_callback_progress)
            {

                    _callback_progress(_percentTotoal);
            }
            // call parent method overide
            ms::utils::Downloader::onSuccess(srcUrl, storagePath, customId);
            for (ThumbDownloadInfo* thumbDownloadInfo : this->getThumbDownloadInfoList())
            {
                if (thumbDownloadInfo->downloadUrl.compare(srcUrl) == 0)
                {
                    onUnzipFile(storagePath, thumbDownloadInfo);

                }
            }
            // continue download next xip file
        }
        void FisrtInstallThumb::onError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr, bool isPushEvent)
        {
            CCLOG("FisrtInstallThumb::onError ");

            // call parent method overide
            ms::utils::Downloader::onError(task, errorCode, errorCodeInternal, errorStr);
            for (ThumbDownloadInfo* thumbDownloadInfo : this->getThumbDownloadInfoList())
            {
                if (thumbDownloadInfo->downloadUrl.compare(task.requestURL) == 0 )
                {
                    thumbDownloadInfo->countRetryDownload++;
                    if(thumbDownloadInfo->countRetryDownload<4)
                        RetryEnqueue(thumbDownloadInfo);
                    else
                    {
                        if (_callback) {
                            _callback(false, -1, -1, -1);
                        }
                    }
                }

            }
           
            // retry download zip file again (retry download 3 times)
        }
        void FisrtInstallThumb::onProgress(double total, double downloaded, const std::string& url, const std::string& customId)
        {
            CCLOG("FisrtInstallThumb::onProgress ");

            // call parent method overide
            ms::utils::Downloader::onProgress(total, downloaded, url, customId);
            _percentOneDownload = ((float)downloaded / (float)total)/ (float)_totolDownloadFile * 100;

            CCLOG("download percentage: totalBytesReceived:%f totalBytesExpected:%f percent: %f", downloaded, total, _percentOneDownload);

            if (_callback_progress)
            {
                if (_oldPercent <= _percentOneDownload - 5)
                {
                    _oldPercent = _percentOneDownload;
                    _callback_progress(_percentTotoal + _percentOneDownload);

                }
            }

        }
        void FisrtInstallThumb::handlerCalBackSuccess()
        {
            _countDownLoad++;
            if (_thumbDownloadInfos.size() >= 0 && _countDownLoad < _thumbDownloadInfos.size())
            {
                _countDownLoad++;
                return;
            }
            else
            {
                if (_callback) {
                    _callback(true, -1, -1, -1);
                    return;
                }
            }

        }
    }
}
