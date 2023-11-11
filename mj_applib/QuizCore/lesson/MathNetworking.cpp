//
//  MathNetworking.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 12/3/17.
//

#include "MathNetworking.h"

#include "network/HttpClient.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "json/prettywriter.h"
#include "json/filewritestream.h"
#include <cstdio>

#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include <../external/unzip/unzip.h>
#endif

#define ZIP_BUFFER_SIZE    8192
#define ZIP_MAX_FILENAME   512

using namespace mj::network;

#pragma mark - Requests to server
void api::Helper::getCategoriesFromServer(requestCategoriesCallback func) {
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl("http://api.daybehoc.com/api/v1/mobile/categories/list?app_id=50&device_id=111222&os=ios&info=info&app_ver=0&device_type=2&lang_id=1&lang=en-US");
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            auto buffer = response->getResponseData();
            auto categories = mj::network::api::Helper::getCategoryList((*buffer).data());
            func(categories);
        }
    });
    request->setTag("Get categories");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void api::Helper::getActivitiesFromServer(requestActivitiesCallback func, Category category, Lession lession) {
    std::string url = "";
    
    if (lession.isNull()) {
        url = cocos2d::StringUtils::format("http://api.daybehoc.com/api/v1/mobile/activities/list?categories_id=%d&app_id=50&device_id=111222&os=ios&info=info&app_ver=0&device_type=2&lang_id=1&lang=en-US", category._id);
    } else {
        url = cocos2d::StringUtils::format("http://api.daybehoc.com/api/v1/mobile/activities/list?categories_id=%d&lesson_id=%d&app_id=50&device_id=111222&os=ios&info=info&app_ver=0&device_type=2&lang_id=1&lang=en-US", category._id, lession._id);
    }
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl(url);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            auto buffer = response->getResponseData();
            auto activities = mj::network::api::Helper::getActivityList((*buffer).data());
            func(activities);
        }
    });
    request->setTag("GET Activities");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void api::Helper::getLessionsFromServer(requestLesionsCallback func, Category category) {
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl(cocos2d::StringUtils::format("http://api.daybehoc.com/api/v1/mobile/lessson/list?categories_id=%d&app_id=50&device_id=111222&os=ios&info=info&app_ver=0&device_type=2&lang_id=1&lang=en-US", category._id));
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            auto buffer = response->getResponseData();
            auto lessions = mj::network::api::Helper::getLessionList((*buffer).data());
            func(lessions);
        }
    });
    request->setTag("Get categories");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void api::Helper::getGamesFromServer(requestGamesCallback func) {
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl("http://api.daybehoc.com/api/v1/mobile/game/list?app_id=50&device_id=111222&os=ios&info=info&app_ver=0&device_type=2&lang_id=1&lang=en-US");
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            auto buffer = response->getResponseData();
            auto games = mj::network::api::Helper::getGameList((*buffer).data());
            func(games);
        }
    });
    request->setTag("Get games");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void api::Helper::downloadActivityZip (Activity activity, std::function<void(const cocos2d::network::DownloadTask& task)> success_call, std::function<void(const cocos2d::network::DownloadTask& task, int errorCode)> error_call) {
    auto path = mj::ZipUtils::getZipPath();
    
    cocos2d::network::Downloader * downloader = new cocos2d::network::Downloader();
    downloader->createDownloadFileTask(activity.zip_link.c_str(), cocos2d::StringUtils::format("%sac_%05d.zip",path.c_str(), activity._id).c_str());
    
    downloader->onFileTaskSuccess = ([=] (const cocos2d::network::DownloadTask& task) {
        success_call(task);
    });
    
    downloader->onTaskError = ([=] (const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr) {
        error_call(task, errorCode);
    });
}


#pragma mark - Category filter
std::vector<api::Category> api::Helper::getGrades(std::vector<Category> categories) {
    std::vector<api::Category> grades;
    
    for (Category category : categories) {
        if (category.parent_id == 0) {
            grades.push_back(category);
        }
    }
    
    return grades;
}

std::vector<api::Category> api::Helper::getSubCategories(api::Category parent, std::vector<api::Category> categories) {
    std::vector<api::Category> sub_categories;
    
    for (Category category : categories) {
        if (category.parent_id == parent._id) {
            sub_categories.push_back(category);
        }
    }
    
    return sub_categories;
}


#pragma mark - Parse data response
std::vector<api::Game> api::Helper::getGameList(char* data) {
    std::vector<api::Game> games;
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(data);
    if (!document.IsObject()) {
        return games;
    }

    std::string status = document.FindMember("status")->value.GetString();
    if (status == "success") {
        rapidjson::Value& data_games = document.FindMember("data")->value;
        for (rapidjson::SizeType i = 0; i < data_games.Size(); i++) {
            rapidjson::Value& game_json = data_games[i];
            api::Game game;
            game._id = game_json.FindMember("id")->value.GetInt();
            game.name = game_json.FindMember("name")->value.GetString();
            game.config_path = game_json.FindMember("path_config")->value.GetString();
            games.push_back(game);
        }
    }
    return games;
}

