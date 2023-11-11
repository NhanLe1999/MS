#ifndef __My_AdditionAndAnswer__
#define __My_AdditionAndAnswer__

#include "Math_Quiz.h"

class My_AdditionAndAnswer : public Math_Quiz{
public:
	static My_AdditionAndAnswer* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);
	void onTouchButtonDone(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point);

private:
	bool init() override;

	int count = 0;
	bool clickable = false;

	std::string game_style;
	std::string number_display;
	std::string operator_display;
	bool math_display;
	bool icon_display;

	int number_object;
	//Vector lưu các object thể hiện phép toán
	std::vector<Math_Object*> _object_1;
	//Vector lưu các object thể hiện hỉnh ảnh
	std::vector<Math_Object*> _object_2;
	//Vector lưu các object mô tả phép toán
	std::vector<Math_Object*> _object_3;
	int operand_1;
	int operand_2;
	cocos2d::Size object_size;

	int number_dragger;
	cocos2d::Size dragger_size;
	//Kiểu kéo thả (kéo thả số hay kéo thả dấu?)
	bool drag_number;
	//Vector Lưu các giá trị kéo thả (nếu kéo thả số)
	std::vector<int> drag_values;
	std::vector<Math_Object*> _draggers;

	//Button Done nếu là game kéo thả
	Math_Answer* answer = nullptr;
	math::resource::Image* bg = nullptr;
	math::resource::Image* icon = nullptr;
	math::resource::Image* drag_bg = nullptr;

	//Vector lưu giá trị các đáp án
	std::vector<int> answer_values;
	int answer_pos;

	//Vector lưu giá trị các đáp án (game chọn đáp án theo mô tả)
	std::vector<std::pair<int, int>> answer_value_1;

	std::vector<math::resource::Image*> drag_imgs;
};

#endif // !__My_AdditionAndAnswer__

