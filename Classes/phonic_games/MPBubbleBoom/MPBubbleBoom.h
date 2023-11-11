//
//	MPBubbleBoom.h
//	Created by mintzy on 22/02/18
//
// 
#ifndef MPBubbleBoom_h
#define MPBubbleBoom_h

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "MPBubbleBoom_Item.h"
#include "PhonicInfoModel.h"
#include "MPBubbleBoom_Pool.h"

USING_NS_CC;
using namespace ui;


class MPBubbleBoom : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public MPBubbleBoom_ItemDelegate {

public:

	CREATE_FUNC(MPBubbleBoom);
	static MPBubbleBoom * createGame(std::string json_file);
	static MPBubbleBoom * createGame(std::string json_file,std::vector<math::resource::Image *> images);
	static MPBubbleBoom * createGame(mj::GAME_PHONIC_INFO game_phonic_info, int max_bubble_push = 30);
	void generateData(rapidjson::Value value_data);
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);

private:
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::string json_file,std::vector<math::resource::Image*> images);
	void didLoadFromCSB(mj::GAME_PHONIC_INFO game_phonic_info, int max_bubble_push = 30);
	
	mj::GAME_PHONIC_INFO _game_phonic_info;
	std::vector<mj::PHONIC_INFO> _list_phonic;
	vector<MPBubbleBoom_Item *> _bubbles;
	std::vector<Vec2> _locations;
	int max_bubble_push;
	bool play_text = false;			// true only show text, false only show image
	bool play_text_image = false;

	bool region_used[6] = { false };
	cocos2d::Size screen_size;
	int max_bubble;
	int min_bubble;
	int combo_count;
	int bubble_push_count;
	float bubble_speed;
	float time_now;
	float last_time;
	float bubble_push_time;
	bool push_locked;
	int soundId = -1;
	// Component
	cocos2d::ui::Layout * layout_root;
	cocos2d::ui::Layout * layout_bg;
	cocos2d::ui::Layout * layout_sand;

	//gaf
	GAFWrapper * gaf_seaweed_left;
	GAFWrapper * gaf_seaweed_right;
	GAFWrapper * gaf_sponge;
	GAFWrapper * gaf_crab;
	
	void loadFrames();
	void showIntroGame();
	void onRefresh(cocos2d::Ref *sender);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	void onClickedBubbleBoomDelegate(cocos2d::Ref *sender) override;
	void onChangeBubbleBoomStateDelegate(cocos2d::Ref * sender,int state) override;
	void pushBubble(int num_bubble);
	void smallBubble(float dt);
	void showTapGuideGame(float delay_time = 5.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideTapGuideGame();	
	void scheduleSoundGuiding();
	void showFinishBubble();
	inline std::vector<MPBubbleBoom_Item *> getListBubble(int num_bubble);

	// autoTest
	void autoPlay();
	int testing_step = 1;
	// Game
	cocos2d::Vec2 pos_guide;
};

CREATE_READER(MPBubbleBoom);

#endif
