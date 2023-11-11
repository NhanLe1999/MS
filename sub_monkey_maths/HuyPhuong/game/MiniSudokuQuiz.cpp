//
//  MiniSudokuQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "MiniSudokuQuiz.h"

#define object_scale 0.7

MiniSudokuQuiz* MiniSudokuQuiz::createQuiz()
{
    MiniSudokuQuiz* quiz=new MiniSudokuQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MiniSudokuQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MiniSudokuQuiz::initQuiz(mj::model::Game game)
{
     
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    sudoku_size = getJsonInt(json_game, "size", 2);
    display_icon = getJsonBool(json_game, "display", true);
    std::string compare = getJsonString(json_game, "rule", "id");
    comparision_key = getComparisionKey(compare);
    has_block = (sudoku_size==4);
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_icon=json_data.FindMember("icon")->value;
    std::string icon_same = getJsonString(json_icon, "same", "none");
    std::string icon_diff = getJsonString(json_icon, "different", "id");
    img_list = getImageList("icons", sudoku_size, icon_diff,  icon_same);
    
    //setup background
    bg_white = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/drop.png", m_theme->getThemePath().c_str()));
    bg_gray = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/drop.png", m_theme->getThemePath().c_str()));
    bg_green = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/drop.png", m_theme->getThemePath().c_str()));
    bg_red = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%scommon/drop.png", m_theme->getThemePath().c_str()));
    
    /*---> Lay xong data cua game*/
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    switch (sudoku_size) {
        case 2:
        {
            item_size = cocos2d::Size(200, 200);
        }
            break;
        case 3:
        {
            item_size = cocos2d::Size(200, 200);
        }
            break;
        default:
        {
            item_size = cocos2d::Size(150, 150);
        }
            break;
    }
    
    for (int i=0; i<sudoku_size; i++) {
        Math_Object* object = mpool->getObject();
        object->setEnable(i, math::common::deny, img_list[i], nullptr);
        object->setObjectSize(item_size * object_scale);
        object->setMoveCallback(CC_CALLBACK_1(MiniSudokuQuiz::onMoveObjectBegan, this), CC_CALLBACK_2(MiniSudokuQuiz::onMoveObjectEnded, this));
        object_dock.push_back(object);
    }
    
    //Gen slot
    int number_slot = sudoku_size*sudoku_size;
    for (int i=0; i<number_slot; i++) {
        i_sudoku.push_back(-1);
        Math_Slot* slot = mpool->getSlot();
        slot->setEndble(i, bg_white);
        slot->setSlotSize(item_size);
        _slots.push_back(slot);
    }
    
    this->sudokuGenerate();
}

void MiniSudokuQuiz::onStart() {
    
    float bar_w = 300/quiz_size.width;
    onShowRightBar(0, bar_w, CC_CALLBACK_1(MiniSudokuQuiz::checkSudokuDone, this));
    
    Math_Quiz::onStart();
    
    //object in dock
    float delta_1 = (quiz_size.height - 180 - sudoku_size * item_size.width * object_scale) / (sudoku_size + 1);
    delta_1 = delta_1 > 100 ? 100 : delta_1;
    math::func::smartSetPosition(object_dock, item_size*object_scale, cocos2d::Rect(50, 150, 200, quiz_size.height-180), delta_1, false);
    
    float delay = 0.0;
    for (Math_Object* object : object_dock) {
        addChildNodeToRightPanel(object);
        object->onShow(0.5 + delay, math::animation::SlideLeft);
        delay += 0.1;
    }
    
    auto delta = 5.0;
    auto start_x = (quiz_size.width - 300 - sudoku_size * item_size.width - (sudoku_size-1)*delta) * 0.5 + item_size.width * 0.5;
    auto start_y = (quiz_size.height - getQuestion()->getContentSize().height - 20 + sudoku_size * item_size.height + (sudoku_size-1)*delta) * 0.5 - item_size.height * 0.5;

    for (int i=0; i<_slots.size(); i++) {
        auto slot = _slots[i];
        slot->setPosition(cocos2d::Vec2(start_x +(i % sudoku_size) * (item_size.width + delta), start_y - (i/sudoku_size) * (item_size.height + delta)));
        addChildNode(slot);
        slot->onShow((i/sudoku_size + (i % sudoku_size))*0.1, math::animation::SlideUp);
    }
}

void MiniSudokuQuiz::startQuestion() {
    
    scheduleOnce([=](float){
        for (Math_Object* object : object_dock) {
            object->enableDrag(_panel_right);
        }
        sudokuSuggestion(m_sudoku);
        this->suggestSudoku();
    }, 1.0, "start quiz");
    
}

void MiniSudokuQuiz::onExplanation(bool correct) {
     
    scheduleOnce([=](float dt){
        if (!correct) {
            
        } else {
            this->onComplete();
        }
    }, 1.0, "selected_anim");

}

void MiniSudokuQuiz::onComplete() {
     
    //TODO: Thuc hien them cac lenh o day
    for (int i=0; i<_slots.size(); i++) {
        auto slot = _slots[i];
        slot->onHide((i/sudoku_size + (i % sudoku_size))*0.1, math::animation::SlideUp);
    }
    
    for (int i=0; i<object_dock.size(); i++) {
        object_dock[i]->onHide(i*0.1, math::animation::SlideLeft);
    }

    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.0, "complete_quiz");
}


