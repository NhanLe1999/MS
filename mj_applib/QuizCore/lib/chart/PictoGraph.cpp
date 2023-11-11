//
//  Pictograph.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "PictoGraph.h"

using namespace math;

#define line_width 1.5
#define padding_size 0.1

PictoGraph* PictoGraph::createGraph() {
    auto p = new PictoGraph();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool PictoGraph::init() {
    
    drawer_bar = cocos2d::DrawNode::create();
    this->addChild(drawer_bar);
    
    _bg_src = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square.png", m_theme->getThemePath().c_str()));
    _title_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/brown2.png", m_theme->getThemePath().c_str()));
    _icon_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square3.png", m_theme->getThemePath().c_str()));
    
    return GraphBase::init();
}

void PictoGraph::show() {
    
}

void PictoGraph::update() {
    if (_animated == NoAnimation) {
        for (int i=0; i<imgdata_list.size(); i++) {
            auto item = _graph_items[i];
            for (int j = 0; j<imgdata_list[i].size(); j++) {
                auto object = imgdata_list[i][j];
                if (j < item.value) {
                    object->setImageColor(cocos2d::Color3B::WHITE);
                    object->getImage()->setOpacity(255);
                } else {
                    object->setImageColor(cocos2d::Color3B::BLACK);
                    object->getImage()->setOpacity(hide_opacity);
                }
            }
        }
    } else if (_animated == UpdateFromZero) {
        for (int i=0; i<imgdata_list.size(); i++) {
            auto item = _graph_items[i];
            for (int j = 0; j<imgdata_list[i].size(); j++) {
                auto object = imgdata_list[i][j];
                object->setImageColor(cocos2d::Color3B::BLACK);
                object->getImage()->setOpacity(hide_opacity);
                if (j < item.value) {
                    object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(j*0.1), cocos2d::CallFunc::create([=](){
                        object->setImageColor(cocos2d::Color3B::WHITE);
                        object->getImage()->setOpacity(255);
                    })));
                }
            }
        }
    } else {
        for (int i=0; i<imgdata_list.size(); i++) {
            auto item = _graph_items[i];
            int old_value=0;
            auto list = imgdata_list[i];
            
            for (int j = 0; j<list.size(); j++) {
                auto object = list[j];
                auto opacity = object->getImage()->getOpacity();
                if (opacity>200) {
                    old_value++;
                }
            }
            
            if (old_value < item.value) {
                for (int j=old_value; j<item.value; j++) {
                    auto object = list[j];
                    object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create((j-old_value)*0.1), cocos2d::CallFunc::create([=](){
                        object->setImageColor(cocos2d::Color3B::WHITE);
                        object->getImage()->setOpacity(255);
                    })));
                }
            } else if (old_value > item.value) {
                for (int j=old_value-1; j>=item.value; j--) {
                    auto object = list[j];
                    object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create((old_value-j)*0.1), cocos2d::CallFunc::create([=](){
                        object->setImageColor(cocos2d::Color3B::BLACK);
                        object->getImage()->setOpacity(hide_opacity);
                    })));
                }
            }
        }
    }
}

void PictoGraph::updateItem(int index) {
    if (_animated == NoAnimation) {
        auto item = _graph_items[index];
        for (int j = 0; j<imgdata_list[index].size(); j++) {
            auto object = imgdata_list[index][j];
            if (j < item.value) {
                object->setImageColor(cocos2d::Color3B::WHITE);
                object->getImage()->setOpacity(255);
            } else {
                object->setImageColor(cocos2d::Color3B::BLACK);
                object->getImage()->setOpacity(hide_opacity);
            }
        }
    } else if (_animated == UpdateFromZero) {
        auto item = _graph_items[index];
        for (int j = 0; j<imgdata_list[index].size(); j++) {
            auto object = imgdata_list[index][j];
            object->setImageColor(cocos2d::Color3B::BLACK);
            object->getImage()->setOpacity(hide_opacity);
            if (j < item.value) {
                object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(j*0.1), cocos2d::CallFunc::create([=](){
                    object->setImageColor(cocos2d::Color3B::WHITE);
                    object->getImage()->setOpacity(255);
                })));
            }
        }
    } else {
        auto item = _graph_items[index];
        int old_value=0;
        auto list = imgdata_list[index];
        
        for (int j = 0; j<list.size(); j++) {
            auto object = list[j];
            auto opacity = object->getImage()->getOpacity();
            if (opacity>200) {
                old_value++;
            }
        }
        
        if (old_value < item.value) {
            for (int j=old_value; j<item.value; j++) {
                auto object = list[j];
                object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create((j-old_value)*0.1), cocos2d::CallFunc::create([=](){
                    object->setImageColor(cocos2d::Color3B::WHITE);
                    object->getImage()->setOpacity(255);
                })));
            }
        } else if (old_value > item.value) {
            for (int j=old_value-1; j>=item.value; j--) {
                auto object = list[j];
                object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create((old_value-j)*0.1), cocos2d::CallFunc::create([=](){
                    object->setImageColor(cocos2d::Color3B::BLACK);
                    object->getImage()->setOpacity(hide_opacity);
                })));
            }
        }
    }
}