std::vector<api::Category> api::Helper::getCategoryList(char *data) {
    std::vector<api::Category> categories;
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(data);
    if (!document.IsObject()) {
        return categories;
    }
    
    std::string status = document.FindMember("status")->value.GetString();
    if (status == "success") {
        rapidjson::Value& data_catgories = document.FindMember("data")->value;
        for (rapidjson::SizeType i = 0; i < data_catgories.Size(); i++) {
            rapidjson::Value& game_json = data_catgories[i];
            api::Category category;
            category._id = game_json.FindMember("id")->value.GetInt();
            category.name = game_json.FindMember("name")->value.GetString();
            
            rapidjson::Value& parent_id = game_json.FindMember("parent_id")->value;
            if (parent_id.IsNull()) {
                category.parent_id = 0;
            } else {
                category.parent_id = parent_id.GetInt();
            }
            categories.push_back(category);
        }
    }
    return categories;
}

std::vector<api::Lession> api::Helper::getLessionList(char *data) {
    std::vector<api::Lession> lessions;
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(data);
    if (!document.IsObject()) {
        return lessions;
    }
    
    std::string status = document.FindMember("status")->value.GetString();
    if (status == "success") {
        rapidjson::Value& data_lessions = document.FindMember("data")->value;
        for (rapidjson::SizeType i = 0; i < data_lessions.Size(); i++) {
            rapidjson::Value& game_json = data_lessions[i];
            api::Lession lession;
            lession._id = game_json.FindMember("id")->value.GetInt();
            lession.name = game_json.FindMember("title")->value.GetString();
            lession.order_by = game_json.FindMember("order_by")->value.GetInt();
            lessions.push_back(lession);
        }
    }
    return lessions;
}

std::vector<api::Activity> api::Helper::getActivityList(char *data) {
    std::vector<api::Activity> activities;
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(data);
    if (!document.IsObject()) {
        return activities;
    }
    
    std::string status = document.FindMember("status")->value.GetString();
    if (status == "success") {
        rapidjson::Value& data_activities = document.FindMember("data")->value;
        for (rapidjson::SizeType i = 0; i < data_activities.Size(); i++) {
            rapidjson::Value& game_json = data_activities[i];
            api::Activity activity;
            activity._id = game_json.FindMember("activity_id")->value.GetInt();
            activity.name = game_json.FindMember("name")->value.GetString();
            activity.zip_link = game_json.FindMember("url_download")->value.GetString();
            if (game_json.HasMember("game_id")) {
                activity.game_id = game_json.FindMember("game_id")->value.GetInt();
            }
            if (game_json.HasMember("game_name")) {
                activity.game_name = game_json.FindMember("game_name")->value.GetString();
            }
            activities.push_back(activity);
        }
    }
    return activities;
}

std::string mj::ZipUtils::getZipPath() {
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath()+"zips/";
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(path);
    }
    return path;
}

std::string mj::ZipUtils::getConfigPath(api::Activity activity) {
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath()+"games/";
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(path);
    }
    if (activity.isNull()) {
        return path;
    }
    return cocos2d::StringUtils::format("%sac_%05d/", path.c_str(), activity._id);
}

std::string mj::ZipUtils::getIconsPath() {
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath()+"images/";
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(path);
    }
    return path;
}

#pragma mark - unzip
std::string mj::ZipUtils::getFilenameFromURL(std::string url, bool include_ext) {
    auto pos = url.find_last_of('/');
    auto temp = url.substr(pos + 1);
    pos = temp.find_first_of('?');
    if (pos != std::string::npos) {
        temp = temp.substr(0, pos);
    }
    if (include_ext) {
        return temp;
    } else {
        size_t found = temp.find_last_of('.');
        if (std::string::npos != found) {
            return temp.substr(0, found);
        } else {
            return temp;
        }
    }
}

std::string mj::ZipUtils::getBasenameFromURL(const std::string& path) {
    size_t found = path.find_last_of("/\\");
    
    if (std::string::npos != found) {
        return path.substr(0, found);
    } else {
        return path;
    }
}

bool mj::ZipUtils::unzip(std::string zip, std::string directory) {
    // Find root path for zip file
    size_t pos = zip.find_last_of("/\\");
    if (pos == std::string::npos) {
        CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
        return false;
    }
    const std::string rootPath = directory;
    
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
    char readBuffer[ZIP_BUFFER_SIZE];
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i) {
        // Get info about current file.
        cocos2d::unz_file_info fileInfo;
        char fileName[ZIP_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  ZIP_MAX_FILENAME,
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
            if ( !cocos2d::FileUtils::getInstance()->createDirectory(getBasenameFromURL(fullPath))) {
                // Failed to create directory
                CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
                cocos2d::unzClose(zipfile);
                return false;
            }
        } else {
            // Create all directories in advance to avoid issue
            std::string dir = getBasenameFromURL(fullPath);
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
                error = cocos2d::unzReadCurrentFile(zipfile, readBuffer, ZIP_BUFFER_SIZE);
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
    }
    cocos2d::unzClose(zipfile);
    return true;
}

void mj::ZipUtils::unzipAsync(std::string zip_file, std::string directory, std::function<void(bool)> callback) {
    bool * unzipResult = new bool;
    * unzipResult = false;
    
    cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_OTHER, [=](void * param) {
        auto uzResult = reinterpret_cast<bool *>(param);
        if (uzResult) {
            if (callback) {
                callback(true);
            }
        } else {
            if (callback) {
                callback(false);
            }
        }
    }, unzipResult, [=]() {
        * unzipResult = mj::ZipUtils::unzip(zip_file, directory);
    });
}



