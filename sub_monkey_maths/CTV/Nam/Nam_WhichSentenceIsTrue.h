//
//  Nam_WhichSentenceIsTrue
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#ifndef Nam_WhichSentenceIsTrue_h
#define Nam_WhichSentenceIsTrue_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_WhichSentenceIsTrue : public Math_Quiz
{
public:
    static Nam_WhichSentenceIsTrue* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
    //Game config
	std::string modeGame;
	
	std::string keyGame; // keyGame cong hay tru

	// sentence
	math::resource::Image* bg_sentence = nullptr;
	math::resource::Image* bg_sentence_choosed = nullptr;
	int minResult, maxResult;
	int numberSentence;
	int soDapAnDung;
	// answer
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node);
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_WhichSentenceIsTrue_h */
