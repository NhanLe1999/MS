//
//  CoverStoryActivity.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 9/6/18.
//

#ifndef CoverStoryActivity_hpp
#define CoverStoryActivity_hpp

#include "MJBaseActivity.h"
#include "../activity/GameList.h"


//#include "cocostudiomacro2.h"

class ActivityLayout : public ms::BaseActivity, public MJMainGBaseDelegate {
public:
    CREATE_FUNC(ActivityLayout);
    static ActivityLayout * createGame(int game_id, std::vector<math::resource::Image*> images);
    static ActivityLayout * createGame(int game_id, string game_path);
    
    void onEnter() override;
private:
    void addGame(int game_id, std::vector<math::resource::Image*> images);
    void addGame (int game_id, string game_path);
    void getGameStatus(string sender) override;
//    void commonEndGame() ;
    void generateData(std::function<void()> callback);
    void onCloseGame() override;
};


#endif /* CoverStoryActivity_hpp */
