#ifndef __MY_ADDITION_AND_SUBTRACTION_TERM_H__
#define __MY_ADDITION_AND_SUBTRACTION_TERM_H__

#include "Math_Quiz.h"

class My_AdditionAndSubtractionTerm : public Math_Quiz {
public:
	static My_AdditionAndSubtractionTerm* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	std::string operation_type;
	
	cocos2d::Size object_size;

	std::string term;
	int term_value;
	std::vector<std::string> answer_values;

	std::vector<std::string> term_values;
    std::vector<std::string> term_values_img;

    int operand_1;
	int operand_2;
	int result;
	std::vector<math::resource::Image*> icons;

	Math_Answer* right_answer = nullptr;
};

#endif // !__MY_ADDITION_AND_SUBTRACTION_TERM_H__

