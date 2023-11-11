//
//  Table.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "Table.h"

using namespace math;

#define line_width 1.5
#define padding_size 0.1

Table* Table::createGraph() {
    auto p = new Table();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Table::init() {
    
    _bg_src = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square.png", m_theme->getThemePath().c_str()));
    _title_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/brown1.png", m_theme->getThemePath().c_str()));
    _title_bg2 = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/purple.png", m_theme->getThemePath().c_str()));
    _icon_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square3.png", m_theme->getThemePath().c_str()));
    
    return GraphBase::init();
}

void Table::show() {
    
}

void Table::update() {
    
}

void Table::updateItem(int index) {
    
}

void Table::reload() {
    
    /*--- CLEAR DATA ---*/
    drawer->clear();
    
    for (Math_Object* object : icons_list) {
        object->setDisable();
    }
    icons_list.clear();
    
    /*--- GEN NEW DATA ---*/
    //int max_value = graph_cap;
    int number_item = getNumberItems();
    
    if (title_chart) {
        auto font_name = _graph_items.front().object->getLabelValue()->getFontName();

        auto title_object = cocos2d::ui::Text::create(title_item, font_name, item_size.height*0.4);
        title_object->setTextColor(cocos2d::Color4B::WHITE);
        title_object->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        addChild(title_object, 3);
        titles_list.push_back(title_object);
        
        auto title_quantity = cocos2d::ui::Text::create(title_number, font_name, item_size.height*0.4);
        title_quantity->setTextColor(cocos2d::Color4B::WHITE);
        title_quantity->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        addChild(title_quantity, 3);
        titles_list.push_back(title_quantity);
    }
    
    if (!is_vertical) {
        float start_data = 0;
        if (title_chart) {
            start_data = item_size.width * title_scalar;
            this->setContentSize(cocos2d::Size(item_size.width * (title_scalar + icon_scalar*number_item), 2 * item_size.height));
        } else {
            this->setContentSize(cocos2d::Size(item_size.width * (0 + icon_scalar*number_item), 2 * item_size.height));
        }
        data_scalar = icon_scalar;
        
        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setComparisionValue(1);
            icon->setObjectSize(item_size);
            icon->setPosition(cocos2d::Vec2(item_size.width * (icon_scalar * (0.5 + item.index)) + start_data, item_size.height * 1.5));
            icon->setBackgroundVisible(false);
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name");
                icon->showValueTextWithAnimation(false);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            auto img_number = math::number::getNumber(item.value);
            auto data_obj = item.object->clone();
            data_obj->removeFromParentAndCleanup(false);
            data_obj->setImage(img_number);
            data_obj->setObjectSize(item_size);
            data_obj->setPosition(cocos2d::Vec2(item_size.width * (icon_scalar * (0.5 + item.index)) + start_data, item_size.height * .5));
            data_obj->setBackgroundVisible(false);
            data_obj->setImageVisible(false);
            data_obj->setValueTextByKey("number");
            data_obj->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            data_obj->getLabelValue()->disableEffect();
            data_obj->showValueTextWithAnimation(false);
            data_obj->onShow(0, math::animation::No_Effect);
            addChild(data_obj,3);
            data_list.push_back(data_obj);
            
            auto icon_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            auto render_size_bg = icon_bg->getVirtualRendererSize();
            icon_bg->setScale9Enabled(true);
            icon_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            icon_bg->setContentSize(cocos2d::Size(item_size.width*icon_scalar-line_width, item_size.height-line_width));
            icon_bg->setPosition(icon->getPosition());
            addChild(icon_bg, 2);
            
            auto data_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            data_bg->setScale9Enabled(true);
            data_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            data_bg->setContentSize(cocos2d::Size(item_size.width * icon_scalar -line_width, item_size.height-line_width));
            data_bg->setPosition(data_obj->getPosition());
            addChild(data_bg, 2);
        }
        
        //set background title table
        if(title_chart) {
            cocos2d::ui::ImageView* _background = cocos2d::ui::ImageView::create(_bg_src->path);
            auto render_size = _background->getVirtualRendererSize();
            float padding = render_size.width * padding_size;
            _background->setScale9Enabled(true);
            _background->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
            _background->setContentSize(cocos2d::Size(getContentSize().width + padding, getContentSize().height+padding));
            _background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            _background->setPosition(getContentSize()/2);
            addChild(_background, 0);

            for(int i = 0; i<2; i++){
                cocos2d::ui::Text* title = titles_list[i];
                
                auto img_title_bg = cocos2d::ui::ImageView::create(_title_bg->path);
                auto title_size = img_title_bg->getVirtualRendererSize();
                img_title_bg->setScale9Enabled(true);
                img_title_bg->setCapInsets(cocos2d::Rect(title_size.width*0.4, title_size.height*0.4, title_size.width*0.2, title_size.height*0.2));
                img_title_bg->setContentSize(cocos2d::Size(item_size.width*title_scalar, item_size.height));
                img_title_bg->setPosition(cocos2d::Vec2(item_size.width*title_scalar/2-line_width, item_size.height * (1.5-i)));
                title->setPosition(img_title_bg->getPosition());
                addChild(img_title_bg, 1);
            }
        } else {
            cocos2d::ui::ImageView* _background = cocos2d::ui::ImageView::create(_bg_src->path);
            auto render_size = _background->getVirtualRendererSize();
            float padding = render_size.width * padding_size;
            _background->setScale9Enabled(true);
            _background->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
            _background->setContentSize(cocos2d::Size(getContentSize().width + padding, getContentSize().height + padding));
            _background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            _background->setPosition(getContentSize()/2);
            addChild(_background, 0);
        }
        
    } else {
        if (title_chart) {
            this->setContentSize(cocos2d::Size(item_size.width * (icon_scalar+icon_scalar), (number_item + 1) * item_size.height));
        } else {
            this->setContentSize(cocos2d::Size(item_size.width * (icon_scalar+icon_scalar), number_item * item_size.height));
        }
        
        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setComparisionValue(1);
            icon->setObjectSize(item_size);
            icon->setBackgroundVisible(false);
            icon->setPosition(cocos2d::Vec2(item_size.width * icon_scalar*0.5, item_size.height * (0.5 + item.index)));
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name");
                icon->showValueTextWithAnimation(false);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            auto img_number = math::number::getNumber(item.value);
            auto data_obj = item.object->clone();
            data_obj->removeFromParentAndCleanup(false);
            data_obj->setImage(img_number);
            data_obj->setBackgroundVisible(false);
            data_obj->setObjectSize(item_size);
            data_obj->setPosition(cocos2d::Vec2(item_size.width * (icon_scalar + data_scalar *.5), item_size.height * (0.5 + item.index)));
            data_obj->setImageVisible(false);
            data_obj->setValueTextByKey("number");
            data_obj->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            data_obj->getLabelValue()->disableEffect();
            data_obj->showValueTextWithAnimation(false);
            data_obj->onShow(0, math::animation::No_Effect);
            addChild(data_obj,3);
            data_list.push_back(data_obj);
            
            auto icon_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            auto render_size_bg = icon_bg->getVirtualRendererSize();
            icon_bg->setScale9Enabled(true);
            icon_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            icon_bg->setContentSize(cocos2d::Size(item_size.width*icon_scalar -line_width, item_size.height-line_width));
            icon_bg->setPosition(icon->getPosition());
            addChild(icon_bg, 1);
            
            auto data_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            data_bg->setScale9Enabled(true);
            data_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            data_bg->setContentSize(cocos2d::Size(item_size.width*data_scalar -line_width, item_size.height-line_width));
            data_bg->setPosition(data_obj->getPosition());
            addChild(data_bg, 1);
        }
        
        //set background title table
        if(title_chart) {
            cocos2d::ui::ImageView* _background = cocos2d::ui::ImageView::create(_bg_src->path);
            auto render_size = _background->getVirtualRendererSize();
            float padding = render_size.width * padding_size;
            _background->setScale9Enabled(true);
            _background->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
            _background->setContentSize(cocos2d::Size(getContentSize().width + padding, getContentSize().height+padding));
            _background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            _background->setPosition(getContentSize()/2);
            addChild(_background, 0);
            
            for(int i = 0; i < 2; i++){
                cocos2d::ui::Text* title = titles_list[i];
                
                auto img_title_bg = cocos2d::ui::ImageView::create(_title_bg2->path);
                auto title_size = img_title_bg->getVirtualRendererSize();
                img_title_bg->setScale9Enabled(true);
                img_title_bg->setCapInsets(cocos2d::Rect(title_size.width*0.4, title_size.height*0.4, title_size.width*0.2, title_size.height*0.2));
                img_title_bg->setContentSize(cocos2d::Size(item_size.width*(i==0?icon_scalar:data_scalar)+padding/2, item_size.height+padding/2));
                img_title_bg->setPosition(cocos2d::Vec2(item_size.width*(i==0?(icon_scalar/2):(icon_scalar + data_scalar/2)) + (i==0?(-padding/4):(padding/4)), item_size.height * (.5 + number_item)+padding/4));
                title->setPosition(img_title_bg->getPosition());
                addChild(img_title_bg, 1);
            }
        } else {
            cocos2d::ui::ImageView* _background = cocos2d::ui::ImageView::create(_bg_src->path);
            auto render_size = _background->getVirtualRendererSize();
            float padding = render_size.width * padding_size;
            _background->setScale9Enabled(true);
            _background->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
            _background->setContentSize(cocos2d::Size(getContentSize().width + padding, getContentSize().height + padding));
            _background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            _background->setPosition(getContentSize()/2);
            addChild(_background, 0);
        }
    }
    
    if(is_Color){
        for(int i = 0; i < icons_list.size(); i++){
            auto size = cocos2d::Size((item_size.width) * (icon_scalar) , item_size.height + 5);
            cocos2d::Point point (0.5, 0.5);
            if(i == 0 ){
                size = cocos2d::Size((item_size.width +5) * (icon_scalar) , item_size.height + 5);
                point = cocos2d::Point(0.51,0.5);
            }else if( i == icons_list.size()-1){
                size = cocos2d::Size((item_size.width +5) * (icon_scalar) , item_size.height + 5);
                point = cocos2d::Point(0.47,0.5);
            }
            cocos2d::ui::ImageView* _icon_bg = cocos2d::ui::ImageView::create(_images.at(i)->path);
            _icon_bg->setAnchorPoint(point);
            _icon_bg->setContentSize(size);
            _icon_bg->setScale9Enabled(true);
            _icon_bg->setPosition(cocos2d::Vec2(icons_list.at(i)->getPosition().x, icons_list.at(i)->getPosition().y+4));
            
            addChild(_icon_bg,2);
        }
    }
}

void Table::setDirectionVertical(bool ver) {
    is_vertical = ver;
}

void Table::showItemTitleAsIcon(bool show) {
    item_title = show;
}

void Table::setChartTitleSize(int scale) {
    title_scalar = scale;
}

void Table::setItemTitleScale(int scale) {
    icon_scalar = scale;
}

void Table::seItemScale(int scale) {
    data_scalar = scale;
}

void Table::setColorIconTitle(bool color, std::vector< math::resource::Image*> images){
    is_Color = color;
    _images = images;
}

void Table::setChartBackground(math::resource::Image* image){
    //_bg_src = image;
}

void Table::setTitleBackground(math::resource::Image* image){
    //_title_bg = image;
}

void Table::setIconBackground(std::vector< math::resource::Image*> images){
//    _images = images;
}
