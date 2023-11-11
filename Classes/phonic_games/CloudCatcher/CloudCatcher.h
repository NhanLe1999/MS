//
//	CloudCatcher.h
//	Created by mintzy on  10/05/18
//
// 
#ifndef CloudCatcher_h
#define CloudCatcher_h

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "CloudCatcher_CloudItem.h"
#include "PhonicInfoModel.h"

USING_NS_CC;
using namespace ui;

class CloudCatcher : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public CloudCatcher_CloudItemDelegate {
public:
	CREATE_FUNC(CloudCatcher);
	static CloudCatcher * createGame(std::string json_file);
	static CloudCatcher * createGame(std::string json_file,std::vector<math::resource::Image *> images);
	static CloudCatcher * createGame(mj::GAME_PHONIC_INFO game_phonic_info, int max_cloud_catch = 30);
	void generateData(rapidjson::Value value_data);
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);

private:
	enum GameState {
		StartGame,
		Playing,
		Finnish
	};
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image *> images);
	void didLoadFromCSB(mj::GAME_PHONIC_INFO game_phonic_info, int max_cloud_catch = 30);
	std::string font_name = "KGWHATTHETEACHERWANTS.TTF";//PhonicConfig::getInstance()->getGameFontName();
	mj::GAME_PHONIC_INFO _game_phonic_info;
	int game_state;
	int max_cloud_catch;
	float cloud_speed;
	int cloud_catch_count;
	int combo_count = 0;
	int _speedCloud = 0;
	float last_time = 0.0f;
	float current_time = 0.0f;
	std::vector<cocos2d::Vec2> _locations_base;
	bool gen_cloud = false;
	float distance = 0.0f;
	float _maxDistance = 0;
	float max_distance;
	cocos2d::Size screen_size;
	cocos2d::ui::Layout * layout_root;
	bool clickable;
	//gaf
	GAFWrapper * gaf_bg;
	GAFWrapper * gaf_bgnight;
	GAFWrapper * gaf_pig;
	GAFWrapper * gaf_endgame;
	std::vector<CloudCatcher_CloudItem *> _clouds;
	cocos2d::ui::Text * lbl_gamename;
	void loadFrames();
    void fixBackgroundWidth();
	void showIntroGame();
	void onRefresh(cocos2d::Ref *sender);
	void updateCloud(float dt);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	void onClickedCloudDelegate(cocos2d::Ref *sender) override;
	void onChangeCloudStateDelegate(cocos2d::Ref * sender, int state) override;
	void showTapGuideGame(float delay_time = 5.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideTapGuideGame();	
	void onPlayingGame();
	void genCloud(int num_cloud, bool only_text);
	void autoGenCloud();
	int gen_cloud_step;

	int last_index = 0;

	// autoTest
	void autoPlay();
	int testing_step = 1;
	// Game
	cocos2d::Vec2 pos_guide;

	std::string _gameType = "image";
};

CREATE_READER(CloudCatcher);

#endif
