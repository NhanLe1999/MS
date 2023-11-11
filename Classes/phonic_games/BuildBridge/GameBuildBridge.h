//
//  GameBuildBridge.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/5/18.
//

#ifndef GameBuildBridge_hpp
#define GameBuildBridge_hpp

#include "cocostudiomacro2.h"
#include "MJMainGBase.h"
class GAFWrapper;

class GameBuildBridge : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameBuildBridge);
    static GameBuildBridge * createGame(std::string config_path);
    
private:
	std::string m_configPath;
	struct RoundPhonicInfo {
		std::string phonic;
		std::string audio_phonic;
	};
    struct RoundInfo {
        std::string word;
		std::string audio;
		std::string slow_audio;
        std::vector<RoundPhonicInfo> phonics;
        std::string image;
    };
    
	void generateData();
    void didLoadFromCSB(std::string config_path);
    void onEnter() override;
    
    void startRound();
    void showRound();
    void showText();
    void showBubbles();
	void initStoneLight();
    
    void onTapStone(cocos2d::Ref * sender);
    void onBubbleClicked(cocos2d::Ref * sender);

	void onMergeText();
	void onTransitionGame();
	void playSyncText();
	void playSlowText();
	void playNormalText();
	void onResetRound();
	void onTransitionRound();
    void setCameraMaskVisible(bool visible);
    
	void enableTouch();
	void disableTouch();

	void inactiveMode(float dt = 10.0f);
	void activeMode();
    void addSpeaker();
    void showHandSuggetionGame();


    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    std::vector<RoundInfo> _data;
    RoundInfo _current_round;
	std::vector<RoundPhonicInfo> _phonics_copy;
    
    GAFWrapper * _nat;
    GAFWrapper * _camera;
	GAFWrapper * stone_light;
    
    cocos2d::ui::Layout * _mark_left;
    cocos2d::ui::Layout * _mark_right;
    
    cocos2d::ui::Layout * _word;
    cocos2d::ui::Layout * touch_layout;
	cocos2d::ui::Text * text_lb;
    std::vector<bool> _char_filled;
    std::vector<cocos2d::ui::Layout *> _underlines;
	std::vector<GAFWrapper *> bubbles;
	int wrong_count;
	bool is_first_round;
    bool _isAudioIntro = false;
    std::string _audioIntroPath = "";
};

CREATE_READER(GameBuildBridge);

#endif /* GameBuildBridge_hpp */
