//
//  RemoteImageView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef RemoteImageView_hpp
#define RemoteImageView_hpp

#include "cocostudiomacro2.h"
#include "network/HttpRequest.h"
#include "network/CCDownloader.h"

class RemoteImageView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    enum Type {
        STORIES = 0,
        AUDIOBOOKS = 1,
        AI_SPEAKING = 2,
        AI_SPEAKING_VIDEO_ZIP = 3,
        OTHER = 4
    };
    
    CREATE_FUNC(RemoteImageView);
    static RemoteImageView * createImage(std::string url, std::function<void(RemoteImageView * img, bool is_success)> callback = nullptr, Type type = Type::OTHER, bool isGame = false, bool isLesson = false);
    
    void setContentSize(const cocos2d::Size & size) override;
    void onImageLoaded();
    cocos2d::ui::ImageView * _image = nullptr;

private:
    void didLoadFromCSB(std::string url, std::function<void(RemoteImageView * img, bool is_success)> callback, Type type, bool isGam, bool isLesso);
    
    void loadImageFromURL(std::string url);
    bool checkFileExitInImageCache();
    
    void onExit() override;
    
    
    std::function<void(RemoteImageView * img, bool is_success)> _callback = nullptr;
    
    std::string _cached_image = "", _cache_image_ai_speaking = "";
    std::string imageName = "";
    std::string _url;
    bool _isGame = false;
    bool _isLesson = false;
    int _countContinue = 0;;
    bool _isReturnCheckFile = false;

    Type _type;
};

CREATE_READER(RemoteImageView);
#endif /* RemoteImageView_hpp */
