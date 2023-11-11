//
//	FoodRestaurant.h
//	Created by mintzy on 28/06/2018
//
// 
#ifndef FoodRestaurant_HPP
#define FoodRestaurant_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "FoodRestaurantModel.h"
#include "FoodItem.h"
#include "CustomerItem.h"
#include "MJBaseActivity.h"

USING_NS_CC;
using namespace ui;

struct FoodItemSlot {
	FoodItem * item;
	cocos2d::Vec2 m_position;
};

struct CustomerItemSlot {
	CustomerItem * item;
	cocos2d::Vec2 m_position;
	bool reserved;
	bool check;
};
enum class FR_GAME_STATE {
	SCRIPTING,
	WAITING ,
	GUIDE,
	PLAYING,
	ENDED,
	NONE
};
class FoodRestaurant : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, FoodItemDelegate,CustomerItemDelegate{
public:
    CREATE_FUNC(FoodRestaurant);
    static FoodRestaurant * createGame(std::string json_file);
    static FoodRestaurant * createGame(std::string json_file,std::vector<math::resource::Image * > images);
    void generateData(rapidjson::Value value_data);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image * > images);

	void informationGame();

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(std::string json_file);
    void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image * > images);
 
    void onEnter() override;
    void onExitTransitionDidStart() override;
	void updateConveyor(float dt);
    void onRefresh(cocos2d::Ref *sender);

	void onChangeFoodItemStateDelegate(FOOD_ITEM_STATE state) override;
	void onTouchBeganFoodItemDelegate(cocos2d::Ref * sender) override;
	void onMoveEndedFoodItemDelegate(cocos2d::Ref * sender) override;
	void onChangeCustomerItemStateDelegate(CUSTOMER_STATE state) override;
	// Game Func 
	void loadFrames();
	void initFoodItems();
	void initCustomer();
	void showIntroGame();
	void startGame();
	void endGame();
	void earnCoin(cocos2d::Vec2 pos);
	void showGuideGame(float delay_time = 5.0f);
	void inActiveMode();
	void showHandGuideGame(std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideGuideGame();
	void setGameState(FR_GAME_STATE state);
	CustomerItemSlot * isOnCustomerSlot(FoodItem * sender);
	bool onGoingToRestaurant(CustomerItem * sender,float dt = 0.0f);
	void onGoingOutRestaurant(CustomerItem * sender);
	bool isSameInfo(FoodRestaurantInfo info1, FoodRestaurantInfo info2);
	FoodItemSlot * getSlotContaintFood(FoodItem * item);
	void ScrollingConveyor();
	void ScrollingFood();
	void addCustomer(int num,bool re_loc =false);
	bool checkNoFoodCorrect();
	void changeFoodItemsOnSlot();
	
	// Game 
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * customer_layout;
	cocos2d::ui::Layout * walk_layout;
	cocos2d::ui::ImageView * conveyor1;
	cocos2d::ui::ImageView * conveyor2;
	cocos2d::ui::ImageView * conveyor3;
	cocos2d::ui::ImageView * img_coin;
	cocos2d::ui::Text * lbl_coin;

	cocos2d::Size screen_size;
	vector<FoodRestaurantInfo> _food_infos;
	mj::GAME_PHONIC_INFO game_phonic_info;
	FR_GAME_STATE m_game_state;
	GAFWrapper * gaf_coin;
	int number_customer;
	int customer_onboard_min;
	int customer_onboard_max;
	int customer_count = 0;
	int remain_customer_onboard = 0;
	int coin_count = 0;
	int total_point;
	int num_cus_start;
	bool is_replace_customer;
	float conveyor_speed;
	float current_time;
	float last_time;
	int combo_count;
	bool flag = true;
	std::vector<FoodItemSlot *> _food_slots;
	std::vector<CustomerItemSlot *> _customer_slots;
	std::vector<CustomerItem *> _customer_items;
	std::vector<FoodItem *> _food_items;
	std::string font_name = "fonts/Linotte Regular.ttf";

	// autoTest
	void autoTest();
	int testing_step;
    std::string question_vi;
    
};

CREATE_READER(FoodRestaurant);

#endif //FoodRestaurant_HPP
