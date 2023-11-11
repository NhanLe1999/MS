#pragma once
#ifndef __MATCHING_NUMBER_H__
#define __MATCHING_NUMBER_H__

#include "Math_Quiz.h"

class My_MatchingNumber : public Math_Quiz {
public:
	static My_MatchingNumber* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;

	void onTouchedObject(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);

	void onAnswerDone(cocos2d::Node* node);

	bool check();
	bool check(int i);
	bool checkTest(int i, int j);

	void autoTestingRun(float delaytime);

private:
	int count = 0;

	int testing_step = 0;

	bool clickable_test = true;
	bool clickable_done = false;

	bool init() override;

	enum GameType {
		Sum,
		Sub,
		Equal,
		Image
	};

	GameType game_type;

	int num_block;
	std::string kieu_sap_xep;
	std::string kieu_hien_thi;
	std::string phep_toan;
	int num_request;
	int num_slot;
	std::vector<std::pair<int, std::string>> convert;

	cocos2d::Size object_size;
	std::string object_bg;
	std::string object_value;
	std::string object_value_position;
	int number_object;

	int number_kind;
	int number_drager;
	cocos2d::Size drager_size;

	std::string question;

	cocos2d::Vec2 originPoint;

	math::resource::Image* img_bg = nullptr;
	std::vector<math::resource::Image*> img_list;

	math::resource::Image* icon = nullptr;

	//Vector lưu các ảnh bên trái (trên)
	std::vector<math::resource::Image*> left_numbers;

	//Vector lưu các ảnh bên phải (dưới)
	std::vector<math::resource::Image*> right_numbers;

	//Vector chứa các giá trị bên trái (trên)
	std::vector<int> left_values;

	//Vector chứa các giá trị bên phải (dưới)
	std::vector<int> right_values;

	//Danh sách các background bên trái (trên)
	std::vector<math::resource::Image*> left_imgs_bg;
	//Danh sách các background bên phải (dưới)
	std::vector<math::resource::Image*> right_imgs_bg;

	//Vector chứa các keyword
	std::vector<std::string> keywords;
	//Vị trí keyword trong vecotr keywords
	int index = 0;

	//vector chứa các dragger
	std::vector<Math_Object*> _draggers;

	//vector chứa các slot
	std::vector<Math_Slot*> slots;

	std::vector<Math_Answer*> answers;

};

#endif // !__MATCHING_NUMBER_H__
