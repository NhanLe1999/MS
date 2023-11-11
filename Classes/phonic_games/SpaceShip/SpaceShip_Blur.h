//
//  MJ_SpaceShip_Blur.h
//

#ifndef SpaceShip_Blur_h
#define SpaceShip_Blur_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
class SpaceShip_BlurDelegate {
public:
};

class SpaceShip_Blur : public cocos2d::ui::Layout
{
public:
    
    enum BLUR_TYPE {
        PLANET_TYPE,
        STAR_TYPE,
        BG_TYPE
    };
    
    CREATE_FUNC(SpaceShip_Blur);
    static SpaceShip_Blur *createBlur(int blur_type);
    void setDelegate(SpaceShip_BlurDelegate *delegate);
    void resetBlur();
    void runBlur();
    void runBlurFast(const std::function<void()> func);
private:
    void didLoadFromCSB(int blur_type);
    void onEnter();
    SpaceShip_BlurDelegate *delegate = nullptr;
    void addBlurPlanet();
    void addBlurStar();
    void addBlurBackground();
    cocos2d::ui::ImageView *img_blur1, *img_blur2;
    int blur_type;
    void runBlur(cocos2d::ui::ImageView *blur_img, int width, float duration);
    int width_blur;
    
};
CREATE_READER(SpaceShip_Blur);
#endif /* MJ_SpaceShip_Blur_h */
