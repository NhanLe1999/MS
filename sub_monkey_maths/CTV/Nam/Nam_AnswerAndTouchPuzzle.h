//
//	Answer And Touch Puzzle
//  MJQuiz
//
//  Created by Tunado on 25/9/2017
//
//
#ifndef Nam_AnswerAndTouchPuzzle_h
#define Nam_AnswerAndTouchPuzzle_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
struct Calculation
{
	int number1, number2, result;
	std::string key;
};
class Nam_AnswerAndTouchPuzzle : public Math_Quiz
{
public:
    static Nam_AnswerAndTouchPuzzle* createQuiz();
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
	void update(float dt) override;
private:
    bool init() override;
	float delay = 0;
    struct Dot {
        int index;
        float x;
        float y;
    };
	//IMAGE
	std::vector <math::resource::Image*> puzzleImg;
	//puzzle
	cocos2d::ui::ImageView* objOutFrame = nullptr;
	Size puzzleSize;
	int numberPuzzle;
	std::vector <Math_Object*> puzzleObject;
	cocos2d::ui::ImageView* resultObjectOfPuzzle = nullptr;
	Math_Object* bangPhepTinh = nullptr;
	void setTextPuzzle(Math_Object* obj, int number,Dot d);
	//Caculation
	int indexOfCurrentCaculation;
	std::vector <Calculation> vecCaculation;
	std::vector <int> vecResults;
	int minResult, maxResult;
	//color for text
	std::vector <Color4B> colorVec;
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(Node* node, int index);
	bool isUpdating;
	void nextStep();
	std::vector <std::string> vecStrPhepTinh;
	int keyBangPhepTinh = 0;
	

};

#endif /* Nam_AnswerAndTouchPuzzle_h */
