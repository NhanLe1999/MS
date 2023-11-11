
#ifndef GameJumpingFrog_Item_hpp
#define GameJumpingFrog_Item_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "GAFWrapper.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class MJGameJumpingFrog_ItemDelegate {
public:
    virtual void onClickItem(Ref *sender) {};
};


class MJGameJumpingFrog_Item : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MJGameJumpingFrog_Item);
    static MJGameJumpingFrog_Item * createFrame(mj::GameObject object, int type, int font_size = 80);
    
    std::string getObjectName();
        
    void setDelegate(MJGameJumpingFrog_ItemDelegate * delegate);
    mj::GameObject _data;
    bool isTouchEnable = true;
    void whenWrongAnswer();
    void whentrueAnswer();
    void whenClickTrue();
    Label *text = nullptr;
    
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(mj::GameObject object, int type, int font_size);
    MJGameJumpingFrog_ItemDelegate * delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    int _type = 0;
    
    void onChooseWord(Ref *sender);
    GAFWrapper *item_gaf = nullptr;
};

CREATE_READER(MJGameJumpingFrog_Item);

#endif /* GameJumpingFrog_Item_hpp */