#pragma mark - object touch events callback
void MiniSudokuQuiz::onMoveObjectBegan(cocos2d::Node* node)
{
    Math_Object*object=(Math_Object*)node;
    start_dock = false;
    int i=0;
    for (Math_Object* obj : object_dock) {
        if (object->isSame(obj)) {
            start_dock = true;
            object_dock.erase(object_dock.begin() + i);
            break;
        }
        i++;
    }
    
    if (!start_dock) {
        for (Math_Slot* slot : _slots) {
            if (object->isSame(slot->getLinker())) {
                slot->removeLinker();
                i_sudoku.erase(i_sudoku.begin()+slot->getId());
                i_sudoku.insert(i_sudoku.begin()+slot->getId(), -1);
                slot->setBackground(bg_white);
                checkSudokuAtPoint(-1, slot->getId());
                break;
            }
        }
    } else {
        auto cloned = object->clone();
        cloned->onShow(0, math::animation::No_Effect);
        object_dock.push_back(cloned);
    }
    onBeganDragSudoku(object->getID());
}


void MiniSudokuQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
    Math_Object*object=(Math_Object*)node;
    
    int index_slot = -1;
    bool add_object = false;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            if (slot->isUsed()) {
                auto linker = slot->getLinker();
                if (linker->isLocked()) {
                    break;
                }
                
                slot->removeLinker();
                linker->setPosition(cocos2d::Vec2(-1000, 0));
                linker->onHide(0);
            }
            
            object->removeFromParentAndCleanup(false);
            addChildNode(object, 10);
            object->runAction(cocos2d::MoveTo::create(0.15, slot->getPosition()));
            slot->setLinker(object);
            
            index_slot = slot->getId();
            add_object = true;
            break;
        }
    }
    
    if (!add_object) {
        object->onHide(0, math::animation::ScaleUpAndRotation);
    }

    //check sudoku
    if (add_object) {
        i_sudoku.erase(i_sudoku.begin()+index_slot);
        i_sudoku.insert(i_sudoku.begin()+index_slot, object->getID());
    } else {
        object->onHide(0, math::animation::ScaleUpAndRotation);
    }
    onEndedDragSudoku(object->getID());
}

void MiniSudokuQuiz::checkSudokuDone(cocos2d::Node* node) {
    auto answer = static_cast<Math_Answer*>(node);
    
    bool done = true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed()) {
            done = false;
            answer->onAnimationClick(done);
            return;
        }
    }
    
    done = checkSudoku(i_sudoku);
    answer->onAnimationClick(done);
    if (done) {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->onExplanation(true);
        }, 1.0, "complete");
    } else {
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed()) {
                auto linker = slot->getLinker();
                if (!linker->isLocked()) {
                    slot->removeLinker();
                    linker->onHide(0, math::animation::ScaleUpAndRotation);
                    i_sudoku.erase(i_sudoku.begin() + slot->getId());
                    i_sudoku.insert(i_sudoku.begin() + slot->getId(), -1);
                }
            }
        }

    }
    
}

#pragma mark - Sudoku
#pragma mark -
void MiniSudokuQuiz::sudokuGenerate()
{
    if (sudoku_size==4) {
        has_block=true;
        int index_block=sqrt(sudoku_size);
        for (int i=0; i<sudoku_size; i++) {
            std::vector<int> iblock;
            iblocks.push_back(iblock);
        }
        int all_slot=sudoku_size*sudoku_size;
        
        for (int i=0; i<all_slot; i++) {
            int kx=i%sudoku_size;
            int ky=i/sudoku_size;
            
            int index_x=kx/index_block;
            int index_y=ky/index_block;
            
            int ib=index_y*index_block+index_x;
            iblocks[ib].push_back(i);
        }
    }
    
    do {
        m_sudoku.clear();
        for (int i=0; i<sudoku_size; i++) {
            std::vector<int> sudoku_row;
            for (int k=0; k<sudoku_size; k++) {
                sudoku_row.push_back(k);
            }
            random_shuffle(sudoku_row.begin(), sudoku_row.end());
            for (int k=0; k<sudoku_size; k++) {
                m_sudoku.push_back(sudoku_row[k]);
            }
        }
    } while (!checkSudoku(m_sudoku));
}

