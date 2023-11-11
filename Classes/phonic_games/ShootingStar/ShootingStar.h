#ifndef ShootingStar_h
#define ShootingStar_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "ShootingStar_Item.h"

USING_NS_CC;

struct ShootingStar_Data {
	std::string text_full;
	std::string text_less;
	std::vector<std::string> psound_full;
	std::vector<std::string> psound_less;
	std::string n_sound_full;
	std::string s_sound_full;
	std::string n_sound_less;
	std::string s_sound_less;
	int pos;
};

class ShootingStar : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(ShootingStar);
	static ShootingStar *createGame(std::string json_file, bool is_auto_play = false);
	static ShootingStar *createGame(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type = GAME_TYPE_PHONIC);
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play);

private:
	void onExit() override;
	void onEnter() override;
	void generateData();
	void loadFrame();
	void onStageBegin();
	void onNextStage();
	void onFinishGame() ;

	void setStarsPosition(std::vector<ShootingStar_Item*> list_star);
	void setStarMovePosition(std::vector<ShootingStar_Item*> list_star);
	void onClickStar(cocos2d::Ref* sender);

	cocos2d::ui::Layout* star_land;
	cocos2d::ui::Layout* root_layout;

	std::string json;
	float touched = false;
	int stage_count = 0;
	int number_stages;
	std::vector<ShootingStar_Data> list_data;
	std::vector<std::string> list_star_gaf;
	std::vector<std::string> list_land_img;
	std::vector<std::string> list_sfx_effect;
	GAFWrapper* star_cloud;
	GAFWrapper* starparticle;
	GAFWrapper* starparticle2;
	GAFWrapper* star_light;
	GAFWrapper* star_light1;

	std::vector<ShootingStar_Item*> list_stars;
	std::vector<ShootingStar_Item*> list_star_move;
	cocos2d::ui::Layout* small_star;
	GAFWrapper* stars_smallstars;
	std::vector<cocos2d::Vec2> getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding = Vec2::ZERO, cocos2d::Vec2 skew = Vec2::ZERO);
	std::vector<cocos2d::Vec2> getTextLocations(cocos2d::Vec2 mid_point, std::vector<ShootingStar_Item*> list_stars, cocos2d::Vec2 padding,float scale);

	//speaker
	bool _touchable = false;
	bool _touchableForStar = true;

	void learningReminder();
	int _delaytimeAudioGuidePlayback = 10;
	std::string _audioGuidePlaybackScheduleName = "Tile Audio Game";
	void stopIntroAudioWhenPlayGame();

};

CREATE_READER(ShootingStar);

#endif // !ShootingStar_h
