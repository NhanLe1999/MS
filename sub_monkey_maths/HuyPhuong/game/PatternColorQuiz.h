//
//  PatternColorQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#ifndef PatternColorQuiz_h
#define PatternColorQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

#include "PatternCreator.h"

class PatternColorQuiz : public Math_Quiz
{
public:
    static PatternColorQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    ~PatternColorQuiz();

    bool init() override;
    
    //Game config
    std::string type_game;
    int number_object;
    int number_slot;

    cocos2d::Size object_size;
    std::string object_bg;
    
    PatternCreator* pattern;
    math::comparision::Key key_compare;
    
    std::vector<int> missing_index;
    math::resource::Image* img_bg;
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_list_color;

    //Danh sach cac doi tuong keo tha.
    std::vector<Math_Object*> white_objects;
    std::vector<Math_Object*> color_objects;
    std::string color_pick = "";
    
    //callback Func
    void onTouchedObject(Node* node);
    void onAnswerDone(Node* node);
    
    //debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
};

#endif /* PatternColorQuiz_h */
