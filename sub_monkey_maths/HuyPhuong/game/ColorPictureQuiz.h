//
//  ColorPictureQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef ColorPictureQuiz_h
#define ColorPictureQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class ColorPictureQuiz : public Math_Quiz
{
public:
    static ColorPictureQuiz* createQuiz();
    
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
    
    struct PicturePoint {
        int index;
        int x;
        int y;
    };
    std::vector<PicturePoint> points;
    
    //Game config
    cocos2d::Size object_size;
    int number_object;
    int colored_object;
    std::string current_color = "";
    std::string ask_object = "";
    std::string ask_color = "";
    
    cocos2d::Vec2 picture_center;
    cocos2d::Size picture_size;
    
    //background image
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_list_color;
    math::resource::Image* img_background = nullptr;
    std::vector<math::resource::Image*> img_color;

    std::vector<Math_Object*> objects_color;

    //for story
    bool color_one = true;
    std::vector<std::string> selected_color;
    std::vector<std::string> list_colors;
    
    std::vector<std::pair<std::string, std::string>> object_color_list;

    //callback Func
    void onTouchedObject(Node* node);
    void onAnswerDone(Node* node);
    cocos2d::Vec2 getLocation(int point_index);

    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
};


#endif /* ColorPictureQuiz_h */
