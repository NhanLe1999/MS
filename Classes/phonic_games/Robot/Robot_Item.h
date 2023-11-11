
#ifndef Robot_Item_HPP
#define Robot_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;
class Robot_ItemDelegate {
public:
    virtual void onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type, int item_pair, std::string audio_path) {};
};

class Robot_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(Robot_Item);
    struct Phonic {
        std::string text;
        std::string img_path;
        std::string audio;
		int pair;
        bool is_right;
    };
    static Robot_Item * createItem(Robot_Item::Phonic phonic, std::string img_path);
    void playEffect(bool correct);
    void setDelegate(Robot_ItemDelegate* delegate);
   
    void setHint(bool is_hint);
    bool getHint();
    
    void setAudio(std::string audio);
    std::string getAudio();
    
    void setFontSize(int size);
	Phonic getPhonic() {
		return _phonic;
	}
   
private:
    void onEnter() override;
    void didLoadFromCSB(Robot_Item::Phonic phonic, std::string img_path);
	Phonic _phonic;
    void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 80;
//    bool _water_splash = false;
    bool _hint;
    std::string _audio;
    Robot_ItemDelegate* _delegate;

};

CREATE_READER(Robot_Item);

#endif
