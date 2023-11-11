//
//  IntroQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#ifndef IntroQuiz_h
#define IntroQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class IntroQuiz : public Math_Quiz
{
public:
    static IntroQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;

    void onAnswerDone(Node* node);
    
    int grade = 1;
    int skill = 1;
    int unit = 1;
    
    std::string intro_path;
    cocos2d::Vec2 center;
    cocos2d::Vec2 delta_GAF = cocos2d::Vec2::ZERO;
    std::vector<math::resource::Image*> img_list;
    
    gaf::GAFObject* content = nullptr;
    gaf::GAFObject* monkey = nullptr;
    gaf::GAFObject* ending = nullptr;
    
    cocos2d::ui::ImageView* img_board;
    
    void playBeginingDone(std::string name);
    void playContentDone ();
    
    math::SequenceAudio* seq_playing = nullptr;
    
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    void autoTestingRun(float dt);
};

#endif /* IntroQuiz_h */
