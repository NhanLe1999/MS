

#include "MsDownloadFile.h"

#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"


namespace mj {
    namespace api {
        void DownloadFilePayment::call(std::function<void(bool)> callback, std::function<void(float)> i_updatePercentCallback) {
            auto api_call = new DownloadFilePayment();
            api_call->_callback = callback;
            api_call->_callback_progress = i_updatePercentCallback;
            api_call->send();
        }

        DownloadFilePayment::DownloadFilePayment() {
            _downloader = std::shared_ptr<cocos2d::network::Downloader>(new cocos2d::network::Downloader({
                6,
                30,
                ".tmp"
                }));
            _downloader->setOnTaskError([this](const cocos2d::network::DownloadTask& task, int error_code, int error_internal_code, const std::string& error_string) {
                
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
                    }
                    else {
                       
                    }
                    });
            };

        }

        void DownloadFilePayment::send() {
            auto url = MJDEFAULT->getStringForKey("key_path_download_price");
            auto download_id = mj::helper::getFilenameFromURL(url);
            auto storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + download_id;
            auto task = _downloader->createDownloadFileTask(url, storage_path, download_id);
        }
    }
}
