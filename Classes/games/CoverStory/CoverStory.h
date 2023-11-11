//
//  CoverStory.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef CoverStory_hpp
#define CoverStory_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "CoverStory_Item.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class CoverStory : public MJMainGBase, public CoverStory_ItemDelegate {
public:
    CREATE_FUNC(CoverStory);
    static CoverStory * createGame(std::string game_path);
//    static CoverStory * createGame(std::string json_file, std::vector<math::resource::Image*> images, bool isAutoPlay);
    
    struct answer {
        std::string text;
        std::string audio;
        bool is_right;
    };
    
    struct answers{
        std::vector<CoverStory::answer> answer_list;
        int q_id;
    };
    
private:
    void didLoadFromCSB(std::string game_path);
//    void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images, bool isAutoPlay);
    void generateData();
    void onEnter() override;
    void gameConfig();
    void loadFrame();
    void onGameStart();
    void onFinishGame() override;
    void onExitTransitionDidStart() override;
 
    void onClickedDelegate(Ref* sender) override;
    void onQuestionClicked(Ref* sender);
    void readAnswer(int count);
    Layout* root_layout;
    std::vector<Layout*> books;
    std::vector<Vec2> position;
    
    std::vector<CoverStory::answer> questions;
    
    std::vector<std::string> cover_paths;
    std::string cover_path ;
    
    std::vector<CoverStory::answers> game;
    std::vector<CoverStory_Item*> answer_btn;
    
    GAFWrapper* q_cloud_gaf;
    int answer_id = -1;
    int question_id = -1;
    float delay =0;

    int count_game = 0;
    int count_answer = 0;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Regular.ttf";
    int font_sz = 37;
    int number_books = 6;
    bool autoPlay = false;
};
CREATE_READER(CoverStory);

#endif /* CoverStory_hpp */
