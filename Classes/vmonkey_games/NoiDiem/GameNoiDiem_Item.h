#ifndef __MJ_GAME_NOI_DIEM_ITEM_H__
#define __MJ_GAME_NOI_DIEM_ITEM_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Quiz.h"
#include "GAFWrapper.h"

class GameNoiDiem_Item : public cocos2d::ui::Layout {
public:
	static GameNoiDiem_Item * createItem(int id,math::common::TouchPermission touch,math::resource::Image * image, std::string gaf_file_path = "", bool color = false);
	void setItemSize(cocos2d::Size size);
	void setImageColor(cocos2d::Color3B color);
	void setImageVisible(bool visible);
	void setBackgroundVisible(bool visible);
	void setBackgroundColor(cocos2d::Color3B color);
	void setGAFVisible(bool visible);
	void setSuperNode(cocos2d::Node * node);
	void onShow(float dt, math::animation::ObjectShowType show = math::animation::ScaleUp, int delta = 0);
	void onShowGAF(float dt);
	void onCorrectGAF(float dt);
	void onHide(float delay_time);

	void setColoring(bool coloring);
	bool isColoring();
	bool isDetected(cocos2d::Vec2 position);
	int getId();
	
	void onTouchItem(bool correct, float delay = 0.0f, int opacity = 255);
	bool isTouchedAt(cocos2d::Vec2 point);
	void setTouchDetectedType(math::common::DetectedType detected_type);
	
	void captureState();
	void restoreCapturedState();

	GAFWrapper * getGAF();
	void setGAF(std::string gaf_file_path);
	std::string fadein_sequence;
	std::string idle_sequence;
	std::string correct_sequence;
protected:
	GameNoiDiem_Item();
	void didLoadFromCSB(int id, math::common::TouchPermission touch, math::resource::Image * image, std::string gaf_file_path, bool color);
	void initLabel(std::string text);
	void setImage(math::resource::Image * image);
	void setBackground(math::resource::Image * background);
	// GUI
	cocos2d::Node * super_node;
	cocos2d::ui::ImageView * _background;
	cocos2d::ui::ImageView * _image;
	cocos2d::ui::Text * _lbl_value;
	GAFWrapper * _gaf;
	// touch detected
	math::common::TouchPermission _touch;
	cocos2d::Image* _image_touch;
	math::common::DetectedType _detected_type = math::common::box;

	// capture node
	math::animation::CapturedNode _captured_node;

	// var
	int _id;
	
	bool _locked;
	bool _coloring;

	math::resource::Image * _img_src;
	math::resource::Image * _bg_src;

};

#endif // !__MJ_GAME_NOI_DIEM_ITEM_H__


