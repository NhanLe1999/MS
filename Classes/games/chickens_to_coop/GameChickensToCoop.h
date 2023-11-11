//
//  GameChickensToCoop.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#ifndef GameChickensToCoop_hpp
#define GameChickensToCoop_hpp

#include "GameBase.h"
#include "Chicken.h"

class MJChicken;
class MJCoop;

class MJGameChickensToCoop : public MJGameBase<MJGameChickensToCoop, mj::ChickensToCoopDataSet>, public MJChickenDelegate {
public:
    CREATE_FUNC(MJGameChickensToCoop);
    static MJGameChickensToCoop * createGame(mj::GameDataSet<mj::ChickensToCoopDataSet> data);
    static mj::GameDataSet<mj::ChickensToCoopDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::ChickensToCoopDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::ChickensToCoopDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void adjustDifficulity();
    void didLoadFromCSB(mj::GameDataSet<mj::ChickensToCoopDataSet> data);
    void onEnter() override;
    void onExit() override;
    bool loadQuestion();
    void loadCoop();
    void loadChicken();
    void showGuide();
    void spawnChicken(mj::GameObject word, float position_y);
    void reOrderChickens();

    void run(MJChicken * chicken);

    void onChickenDraggedTo(MJChicken * chicken, cocos2d::Point position) override;
    void onChickenTouchedTo() override;
    void inActiveMode();
    void addSpeaker();
    
    mj::ChickensToCoopDataSet _current_set;
    std::vector<mj::GameObject> _shuffle_words;
    std::vector<MJChicken *> _chickens;
    std::vector<MJCoop *> _coops;

    std::vector<std::vector<MJChicken *> > _row;

    int _id_audio_chicken_run;
	int chicken_to_coop = 0;
    int _repeat_number;
    int _coop_door;
    cocos2d::Vec2 _postBeginChickenSugettion = cocos2d::Vec2(0, 0);
    std::string audio_path;
    int _idAudioIntro = 0;
    bool _isPlayAudioIntro = true;
    bool _isShowGui = true;
    std::function<void()> _callbackAddSpeakAndShowGuide = nullptr;
};

CREATE_READER(MJGameChickensToCoop);

#endif /* GameChickensToCoop_hpp */
