//
//  MSPlaylistPrepare.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/18.
//

#ifndef MSPlaylistPrepare_hpp
#define MSPlaylistPrepare_hpp

#include "cocostudiomacro2.h"
#include "MSGradeBoxDelegate.h"
#include "StoryAlert.h"
#include "MSGradeManager.h"

namespace mj { namespace model {
    struct AudiobookInfo;
}}

class MSPlaylist_Prepare : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::GradeBox,public StoryAlertDelegate  {
public:
    CREATE_FUNC(MSPlaylist_Prepare);
    static MSPlaylist_Prepare * createView(std::string name, std::string series_id, std::vector<mj::model::AudiobookInfo> audiobooks,int i_gradeID);
    
    void setContentSize(const cocos2d::Size & sz) override;
    void setLoadingLayerVisible(bool i_visible );
private:
    void didLoadFromCSB(std::string name, std::string series_id, std::vector<mj::model::AudiobookInfo> audiobooks,int i_gradeID);
    void onChangeDisplayLanguage();
    void onEnter() override;
    void onExit() override;
    
    void onBack(cocos2d::Ref * sender);
    void onChangeDuration(cocos2d::Ref * sender);
    void onSelectDuration(cocos2d::Ref * sender);
    void onPlay(cocos2d::Ref * sender);
    void onSelectItem(cocos2d::Ref * sender);
    
    void onReceivedEventGradeChanged(int grade_id);
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    
    void onDraggingItem(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void saveOrder();
    
    void reloadData();
    void addItem(int index);
    void update(float dt) override;
    void updateScrollviewItems(float dt);
    
    std::vector<mj::model::AudiobookInfo> _audiobooks, _filtered_audiobooks;
    std::string _name;
    
    std::string _series_id;
    
    cocos2d::ui::Widget * _moving_item = nullptr;
    cocos2d::ui::Widget * _model = nullptr;
    cocos2d::ui::ScrollView * _scrollview = nullptr;
    
    float _last_scroll_position_y = 0;
    float _scroll_velocity = 0;
    bool _stopAction = false;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
private:
    void onAlertOK(cocos2d::Ref * sender);
    void onAlertCancel(cocos2d::Ref * sender);
    bool _didChangeListOrder = false;   // Did list audiobook change  its order ?
    void updatePlaylist();   // This function is used to update audiobook playlist each time it changes its order.

private:
    ms::GradeManager::Scope m_scope;

};

CREATE_READER(MSPlaylist_Prepare);

#endif /* MSPlaylistPrepare_hpp */
