//
//  MJ_SpaceShip_Blur.h
//

#include "SpaceShip_Blur.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#define num_blur_bg 15
using namespace cocos2d;
SpaceShip_Blur * SpaceShip_Blur::createBlur(int blur_type) {
    SpaceShip_Blur * obj = new SpaceShip_Blur();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(blur_type);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void SpaceShip_Blur::didLoadFromCSB(int blur_type){
    this->blur_type = blur_type;
    switch (blur_type) {
        case PLANET_TYPE:
            this->addBlurPlanet();
            break;
        case STAR_TYPE:
            this->addBlurStar();
            break;
        case BG_TYPE:
            this->addBlurBackground();
            break;
        default:
            break;
    }
}

void SpaceShip_Blur::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void SpaceShip_Blur::addBlurPlanet(){
    auto img_path = std::string("games/spaceship/spaceship_planet.png");
    img_blur1 = cocos2d::ui::ImageView::create(img_path);
    img_blur1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(img_blur1);

    img_blur2 = cocos2d::ui::ImageView::create(img_path);
    img_blur2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(img_blur2);
    
    auto width = img_blur1->getContentSize().width*1.5;
    img_blur2->setPositionX(width);
    width_blur = width;
    
}
void SpaceShip_Blur::addBlurStar(){
    auto img_path = std::string("games/spaceship/spaceship_blur_star.png");
    img_blur1 = cocos2d::ui::ImageView::create(img_path);
    img_blur1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(img_blur1);
    
    img_blur2 = cocos2d::ui::ImageView::create(img_path);
    img_blur2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(img_blur2);
    
    auto width = img_blur1->getContentSize().width;
    img_blur2->setPositionX(width);
    width_blur = width;
}
void SpaceShip_Blur::addBlurBackground(){
    auto img_path = std::string("games/spaceship/spaceship_bg.png");
    for (int i = 0; i < num_blur_bg; i++) {
        auto img_blur = cocos2d::ui::ImageView::create(img_path);
        img_blur->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        this->addChild(img_blur);
        img_blur->setPositionX(img_blur->getContentSize().width);
        img_blur->setName(StringUtils::format("img_blur%d", i));
    }
}

void SpaceShip_Blur::runBlur(){
    if (blur_type != BG_TYPE) {
        auto width = width_blur;
        float speed;
        if (blur_type == PLANET_TYPE) {
            speed = 10.0f;
        }else if (blur_type == STAR_TYPE){
            speed = 1.0f;
        }else if (blur_type == BG_TYPE){
            speed = 5.0f;
        }
        auto duration = speed*1.0f/1024*width;
        img_blur1->runAction(Sequence::create(MoveTo::create(duration, Vec2(-width, 0)), CallFunc::create([=]{
            this->runBlur(img_blur1, width, duration);
        }), NULL));
        this->runBlur(img_blur2, width, duration);
    }else {
        
    }
}

void SpaceShip_Blur::runBlur(cocos2d::ui::ImageView *blur_img, int width, float duration){
    blur_img->setPositionX(width);
    blur_img->runAction(Sequence::create(MoveTo::create(2*duration, Vec2(-width, 0)), CallFunc::create([=]{
        this->runBlur(blur_img, width, duration);
    }), NULL));
}

void SpaceShip_Blur::runBlurFast(const std::function<void()> func){
    auto delay_time = 0.0f;
    auto duration = 0.25f;
    for (int i = 0 ; i < num_blur_bg; i ++) {
        auto img_blur = this->getChildByName(StringUtils::format("img_blur%d", i));
        
        auto width = img_blur->getContentSize().width;
        img_blur->runAction(Sequence::create(DelayTime::create(delay_time), MoveTo::create(duration*2, Vec2(-img_blur->getContentSize().width, 0)), CallFunc::create([=]{
            img_blur->setPositionX(width);
            if (i == (num_blur_bg - 3)) {
                if (func) {
                    func();
                }
                this->runAction(FadeOut::create(0.5f));
            }
        }), NULL));
        delay_time += duration;
    }
}

void SpaceShip_Blur::resetBlur(){
    if (blur_type == BG_TYPE) {
        this->setVisible(false);
        this->setOpacity(0);
    }
}


void SpaceShip_Blur::setDelegate(SpaceShip_BlurDelegate *delegate){
    this->delegate = delegate;
}
