//
//  Math_Group.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/17/17.
//
//

#include "Math_Group.h"
#include "AudioHelper.h"

INITIALIZE_READER_INSTANCE(Math_GroupReader);


Math_Group * Math_Group::createGroup() {
    auto p = (Math_Group *)(cocos2d::CSLoader::createNode("math_group.csb"));
    
    if (p && p->init()) {
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Math_Group::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    _background = this->getChildByName<cocos2d::ui::ImageView*>("background");
    _foreground = this->getChildByName<cocos2d::ui::ImageView*>("foreground");
    _check_touch = this->getChildByName<cocos2d::ui::ImageView*>("check_touch");

    _txt_name = this->getChildByName<cocos2d::ui::Text*>("name");
    _txt_value = this->getChildByName<cocos2d::ui::Text*>("value");
    _panel = this->getChildByName<cocos2d::ui::Layout*>("panel");
    _button = this->getChildByName<cocos2d::ui::Button*>("button");
    
    this->setTag(2002);
    this->setName("Math_Group");
    return true;
}

void Math_Group::setEnable(int g_id, std::vector<Math_Object *> list, math::group::GroupType type, int cap) {
    
    _enable = true;
    _id=g_id;
    _objects=list;
    _group_type=type;
    _touch_type=math::group::box;
    _group_align=math::group::random;
    _group_cap = cap;
    
    _bg_src = NULL;
    _fg_src = NULL;
    _touch_src = NULL;
    
    hideValueText(false);
    hideNameText(false);
    _txt_name->setFontName(m_text_font);
    _txt_name->setTextColor(m_text_color);
    _txt_name->enableOutline(cocos2d::Color4B::WHITE, 5);
    _txt_value->setFontName(m_text_font);
    _txt_value->setTextColor(m_text_color);
    _txt_value->enableOutline(cocos2d::Color4B::WHITE, 5);

    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _button->setVisible(false);
    _callback_func=nullptr;
    
    for (Math_Object* object : _objects) {
        object->addToGroup(object->getID(), this, _id);
    }
    
    _background->setVisible(false);
    _foreground->setVisible(false);
    _check_touch->setVisible(false);
}

void Math_Group::setDisable(float delay, math::animation::ObjectShowType revert_show, math::animation::ObjectShowType revert_itemshow) {
    _enable = false;
    if (this->getParent() != nullptr) {
        onHide(delay, revert_show, revert_itemshow);
    }
}

bool Math_Group::isEnable() {
    return _enable;
}

void Math_Group::onShow(float dt, math::animation::ObjectShowType show, math::animation::ObjectShowType itemshow) {
    
    this->stopAllActions();
    int delta = 100;
    this->setOpacity(0);
    auto position = this->getPosition();
    
    switch (show) {
        case math::animation::No_Effect :
        {
            //this->setOpacity(255);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::FadeIn::create(0.3)));
        }
            break;
        case math::animation::ScaleUp :
        {
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
        }
            break;
        case math::animation::SlideUp :
        {
            this->setPosition(cocos2d::Vec2(position.x, position.y - delta));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideDown :
        {
            this->setPosition(cocos2d::Vec2(position.x, position.y + delta));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideLeft :
        {
            this->setPosition(cocos2d::Vec2(position.x + delta, position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideRight :
        {
            this->setPosition(cocos2d::Vec2(position.x - delta, position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        default:
        {
            this->setRotation(180);
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::create(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)), cocos2d::RotateTo::create(0.4, 0), NULL)));
        }
            break;
    }
    
    scheduleOnce([=](float dt){
        int number_objs = (int)_objects.size();
        std::vector<int> random_show;
        for (int i=0; i<number_objs; i++) {
            random_show.push_back(i);
        }
        int number_div = (number_objs+4)/5;
        if (number_div!=1) {
            math::func::shuffle(random_show);
        }
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            _panel->addChild(object, 2);
            object->onShow((random_show[i]/number_div)*0.1, itemshow);
        }
    }, dt + 0.5, "show_object_in_group");
}

void Math_Group::onHide(float dt, math::animation::ObjectShowType revert_show, math::animation::ObjectShowType revert_itemshow) {
    
    int number_objs = (int)_objects.size();
    std::vector<int> random_hide;
    for (int i=0; i<number_objs; i++) {
        random_hide.push_back(i);
    }
    int number_div = (number_objs+4)/5;
    if (number_div!=1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide((random_hide[i]/number_div)*0.1 + dt, revert_itemshow);
    }
    
    this->stopAllActions();
    scheduleOnce([=](float){
        _txt_name->runAction(cocos2d::FadeOut::create(0.35));
        _txt_value->runAction(cocos2d::FadeOut::create(0.35));
        int delta=100;
        auto cposition = this->getPosition();

        switch (revert_show) {
            case math::animation::No_Effect :
            {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
                break;
            case math::animation::ScaleUp :
            {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0))), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
                break;
            case math::animation::SlideUp :
            {
                auto position = cocos2d::Vec2(cposition.x, cposition.y - delta);
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
                
            }
                break;
            case math::animation::SlideDown :
            {
                auto position = cocos2d::Vec2(cposition.x, cposition.y + delta);
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
                
            }
                break;
            case math::animation::SlideLeft :
            {
                auto position = cocos2d::Vec2(cposition.x + delta, cposition.y);
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
                
            }
                break;
            case math::animation::SlideRight :
            {
                auto position = cocos2d::Vec2(cposition.x - delta, cposition.y);
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
                break;
            default:
            {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::create(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::ScaleTo::create(0.5, 0), cocos2d::RotateTo::create(0.5, 180), NULL), cocos2d::CallFunc::create([=](){
                    _panel->removeAllChildren();
                    _locations.clear();
                    _frames.clear();
                    _objects.clear();
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
                break;
        }
    }, 0.5, "hide_group");
}

int Math_Group::getId() {
    return _id;
}

void Math_Group::enableToch(math::callback::onTouchObjectBegan func) {
    _button->setVisible(true);
    _callback_func=func;
}

void Math_Group::setObjectSize(cocos2d::Size size) {
    _object_size=size;
}

void Math_Group::setGroupSize(cocos2d::Size size) {
    this->setContentSize(size);
    
    _background->setContentSize(size);
    _foreground->setContentSize(size);
    _check_touch->setContentSize(size);
    _panel->setContentSize(size);
    _button->setContentSize(size);
    
    updatePosition(false, false);
}

void Math_Group::setGroupQueue(cocos2d::Size size) {
    
    _group_type=math::group::queue;
    _group_align=math::group::left;
    _object_size= _object_size * 1.2;
    int number_in_line = size.width / _object_size.width;
    
    int mod = _group_cap % number_in_line;
    if (mod>0) {
        _group_cap+=(number_in_line-mod);
    }
    int number_row = _group_cap/number_in_line;
    float padding = (size.width - number_in_line*_object_size.width);
    auto cal_size = cocos2d::Size(size.width, number_row*_object_size.height + padding);
    if (cal_size.height < size.height) {
        cal_size = size;
    }
    
    _frame_col = number_in_line;
    _frame_row = (int)((cal_size.height - padding) / _object_size.height);
    this->setGroupSize(cal_size);
    _locations.clear();
    
    float start_x = (size.width - _object_size.width*number_in_line)/2 + _object_size.width/2;
    float start_y= (number_row-0.5)*_object_size.height + padding/2;
    for (int i=0; i<_group_cap; i++) {
        float pos_x = start_x + (i%number_in_line)*_object_size.width;
        float pos_y = start_y - (i/number_in_line)*_object_size.height;

        cocos2d::Vec2 position = cocos2d::Vec2(pos_x, pos_y);
        _locations.push_back(position);
    }
    updatePosition(false, false);
}

void Math_Group::setGroupFrames(int column, int row, math::resource::Image* img, math::resource::Image* bg_img) {
    _group_cap=column*row;
    _frame_col=column;
    _frame_row=row;
    
    _group_type=math::group::frame;
    _group_align=math::group::left;
    _object_size= _object_size * 1.2;
    
    int delta=1;
    int padding = 0;
    if (bg_img != nullptr) {
        padding = 15;
        setBackground(bg_img, 0.3);
    }

    this->setGroupSize(cocos2d::Size(column*_object_size.width + padding*2, row*_object_size.height + padding*2));
    _locations.clear();
    
    for (int i=0; i<_group_cap; i++) {
        cocos2d::ui::ImageView* frame;
        if (i>=_frames.size()) {
            frame=cocos2d::ui::ImageView::create(img->path);
            _frames.push_back(frame);
            _panel->addChild(frame, 0);
        } else {
            frame=_frames[i];
            frame->loadTexture(img->path);
        }
        frame->setScale9Enabled(true);
        auto render_size = frame->getContentSize();
        frame->setCapInsets(cocos2d::Rect(render_size.width*0.3, render_size.height*0.3, render_size.width*0.4, render_size.height*0.4));
        frame->setContentSize(cocos2d::Size(_object_size.width-delta, _object_size.height-delta));
        auto position = cocos2d::Vec2(padding + (i%_frame_col)*(_object_size.width), padding + ((_group_cap-i-1)/_frame_col)*(_object_size.height))+_object_size/2;
        
        frame->setPosition(position);
        _locations.push_back(position);
    }
    
    updatePosition(false, false);
}

void Math_Group::setAlignObjects(math::group::GroupAlign align) {
    if (_group_type == math::group::custom) {
        return;
    }

    _locations.clear();
    float padding = (getContentSize().width - _object_size.width*_frame_col)/2;
    for (int i=0; i<_group_cap; i++) {
        cocos2d::Vec2 point = cocos2d::Vec2(padding + (i%_frame_col)*(_object_size.width), padding + ((_group_cap-i-1)/_frame_col)*(_object_size.height))+_object_size/2;
        _locations.push_back(point);
    }
    
    if (align == math::group::random) {
        math::func::shuffle(_locations);
    } else {
        if (align == math::group::right) {
            std::vector<cocos2d::Vec2> locs = _locations;
            _locations.clear();
            for (int i1=0; i1<_frame_row; i1++) {
                for (int i2 = _frame_col-1; i2>=0; i2--) {
                    int index = i1*_frame_col + i2;
                    if (index >= locs.size()) {
                        continue;
                    }
                    _locations.push_back(locs[index]);
                }
            }
        }
    }
    for (int i=0; i<_objects.size(); i++) {
        _objects[i]->setPosition(_locations[i]);
    }
    _group_align=align;
}

void Math_Group::setArrangeObjects(math::group::GroupArange arrange)
{
    if (arrange == _group_arange) {
        return;
    }
    _group_arange = arrange;
    if (_group_arange == math::group::mix) {
        math::func::shuffle(_objects);
        updatePosition(false, false);
    } else {
        if (_group_type == math::group::custom || _group_align == math::group::random) {
            _group_arange = math::group::mix;
        } else {
            if (cocos2d::random(0, 1) == 0) {
                std::sort(_objects.begin(), _objects.end(), [=](Math_Object* a, Math_Object* b){
                    return a->getComparisionValue().id < b->getComparisionValue().id;
                });
            } else {
                std::sort(_objects.begin(), _objects.end(), [=](Math_Object* a, Math_Object* b){
                    return a->getComparisionValue().id > b->getComparisionValue().id;
                });
            }
            updatePosition(false, false);
        }
    }
}

void Math_Group::updatePosition(bool no_free, bool animated) {
    if (no_free || _objects.empty()) {
        return;
    }
    if (_group_type == math::group::custom) {
        if (_custom_position) {
            for (int i=0; i<_objects.size(); i++) {
                _objects[i]->setPosition(_locations[i]);
            }
        } else {
            float delta = 1.0;
            int count_rand = 0;

            for (int i=0; i<_objects.size(); i++) {
                if (count_rand>=10) {
                    count_rand=0;
                    delta-=0.1;
                }
                cocos2d::Vec2 point;
                if (i<_locations.size()) {
                    point = _locations[i];
                } else {
                    bool avail=true;
                    int count=0;
                    do {
                        count++;
                        if (count>50) {
                            break;
                        }
                        
                        avail=true;
                        int rand_x = cocos2d::random(0, (int)(this->getContentSize().width-_object_size.width*1.4)) + (int)_object_size.width*0.7;
                        int rand_y = cocos2d::random(0, (int)(this->getContentSize().height-_object_size.height*1.4)) + (int)_object_size.height*0.7;
                        point=cocos2d::Vec2(rand_x, rand_y);
                        
                        for (cocos2d::Vec2 _point : _locations) {
                            if (point.distance(_point)< _object_size.width*delta) {
                                avail=false;
                                break;
                            }
                        }
                        if (_touch_type == math::group::non_trans && avail) {
                            avail = Shape2DHitUtility::touchOnImage(_touch_src->path, this->getContentSize(), point);
                        }
                        
                    } while (!avail);
                    if (count>50) {
                        i=-1;
                        _locations.clear();
                        count_rand++;
                        continue;
                    } else {
                        _locations.push_back(point);
                    }
                }
                
                if (animated) {
                    _objects[i]->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, point), 0.5));
                } else {
                    _objects[i]->setPosition(point);
                }
            }
        }
    } else {
        _locations.clear();
        float padding = (getContentSize().width - _object_size.width*_frame_col)/2;
        for (int i=0; i<_group_cap; i++) {
            cocos2d::Vec2 point = cocos2d::Vec2(padding + (i%_frame_col)*(_object_size.width), padding + ((_group_cap-i-1)/_frame_col)*(_object_size.height))+_object_size/2;
            _locations.push_back(point);
        }
        
        if (_group_align == math::group::random) {
            math::func::shuffle(_locations);
        } else {
            if (_group_align == math::group::right) {
                std::vector<cocos2d::Vec2> locs = _locations;
                _locations.clear();
                for (int i1=0; i1<_frame_row; i1++) {
                    for (int i2 = _frame_col-1; i2>=0; i2--) {
                        int index = i1*_frame_col + i2;
                        if (index >= locs.size()) {
                            continue;
                        }
                        _locations.push_back(locs[index]);
                    }
                }
            }
        }
        
        if (_group_arange == math::group::mix) {
            for (int i=0; i<_objects.size(); i++) {
                if (animated) {
                    _objects[i]->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, _locations[i]), 0.5));
                } else {
                    _objects[i]->setPosition(_locations[i]);
                }
            }
        } else {
            auto id_object = _objects.front()->getImageSource()->code;
            int current_col = -1;
            int current_row = 0;
            for (int i=0; i<_objects.size(); i++) {
                auto object = _objects[i];
                
                if (object->getImageSource()->code == id_object) {
                    current_col++;
                    if (current_col >= _frame_col) {
                        current_col = 0;
                        current_row++;
                    }
                } else {
                    id_object = object->getImageSource()->code;
                    current_col=0;
                    current_row++;
                }
                auto point = _locations[current_row*_frame_col+current_col];
                if (animated) {
                    _objects[i]->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, point), 0.5));
                } else {
                    _objects[i]->setPosition(point);
                }
            }
            
        }
        
    }
}


