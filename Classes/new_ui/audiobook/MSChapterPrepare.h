//
//  MSChapterPrepare.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/15/19.
//

#ifndef MSChapterPrepare_hpp
#define MSChapterPrepare_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "StoryAlert.h"

class MSChapter_Prepare : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol,public StoryAlertDelegate {
public:
    CREATE_FUNC(MSChapter_Prepare);
    static MSChapter_Prepare * createView(mj::model::AudiobookInfo info, std::vector<mj::model::AudiobookInfo> chapters);
    
    void setContentSize(const cocos2d::Size & sz) override;
    void setLoadingLayerVisible(bool i_visible );
private:
    void didLoadFromCSB(mj::model::AudiobookInfo info, std::vector<mj::model::AudiobookInfo> chapter);
    void onChangeDisplayLanguage();
    void onEnter() override;
    void onExit() override;

    void onBack(cocos2d::Ref * sender);
    void onChangeDuration(cocos2d::Ref * sender);
    void onSelectDuration(cocos2d::Ref * sender);
    void onPlay(cocos2d::Ref * sender);
    void onSelectItem(cocos2d::Ref * sender);
    void onSummary(cocos2d::Ref * sender);
    void onSummaryClose(cocos2d::Ref * sender);
    
    void reloadData();
    void addItem(int index, int state);
    void updateScrollviewItems(float dt);
    
    void updateLastTimePlayedChapter();
    
    std::vector<mj::model::AudiobookInfo> _audiobooks;
    mj::model::AudiobookInfo _info;
    
    cocos2d::ui::Widget * _moving_item = nullptr;
    cocos2d::ui::Widget * _model = nullptr;
//    cocos2d::ui::ScrollView * _scrollview = nullptr;
    cocos2d::ui::ListView * _listview = nullptr;
    
    float _last_scroll_position_y = 0;
    float _scroll_velocity = 0;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

private:
    void onAlertOK(cocos2d::Ref * sender);
    void onAlertCancel(cocos2d::Ref * sender);
};

CREATE_READER(MSChapter_Prepare);

#endif /* MSChapterPrepare_hpp */
