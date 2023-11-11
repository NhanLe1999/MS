
#ifndef MsNewDownloadFile_hpp
#define MsNewDownloadFile_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"
#include "MSDownloader.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "cocos2d.h"
#include "RemoteImageView.h"

#define TURN_ON_NEWDOWNLOAD true

namespace ms {
    class NewDownload {
        public:
        static NewDownload& getInstance() {
            static NewDownload sharedManager;
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

//            InfoFileDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type)
//            {
//                _unit = unit;
//                _screenName = screenName;
//                _tytpeUrl = typeUrl;
//                _typeDownLoadURL = type;
//            }
        };

        void initDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type = TYPE_LINK_URL::DEFAULT);

        void downloadSucess(InfoFileDownload info, bool isFileNull = false);

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
        explicit NewDownload();
        private:

        InfoFileDownload createInfoDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type);

        void continueDownloadWhenErrorURL();

        bool checkFileDownloadSucess(std::string pathFile);

        void deleteFileDownloadSucess(InfoFileDownload info);

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
