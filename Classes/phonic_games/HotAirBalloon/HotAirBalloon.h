
#ifndef HotAirBalloon_HPP
#define HotAirBalloon_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "HotAirBalloon_Item.h"
#include "PhonicInfoModel.h"

USING_NS_CC;
using namespace ui;

class HotAirBalloon : public MJMainGBase, public HotAirBalloon_ItemDelegate {
public:
	CREATE_FUNC(HotAirBalloon);
	static HotAirBalloon * createGame(std::string json_file, bool isAutoPlay = false);
	void generateData(rapidjson::Value value_data);

private:
	void didLoadFromCSB(std::string json_file, bool isAutoPlay);
	mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

	
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void onClickedSpeaker(cocos2d::Ref* sender);
	void onClickedDelegate(cocos2d::Ref *sender) override;
	
	std::vector<Vec2> getArcLocations(int number, cocos2d::Vec2 central_point, float arcInDegrees, float radius);
	std::vector<cocos2d::Vec2> getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding = Vec2::ZERO, cocos2d::Vec2 skew = Vec2::ZERO);
	void onAutoPlay();
	void autoPlayRun(float dt);
	void onFinishGame() ;

	void showIntroGame();
	void showGamename();

	void inactiveMode(float delay_time = 5.0f);
	void unInactiveMode();

	inline void runBalloonTo(cocos2d::Vec2 pos, float duration, std::function<void()> call_back = nullptr);
	void idleBalloon( float idle_time,float idle_interval = 4.0f);
	void attackBalloon(HotAirBalloon_Item * item,std::function<void()> call_back = nullptr);
	void transitionQuestion(std::function<void()> call_back = nullptr);
	inline Vec2 getPointParallel(Vec2 A, Vec2 B, float xC);
	void playBalloonEffect(bool correct);
	void onNextStage();

	void nextGame();
	void showGame();
	void showQuestion();
	void showTextEndQuestion();

	std::vector<bool> _char_filled;
	void playSyncText();
	void playSlowText();
	void playNormalText();
	inline void setColorForText(cocos2d::Color3B color_text);

	std::string type_game;
	int count_fail;
	void showHint(float duration);
	struct HAB_INFO
	{
		std::string text;
		int result;
		std::string audio_path;
		std::string audio_slow_path;
		std::vector<std::string> phonic_audios;
		std::vector<std::string> phonics;
	};
	std::vector<HAB_INFO>data_game;
	HAB_INFO current_data;
	Layout* root_layout;
	Layout* mountain_layout;
	Layout* cloud_layout;
	int current_index;
	int current_result;
	bool is_syllable_game;

	std::vector<HotAirBalloon_Item*> items;
	GAFWrapper* gaf_balloon;
	GAFWrapper* gaf_waterfall;
	GAFWrapper * gaf_effect;
	GAFWrapper * gaf_effectlight1;
	GAFWrapper * gaf_effectlight2;
	cocos2d::ui::Text * text_lb;
	Layout* touch_layout;
	Button * speaker;
	void enableTouch();
	void disableTouch(int opacity = 0);

	int count_false = 0;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 150;

	//autoPlay
	int testing_step = 1;

	bool autoPlay = false;
};


#endif
