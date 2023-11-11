#ifndef MJ_GAME_DELIVERY_FOOD_H_
#define MJ_GAME_DELIVERY_FOOD_H_ 

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "DeliveryFood_Window.h"
#include "DeliveryFood_Hand.h"

using namespace ui;
USING_NS_CC;

enum class DeliveryFoodState {
	GUIDE,
	SCRIPT_PLAY,
	WAITTING,
	START,
	PLAYING,
	ENDED
};

class DeliveryFood : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public DeliveryFood_WindowDelegate, public DeliveryFood_HandDelegate
{
public:
	CREATE_FUNC(DeliveryFood);
	static DeliveryFood * createGame(std::string json_file);
	static DeliveryFood * createGame(std::string json_file,std::vector<math::resource::Image *> images);
protected:
	void showGuideGame(int intervalTime=8.0f);
	void hideGuideGame();
private:
	struct DeliveryFoodInfo {
		mj::PHONIC_INFO info;
		int repeat;
	};
	void onClickedWindowDelegate(cocos2d::Ref * sender) override;
	virtual void onClickedHand(cocos2d::Ref * sender) override;
	virtual void onMoveBeginHand(cocos2d::Ref * sender) override;
	virtual void onMoveEndedHand(cocos2d::Ref * sender) override;
	//cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
	void generateData(rapidjson::Value value_data);
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image *> images);

	void loadFrames(std::function<void()> callback = nullptr);
	void showIntroGame();
	void startGame();
	void onFinishGame() ;
	void onRefresh(cocos2d::Ref *sender);
	void update(float dt) override;
	void onEnter() override;
	void onNextGame();
	void onNextHand();
	void onExitTransitionDidStart() override;
	
	void initWindows();
	void initHands();
	
	void onRefreshWindow();
	void earnCoin(cocos2d::Vec2 pos, DeliveryFood_Window* window);
	void deliverFoodForCustomer(DeliveryFood_Window * window);

	void setTouchLock(bool locked);
	std::vector<Vec2> getLocationWindows(int number_window,cocos2d::Size window_size = cocos2d::Size(223, 172),int padding = 20);

	void showOffer(float dt = 3.0f);

	void showHandGuideGame(int intervalTime = 8.0f ,bool offerFood = false);

	cocos2d::Size screen_size;
	DeliveryFoodState m_game_state;
	DeliveryFood_Hand * m_selected_hand;
	std::vector<DeliveryFoodInfo> m_game_data;
	std::vector<DeliveryFood_Window *> m_windows;
	std::vector<DeliveryFood_Hand *> m_hands;
	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_locked;
	cocos2d::ui::ImageView * m_background;
	cocos2d::ui::ImageView * m_coin_background;
	cocos2d::ui::ImageView* _backgroundIntro;
	cocos2d::ui::Text * m_lbl_coin;
	GAFWrapper * m_gaf_coin;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int m_wrong_count;
	int m_hand_index;
	int m_coin_earn;
	int m_total_coin;
	bool m_is_drag_game;
    bool m_touchable;
	// auto play 
	void autoPlay();
    std::string question_vi;
	std::string _audioIntro = "";
	bool isShowingGuid = false;
};

#endif // !MJ_GAME_DELIVERY_FOOD_H_
