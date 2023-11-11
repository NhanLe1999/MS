#ifndef __MY_ADDITION_IN_TABLE_H__
#define __MY_ADDITION_IN_TABLE_H__

#include "Math_Quiz.h"

class My_AdditionInTable : public Math_Quiz {
public:
	static My_AdditionInTable* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point);
	void onTouchButtonDone(cocos2d::Node* node);
private:
	bool init() override;

	std::string number_slot;
	int min_number;
	int max_number;
	int max_sum;

	cocos2d::Size object_size;

	Math_Answer* done = nullptr;

	//Vector lưu các giá trị ở hàng ngang trên cùng
	std::vector<int> horizon_values;
	//Vector lưu các giá trị ở hàng dọc bên trái
	std::vector<int> vertical_values;

	std::vector<math::resource::Image*> horizon_imgs;
	std::vector<math::resource::Image*> vertical_imgs;	

	std::vector<Math_Object*> horizon_objects;
	std::vector<Math_Object*> vertical_objects;

	std::vector<Math_Object*> sum_objects;
	std::vector<Math_Object*> empty_objects;

	std::vector<int> dragger_values;
	std::vector<math::resource::Image*> dragger_imgs;
	std::vector<Math_Object*> _draggers;

	std::vector<Math_Slot*> _dragger_slots;

	cocos2d::DrawNode* drawNode = nullptr;

	std::vector<math::resource::Image*> numbers;

	int min;
	int max;
	int number_answer;
	Math_Answer* answer;
	std::vector<int> answer_values;

	math::resource::Image* bg_img = nullptr;
	math::resource::Image* bg_img_1 = nullptr;
	math::resource::Image* bg_img_2 = nullptr;

	cocos2d::ui::ImageView* background = nullptr;

	//Vector lưu những vị trí trống
	std::vector<int> slot_index;

};

#endif // !__MY_ADDITION_IN_TABLE_H__
