
#include "GameQA_Item.h"
#include "GAFWrapper.h"

INITIALIZE_READER(MJGameQA_Item);

USING_NS_CC;

MJGameQA_Item* MJGameQA_Item::createFrame(int item_id) {
    auto p = static_cast<MJGameQA_Item*>(cocos2d::CSLoader::createNode("csb/game/qa/GameQA_Item.csb"));
    if (p) {
        p->didLoadFromCSB(item_id);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameQA_Item::didLoadFromCSB(int item_id) {
    _itemid = item_id;

    item_bg1 = this->getChildByName<ImageView*>("item_bg1");
    item_bg2 = this->getChildByName<ImageView*>("item_bg2");
    _item_bg2_red = item_bg2->getChildByName<ImageView*>("item_bg2_red");
    _item_bg2_green = item_bg2->getChildByName<ImageView*>("item_bg2_green");

    lbnum1 = item_bg1->getChildByName<Text*>("lbnum1");
    lbnum2 = item_bg2->getChildByName("circle")->getChildByName<Text*>("lbnum2");

    if (item_id == 1) {
        setCurrentItem();
    }
    else {
        setNormalItem();
    }

    lbnum1->setString(StringUtils::toString(item_id));
    lbnum2->setString(StringUtils::toString(item_id));

}

void MJGameQA_Item::setNormalItem() {
    item_bg1->setVisible(true);
    item_bg2->setVisible(false);
    item_bg1->loadTexture("games/qa/qa_item_normal.png");
    lbnum1->setTextColor(Color4B(0, 92, 167, 255));
    lbnum1->setVisible(true);
}

void MJGameQA_Item::setWasChooseQes()
{
    _wasChooseQues = true;
}

bool MJGameQA_Item::getWasChooseQes()
{
    return _wasChooseQues;
}

void MJGameQA_Item::setWasChooseAnswer()
{
    _wasChooseAnswer = true;
}

bool MJGameQA_Item::getWasChooseAnswer()
{
    return _wasChooseAnswer;
}

void MJGameQA_Item::setWasChooseCorrectAnswer(bool answerStatus)
{
    _wasChooseCorrectAnswer = answerStatus;
}

bool MJGameQA_Item::getWasChooseCorrectAnswer()
{
    return _wasChooseCorrectAnswer;
}

void MJGameQA_Item::setVisibleItem_QARed(string color)
{
    // enable - red; not enable - green
    if (color == "red") {
        _item_bg2_red->setVisible(true);
        _item_bg2_green->setVisible(false);
    }
    if (color == "green") {
        _item_bg2_red->setVisible(false);
        _item_bg2_green->setVisible(true);
    }
    if (color == "origin") {
        _item_bg2_red->setVisible(false);
        _item_bg2_green->setVisible(false);
    }
}

void MJGameQA_Item::setCurrentItem() {
    item_bg1->setVisible(false);
    item_bg2->setVisible(true);
}
void MJGameQA_Item::setChooseItem() {
    item_bg1->setVisible(true);
    item_bg2->setVisible(false);
    item_bg1->loadTexture("games/qa/qa_item_choose.png");
    lbnum1->setTextColor(Color4B::YELLOW);
    lbnum1->setVisible(true);
}
void MJGameQA_Item::setCheckTrueFalseItem(bool isTrue) {
    item_bg1->setVisible(true);
    item_bg2->setVisible(false);
    lbnum1->setVisible(false);
    _isCheckAnswer = isTrue;
    if (isTrue) {
        item_bg1->loadTexture("games/qa/qa_item_true.png");
        is_show_guide_wrong = true;
    }
    else {
        item_bg1->loadTexture("games/qa/qa_item_wrong.png");
        this->showArrowGuide(true);
    }
}

void MJGameQA_Item::setStateItem() {
    switch (_state) {
    case NORMAL_STATE:
        setNormalItem();
        break;
    case CHOOSE_STATE:
        setChooseItem();
        break;
    case CHECK_STATE:
        setCheckTrueFalseItem(_isCheckAnswer);
        break;
    default:
        break;
    }
}

ImageView* MJGameQA_Item::getImgWith(string path_img, int type) {//create ICON
    auto img = ImageView::create(path_img);
    if (type == 2) {
        img->setPosition(this->getContentSize() / 2);

    }
    else if (type == 3) {
        img->setPosition(Size(this->getContentSize().width / 2, 118));
    }
    return img;
}

std::string MJGameQA_Item::getObjectName() {
    return _data.text;
}

void MJGameQA_Item::setDelegate(MJGameQA_ItemDelegate* delegate) {
    this->delegate = delegate;
}

void MJGameQA_Item::onChoose(Ref* sender) {//delegate when click item.

    if (delegate) {
        is_set_choose = true;
        this->hideArrowGuide();
        delegate->onChooseQuesItem(this);
    }
}

void MJGameQA_Item::showArrowGuide(bool is_show_when_wrong, bool active) {
    if (!active) {
        if (is_show_when_wrong) {
            if (is_show_guide_wrong) return;
            is_show_guide_wrong = true;
        }
        else {
            if (is_show_arrowguide || is_show_guide_wrong || is_set_choose) return;
            is_show_arrowguide = true;
        }
    } 

    auto arrow_guide = cocos2d::ui::ImageView::create("games/arrow_guide.png");
    arrow_guide->setAnchorPoint(Vec2(0.5, 0));
    arrow_guide->setPosition(Vec2(this->getContentSize().width / 2, 70));
    this->addChild(arrow_guide, 10000);
    auto duration = 0.3f;
    arrow_guide->runAction(Repeat::create(Sequence::create(MoveBy::create(duration, Vec2(0, 10)), MoveBy::create(duration, Vec2(0, -10)), NULL), 100000));
    arrow_guide->setName("arrow_guide");
}

void MJGameQA_Item::hideArrowGuide() {
    if (auto arrow = this->getChildByName("arrow_guide")) {
        this->removeChildByName("arrow_guide");
    } 
}

ui::Widget::ccWidgetClickCallback MJGameQA_Item::onLocateClickCallback(const std::string& callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameQA_Item, onChoose),
    };

    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}


