
#ifndef SkiingRacing_Map_HPP
#define SkiingRacing_Map_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class SkiingRacing_Map : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(SkiingRacing_Map);

	static SkiingRacing_Map * createMap(std::string name_character_1, std::string name_character_2, std::string name_main_character);
	void playEffect(bool correct);
	void update_character1(float rate);
	void update_character2(float rate);
	void update_main_character(float rate);

private:
	void didLoadFromCSB(std::string name_character_1, std::string name_character_2, std::string name_main_character);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	float long_map;
	Layout* charater_1_arrow;
	Layout* charater_2_arrow;
	Layout* main_charater_arrow;
	float origin;

	//    bool _water_splash = false;
	
	
	
};

CREATE_READER(SkiingRacing_Map);

#endif
