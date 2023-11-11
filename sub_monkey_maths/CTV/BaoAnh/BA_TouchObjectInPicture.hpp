//
//  BA_TouchObjectInPicture.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 3/8/18.
//

#ifndef BA_TouchObjectInPicture_hpp
#define BA_TouchObjectInPicture_hpp

#include <stdio.h>
#include "Math_Quiz.h"

class BA_TouchObjectInPicture : public Math_Quiz{
public:
    static BA_TouchObjectInPicture* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    struct Dot {
        int index;
        float x;
        float y;
    };
    //Game config
    int number_question;
    //obj config
    cocos2d::Size object_size;
    
    //for dot
    std::vector<math::resource::Image*> img_list;
    std::vector<Dot> points;
    cocos2d::Vec2 image_center;
    cocos2d::Size image_size;
    
    Math_Object* template_object;
 
    //callback Func
    void onTouchedObject(cocos2d::Node* node);
    cocos2d::Vec2 getLocation(int point);
    //rieng cua game nay
    Math_Object* touched_obj = nullptr;
    int q_id;
    std::vector<math::resource::Image*> icon_obj_list;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_TouchObjectInPicture_hpp */
