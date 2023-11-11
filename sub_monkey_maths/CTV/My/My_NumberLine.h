#ifndef __MY_NUMBER_LINE_H__
#define __MY_NUMBER_LINE_H__

#include "Math_Quiz.h"

class My_NumberLine : public Math_Quiz {
public:
	static My_NumberLine* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);
	void drawSemiCircle(cocos2d::DrawNode* node, cocos2d::Point origin, float radius, float degree, cocos2d::Color4F color, int num_points);

private:
	bool init() override;
	int min_number;
	std::string game_style;
	int distance;

	int operand;
	int result;
	std::vector<Math_Object*> operand_object;

	cocos2d::Point center_pos;
	float radius;

	int number_object;
	cocos2d::Size object_size;

	std::vector<math::resource::Image*> number_imgs;
	math::resource::Image* bg_img = nullptr;

	//Vector lưu giá trị các đáp án
	Math_Answer* right_answer = nullptr;
	std::vector<std::string> answer_values;

	cocos2d::DrawNode *drawNode = nullptr;

};

#endif // !__MY_NUMBER_LINE_H__