#pragma mark - Ground
cocos2d::ui::ImageView* Math_Group::getBackground() {
    return _background;
}

void Math_Group::setBackground(math::resource::Image* img, float padding) {
    if (img!=NULL) {
        _bg_src = img;
        _bg_src->used = true;
        _background->loadTexture(img->path);
        _background->setScale9Enabled(true);
        if (padding == 0) {
            _background->setCapInsets(cocos2d::Rect(1, 1, _background->getVirtualRendererSize().width-2, _background->getVirtualRendererSize().height-2));
        } else {
            if (padding > 0.5) {
                padding = 0.45;
            }
            _background->setCapInsets(cocos2d::Rect(_background->getVirtualRendererSize().width * padding, _background->getVirtualRendererSize().height * padding, _background->getVirtualRendererSize().width*(1-2*padding), _background->getVirtualRendererSize().height*(1-2*padding)));
        }
        _background->setContentSize(this->getContentSize());
        _background->setVisible(true);
    }
}

void Math_Group::setForeground(math::resource::Image* img, float padding) {
    if (img!=NULL) {
        _fg_src = img;
        _fg_src->used = true;
        _foreground->loadTexture(img->path);
        _foreground->setScale9Enabled(true);
        if (padding == 0) {
            _foreground->setCapInsets(cocos2d::Rect(1, 1, _foreground->getVirtualRendererSize().width-2, _foreground->getVirtualRendererSize().height-2));
        } else {
            if (padding > 0.5) {
                padding = 0.45;
            }
            _foreground->setCapInsets(cocos2d::Rect(_foreground->getVirtualRendererSize().width * padding, _foreground->getVirtualRendererSize().height * padding, _foreground->getVirtualRendererSize().width*(1-2*padding), _foreground->getVirtualRendererSize().height*(1-2*padding)));
        }
        _foreground->setContentSize(this->getContentSize());
        _foreground->setVisible(true);
    }
}

