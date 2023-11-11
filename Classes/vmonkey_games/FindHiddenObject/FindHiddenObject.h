//
//  FindHiddenObject.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef FindHiddenObject_hpp
#define FindHiddenObject_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "GameData.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class FindHiddenObject : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(FindHiddenObject);
    static FindHiddenObject * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
 
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    void onClick(Ref* sender);
    void onClickSpeaker(Ref* sender);
    Layout* root_layout;

    bool is_portrait = false;
    int count = 0;
    int number_obj = 0;
    mj::GameObject question;
    int q_id = -1;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Medium.ttf";
    int font_sz = 37;
    bool autoPlay = false;
};
CREATE_READER(FindHiddenObject);

#endif /* FindHiddenObject_hpp */
