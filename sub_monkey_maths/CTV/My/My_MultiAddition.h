#ifndef __MY_MULTI_ADDITION_H__
#define __MY_MULTI_ADDITION_H__

#include "Math_Quiz.h"

class My_MultiAddition : public Math_Quiz {
public:
	static My_MultiAddition * createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	/*void autoTesting() override;*/

	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	int count = 0;
	bool clickable = false;

	cocos2d::Vec2 root_pos;

	std::string game_style;
	int start_number;
	int min;
	int max;
	int number_dragger;
	std::vector<int> result_numbers;
	
	int number_object;
	std::vector<Math_Object*> _objects_1;
	std::vector<Math_Object*> _objects_2;
	cocos2d::Size object_size;

	//Vector lưu dữ liệu các phép toán (số trừ/số hạng và phép toán)
	std::vector<std::pair<int, std::string>> game_data;

	//Vector lưu các giá trị kéo thả
	std::vector<int> dragger_values;
	std::vector<Math_Object*> _draggers;
	std::vector<math::resource::Image*> dragger_imgs;

	math::resource::Image* background = nullptr;
	math::resource::Image* drag_bg = nullptr;
	std::vector<math::resource::Image*> result_imgs;
	std::vector<math::resource::Image*> operand_imgs;

	std::vector<cocos2d::ui::ImageView*> ui_images;

};

#endif // !__MY_MULTI_ADDITION_H__

