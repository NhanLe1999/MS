#ifndef __My_MathWithExplain_H__
#define __My_MathWithExplain_H__

#include "Math_Quiz.h"

class My_MathWithExplain : public Math_Quiz {
public:
	static My_MathWithExplain* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	std::string game_style;
	//Kiểu hiển thị đáp án (đúng là hiển thị đáp số, nếu không hiển thị phép toán)
	bool answer_display;

	int number_object;
	std::vector<Math_Object*> _objects_1;
	std::vector<Math_Object*> _objects_2;
	int operand_1;
	int operand_2;
	int result;
	cocos2d::Size object_size;
	math::resource::Image* object_image = nullptr;

	//Vector lưu giá trị các đáp số (trường hợp hiển thị đáp án)
	std::vector<int> answer_values;
	Math_Answer* ans = nullptr;
	//Vector lưu các phép toán (trường hợp hiển thị phép toán)
	std::vector<std::string> answer_strings;

	////Hiển thị biểu thức hay đáp án (true là hiển thị đáp án)
	//bool answer_display;

};

#endif // !__My_MathWithExplain_H__
