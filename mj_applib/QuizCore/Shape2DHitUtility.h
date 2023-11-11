//
//  Shape2DHitUtility.h
//  MJQuiz
//
//  Created by HuyPhuong on 4/1/17.
//
//

#ifndef Shape2DHitUtility_hpp
#define Shape2DHitUtility_hpp

#include <stdio.h>
#include "cocos2d.h"
struct VertexPoint {
    cocos2d::Vec2   Point;
    VertexPoint     *Next;
    VertexPoint     *Prev;
};

class Shape2DHitUtility
{
public:
    static bool tapsOnTriangle2D(cocos2d::Vec2 pointA, cocos2d::Vec2 pointB, cocos2d::Vec2 pointC, cocos2d::Vec2 check_point);
    static bool tapsOnCircle2D(cocos2d::Vec2 center, float radius, cocos2d::Vec2 check_point);
    static bool tapsOnElipse2D(cocos2d::Vec2 center, float a, float b, cocos2d::Vec2 check_point);
    static bool tapsOnPolygon2D(cocos2d::Vec2 point, std::vector<cocos2d::Vec2> verts);
    static bool tapsOnPolygon2D(cocos2d::Vec2 point, VertexPoint *vp);
    static bool tapsOnNonTransparent(const cocos2d::Vec2& tap , cocos2d::Image * imgPtr, bool clean = false);
    
    static bool isSameSide(cocos2d::Vec2 pointA, cocos2d::Vec2 pointB, cocos2d::Vec2 check_1, cocos2d::Vec2 check_2);
    static bool isZeroPixel(unsigned char *pixel);
    static int PointInPolygon (const cocos2d::Vec2 &pt, VertexPoint *vp);
    static VertexPoint* convertToVertexPoint(std::vector<cocos2d::Vec2> verts);
    
    
    /**
     *  @brief Kiểm tra xem điểm chạm có nằm trong vùng non-trans của ảnh không.
     *  @return true nếu điểm chạm thuộc pixel non-trans (zero)
     */
    static bool touchOnImage(std::string img_path, cocos2d::Size img_size, cocos2d::Vec2 location);


};

#endif /* Shape2DHitUtility_hpp */