void Math_Group::setBackgroundVisiable(bool visible) {
    _background->setVisible(visible);
}

void Math_Group::setForegroundVisiable(bool visible) {
    _foreground->setVisible(visible);
}

void Math_Group::setGroupName(std::string name, int position) {
    _txt_name->setString(name);
    float txt_height = _txt_name->getContentSize().height;
    float txt_width = _txt_name->getContentSize().width;

    if (position == 0) {
        _txt_name->setPosition(cocos2d::Vec2(getContentSize().width/2, getContentSize().height-txt_height*.65));
    } else if (position == 1) {
        _txt_name->setPosition(cocos2d::Vec2(getContentSize().width/2, txt_height*.65));
    } else if (position == 2) {
        _txt_name->setPosition(cocos2d::Vec2(txt_width/2 + 30, getContentSize().height * 0.5));
    } else {
        _txt_name->setPosition(cocos2d::Vec2(getContentSize().width - txt_width/2 - 30, getContentSize().height * 0.5));
    }
}

cocos2d::ui::Text* Math_Group::getNameText() {
    return _txt_name;
}

void Math_Group::showNameText(bool animated) {
    _txt_name->setVisible(true);
    if (animated) {
        _txt_name->setOpacity(0);
        _txt_name->runAction(cocos2d::FadeIn::create(0.35));
    }
}

