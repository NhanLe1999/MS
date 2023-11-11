#ifndef TimTuCungVanLoai_Item_HPP
#define TimTuCungVanLoai_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class TimTuCungVanLoai_ItemDelegate {
public:
	virtual void onClickedBox(cocos2d::Ref * sender) {};
};



class TimTuCungVanLoai_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(TimTuCungVanLoai_Item);
	struct Answer {
		std::string text;
		std::string audio;
		std::vector<std::string> p_audio;
		std::vector<std::string> phonics;
		bool is_true;
        bool co_dau = false;
        std::string dau;
        int pos_dau;
	};
	static TimTuCungVanLoai_Item * createItem(Answer ans, std::string path, bool rotate);
	void setDelegate(TimTuCungVanLoai_ItemDelegate* delegate);
	void showItem();

	void setTouchEnable(bool enable);

	void setAudio(std::string audio);
	void playAudio();
	std::string getAudio();

	bool isTrue() {
		return _ans.is_true;
	}
	void effectCorrect(std::string result, bool is_phonic);
	void effectWrong(std::string result, bool is_phonic);
	
	int id;
	Text* text;
private:
	void onEnter() override;
	void didLoadFromCSB(Answer ans, std::string path, bool rotate);
	//void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
	std::string font_name = "fonts/Montserrat-Medium.ttf";
	int font_sz = 60;
	//    bool _water_splash = false;
	bool _hint;
	Answer _ans;
	std::string _audio;
	TimTuCungVanLoai_ItemDelegate* _delegate;
	Layout* item_layout;
	GAFWrapper* gaf_fish;
};

CREATE_READER(TimTuCungVanLoai_Item);

#endif
