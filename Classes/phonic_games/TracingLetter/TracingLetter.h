//
//  TracingLetter.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef __MJ_GAME_TRACING_QUIZ_H__
#define __MJ_GAME_TRACING_QUIZ_H__

#include <stdio.h>
#include "Math_Quiz.h"
#include "MJMainGBase.h"
#include "TracingLetter_Item.h"
#include "GAFWrapper.h"
#include "ParseJSONHelper.hpp"

class TracingLetter : public MJMainGBase
{
public:
    static TracingLetter* createGame(std::string configPath);
    
	void onEnter() override;
	void loadFrames();
    void onStart();
	void onGuideStarted();
	void onGuideDone();
    void startQuestion();
	void nextQuestion(float delay_time = 0.5f);
	void onExitTransitionDidStart() override;
    void onExplanation(bool correct);
	void showImageEndQuiz();
    void onFinishGame();
    void autoTesting();
	void generateData();
	void finishQuestion();
    
private:

	std::string m_configPath;
	void didLoadFromCSB(std::string configPath);
	
	bool init() override;

	cocos2d::ui::Layout* _panel_move;
	cocos2d::ui::Layout* _panel;
	cocos2d::ui::Layout* _panel_disable;

    math::resource::Image*  img_tracing = nullptr;
    math::resource::Image*  img_item = nullptr;
    cocos2d::Size           tracing_size;
    cocos2d::Vec2           tracing_center;
    std::string             color_tracing;

	cocos2d::ui::ImageView * img_character = nullptr;
	cocos2d::ui::Text * lbl_tracing;
	math::JSON::MMTrace trace_data;
    
	struct MMLine {
		//y = ax + b
		float a = 1;
		float b = 0;
		float c = 0;
		cocos2d::Vec2 p1;
		cocos2d::Vec2 p2;

		MMLine() {
			a = 1;
			b = 0;
			c = 0;
		}

		MMLine(cocos2d::Vec2 A, cocos2d::Vec2 B) {
			setup(A, B);
		}

		void setup(cocos2d::Vec2 A, cocos2d::Vec2 B) {
			p1 = A;
			p2 = B;

			auto vtcp = B - A;
			auto vtpt = cocos2d::Vec2(-vtcp.y, vtcp.x);

			a = vtpt.x;
			b = vtpt.y;
			c = -(a * A.x + b * A.y);
		}

		MMLine getPerpendicularLine(cocos2d::Vec2 M) {
			auto vtcp = p2 - p1;
			MMLine line = MMLine();
			line.p1 = M;

			line.a = vtcp.x;
			line.b = vtcp.y;
			line.c = -(line.a * M.x + line.b * M.y);
			return line;
		}

		cocos2d::Vec2 getPerpendicularPoint(cocos2d::Vec2 M) {
			if (a == 0) {
				return cocos2d::Vec2(M.x, -c / b);
			}
			else if (b == 0) {
				return cocos2d::Vec2(-c / a, M.y);
			}
			else {
				auto mline = getPerpendicularLine(M);
				float y = (mline.c * a - c * mline.a) / (mline.a * b - a * mline.b);
				float x = -(b*y + c) / a;
				return cocos2d::Vec2(x, y);
			}
		}

		float distance(cocos2d::Vec2 M) {
			auto mpoint = getPerpendicularPoint(M);
			auto d = M.distance(mpoint);
			return d;
		}

		float distanceA(cocos2d::Vec2 M) {
			auto mpoint = getPerpendicularPoint(M);
			auto d = p1.distance(mpoint);
			return d;
		}

		float distanceB(cocos2d::Vec2 M) {
			auto mpoint = getPerpendicularPoint(M);
			auto d = p2.distance(mpoint);
			return d;
		}

		bool isPointInnerEdge(cocos2d::Vec2 M) {
			auto mpoint = getPerpendicularPoint(M);

			float d0 = p1.distance(p2);
			float d1 = mpoint.distance(p1);
			float d2 = mpoint.distance(p2);
			return d0 + 5 >= d1 + d2;
		}

	};

	GAFWrapper * gaf_balloon;
	void shakingBalloon();

    TracingLetter_Item* game_object = nullptr;
	cocos2d::Vec2 current_point;
	cocos2d::Vec2 last_point;

    std::vector<TracingLetter_Item*> point_objects;
    std::vector<TracingLetter_Item*> item_list;

	int audio_id;

	//Control
	bool start_quiz = false;
	bool moveable = false;
	bool drawable = false;
	bool path_reload = false;
	bool shake_balloon = false;
	bool path_done = false;
	bool is_completed = false;
	int index_path = 0;
	int end_of_last_path = -1;
	int pair_value;

    cocos2d::EventListenerTouchOneByOne* touch_listener;
    void addEvents();
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;
    
    TracingLetter_Item* touchOnObject(cocos2d::Vec2 point);
    bool isTraceDone();
    
	MMLine current_line;

	void speakQuestion();
	void goNextPath();
	bool isInPath(cocos2d::Vec2 point);
	void startTracing(bool next_path, float dt = 0);
	void activeItem(TracingLetter_Item* object);
	TracingLetter_Item* nextItem();

	void enableControlQuiz();
	void disableControlQuiz(float time = 0);

	void onShowItem(Node* item, float delay, float duration, cocos2d::CallFunc* call_func);

	cocos2d::Color3B getColor3B(std::string color);

	cocos2d::DrawNode* _drawer = nullptr;
	std::vector<cocos2d::DrawNode*> _drawers;
	cocos2d::Color4F color_line;
	void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
	std::string font_name = "fonts/Linotte Bold.ttf";
	float font_size = 200.0f;

};


#endif /* __MJ_GAME_TRACING_QUIZ_H__ */