void Math_Group::hideNameText(bool animated) {
    if (animated) {
        _txt_name->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.35), cocos2d::CallFunc::create([=](){
            _txt_name->setVisible(false);
            _txt_name->setOpacity(255);
        })));
    } else {
        _txt_name->setVisible(false);
        _txt_name->setOpacity(255);
    }
}


void Math_Group::setValue(std::string value) {
    _txt_value->setString(value);
    _txt_value->setTextColor(cocos2d::Color4B::ORANGE);
}

cocos2d::ui::Text* Math_Group::getValueText() {
    return _txt_value;
}

void Math_Group::showValueText(bool animated) {
    _txt_value->setVisible(true);
    if (animated) {
        _txt_value->setOpacity(0);
        _txt_value->runAction(cocos2d::FadeIn::create(0.35));
    }
}

void Math_Group::hideValueText(bool animated) {
    if (animated) {
        _txt_value->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.35), cocos2d::CallFunc::create([=](){
            _txt_value->setVisible(false);
            _txt_value->setOpacity(255);
        })));
    } else {
        _txt_value->setVisible(false);
        _txt_value->setOpacity(255);
    }
}

#pragma mark - Animation
void Math_Group::onTouchedAnimation(bool correct) {
    if (correct) {
        audio_helper->play("group_correct.mp3");
        
        auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/click1/click1.gaf", m_theme->getThemePath().c_str()));
        auto anim = gaf_asset->createObjectAndRun(false);
        anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        anim->setPosition(this->getPosition());
        this->getParent()->addChild(anim, 20);
        
        auto img_bg = this->getBackground();
        img_bg->setColor(cocos2d::Color3B(191, 227, 149));
        this->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.2, 1.2)), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.25, 1)), cocos2d::CallFunc::create([=](){
            img_bg->setColor(cocos2d::Color3B::WHITE);
        }), NULL));
        
        scheduleOnce([=](float){
            auto gaf_asset2 = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/tick/tick.gaf", m_theme->getThemePath().c_str()));
            auto anim2 = gaf_asset2->createObjectAndRun(false);
            anim2->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            anim2->setPosition(this->getPosition());
            this->getParent()->addChild(anim2, 20);
        }, 0.2, cocos2d::StringUtils::format("remove group anim %d", _id));
        
    } else {
        audio_helper->play("group_wrong.mp3");

        this->setColor(cocos2d::Color3B(253, 192, 193));
        auto shake=cocos2d::Sequence::create(cocos2d::MoveBy::create(0.15, cocos2d::Vec2(8, 0)), cocos2d::MoveBy::create(0.15, cocos2d::Vec2(-8, 0)), NULL);
        this->runAction(cocos2d::Sequence::create(cocos2d::Repeat::create(shake, 2), cocos2d::CallFunc::create([=](){
            this->setColor(cocos2d::Color3B::WHITE);
        }), NULL));
    }
}


