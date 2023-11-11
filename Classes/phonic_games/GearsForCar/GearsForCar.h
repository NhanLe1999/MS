//
//  GearsForCar.h
//

#ifndef GearsForCar_h
#define GearsForCar_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "GearsForCar_Item.h"
#include "PhonicInfoModel.h"
#include "GearsForCar_Gear.h"
using namespace cocos2d;
class GearsForCar : public MJMainGBase
{
public:
    CREATE_FUNC(GearsForCar);
    
    enum CAR_STATE{
        CS_IDLE,
        CS_DROP,
        CS_RUN_OUT,
        CS_RUN_IN
    };
    
    struct GEARS_INFO{
        std::string type;
        std::string word_audio;
		std::string word_text;
        std::vector<GearsForCar_Item::GEAR_ITEM_WITH_TEXT> items;
    };
    
    struct GAME_INFO {
        int number_level;
        std::vector<int> gears_per_level;
        std::vector<GEARS_INFO> gears_info;
    };
    
    static GearsForCar *createGame(std::string json_file);

    void itemOverLay();
    void removeItemOverLay();
    void onExit() override;
    
private:
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    GAME_INFO game_info;
    
    void onEnter() override;
    void onExitTransitionDidStar();
    void showIntroGame();
    void loadFrames();
    void startGame();
    void startLevel();
    void playCar(CAR_STATE state);

    void showGuideGame();
    void stopGuideGame();
    
    std::string getIntroPath(std::string type);
	std::string getTextQuestion(std::string type);
    cocos2d::ui::Layout *touchLayout, *item_layout, *carLayout;
    std::vector<GearsForCar_Gear*> gears;
    std::vector<GearsForCar_Item*> items;
    std::vector<Vec2> position_list;
    
    GAFWrapper *car;
    GearsForCar_Item* right_item = nullptr;
    cocos2d::ui::Button* btn_Loa;
    
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	bool is_show_question;
	bool is_show_text_item;
    int index_item = 0;
    int cur_level = 0;
    int curZ=0;
    int guide = 0;
    void showItem();
    cocos2d::ui::ImageView *bg_Gear;
    Sprite* _gameNameImage = nullptr;

    bool _flag = true;
};
CREATE_READER(GearsForCar);
#endif /* MJ_GearsForCar_h */
