//
//  Pictograph.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef Pictograph_h
#define Pictograph_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"
#include "GraphBase.h"

namespace math {
    class PictoGraph : public GraphBase {
    public:
        static PictoGraph* createGraph();
        
        void show() override;
        void update() override;
        void updateItem(int index) override;
        void reload() override;
        
        /**
         *  @brief Hien thi data bang hinh anh.
         *  @param icon true if show icon, false if show symbol as 'X'
         **/
        void showDataAsIcon(bool icon);
        
        /**
         *  @brief Hien thi tu khoa cac item bang hinh anh.
         *  @param icon true if show icon, false if show name
         **/
        void showItemTitleAsIcon(bool show);
        
        /**
         *  @brief Hien thi theo chieu doc hoac ngang.
         *  @param ver true la chieu doc, false la chieu ngang
         **/
        void setDirectionVertical (bool ver);
        
        /**
         *  @brief Set tieu de cua graph.
         *  @param scale title size = item_size.width*scale
         **/
        void setChartTitleSize(int scale);
        
        void setTitleItemColor(cocos2d::Color4F color);
        void setTitleChartColor(cocos2d::Color4F color);
        
        void setChartBackground(math::resource::Image* image);
        void setTitleBackground(math::resource::Image* image);
        void setIconBackground(math::resource::Image* image);
        
        void enableTouchItem(int index);
        void disableTouchItem(int index);
        void enableTouchAll();
        void disableTouchAll();
    private:
        bool init() override;
        bool item_data = true; // show icon or x mark
        GLbyte hide_opacity = 80;
        int title_scalar = 1;
        
        cocos2d::Color4F color_icon_bar = cocos2d::Color4F(0.32, 0.82, 0.54, 0.4);
        cocos2d::Color4F color_chart_title = cocos2d::Color4F(.87, .21, 0.5, 0.4);
        cocos2d::Color4F color_line = cocos2d::Color4F(92/255.0, 129/255.0, 69/255.0, 1);

        cocos2d::DrawNode* drawer_bar;
        
        std::vector<Math_Object*> icons_list;
        std::vector<std::vector<Math_Object*>> imgdata_list;
        
        void onTouchObject(cocos2d::Node* node);
        
        math::resource::Image* _bg_src ;
        math::resource::Image* _title_bg;
        math::resource::Image* _icon_bg;
        
    };

}

#endif /* Pictograph_h */

