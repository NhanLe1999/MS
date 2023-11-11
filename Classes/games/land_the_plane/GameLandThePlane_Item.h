
#ifndef GameLandThePlane_Item_hpp
#define GameLandThePlane_Item_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class MJGameLandThePlane_ItemDelegate {
public:
    virtual void onClickLand(Ref *sender) {};
};


class MJGameLandThePlane_Item : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MJGameLandThePlane_Item);
    static MJGameLandThePlane_Item * createFrame(mj::GameObject object, int type, int font_size);
    
    std::string getObjectName();
        
    void setDelegate(MJGameLandThePlane_ItemDelegate * delegate);
    mj::GameObject _data;
    bool isTouchEnable = true;
    void addCloudWhenWrongAnswer();
    Label *text;
    
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(mj::GameObject object, int type, int font_size);
    MJGameLandThePlane_ItemDelegate * delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    int _type = 0;
    
    void onChooseWord(Ref *sender);
    GAFWrapper *cloud = nullptr;
};

CREATE_READER(MJGameLandThePlane_Item);

#endif /* GameLandThePlane_Item_hpp */
