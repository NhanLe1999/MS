//
//  StoryAudioPlayer.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/24/18.
//

#ifndef StoryAudioPlayer_hpp
#define StoryAudioPlayer_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSEventLogger.h"

class MSStoryAudioPlayer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSStoryAudioPlayer);
    static MSStoryAudioPlayer * createView(int story_id);
    
    static int pending_story_id;
    
private:
	~MSStoryAudioPlayer();
    void didLoadFromCSB(int story_id);
    void onEnter() override;
    void onExit() override;
    
    bool isPlaying();
    
    void onDownload(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);
    void onPlay(cocos2d::Ref * sender);
    void onPause(cocos2d::Ref * sender);
    void onPrevPage(cocos2d::Ref * sender);
    void onNextPage(cocos2d::Ref * sender);
    
    void showNetworkError(std::string message);
    
    void parseData();
    
    void playFromPage(int page_id, int current_audio_in_page);
    void setIndicator(int page_id);
    
    void onDownloadAndUnzipDone();
    void onDownloadError(int error_code, std::string error_string);
    void onUnzipError(int error_code, std::string error_string);
    
    std::string _audio_title, _audio_written, _audio_design;
    
    std::vector<std::pair<std::string, int>> _list_audio;
    
    mj::model::StoryInfo _info;
    int _current_audio_id;
    int _current_page_id;
    int _current_audio_in_page;
    
    bool _is_metadata_played = false;
    
    std::vector<ui::ImageView *> _dots;
    cocos2d::ui::LoadingBar * _loading_bar;
    cocos2d::ui::Text * _progress_percent;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

};

CREATE_READER(MSStoryAudioPlayer);
#endif /* StoryAudioPlayer_hpp */