void PictoGraph::reload() {
    
    int icon_size = 1;
    /*--- CLEAR DATA ---*/
    drawer->clear();
    drawer_bar->clear();
    
    for (Math_Object* object : icons_list) {
        object->setDisable();
    }
    icons_list.clear();
    
    for (int i=0; i<imgdata_list.size(); i++) {
        for (Math_Object* object : imgdata_list[i]) {
            object->setDisable();
        }
        imgdata_list[i].clear();
    }
    imgdata_list.clear();
    
    
    /*--- GEN NEW DATA ---*/
    int max_value = graph_cap;
    int number_item = getNumberItems();
    math::resource::Image* data_img = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgame_ui/chart/mark_x.png", m_theme->getThemePath().c_str()));
    
    if (title_chart) {
        auto font_name = _graph_items.front().object->getLabelValue()->getFontName();
        auto title_object = cocos2d::ui::Text::create(title_item, font_name, item_size.height*0.4);
        title_object->setTextColor(cocos2d::Color4B::WHITE);
        title_object->setPosition(cocos2d::Vec2(item_size.width * title_scalar * 0.5, item_size.height*(number_item - 0.5)));
        title_object->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        addChild(title_object, 3);
        titles_list.push_back(title_object);
        
        auto title_quantity = cocos2d::ui::Text::create(title_number, font_name, item_size.height*0.4);
        title_quantity->setTextColor(cocos2d::Color4B::WHITE);
        title_quantity->setPosition(cocos2d::Vec2(item_size.width * (title_scalar + graph_cap * 0.5), item_size.height*(number_item - 0.5)));
        title_quantity->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        addChild(title_quantity, 3);
        titles_list.push_back(title_quantity);
    }
    
    if (is_vertical) {
        this->setContentSize(cocos2d::Size(number_item * item_size.width, (max_value + 1) * item_size.height));

        int index_item=0;
        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setBackground(_title_bg, 0.3);
            icon->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            icon->getLabelValue()->disableEffect();
            icon->setComparisionValue(icon_size);
            icon->setObjectSize(cocos2d::Size(item_size.width*title_scalar-line_width, item_size.height-line_width));
            icon->setPosition(cocos2d::Vec2(item_size.width * (index_item + 0.5), item_size.height * 0.5));
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name_1");
                icon->showValueTextWithAnimation(false);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            std::vector<Math_Object*> imgdata;
            for (int i=0; i<max_value; i++) {
                auto icon_data = item.object->clone();
                icon_data->removeFromParentAndCleanup(false);
                if (!item_data) {
                    icon_data->setImage(data_img);
                }
                icon_data->setComparisionValue(icon_size);
                icon_data->setBackground(_icon_bg, 0.3);
                icon_data->setObjectSize(cocos2d::Size(item_size.width-line_width, item_size.height-line_width));
                icon_data->setPosition(cocos2d::Vec2(item_size.width * (index_item + 0.5), item_size.height * (1.5 + i)));
                icon_data->setComparisionValue("number", i+1);
                icon_data->setTouchCallback(CC_CALLBACK_1(PictoGraph::onTouchObject, this));
                icon_data->setImageColor(cocos2d::Color3B::BLACK);
                icon_data->getImage()->setOpacity(hide_opacity);
                icon_data->onShow(0, math::animation::No_Effect);
                addChild(icon_data,3);

                if (i<item.value) {
                    if (_animated == NoAnimation) {
                        icon_data->setImageColor(cocos2d::Color3B::WHITE);
                        icon_data->getImage()->setOpacity(255);
                    } else {
                        icon_data->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5+i*0.1), cocos2d::CallFunc::create([=](){
                            icon_data->setImageColor(cocos2d::Color3B::WHITE);
                            icon_data->getImage()->setOpacity(255);
                        })));
                    }
                }
                
                imgdata.push_back(icon_data);
            }
            imgdata_list.push_back(imgdata);
            index_item++;
        }

    } else {
        if (title_chart) {
            number_item++;
        }

        this->setContentSize(cocos2d::Size((max_value + title_scalar) * item_size.width, number_item * item_size.height));
        //Add titles
        int index_item=0;
        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setBackground(_title_bg, 0.3);
            icon->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            icon->getLabelValue()->disableEffect();
            icon->setComparisionValue(icon_size);
            icon->setObjectSize(cocos2d::Size(item_size.width*title_scalar-line_width, item_size.height-line_width));
            icon->setPosition(cocos2d::Vec2(item_size.width * title_scalar * 0.5, item_size.height * (index_item + 0.5)));
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name_1");
                icon->showValueTextWithAnimation(false);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            std::vector<Math_Object*> imgdata;
            for (int i=0; i<max_value; i++) {
                auto icon_data = item.object->clone();
                icon_data->removeFromParentAndCleanup(false);
                if (!item_data) {
                    icon_data->setImage(data_img);
                }
                icon_data->setComparisionValue(icon_size);
                icon_data->setBackground(_icon_bg, 0.3);
                icon_data->setObjectSize(cocos2d::Size(item_size.width-line_width, item_size.height-line_width));
                icon_data->setPosition(cocos2d::Vec2(item_size.width * (title_scalar + 0.5 + i), item_size.height * (index_item + 0.5)));
                icon_data->setComparisionValue("number", i+1);
                icon_data->setTouchCallback(CC_CALLBACK_1(PictoGraph::onTouchObject, this));
                icon_data->setImageColor(cocos2d::Color3B::BLACK);
                icon_data->getImage()->setOpacity(hide_opacity);
                icon_data->onShow(0, math::animation::No_Effect);
                addChild(icon_data, 3);
                
                if (i<item.value) {
                    if (_animated == NoAnimation) {
                        icon_data->setImageColor(cocos2d::Color3B::WHITE);
                        icon_data->getImage()->setOpacity(255);
                    } else {
                        icon_data->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5+i*0.1), cocos2d::CallFunc::create([=](){
                            icon_data->setImageColor(cocos2d::Color3B::WHITE);
                            icon_data->getImage()->setOpacity(255);
                        })));
                    }
                }
                imgdata.push_back(icon_data);
            }
            imgdata_list.push_back(imgdata);
            index_item++;
        }
    }
    
    //set background
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

