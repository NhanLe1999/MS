//
//  GameMatching.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#ifndef GameMatching_hpp
#define GameMatching_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameMatchingFrame.h"
#include "GameMatchingAnswer.h"
class MJGameMatching : public MJGameBase<MJGameMatching, mj::MatchingGameDataSet>, public MJGameMatchingAnswerDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameMatching);
    static MJGameMatching* createGame(mj::GameDataSet<mj::MatchingGameDataSet> data);
    static mj::GameDataSet<mj::MatchingGameDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::MatchingGameDataSet> generateData(rapidjson::Value value_data, std::string resource);
    static mj::GameDataSet<mj::MatchingGameDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::MatchingGameDataSet> data);
    ~MJGameMatching();
    void adjustDifficulity();

    void onEnter() override;
    void showTutorial(float dt);
    void onExit() override;
    //void showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop, float delay_time, float dt, std::string gaf_guide_path);
    void doAppear();
    void doDisappear();
    void radio();
    void loadFrames();
    void loadAnswers();
    void refreshLayout();
    void fadeUnanswered(int opacity);


    void spawnAnswer(MJGameMatchingAnswer* answer);

    std::vector<MJGameMatchingFrame*> _frames;
    std::vector<MJGameMatchingAnswer*> _answers;
    std::vector<MJGameMatchingAnswer*> _answersCopy;
    std::vector<MJGameMatchingAnswer*> _answersNotDone;
    mj::MatchingGameDataSet _current_set;
    Button* _speaker;
    void onAnswerMoved(MJGameMatchingAnswer* answer, cocos2d::Point position) override;
    void onAnswerMoving(MJGameMatchingAnswer* answer, cocos2d::Point position) override;
    void onAnswerBegan(MJGameMatchingAnswer* answer, cocos2d::Point position) override;
    void addLayoutForNotClickItem();

    bool _is_undersea;
    bool flag;
    int answerCorrect = 0;
    int _audio = -1;
    int _indexTrueAnswer = 0;
    void showGuide(cocos2d::Point, cocos2d::Point);

    bool _is_guide_showed = false;
    void inActiveMode();
    void frameClick();
    bool _handShowed = false;
    bool finishedGame = false;
    ui::Layout* _overlay;

    std::function<void()> _callbackAudio = nullptr;
    
    std::function<void()> _callbackLoadAnswers = nullptr;

    void stopGuiding();
    CC_SYNTHESIZE(bool, _isSpeaking, Speaking);
};

CREATE_READER(MJGameMatching);

#endif /* GameMatching_hpp */
