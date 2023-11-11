//
//  MSDownloader.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/22/17.
//

#include "MSDownloader.h"
#define DEFAULT_CONNECTION_TIMEOUT 60
#define MAX_RETRY 1
#include "MSEventLogger.h"
#include "MJDefault.h"
#include "base64.h"
#include "MJDefinitions.h"

USING_NS_CC;

namespace ms { namespace  utils {
    Downloader::Downloader() {
        _max_concur_download = 1;
        _cur_concur_download = 0;
        network::DownloaderHints hints = {
            static_cast<uint32_t>(_max_concur_download),
            DEFAULT_CONNECTION_TIMEOUT,
            ".tmp"
        };
        _downloader = std::shared_ptr<network::Downloader>(new network::Downloader(hints));
        _downloader->setOnTaskError([this](const network::DownloadTask & task, int error_code, int error_internal_code, const std::string & error_string) {
            auto unit = _downloadingUnits.at(task.identifier);
            auto retry_count = _retry_count.find(task.identifier) != std::end(_retry_count) ? _retry_count.at(task.identifier) : 0;
            if (retry_count > MAX_RETRY) {
                this->onError(task, error_code, error_internal_code, error_string, _isPushEvent);
            } else {
                CCLOG("retrying... %s", unit.download_id.c_str());
                _retry_count[task.identifier] = retry_count + 1;
                ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartDownloading, {
                    {"item_name", cocos2d::Value(unit.url.empty()?"url_empty":unit.url)},
                    {"download_id", cocos2d::Value(unit.download_id.empty()?"download_id_empty":unit.download_id)}
                });
                _isPushEvent = true;
                _downloader->createDownloadFileTask(unit.url, unit.storage_path, unit.download_id);
            }
        });
        _downloader->onTaskProgress = [this](const network::DownloadTask& task,
                                             int64_t /*bytesReceived*/,
                                             int64_t totalBytesReceived,
                                             int64_t totalBytesExpected) {
            this->onProgress(totalBytesExpected, totalBytesReceived, task.requestURL, task.identifier);
        };
        _downloader->onFileTaskSuccess = [this](const network::DownloadTask& task) {
            this->onSuccess(task.requestURL, task.storagePath, task.identifier);
        };
    }

    bool isValidUnit(ms::utils::DownloadUnit unit){
        return !(unit.download_id.empty() || unit.storage_path.empty() || unit.url.empty());
    }

    void Downloader::init() {
        
        _countryName    =   MJDEFAULT->getStringForKey("key_country_code","vi-VN");
    }

    void Downloader::enqueue(ms::utils::DownloadUnit unit) {
        
        std::string alpBName = unit.url;
        _typeUrl = unit.typeUrl;
    
        alpBName.erase(remove_if(alpBName.begin(), alpBName.end(), [](char c) { return !isalpha(c) &&  !isdigit(c); } ), alpBName.end());
        
        if(!MJDEFAULT->getBoolForKey("is_log_enable",false)) {
       
            //#FIXME: to fix bugs.
            ms::EventLogger::getInstance().logEvent("downloader_enqueue_temp", {
                {"item_name", cocos2d::Value(alpBName.empty()?"url_empty":alpBName)},
            });
            
            ms::EventLogger::getInstance().logEvent("downloader_enqueue", {
                {"country", cocos2d::Value(_countryName.empty()?"empty":_countryName)},
                {"item_name", cocos2d::Value(unit.url.empty()?"url_empty":unit.url)},
                {"download_id", cocos2d::Value(unit.download_id.empty()?"download_id_empty":unit.download_id)}
            });
            
        }
        
        if(!isValidUnit(unit)) return;
        
        if (FileUtils::getInstance()->isFileExist(unit.storage_path)) {
            _downloadingUnits.emplace(unit.download_id, unit);
            //fake downloading
            this->onSuccess(unit.url, unit.storage_path, unit.download_id);
            return;
        }
        CCLOG("enqueue %s", unit.download_id.c_str());
        for (auto i : _downloadingUnits) {
            CCLOG("downloading: %s", i.first.c_str());
        }
        if (_waitingUnits.find(unit.download_id) != std::end(_waitingUnits)) {
            return;
        }
        if (_downloadingUnits.find(unit.download_id) != std::end(_downloadingUnits)) {
            return;
        }
        _waitingUnits.emplace(unit.download_id, unit);
        _queue.insert(_queue.begin(), unit.download_id);
        batchDownload();
    }
    
    void Downloader::cancel(std::string download_id) {
        auto it = _waitingUnits.find(download_id);
        if (it != _waitingUnits.end()) {
            _waitingUnits.erase(it);
        }
        
        it = _downloadingUnits.find(download_id);
        if (it != _downloadingUnits.end()) {
            _downloadingUnits.erase(it);
        }
        
        it = _failedUnits.find(download_id);
        if (it != _failedUnits.end()) {
            _failedUnits.erase(it);
        }
    }
    
