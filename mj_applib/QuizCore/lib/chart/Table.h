//
//  Table.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef Table_h
#define Table_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"
#include "GraphBase.h"
#include "NumberUtils.h"

namespace math {
    class Table : public GraphBase {
    public:
        static Table* createGraph();
        
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
        void setTitleBackground(math::resource::Image* image);
        void setIconBackground(std::vector<math::resource::Image*> image);
        void setColorIconTitle(bool color, std::vector< math::resource::Image*> images);
        
        void setItemTitleScale(int scale);
        void seItemScale(int scale);
    private:
        bool init() override;
        bool item_title = true;
        
        int title_scalar = 3;
        int icon_scalar = 2;
        int data_scalar = 2;
        
        cocos2d::Color4F color_line = cocos2d::Color4F(92/255.0, 129/255.0, 69/255.0, 1);
        std::vector<Math_Object*> icons_list;
        std::vector<Math_Object*> data_list;
        
        cocos2d::ui::ImageView* _background;
        math::resource::Image* _bg_src ;
        math::resource::Image* _title_bg;
        math::resource::Image* _title_bg2;
        math::resource::Image* _icon_bg;
        
        std::vector< math::resource::Image*> _images;
        bool is_Color = false;
    };

}
#endif /* Table_h */

