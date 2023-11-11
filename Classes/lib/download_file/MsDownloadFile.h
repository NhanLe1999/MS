
#ifndef Ms_Download_file_hpp
#define Ms_Download_file_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"

namespace mj {
    namespace api {
        class DownloadFilePayment {
		public:
            static void call(std::function<void(bool)> callback = nullptr, std::function<void(float)> i_updatePercentCallback  = nullptr);
        private:
            void send();
            DownloadFilePayment();
            std::function<void(bool)> _callback;
            std::function<void(float)> _callback_progress;
            
            std::shared_ptr<cocos2d::network::Downloader> _downloader;
        };
    }
}
#endif
