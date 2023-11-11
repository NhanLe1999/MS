#ifndef __My_NumberBone_H__
#define __My_NumberBone_H__

#include "Math_Quiz.h"
#include "Domino.h"

class My_NumberBone : public Math_Quiz {
public:
	static My_NumberBone* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void setPositionGroups(float radius, cocos2d::Vec2 center_pos, std::vector<Math_Slot*> groups);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point);
	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	//void setDominoPos(std::vector<Math_Object*> list, cocos2d::Rect rect, cocos2d::Point point);

	int count = 0;
	int number_group;
	int number_slot;
	int number_dragger;
	cocos2d::Size group_size;
	cocos2d::Size slot_size;
	cocos2d::Size dragger_size;

	std::string game_style;

	Domino* domino;

	int tong;
	bool clickable = false;

	cocos2d::DrawNode* draw = cocos2d::DrawNode::create();

	//vector lưu giá trị object hiện trên group và vị trí của nó
	std::vector<std::pair<int, int>> values;
	//Vector lưu giá trị của dragger
	std::vector<int> dragger_values;
	std::vector<math::resource::Image*> numbers;
	std::vector<Math_Object*> object_resources;

	Math_Object* obj_re_1 = nullptr;
	Math_Object* obj_re_3 = nullptr;
	Math_Object* object_tong = nullptr;
	//Vector lưu các object (game domino)
	std::vector<Math_Object*> _objects_1;
	std::vector<Math_Object*> _objects_2;
	Math_Object* obj_square = nullptr;

	//Vị trí đặt Tổng các phép toán
	cocos2d::Vec2 object_center;
	//vector lưu các dragger
	std::vector<Math_Object*> _draggers;

	Math_Answer* answer = nullptr;

	math::resource::Image* bg_1 = nullptr;
	math::resource::Image* bg_2 = nullptr;
	math::resource::Image* icon = nullptr;
	math::resource::Image* resource_1 = nullptr;
	math::resource::Image* resource_2 = nullptr;
	math::resource::Image* resource_3 = nullptr;
	math::resource::Image* resource_4 = nullptr;

	std::vector<cocos2d::ui::ImageView*> ui_image;

	//vector lưu số lượng object mỗi group (game domino)
	std::vector<int> group_value;
	//Vector lưu hình ảnh cho dragger (game domino)
	std::vector<math::resource::Image*> drag_imgs;

};

#endif // !__My_NumberBone_H__