void MiniSudokuQuiz::sudokuSuggestion(std::vector<int> sudoku)
{
    //Test:
    std::string text="";
    for (int i=0; i<sudoku.size(); i++) {
        if (i%sudoku_size==0&&i>0) {
            CCLOG("%s", text.c_str());
            text="";
        }
        text=cocos2d::StringUtils::format("%s %2d", text.c_str(), sudoku[i]);
    }
    CCLOG("%s", text.c_str());
    
    int number_suggest;
    if (sudoku_size==2) {
        //suggest 1 icon
        number_suggest=1;
    } else {
        number_suggest=sudoku_size*sudoku_size/2;
    }
    
    auto list=randomArrayIndexes(number_suggest, sudoku_size*sudoku_size-1);
    for (int i=0; i<list.size(); i++) {
        m_suggest.push_back(list[i]);
    }
    std::sort(m_suggest.begin(), m_suggest.end(), [=](int a, int b)->bool{
        return a<b;
    });
}

void MiniSudokuQuiz::suggestSudoku()
{
    int index=100;
    if (m_suggest.size()>0) {
        index=m_suggest.back();
        m_suggest.pop_back();
    }
    
    if (index<m_sudoku.size()) {
        i_sudoku.erase(i_sudoku.begin()+index);
        i_sudoku.insert(i_sudoku.begin()+index, m_sudoku[index]);
        
        int val=m_sudoku[index];
        Math_Object* m_object = nullptr;
        int m_index=0;
        for (Math_Object* object : object_dock) {
            if (object->getID() == val) {
                m_object=object;
                break;
            }
            m_index++;
        }
        
        Math_Slot* slot = _slots[index];
        auto cloned = m_object->clone();
        auto position = m_object->getWorldPosition();
        cloned->removeFromParentAndCleanup(false);
        cloned->setPosition(position);
        cloned->setLocked(true);
        _panel_move->addChild(cloned, 100);
        slot->setLinker(cloned);
        cloned->onShow(0, math::animation::No_Effect);
        cloned->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::MoveTo::create(0.5, slot->getPosition()), cocos2d::CallFunc::create([=](){
            cloned->removeFromParentAndCleanup(false);
            this->addChildNode(cloned);
            slot->setBackground(bg_gray);
        })));

        scheduleOnce([=](float dt){
            this->suggestSudoku();
        }, 0.4, cocos2d::StringUtils::format("suggestSudoku_%d", index));
    } else {
        enableControlQuiz();
    }
}


//checking sudoku
bool MiniSudokuQuiz::checkSudoku(std::vector<int> sudoku)
{
    if (!checkSudokuRow(sudoku)) {
        return false;
    }
    if (!checkSudokuColumn(sudoku)) {
        return false;
    }
    if (!checkSudokuBlocks(sudoku)) {
        return false;
    }
    return true;
}

bool MiniSudokuQuiz::checkSudokuRow(std::vector<int> sudoku)
{
    for (int row=0; row<sudoku_size; row++) {
        std::vector<int> i_sud;
        for (int i=0; i<sudoku_size; i++) {
            i_sud.push_back(sudoku[i+row*sudoku_size]);
        }
        std::sort(i_sud.begin(), i_sud.end(), [=](int a, int b)->bool{
            return a<b;
        });
        for (int i=1; i<sudoku_size; i++) {
            if (i_sud[i-1]==i_sud[i]) {
                return false;
            }
        }
    }
    return true;
}

bool MiniSudokuQuiz::checkSudokuColumn(std::vector<int> sudoku)
{
    for (int col=0; col<sudoku_size; col++) {
        std::vector<int> i_sud;
        for (int i=0; i<sudoku_size; i++) {
            i_sud.push_back(sudoku[col+i*sudoku_size]);
        }
        std::sort(i_sud.begin(), i_sud.end(), [=](int a, int b)->bool{
            return a<b;
        });
        
        for (int i=1; i<sudoku_size; i++) {
            if (i_sud[i-1]==i_sud[i]) {
                return false;
            }
        }
    }
    return true;
}

