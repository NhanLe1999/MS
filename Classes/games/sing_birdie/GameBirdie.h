//
//  GameBirdie.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#ifndef GameBirdie_hpp
#define GameBirdie_hpp

#include "GameBase.h"
#include "GameBirdieQuestion.h"
#define GAMEBIRDIE_INSTANCE MJGameBirdie::getInstance()

class MJGameBirdieAnswerOverlay;
class MJGameBirdieQuestion;

class MJGameBirdie : public MJGameBase<MJGameBirdie, mj::BirdieDataSet>, public MJGameBirdieQuestion_ItemDelegate,public MJGameBirdieAnswerDelegate {
public:
    void setHandShowed(bool handShowed);
    static MJGameBirdie* getInstance();
    CREATE_FUNC(MJGameBirdie);
    static MJGameBirdie * createGame(mj::GameDataSet<mj::BirdieDataSet> data);
    static mj::GameDataSet<mj::BirdieDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::BirdieDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::BirdieDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    static MJGameBirdie* _MJGameBirdie;
    void didLoadFromCSB(mj::GameDataSet<mj::BirdieDataSet> data);
    void doAppear();
    void loadQuestion();
    void doDisappear();
    void showGuide();
    void createLayoutAboveImage();
  

    void updateGround(float limit);

    void onEnter() override;
    void onExit() override;
	void onPauseQuestion() override;
    
    void showGuide(cocos2d::Point start, cocos2d::Point stop);
    void inActiveMode(int time=10);
    void onAnswerBegan() override;
    void onAnswerEnded() override;
    void onAnswerDropped(MJGameBirdieAnswer* answer, cocos2d::Point position) override;
    void unscheduleInactiveMode();

    mj::BirdieDataSet _current_set;

    MJGameBirdieAnswerOverlay * _current_answers = nullptr;
    MJGameBirdieQuestion * _current_question = nullptr;
    
    bool _is_guide_showed = false;
    std::once_flag _update_guide_count;
    cocos2d::ui::Button* _speaker;
    std::string _audioPath;
    bool _isSpeakable = true;
    bool _handShowed = false;
    int _countWrongClicked;
    cocos2d::ui::Layout* _layoutImage1;
    cocos2d::ui::Layout* _layoutImage2;
};

CREATE_READER(MJGameBirdie);

#endif /* GameBirdie_hpp */
