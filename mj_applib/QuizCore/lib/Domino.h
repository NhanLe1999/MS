//
//  Domino.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#ifndef Domino_hpp
#define Domino_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Domino : public cocos2d::ui::Layout {
public:
    Domino(){};
    ~Domino(){};

    static Domino* createDomino();
    void setDomino(int , int, int , bool);
    
    int first;
    int second;
    
private:
    bool init() override;
    cocos2d::DrawNode* drawer;
    cocos2d::ui::ImageView* _background;
    
    void drawDots(int, int, cocos2d::Vec2);
    void drawDot(cocos2d::Vec2, float, cocos2d::Color4F);
    
    std::vector<cocos2d::ui::ImageView*> dots_list;
    
};
#endif /* Domino_hpp */

