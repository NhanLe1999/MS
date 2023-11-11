//
//  ItemImageBase.cpp
//  MonkeyJunior
//
//  Created by developer on 3/9/16.
//
//
#include "ItemImageBase.h"
//#include <regex.h>
#include "spine/Json.h"
#include "network/HttpClient.h"
#include "NativeBridge.h"
#include "HSLib.h"
#include"MJDefinitions.h"
#define ktag_indicator 200

void ItemImageBase::loadImageFromURL(string url, string img_id){
    if (url == "") return;
    image_id = getFileNameFromUrl(url);
    std::string writeable = FileUtils::getInstance()->getWritablePath();
    writeable = writeable + IMAGE_CACHE_PATH + image_id;
    if (FileUtils::getInstance()->isFileExist(writeable)){
        onFinishLoadImage();
        return;
    }
    request_loadIMG = new cocos2d::network::HttpRequest();
    request_loadIMG->setUrl(url);
    request_loadIMG->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request_loadIMG->setTag(image_id.c_str());
    request_loadIMG->setResponseCallback(CC_CALLBACK_2(ItemImageBase::onImageDownloaded, this));
    cocos2d::network::HttpClient::getInstance()->send(request_loadIMG);
}

void ItemImageBase::onImageDownloaded(cocos2d::network::HttpClient * pClient, cocos2d::network::HttpResponse * pResponse){
    if (!pResponse) {
        CCLOG("NO RESPONSE");
        return;
    }
    std::string k(pResponse->getHttpRequest()->getTag());
    if (k != image_id) {
        return;
    }
    if (!pResponse->isSucceed()) {
        return;
    }
    if ((int)pResponse->getResponseCode() != 200) {
        return;
    }

    std::vector <char> buffer = *(pResponse->getResponseData());
    std::string writeable = FileUtils::getInstance()->getWritablePath();
    if (this)
        writeable = writeable + image_cache_path + image_id;

    auto file = fopen(std::string(writeable).c_str(), "wb");
    if (!file) return;
    fwrite(&(buffer.front()), 1, buffer.size(), file);
    fclose(file);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NativeBridge::addSkipBackupAttributeToItemAtPath(writeable);
#endif
    if(request_loadIMG != nullptr){
        onFinishLoadImage();
    }

//    auto image = new Image;
//    image->initWithImageData((const unsigned char *)&buffer.front(), buffer.size());
//        image->saveToFile(writeable);
//    image->release();

}
void ItemImageBase::onFinishLoadImage(){
}

string ItemImageBase::getFileNameFromUrl(string url){
    auto pos = url.find_last_of('/');
    return url.substr(pos);
}

#pragma mark - LOAD_IMAGE_NOT_SHOW
void ItemImageBase::loadImageFromURLNotShow(string url){
    if (url == "") return;
    image_notshow = getFileNameFromUrl(url);
    std::string writeable = FileUtils::getInstance()->getWritablePath();
    writeable = writeable + image_cache_path + image_notshow;
    if (FileUtils::getInstance()->isFileExist(writeable)){
        onFinishLoadImage();
        return;
    }
    request = new cocos2d::network::HttpRequest();
    request->setUrl(url);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setTag(image_notshow.c_str());
    request->setResponseCallback(CC_CALLBACK_2(ItemImageBase::onloadImageFromURLNotShowDone, this));
    cocos2d::network::HttpClient::getInstance()->send(request);
}
void ItemImageBase::onloadImageFromURLNotShowDone(cocos2d::network::HttpClient * pClient, cocos2d::network::HttpResponse * pResponse){
    if (!pResponse) {
        CCLOG("NO RESPONSE");
        return;
    }
    std::string k(pResponse->getHttpRequest()->getTag());
    if (k != image_notshow) {
        return;
    }
    if (!pResponse->isSucceed()) {
        return;
    }
    
    if ((int)pResponse->getResponseCode() != 200) {
        return;
    }
    
    std::vector <char> buffer = *(pResponse->getResponseData());
    std::string writeable = FileUtils::getInstance()->getWritablePath();
    if (this)
        writeable = writeable + image_cache_path + image_notshow;
    
    auto file = fopen(std::string(writeable).c_str(), "wb");
    if (!file) return;
    fwrite(&(buffer.front()), 1, buffer.size(), file);
    fclose(file);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NativeBridge::addSkipBackupAttributeToItemAtPath(writeable);
#endif

}

void ItemImageBase::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();

}
void ItemImageBase::onExit(){
    Layout::onExit();
    if (request){
        request->setResponseCallback(nullptr);
        request->release();
        request = nullptr;
    }
    if (request_loadIMG){
        
        request_loadIMG->setResponseCallback(nullptr);
        request_loadIMG->release();
        request_loadIMG = nullptr;
        CCLOG("request_loadIMG");
    }
}

