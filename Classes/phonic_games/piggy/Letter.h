//
//  Letter.hpp
//  Piggy-mobile
//
//  Created by Kiet Le on 1/5/18.
//

#ifndef Letter_hpp
#define Letter_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class Letter : public Label {
public:
    Letter();
    ~Letter();
    static Letter * createWithTTF(const std::string& text, const std::string& fontFilePath, float fontSize,
                                 const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
                                 TextVAlignment vAlignment = TextVAlignment::TOP);
    void initOption();
    bool isLetterIntersectRect(const Rect & rect);
    void addToBasket(Sprite * basket, const int & zOrder);
    void jumpOutBasket();
	void fitValueTextSize(cocos2d::Size box);
private:
    Vec2 _leftPoint;
    Vec2 _rightPoint;
};

#endif /* Letter_hpp */
