//
//  FindTheWay.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//
#ifndef FindTheWay_hpp
#define FindTheWay_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
#include "ParseJSONHelper.hpp"
#include "Math_Object.h"
#include "PhonicInfoModel.h"

//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class FindTheWay : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol  {
public:

	CREATE_FUNC(FindTheWay);
    static FindTheWay * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
	void onStart();
	void onGuideStarted();
	void onGuideDone();
	void loadQuestion();
	void startQuestion();
	void nextQuestion(float delay_time = 0.5f);
	void onExplanation(bool correct);
	void showImageEndQuiz();
	void autoTesting();
	void finishQuestion();

    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    void onClickHolder(Ref* sender);
    void onClickSpeaker(Ref* sender);
    void readAnswer(int count);
    Layout* root_layout;

	math::resource::Image * img_map;
	math::lang::MathLanguage lang;
	int game_index = 0;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Regular.ttf";
    int font_sz = 37;
    bool autoPlay = false;


private:
	// trả về 1 đường đi đúng bắt đầu từ điểm đầu
	int getGuidePath(int start_point);
	cocos2d::ui::Layout* _panel_move;
	cocos2d::ui::Layout* _panel;
	cocos2d::ui::Layout* _panel_disable;

	Math_Object * character_start;
	Math_Object * character_end;
	math::resource::Image*  img_tracing = nullptr;
	cocos2d::Size           tracing_size;
	cocos2d::Vec2           tracing_center;
	std::string             color_tracing;


	cocos2d::ui::Text* m_lbl_question;
	cocos2d::ui::ImageView * img_character = nullptr;
	math::JSON::MMTrace trace_data;

	struct GameData {
		cocos2d::Size map_size;
		mj::PHONIC_INFO question;
		math::resource::Image * map_image;
		math::resource::Image * character_start;
		math::resource::Image * character_end;
		math::resource::Image * animation;
        bool retry_pos_right;
        std::string pos_character_end;
		GameData() : 
			map_size(1700, 768),
			map_image(nullptr),
			character_start(nullptr),
			character_end(nullptr),
			animation(nullptr),
            retry_pos_right(true),
        pos_character_end("right")
		{}
	};

	
	GameData mCurrentData;
	std::vector<GameData> mGameDataSet;
    Button * retry_btn;
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

	struct edge {		
		int start;
		int end;
		int path_index;
		int priority;
		MMLine line;
		edge() {
			start = 0;
			end = 0;
			path_index = 0;
			priority = 0;
		}

		edge(int start, int end) {
			this->start = start;
			this->end = end;
			line = MMLine();
		}
	};

	edge current_edge;

	int current_node;

	std::vector<edge> findNextEdges(int point, int path_index);
	std::map<int, std::vector<edge>> graph;

	Math_Object* game_object = nullptr;
	cocos2d::Vec2 current_point;
	cocos2d::Vec2 last_point;
	cocos2d::Vec2 last_item_point;

	std::map<int,Math_Object*> map_objects;
	std::vector<Math_Object*> item_list;

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

	cocos2d::EventListenerTouchOneByOne* touch_listener;
	void addEvents();
	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
	void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;

	Math_Object* touchOnObject(cocos2d::Vec2 point);

	void speakQuestion();
	void goNextPath();
	void goStartPath();
	std::vector<int> next_paths;
	std::vector<int> getNextPathIndexes(int path_index);
	std::vector<int> getBackPathIndexes(int path_index);
	bool isInPath(cocos2d::Vec2 point);
	void startTracing(bool next_path, float dt = 0);
	void activeItem(Math_Object* object);

	void enableControlQuiz();
	void disableControlQuiz(float time = 0);

	bool canNextPath(int path_index);

	void onShowItem(Node* item, float delay, float duration, cocos2d::CallFunc* call_func);

	cocos2d::Color3B getColor3B(std::string color);

	cocos2d::DrawNode* _drawer = nullptr;
	std::map<int,cocos2d::DrawNode*> _drawers;
	cocos2d::Color4F color_line;
	void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
	float font_size = 200.0f;
    int q_id = -1;
    //show/hide chấm trên đường đi
    void showPoint();
    void hidePoint();
    //lấy obj trên đường đi hiện tại
    std::vector<Math_Object*> getTracingObject(bool is_tracing = true);
    std::vector<Math_Object*> not_tracing_items;
    
    //start pos để reset game
    Vec2 start_pos = Vec2(0, 0);
    
	// Dùng DFS tìm đường đi đúng
	void findRightPath_DFS();
	void findRightPath_DFS(int start_path,int end_path, bool * visited, std::set<int> path_list);
	std::map<int, bool> m_right_path;
};
CREATE_READER(FindTheWay);

#endif /* FindTheWay_hpp */
