//
//  Minh_IdentifySideAndCornerQuiz.h
//  MJQuiz
//
//  Created by MinhTzy on 7/3/18.
//
//

#ifndef Minh_IdentifySideAndCornerQuiz_h
#define Minh_IdentifySideAndCornerQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class Minh_IdentifySideAndCornerQuiz : public Math_Quiz
{
public:
    static Minh_IdentifySideAndCornerQuiz* createQuiz();
    
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
	bool mode_corner;
	bool mode_side;
    
    Math_Object * object_victim = nullptr;

    // Drager
    Math_Object * corner_drag = nullptr;
	Math_Object * side_drag = nullptr;
	// Slots
	Math_Slot * corner_slot = nullptr;
	Math_Slot * side_slot = nullptr;
	// Image
	std::vector<math::resource::Image*> img_list;

	cocos2d::Vec2 image_center;
	cocos2d::Size image_size;
	cocos2d::Size drag_size;
    std::vector<Dot> points;

    int index_path=0;
    
    //callback Func
    cocos2d::Vec2 getLocation(int point);    
	cocos2d::Vec2 getSlotLocation(int point);
	void onAnswerDone(cocos2d::Node * node);
	float autoDragCorrect();
	void onTouchedObject(cocos2d::Node *node);
	void onMoveObjectEnded(cocos2d::Node * node,cocos2d::Vec2 position);
  
    bool drawing = false;


};


#endif /* Minh_IdentifySideAndCornerQuiz */
