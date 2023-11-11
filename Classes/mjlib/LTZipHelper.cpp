//
//  LTZipHelper.cpp
//  CocosTest
//
//  Created by To Ngoc Linh on 4/14/17.
//
//

#include "LTZipHelper.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "base/CCAsyncTaskPool.h"

#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include <../external/unzip/unzip.h>
#endif
#include "NativeBridge.h"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

LTZipHelper::LTZipHelper() {
    downloadState = DownloadState::STOP;
    _downloader = std::shared_ptr<cocos2d::network::Downloader>(new cocos2d::network::Downloader);
    _downloader->onTaskError = std::bind(&LTZipHelper::onDownloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&LTZipHelper::onDownloadSuccess, this, std::placeholders::_1);
    _downloader->onTaskProgress = std::bind(&LTZipHelper::onDownloadProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

void LTZipHelper::startCopyToFolder(const std::string path_file, const std::string folder, LTZipHelperDelegate * delegate){
    this->_delegate = delegate;
    _folder = folder;
    auto storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + folder;
    cocos2d::FileUtils::getInstance()->createDirectory(storage_path);
    
    long timex = time(nullptr);
    filename = cocos2d::StringUtils::format("/%ld.zip", timex);
    
    FileUtils::getInstance()->getDataFromFile(path_file, [=](Data d) {
        CCLOG("Read file Completed %s", path_file.c_str());
        auto fullDestinationPath = FileUtils::getInstance()->getWritablePath() + _folder + filename;
        FileUtils::getInstance()->writeDataToFile(d, fullDestinationPath, [=](bool isSucceed) {
            Director::getInstance()->getScheduler()->schedule([=](float) {
                auto zipPath = cocos2d::FileUtils::getInstance()->getWritablePath()+ _folder + filename;
                this->unzip(zipPath);
                cocos2d::FileUtils::getInstance()->removeFile(zipPath);
            }, Application::getInstance(), 0, 0, 2.0f, false, "delay_unzip_file");
            
        });
    });
}

void LTZipHelper::startDownloadToFolder(const std::string &url, const std::string folder, LTZipHelperDelegate * delegate) {
    if (downloadState != DownloadState::STOP) {
        return;
    }
    _downloader.reset(new cocos2d::network::Downloader());

    this->_delegate = delegate;
    _folder = folder;
    auto storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + folder;
    cocos2d::FileUtils::getInstance()->createDirectory(storage_path);
//    long timex = time(nullptr);
//    filename = cocos2d::StringUtils::format("/%ld.zip", timex);
    storage_path +=  filename;
    CCLOG("%s", storage_path.c_str());
    _downloader->onTaskError = std::bind(&LTZipHelper::onDownloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&LTZipHelper::onDownloadSuccess, this, std::placeholders::_1);
    _downloader->onTaskProgress = std::bind(&LTZipHelper::onDownloadProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->createDownloadFileTask(url, storage_path);
}

void LTZipHelper::onDownloadTaskError(const cocos2d::network::DownloadTask &task, int errorCode, int errorCodeInternal, const std::string &errorString) {
    if (_delegate) {
        _delegate->onDownloadError(errorCode, errorString);
    }
    downloadState = DownloadState::STOP;
}

void LTZipHelper::onDownloadProgress(const cocos2d::network::DownloadTask &task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected) {
    if (_delegate) {
        _delegate->onProgressChange(DownloadState::DOWNLOADING, totalBytesReceived * 100.0 / totalBytesExpected);
    }
}

void LTZipHelper::onDownloadSuccess(const cocos2d::network::DownloadTask &task) {
    bool * unzipResult = new bool;
    * unzipResult = false;

    if (_delegate) {
        _delegate->onDownloadSuccess();
    }

    cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_UNZIP, [=](void * param) {
        auto uzResult = reinterpret_cast<bool *>(param);
        if (uzResult) {
            if (_delegate) {
                _delegate->onUnzipSuccess();
            }else{
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("unzip_done");
            }
        } else {
            if (_delegate) {
                _delegate->onUnzipError(-1, "UNZIP error");
            }
        }
        downloadState = DownloadState::STOP;
    }, unzipResult, [=]() {
        auto zipPath = cocos2d::FileUtils::getInstance()->getWritablePath()+ _folder + filename;
        * unzipResult = this->unzip(zipPath);
        cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    });
}

bool LTZipHelper::unzip(const std::string &zip) {
    downloadState = DownloadState::UNZIPPING;
    // Find root path for zip file
    size_t pos = zip.find_last_of("/\\");
    if (pos == std::string::npos) {
        CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
        return false;
    }
    const std::string rootPath = zip.substr(0, pos+1);

    // Open the zip file
    unzFile zipfile = cocos2d::unzOpen(cocos2d::FileUtils::getInstance()->getSuitableFOpen(zip).c_str());
    if (! zipfile) {
        CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
        return false;
    }

    // Get info about the zip file
    cocos2d::unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK) {
        CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
        cocos2d::unzClose(zipfile);
        return false;
    }

    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i) {
        if (_delegate) {
            _delegate->onProgressChange(DownloadState::UNZIPPING, (i + 1) * 100.0 / global_info.number_entry);
        }
        // Get info about current file.
        cocos2d::unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK) {
            CCLOG("AssetsManagerEx : can not read compressed file info\n");
            cocos2d::unzClose(zipfile);
            return false;
        }
        const std::string fullPath = rootPath + fileName;

        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/') {
            //There are not directory entry in some case.
            //So we need to create directory when decompressing file entry
            if ( !cocos2d::FileUtils::getInstance()->createDirectory(basename(fullPath))) {
                // Failed to create directory
                CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
                cocos2d::unzClose(zipfile);
                return false;
            }
        } else {
            // Create all directories in advance to avoid issue
            std::string dir = basename(fullPath);
            if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(dir)) {
                if (!cocos2d::FileUtils::getInstance()->createDirectory(dir)) {
                    // Failed to create directory
                    CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
                    cocos2d::unzClose(zipfile);
                    return false;
                }
            }
            // Entry is a file, so extract it.
            // Open current file.
            if (cocos2d::unzOpenCurrentFile(zipfile) != UNZ_OK) {
                CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
                cocos2d::unzClose(zipfile);
                return false;
            }

            // Create a file to store current file.
            FILE *out = fopen(cocos2d::FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
            if (!out) {
                CCLOG("AssetsManagerEx : can not create decompress destination file %s (errno: %d)\n", fullPath.c_str(), errno);
                cocos2d::unzCloseCurrentFile(zipfile);
                cocos2d::unzClose(zipfile);
                return false;
            }

            // Write current file content to destinate file.
            int error = UNZ_OK;
            do {
                error = cocos2d::unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0) {
                    CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
                    fclose(out);
                    cocos2d::unzCloseCurrentFile(zipfile);
                    cocos2d::unzClose(zipfile);
                    return false;
                }

                if (error > 0) {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);

            fclose(out);
        }

        cocos2d::unzCloseCurrentFile(zipfile);

        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry) {
            if (cocos2d::unzGoToNextFile(zipfile) != UNZ_OK) {
                CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
                cocos2d::unzClose(zipfile);
                return false;
            }
        }
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        NativeBridge::addSkipBackupAttributeToItemAtPath(fullPath);
#endif
    }
    cocos2d::unzClose(zipfile);
    return true;
}

void LTZipHelper::stopDownload() {
    _downloader.reset(new cocos2d::network::Downloader());
}

std::string LTZipHelper::basename(const std::string& path) const {
    size_t found = path.find_last_of("/\\");

    if (std::string::npos != found) {
        return path.substr(0, found);
    } else {
        return path;
    }
}
