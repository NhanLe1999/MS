//
//  ThumbQueue.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/13/18.
//

#ifndef ThumbQueue_hpp
#define ThumbQueue_hpp

#include <deque>
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "RemoteImageView.h"

class RemoteImageView;

namespace ms { namespace utils {
    class ThumbQueue {
    public:
        static ThumbQueue & getInstance() {
            static ThumbQueue thumbQueue;
            return thumbQueue;
        }
        
        void enqueue(std::string url, RemoteImageView* handler, RemoteImageView::Type type = RemoteImageView::Type::OTHER, bool isContinue = false);
        void unregister(RemoteImageView * handler);
        void removeURL(std::string url);
        void unDownloadImage();
        int getQueueSize();
        void loadImageNew(std::string url, RemoteImageView::Type type);
        void onDownloadImageFinishedNew(std::string url, RemoteImageView::Type type, std::vector<unsigned char> &buffer);
    protected:
        explicit ThumbQueue();
        
    private:
        void batchDownload();
        void loadImageFromURL(std::string url, RemoteImageView::Type type);
        void onRequestImageFinished(cocos2d::network::HttpClient * pClient, cocos2d::network::HttpResponse * pResponse);
        void onDownloadImageFinished(const cocos2d::network::DownloadTask & task, std::vector<unsigned char> & data);
        void onImageLoaded(std::string url);
        
        void removeUrl();

        std::deque<std::pair<std::string, int>> _queue;
        std::vector<std::string> _downloading;
        std::vector<std::pair<std::string, RemoteImageView *>> _map;
        
        std::vector<std::pair<std::string,  cocos2d::network::Downloader *>> _mapTask;
        ThumbQueue(ThumbQueue const &);
        void operator = (ThumbQueue const &);
        RemoteImageView::Type _type;
    };
}}

#endif /* ThumbQueue_hpp */
