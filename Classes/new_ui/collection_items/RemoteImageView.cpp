//
//  RemoteImageView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "RemoteImageView.h"
#include "MJHelper.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "NativeBridge.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "ThumbQueue.h"

USING_NS_CC;
INITIALIZE_READER(RemoteImageView);

std::once_flag remote_image_view_reader;
cocos2d::Data remote_image_view_node_data;
RemoteImageView * RemoteImageView:: createImage(std::string url, std::function<void(RemoteImageView * img, bool is_success)> callback, Type type, bool isGame, bool isLesson) {
    std::call_once(remote_image_view_reader, [] {
        REGISTER_CSB_READER(RemoteImageView);
        remote_image_view_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/RemoteImageView.csb");
    });
    auto image = reinterpret_cast<RemoteImageView *>(CSLoader::createNode(remote_image_view_node_data));
    if (image) {
        image->didLoadFromCSB(url, callback, type, isGame, isLesson);
        return image;
    }
    
    CC_SAFE_DELETE(image);
    return nullptr;
}

void RemoteImageView::didLoadFromCSB(std::string url, std::function<void(RemoteImageView * img, bool is_success)> callback, Type type, bool isGame, bool isLesson) {
    _url = url;
    _type = type;
    this->setName(url);
    this->setTag(random(0, INT_MAX));
    if (url.empty()) {
        callback(nullptr, false);
        return;
    }
    _isGame = isGame;
    _isLesson = isLesson;
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setClippingEnabled(true);
    imageName = mj::helper::getFilenameFromURL(url);
    if(type!= Type::AI_SPEAKING)
        _cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + imageName;
    else
    {
        _cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH_FOR_AISPEAKING + "image/" + imageName;
    }

    _image =  this->getChildByName<ui::ImageView *>("r_image");
    _callback = callback;
    
    this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([this, url] {
        loadImageFromURL(url);
    }), nullptr));
}

void RemoteImageView::loadImageFromURL(std::string url) {
    if (url.empty()) {
        return;
    }
    
    if (checkFileExitInImageCache()) {
        this->unschedule(url);
        Director::getInstance()->getTextureCache()->addImageAsync(_cached_image, [this](Texture2D * texture) {
            if (!_image) {
                _callback ? _callback(this, false) : (void) nullptr;
                return;
            }
            _image->loadTexture(_cached_image);

            this->setContentSize(this->getContentSize());
            _callback ? _callback(this, true) : (void) nullptr;
        }, _cached_image + StringUtils::toString(this->getTag()));
        return;
    }
    else {
        ms::utils::ThumbQueue::getInstance().enqueue(url, this, _type, true);
        this->schedule([=] (float) {
            loadImageFromURL(url);
        }, 1, 30, 1.5f, url);
    }

    return;
}

bool RemoteImageView::checkFileExitInImageCache()
{
    if (FileUtils::getInstance()->isFileExist(_cached_image))
    {
        return true;
    }else if(FileUtils::getInstance()->isFileExist("imgcache/" +imageName)){
        _cached_image = "imgcache/" +imageName;
        return true;
    }
    else
    {
        if (_isGame)
        {
            _cached_image = ("thumbGame/" + imageName);
            if (FileUtils::getInstance()->isFileExist(_cached_image))
                return true;
            _cached_image = "thumbGame/" + std::string("6zvcih2yaob2Zm15VWNCOYOB5l5Y8BdE.png");
            return true;
        }

        std::string subFolderStr = MJDEFAULT->getStringForKey(key_image_cache_subfolders_converted, "");
        if (subFolderStr.empty())
            return false;
        std::vector <std::string> subFolders;
        string token1; // define a string variable  
        size_t pos = 0;
        string delim = ", "; // delimiter  
        while ((pos = subFolderStr.find(delim)) != std::string::npos)
        {
            subFolders.push_back(subFolderStr.substr(0, pos)); // store the substring   
            subFolderStr.erase(0, pos + delim.length());  /* erase() function store the current positon and move to next token. */
        }
        if (subFolders.size() <= 0)
            return false;
        for (std::string subfolder : subFolders)
        {
            std::string audioBookPath = subfolder;
            if (FileUtils::getInstance()->isDirectoryExist(audioBookPath)) {
                string imagePath = audioBookPath + imageName;
                if (FileUtils::getInstance()->isFileExist(imagePath))
                {
                    _cached_image = imagePath;
                    return true;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
        // if haven't subfolder contain imagename --> fileName not exits in folder, return falsem
        return false;

    }
}

void RemoteImageView::onImageLoaded() {
    auto block_image = "event.image.loaded." + StringUtils::toString(this->getTag());
    Director::getInstance()->getTextureCache()->addImageAsync(_cached_image, [block_image, this](Texture2D * texture) {
        if (!_image) {
            _callback ? _callback(this, false) : (void) nullptr;
            return;
        }
        _image->loadTexture(_cached_image);
        this->setContentSize(this->getContentSize());
        _callback ? _callback(this, true) : (void) nullptr;
    }, _cached_image + StringUtils::toString(this->getTag()));
}

void RemoteImageView::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    if (_image) {
        _image->setContentSize(size);
        _image->setPosition(size / 2);
    }
}

void RemoteImageView::onExit() {
    ms::utils::ThumbQueue::getInstance().unregister(this);
    ms::utils::ThumbQueue::getInstance().removeURL(_url);
    Director::getInstance()->getTextureCache()->unbindImageAsync(_cached_image + StringUtils::toString(this->getTag()));
    
    auto tex = Director::getInstance()->getTextureCache()->getTextureForKey(_cached_image);
    if (tex) {
        if (tex->getReferenceCount() == 2) {
            Director::getInstance()->getTextureCache()->removeTexture(tex);
        }
    }

    FileUtils::getInstance()->removeFile(_cached_image + ".tmp");
    ui::Layout::onExit();
}
