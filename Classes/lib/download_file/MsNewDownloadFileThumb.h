
#ifndef MsNewDownloadFileThumb_hpp
#define MsNewDownloadFileThumb_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"
#include "MSDownloader.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "cocos2d.h"
#include "RemoteImageView.h"

#define TURN_ON_NEWDOWNLOAD true

namespace ms {
    class MsNewDownloadFileThumb {
        public:
        static MsNewDownloadFileThumb& getInstance() {
            static MsNewDownloadFileThumb sharedManager;
            return sharedManager;
        };

        enum TYPE_LINK_URL {
            THUMB_QUEUE,
            THUMB_ZIP,
            LESSON,
            OTHER,
            DEFAULT
        };

        struct InfoFileDownload {
            std::string _screenName;
            std::string _tytpeUrl;
            ms::utils::DownloadUnit _unit;
            bool _islesson = false;
            TYPE_LINK_URL _typeDownLoadURL;
        };

        void initDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type = TYPE_LINK_URL::DEFAULT);

        void downloadSucess();

        void downloadError(std::vector<std::string> value, std::vector<std::string> listMessageError = {});
        
        void processDownload(double index);

        void startDownload(InfoFileDownload  infoFileDownload);

        void beginDownload();

        void clearListDownLoadStory(bool isClearLesson = false);

        bool isNewDownload();

        bool isNewDownloadAndroid();

        void clearListInfoFileDownload(std::vector<InfoFileDownload> listValue);
        void reDownload();
        
        void clearListRemoteImageView();
        
        void deleteDownloadThumb(std::string url);

        protected:
        explicit MsNewDownloadFileThumb();
        private:

        InfoFileDownload createInfoDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type);

        void continueDownloadWhenErrorURL();

        bool checkFileDownloadSucess(std::string pathFile);

        std::vector<InfoFileDownload> _listInfoDownload;
        std::vector<InfoFileDownload> _listInfoDownloadOfLessonContinue;
        std::vector<InfoFileDownload> _listInfoDownloadThumbQueue;
        std::vector<InfoFileDownload> _listInfoDownloadZipThumb;
        std::vector<RemoteImageView*> _listRemoteImageView;
        std::string _urlIsLoading = "";
        int _index = 0;
        double _precen = 0.0;
        double _indexProcess = 0.0;

    };
}
#endif
