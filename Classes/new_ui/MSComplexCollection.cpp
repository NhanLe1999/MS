//
//  MSComplexCollection.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/9/17.
//

#include "MSComplexCollection.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "APProfileManager.h"

USING_NS_CC;
INITIALIZE_READER(MSComplexCollection);

std::once_flag ms_complex_collection_reader;
MSComplexCollection * MSComplexCollection::createCollection(ms::delegate::CollectionDataSource * data_source) {
    std::call_once(ms_complex_collection_reader, [] {
        REGISTER_CSB_READER(MSComplexCollection);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/ComplexCollection.csb");
    auto collection = reinterpret_cast<MSComplexCollection *>(CSLoader::createNode(csb_name));
    if (collection) {
        collection->didLoadFromCSB(data_source);
        return collection;
    }
    
    CC_SAFE_DELETE(collection);
    return nullptr;
}

void MSComplexCollection::didLoadFromCSB(ms::delegate::CollectionDataSource * data_source) {
    assert(data_source != nullptr);
    _data_source = data_source;
    
    _min_id_showing = -1;
    _max_id_showing = -1;
    
    _top_padding = 50;
    _bottom_padding = 50;
    
    _scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    _scrollview->setScrollBarEnabled(false);
    mListIdOfLessonsLearned = PROFILE_MANAGER.getAllLessonLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
    //how many row?
    reloadData();
    
    this->setTouchEnabled(true);
    _scrollview->setPropagateTouchEvents(true);
    _scrollview->setSwallowTouches(false);
    this->addTouchEventListener([=](Ref * sender, ui::Widget::TouchEventType event) {
        switch (event) {
            case ui::Widget::TouchEventType::BEGAN:
                _time_touch_start = utils::getTimeInMilliseconds();
                _position_touch_start = this->getTouchBeganPosition();
                _should_process_click = true;
                break;
                
            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED:{
                if (!_should_process_click) {
                    return;
                }
                auto diff_time = utils::getTimeInMilliseconds() - _time_touch_start;
                auto current_position = this->getTouchEndPosition();
                if (current_position.fuzzyEquals(_position_touch_start, 5) && diff_time < 200) {
                    CCLOG("Clicked!!!!");
                    this->processClickEvent(_position_touch_start);
                }
                
                break;
            }
                
            default:
                break;
        }
    });
}

void MSComplexCollection::updatePage(float dt) {
    if (_data_source->numberOfCells() == 0) {
        return;
    }
    
//    auto bb = _scrollview->getInnerContainer()->getBoundingBox();
//    bb.origin = _scrollview->convertToWorldSpace(bb.origin);
//    CCLOG("inner bb: %f %f %f %f", bb.origin.x, bb.origin.y, bb.size.width, bb.size.height);
//    auto bb_o = _scrollview->getInnerContainerPosition();
//    bb_o = _scrollview->convertToWorldSpace(bb_o);
//    CCLOG("inner bb_o: %f %f", bb_o.x, bb_o.y);
    
//    auto scrollview_bb = _scrollview->getBoundingBox();
//    scrollview_bb.origin = _scrollview->getParent()->convertToWorldSpace(scrollview_bb.origin);
    
    //Position.y cua Inner Container cua Scroll View co the bien thanh NaN khi update khong biet tai sao
    if(!(_innerContainer->getPositionY() == _innerContainer->getPositionY()))
        _innerContainer->setPositionY(0);

    auto viewport = _scrollview_bb;
    viewport.origin = _innerContainer->convertToNodeSpace(_scrollview_bb.origin);
    
    
    auto miny = viewport.getMinY();
    auto maxy = viewport.getMaxY();
    
    auto minrow = int(miny / _model_size.height);
    minrow = _nrow - minrow;
    auto maxrow = int(maxy / _model_size.height);
    maxrow = _nrow - maxrow;
    
    std::swap(minrow, maxrow);
    
    minrow -= 2;
    maxrow += 1;
    minrow = std::max(minrow, 0);
    maxrow = std::min(maxrow, _nrow);
    
    auto minid = minrow * _ncol;
    auto maxid = maxrow * _ncol - 1;
    maxid = std::min(maxid, _ncell - 1);

    if (minid != _min_id_showing || maxid != _max_id_showing) {
        for (auto i = _min_id_showing; i <= _max_id_showing; ++i)  {
            if (i >= minid && i <=maxid) {
                continue;
            }
            _data_source->offLoadCellAt(i);
        }        
        for (auto i = minid; i <= maxid; ++i) {
            auto row = i / _ncol;
            auto col = i % _ncol;
            auto item = _data_source->cellForPosition(row, col, mListIdOfLessonsLearned);
            if (!item) {
                continue;
            }
            
            if (item->getParent()) {
                
            } else {
                row = _nrow - row - 1;
                auto position = Point(_horizontal_space + col * (_model_size.width + _horizontal_space), row * _model_size.height + _bottom_padding);
                item->setPosition(position);
                auto lesson_learned = MJDEFAULT->getBoolForKey(key_lesson_learn, false);
                _scrollview->addChild(item, lesson_learned ? -maxid : maxid - i);
                
                CCLOG("Drew Item Number %d. row %d, col %d", i, row, col);
            }
        }
    }

    
    _min_id_showing = minid;
    _max_id_showing = maxid;
}

void MSComplexCollection::reloadData() {
//    this->unscheduleUpdate();
    this->unschedule(schedule_selector(MSComplexCollection::updatePage));
    
    _model_size = _data_source->modelSize();
    
    _min_id_showing = -1;
    _max_id_showing = -1;
    _bottom_padding = 50;
    _ncell = _data_source->numberOfCells();

    this->setContentSize(_data_source->collectionSize());
    _scrollview->setContentSize(_data_source->collectionSize());
//    ui::Helper::doLayout(this);
    
    _ncol = _data_source->numberOfColumns();
    _nrow = (_data_source->numberOfCells() - 1) / _ncol + 1;
    
    _horizontal_space = (_data_source->collectionSize().width - _model_size.width * _ncol) / (_ncol + 1);
//
    
    _scrollview->removeAllChildren();
    
    auto innersize = Size(_ncol * _model_size.width, _nrow * _model_size.height + _top_padding + _bottom_padding);
    if (innersize.height < _scrollview->getContentSize().height) {
        _scrollview->setContentSize(Size(_scrollview->getContentSize().width, innersize.height));
        _scrollview->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        _scrollview->setPosition(Point(0, this->getContentSize().height));
    }
    _scrollview->setInnerContainerSize(innersize);
    _scrollview->jumpToTop();
    
    if (_data_source->shouldScrollWhenReload()) {
        _scrollview->scrollToPercentVertical(_data_source->percentScrollWhenReload(), 0.5, true);
    }
    
//    this->scheduleUpdate();
//    this->schedule(schedule_selector(MSComplexCollection::updatePage), 1.0/10, "schedule_complex_page");
    _scrollview_bb = _scrollview->getBoundingBox();
    _innerContainer = _scrollview->getInnerContainer();

    this->schedule(schedule_selector(MSComplexCollection::updatePage), 1.0 / 3);
}

void MSComplexCollection::scrollTo(float percent) {
    _scrollview->scrollToPercentVertical(percent, 0.5, true);
}

void MSComplexCollection::processClickEvent(cocos2d::Point touch_position) {
    for (auto i = _min_id_showing; i <= _max_id_showing; ++i) {
        auto row = i / _ncol;
        auto col = i % _ncol;
        auto item = _data_source->cellForPosition(row, col);
        if (item == nullptr) {
            continue;
        }
        auto bb = item->getBoundingBox();
        bb.origin = item->getParent()->convertToWorldSpace(bb.origin);
        if (bb.containsPoint(touch_position)) {
            CCLOG("Clicked item %d", i);
            _delegate ? _delegate->onSelectCollectionItem(item) : (void) nullptr;
            break;
        }
    }
}

void MSComplexCollection::runAnimationAppear() {
    this->setVisible(false);
    this->scheduleOnce([=](float) {
        this->setVisible(true);
        for (auto i = _min_id_showing; i <= _max_id_showing; ++i) {
            auto row = i / _ncol;
            auto col = i % _ncol;
            auto item = _data_source->cellForPosition(row, col);
            if (!item)
                continue;
            
            auto saved_position = item->getPosition();
            item->setPosition(saved_position + Point(200, 0));
            item->setOpacity(0);
            
            item->runAction(Sequence::create(DelayTime::create(0.025 * (i - _min_id_showing)), Spawn::create(EaseQuinticActionOut::create(MoveTo::create(0.5, saved_position)), FadeIn::create(0.5), nullptr), NULL));
        }
    }, 0.1, "delay_complex_appear");
}

void MSComplexCollection::runAnimationDisappear() {
    auto bb = this->getBoundingBox();
    bb.origin = this->getParent()->convertToWorldSpace(bb.origin);
    
    auto counter = 0;
    for (auto i = _max_id_showing; i >= _min_id_showing; --i) {
        auto row = i / _ncol;
        auto col = i % _ncol;
        auto item = _data_source->cellForPosition(row, col);
        if (!item)
            continue;
        
        auto item_bb = item->getBoundingBox();
        item_bb.origin = item->getParent()->convertToWorldSpace(item_bb.origin);
        if (!bb.intersectsRect(item_bb)) {
            continue;
        }
       
//        item->runAction(Sequence::create(DelayTime::create(0.05 * counter++), Spawn::create(EaseSineIn::create(MoveBy::create(0.5, Vec2(600, 0))), FadeOut::create(0.5), nullptr), NULL));
        item->runAction(FadeOut::create(0.5));
    }
}

bool MSComplexCollection::updatePadding() {
//    auto new_row = (_data_source->numberOfCells() - 1) / _ncol + 1;
//    if (new_row != _nrow) {
//        auto padding =  (_nrow - new_row) * _model_size.height;
//        _scrollview->setInnerContainerSize(_scrollview->getInnerContainerSize() + Size(0, -padding));
//        _scrollview->setInnerContainerPosition(_scrollview->getInnerContainerPosition() + Point(0, padding));
//        _scrollview->getInnerContainer()->setBackGroundColor(Color3B::BLACK);
//        _scrollview->getInnerContainer()->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//        _nrow = new_row;
//        return true;
//    }
    return false;
}