#pragma mark - Group+Object
bool Math_Group::hasObject(Math_Object *object) {
    for (Math_Object* child : _objects) {
        if (object->isSame(child)) {
            return true;
        }
    }
    return false;
}

int Math_Group::getNumberObjects() {
    return (int)_objects.size();
}

int Math_Group::getIndexOfObject(Math_Object *object) {
    for (int i=0; i<_objects.size(); i++) {
        if (object->isSame(_objects[i])) {
            return i;
        }
    }
    return -1;
}

Math_Object* Math_Group::getObjectAtIndex(int i) {
    if (i<_objects.size()) {
        return _objects[i];
    }
    return NULL;
}

bool Math_Group::removeObject(Math_Object *object, bool seed) {
    int index=0;
    for (; index<_objects.size(); index++) {
        if (object->isSame(_objects[index])) {
            break;
        }
    }
    if (index<_objects.size()) {
        _objects.erase(_objects.begin()+index);
        //remove location
        cocos2d::Vec2 obj_position = object->getWorldPosition();
        if (seed) {
            obj_position = object->getSeedPosition();
        }
        obj_position = this->convertToGroupPosition(obj_position);

        if (object->getParent()==_panel) {
            object->removeFromParentAndCleanup(false);
        }

        if (_group_type == math::group::custom) {
            for (int i=0; i < _locations.size(); i++) {
                if (_locations[i].distance(obj_position) < 10) {
                    _locations.erase(_locations.begin()+i);
                    break;
                }
            }
        }
        
        if (_group_type!=math::group::custom && _group_align != math::group::random) {
            updatePosition(false, true);
        }
        return true;
    }
    return false;
}

void Math_Group::removeAllObjects() {
    for (Math_Object* object : _objects) {
        auto position = object->getPosition();
        auto point = this->convertToWorldSpace(position);
        object->setPosition(point);
        
        if (object->getParent()==_panel) {
            object->removeFromParentAndCleanup(false);
        }
    }
    _objects.clear();
    if (_group_type == math::group::custom) {
        _locations.clear();
    }
}

bool Math_Group::pushBackObject(Math_Object *object) {
    if (hasObject(object)) {
        removeObject(object);
        return pushBackObject(object);
    }
    if (_objects.size()<_group_cap) {
        auto position= object->getWorldPosition();
        auto bounding_box=this->getBoundingBox();
        position=this->convertToGroupPosition(position);
        
        object->removeFromParentAndCleanup(false);
        object->setPosition(position);
        _panel->addChild(object, 2);
        object->addToGroup(object->getID(), this, _id);
        _objects.push_back(object);

        if (_group_align == math::group::random) {
            if (_group_type == math::group::custom) {
                // Đặt đâu nằm đấy luôn.
                float delta = _object_size.width * 1.0;
                bool avail=true;
                for (cocos2d::Vec2 loc : _locations) {
                    if (loc.distance(position) < delta) {
                        avail = false;
                        break;
                    }
                }
                //check border
                if ((position.x < _object_size.width*0.7) ||
                    (position.x > this->getContentSize().width-_object_size.width*0.7) ||
                    (position.y < _object_size.height*0.7) ||
                    (position.y > this->getContentSize().height-_object_size.height*0.7)) {
                    avail = false;
                }
                if (_touch_type == math::group::non_trans && avail) {
                    avail = Shape2DHitUtility::touchOnImage(_touch_src->path, this->getContentSize(), position);
                }
                if (avail) {
                    _locations.push_back(position);
                    object->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, position), 0.5));
                    return true;
                }
                
                int count=0;
                do {
                    count++;
                    if (count>50) {
                        delta-=5;
                        count = 0;
                        continue;
                    }
                    
                    avail=true;
                    int rand_x = cocos2d::random(0, (int)(this->getContentSize().width-_object_size.width*1.4)) + (int)_object_size.width*0.7;
                    int rand_y = cocos2d::random(0, (int)(this->getContentSize().height-_object_size.height*1.4)) + (int)_object_size.height*0.7;
                    position=cocos2d::Vec2(rand_x, rand_y);
                    
                    for (cocos2d::Vec2 _point : _locations) {
                        if (position.distance(_point)< delta) {
                            avail=false;
                            break;
                        }
                    }
                    if (_touch_type == math::group::non_trans && avail) {
                        avail = Shape2DHitUtility::touchOnImage(_touch_src->path, this->getContentSize(), position);
                    }
                    
                } while (!avail);
                
                _locations.push_back(position);
                object->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, position), 0.5));
                return true;
                
            } else {
                //Find position?
                //
                std::vector<cocos2d::Vec2> _places = _locations;
                std::sort(_places.begin(), _places.end(), [=](cocos2d::Vec2 a, cocos2d::Vec2 b)->bool{
                    return a.distance(position) < b.distance(position);
                });
                for (cocos2d::Vec2 point : _places) {
                    bool avail = true;
                    for (Math_Object * _obj : _objects) {
                        if (point.distance(_obj->getPosition())<_object_size.width/2) {
                            avail=false;
                            break;
                        }
                    }
                    if (avail) {
                        object->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, point), 0.5));
                        break;
                    }
                }
            }
        } else {
            updatePosition(false, true);
        }
        return true;
    }
    onFullWarning();
    return false;
}

