

#include "MsNewDownloadFile.h"
#include "MsNewDownloadFileThumb.h"

#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

#include "MSCacheManager.h"
#include "StoryInfoCover.h"
#include "CleverTapManager.h"

namespace ms {
    NewDownload::NewDownload() {

    }

    NewDownload::InfoFileDownload NewDownload::createInfoDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type)
    {
        InfoFileDownload infoDownload;
        infoDownload._unit = unit;
        infoDownload._screenName = screenName;
        infoDownload. _tytpeUrl = typeUrl;
        infoDownload._typeDownLoadURL = type;

        return infoDownload;
    }

    void NewDownload::initDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type) {

        std::string urlError = MJDEFAULT->getStringForKey("link_download_story_error", "");

        if(urlError != "")
        {
            unit.url = urlError;
        }

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

            InfoFileDownload infoFileDownload = createInfoDownload(unit, screenName, typeUrl, type);

            if (!_listInfoDownload.empty()) {
                if (type == TYPE_LINK_URL::LESSON) {
                    auto it = std::find_if(_listInfoDownloadOfLessonContinue.begin(),
                                           _listInfoDownloadOfLessonContinue.end(),
                                           [=](InfoFileDownload a) {
                                                return unit.url == a._unit.url;
                                            });
                    if (it == _listInfoDownloadOfLessonContinue.end()) {
                        //_listInfoDownloadOfLessonContinue.push_back(infoFileDownload);
                        _listInfoDownloadOfLessonContinue.insert(_listInfoDownloadOfLessonContinue.begin(), infoFileDownload);
                    }
                    return;
                } else if (type == TYPE_LINK_URL::THUMB_QUEUE) {

//                    ms::MsNewDownloadFileThumb::getInstance().initDownload(unit, screenName, typeUrl,  (ms::MsNewDownloadFileThumb::TYPE_LINK_URL)type);
//                    return;

//                    auto it = std::find_if(_listInfoDownloadThumbQueue.begin(),
//                                           _listInfoDownloadThumbQueue.end(),
//                                           [=](InfoFileDownload a) {
//                                               return unit.url == a._unit.url;
//                                           });
//                    if (it == _listInfoDownloadThumbQueue.end()) {
//                           _listInfoDownloadThumbQueue.push_back(infoFileDownload);
//                    }
                    return;
                } else if (type == TYPE_LINK_URL::THUMB_ZIP) {
                    auto it = std::find_if(_listInfoDownloadZipThumb.begin(),
                                           _listInfoDownloadZipThumb.end(),
                                           [=](InfoFileDownload a) {
                                               return unit.url == a._unit.url;
                                           });
                    if (it == _listInfoDownloadZipThumb.end()) {
                        _listInfoDownloadZipThumb.push_back(infoFileDownload);
                    }
                    return;
                }
            }
            _listInfoDownload.push_back(infoFileDownload);
        });
    }

    void NewDownload::downloadSucess(InfoFileDownload infoDdownload, bool isFileNull) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            _indexProcess = 0;
            if(!_listInfoDownload.empty())
            {
                _urlIsLoading = "";
                _index++;
                MJDEFAULT->deleteValueForKey(key_error_url_stories);

                auto info = _listInfoDownload[0];

                auto infoUnit = _listInfoDownload[0]._unit;

                if(isFileNull)
                {
                    info = infoDdownload;
                }

                EventCustom e(infoUnit.success_event);
                e.setUserData(&infoUnit);
                if(Director::getInstance()->getEventDispatcher()->hasEventListener(e.getEventName()))
                {
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                }

                if(_listInfoDownload[0]._typeDownLoadURL == TYPE_LINK_URL::THUMB_ZIP)
                {
                    if(!_listInfoDownloadZipThumb.empty() && _listInfoDownload[0]._unit.url == _listInfoDownloadZipThumb[0]._unit.url)
                    {
                        _listInfoDownloadZipThumb.erase(_listInfoDownloadZipThumb.begin());
                    }
                }

                deleteFileDownloadSucess(info);

               // _listInfoDownload.erase(_listInfoDownload.begin());

                continueDownloadWhenErrorURL();
            }
        });

    }

    void NewDownload::deleteFileDownloadSucess(InfoFileDownload info) {
        auto it = std::find_if(_listInfoDownload.begin(),
                               _listInfoDownload.end(),
                               [=](InfoFileDownload a) {
                                   return info._unit.storage_path == a._unit.storage_path;
                               });

        if (it == _listInfoDownload.end())
        {
            _listInfoDownload.erase(_listInfoDownload.begin());
        }else{
            _listInfoDownload.erase(it);
        }
    }

    void NewDownload::startDownload(InfoFileDownload  infoFileDownload) {

        if(checkFileDownloadSucess(infoFileDownload._unit.storage_path))
        {
            downloadSucess(infoFileDownload, true);
            return;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.DownloadFile","insertToListDownLoadFile",\
                                                 infoFileDownload._unit.url, infoFileDownload._unit.storage_path);
       // cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.BinaryFileDownloader","downloadFile",\
                                                 infoFileDownload->_unit.url, infoFileDownload->_unit.storage_path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        mj::helper::beginDownloadIos(infoFileDownload._unit.url, infoFileDownload._unit.storage_path);
#endif
    }

    void NewDownload::processDownload(double index) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            if(!_listInfoDownload.empty())
            {
                std::string urlProcess = _listInfoDownload[0]._unit.url;
                if (urlProcess.find(".png") != std::string::npos || urlProcess.find(".jpg") != std::string::npos) {
                    return ;
                }else{
                    clearListRemoteImageView();
                }
                
                _indexProcess = index;
                EventCustom e(_listInfoDownload[0]._unit.progress_event);
                double * progress = new double;
                * progress = index * 100.0;

                e.setUserData(progress);

                if(Director::getInstance()->getEventDispatcher()->hasEventListener(_listInfoDownload[0]._unit.progress_event))
                {
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                }
                delete progress;
            }
        });
    }


    void NewDownload::downloadError(std::vector<std::string> value, std::vector<std::string> listMessageError) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            if(!_listInfoDownload.empty())
            {
                _urlIsLoading = "";
                _index = 0;
                std::string urlError = _listInfoDownload[0]._unit.url;
                if(!value.empty())
                {
                    std::string typeError = value[value.size() - 1];
                    if(typeError == "lost_connection")
                    {
                        auto info = _listInfoDownload[0];
                        if (info._unit.typeUrl == StoryDataModel::TYPEURL::STORY)
                        {
                            MJDEFAULT->setStringForKey(key_error_url_stories, key_error_url_stories);
                        }
                        else if(info._unit.typeUrl == StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME) {
                            MJDEFAULT->setStringForKey(key_error_url_stories, key_error_url_game_audio_book);
                        }

                        std::string message = "None", ipAdress = "None", statusCode = "None";

                        if(value.size() >= 3)
                        {
                            message = value[2];
                            ipAdress = value[1];
                            statusCode = value[0];
                            if(listMessageError.size() > 0 && message == "None")
                            {
                                message = listMessageError[0];
                            }
                        }
                        mj::helper::pushEvnetLostConnection(info._unit.url, message, "download" + info._tytpeUrl + info._screenName, statusCode, "0", "", ipAdress, statusCode, "new", listMessageError);
                        
                        std::string nameEvent = info._unit.failed_event;
                        _listInfoDownload.erase(_listInfoDownload.begin());
                        EventCustom e(nameEvent);
                        e.setUserData(&info._unit);
                        if(Director::getInstance()->getEventDispatcher()->hasEventListener(nameEvent))
                        {
                            Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                        }

                    }else {
                        if(value.size() >= 3)
                        {
                            clearListRemoteImageView();
//                            std::vector<std::pair<std::string, cocos2d::Value>> valueEvent = {};
//                            valueEvent.push_back({"message_faile_1", cocos2d::Value(value[0])});
//                            valueEvent.pushdol_back({"message_faile_2", cocos2d::Value(value[1])});
//                            valueEvent.push_back({"type_error", cocos2d::Value(value[2])});
//                            CleverTapManager::GetInstance()->pushEvent("End_download_file", valueEvent);
                        }
                        if(!_listInfoDownload.empty())
                        {
                            continueDownloadWhenErrorURL();
                        }
                    }
                }else{
                    if(!_listInfoDownload.empty())
                    {
                        continueDownloadWhenErrorURL();
                    }
                }
            }
        });
    }

    void NewDownload::beginDownload() {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            if(!_listInfoDownload.empty())
            {
                if(_urlIsLoading != _listInfoDownload[0]._unit.url)
                {
                    _urlIsLoading = _listInfoDownload[0]._unit.url;
                    startDownload(_listInfoDownload[0]);
                }
            }else{
                continueDownloadWhenErrorURL();
            }
        });
    }

    void NewDownload::clearListDownLoadStory(bool isClearLesson)
    {
        ms::MsNewDownloadFileThumb::getInstance().clearListDownLoadStory(isClearLesson);
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            _urlIsLoading = "";
            clearListRemoteImageView();
            clearListInfoFileDownload(_listInfoDownloadZipThumb);
            if(isClearLesson)
            {
                clearListInfoFileDownload(_listInfoDownloadOfLessonContinue);
            }
            clearListInfoFileDownload(_listInfoDownloadThumbQueue);
            clearListInfoFileDownload(_listInfoDownload);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
           cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.DownloadFile","disConnectDownload");
           // cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.BinaryFileDownloader","disconnectionDownload");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            mj::helper::stopConnection();
#endif
        });
    }

    bool NewDownload::checkFileDownloadSucess(std::string pathFile)
    {
        if (FileUtils::getInstance()->isFileExist(pathFile))
        {
            return true;
        }
        return false;
    }

    void NewDownload::continueDownloadWhenErrorURL()
    {
        if(!_listInfoDownloadOfLessonContinue.empty())
        {
            _listInfoDownload.insert(_listInfoDownload.begin(), _listInfoDownloadOfLessonContinue[0]);
            _listInfoDownloadOfLessonContinue.erase(_listInfoDownloadOfLessonContinue.begin());
        } else if(!_listInfoDownloadZipThumb.empty())
        {
            _listInfoDownload.insert(_listInfoDownload.begin(), _listInfoDownloadZipThumb[0]);
        }else if(_listInfoDownload.empty() && !_listInfoDownloadThumbQueue.empty())
            {
                _listInfoDownload.insert(_listInfoDownload.begin(), _listInfoDownloadThumbQueue[0]);
                _listInfoDownloadThumbQueue.erase(_listInfoDownloadThumbQueue.begin());
            }

        if(!_listInfoDownload.empty())
        {
            _urlIsLoading = _listInfoDownload[0]._unit.url;
            startDownload(_listInfoDownload[0]);
        }else{
            _index = 0;
        }
    }

    bool NewDownload::isNewDownloadAndroid()
    {
        return true;
    }

    bool NewDownload::isNewDownload()
    {
        //return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
       return MJDEFAULT->getBoolForKey(key_turn_on_new_download, true);
#else
        return false;
#endif

    }

    void NewDownload::reDownload()
    {

    }

    void NewDownload::clearListInfoFileDownload(std::vector<InfoFileDownload> listValue)
    {
        if(listValue.empty())
        {
            return;
        }
        
        for(auto info : listValue)
        {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(info._unit.failed_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(info._unit.success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(info._unit.progress_event);

            if(!_listInfoDownload.empty())
            {
                if(_listInfoDownload[0]._unit.url == info._unit.url)
                {
                    if(!_listInfoDownload.empty())
                    {
                        _listInfoDownload.erase(_listInfoDownload.begin());
                    }
                }
            }

        }
        listValue.clear();
    }

    void NewDownload::deleteDownloadThumb(std::string url)
    {
        if(auto remtote = cocos2d::utils::findChild<RemoteImageView*>(Director::getInstance()->getRunningScene(),url))
        {
            remtote->unscheduleAllCallbacks();
            remtote->unschedule(url);
        }
    }

   void NewDownload::clearListRemoteImageView()
   {
        ms::MsNewDownloadFileThumb::getInstance().clearListRemoteImageView();
   }

}