    void Downloader::batchDownload() {
        while (_cur_concur_download < _max_concur_download && _queue.size() > 0) {
            auto download_id = _queue.back();
            _queue.pop_back();
            CCLOG("start %s", download_id.c_str());
            auto unit = _waitingUnits.at(download_id);

            _waitingUnits.erase(download_id);
            _cur_concur_download++;
            _downloadingUnits.emplace(download_id, unit);
            
            CCLOG("unit.url:%s",unit.url.c_str());
            MJDEFAULT->setStringForKey(key_link_url_download, unit.url);
            
            if(!MJDEFAULT->getBoolForKey("is_log_enable",false)) {
                
                ms::EventLogger::getInstance().logEvent("ms_start_downloading_temp", {
                    {ms::analytics::kDownloadItemName, cocos2d::Value(unit.url.empty()?"url_empty":base64::encode(unit.url))},
                    {"country", cocos2d::Value(MJDEFAULT->getStringForKey(key_country_code,"none"))},
                    {"download_id", cocos2d::Value(unit.download_id.empty()?"download_id_empty":unit.download_id)}
                });
                
                ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartDownloading, {
                    {ms::analytics::kDownloadItemName, cocos2d::Value(unit.url.empty()?"url_empty":unit.url)},
                    {"download_id", cocos2d::Value(unit.download_id.empty()?"download_id_empty":unit.download_id)}
                });
            }

            if (FileUtils::getInstance()->isFileExist(unit.storage_path)) {
                this->onSuccess(unit.url, unit.storage_path, unit.download_id);
                return;
            }
            
            auto task = _downloader->createDownloadFileTask(unit.url, unit.storage_path, unit.download_id);

           // _downloader->stopDownloadFile();
     

            _listDownloadALL.push_back({ _downloader , task});

        }
    }

    void Downloader::unEnqueue()
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
       /* for (int i = 0; i < _listDownloadALL.size(); i++)
        {
            _listDownloadALL[i].first->stopDownloadFile();
            std::string pathCache = _listDownloadALL[i].second->storagePath + ".tmp";
            if (FileUtils::getInstance()->isFileExist(pathCache))
            {
                FileUtils::getInstance()->removeFile(pathCache);
                FileUtils::getInstance()->removeDirectory(pathCache);
            }
            
        }

        for (int i = 0; i < _queue.size(); i++)
        {
            std::string id = _queue[i];

            if (_waitingUnits.count(id) > 0)
            {
                auto task = _waitingUnits.at(id);
                std::string pathCache = task.storage_path + ".tmp";
                if (FileUtils::getInstance()->isFileExist(pathCache))
                {
                    FileUtils::getInstance()->removeFile(pathCache);
                }
            }
        }

        _failedUnits.clear();
        _downloadingUnits.clear();
        _waitingUnits.clear();
        _queue.clear();
        _listDownloadALL.clear();
        _cur_concur_download = 0;*/
#endif
    }
    
    void Downloader::onError(const cocos2d::network::DownloadTask &task, int errorCode, int errorCodeInternal, const std::string &errorStr, bool isPushEvent) {
        CCLOG("download error %s", errorStr.c_str());
        if (_typeUrl == StoryDataModel::TYPEURL::STORY)
        {
            MJDEFAULT->setStringForKey(key_error_url_stories, key_error_url_stories);
        }
        else if(_typeUrl == StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME) {
            MJDEFAULT->setStringForKey(key_error_url_stories, key_error_url_game_audio_book);
        }
        numberPushEvent++;
      //  if (numberPushEvent % 2 == 0)
        {
            auto countRetry = MJDEFAULT->getIntegerForKey(key_count_retry, 0);
            mj::helper::pushEventCrashNetWork(task.requestURL, errorStr, "dowload", errorCode, countRetry);
            MJDEFAULT->setIntegerForKey(key_count_retry, ++countRetry);
        }
        _isPushEvent = false;

        auto result =  _downloadingUnits.find(task.identifier);

        if(result == _downloadingUnits.end())
        {
            return;//not found
        }

        auto unit = _downloadingUnits.at(task.identifier);
        unit.error_message = errorStr;
        _failedUnits.emplace(task.identifier, unit);
        _downloadingUnits.erase(task.identifier);
        _cur_concur_download = std::max(0, _cur_concur_download - 1);
        batchDownload();
        
//        CCLOG("dispatch %s", unit.failed_event.c_str());
        EventCustom e(unit.failed_event);
        e.setUserData(&unit);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    }
    
    void Downloader::onSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId) {
        MJDEFAULT->deleteValueForKey(key_count_retry);
        CCLOG("success download: %s", srcUrl.c_str());
        _cur_concur_download = std::max(0, _cur_concur_download - 1);
        

        auto result =  _downloadingUnits.find(customId);

        if(result == _downloadingUnits.end())
        {
            return;//not found
        }

        //TODO: notify to ui thread
        auto unit = _downloadingUnits.at(customId);
        
        if(!MJDEFAULT->getBoolForKey("is_log_enable",false)) {
       
            ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishDownloading, {
                {"item_name", cocos2d::Value(unit.url.empty()?"url_empty":unit.url)},
                {"download_id", cocos2d::Value(unit.download_id.empty()?"download_id_empty":unit.download_id)
                }
            });
        }
        
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([unit]() {
            auto u = unit;
            EventCustom e(u.success_event);
            e.setUserData(&u);
            
            
            if(Director::getInstance()->getEventDispatcher()->hasEventListener(e.getEventName()))
            {
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
            }
        });
        
        _downloadingUnits.erase(customId);
        batchDownload();
    }
    
    void Downloader::onProgress(double total, double downloaded, const std::string &url, const std::string &customId) {
        CCLOG("download %s %f", customId.c_str(), downloaded);

        auto result =  _downloadingUnits.find(customId);

        if(result == _downloadingUnits.end())
        {
            return;//not found
        }

        auto unit = _downloadingUnits.at(customId);
        if (unit.progress_event.empty()) {
            return;
        }
        EventCustom e(unit.progress_event);
        double * progress = new double;
        * progress = downloaded / total * 100.f;
        e.setUserData(progress);
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
        delete progress;
    }
}}
