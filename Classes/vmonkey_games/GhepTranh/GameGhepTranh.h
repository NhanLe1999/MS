//
//  GameGhepTranh.h
//  MJQuiz
//
//  Created by Minhtzy on 2/5/19.
//
//

#ifndef GameGhepTranh_h
#define GameGhepTranh_h

#include <stdio.h>
#include "MJMainGBase.h"
#include "Math_Object.h"
#include "Math_Slot.h"
#include "ParseJSONHelper.hpp"
#include "QuizGuideCenter.h"
#include "cocostudiomacro2.h"

class GameGhepTranh : public MJMainGBase
{
public:
	CREATE_FUNC(GameGhepTranh);
	static GameGhepTranh* createGame(std::string json_path = "config.json");

	void initGame(std::string json_path) ;
	void onStart();
	void onStartQuestion();
	void onExplanation(bool correct);
	void onComplete();

	void onGuideStarted();
	void nextQuestion(float dt);
	void onEnter() override;

private:
	bool init() override;

	int suggess_index = 0;
	bool complete = false;

	math::JSON::MMTrace trace_data;
	math::JSON::MMTrace trace_start;

	cocos2d::Size image_size;
	cocos2d::Vec2 image_center;

	cocos2d::Size start_size;
	cocos2d::Vec2 start_center;

	Math_Object* comp_object = nullptr;
	Math_Object* sugg_object = nullptr;

	Math_Object* selected_object;
	int move_count = 0;

	math::resource::Image* img_complete = nullptr;
	math::resource::Image* img_suggess = nullptr;
	math::resource::Image* img_target = nullptr;
	math::resource::Image* img_start = nullptr;

	std::vector<math::resource::Image*> img_list;
	std::vector<Math_Object*> _objects;
	std::vector<Math_Slot*> _slots;
	//callback Func
	void onMoveObjectBegan(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	
	void addChildNode(Node * node, int zOrder = 1);
	void randomShow(std::vector<Math_Object*> list, float delay = 0.0, math::animation::ObjectShowType show = math::animation::ScaleUp);
	bool isPointInnerSlot(Math_Slot * slot, cocos2d::Vec2 point, int padding);

	cocos2d::ui::Layout * _panel;
	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_locked;
	cocos2d::ui::Text * m_lbl_question;
	cocos2d::ui::Button * m_btn_loa;
	cocos2d::ui::ImageView * m_bg_question;

	void loadFrames();
	void onClickedSpeaker(Ref * sender);

	void disableControlQuiz(float time = 0);

	void enableControlQuiz();

	math::lang::MathLanguage lang;
};

CREATE_READER(GameGhepTranh);

#endif /* GameGhepTranh_h */