Math_Object* Math_Group::popBackObject() {
    Math_Object* object=_objects.back();
    _objects.pop_back();
    
    //remove location
    cocos2d::Vec2 obj_position = object->getWorldPosition();
    obj_position = this->convertToGroupPosition(obj_position);
    for (int i=0; i < _locations.size(); i++) {
        if (_locations[i].distance(obj_position) < 10) {
            _locations.erase(_locations.begin()+i);
            break;
        }
    }
    
    //remove in position
    if (object->getParent()==_panel) {
        object->removeFromParentAndCleanup(false);
    }
    
    if (_group_type!=math::group::custom && _group_align != math::group::random) {
        updatePosition(false, true);
    }

    return object;
}

bool Math_Group::pushFrontObject(Math_Object *object) {
    if (hasObject(object)) {
        removeObject(object);
        return pushFrontObject(object);
    }
    if (_objects.size()<_group_cap) {
        object->removeFromParentAndCleanup(false);
        auto bounding_box=this->getBoundingBox();
        auto position=this->convertToGroupPosition(object->getPosition());
        object->setPosition(position);
        _panel->addChild(object, 2);
        object->addToGroup(object->getID(), this, _id);

        _objects.insert(_objects.begin(), object);

        if (_group_align == math::group::random) {
            if (_group_type == math::group::custom) {
                // Đặt đâu nằm đấy luôn.
                _locations.push_back(position);
            } else {
                //Find position?
                //
                std::vector<cocos2d::Vec2> _places = _locations;
                std::sort(_places.begin(), _places.end(), [=](cocos2d::Vec2 a, cocos2d::Vec2 b)->bool{
                    return a.distance(position) < b.distance(position);
                });
                for (cocos2d::Vec2 point : _places) {
                    bool avail = true;
                    for (Math_Object * _obj : _objects) {
                        if (point.distance(_obj->getPosition())<_object_size.width/2) {
                            avail=false;
                            break;
                        }
                    }
                    if (avail) {
                        object->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, point), 0.5));
                        break;
                    }
                }
            }
        } else {
            updatePosition(false, true);
        }
        return true;
    }
    onFullWarning();
    return false;
}

Math_Object* Math_Group::popFrontObject() {
    Math_Object* object=_objects.front();
    _objects.erase(_objects.begin());
    
    //remove in position
    if (object->getParent()==_panel) {
        object->removeFromParentAndCleanup(false);
    }
    
    if (_group_type!=math::group::custom && _group_align != math::group::random) {
        updatePosition(false, true);
    }
    
    return object;
}

bool Math_Group::addObject(Math_Object *object, cocos2d::Vec2 point) {
    if (_objects.size()<_group_cap) {
        if (_group_type!=math::group::custom) {
            object->removeFromParentAndCleanup(false);
            auto position=this->convertToGroupPosition(point);
            
            object->setPosition(position);
            _panel->addChild(object, 2);
            object->addToGroup(object->getID(), this, _id);
            
            if (_group_align == math::group::random) {
                //Find position?
                std::vector<cocos2d::Vec2> _places = _locations;
                std::sort(_places.begin(), _places.end(), [=](cocos2d::Vec2 a, cocos2d::Vec2 b)->bool{
                    return a.distance(position) < b.distance(position);
                });
                for (cocos2d::Vec2 pt : _places) {
                    bool avail = true;
                    for (Math_Object * _obj : _objects) {
                        if (pt.distance(_obj->getPosition())<_object_size.width/2) {
                            avail=false;
                            break;
                        }
                    }
                    if (avail) {
                        object->runAction(cocos2d::EaseOut::create(cocos2d::MoveTo::create(0.1, pt), 0.5));
                        break;
                    }
                }
                _objects.push_back(object);
            } else {
                _objects.push_back(object);
                updatePosition(false, true);
            }
            return true;
        } else {
            return pushBackObject(object);
        }
    } else {
        onFullWarning();
        return false;
    }
}

void Math_Group::swapObjects(Math_Object *object1, Math_Object *object2) {
    
    //Swap in list object
    int index_1=-1;
    int index_2=-1;
    for (int i=0; i<_objects.size(); i++) {
        if (object1->isSame(_objects[i])) {
            index_1=i;
        }
        if (object2->isSame(_objects[i])) {
            index_2=i;
        }
        if (index_1*index_2>=0) {
            break;
        }
    }
    std::iter_swap(_objects.begin()+index_1, _objects.begin()+index_2);
    
    //Swap position
    auto point_1=object1->getPosition();
    auto point_2=object2->getPosition();
    object1->runAction(cocos2d::JumpTo::create(0.25, point_2, _object_size.height, 1));// MoveTo::create(0.25, point_2));
    object2->runAction(cocos2d::JumpTo::create(0.25, point_1, -_object_size.height, 1));//  MoveTo::create(0.25, point_1));
}


