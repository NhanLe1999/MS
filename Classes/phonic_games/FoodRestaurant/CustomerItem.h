//
//	CustomerItem.h
//	Created by mintzy on 28/06/2018
//
//
#ifndef CustomerItem_h
#define CustomerItem_h

#include "GAFWrapper.h"
#include "FoodRestaurantModel.h"

USING_NS_CC;

class CustomerItemDelegate;
class CustomerItem;

enum class CUSTOMER_STATE{
	IDLE,
	WALKING,
	WIN,
	LOSE,
	WAITTING_FOOD,
	SHOW_ORDER,
	HIDE_ORDER
};

class CustomerItemDelegate {
public :
	virtual void onChangeCustomerItemStateDelegate(CUSTOMER_STATE state) {};
};
class CustomerItem : public cocos2d::ui::Layout {

public :
	CREATE_FUNC(CustomerItem);
	static CustomerItem * createItem(FoodRestaurantInfo info,int customer_id = 4);
	void showOrder(std::function<void()> callback = nullptr);
	void hideOrder(std::function<void()> callback = nullptr);
	void playGafSequence(std::string sequenceName, bool loop = false, std::function<void()> call_back = nullptr);
	void setInfo(FoodRestaurantInfo  info);
	void setDelegate(CustomerItemDelegate * delegate);
	void setInfoNumberRepeat(int re);
	void setCustomerFlipped(bool flipped);
	bool isCustomerFlipped();
	FoodRestaurantInfo getInfo() const;
	float font_size = 80;
private:
	void didLoadFromCSB(FoodRestaurantInfo info, int customer_id = 4);
	void loadFrames();	
	void onChangeCustomerState(CUSTOMER_STATE state);
	std::string font_name = "fonts/Linotte Regular.ttf";
	FoodRestaurantInfo _food_restaurant_info;
	int customer_id;
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * order_layout;

	GAFWrapper * gaf_customer;
	cocos2d::ui::ImageView * img_talk;
	cocos2d::ui::Text * lbl_order_info;
	CustomerItemDelegate * m_delegate;
	CUSTOMER_STATE m_state;
};


#endif // !CustomerItem_h
