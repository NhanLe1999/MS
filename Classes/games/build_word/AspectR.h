//
//  AspectR.hpp
//  InfiniteSpell
//
//  Created by Pijush Sarkar on 24/05/16.
//
//

#ifndef AspectR_h
#define AspectR_h

#include "cocos2d.h"
#define DESIGN_WIDTH 1024
#define DESIGN_HEIGHT 768

using namespace cocos2d;

class AspectR
{
public:
    static float getWidth(float width);
    static float getHeight(float height);
    static float getX(float x);
    static float getY(float y);
    static Vec2 getPos(Vec2 pos);
    static Vec2 getPos(float x, float y);
    static Size getSize(Size size);
    static Size getSize(float w, float h);
    static Size getScale(Size curSize, Size newSize);
	static float getMidX();
	static float getMidY();
	static Vec2 getMid();
	static float getFontSize(float val);
};

#endif /* AspectR_h */