bool MiniSudokuQuiz::checkSudokuBlocks(std::vector<int> sudoku)
{
    if (has_block) {
        for (int i=0; i<sudoku_size; i++) {
            auto block=iblocks[i];
            std::vector<int> i_sud;
            for (int k=0; k<sudoku_size; k++) {
                i_sud.push_back(sudoku[block[k]]);
            }
            std::sort(i_sud.begin(), i_sud.end(), [=](int a, int b)->bool{
                return a<b;
            });
            for (int i=1; i<sudoku_size; i++) {
                if (i_sud[i-1]==i_sud[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool MiniSudokuQuiz::checkSudokuAtPoint(int value, int point)
{
    int col=point%sudoku_size;
    int row=point/sudoku_size;
    
    if (value>=0) {
        //Check them icon conflic
        for (int i=0; i<list_conflic.size(); i++) {
            if (list_conflic[i]==point) {
                list_conflic.erase(list_conflic.begin()+i);
                break;
            }
        }
        
        std::vector<int> i_same;
        //check column
        for (int i=0; i<sudoku_size; i++) {
            int index=col+i*sudoku_size;
            if (i_sudoku[index]==value && index!=point) {
                i_same.push_back(index);
            }
        }
        
        //check row
        for (int i=0; i<sudoku_size; i++) {
            int index=row*sudoku_size+i;
            if (i_sudoku[index]==value && index!=point) {
                i_same.push_back(index);
            }
        }
        
        //Check block
        if (has_block) {
            int index_block=0;
            bool finded=false;
            for (index_block=0; index_block<sudoku_size; index_block++) {
                auto block=iblocks[index_block];
                for (int i=0; i<sudoku_size; i++) {
                    if (block[i]==point) {
                        finded=true;
                        break;
                    }
                }
                if (finded) {
                    break;
                }
            }
            auto block=iblocks[index_block];
            for (int i=0; i<sudoku_size; i++) {
                if (i_sudoku[block[i]]==value && block[i]!=point) {
                    i_same.push_back(block[i]);
                }
            }
        }
        if (i_same.size()>0) {
            i_same.push_back(point);
        }
        for (int i=0; i<i_same.size(); i++) {
            list_conflic.push_back(i_same[i]);
        }
        if (list_conflic.size()>0) {
            sort(list_conflic.begin(), list_conflic.end(), [=](int a, int b)->bool{return a<b;});
            for (int i=0; i<list_conflic.size()-1; ) {
                if (list_conflic[i]==list_conflic[i+1]) {
                    list_conflic.erase(list_conflic.begin()+i);
                } else {
                    i++;
                }
            }
            for (int i : list_conflic) {
                if (!_slots[i]->getLinker()->isLocked()) {
                    _slots[i]->setBackground(bg_red);
                }
            }
        }
    } else {
        //Check giam icon conflic
        bool is_conflic=false;
        int i_conf=0;
        for (;i_conf<list_conflic.size(); i_conf++) {
            if (point==list_conflic[i_conf]) {
                is_conflic=true;
                _slots[list_conflic[i_conf]]->setBackground(bg_white);
                break;
            }
        }
        if (is_conflic) {
            list_conflic.erase(list_conflic.begin()+i_conf);
            std::vector<int> list_check;
            for (int i : list_conflic) {
                list_check.push_back(i);
            }
            for (int i : list_check) {
                if (!_slots[i]->getLinker()->isLocked()) {
                    checkSudokuAtPoint(i_sudoku[i], i);
                }
            }
        }
    }
    
    return true;
}

void MiniSudokuQuiz::onBeganDragSudoku(int value)
{
    for (int i=0; i<i_sudoku.size(); i++) {
        if (i_sudoku[i]==value) {
            if (_slots[i]->getLinker()->isLocked()) {
            } else {
                _slots[i]->setBackground(bg_green);
            }
        }
    }
}

void MiniSudokuQuiz::onEndedDragSudoku(int value)
{
    for (int i=0; i<i_sudoku.size(); i++) {
        if (i_sudoku[i]==value) {
            if (!_slots[i]->getLinker()->isLocked()) {
                _slots[i]->setBackground(bg_white);
            }
        }
    }
    
    for (int i : list_conflic) {
        if (!_slots[i]->getLinker()->isLocked()) {
            _slots[i]->setBackground(bg_red);
        }
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MiniSudokuQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        //this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}

