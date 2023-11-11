//
//  GameWordMachine.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/26/17.
//
//

#ifndef GameWordMachine_hpp
#define GameWordMachine_hpp

#include "GameBase.h"
#include "GameWordMachinePipe.h"

class GAFWrapper;

class MJGameWordMachine : public MJGameBase<MJGameWordMachine, mj::WordMachineDataSet>, public MJGameWordMachineDelegate, MJGameInstructionDelegate {
public:
    CREATE_FUNC(MJGameWordMachine);
    static MJGameWordMachine * createGame(mj::GameDataSet<mj::WordMachineDataSet> data);
    static mj::GameDataSet<mj::WordMachineDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::WordMachineDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::WordMachineDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::WordMachineDataSet> data);

    void adjustDifficulity();

    void onEnter() override;
    void onExit() override;
    void showGuide();
    void handSuggetion();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;

    void onNextQuestion(cocos2d::Ref * sender);
    double reloadAnimation();
    void loadAnimation();

    
    mj::WordMachineDataSet _current_set;

    void onTouchOption(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
    cocos2d::Point _saved_option_position;
    void onOptionMoved(cocos2d::ui::ImageView * option, cocos2d::Point position);
    void runAnimationEndQuestion(cocos2d::ui::ImageView * option);

    void loadQuestion(mj::WordMachineDataSet question, bool isLoadSentence = false);
    void onReadSentence() override;
    void addSpeaker();
    void readGafBlinklink();

    std::vector<Vec2> position_list;
    GAFWrapper * _filter_gaf = nullptr;
    GAFWrapper * _machine_gaf = nullptr;
	int cur_level = 0;
    MJGameWordMachinePipe * _pipe_structure = nullptr;
    ui::ImageView* _img = nullptr;

    std::vector<std::string> _splited_sentence;
    bool _is_guide_showed = true;
    int _guide_count = 0;
    int bg_music_id = -1;
    int q_id = -1;
    std::string _pathAudioSpeak = "";
    std::string _pathAudioImg = "";
    cocos2d::ui::Button* _speak = nullptr;
    bool _isPlayAudioIntro = true;
};

CREATE_READER(MJGameWordMachine);

#endif /* GameWordMachine_hpp */