void PictoGraph::setChartTitleSize(int scale) {
    title_scalar = scale;
}

void PictoGraph::showDataAsIcon(bool icon) {
    item_data = icon;
    if (item_data) {
        hide_opacity = 80;
    } else {
        hide_opacity = 0;
    }
}

void PictoGraph::showItemTitleAsIcon(bool show) {
    item_title = show;
}

void PictoGraph::setTitleItemColor(cocos2d::Color4F color) {
    color_icon_bar = color;
}

void PictoGraph::setTitleChartColor(cocos2d::Color4F color) {
    color_chart_title = color;
}

void PictoGraph::setDirectionVertical(bool ver) {
    is_vertical = ver;
}

void PictoGraph::enableTouchItem(int index) {
    if (index < 0 || index >= _graph_items.size()) {
        return;
    }
    for (Math_Object* object : imgdata_list[index]) {
        object->setTouch(math::common::touch);
        object->setLocked(false);
    }
}

void PictoGraph::disableTouchItem(int index) {
    if (index < 0 || index >= _graph_items.size()) {
        return;
    }
    for (Math_Object* object : imgdata_list[index]) {
        object->setTouch(math::common::deny);
        object->setLocked(true);
    }
}

void PictoGraph::enableTouchAll() {
    for (int i=0; i<imgdata_list.size(); i++) {
        for (Math_Object* object : imgdata_list[i]) {
            object->setTouch(math::common::touch);
            object->setLocked(false);
        }
    }
}

void PictoGraph::disableTouchAll() {
    for (int i=0; i<imgdata_list.size(); i++) {
        for (Math_Object* object : imgdata_list[i]) {
            object->setTouch(math::common::deny);
            object->setLocked(true);
        }
    }
}

void PictoGraph::onTouchObject(cocos2d::Node *node) {
    Math_Object* object = (Math_Object*)node;
    int index_item = object->getID();
    int value = object->getComparisionValue().number;
    CCLOG("update item at %d value %d", index_item, value);
    changeValueForItem(index_item, value);
    
    updateItem(index_item);
}

void PictoGraph::setChartBackground(math::resource::Image* image){
    _bg_src = image;
}

void PictoGraph::setTitleBackground(math::resource::Image* image){
    _title_bg = image;
}

void PictoGraph::setIconBackground(math::resource::Image* image){
    _icon_bg = image;
}
