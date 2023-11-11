//
//  Nam_AddObjectAndAnswerQuiz.h
//  MJQuiz
//
//  Created by Tunado on 8/1/17.
//
//

#ifndef Nam_AddObjectAndAnswerQuiz_h
#define Nam_AddObjectAndAnswerQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_AddObjectAndAnswerQuiz : public Math_Quiz
{
public:
	static Nam_AddObjectAndAnswerQuiz* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void update(float dt)override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;
	//void autoTest();
	void autoTestingRun(float dt);
private:
	bool init() override;
	// Game config
    bool isActivity;
    std::string game_mode;
	std::string linePath;
	math::resource::Image* img_bg_number = nullptr;
	math::resource::Image* img_frame_object = nullptr;
    math::resource::Image* img_result_bg = nullptr;
	cocos2d::Size layer_size = cocos2d::Size(900, 750);
	bool sequence_line;
	int number_icon_left;
	int number_icon_right;
	int total;
	int max_total;
	int number_of_line;
	std::string style_display_icon;
	bool divide_icon;
	// object
	cocos2d::Size object_size;
	std::string object_bg;
	//image cho object
	std::string name_icon;
	math::resource::Image* image_icon = nullptr;
	// danh sách câu hỏi
	// object danh sách số
	cocos2d::ui::ImageView* box1 = nullptr;
	cocos2d::ui::ImageView* box2 = nullptr;
    cocos2d::ui::ImageView* line1 = nullptr;
    cocos2d::ui::ImageView* line2 = nullptr;
	std::vector <Math_Object*> numbers_object;
	std::vector <Math_Object*> left_objects;
	std::vector <Math_Object*> right_objects;
	Math_Object* line = nullptr;
	
	// hàm gen object hiện kí tự
	void gen_value_object(Math_Object* object, cocos2d::Size object_size, float font_size, std::string text, cocos2d::Color3B color);
	// hàm sắp xếp object bên trái và sắp xếp bến phải trong mode hiện sắn
	std::vector <Vec2> straight_right(int n);
	std::vector <Vec2> v_style_right(int n);
	std::vector <Vec2> straight_left(int n);
	std::vector <Vec2> v_style_left(int n);
	float ratio_wild;
	// số object bên phải đang được bật
	int countObject;
	void AddObject(cocos2d::Node* node);
	void RemoveObject(cocos2d::Node* node,int index); 
	void onTouchObject2(cocos2d::Node* node, int index);
	// ham kiem tra ket qua
	void onAnswerDone1(bool correct);
	void onAnswerDone2(cocos2d::Node* node,int index);
	// button done va next game
	Math_Answer* btn_done = nullptr;
	Math_Answer* btn_next = nullptr;
	// biểu thức kết quả
	std::vector<Math_Object*> result;
	std::vector <std::string> audios_result;
	std::vector <Math_Object*> lines;
	math::resource::Image* imgline = nullptr;
	// debuging
	int testing_step = 0;
	
	// image number and operator
	std::vector <math::resource::Image*> image_number;
	math::resource::Image* image_equal = nullptr;
	math::resource::Image* image_plus = nullptr;
	//
	bool numbers_show_begin;
	bool display_sequence_number;
	std::vector <Math_Answer*> btn_result;
	std::vector <int> number_result;
};


#endif /* Nam_AddObjectAndAnswerQuiz_hpp */
