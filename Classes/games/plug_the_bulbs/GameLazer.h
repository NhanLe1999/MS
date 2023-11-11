//
//  GameLazer.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//

#ifndef GameLazer_hpp
#define GameLazer_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameLazerFrameTop.h"
#include "GameLazerFrameBottom.h"

class MJGameLazer : public MJGameBase<MJGameLazer, mj::LazerGameDataSet>, public MJGameLazerPlugDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameLazer);
    static MJGameLazer * createGame(mj::GameDataSet<mj::LazerGameDataSet> data);
    static mj::GameDataSet<mj::LazerGameDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::LazerGameDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::LazerGameDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    void showGuide(float delay = 0.0);
    void stopSoundAudioSuggetion();
    void setValueCheckSuggetion(bool value);
    
private:
    void didLoadFromCSB(mj::GameDataSet<mj::LazerGameDataSet> data);
    void onEnter() override;
    void onExit() override;
    ~MJGameLazer();

    void doAppear();
    bool loadQuestion();
    void doDisappear();
    void handSuggetionGame();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onRefresh(cocos2d::Ref * sender);

    void onPlugMoving(MJGameLazerFrameTop * frame, cocos2d::Point position) override;
    void onPlugMoved(MJGameLazerFrameTop * frame, cocos2d::Point position) override;

    std::vector<MJGameLazerFrameBottom *> _bottom_frames;
    std::vector<MJGameLazerFrameTop *> _top_frames;
    std::vector<MJGameLazerFrameTop*> _topFramesCopy = {};

    
    mj::LazerGameDataSet _current_set;

    int _total_connected;
    
    
    void addSpeaker();
    int randomTopFrame();
    void runActionHand();
    void setCallbackCickSpeak();
    void setCallbackPlayAudio();
    bool _is_guide_showed = false;
    bool _isPlayAudioSpeak = true;
    bool _isPlayAudioSugget = true;
    bool _isSuggetionGame = true;
    Point postStartHand = Point(0, 0);
    Point postEndHand = Point(0, 0);
    std::string _pathAudioIntro = "";
    std::vector<int> _valueSizeObject = {};
    std::function<void()> _callbackAddSpeakAndShowGuide = nullptr;
    std::function<void()> _callbackCickSpeak = nullptr;
    std::function<void()> _callbackPlayAudio = nullptr;

};

CREATE_READER(MJGameLazer);

#endif /* GameLazer_hpp */