#pragma mark - Control
bool Math_Group::isTouchOnGroup(cocos2d::Vec2 point) {
    auto bounding_box=this->getBoundingBox();
    //auto parent=this->getParent();
    auto point2 = point;//parent->convertToNodeSpace(point);
    
    if (_touch_type == math::group::box) {
        //Fix bounding box
        float delta_w = _object_size.width*.25;
        float delta_h = _object_size.height*.25;
        bounding_box.setRect(bounding_box.origin.x + delta_w, bounding_box.origin.y + delta_h, bounding_box.size.width - delta_w*2, bounding_box.size.height - delta_h*2);
        return bounding_box.containsPoint(point2);
    } else {
        if (bounding_box.containsPoint(point2)) {
            auto point3=point2-this->getBoundingBox().origin;
            return Shape2DHitUtility::touchOnImage(_touch_src->path, this->getContentSize(), point3);
        }
        return false;
    }
}

cocos2d::Vec2 Math_Group::convertToGroupPosition(cocos2d::Vec2 point) {
    auto bounding_box=this->getBoundingBox();
    //auto parent=this->getParent();
    auto point2=_parent->convertToNodeSpace(point);
    return point2-bounding_box.origin;
}

void Math_Group::onAddObjectSuccess(Math_Object *object) {
    
}

#pragma mark - Compare
void Math_Group::setComparisionValue(math::comparision::Value compare_value) {
    _compare_value=compare_value;
}

void Math_Group::setComparisionKey(math::comparision::Key compare_key) {
    _compare_key=compare_key;
}

bool Math_Group::isMatchWithGroup(Math_Object *object) {
//    if (_compare_key==math::comparision::number) {
//        return true;
//    }
    return object->isEqual(_compare_value, _compare_key);
}

bool Math_Group::isMatchGroup() {
    for (Math_Object* object : _objects) {
        if (!isMatchWithGroup(object)) {
            return false;
        }
    }
    return true;
}

bool Math_Group::isMatchWithOther(Math_Object *object) {
//    if (_compare_key==math::comparision::number) {
//        return true;
//    }
    if (_objects.size()>0) {
        return object->isEqual(_objects[0], _compare_key);
    }
    CCLOG("group has no object!");
    return true;
}

bool Math_Group::isMatchGroupWithOther() {
    for (Math_Object* object : _objects) {
        if (!isMatchWithOther(object)) {
            return false;
        }
    }
    return true;
}

#pragma mark - testing
void Math_Group::onTouchOnGroup() {
    if (_callback_func) {
        _callback_func(this);
    }
}

#pragma mark -
void Math_Group::onFullWarning() {
    if (_objects.empty()) {
        return;
    }
    float object_scale = _objects[0]->getScale();
    for (Math_Object* object : _objects) {
        object->runAction(cocos2d::Repeat::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.1, object_scale*1.1), cocos2d::ScaleTo::create(0.1, object_scale)), 2));
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback Math_Group::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(Math_Group, onGroupClicked),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void Math_Group::onGroupClicked(cocos2d::Ref *) {
    if (_callback_func) {
        _callback_func(this);
    }
}

#pragma mark - Code ver 2.1
void Math_Group::setCheckTouchImage(math::resource::Image *img) {
    if (img!=NULL) {
        _group_type = math::group::custom;
        _group_align = math::group::random;
        _touch_type = math::group::non_trans;
        
        _touch_src=img;
        _touch_src->used = true;
        float padding = 1;
        
        _check_touch->loadTexture(img->path);
        cocos2d::Image* image=new cocos2d::Image();
        image->initWithImageFile(img->path);
        _check_touch->setCapInsets(cocos2d::Rect(padding, padding, image->getWidth()-2*padding, image->getHeight()-2*padding));
        _check_touch->setScale9Enabled(true);
        _check_touch->setContentSize(this->getContentSize());
        _check_touch->setVisible(false);
        
        updatePosition(false, false);
    }
}

void Math_Group::setVisibleCheckTouchImage(bool visible) {
    _check_touch->setVisible(visible);
}


#pragma mark - Code ver 2.1.1
cocos2d::Vec2 Math_Group::getPositionAtFrame(int col, int row) {
    if (_group_type == math::group::frame) {
        int index = (col>(_frame_col-1)?(_frame_col-1):col) + (row>=(_frame_row-1)?(_frame_row-1):row) * _frame_col;
        return _locations[index];
    }
    return cocos2d::Vec2::ZERO;
}

cocos2d::Vec2 Math_Group::getPositionAtIndex(int index) {
    if (_group_type != math::group::custom) {
        if (index < _locations.size()) {
            return _locations[index];
        }
    }
    return cocos2d::Vec2::ZERO;
}

void Math_Group::addBackObject(Math_Object *object) {
    auto position=this->convertToGroupPosition(object->getWorldPosition());
    object->removeFromParentAndCleanup(false);
    
    _panel->addChild(object, 2);
    object->setPosition(position);
    object->addToGroup(object->getID(), this, _id);

    _objects.push_back(object);
}

