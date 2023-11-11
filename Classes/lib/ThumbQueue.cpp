//
//  ThumbQueue.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/13/18.
//

#include <fstream>
#include "ThumbQueue.h"
#include "RemoteImageView.h"
#include "MJHelper.h"
#include "NativeBridge.h"
#include "network/CCDownloader.h"
#include "NativeBridge.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MsNewDownloadFileThumb.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>


#define LIMIT_CONCURRENT_DOWNLOAD 10
#define CC_DOWNLOADER_DEBUG 1
#define NUMBER_LIMIT_DOWNLOAD 3

USING_NS_CC;

namespace ms { namespace utils {
    ThumbQueue::ThumbQueue() {
    }
    
    void ThumbQueue::enqueue(std::string url, RemoteImageView *handler, RemoteImageView::Type type, bool isContinue) {
         _type = type;
        if(!NativeBridge::networkAvaiable())
           {
            return;
        }
         if(ms::MsNewDownloadFileThumb::getInstance().isNewDownload())
         {
             if(!url.empty() && url[url.length() - 1] != '/')
             {
                 loadImageNew(url, type);
             }
             return;
         }

        int limitDownload = MJDEFAULT->getIntegerForKey(key_Limit_download_thumb);
        if (!isContinue && limitDownload > NUMBER_LIMIT_DOWNLOAD)
        {
            return;
        }
        if (!NativeBridge::networkAvaiable()) {
            return;
        }

		if (url.empty())
			return;

		if (url[url.length() - 1] == '/') {
			return;
		}

        _map.push_back(std::make_pair(url, handler));
        
        if (std::find_if(std::begin(_queue), std::end(_queue), [url, type](std::pair<std::string, int> e) {
            return e.first == url && e.second == type;
        }) != std::end(_queue)) {
            return;
        }
        if (std::find(std::begin(_downloading), std::end(_downloading), url) != std::end(_downloading)) {
            return;
        }
        
        _queue.push_back(std::make_pair(url, type));
        if (_downloading.size() < LIMIT_CONCURRENT_DOWNLOAD) {
            batchDownload();
        }
    }
    
    void ThumbQueue::batchDownload() {
        while (_downloading.size() < 10 && !_queue.empty()) {
            auto url = _queue.front();
            _downloading.push_back(url.first);
            _queue.pop_front();
            loadImageFromURL(url.first, (RemoteImageView::Type) url.second);
        }
    }

    void ThumbQueue::loadImageNew(std::string url, RemoteImageView::Type type)
    {
        std::string fileName = mj::helper::getFilenameFromURL(url);
        std::string forderFile = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH;
        std::string filePath = forderFile +fileName;
        ms::utils::DownloadUnit unit;

        unit.storage_path = filePath;
        unit.download_id = fileName;
        unit.forderPath = forderFile;
        unit.url = url;
        unit.progress_event = std::string("Progress") + IMAGE_CACHE_PATH + fileName;
        unit.success_event = std::string("Sucess") + IMAGE_CACHE_PATH + fileName;
        unit.failed_event = std::string("error") + IMAGE_CACHE_PATH + fileName;

        Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.failed_event, [=](EventCustom * e) {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
        });

        Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.success_event, [=](EventCustom * e) {

        });

