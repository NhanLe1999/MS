//
//  MSDownloader.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/22/17.
//

#ifndef MSDownloader_hpp
#define MSDownloader_hpp

#include "cocos2d.h"
#include "network/CCDownloader.h"
#include "StoryDataModel.h"

namespace ms { namespace utils {
    struct DownloadUnit {
        std::string download_id;
        std::string storage_path;
        std::string forderPath;
        std::string url;
        std::string success_event;
        std::string failed_event;
        std::string progress_event;
        std::string error_message;

       StoryDataModel::TYPEURL typeUrl = StoryDataModel::TYPEURL::DEFAULT;
    };
    
    enum DownloadState {
        DOWNLOADING,
        PROCESSING,
        STOPPED
    };
    
    class Downloader {
    public:
        static Downloader & getInstance() {
            static Downloader sharedDownloader;
            return sharedDownloader;
        };
        enum TYPE_DOMAIN
        {
            DEFAULT = 0,
            STORY = 1,
            AUDIOBOOK_ACTIVITY_GAME = 2
        };
        void init();
        void enqueue(DownloadUnit unit);
        void unEnqueue();
        void cancel(std::string download_id);
        virtual void onSuccess(const std::string& srcUrl, const std::string& storagePath, const std::string& customId);
        virtual void onError(const cocos2d::network::DownloadTask& task,
            int errorCode,
            int errorCodeInternal,
            const std::string& errorStr, bool isPushEvent = false);
        virtual void onProgress(double total, double downloaded, const std::string& url, const std::string& customId);
        virtual void setDownloader(std::shared_ptr<cocos2d::network::Downloader> downloader) {
            _downloader = downloader;
        };
        virtual std::shared_ptr<cocos2d::network::Downloader> getDownloader() { return _downloader; };
    protected:
        explicit Downloader();
    private:
        Downloader(Downloader const &);
        void operator = (Downloader const &);
        /**
         * add a download unit and auto start downloading
         */
        
        /**
         * cancel a download
         */
        void cancel(DownloadUnit unit);
        
        /**
         * set max number of downloading tasks at a time
         */
        void setMaxConcurrentDownload(int max_concurrent_download);
        
        
    private:
        void batchDownload();
        
        std::map<std::string, DownloadUnit> _waitingUnits;
        std::map<std::string, DownloadUnit> _downloadingUnits;
        std::map<std::string, DownloadUnit> _failedUnits;
        std::map<std::string, int> _retry_count;
        std::vector<std::string> _queue;
        
        int _max_concur_download;
        int _cur_concur_download;
        bool  _isPushEvent = false;
        int numberPushEvent = 0;
        
        DownloadState _state;
        
        std::shared_ptr<cocos2d::network::Downloader> _downloader;
        
        //for tracking
        std::string _countryName = "";
        std::string _ip = "";
        StoryDataModel::TYPEURL _typeUrl = StoryDataModel::TYPEURL::DEFAULT;
        std::vector<std::pair<std::shared_ptr<cocos2d::network::Downloader>, std::shared_ptr<const cocos2d::network::DownloadTask>>> _listDownloadALL = {};
    };
}}

#endif /* MSDownloader_hpp */
