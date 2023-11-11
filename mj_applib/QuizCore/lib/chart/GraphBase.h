//
//  GraphBase.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef GraphBase_h
#define GraphBase_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"

namespace math {
    class GraphBase : public cocos2d::ui::Layout {
    public:
        GraphBase(){};
        ~GraphBase(){};
        
        static GraphBase* createGraph();
        bool init() override;
        
        struct GraphItem {
            int index;
            Math_Object* object;
            int value;
        };
        
        enum AnimationMode {
            NoAnimation = 0,
            UpdateFromZero,
            UpdateContinue
        };
        
        void setGraphCap(int cap);
        void setAnimationMode(AnimationMode mode);
        void setTitleChar(bool title);
        
        /**
         *  @brief insert item into graph.
         *  Them item vao bieu do.
         *  @param object GraphItem object
         *  @param value GraphItem value
         **/
        void addItem(Math_Object* object, int value);
        
        bool removeItem(int index);
        
        int getNumberItems();
        
        /**
         *  @brief Item co chi so lon nhat.
         **/
        GraphItem getMostItem();
        
        /**
         *  @brief Item co chi so be nhat.
         **/
        GraphItem getLeastItem();
        
        /**
         *  @brief Get graph range.
         *  Gia tri chenh lech giua item co chi so cao nhat so voi item co chi so thap nhat
         **/
        int graphRange();
        
        int distanceBetweenTwoItems(int index_1, int index_2);
        
        void changeValueForItem(int index, int value);
        void increaseValueForItem(int index, int value);
        
        void setItemSize(cocos2d::Size size);
        void setTitleItem(std::string title);
        void setTitleNumber(std::string title);
        
        std::vector<GraphItem> getGraphItems();
        
        //Compare with number
        std::vector<GraphItem> getItemsMoreThan(int value, bool include_equal = true);
        std::vector<GraphItem> getItemsLessThan(int value, bool include_equal = true);
        std::vector<GraphItem> getItemsEqual(int value);
        
        int numberOfItemsMoreThan(int value, bool include_equal = true);
        int numberOfItemsLessThan(int value, bool include_equal = true);
        int numberOfItemsEqual(int value);
        
        virtual void show();
        virtual void update();
        virtual void updateItem(int index);
        virtual void reload();
        
    protected:
        std::string title_item = "";
        std::string title_number = "";
        
        bool is_vertical = false;
        bool item_title = true; // show icon or name
        bool title_chart = true; // show or hide title chart
        cocos2d::Size item_size;
        
        cocos2d::DrawNode* drawer;
        std::vector<GraphItem> _graph_items;
        AnimationMode _animated = AnimationMode::NoAnimation;
        int graph_cap;
        std::vector<cocos2d::ui::Text*> titles_list;
        
    };

}

#endif /* GraphBase_h */
