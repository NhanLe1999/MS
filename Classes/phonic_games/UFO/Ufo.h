#ifndef _Ufo_h_
#define _Ufo_h_

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "Ufo_sheep.h"
#include "GameData.h"

USING_NS_CC;

struct Ufo_Data {
	//Từ cần điền của màn chơi
	std::string text;
	//Link ảnh
	std::string image_link;
	//Sound của từ
	std::string n_sound;
	std::string p_sound;
	//Vị trí các chữ cái trống trong từ
	std::vector<int> position;
	//Số lượng câu trả lời
	int number;
	//Câu trả lời đúng
    mj::GameObject right_answer;
    std::string right_sub_text;
	//Các đáp án
	std::vector<mj::GameObject> answer;
};

class Ufo :public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(Ufo);
	static Ufo *createGame(std::string json_file, bool is_auto_play = false);
	static Ufo *createGame(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type = GAME_TYPE_PHONIC);
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play);

private:
    std::string question_vi = "";

	GAFWrapper* blackhole;
	GAFWrapper* alien;
	GAFWrapper* alien_light;
	//vector lưu các đáp án
	std::vector<Ufo_Sheep*> sheeps;
	//Dữ liệu trò chơi
	std::vector<Ufo_Data> list_data;
	int number_answer;
	bool is_auto_play;
	bool clickable = false;
	bool clickedText = false;
	std::vector<mj::PHONIC_INFO> _list_phonic_info;
	cocos2d::ui::Layout* root_layout;
	//mj::PHONIC_WORD phonic_word;
	//std::vector<int> list_pos;
	//std::vector<std::string> list_answer;
	//std::string answer_audio;
	Ufo_Sheep* right_answer;
	std::vector<GAFWrapper*> list_firework;

	std::vector<cocos2d::ui::Text*> list_text;
	std::vector<GAFWrapper*> list_light;
	std::vector<GAFWrapper*> list_light_2;
	GAFWrapper* textholder;
	cocos2d::ui::Layout* panel;
	cocos2d::ui::ImageView* image;
	cocos2d::ui::Layout* trans_layout;
	//Số màn chơi hiện tại
	int stage_count = 0;
	//Tổng số màn chơi
	int number_stage;

	//void onEnter() override;
	//void onExitTransitionDidStar();
	void loadFrame();
    void onEnter() override;
	void onFinishGame() ;
    
	void setSheepsPosition(std::vector<Ufo_Sheep*> sheeps);

	std::vector<cocos2d::Vec2> getTextPosition(std::vector<cocos2d::ui::Text*> texts, cocos2d::Vec2 point, float space);
	void setTextPosition(std::vector<cocos2d::ui::Text*> texts, cocos2d::Vec2 point);
	void moveText(std::vector<cocos2d::ui::Text*> texts, cocos2d::Vec2 Point);
	void onClickSheep(cocos2d::Ref* sender);
	void onClickText(cocos2d::Ref* sender);
	void onBeginStage();
	void onNextStage();
	void onClickImage(cocos2d::Ref* sender);
    
    
    void inactiveMode(bool enabled, float delay = 7.0f);
};

CREATE_READER(Ufo);

#endif // !_Ufo_h_
