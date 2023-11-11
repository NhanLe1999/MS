//
//  TallyChart.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "TallyChart.h"
using namespace math;

#define tally_scalar 2.0
#define line_width 1.5
#define padding_size 0.1

TallyChart* TallyChart::createGraph() {
    auto p = new TallyChart();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool TallyChart::init() {
    
    _bg_src = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square.png", m_theme->getThemePath().c_str()));
    _title_bg2 = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/purple.png", m_theme->getThemePath().c_str()));
    _title_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/brown1.png", m_theme->getThemePath().c_str()));
    _icon_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/frame/square3.png", m_theme->getThemePath().c_str()));
    
    return GraphBase::init();
}

void TallyChart::show() {
    
}

void TallyChart::update() {
    
}

void TallyChart::updateItem(int index) {
    
}

void TallyChart::reload() {
    
    /*--- CLEAR DATA ---*/
    drawer->clear();
    for (int i=0; i<drawers_item.size(); i++) {
        drawers_item[i]->clear();
    }
    drawers_item.clear();
    
    for (Math_Object* object : icons_list) {
        object->setDisable();
    }
    icons_list.clear();
    
    /*--- GEN NEW DATA ---*/
    int max_value = graph_cap;
    int number_item = getNumberItems();
    int number_tally_5 = (max_value+4)/5;

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
    
    if (is_vertical) {
        float start_x = 0;
        if (title_chart) {
            this->setContentSize(cocos2d::Size(item_size.width * (number_tally_5*tally_scalar*number_item + title_scalar), 2 * item_size.height));
            start_x = item_size.width * title_scalar;
        } else {
            this->setContentSize(cocos2d::Size(item_size.width * number_tally_5*tally_scalar * number_item, 2 * item_size.height));
        }

        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setObjectSize(item_size * 0.95);
            icon->setPosition(cocos2d::Vec2(item_size.width * number_tally_5 * tally_scalar * (0.5 + item.index) + start_x, item_size.height * 1.5));
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name");
                icon->showValueTextWithAnimation(false);
                icon->getLabelValue()->disableEffect();
                icon->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            //draw tally
            cocos2d::DrawNode* drawer_tally = cocos2d::DrawNode::create();
            addChild(drawer_tally,3);
            drawers_item.push_back(drawer_tally);
            
            float delta = number_tally_5 * tally_scalar * item_size.width;
            
            for (int i=0; i<item.value; i++) {
                int l_index = i%5 + 1;
                int t_index = i/5;
                if (_animated == NoAnimation) {
                    if (l_index == 5) {
                        //duong cheo
                        drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + tally_scalar/7) + delta * item.index, item_size.height * 0.3),
                                                  cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.7), 3, color_tally);
                    } else {
                        drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.2),
                                                  cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.8), 3, color_tally);
                    }
                } else {
                    this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(i*0.1), cocos2d::CallFunc::create([=](){
                        if (l_index == 5) {
                            //duong cheo
                            drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + tally_scalar/7) + delta * item.index, item_size.height * 0.3),
                                                      cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.7), 3, color_tally);
                        } else {
                            drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.2),
                                                      cocos2d::Vec2(item_size.width * (t_index * tally_scalar + (l_index + 1) * tally_scalar/7) + delta * item.index, item_size.height * 0.8), 3, color_tally);
                        }
                    })));
                }
            }
            
            auto icon_bg = cocos2d::ui::ImageView::create(_title_bg->path);
            auto render_size_bg = icon_bg->getVirtualRendererSize();
            icon_bg->setScale9Enabled(true);
            icon_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            icon_bg->setContentSize(cocos2d::Size(item_size.width*number_tally_5*tally_scalar-line_width, item_size.height-line_width));
            icon_bg->setPosition(icon->getPosition());
            addChild(icon_bg, 2);
            
            auto data_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            render_size_bg = data_bg->getVirtualRendererSize();
            data_bg->setScale9Enabled(true);
            data_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            data_bg->setContentSize(cocos2d::Size(item_size.width * number_tally_5*tally_scalar -line_width, item_size.height-line_width));
            data_bg->setPosition(cocos2d::Vec2(item_size.width * (title_scalar+number_tally_5*tally_scalar*0.5) , icon->getPosition().y));
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
                
                auto img_title_bg = cocos2d::ui::ImageView::create(_title_bg2->path);
                auto title_size = img_title_bg->getVirtualRendererSize();
                img_title_bg->setScale9Enabled(true);
                img_title_bg->setCapInsets(cocos2d::Rect(title_size.width*0.4, title_size.height*0.4, title_size.width*0.2, title_size.height*0.2));
                img_title_bg->setContentSize(cocos2d::Size(item_size.width*title_scalar, item_size.height));
                img_title_bg->setPosition(cocos2d::Vec2(item_size.width*title_scalar/2+padding/2-line_width, item_size.height * (1.5-i)));
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
            this->setContentSize(cocos2d::Size(item_size.width * (title_scalar+number_tally_5*tally_scalar), (number_item+1) * item_size.height));
        } else {
            this->setContentSize(cocos2d::Size(item_size.width * (title_scalar+number_tally_5*tally_scalar), number_item * item_size.height));
        }
        
        //Add data
        for (GraphItem item : _graph_items) {
            auto icon = item.object->clone();
            icon->removeFromParentAndCleanup(false);
            icon->setObjectSize(item_size * 0.95);
            icon->setPosition(cocos2d::Vec2(item_size.width * title_scalar*0.5, item_size.height * (0.5 + item.index)));
            if (!item_title) {
                icon->setImageVisible(false);
                icon->setValueTextByKey("name");
                icon->showValueTextWithAnimation(false);
                icon->getLabelValue()->disableEffect();
                icon->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
            }
            icon->onShow(0, math::animation::No_Effect);
            addChild(icon,3);
            icons_list.push_back(icon);
            
            //draw tally
            cocos2d::DrawNode* drawer_tally = cocos2d::DrawNode::create();
            addChild(drawer_tally,3);
            drawers_item.push_back(drawer_tally);
            
            for (int i=0; i<item.value; i++) {
                int l_index = i%5 + 1;
                int t_index = i/5;
                if (_animated == NoAnimation) {
                    if (l_index == 5) {
                        //duong cheo
                        drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + tally_scalar/7), item_size.height * (0.3 + item.index)),
                                                  cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.7 + item.index)), 3, color_tally);
                    } else {
                        drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.2 + item.index)),
                                                  cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.8 + item.index)), 3, color_tally);
                    }
                } else {
                    this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(i*0.1), cocos2d::CallFunc::create([=](){
                        if (l_index == 5) {
                            //duong cheo
                            drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + tally_scalar/7), item_size.height * (0.3 + item.index)),
                                                      cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.7 + item.index)), 3, color_tally);
                        } else {
                            drawer_tally->drawSegment(cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.2 + item.index)),
                                                      cocos2d::Vec2(item_size.width * (t_index * tally_scalar + title_scalar + (l_index + 1) * tally_scalar/7), item_size.height * (0.8 + item.index)), 3, color_tally);
                        }
                    })));
                }
            }
            
            auto icon_bg = cocos2d::ui::ImageView::create(_title_bg->path);
            auto render_size_bg = icon_bg->getVirtualRendererSize();
            icon_bg->setScale9Enabled(true);
            icon_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            icon_bg->setContentSize(cocos2d::Size(item_size.width*title_scalar -line_width, item_size.height-line_width));
            icon_bg->setPosition(icon->getPosition());
            addChild(icon_bg, 1);
            
            auto data_bg = cocos2d::ui::ImageView::create(_icon_bg->path);
            render_size_bg = data_bg->getVirtualRendererSize();
            data_bg->setScale9Enabled(true);
            data_bg->setCapInsets(cocos2d::Rect(render_size_bg.width*0.4, render_size_bg.height*0.4, render_size_bg.width*0.2, render_size_bg.height*0.2));
            data_bg->setContentSize(cocos2d::Size(item_size.width * number_tally_5*tally_scalar -line_width, item_size.height-line_width));
            data_bg->setPosition(cocos2d::Vec2(item_size.width * (title_scalar + number_tally_5*tally_scalar*0.5), icon->getPosition().y));
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
                img_title_bg->setContentSize(cocos2d::Size(item_size.width*(i==0?item_title:number_tally_5*tally_scalar)+padding/2, item_size.height+padding/2));
                img_title_bg->setPosition(cocos2d::Vec2(item_size.width*(i==0?(title_scalar/2):(title_scalar + number_tally_5*tally_scalar/2)) + (i==0?(-padding/4):(padding/4)), item_size.height * (.5 + number_item)+padding/4));
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
}

void TallyChart::setDirectionVertical(bool ver) {
    is_vertical = ver;
}

void TallyChart::showItemTitleAsIcon(bool show) {
    item_title = show;
}

void TallyChart::setChartTitleSize(int scale) {
    //title_scalar = scale;
}

void TallyChart::setChartBackground(math::resource::Image* image){
    //_bg_src = image;
}

void TallyChart::setIconBackground(math::resource::Image* image){
    //_title_bg = image;
}
