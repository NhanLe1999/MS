

#include "MsNewDownloadFileThumb.h"

#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

#include "MSCacheManager.h"
#include "StoryInfoCover.h"
#include "CleverTapManager.h"

namespace ms {
    MsNewDownloadFileThumb::MsNewDownloadFileThumb() {

    }

    MsNewDownloadFileThumb::InfoFileDownload MsNewDownloadFileThumb::createInfoDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type)
    {
        InfoFileDownload infoDownload;
        infoDownload._unit = unit;
        infoDownload._screenName = screenName;
        infoDownload. _tytpeUrl = typeUrl;
        infoDownload._typeDownLoadURL = type;

        return infoDownload;
    }

    void MsNewDownloadFileThumb::initDownload(ms::utils::DownloadUnit unit, std::string screenName, std::string typeUrl, TYPE_LINK_URL type) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

            InfoFileDownload infoFileDownload = createInfoDownload(unit, screenName, typeUrl, type);

            if (!_listInfoDownload.empty()) {
                auto it = std::find_if(_listInfoDownloadThumbQueue.begin(),
                                       _listInfoDownloadThumbQueue.end(),
                                       [=](InfoFileDownload a) {
                                           return unit.url == a._unit.url;
                                       });
                if (it == _listInfoDownloadThumbQueue.end()) {
                    _listInfoDownloadThumbQueue.push_back(infoFileDownload);
                }
                return;
            }
            _listInfoDownload.push_back(infoFileDownload);
        });
    }

    void MsNewDownloadFileThumb::downloadSucess() {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            _indexProcess = 0;
            if(!_listInfoDownload.empty())
            {
                _urlIsLoading = "";
                _index++;
                MJDEFAULT->deleteValueForKey(key_error_url_stories);
                auto info = _listInfoDownload[0]._unit;
                EventCustom e(info.success_event);
                e.setUserData(&info);
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

                _listInfoDownload.erase(_listInfoDownload.begin());

                continueDownloadWhenErrorURL();
            }
        });

    }

    void MsNewDownloadFileThumb::startDownload(InfoFileDownload  infoFileDownload) {

        if(checkFileDownloadSucess(infoFileDownload._unit.storage_path))
        {
            downloadSucess();
            return;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.DownloadFileThumb","insertToListDownLoadFile",\
                                                 infoFileDownload._unit.url, infoFileDownload._unit.storage_path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        mj::helper::beginDownloadIos(infoFileDownload._unit.url, infoFileDownload._unit.storage_path, true);
#endif
    }

    void MsNewDownloadFileThumb::processDownload(double index) {

    }


    void MsNewDownloadFileThumb::downloadError(std::vector<std::string> value, std::vector<std::string> listMessageError) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            if(!_listInfoDownload.empty())
            {
                _urlIsLoading = "";
                _index = 0;
                clearListRemoteImageView();
                std::string urlError = _listInfoDownload[0]._unit.url;

                if(!value.empty())
                {
                    std::string typeError = value[value.size() - 1];
                    if(typeError == "lost_connection")
                    {
                        auto info = _listInfoDownload[0];

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
//                            valueEvent.push_back({"message_faile_2", cocos2d::Value(value[1])});
//                            valueEvent.push_back({"type_error", cocos2d::Value(value[2])});
//                            CleverTapManager::GetInstance()->pushEvent("End_download_file", valueEvent);
                            if(!_listInfoDownload.empty())
                            {
                                _listInfoDownload.erase(_listInfoDownload.begin());
                            }
                        }
                    }
                }

                deleteDownloadThumb(urlError);
                if(!_listInfoDownload.empty())
                {
                    _listInfoDownload.erase(_listInfoDownload.begin());
                }

                if(!_listInfoDownload.empty())
                {
                    continueDownloadWhenErrorURL();
                }
                clearListRemoteImageView();
            }
        });
    }

    void MsNewDownloadFileThumb::beginDownload() {
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

    void MsNewDownloadFileThumb::clearListDownLoadStory(bool isClearLesson)
    {
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
           cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.DownloadFileThumb","disConnectDownload");
           // cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.BinaryFileDownloader","disconnectionDownload");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            mj::helper::stopConnection(true);
#endif
        });
    }

    bool MsNewDownloadFileThumb::checkFileDownloadSucess(std::string pathFile)
    {
        if (FileUtils::getInstance()->isFileExist(pathFile))
        {
            return true;
        }
        return false;
    }

    void MsNewDownloadFileThumb::continueDownloadWhenErrorURL()
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

    bool MsNewDownloadFileThumb::isNewDownloadAndroid()
    {
        return true;
    }

    bool MsNewDownloadFileThumb::isNewDownload()
    {
        //return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
       return MJDEFAULT->getBoolForKey(key_turn_on_new_download, true);
#else
        return false;
#endif

    }

    void MsNewDownloadFileThumb::reDownload()
    {

    }

    void MsNewDownloadFileThumb::clearListInfoFileDownload(std::vector<InfoFileDownload> listValue)
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

    void MsNewDownloadFileThumb::deleteDownloadThumb(std::string url)
    {
        if(auto remtote = cocos2d::utils::findChild<RemoteImageView*>(Director::getInstance()->getRunningScene(),url))
        {
            remtote->unscheduleAllCallbacks();
            remtote->unschedule(url);
        }
    }

   void MsNewDownloadFileThumb::clearListRemoteImageView()
   {
        if(_listInfoDownloadThumbQueue.empty())
        {
            return;
        }
       for(int i = 0; i < _listInfoDownloadThumbQueue.size(); i++)
       {
           std::string url =  _listInfoDownloadThumbQueue[i]._unit.url;
           deleteDownloadThumb(url);
       }
       _listInfoDownloadThumbQueue.clear();
   }

}