void Math_Group::setCustomPosition(std::vector<cocos2d::Vec2> positions) {
    _group_cap = (int)positions.size();
    _locations = positions;
    _custom_position = true;
    updatePosition(false, false);
}

void Math_Group::removeCustomPosition() {
    _custom_position=false;
    _locations.clear();
    updatePosition(false, false);
}


void Math_Group::setQueueGroup(int cap, math::resource::Image* bg_img) {
    _group_cap = cap;
    while (_objects.size() > cap) {
        _objects.pop_back();
    }
    
    _group_type=math::group::queue;
    _group_align=math::group::left;
    
    int delta=2;
    int padding = 0;
    if (bg_img != NULL) {
        padding = _object_size.width * 0.3;
        setBackground(bg_img);
    }
    
    this->setGroupSize(cocos2d::Size(cap*_object_size.width + padding*2, _object_size.height + padding*2));
    
    _locations.clear();
    for (int i=0; i<_group_cap; i++) {
        cocos2d::Vec2 point = cocos2d::Vec2(padding + (i%_frame_col)*(_object_size.width+delta), padding + _object_size.height+delta)+_object_size/2;
        _locations.push_back(point);
        _objects[i]->setPosition(point);
    }
}

void Math_Group::setQueueGroupWithLists(std::vector<Math_Object *> list, std::vector<Math_Object *> list2) {

}


void Math_Group::setItemsAlign (ItemAlign align) {
    if (_group_type != math::group::custom) {
        cocos2d::Size cell_size = _object_size/1.2;
        if (align == ItemAlign::ItemAlignBottom) {
            for (Math_Object* object : _objects) {
                auto position = object->getPosition();
                auto item_size = object->getContentSize();
                if (item_size.height < cell_size.height) {
                    object->setPosition(cocos2d::Vec2(position.x, position.y - (cell_size.height-item_size.height)/2));
                }
            }
        } else if (align == ItemAlign::ItemAlignTop) {
            for (Math_Object* object : _objects) {
                auto position = object->getPosition();
                auto item_size = object->getContentSize();
                if (item_size.height < cell_size.height) {
                    object->setPosition(cocos2d::Vec2(position.x, position.y + (cell_size.height-item_size.height)/2));
                }
            }
        } else if (align == ItemAlign::ItemAlignLeft) {
            for (Math_Object* object : _objects) {
                auto position = object->getPosition();
                auto item_size = object->getContentSize();
                if (item_size.width < cell_size.width) {
                    object->setPosition(cocos2d::Vec2(position.x - (cell_size.width-item_size.width)/2, position.y));
                }
            }
        } else if (align == ItemAlign::ItemAlignRight) {
            for (Math_Object* object : _objects) {
                auto position = object->getPosition();
                auto item_size = object->getContentSize();
                if (item_size.width < cell_size.width) {
                    object->setPosition(cocos2d::Vec2(position.x + (cell_size.width-item_size.width)/2, position.y));
                }
            }
        }
    }
}

//Set color
void Math_Group::setGroupBackgroungColor(cocos2d::Color3B color) {
    if (_background!=nullptr) {
        _background->setColor(color);
    }
}

void Math_Group::setGroupForegroungColor(cocos2d::Color3B color) {
    if (_foreground!=nullptr) {
        _foreground->setColor(color);
    }
}

void Math_Group::setGroupBackgroungOpacity(GLubyte opac) {
    if (_background!=nullptr) {
        _background->setOpacity(opac);
    }
}

void Math_Group::setGroupForegroungOpacity(GLubyte opac) {
    if (_foreground!=nullptr) {
        _foreground->setOpacity(opac);
    }
}

void Math_Group::setSuperNode(cocos2d::Node * node) {
    super_node = node;
    for (Math_Object* object : _objects) {
        object->setSuperNode(node);
    }
}


void Math_Group::clippedBackgroundEdge(int edge) {
    cocos2d::DrawNode* stencil = cocos2d::DrawNode::create();
    stencil->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(getContentSize().width, getContentSize().height), cocos2d::Color4F::WHITE);

    cocos2d::ClippingNode* clipper = cocos2d::ClippingNode::create();
    clipper->setStencil(stencil);
    clipper->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(clipper, -1);
    
    float delta = 100;
    if (edge == 0) {
        //Right
        _background->setContentSize(cocos2d::Size(getContentSize().width+delta, getContentSize().height));
    } else if (edge == 1) {
        //Left
        _background->setContentSize(cocos2d::Size(getContentSize().width+delta, getContentSize().height));
        _background->setPosition(cocos2d::Vec2(_background->getPosition().x-delta, _background->getPosition().y));
    } else if (edge == 2) {
        //Right
        _background->setContentSize(cocos2d::Size(getContentSize().width, getContentSize().height+delta));
    } else if (edge == 3) {
        //Left
        _background->setContentSize(cocos2d::Size(getContentSize().width, getContentSize().height+delta));
        _background->setPosition(cocos2d::Vec2(_background->getPosition().x, _background->getPosition().y-delta));
    }
    
    auto new_bg = _background->clone();
    _background->setVisible(false);
    clipper->addChild(new_bg);
}
