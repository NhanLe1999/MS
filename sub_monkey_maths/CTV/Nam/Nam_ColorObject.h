//
//  Nam_ColorObject
//  MJQuiz
//
//  Created by HuyPhuong on 13/9/17.
//
//
#ifndef Nam_ColorObject_h
#define Nam_ColorObject_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_ColorObject : public Math_Quiz
{
public:
    static Nam_ColorObject* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;

    //Game config

    // mode ColorObject
	bool pickColor;
	std::string typeCompare;
	std::string keyCompare;
    math::comparision::Key compare_key;
    
	int resultNumber;
	std::string resultIdImage;
	// mode greather or smaller
	std::string keyGame;
	std::string keyColor;
    
    // color config
	int colorNumber;
    int sameNumber;

    std::vector <std::string> colors;
    std::vector <Math_Object*> colored_objects;

    std::vector <math::resource::Image*> white_imgs;
    std::vector <math::resource::Image*> color_imgs;
    std::vector <math::resource::Image*> number_imgs;
    std::vector <math::resource::Image*> ordinal_imgs;
	std::vector <math::resource::Image*> imagesColor;
    math::resource::Image* first_img;
    
	// object config
	int typeNumber;
	cocos2d::Size objectSize;
	math::resource::Image* imageBackground = nullptr;
	std::string valueType;
	std::vector <math::resource::Image*> allImagesObject;
	std::vector <math::resource::Image*> imagesObject;
    //callback Func
	std::string currentColor;
    void onTouchedObject(Node* node);
	void onTouchedColor(Node* node);
    //debugging mode
    int testing_step=0;

    void autoTestingRun(float dt);

};

#endif /* Nam_ColorObject_h */
