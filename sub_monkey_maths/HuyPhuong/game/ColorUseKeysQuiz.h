//
//  ColorUseKeysQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef ColorUseKeysQuiz_h
#define ColorUseKeysQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class ColorUseKeysQuiz : public Math_Quiz
{
public:
    static ColorUseKeysQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;
    
private:
    bool init() override;
    
    //Game config
    math::comparision::Key compare_key;
    cocos2d::Size object_size;
    std::string fit_object;
    int number_object;
    int number_kind;
    std::string current_color = "";
    std::string ask_color = "";
    
    //background image
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_list_color;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> img_color;
    std::vector<std::pair<std::string, std::string>> object_color_list;
    
    std::vector<math::resource::Image*> key_imgs;

    std::vector<int> size_list;
    std::vector<int> number_key_list;

    cocos2d::ui::ImageView* colorBox = nullptr;
    std::vector<cocos2d::ui::ImageView*> line_boxes;
    std::vector<Math_Object*> objectsKey;

    //for story
    bool is_story_config = false;
    bool color_one = false;
    bool rand_color = true;
    bool show_table = true;
    bool sort_in_line = false;
    std::vector<std::string> selected_color;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onAnswerDone(Node* node);
    bool checkAnswer();


    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
	math::lang::MathLanguage lang;
	bool isVMonkey;
};


#endif /* ColorUseKeysQuiz_h */
