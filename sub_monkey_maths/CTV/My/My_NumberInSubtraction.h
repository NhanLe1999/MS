#ifndef My_NumberInSubtraction_h
#define My_NumberInSubtraction_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class My_NumberInSubtraction : public Math_Quiz {
public:
	static My_NumberInSubtraction* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;
    
	void autoTestingRun(float delaytime);

    //Call func
    void onTouchedObject(Node* node);
    void onMoveObjectBegan(Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchAnswer(cocos2d::Node *node);

private:
	bool init() override;

	int number_object;
	int number_remove;
	int result;

    bool drag;
    
    bool show_icon;
    bool touch_remove;
	std::string multiple;
    cocos2d::Size icon_size;

    cocos2d::Size object_size;
    std::string object_bg;
    std::string object_show;
    std::string object_value_position;
    std::string operator_show;

    int number_dragger;

	int testing_step = 0;
	//Số object đã remove
	int object_removed = 0;
	//Vector lưu các đáp án chưa Drag khi Test
	std::vector<Math_Object*> test_draged;
	//Vector lưu các đáp án chưa chọn khi Test
	std::vector<Math_Answer*> test_chose;

	int x = 0;
	int y = 0;

	bool clickable = false;

	//Biến đếm những object đã được chọn
	int count = 0;

	bool isTouch = false;

	cocos2d::Color3B originColor;

	//Vector lưu ảnh cho các chữ số
	std::vector<math::resource::Image*> number_imgs;

	//Vector lưu object các chữ số
	std::vector<Math_Object*> number_objects;

	//Vector lưu những object đã chọn
	std::vector<Math_Object*> selected_Objects;

	//Vector lưu hình ảnh của objects
	std::vector<math::resource::Image*> img_list;
	math::resource::Image* image = nullptr;
	std::vector<math::resource::Image*> icon_imgs;

	//vector lưu các draggers
	std::vector<Math_Object*> _draggers;
    std::vector<Math_Object*> _icons;

	//Vector lưu giá trị đáp án
	std::vector<int> drag_values;

	std::vector<math::resource::Image*> num;

	cocos2d::Size dragger_size;
	math::resource::Image *img_bg = nullptr;

};

#endif /* My_NumberInSubtraction_h */
