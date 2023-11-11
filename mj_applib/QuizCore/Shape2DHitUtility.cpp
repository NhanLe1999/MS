//
//  Shape2DHitUtility.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 4/1/17.
//
//

#include "Shape2DHitUtility.h"

#include <algorithm>
#include <functional>
#include <math.h>
#include "math/CCMathBase.h"

#pragma mark - check tap in Triangle
bool Shape2DHitUtility::isSameSide(cocos2d::Vec2 pointA, cocos2d::Vec2 pointB, cocos2d::Vec2 check_1, cocos2d::Vec2 check_2)
{
    float cp_1 = (pointA-pointB).cross(check_1-pointA);
    float cp_2 = (pointA-pointB).cross(check_2-pointA);
    
    return (cp_1*cp_2)>=0;
}

bool Shape2DHitUtility::tapsOnTriangle2D(cocos2d::Vec2 pointA, cocos2d::Vec2 pointB, cocos2d::Vec2 pointC, cocos2d::Vec2 check_point)
{
    bool check_C=Shape2DHitUtility::isSameSide(pointA, pointB, pointC, check_point);
    bool check_A=Shape2DHitUtility::isSameSide(pointB, pointC, pointA, check_point);
    bool check_B=Shape2DHitUtility::isSameSide(pointC, pointA, pointB, check_point);
    
    return check_A && check_B && check_C;
}

#pragma mark - check tap in Circle
bool Shape2DHitUtility::tapsOnCircle2D(cocos2d::Vec2 center, float radius, cocos2d::Vec2 check_point)
{
    return center.distance(check_point)<=radius;
}

bool Shape2DHitUtility::tapsOnElipse2D(cocos2d::Vec2 center, float a, float b, cocos2d::Vec2 check_point)
{
    cocos2d::Vec2 c_point=check_point-center;
    float d = (float)pow(c_point.x, 2)/(float)pow(a, 2) + (float)pow(c_point.y, 2)/(float)pow(b, 2);
    return d<=1;
}


#pragma mark - check tap on non-transparent
bool Shape2DHitUtility::isZeroPixel(unsigned char *pixel) {
    return 0 == pixel[0] && 0 == pixel[1] && 0 == pixel[2] && 0 == pixel[3];
}

bool Shape2DHitUtility::tapsOnNonTransparent(const cocos2d::Vec2& tap , cocos2d::Image * imgPtr, bool clean) {
    const int width = imgPtr ->getWidth();
    const int height = imgPtr ->getHeight();
    unsigned x = unsigned( tap.x ) % width;
    unsigned y = unsigned( height - tap.y ) % height;
    unsigned index = x + y * width;
    unsigned int dataLen = static_cast<unsigned int>( imgPtr ->getDataLen() );
    
    CCAssert( index < dataLen, "index is bigger than image size." );
    unsigned char* pixel = imgPtr ->getData() + (4 * index);
    bool res = !Shape2DHitUtility::isZeroPixel( pixel );
    if (clean) {
        delete imgPtr;
    }
    return res;
}


#pragma mark - check tap on polygon
bool Shape2DHitUtility::tapsOnPolygon2D(cocos2d::Vec2 point, std::vector<cocos2d::Vec2> verts)
{
    VertexPoint* vp=Shape2DHitUtility::convertToVertexPoint(verts);
    int result=Shape2DHitUtility::PointInPolygon(point, vp);
    
    return result!=0;
    
}

bool Shape2DHitUtility::tapsOnPolygon2D(cocos2d::Vec2 point, VertexPoint* verts)
{
    int result=Shape2DHitUtility::PointInPolygon(point, verts);
    
    return result!=0;
    
}

int Shape2DHitUtility::PointInPolygon (const cocos2d::Vec2 &pt, VertexPoint *vertex_point)
{
    //returns 0 if false, +1 if true, -1 if pt ON polygon boundary
    int result=0;
    VertexPoint* startVp = vertex_point;
    
    for(;;) {
        if (vertex_point->Next->Point.y==pt.y) {
            if ((vertex_point->Next->Point.x==pt.x) ||
                (vertex_point->Point.y==pt.y&&((vertex_point->Next->Point.x > pt.x)==(vertex_point->Point.x < pt.x)))) {
                return -1;
            }
        }
        
        if ((vertex_point->Point.y<pt.y)!=(vertex_point->Next->Point.y<pt.y)) {
            if (vertex_point->Point.x>=pt.x) {
                
                if (vertex_point->Next->Point.x>pt.x) {
                    result=1-result;
                } else {
                    double d=(double)(vertex_point->Point.x-pt.x)*(vertex_point->Next->Point.y-pt.y)-
                    (double)(vertex_point->Next->Point.x-pt.x)-(vertex_point->Point.y-pt.y);
                    
                    if (!d) {
                        return -1;
                    }
                    
                    if ((d>0)==(vertex_point->Next->Point.y>vertex_point->Point.y)) {
                        result=1-result;
                    }
                }
            } else {
                if (vertex_point->Next->Point.x>pt.x) {
                    double d=(double)(vertex_point->Point.x-pt.x)*(vertex_point->Next->Point.y-pt.y)-
                    (double)(vertex_point->Next->Point.x-pt.x)*(vertex_point->Point.y-pt.y);
                    
                    if (!d) {
                        return -1;
                    }
                    
                    if ((d>0)==(vertex_point->Next->Point.y>vertex_point->Point.y)) {
                         result=1-result;
                    }
                }
            }
        }
        
        vertex_point=vertex_point->Next;
        if (startVp==vertex_point) {
            break;
        }
    }
    
    return result;
}

VertexPoint* Shape2DHitUtility::convertToVertexPoint(std::vector<cocos2d::Vec2> verts)
{
    VertexPoint* start_vp=new VertexPoint();
    start_vp->Point=verts[0];
    
    VertexPoint* prev_vp=start_vp;
    
    for (int i=1; i<verts.size(); i++) {
        VertexPoint* vp=new VertexPoint();
        vp->Point=verts[i];
        vp->Prev=prev_vp;
        prev_vp->Next=vp;

        if (i==verts.size()-1) {
            vp->Next=start_vp;
            start_vp->Prev=vp;
        } else {
            prev_vp=vp;
        }
    }
    
    return start_vp;
}

#pragma mark - add code ver 2.0
bool Shape2DHitUtility::touchOnImage(std::string img_path, cocos2d::Size img_size, cocos2d::Vec2 location) {
    cocos2d::Image* image = new cocos2d::Image();
    image->initWithImageFile(img_path);
    
    auto img_width = image->getWidth();
    auto img_height = image->getHeight();
    
    //Convert location
    float c_location_x = location.x * img_width / img_size.width;
    float c_location_y = location.y * img_height / img_size.height;
    
    cocos2d::Vec2 c_location = cocos2d::Vec2(c_location_x, c_location_y);
    return tapsOnNonTransparent(c_location, image, true);
}


