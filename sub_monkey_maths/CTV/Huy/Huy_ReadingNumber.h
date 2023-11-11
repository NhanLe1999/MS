#ifndef Huy_ReadingNumber_H
#define Huy_ReadingNumber_H

#include "Math_Quiz.h"

class Huy_ReadingNumber : public Math_Quiz {
public:
	static Huy_ReadingNumber* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;

private:
	bool init() override;
	void onAnswerDone(cocos2d::Node* node);
	void autoTestingRun(float delaytime);

	int rang_min;
	int rang_max;
	int number_value;
	
	std::vector<Math_Object*> _object_images;
	std::vector<math::resource::Image*> _image_answer;

};

#endif