        ms::MsNewDownloadFileThumb::getInstance().initDownload(unit, "load_thumb", "ThumbQueue", ms::MsNewDownloadFileThumb::TYPE_LINK_URL::THUMB_QUEUE);
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            ms::MsNewDownloadFileThumb::getInstance().beginDownload();
        });

    }

    void ThumbQueue::onDownloadImageFinishedNew(std::string url, RemoteImageView::Type type, std::vector<unsigned char> &buffer)
    {
        removeURL(url);
        auto cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + mj::helper::getFilenameFromURL(url);

        if (type == RemoteImageView::Type::OTHER) {
            std::string writeable = FileUtils::getInstance()->getWritablePath();

            auto file = fopen(std::string(cached_image).c_str(), "wb");
            if (!file) return;
            fwrite(&(buffer.front()), 1, buffer.size(), file);
            fclose(file);
        } else {
            Size actual_size;
            Size mask_size;

            if (type == RemoteImageView::AUDIOBOOKS) {
                actual_size = Size(240, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(228, 230) / Director::getInstance()->getContentScaleFactor();
            } else {
                actual_size = Size(180, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(168, 230) / Director::getInstance()->getContentScaleFactor();
            }

            auto img = new (std::nothrow) Image();
            bool r =false;
            // neu buffer  == nullptr thi khong co size   -> check != null  roi check size roi moi check gia tri.
            if (&buffer != nullptr &&  buffer.size()!= 0 && &buffer.front() != nullptr) {
                r = img->initWithImageData((const unsigned char*)&(buffer.front()), buffer.size());
            }
            if (r) {
                auto tex = new (std::nothrow) Texture2D();
                tex->initWithImage(img);

                auto sprite = Sprite::createWithTexture(tex);
                auto rendersize = sprite->getContentSize();
                sprite->setScaleX(actual_size.width / sprite->getContentSize().width);
                sprite->setScaleY(actual_size.height / sprite->getContentSize().height);
                if (type == RemoteImageView::STORIES) {
                    sprite->setPosition(Point(1, -2.5));
                }

                auto mask_sprite = ui::Scale9Sprite::create("mjstory/new_ui/image_item_mask_default.png");
                mask_sprite->setCapInsets(Rect(24, 24, 2, 2));
                mask_sprite->setContentSize(mask_size);
                auto mask = ClippingNode::create(mask_sprite);
                mask->setName("mask");
                mask->setAlphaThreshold(0);
                mask->setPosition(mask_size / 2);
                mask->addChild(sprite);

                auto layout = ui::Layout::create();
                layout->setContentSize(mask_size);
                layout->addChild(mask);

                auto dt = cocos2d::utils::captureNode(layout);
                auto saved = cached_image;
                AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [=](void *) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                    NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
#endif
                    onImageLoaded(url);
                }, nullptr, [dt, saved] {
                    try {
                        dt->saveToFile(saved, false);
                        dt->release();
                    }
                    catch (...) {
                    }
                });
                tex->release();
                img->release();
            } else {
                CCLOG("failed url: %s", url.c_str());
            }
            return;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
#endif
        onImageLoaded(url);

    }
    
    void ThumbQueue::loadImageFromURL(std::string url, RemoteImageView::Type type) {
        auto downloader = new (std::nothrow) network::Downloader();
        
        downloader->setOnTaskError([this](const network::DownloadTask & task, int error_code, int error_internal_code, const std::string & error_string) {
            mj::helper::pushEventCrashNetWork(task.requestURL, error_string, "ThumbQueue", error_code);

            int limitDowload = MJDEFAULT->getIntegerForKey(key_Limit_download_thumb);

            MJDEFAULT->setIntegerForKey(key_Limit_download_thumb, limitDowload + 1);

            ThumbQueue::getInstance().removeURL(task.requestURL);
			ThumbQueue::getInstance().batchDownload();
        });
        downloader->onTaskProgress = [this](const network::DownloadTask& task,
                                             int64_t /*bytesReceived*/,
                                             int64_t totalBytesReceived,
                                             int64_t totalBytesExpected) {
        };
        downloader->setOnFileTaskSuccess([this](const network::DownloadTask& task) {
			ThumbQueue::getInstance().onImageLoaded(task.requestURL);
        });
        downloader->onDataTaskSuccess = CC_CALLBACK_2(ThumbQueue::onDownloadImageFinished, this);
        auto task = downloader->createDownloadDataTask(url, StringUtils::toString((int)type) + url);
    }

    void ThumbQueue::unDownloadImage()
    {
       
    }
    
    void ThumbQueue::onDownloadImageFinished(const cocos2d::network::DownloadTask &task, std::vector<unsigned char> &buffer) {
        auto url = task.requestURL;
        removeURL(url);
        batchDownload();
        std::string cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + mj::helper::getFilenameFromURL(url);
        if (_type == RemoteImageView::Type::AI_SPEAKING)
            cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/" + mj::helper::getFilenameFromURL(url);
        auto type = (RemoteImageView::Type)atoi(task.identifier.substr(0, 1).c_str());

        if (type == RemoteImageView::Type::OTHER) {
            std::string writeable = FileUtils::getInstance()->getWritablePath();

            auto file = fopen(std::string(cached_image).c_str(), "wb");
            if (!file) return;
            fwrite(&(buffer.front()), 1, buffer.size(), file);
            fclose(file);
        } else {
            Size actual_size;
            Size mask_size;

            if (type == RemoteImageView::AUDIOBOOKS) {
                actual_size = Size(240, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(228, 230) / Director::getInstance()->getContentScaleFactor();
            } else {
                actual_size = Size(180, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(168, 230) / Director::getInstance()->getContentScaleFactor();
            }

            auto img = new (std::nothrow) Image();
            bool r =false;
            // neu buffer  == nullptr thi khong co size   -> check != null  roi check size roi moi check gia tri.
            if (&buffer != nullptr &&  buffer.size()!= 0 && &buffer.front() != nullptr) {
                r = img->initWithImageData((const unsigned char*)&(buffer.front()), buffer.size());
            }
            if (r) {
                auto tex = new (std::nothrow) Texture2D();
                tex->initWithImage(img);

                auto sprite = Sprite::createWithTexture(tex);
                auto rendersize = sprite->getContentSize();
                sprite->setScaleX(actual_size.width / sprite->getContentSize().width);
                sprite->setScaleY(actual_size.height / sprite->getContentSize().height);
                if (type == RemoteImageView::STORIES) {
                    sprite->setPosition(Point(1, -2.5));
                }

                auto mask_sprite = ui::Scale9Sprite::create("mjstory/new_ui/image_item_mask_default.png");
                mask_sprite->setCapInsets(Rect(24, 24, 2, 2));
                mask_sprite->setContentSize(mask_size);
                auto mask = ClippingNode::create(mask_sprite);
                mask->setName("mask");
                mask->setAlphaThreshold(0);
                mask->setPosition(mask_size / 2);
                mask->addChild(sprite);

                auto layout = ui::Layout::create();
                layout->setContentSize(mask_size);
                layout->addChild(mask);

                auto dt = cocos2d::utils::captureNode(layout);
                auto saved = cached_image;
                AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [=](void *) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                    NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
#endif
                    onImageLoaded(url);
                }, nullptr, [dt, saved] {
                    try {
                        dt->saveToFile(saved, false);
                        dt->release();
                    }
                    catch (...) {
                    }
                });
                tex->release();
                img->release();
            } else {
                CCLOG("failed url: %s", url.c_str());
            }
            return;
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
#endif
        onImageLoaded(url);
    }
    
    void ThumbQueue::onRequestImageFinished(cocos2d::network::HttpClient *pClient, cocos2d::network::HttpResponse *pResponse) {
        if (!pResponse) {
            CCLOG("NO RESPONSE");
            return;
        }
        
        auto url = std::string(pResponse->getHttpRequest()->getUrl());
        
        removeURL(url);
        batchDownload();
        
        if (!pResponse->isSucceed()) {
            _map.erase(std::remove_if(_map.begin(), _map.end(), [url](std::pair<std::string, RemoteImageView *> e) {
                return e.first == url;
            }), _map.end());
            return;
        }
        if ((int)pResponse->getResponseCode() != 200) {
            _map.erase(std::remove_if(_map.begin(), _map.end(), [url](std::pair<std::string, RemoteImageView *> e) {
                return e.first == url;
            }), _map.end());
            return;
        }

        auto cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + mj::helper::getFilenameFromURL(url);
        auto type = (RemoteImageView::Type) ATOI(pResponse->getHttpRequest()->getTag());
        
        std::vector <char> * buffer = (pResponse->getResponseData());
        
        if (type == RemoteImageView::Type::OTHER) {
            std::string writeable = FileUtils::getInstance()->getWritablePath();
            
            auto file = fopen(std::string(cached_image).c_str(), "wb");
            if (!file) return;
            fwrite(&(buffer->front()), 1, buffer->size(), file);
            fclose(file);
        } else {
            Size actual_size;
            Size mask_size;
            
            if (type == RemoteImageView::AUDIOBOOKS) {
                actual_size = Size(240, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(228, 230) / Director::getInstance()->getContentScaleFactor();
            } else {
                actual_size = Size(180, 240) / Director::getInstance()->getContentScaleFactor();
                mask_size = Size(168, 230) / Director::getInstance()->getContentScaleFactor();
            }
            
            auto img = new (std::nothrow) Image();
            img->initWithImageData((const unsigned char *)&(buffer->front()), buffer->size());
            
            auto tex = new (std::nothrow) Texture2D();
            tex->initWithImage(img);
            
            auto sprite = Sprite::createWithTexture(tex);
            auto rendersize = sprite->getContentSize();
            sprite->setScaleX(actual_size.width / sprite->getContentSize().width);
            sprite->setScaleY(actual_size.height / sprite->getContentSize().height);
            if (type == RemoteImageView::STORIES) {
                sprite->setPosition(Point(1, -2.5));
            }
            
            auto mask_sprite = ui::Scale9Sprite::create("mjstory/new_ui/image_item_mask_default.png");
            mask_sprite->setCapInsets(Rect(24, 24, 2, 2));
            mask_sprite->setContentSize(mask_size);
            auto mask = ClippingNode::create(mask_sprite);
            mask->setName("mask");
            mask->setAlphaThreshold(0);
            mask->setPosition(mask_size / 2);
            mask->addChild(sprite);
            
            auto layout = ui::Layout::create();
            layout->setContentSize(mask_size);
            layout->addChild(mask);
            
            auto dt = cocos2d::utils::captureNode(layout);
            auto saved = cached_image;
            
            AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [=](void *) {
                #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
                #endif
                onImageLoaded(url);
            }, nullptr, [dt, saved] {
				try {
					dt->saveToFile(saved, false);
					dt->release();
				}
				catch (...) {
				}
            });
            
            tex->release();
            img->release();
            return;
        }
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        NativeBridge::addSkipBackupAttributeToItemAtPath(cached_image);
#endif
        onImageLoaded(url);
    }
    
    void ThumbQueue::onImageLoaded(std::string url) {
        for (auto p : _map) {
            if (p.first == url) {
                if (p.second) {
                    p.second->onImageLoaded();
                }
            }
        }
        _map.erase(std::remove_if(_map.begin(), _map.end(), [url](std::pair<std::string, RemoteImageView *> e) {
            return e.first == url;
        }), _map.end());
    }
    
    void ThumbQueue::unregister(RemoteImageView *handler) {
        _map.erase(std::remove_if(_map.begin(), _map.end(), [handler](std::pair<std::string, RemoteImageView *> e) {
            return e.second == handler;
        }), _map.end());
    }
    
    void ThumbQueue::removeURL(std::string url) {
        for (auto it = _mapTask.begin(); it != _mapTask.end();) {
            if ((*it).first == url) {
                delete (*it).second;
                it = _mapTask.erase(it);
            } else {
                ++it;
            }
        }
        
        _downloading.erase(std::remove_if(_downloading.begin(), _downloading.end(), [url](std::string d) {
            return d == url;
        }), _downloading.end());
//        _queue.erase(std::remove_if(_queue.begin(), _queue.end(), [url](std::pair<std::string, int> e) {
//            return e.first == url;
//        }), _queue.end());
        for (auto it = _queue.begin(); it != _queue.end(); ) {
            if ((*it).first == url) {
                it = _queue.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    int ThumbQueue::getQueueSize() {
        return (int)_queue.size() + (int)_downloading.size();
    }
}}
