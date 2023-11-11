//
//  MSComplexCollection.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/9/17.
//

#ifndef MSComplexCollection_hpp
#define MSComplexCollection_hpp

#include "cocostudiomacro2.h"

namespace ms {
    namespace delegate {
        class CollectionDataSource;
        class Collection;
    }
}

class MSComplexCollection : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSComplexCollection);
    static MSComplexCollection * createCollection(ms::delegate::CollectionDataSource * data_source);
    
    void setPadding(float top, float bottom) {_top_padding = top;_bottom_padding = bottom;};
    void reloadData();
    
    void processClickEvent(cocos2d::Point touch_position);
    void runAnimationAppear();
    void runAnimationDisappear();
    
    void scrollTo(float percent);
    
    bool updatePadding();
    
    
    cocos2d::ui::ScrollView * getScrollview() {return _scrollview;};
    float getHorizontalSpace() {return _horizontal_space;};
    
private:
    void didLoadFromCSB(ms::delegate::CollectionDataSource * data_source);
    
    cocos2d::Size _model_size;
    void updatePage(float dt);
    std::vector<int> mListIdOfLessonsLearned;
    int _nrow, _ncol;
    int _min_id_showing, _max_id_showing;
    int _ncell;
    
    float _horizontal_space = 0;
    cocos2d::ui::ScrollView * _scrollview;
    
    ms::delegate::CollectionDataSource * _data_source;
    CC_SYNTHESIZE(ms::delegate::Collection *, _delegate, Delegate);
    
    float _top_padding, _bottom_padding;
    
    long long _time_touch_start;
    bool _should_process_click;
    cocos2d::Vec2 _position_touch_start;
    cocos2d::Rect _scrollview_bb;
    cocos2d::ui::Layout* _innerContainer;
};

CREATE_READER(MSComplexCollection);

#endif /* MSComplexCollection_hpp */
