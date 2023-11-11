//
//  Math_Func.h
//  MJStory
//
//  Created by HuyPhuong on 3/27/18.
//

#ifndef Math_Func_h
#define Math_Func_h

#include <stdio.h>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>

#include "Math_Libs.h"

namespace math {
#pragma mark - function
    namespace func {
		// trim from start (in place)
		static inline void ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
			}));
		}

		// trim from end (in place)
		static inline void rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
			}).base(), s.end());
		}

        // trim from both ends
        static inline std::string &trim(std::string &s) {
			ltrim(s);
			rtrim(s);
			return s;
		}
        
        static inline std::string upperFirstLetter(std::string s) {
            s[0] = std::toupper(s[0]);
            return s;
        }
        
        static inline std::string upperString(std::string s) {
            for (int i=0; i<s.length(); i++) {
                s[i] = std::toupper(s[i]);
            }
            return s;
        }
        
        static inline std::string lowerString(std::string s) {
            for (int i=0; i<s.length(); i++) {
                s[i] = std::tolower(s[i]);
            }
            return s;
        }
        
        template<typename T>
        void shuffle(std::vector<T> &v) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(v.begin(), v.end(), g);
        }
        
        template<typename T>
        T getRandomNode(std::vector<T> list) {
            int index=cocos2d::random(0, (int)list.size()-1);
            return list[index];
        }

        //Set position List
        enum ListVAlign {
            middle = 0,
            bottom,
            top
        };
        enum ListHAlign {
            center = 0,
            left,
            right
        };
        template<typename T>
        void setPositions (std::vector<T> list, cocos2d::Rect rspace, ListVAlign valign = math::func::middle, ListHAlign halign = math::func::center) {
            struct ItemLine {
                int index;
                cocos2d::Size size;
                cocos2d::Vec2 origin;
                std::vector<cocos2d::Node*> items;
                
                float getLineWidth(float pad) {
                    float w=0;
                    for (cocos2d::Node* node : items) {
                        auto snode = node->getContentSize();
                        w += (snode.width + pad);
                    }
                    return w-pad;
                }
                
                float getLineHeight() {
                    float h=0;
                    for (cocos2d::Node* node : items) {
                        auto snode = node->getContentSize();
                        if (h < snode.height) {
                            h = snode.height;
                        }
                    }
                    return h;
                }
            };
            
            float min_pad = 20;
            auto space = rspace.size;
            
            int number_item = (int) list.size();
            std::vector<ItemLine*> ilines;
            int iline = 0;
            
            float wline = min_pad;
            float hline = 0;
            std::vector<cocos2d::Node*> items;
            
            for (int i=0; i<number_item; i++) {
                auto node = static_cast<cocos2d::Node*>(list[i]);
                auto snode = node->getContentSize();
                if (wline + snode.width + min_pad < space.width) {
                    items.push_back(node);
                    wline += snode.width + min_pad;
                    if (snode.height > hline) {
                        hline = snode.height;
                    }
                } else {
                    ItemLine* item_line = new ItemLine();
                    item_line->index = iline;
                    item_line->items = items;
                    item_line->size = cocos2d::Size(wline, hline);
                    ilines.push_back(item_line);
                    
                    items.clear();
                    items.push_back(node);
                    iline ++;
                    wline = min_pad * 2 + snode.width;
                    hline = snode.height;
                }
            }
            if (!items.empty()) {
                ItemLine* item_line = new ItemLine();
                item_line->index = iline;
                item_line->items = items;
                item_line->size = cocos2d::Size(wline, hline);
                ilines.push_back(item_line);
            }
            
            //TODO: San object giữa các lines cho deu nhau.
            if (ilines.size() == 2) {
                while (true) {
                    float fline = ilines.front()->getLineWidth(min_pad);
                    float bline = ilines.back()->getLineWidth(min_pad);
                    auto last = ilines.front()->items.back();
                    auto wlast = last->getContentSize().width + min_pad;
                    if (fline - bline - 2* wlast >= 0) {
                        ilines.front()->items.pop_back();
                        ilines.back()->items.insert(ilines.back()->items.begin(), last);
                    } else {
                        break;
                    }
                }
            } else if (ilines.size() > 2) {
                float totalw = 0;
                for (ItemLine* il : ilines) {
                    totalw += il->getLineWidth(min_pad);
                }
                float average = totalw / (int)ilines.size();
                for (int i=0; i<ilines.size()-1; i++) {
                    auto iline = ilines[i];
                    auto nline = ilines[i+1];
                    auto last = iline->items.back();
                    while (iline->getLineWidth(min_pad) - last->getContentSize().width - min_pad >= average) {
                        iline->items.pop_back();
                        nline->items.insert(nline->items.begin(), last);
                        last = iline->items.back();
                    }
                }
            }
            
            //Tinh toan tran khung
            float hlines = min_pad;
            for (ItemLine* il : ilines) {
                hlines += (il->getLineHeight() + min_pad);
            }
            
            if (hlines > space.height) {
                CCLOG("math::func::setPositions -- OVER FRAME");
                //decreament size item and run func again
                std::vector<cocos2d::Node*> flist;
                for (int i=0; i<number_item; i++) {
                    auto node = static_cast<cocos2d::Node*>(list[i]);
                    node->setContentSize(node->getContentSize()*0.95);
                    flist.push_back(node);
                }
                math::func::setPositions (flist, rspace, valign, halign);
            } else {
                //1: set origin all lines
                hlines = 0;
                float wpad = 125;
                //1.1: find padings
                for (ItemLine* il : ilines) {
                    float w = il->getLineWidth(0);
                    float h = il->getLineHeight();
                    il->size = cocos2d::Size(w, h);
                    hlines += h;
                    
                    float pad = (space.width - w) / ((int)il->items.size() + 1);
                    if (pad < wpad) {
                        wpad = pad;
                    }
                }
                float hpad = (space.height - hlines) / ((int)ilines.size() + 1);
                if (hpad > 100) {
                    hpad = 100;
                }
                //1.2: fix size of line with pad
                float max_wlines = 0;
                for (ItemLine* il : ilines) {
                    il->size = cocos2d::Size(il->size.width + wpad * (int)(il->items.size() - 1), il->size.height);
                    if (max_wlines < il->size.width) {
                        max_wlines = il->size.width;
                    }
                }
                //1.3: set origins
                float origin_y = space.height - ((space.height - hlines - ((int)ilines.size() + 1)*hpad) / 2);
                for (ItemLine* il : ilines) {
                    float origin_x = 0;
                    switch (halign) {
                        case math::func::center :
                        {
                            origin_x = (space.width - il->size.width) / 2;
                        }
                            break;
                        case math::func::left :
                        {
                            origin_x = (space.width - max_wlines) / 2;
                        }
                            break;
                        default:
                        {
                            origin_x = (space.width - max_wlines) / 2 + (max_wlines - il->size.width);
                        }
                            break;
                    }
                    
                    origin_y -= (hpad + il->size.height);
                    il->origin = cocos2d::Vec2(origin_x + rspace.origin.x, origin_y + rspace.origin.y);
                }
                
                //TODO: set position item valign
                switch (valign) {
                    case math::func::middle :
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y + il->size.height/2;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                    case math::func::bottom :
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y + node->getContentSize().height/2));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                    default:
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y + il->size.height;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y - node->getContentSize().height/2));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                }
            }
        }

        template<typename T>
        void setSmartPositions (std::vector<T> list, cocos2d::Rect rspace, ListVAlign valign = math::func::middle, ListHAlign halign = math::func::center) {
            struct ItemLine {
                int index;
                cocos2d::Size size;
                cocos2d::Vec2 origin;
                std::vector<cocos2d::Node*> items;

                float getLineWidth(float pad) {
                    float w=0;
                    for (cocos2d::Node* node : items) {
                        auto snode = node->getContentSize();
                        w += (snode.width + pad);
                    }
                    return w-pad;
                }

                float getLineHeight() {
                    float h=0;
                    for (cocos2d::Node* node : items) {
                        auto snode = node->getContentSize();
                        if (h < snode.height) {
                            h = snode.height;
                        }
                    }
                    return h;
                }
            };

            float min_pad = 20;

            int number_item = (int) list.size();

            const float K_PADDING_MAX = 125;

            float widthOfObjects = 0;

            for (int i=0; i<number_item; i++)
            {
                auto node = static_cast<cocos2d::Node*>(list[i]);
                widthOfObjects += node->getContentSize().width;
            }

            const float k_spaceBTObject = (rspace.size.width - widthOfObjects ) / (number_item + 1);

            if(k_spaceBTObject < K_PADDING_MAX)
            {
                min_pad = std::max(min_pad,k_spaceBTObject);
            }

            auto space = rspace.size;

            std::vector<ItemLine*> ilines;
            int iline = 0;

            float wline = min_pad;
            float hline = 0;
            std::vector<cocos2d::Node*> items;
            auto sizeXX = cocos2d::Director::getInstance()->getVisibleSize();
            for (int i=0; i<number_item; i++) {
                auto node = static_cast<cocos2d::Node*>(list[i]);
                auto snode = node->getContentSize();
                if (wline + snode.width + min_pad < space.width) {
                    items.push_back(node);
                    wline += snode.width + min_pad;
                    if (snode.height > hline) {
                        hline = snode.height;
                    }
                } else {
                    ItemLine* item_line = new ItemLine();
                    item_line->index = iline;
                    item_line->items = items;
                    item_line->size = cocos2d::Size(wline, hline);
                    ilines.push_back(item_line);

                    items.clear();
                    items.push_back(node);
                    iline ++;
                    wline = min_pad * 2 + snode.width;
                    hline = snode.height;
                }
            }

            min_pad = 20;

            if (!items.empty()) {
                ItemLine* item_line = new ItemLine();
                item_line->index = iline;
                item_line->items = items;
                item_line->size = cocos2d::Size(wline, hline);
                ilines.push_back(item_line);
            }

            //TODO: San object giữa các lines cho deu nhau.
            if (ilines.size() == 2) {
                while (true) {
                    float fline = ilines.front()->getLineWidth(min_pad);
                    float bline = ilines.back()->getLineWidth(min_pad);
                    auto last = ilines.front()->items.back();
                    auto wlast = last->getContentSize().width + min_pad;
                    if (fline - bline - 2* wlast >= 0) {
                        ilines.front()->items.pop_back();
                        ilines.back()->items.insert(ilines.back()->items.begin(), last);
                    } else {
                        break;
                    }
                }
            } else if (ilines.size() > 2) {
                float totalw = 0;
                for (ItemLine* il : ilines) {
                    totalw += il->getLineWidth(min_pad);
                }
                float average = totalw / (int)ilines.size();
                for (int i=0; i<ilines.size()-1; i++) {
                    auto iline = ilines[i];
                    auto nline = ilines[i+1];
                    auto last = iline->items.back();
                    while (iline->getLineWidth(min_pad) - last->getContentSize().width - min_pad >= average) {
                        iline->items.pop_back();
                        nline->items.insert(nline->items.begin(), last);
                        last = iline->items.back();
                    }
                }
            }

            //Tinh toan tran khung
            float hlines = min_pad;
            for (ItemLine* il : ilines) {
                hlines += (il->getLineHeight() + min_pad);
            }

            if (hlines > space.height) {
                CCLOG("math::func::setPositions -- OVER FRAME");
                //decreament size item and run func again
                std::vector<cocos2d::Node*> flist;
                for (int i=0; i<number_item; i++) {
                    auto node = static_cast<cocos2d::Node*>(list[i]);
                    node->setContentSize(node->getContentSize()*0.95);
                    flist.push_back(node);
                }
                math::func::setPositions (flist, rspace, valign, halign);
            } else {
                //1: set origin all lines
                hlines = 0;
                float wpad = 125;
                //1.1: find padings
                for (ItemLine* il : ilines) {
                    float w = il->getLineWidth(0);
                    float h = il->getLineHeight();
                    il->size = cocos2d::Size(w, h);
                    hlines += h;

                    float pad = (space.width - w ) / ((int)il->items.size() + 1);
                    if (pad < wpad) {
                        wpad = pad;
                    }
                }
                float hpad = (space.height - hlines) / ((int)ilines.size() + 1);
                if (hpad > 100) {
                    hpad = 100;
                }
                //1.2: fix size of line with pad
                float max_wlines = 0;
                for (ItemLine* il : ilines) {
                    il->size = cocos2d::Size(il->size.width + wpad * (int)(il->items.size() - 1), il->size.height);
                    if (max_wlines < il->size.width) {
                        max_wlines = il->size.width;
                    }
                }
                //1.3: set origins
                float origin_y = space.height - ((space.height - hlines - ((int)ilines.size() + 1)*hpad) / 2);
                for (ItemLine* il : ilines) {
                    float origin_x = 0;
                    switch (halign) {
                        case math::func::center :
                        {
                            origin_x = (space.width - il->size.width) / 2;
                        }
                            break;
                        case math::func::left :
                        {
                            origin_x = (space.width - max_wlines) / 2;
                        }
                            break;
                        default:
                        {
                            origin_x = (space.width - max_wlines) / 2 + (max_wlines - il->size.width);
                        }
                            break;
                    }

                    origin_y -= (hpad + il->size.height);
                    il->origin = cocos2d::Vec2(origin_x + rspace.origin.x, origin_y + rspace.origin.y);
                }

                //TODO: set position item valign
                switch (valign) {
                    case math::func::middle :
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y + il->size.height/2;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                    case math::func::bottom :
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y + node->getContentSize().height/2));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                    default:
                    {
                        for (ItemLine* il : ilines) {
                            float point_x = il->origin.x;
                            float point_y = il->origin.y + il->size.height;
                            for (cocos2d::Node* node : il->items) {
                                node->setPosition(cocos2d::Vec2(point_x + node->getContentSize().width/2, point_y - node->getContentSize().height/2));
                                point_x += (node->getContentSize().width + wpad);
                            }
                        }
                    }
                        break;
                }
            }
        }

        template<typename T>
        void setPositionsRandom (std::vector<T> list, cocos2d::Rect rspace) {
            int number_item = (int)list.size();
            auto center = cocos2d::Vec2(rspace.origin.x + rspace.size.width/2, rspace.origin.y + rspace.size.height/2);
            
            auto size = static_cast<cocos2d::Node*>(list.front())->getContentSize();
            float dmax =  size.width * 1.7;
            float smin = rspace.size.width > rspace.size.height ? rspace.size.height : rspace.size.width;
            if (dmax + size.width > smin) {
                dmax = smin - size.width;
            }
            float dmin = dmax * 0.8;
            std::vector<cocos2d::Vec2> points;
            
            if (number_item == 1) {
                auto node = static_cast<cocos2d::Node*>(list.front());
                node->setPosition(center);
            } else if (number_item == 2) {
                points.push_back(cocos2d::Vec2::ZERO);
                int angle = cocos2d::random(1, 360);
                float distance = cocos2d::random(dmin, dmax);
                points.push_back(cocos2d::Vec2(distance * cos(angle), distance * sin(angle)));
                
                auto delta = center - (points.back()/2);
                for (int i=0; i<number_item; i++) {
                    auto node = static_cast<cocos2d::Node*>(list[i]);
                    node->setPosition(points[i]+delta);
                }
            } else if (number_item == 3) {
                points.push_back(cocos2d::Vec2::ZERO);
                int angle = cocos2d::random(1, 360);
                float distance = cocos2d::random(dmin, dmax);
                points.push_back(cocos2d::Vec2(distance * cos(angle), distance * sin(angle)));

                while (true) {
                    cocos2d::Vec2 point = cocos2d::Vec2(cocos2d::random<float>(-dmax, dmax), cocos2d::random<float>(-dmax, dmax));
                    auto distance1 = point.distance(points.front());
                    auto distance2 = point.distance(points[1]);
                    if (distance1 >= dmin && distance1 <= dmax && distance2 >= dmin && distance2 <= dmax) {
                        points.push_back(point);
                        break;
                    }
                }
                auto delta = center - ((points[2] + points[1])/3);
                for (int i=0; i<number_item; i++) {
                    auto node = static_cast<cocos2d::Node*>(list[i]);
                    node->setPosition(points[i]+delta);
                }
            } else if (number_item == 4) {
                if (cocos2d::random(0, 1) == 0) {
                    for (int i=0; i<number_item; i++) {
                        auto node = static_cast<cocos2d::Node*>(list[i]);
                        node->setPosition(cocos2d::Vec2(center.x + size.width * (i%2 * 2 - 1), center.y + size.height * 0.65 * (i/2 * 2 - 1)));
                    }
                } else {
                    for (int i=0; i<number_item; i++) {
                        auto node = static_cast<cocos2d::Node*>(list[i]);
                        if (i<2) {
                            node->setPosition(cocos2d::Vec2(center.x + size.width * 1.2 * (i%2 * 2 - 1), center.y));
                        } else {
                            node->setPosition(cocos2d::Vec2(center.x, center.y + size.height * 0.65 * (i%2 * 2 - 1)));
                        }
                    }
                }
            } else if (number_item == 5) {
                static_cast<cocos2d::Node*>(list.back())->setPosition(center);
                if (cocos2d::random(0, 1) == 0) {
                    for (int i=0; i<number_item-1; i++) {
                        auto node = static_cast<cocos2d::Node*>(list[i]);
                        node->setPosition(cocos2d::Vec2(center.x + size.width * 1.35 * (i%2 * 2 - 1), center.y + size.height * 0.65 * (i/2 * 2 - 1)));
                    }
                } else {
                    for (int i=0; i<number_item-1; i++) {
                        auto node = static_cast<cocos2d::Node*>(list[i]);
                        if (i<2) {
                            node->setPosition(cocos2d::Vec2(center.x + size.width * 1.35 * (i%2 * 2 - 1), center.y));
                        } else {
                            node->setPosition(cocos2d::Vec2(center.x, center.y + size.height * 1.1 * (i%2 * 2 - 1)));
                        }
                    }
                }
            } else {
                int count_rand = 0;
                float delta = 1.5;
                for (int i=0; i<number_item; i++) {
                    if (count_rand>=10) {
                        count_rand=0;
                        delta-=0.1;
                    }
                    cocos2d::Vec2 point;
                    bool avail=true;
                    int count=0;
                    do {
                        count++;
                        if (count>50) {
                            break;
                        }
                        
                        avail=true;
                        int rand_x = cocos2d::random(0, (int)(rspace.size.width-size.width*1.2)) + (int)size.width*0.6;
                        int rand_y = cocos2d::random(0, (int)(rspace.size.height-size.height*1.2)) + (int)size.height*0.6;
                        point=cocos2d::Vec2(rand_x, rand_y);
                        
                        for (cocos2d::Vec2 _point : points) {
                            if (point.distance(_point)< size.width * delta) {
                                avail=false;
                                break;
                            }
                        }
                    } while (!avail);
                    
                    if (count>50) {
                        i=-1;
                        points.clear();
                        count_rand++;
                        continue;
                    } else {
                        points.push_back(point);
                    }
                }
                
                cocos2d::Vec2 delta_point = cocos2d::Vec2::ZERO;
                for (cocos2d::Vec2 point : points) {
                    delta_point = delta_point + point;
                }
                delta_point = delta_point/number_item;
                delta_point = center - delta_point;
                for (int i=0; i<number_item; i++) {
                    auto node = static_cast<cocos2d::Node*>(list[i]);
                    node->setPosition(points[i]+delta_point);
                }
            }
        }
        
        template<typename T>
        void setPositionList(std::vector<T> list, int max_line, cocos2d::Rect space, bool animated=false, int align = 0, cocos2d::Size game_space = quiz_size) {
            int total_node = (int)list.size();
            int remain_node = total_node % max_line;
            int total_line = total_node / max_line;
            if (remain_node != 0) {
                total_line++;
            }
            
            //fix space
            if (space.size.width > 2) {
                space = cocos2d::Rect(space.origin.x / game_space.width, space.origin.y / game_space.height, space.size.width / game_space.width, space.size.height / game_space.height);
            }
            
            float node_width = space.size.width * game_space.width / max_line;
            float node_height = space.size.height * game_space.height / total_line;
            float start_x = node_width / 2 + space.origin.x * game_space.width;
            float start_y = space.size.height * game_space.height - node_height / 2 + space.origin.y * game_space.height;
            
            float max_height = 0;
            float max_width = 0;
            
            if (align == 1) {
                for (int i=0; i<list.size(); i++) {
                    cocos2d::Node* node = static_cast<cocos2d::Node*>(list[i]);
                    auto height = node->getContentSize().height;
                    if (height > max_height) {
                        max_height = height;
                    }
                }
            } else if (align == 2) {
                for (int i=0; i<list.size(); i++) {
                    cocos2d::Node* node = static_cast<cocos2d::Node*>(list[i]);
                    auto width = node->getContentSize().width;
                    if (width > max_width) {
                        max_width = width;
                    }
                }
            }
            
            if (total_line==1) {
                if (remain_node!=0) {
                    start_x=(max_line-remain_node)*(node_width/2)+start_x;
                }
                for (int i=0; i<list.size(); i++) {
                    cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                    float fix_y = 0;
                    float fix_x = 0;
                    if (align == 1) {
                        auto height = node->getContentSize().height;
                        fix_y = (height - max_height)/2;
                    } else if (align == 2) {
                        auto width = node->getContentSize().width;
                        fix_x = (width - max_width)/2;
                    }
                    cocos2d::Vec2 point=cocos2d::Vec2(start_x+node_width*i + fix_x, start_y + fix_y);
                    if (animated) {
                        node->runAction(cocos2d::MoveTo::create(0.25, point));
                    } else {
                        node->setPosition(point);
                    }
                }
            } else {
                float remain_start_x=start_x;
                if (remain_node!=0) {
                    remain_start_x=(max_line-remain_node)*(node_width/2)+start_x;
                }
                for (int i=0; i<list.size(); i++) {
                    cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                    float fix_y = 0;
                    float fix_x = 0;
                    if (align == 1) {
                        auto height = node->getContentSize().height;
                        fix_y = (height - max_height) / 2;
                    } else if (align == 2) {
                        auto width = node->getContentSize().width;
                        fix_x = (width - max_width)/2;
                    }
                    cocos2d::Vec2 point;
                    if (i<max_line*(total_line-1)) {
                        point=cocos2d::Vec2(start_x+node_width*(i%max_line) + fix_x, start_y-(i/max_line)*node_height + fix_y);
                    } else {
                        point=cocos2d::Vec2(remain_start_x+node_width*(i%max_line) + fix_x, start_y-(i/max_line)*node_height + fix_y);
                    }
                    if (animated) {
                        node->runAction(cocos2d::MoveTo::create(0.25, point));
                    } else {
                        node->setPosition(point);
                    }
                }
            }
        }
        
        static inline std::vector<cocos2d::Vec2> getLocations(int total_node, int max_line, cocos2d::Rect space, cocos2d::Size game_space, cocos2d::Size change = cocos2d::Size(0, 0)) {
            std::vector<cocos2d::Vec2> locations;
            
            int remain_node=total_node%max_line;
            int total_line=total_node/max_line;
            if (remain_node!=0) {
                total_line++;
            }
            
            //fix space
            if (space.size.width > 2) {
                space = cocos2d::Rect(space.origin.x/game_space.width, space.origin.y/game_space.height, space.size.width/game_space.width, space.size.height/game_space.height);
            }
            
            float node_width = space.size.width * game_space.width / max_line;
            float node_height = space.size.height * game_space.height / total_line;
            float start_x = node_width / 2 + space.origin.x * game_space.width;;
            float start_y = space.size.height * game_space.height - node_height / 2 + space.origin.y * game_space.height;
            
            if (total_line==1) {
                if (remain_node!=0) {
                    start_x=(max_line-remain_node)*(node_width/2)+start_x;
                }
                for (int i=0; i<total_node; i++) {
                    float fix_y = 0;
                    float fix_x = 0;
                    cocos2d::Vec2 point=cocos2d::Vec2(start_x+node_width*i + fix_x, start_y + fix_y);
                    locations.push_back(point);
                }
            } else {
                float remain_start_x=start_x;
                if (remain_node!=0) {
                    remain_start_x=(max_line-remain_node)*(node_width/2)+start_x;
                }
                for (int i=0; i<total_node; i++) {
                    float fix_y = 0;
                    float fix_x = 0;
                    cocos2d::Vec2 point;
                    if (i<max_line*(total_line-1)) {
                        point=cocos2d::Vec2(start_x+node_width*(i%max_line) + fix_x, start_y-(i/max_line)*node_height + fix_y);
                    } else {
                        point=cocos2d::Vec2(remain_start_x+node_width*(i%max_line) + fix_x, start_y-(i/max_line)*node_height + fix_y);
                    }
                    locations.push_back(point);
                }
            }
            
            std::vector<cocos2d::Vec2> results;
            for (cocos2d::Vec2 point : locations) {
                cocos2d::Vec2 point2 = cocos2d::Vec2(point.x + cocos2d::random(-change.width, change.width), point.y + cocos2d::random(-change.height, change.height));
                results.push_back(point2);
            }
            return results;
        }
        
        template<typename T>
        static inline int smartSetPosition(std::vector<T> list, cocos2d::Size item_size, cocos2d::Rect space, float padding = 0, bool align = true) {
            int total_node = (int)list.size();
            cocos2d::Vec2 center = cocos2d::Vec2(space.origin.x + space.size.width / 2, space.origin.y + space.size.height / 2);
            if (padding < 10) {
                padding = 10;
                //padding = item_size.width > item_size.height? item_size.height : item_size.width;
            }
            
            int number_in_line = (space.size.width - padding) / (item_size.width + padding);
            if (!align) {
                number_in_line = (space.size.height - padding) / (item_size.height + padding);
            }
            number_in_line = number_in_line > total_node ? total_node : number_in_line;
            int first_line = number_in_line;
            int last_line = total_node % number_in_line;
            int number_line = (total_node / number_in_line) + (last_line == 0 ? 0 : 1);
            
            if (last_line == 0) {
                last_line = number_in_line;
            } else {
                if (number_line == 2) {
                    first_line = (first_line + last_line) / 2;
                    last_line = total_node - first_line;
                } else if (number_line == 3) {
                    int total = number_in_line + last_line;
                    if (last_line < number_in_line - 1) {
                        if (total % 2 == 0) {
                            first_line = total / 2;
                            last_line = first_line;
                        } else {
                            int total = number_in_line + last_line + 1;
                            number_in_line--;
                            first_line = (total + 1) / 2;
                            last_line = total - first_line;
                        }
                    }
                } else if (number_line > 3) {
                    first_line = (number_in_line + last_line + 1) / 2;
                    last_line = number_in_line + last_line - first_line;
                }
            }
            
            if (align) {
                if (number_line == 1) {
                    float start_x = center.x - (number_in_line - 1) * (item_size.width + padding) / 2;
                    for (int i=0; i < total_node; i++) {
                        cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                        node->setPosition(cocos2d::Vec2(start_x + i * (item_size.width + padding), center.y));
                    }
                } else {
                    float start_y = center.y + (number_line - 1) * (item_size.height + padding) / 2;
                    float start_x = center.x - (number_in_line - 1) * (item_size.width + padding) / 2;
                    float start_1st = center.x - (first_line - 1) * (item_size.width + padding) / 2;
                    float start_last = center.x - (last_line - 1) * (item_size.width + padding) / 2;
                    int count = 0;
                    int line = 0;
                    for (int i=0; i < total_node; i++) {
                        cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                        if (line == 0) {
                            node->setPosition(cocos2d::Vec2(start_1st + count * (item_size.width + padding), start_y));
                            count++;
                            if (count == first_line) {
                                line++;
                                count = 0;
                                start_y -= (item_size.height + padding);
                            }
                        } else if (line == number_line - 1) {
                            node->setPosition(cocos2d::Vec2(start_last + count * (item_size.width + padding), start_y));
                            count++;
                        } else {
                            node->setPosition(cocos2d::Vec2(start_x + count * (item_size.width + padding), start_y));
                            count++;
                            if (count == number_in_line) {
                                line++;
                                count = 0;
                                start_y -= (item_size.height + padding);
                            }
                        }
                    }
                }
            } else {
                if (number_line == 1) {
                    float start_y = center.y + (number_in_line - 1) * (item_size.height + padding) / 2;
                    for (int i=0; i < total_node; i++) {
                        cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                        node->setPosition(cocos2d::Vec2(center.x, start_y - i * (item_size.height + padding)));
                    }
                } else {
                    float start_x = center.x - (number_line - 1) * (item_size.width + padding) / 2;
                    float start_y = center.y + (number_in_line - 1) * (item_size.height + padding) / 2;
                    float start_1st = center.y + (first_line - 1) * (item_size.height + padding) / 2;
                    float start_last = center.y + (last_line - 1) * (item_size.height + padding) / 2;
                    int count = 0;
                    int line = 0;
                    for (int i=0; i < total_node; i++) {
                        cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                        if (line == 0) {
                            node->setPosition(cocos2d::Vec2(start_x, start_1st - count * (item_size.height + padding)));
                            count++;
                            if (count == first_line) {
                                line++;
                                count = 0;
                                start_x += (item_size.width + padding);
                            }
                        } else if (line == number_line - 1) {
                            node->setPosition(cocos2d::Vec2(start_x, start_last - count * (item_size.height + padding)));
                            count++;
                        } else {
                            node->setPosition(cocos2d::Vec2(start_x, start_y - count * (item_size.height + padding)));
                            count++;
                            if (count == number_in_line) {
                                line++;
                                count = 0;
                                start_x += (item_size.width + padding);
                            }
                        }
                    }
                }
            }
            return number_line;
        }
        
        //Set position List In Frame
        template<typename T>
        std::vector<cocos2d::Vec2> setPositionListOnFrame(std::vector<T> list, int column, int row, cocos2d::Size cell, cocos2d::Vec2 center, int align) {
            
            std::vector<cocos2d::Vec2> frames;
            
            float start_x= center.x - column*cell.width/2 + cell.width/2;
            float start_y= center.y + row*cell.height/2 - cell.height/2;
            
            if (align==0) {
                //from top left
                for (int i=0; i<row; i++) {
                    for (int j=0; j<column; j++) {
                        frames.push_back(cocos2d::Vec2(start_x+cell.width*j, start_y-cell.height*i));
                    }
                }
            } else if (align==1) {
                //from top right
                for (int i=0; i<row; i++) {
                    for (int j=column-1; j>=0; j--) {
                        frames.push_back(cocos2d::Vec2(start_x+cell.width*j, start_y-cell.height*i));
                    }
                }
            } else if (align==2) {
                //from bottom left
                for (int i=row-1; i>=0; i--) {
                    for (int j=0; j<column; j++) {
                        frames.push_back(cocos2d::Vec2(start_x+cell.width*j, start_y-cell.height*i));
                    }
                }
            } else if (align==3) {
                //from bottom right
                for (int i=row-1; i>=0; i--) {
                    for (int j=column-1; j>=0; j--) {
                        frames.push_back(cocos2d::Vec2(start_x+cell.width*j, start_y-cell.height*i));
                    }
                }
            }
            
            for (int i=0; i<list.size(); i++) {
                cocos2d::Node* node=static_cast<cocos2d::Node*>(list[i]);
                node->setPosition(frames[i]);
            }
            
            return frames;
        }
    }
    
}


#endif /* Math_Func_h */
