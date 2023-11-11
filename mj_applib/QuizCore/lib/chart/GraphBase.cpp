//
//  GraphBase.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "GraphBase.h"

using namespace math;

GraphBase* GraphBase::createGraph() {
    auto p = new GraphBase();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool GraphBase::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    drawer = cocos2d::DrawNode::create();
    this->addChild(drawer, 2);
    return true;
}

void GraphBase::setGraphCap(int cap) {
    graph_cap = cap;
}

void GraphBase::setAnimationMode(GraphBase::AnimationMode mode) {
    _animated = mode;
}

void GraphBase::addItem(Math_Object *object, int value) {
    GraphItem item;
    item.index = (int)_graph_items.size();
    item.object = object;
    item.value = value;
    _graph_items.push_back(item);
}

bool GraphBase::removeItem(int index) {
    if (index < 0 || index >= _graph_items.size()) {
        return false;
    }
    
    for (int i=index+1; i<_graph_items.size(); i++) {
        GraphItem item = _graph_items[i];
        item.index = i-1;
    }
    
    _graph_items.erase(_graph_items.begin()+index);
    return true;
}

int GraphBase::getNumberItems() {
    return (int)_graph_items.size();
}

GraphBase::GraphItem GraphBase::getMostItem() {
    if (_graph_items.size() > 0) {
        GraphItem item = _graph_items.front();
        for (GraphItem gitem : _graph_items) {
            if (gitem.value > item.value) {
                item = gitem;
            }
        }
        return item;
    }
    return GraphItem();
}

GraphBase::GraphItem GraphBase::getLeastItem() {
    if (_graph_items.size() > 0) {
        GraphItem item = _graph_items.front();
        for (GraphItem gitem : _graph_items) {
            if (gitem.value < item.value) {
                item = gitem;
            }
        }
        return item;
    }
    return GraphItem();
}

int GraphBase::graphRange () {
    auto item_max = getMostItem();
    auto item_min = getLeastItem();
    return item_max.value - item_min.value;
}

int GraphBase::distanceBetweenTwoItems(int index_1, int index_2) {
    if (index_1 < 0 || index_1 >= _graph_items.size()) {
        return 0;
    }
    if (index_2 < 0 || index_2 >= _graph_items.size()) {
        return 0;
    }
    if (index_1 == index_2) {
        return 0;
    }
    return _graph_items[index_1].value - _graph_items[index_2].value;
}

std::vector<GraphBase::GraphItem> GraphBase::getItemsMoreThan(int value, bool include_equal) {
    std::vector<GraphItem> items;
    if (include_equal) {
        for (GraphItem item : _graph_items) {
            if (item.value >= value) {
                items.push_back(item);
            }
        }
    } else {
            for (GraphItem item : _graph_items) {
                if (item.value > value) {
                    items.push_back(item);
                }
            }
    }
    return items;
}

std::vector<GraphBase::GraphItem> GraphBase::getItemsLessThan(int value, bool include_equal) {
    std::vector<GraphItem> items;
    if (include_equal) {
        for (GraphItem item : _graph_items) {
            if (item.value <= value) {
                items.push_back(item);
            }
        }
    } else {
        for (GraphItem item : _graph_items) {
            if (item.value < value) {
                items.push_back(item);
            }
        }
    }
    return items;
}

std::vector<GraphBase::GraphItem> GraphBase::getItemsEqual(int value) {
    std::vector<GraphItem> items;
    for (GraphItem item : _graph_items) {
        if (item.value == value) {
            items.push_back(item);
        }
    }
    return items;
}

int GraphBase::numberOfItemsMoreThan(int value, bool include_equal) {
    return (int)getItemsMoreThan(value, include_equal).size();
}

int GraphBase::numberOfItemsLessThan(int value, bool include_equal) {
    return (int)getItemsLessThan(value, include_equal).size();
}

int GraphBase::numberOfItemsEqual(int value) {
    return (int)getItemsEqual(value).size();
}

void GraphBase::changeValueForItem(int index, int value) {
    if (index < 0 || index >= _graph_items.size()) {
        return;
    }
    _graph_items[index].value = value;
}

void GraphBase::increaseValueForItem(int index, int value) {
    if (index < 0 || index >= _graph_items.size()) {
        return;
    }
    _graph_items[index].value = value + _graph_items[index].value;
}

void GraphBase::setItemSize(cocos2d::Size size) {
    item_size = size;
}

void GraphBase::setTitleItem(std::string title) {
    title_item = title;
}

void GraphBase::setTitleNumber(std::string title) {
    title_number = title;
}

std::vector<GraphBase::GraphItem> GraphBase::getGraphItems() {
    return _graph_items;
}

void GraphBase::setTitleChar(bool title) {
    title_chart = title;
}

void GraphBase::show() {
    
}

void GraphBase::update() {
    
}

void GraphBase::updateItem(int index) {
    
}

void GraphBase::reload() {
    
}
