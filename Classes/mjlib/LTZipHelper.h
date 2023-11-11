//
//  LTZipHelper.hpp
//  CocosTest
//
//  Created by To Ngoc Linh on 4/14/17.
//
//

#ifndef LTZipHelper_hpp
#define LTZipHelper_hpp

#include <cstdio>
#include <queue>
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"

class LTZipHelperDelegate;

class LTZipHelper : public cocos2d::Ref {
public:

    enum DownloadState {
        STOP,
        DOWNLOADING,
        UNZIPPING
    };

    static LTZipHelper & getInstance() {
        static LTZipHelper zipHelperInstance;
        return zipHelperInstance;
    }

    LTZipHelper(LTZipHelper const&) = delete;
    LTZipHelper(LTZipHelper &&) = delete;
    LTZipHelper& operator = (LTZipHelper const&) = delete;
    LTZipHelper& operator = (LTZipHelper &&) = delete;

    void startDownloadToFolder(const std::string &url, const std::string folder, LTZipHelperDelegate * delegate);
    void stopDownload();
    bool unzip(const std::string &file);
    void startCopyToFolder(const std::string path_file, const std::string folder, LTZipHelperDelegate * delegate);
private:
    LTZipHelper();

    CC_SYNTHESIZE_READONLY(DownloadState, downloadState, DownloadState);
    CC_SYNTHESIZE(LTZipHelperDelegate *, _delegate, Delegate);

    void onDownloadTaskError(const cocos2d::network::DownloadTask & task, int errorCode, int errorCodeInternal, const std::string & errorString);
    void onDownloadSuccess(const cocos2d::network::DownloadTask & task);
    void onDownloadProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);

   
    std::string basename(const std::string &file) const;
    std::string _folder;

    std::shared_ptr<cocos2d::network::Downloader> _downloader;
    std::string filename;
};

class LTZipHelperDelegate {
public:
    virtual void onDownloadError(int errorCode, std::string message) {}
    virtual void onUnzipError(int errorCode, std::string) {}
    virtual void onDownloadSuccess() {}
    virtual void onUnzipSuccess() {}
    virtual void onProgressChange(LTZipHelper::DownloadState download_state, float percent) {}
};

#endif /* LTZipHelper_hpp */
