//
//  WhoStandRightPlaceQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef WhoStandRightPlaceQuiz_h
#define WhoStandRightPlaceQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class WhoStandRightPlaceQuiz : public Math_Quiz
{
public:
    static WhoStandRightPlaceQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onTouchObject(Node* node);

    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
    //att
    
    cocos2d::Size object_size;    
    std::vector<math::resource::Image*> img_list;
    
};
#endif /* WhoStandRightPlaceQuiz_h */
