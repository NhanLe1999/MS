//
//  MJ_GameQuizLayout.h
//  M
//
//  Created by To Ngoc Linh on 5/6/17.
//
//

#ifndef MJ_GameQuizLayout_h
#define MJ_GameQuizLayout_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "MJGame.h"
#include "Math_Quiz.h"
#include "MJBaseActivity.h"

class GameQuizLayout : public ms::BaseActivity
{
public:
    
    CREATE_FUNC(GameQuizLayout);
    
    static GameQuizLayout *createGame(std::vector<int> list_quiz_id, std::vector<int> list_num_turn);
    
    void clearData();
    
private:
    void didLoadFromCSB(std::vector<int> list_quiz_id, std::vector<int> list_num_turn);
    mj::model::Game _game_model;
    void onPrev(cocos2d::Ref *);
    void onNext(cocos2d::Ref *);
    void onBack(cocos2d::Ref *);
    mj::model::Game _game;
    std::vector<mj::model::Game> _games;
    
    /*phuong*/
    void onNextExercise(cocos2d::Ref *sender);
    void onPrevExercise(cocos2d::Ref *sender);
    
    bool is_verson_1;
    
    int begin_game;
    int begin_level;
    
    int cur_exercise;
    int number_exercise;
    cocos2d::Label *label_indexing = nullptr;
    cocos2d::Label *label_correct = nullptr;
    cocos2d::ui::Layout* gamePanel = nullptr;
    
    std::vector<Math_Quiz *> list_quizes;
    std::vector<int> _list_quiz_id;
    std::vector<int> _list_num_turnplay;
    
    void createCource();
    
    void loadNextExercise(float dt);
    void loadPrevExercise(float dt);
    void loadExercise(float dt);
    void removeExercise();
    std::string search_path_quiz_add;
    
    //Delegate
    bool isAutoTesting;
    void onAnswered(bool correct);
    void onFinishGame();
    void onCloseGame() override;
    //bool game_play_enable = true;
    
    int quiz_play_type = -1;
    enum QUIZ_PLAY {
        SINGLE_QUIZ,
        LIST_QUIZ
    };
    
    enum QUIZZ_PAUSE_TYPE {
        PAUSE_WHEN_COMPLETED,
        PAUSE_AFTER_ACTION
    };
    int quizz_pause_type = 0;
    
    std::string getQuizClassNameWithID(int quiz_id);
    void onEnter() override;
    int count_turn_play = 0;
};
CREATE_READER(GameQuizLayout);
#endif /* MJ_GameQuizLayout_h */
