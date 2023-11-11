//
//  TallyChart.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef TallyChart_h
#define TallyChart_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"
#include "GraphBase.h"

namespace math {
    class TallyChart : public GraphBase {
    public:
        static TallyChart* createGraph();
        
        void show() override;
        void update() override;
        void updateItem(int index) override;
        void reload() override;
        
        void setDirectionVertical (bool ver);
        void showItemTitleAsIcon(bool show);
        
        /**
         *  @brief Set tieu de cua graph.
         *  @param scale title size = item_size.width*scale
         **/
        void setChartTitleSize(int scale);
        
        void setChartBackground(math::resource::Image* image);
        void setIconBackground(math::resource::Image* image);
        
    private:
        bool init() override;
        bool item_title = true;
        int title_scalar = 3;
        
        cocos2d::Color4F color_title = cocos2d::Color4F(0.5, 0.5, 1, 1);
        cocos2d::Color4F color_items_title = cocos2d::Color4F(1, 1, 1, 1);
        cocos2d::Color4F color_tally = cocos2d::Color4F(1, 1, 1, 1);
        cocos2d::Color4F color_line = cocos2d::Color4F(92/255.0, 129/255.0, 69/255.0, 1);
        
        std::vector<cocos2d::DrawNode*> drawers_item;
        std::vector<Math_Object*> icons_list;
        
        cocos2d::ui::ImageView* _background;
        math::resource::Image* _bg_src ;
        math::resource::Image* _title_bg;
        math::resource::Image* _title_bg2;
        math::resource::Image* _icon_bg;

    };

}

#endif /* TallyChart_h */

