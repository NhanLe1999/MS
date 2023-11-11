//
//  Firefly.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/31/18.
//

#ifndef Firefly_hpp
#define Firefly_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "PhonicInfoModel.h"
#include "Firefly_Item.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"

USING_NS_CC;
using namespace ui;

class Firefly : public MJMainGBase, public Firefly_ItemDelegate {
public:
    CREATE_FUNC(Firefly);
    static Firefly * createGame(std::string json_file, bool isAutoPlay);
   
private:
   void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    void onEnter() override;
    void gameConfig();
    void loadFrame();
    void onGameStart();
    void onFinishGame() ;
    void onExitTransitionDidStart() override;
    
    void updateFlyPostion(float time);
    Firefly_Item* createFly(std::string word, int fly_id, int word_id);

    void onTouchedDelegate(Ref* sender, ui::Widget::TouchEventType type) override;
    void onClickedBasket(Ref* sender);
    int changeFlyPosition(Firefly_Item* fly, bool changeSpeed);
    int findWordIndex(Node* fly);
    void showGuideGame(std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void hideGuideGame();
    
    mj::PHONIC_INFO word;
  
    int count_word_1 = 0, count_word_2 = 0, count_word_3 = 0, count_word_4 = 0;
    int light_level = 1;
   
    bool _is_vocab = false;
    int max_word = 0;
    int total_count = 0;
    int number_basket = 1;
    
    Layout* root_layout;
    Layout* area_51;
    std::vector<Layout*> baskets;
    std::vector<Firefly_Item*> flies;
    gaf::GAFAsset* gaf_asset;
    
    std::vector<Layout*> stars;

    //data
    std::vector<std::pair<Vec2, int>> position_list;
    std::vector<mj::PHONIC_INFO> words;
    cocos2d::ui::Button* _speaker;
    
    Size screen_size;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 50;
    bool autoPlay = false;
	void inActiveMode(float delay_time = 7.0f);
    int _currentGuidingAudio;
    bool _guideShowed = false;
    int _countWrongClick;
};
CREATE_READER(Firefly);
#endif /* Firefly_hpp */
