#ifndef __My_MissingNumber_H__
#define __My_MissingNumber_H__

#include "Math_Quiz.h"

class My_MissingNumber :public Math_Quiz {
public:
	static My_MissingNumber* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;
    
	void autoTestingRun(float delaytime);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onAnswerDone(cocos2d::Node* node);
	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	int result;

	int number_object;
	cocos2d::Size object_size;
	std::string object_value;
	//Vector lưu hình ảnh cho object
	std::vector<math::resource::Image*> object_imgs;

	int number_slot;
	cocos2d::Size slot_size;
    std::vector<int> slots_pos;
	math::resource::Image* slot_bg = nullptr;
    bool number_line;

	bool drag;
	int number_dragger;
	cocos2d::Size dragger_size;
	//Vector lưu hình ảnh cho dragger
	std::vector<math::resource::Image*> dragger_imgs;
    std::vector<Math_Object*> commas;

    //Vector lưu giá trị cho các đáp án
    cocos2d::DrawNode* draw = nullptr;

	std::string count_style;
	int min_number;
	int count_distance;

	bool clickable = false;

	int testing_step = 0;
	//Vector lưu những object đã được kéo khi test
	std::vector<Math_Object*> test_draged;
	//Vector lưu những câu trả lời đã đưuọc chọn
	std::vector<Math_Answer*> test_chose;
};

#endif //__My_MissingNumber_H__
