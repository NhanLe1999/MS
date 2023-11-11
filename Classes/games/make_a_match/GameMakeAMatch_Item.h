
#ifndef GameMakeAMatch_Item_hpp
#define GameMakeAMatch_Item_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class MJGameMakeAMatch_ItemDelegate {
public:
    virtual void onFlipCardWithName(Ref *sender) {};
};


class MJGameMakeAMatch_Item : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MJGameMakeAMatch_Item);
    static MJGameMakeAMatch_Item * createFrame(mj::GameObject object, int type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    std::string getObjectName();
    
    enum Item_Type {
        Text_Type = 1,
        Img_Type = 2,
        Merge_ImgText_Type = 3,
        Merge_TextText_Type = 4,
    };
    void setColorForLabel(Color3B color);
    void setDelegate(MJGameMakeAMatch_ItemDelegate * delegate);
    void flipCard(Node *card1, Node *card2);
    ImageView *box1 = nullptr;
    Layout *box2 = nullptr;
    mj::GameObject _data;
    void setOnResults();
    int _type = 0;
    Text* text = nullptr;
    Text* getText();
    void setFontSizeText(float fontSize);
    float getFontSizeText();
private:
    void didLoadFromCSB(mj::GameObject object, int type);
    Color3B getColor();
    void onFlip(Ref *sender);
    MJGameMakeAMatch_ItemDelegate * delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    bool isRunAni = false;
    Text* label = nullptr;
    Layout* layout_type;
};

CREATE_READER(MJGameMakeAMatch_Item);

#endif /* GameMakeAMatch_Item_hpp */
