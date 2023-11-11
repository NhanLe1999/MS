//
//  Math_Object.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/10/17.
//
//

#include "Math_Object.h"
#include "PoolManager.h"
#include "TestingModeUtils.h"
#include "AudioHelper.h"

INITIALIZE_READER_INSTANCE(Math_ObjectReader);

Math_Object* Math_Object::createObject() {
    auto p = (Math_Object *)(cocos2d::CSLoader::createNode("math_object.csb"));
    if (p && p->init()) {
    } else {
        CC_SAFE_DELETE(p);
    }
    return p;
}

bool Math_Object::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    this->setContentSize(cocos2d::Size(200, 200));
    
    _background=this->getChildByName<cocos2d::ui::ImageView*>("bg");
    _image=this->getChildByName<cocos2d::ui::ImageView*>("image");
    _lbl_value=this->getChildByName<cocos2d::ui::Text*>("label");
    _enable=false;
    
    addEvents();
        
    this->setTag(2003);
    this->setName("Math_Object");

    return true;
}

void Math_Object::setEnable(int id, math::common::TouchPermission touch, math::resource::Image* image, math::resource::Image* bg, bool color, bool copy) {
    
    _id = id;
    _enable=true;
    _selected=false;
    _coloring=color;
    _copied=copy;
    
    _locked=false;
    _touch=touch;
    _allow_move=true;
    _move_dir=0;
    _kind=0;
    
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    
    if (touch==math::common::TouchPermission::deny) {
        _locked=true;
    }
    _comparision_value.size = 0;
    
    _image->setScale9Enabled(false);
    _image->setPositionZ(3);
    _lbl_value->setPositionZ(4);
    _lbl_value->setString("");
    _lbl_value->setTextColor(m_text_color);
    _lbl_value->setVisible(false);
    _lbl_value->setFontName(m_text_font);
    _lbl_value->enableOutline(cocos2d::Color4B::WHITE, 3);

    setImage(image);
    setBackground(bg);

    setComparisionValue(_comparision_value.size);
}

void Math_Object::setDisable() {
    _enable=false;
    
    delete _image_touch;
    _image_touch=nullptr;
    
    if (_drawer != nullptr) {
        _drawer->clear();
    }    
    onHide(0);
}

bool Math_Object::isEnable() {
    return _enable;
}

void Math_Object::cloneFrom(Math_Object *object) {
    setEnable(object->getID(), object->getTouch(), object->getImageSource(), object->getBackgroundSource(), object-isColoring(), object->isCopied());
    setComparisionValue(object->getComparisionValue().size);
    setObjectSize(object->getContentSize());
}

Math_Object* Math_Object::clone() {
    auto cloned = mpool->getObject();
    cloned->setEnable(_id, _touch, _img_src, _bg_src, _coloring, _copied);
    cloned->setComparisionValue(_comparision_value.size);
    cloned->setObjectSize(getContentSize());
    
    cloned->setImageVisible(_image->isVisible());
    cloned->setValueText(_lbl_value->getString());
    cloned->showValueTextWithAnimation(false);
    cloned->getLabelValue()->setFontSize(_lbl_value->getFontSize());
    cloned->getLabelValue()->setFontName(_lbl_value->getFontName());
    cloned->getLabelValue()->setTextColor(_lbl_value->getTextColor());
    
    if (_root != nullptr) {
        cloned->setPosition(_root->convertToNodeSpace(getWorldPosition()));
        _root->addChild(cloned, 2);
    }

    if (_touch == math::common::touch) {
        cloned->setTouchCallback(_func_touched);
    } else if (_touch == math::common::move) {
        cloned->setSuperNode(super_node);
        cloned->setMoveCallback(_func_touch_began, _func_touch_ended);
        cloned->enableDrag(_root);
    }
    
    return cloned;
}

void Math_Object::setMoveCallback(math::callback::onTouchObjectBegan began_func, math::callback::onTouchObjectEnded ended_func) {
    _func_touch_began=began_func;
    _func_touch_ended=ended_func;
    
}
void Math_Object::setTouchCallback(math::callback::onTouchObjectBegan touch_func) {
    _func_touched=touch_func;
}

void Math_Object::enableDrag(Node* root) {
    _touch=math::common::move;
    _locked=false;
    updateRoot(root);
}

void Math_Object::updateRoot(cocos2d::Node *root) {
    _root_group=_group;
    _root_index=_id;
    _root=root;
    _parent=root;
    
    if (super_node == nullptr) {
        super_node = _parent;
    }
    _root_pos= super_node->convertToNodeSpace(this->getWorldPosition());
    _root_z=this->getLocalZOrder();
}

void Math_Object::addToGroup(int id, Node* group, int kind) {
    _id=id;
    _group=group;
    _kind=kind;
}

#pragma mark - setter & getter

int Math_Object::getID() {
    return _id;
}

math::common::TouchPermission Math_Object::getTouch() {
    return _touch;
}

void Math_Object::setTouch(math::common::TouchPermission touch) {
    _touch = touch;
}

void Math_Object::setComparisionValue(int size) {
    _comparision_value.id=_img_src->code;
    _comparision_value.set=_img_src->getText("set");
    _comparision_value.name=_img_src->getText("name_1");
    _comparision_value.family=_img_src->getText("family");
    _comparision_value.kind=_img_src->getText("kind");
    _comparision_value.color=_img_src->getText("color");
    _comparision_value.position=_img_src->getText("position");
    _comparision_value.like=_img_src->getText("like");

    //Comparision
    _comparision_value.size=size;
    _comparision_value.height=_img_src->getComparisionValue("height");
    _comparision_value.width=_img_src->getComparisionValue("width");
    _comparision_value.length=_img_src->getComparisionValue("length");
    _comparision_value.weight=_img_src->getComparisionValue("weight");
    _comparision_value.hold=_img_src->getComparisionValue("hold");
    _comparision_value.number=_img_src->getComparisionValue("number");
    _comparision_value.character=_img_src->getComparisionValue("character");
}

void Math_Object::setComparisionValue(std::string key, int comparision_value) {
    if (key.compare("number")==0) {
        _comparision_value.number=comparision_value;
    } else if (key.compare("height")==0) {
        _comparision_value.height=comparision_value;
    } else if (key.compare("width")==0) {
        _comparision_value.width=comparision_value;
    } else if (key.compare("length")==0) {
        _comparision_value.length=comparision_value;
    } else if (key.compare("weight")==0) {
        _comparision_value.weight=comparision_value;
    } else if (key.compare("hold")==0) {
        _comparision_value.hold=comparision_value;
    } else if (key.compare("character")==0) {
        _comparision_value.character=comparision_value;
    }
}

void Math_Object::setComparisionValue(std::string key, std::string property) {
    
    if (key.compare("set")==0) {
        _comparision_value.set=property;
    } else if (key.compare("name")==0) {
        _comparision_value.name=property;
    } else if (key.compare("family")==0) {
        _comparision_value.family=property;
    } else if (key.compare("kind")==0) {
        _comparision_value.kind=property;
    } else if (key.compare("color")==0) {
        _comparision_value.color=property;
    } else if (key.compare("like")==0) {
        _comparision_value.like=property;
    } else if (key.compare("position")==0) {
        _comparision_value.position=property;
    }
}

math::comparision::Value Math_Object::getComparisionValue() {
    return _comparision_value;
}

void Math_Object::setCopied(bool copied) {
    _copied=copied;
}

bool Math_Object::isCopied() {
    return _copied;
}

void Math_Object::setColoring(bool coloring) {
    _coloring=coloring;
}

bool Math_Object::isColoring() {
    return _coloring;
}

int Math_Object::getGroupID() {
    if (_group!=NULL) {
        Math_Group* group=(Math_Group*)_group;
        return group->getId();
    }
    return -1;
}

int Math_Object::getKind() {
    return _kind;
}

void Math_Object::setLocked(bool lock) {
    _locked=lock;
}

bool Math_Object::isLocked() {
    return _locked;
}

void Math_Object::setAllowMove(bool allow) {
    _allow_move=allow;
}

bool Math_Object::isAllowMove() {
    return _allow_move;
}

void Math_Object::setMoveDirection(int md) {
    _move_dir=md;
}

int Math_Object::getMoveDirection() {
    return _move_dir;
}

#pragma mark - Object
void Math_Object::setObjectSize(cocos2d::Size size, bool align_bot) {
    this->setContentSize(size);
    
    int compare_size = _comparision_value.size;
    float scalar = 1;
    if (_bg_src!=nullptr) {
        if (_bg_src->getText("name_1").compare("circle")==0) {
            scalar = (1 - compare_size*0.18)*0.7;
            _image->setContentSize(size*scalar);
        } else {
            scalar = (1 - compare_size*0.18)*0.8;
            _image->setContentSize(size*scalar);
        }
    } else {
        scalar = (1 - compare_size*0.18);
        _image->setContentSize(size * scalar);
    }
    
    if (align_bot) {
        _image->setPosition(cocos2d::Size(size.width * .5, size.height * (1+scalar) * .25));
    } else {
        _image->setPosition(size/2);
    }
    
    _background->setContentSize(size);
    _background->setPosition(size/2);
    
    _lbl_value->setFontSize(size.height*.5);
    _lbl_value->setPosition(size/2);
    
//    getDrawer()->clear();
//    getDrawer()->drawCircle(size/2, 8, 360, 50, true, 1, 1, cocos2d::Color4F::RED);
//    getDrawer()->drawRect(cocos2d::Vec2::ZERO, cocos2d::Vec2(size.width, size.height), cocos2d::Color4F::RED);
}

void Math_Object::fitObjectSize() {
    auto font_size = _lbl_value->getFontSize();
    setObjectSize(getContentSize());
    _lbl_value->setFontSize(font_size);
    setValueTextPosition(_position_txt);
}

void Math_Object::setObjectFullSize(cocos2d::Size size) {
    this->setContentSize(size);
    
    _image->setContentSize(size *(1 - _comparision_value.size*0.15));
    _image->setPosition(size/2);
    
    setBackgroundVisible(false);
    
    _lbl_value->setFontSize(size.height*.35);
    _lbl_value->setPosition(size/2);
}

#pragma mark - image
void Math_Object::setImage(math::resource::Image *image)
{
    if (image != nullptr) {
        if (_img_src != nullptr) {
            _img_src->used = false;
        }
        _img_src=image;
        _img_src->used=true;
        _image->loadTexture(image->path);
        
        if (_image_touch != nullptr) {
            delete _image_touch;
            _image_touch = nullptr;
        }
        _image_touch=new cocos2d::Image();
        _image_touch->initWithImageFile(image->path);
        _image->setVisible(true);
    }
}

void Math_Object::setImageVisible(bool visible)
{
    _image->setVisible(visible);
}

void Math_Object::setImageColor(std::string color_name) {
    //find image
    auto gp= math_statistic->getGamePath();
    
    std::string pre_fix = "";
    if (testing::TestManager::getInstance()->getTestMode() == testing::TestMode::activity) {
        pre_fix = gp;
    }
    
    auto image_list=math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", gp.c_str()), _comparision_value.name, pre_fix);
    for (math::resource::Image * image : image_list) {
        if (image->getText("color").compare(color_name)==0) {
            setImage(image);
            if (_coloring) {
                _comparision_value.color = color_name;
            }
            break;
        }
    }
    //mpool->resetImages();
}

void Math_Object::setImageColor(cocos2d::Color3B color)
{
    _image->setColor(color);
}

cocos2d::ui::ImageView* Math_Object::getImage()
{
    return _image;
}

math::resource::Image* Math_Object::getImageSource()
{
    return _img_src;
}


#pragma mark - background
void Math_Object::setBackground(math::resource::Image* image, float padding)
{
    if (image!=nullptr) {
        if (_bg_src != nullptr) {
            _bg_src->used = false;
        }
        _bg_src=image;
        _bg_src->used=true;
        _background->loadTexture(image->path);
        _background->setScale9Enabled(true);
        auto render_size = _background->getVirtualRendererSize();
        if (padding == 0) {
            cocos2d::Rect cap=cocos2d::Rect(1, 1, render_size.width-2, render_size.height-2);
            _background->setCapInsets(cap);
        } else {
            if (padding >= 0.5) {
                padding = 0.45;
            }
            cocos2d::Rect cap=cocos2d::Rect(padding * render_size.width, padding * render_size.height, render_size.width*(1-padding*2), render_size.height * (1-padding*2));
            _background->setCapInsets(cap);
        }
        _background->setVisible(true);
        return;
    }
    _background->setVisible(false);
}

void Math_Object::setBackgroundVisible(bool visible)
{
    _background->setVisible(visible);
}

void Math_Object::setBackgroundColor(cocos2d::Color3B color)
{
    _background->setColor(color);
}

cocos2d::ui::ImageView* Math_Object::getBackground()
{
    return _background;
}

math::resource::Image* Math_Object::getBackgroundSource()
{
    return _bg_src;
}

void Math_Object::setBackgroundColor(std::string color_name)
{
    //Tim image
    auto gp= math_statistic->getGamePath();
    
    std::string pre_fix = "";
    if (testing::TestManager::getInstance()->getTestMode() == testing::TestMode::activity) {
        pre_fix = gp;
    }

    auto image_list=math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", math_statistic->getGamePath().c_str()), _bg_src->getText("name_1"), pre_fix);
    for (math::resource::Image * image : image_list) {
        if (image->getText("color").compare(color_name)==0) {
            setBackground(image);
            if (_coloring) {
                _comparision_value.color = color_name;
            }
            break;
        }
    }
    //mpool->resetImages();
}

#pragma mark - value
void Math_Object::setValueText(std::string text)
{
    _lbl_value->setString(text);
}

void Math_Object::setValueTextByKey(std::string key)
{
    if (key == "name") {
        key = "name_1";
    }
    std::string text_of_key=key;
    bool filter_prop = true;
    for (math::resource::Property prop : _img_src->comparisions) {
        if (prop.key == key) {
            filter_prop = false;
            break;
        }
    }
    if (filter_prop) {
        text_of_key=_img_src->getText(key);
    } else {
        text_of_key=cocos2d::StringUtils::format("%d", _img_src->getComparisionValue(key));
    }
    text_of_key = math::func::upperFirstLetter(text_of_key);
    _lbl_value->setString(text_of_key);
}

std::string Math_Object::getValueText()
{
    return _lbl_value->getString();
}

void Math_Object::fitValueTextSize()
{
    auto size=this->getContentSize();
    float label_width=_lbl_value->getContentSize().width;
    
    if (label_width>=size.width) {
        float fSize=_lbl_value->getFontSize();
        fSize=fSize*size.width/label_width;
        _lbl_value->setFontSize(fSize);
    }
}

void Math_Object::setValueTextPosition(std::string position) {
    _lbl_value->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _position_txt = position;
    if (position.compare("middle")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.5, getContentSize().height*.45));
    } else if (position.compare("bottom")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.5, -_lbl_value->getFontSize()*.5));
    } else if (position.compare("bottom_right")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.95, 0));
    } else if (position.compare("bottom_left")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.05, 0));
    } else if (position.compare("right")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.95, getContentSize().height*.45));
    } else if (position.compare("left")==0) {
        _lbl_value->setPosition(cocos2d::Vec2(getContentSize().width*.05, getContentSize().height*.45));
    }
}

cocos2d::ui::Text* Math_Object::getLabelValue()
{
    return _lbl_value;
}

void Math_Object::showValueTextWithAnimation(bool animated)
{
    _lbl_value->setVisible(true);
    if (animated) {
        _lbl_value->setScale(0);
        _lbl_value->runAction(cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.25, 1)));
    }
}

void Math_Object::hideValueTextWithAnimation(bool animated)
{
    if (animated) {
        _lbl_value->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.25, 0)), cocos2d::CallFunc::create([=](){
            _lbl_value->setVisible(false);
        })));
    } else {
        _lbl_value->setVisible(false);
    }
}

#pragma mark - compare
bool Math_Object::isSame(Math_Object *other)
{
    return this==other;
}

int Math_Object::compare(math::comparision::Value comparision, math::comparision::Key key)
{
    switch (key) {
        case math::comparision::exactly :
            return _comparision_value.id.compare(comparision.id);
            break;
        case math::comparision::name :
            return _comparision_value.name.compare(comparision.name);
            break;
        case math::comparision::kind :
            return _comparision_value.kind.compare(comparision.kind);
            break;
        case math::comparision::color :
            return _comparision_value.color.compare(comparision.color);
            break;
        case math::comparision::family :
            return _comparision_value.family.compare(comparision.family);
            break;
        case math::comparision::size :
        {
            if (_comparision_value.size==comparision.size) {
                return 0;
            } else if (_comparision_value.size<comparision.size) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::height :
        {
            if (_comparision_value.height==comparision.height) {
                return 0;
            } else if (_comparision_value.height>comparision.height) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::width :
        {
            if (_comparision_value.width==comparision.width) {
                return 0;
            } else if (_comparision_value.width>comparision.width) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::length :
        {
            if (_comparision_value.length==comparision.length) {
                return 0;
            } else if (_comparision_value.length>comparision.length) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::weight :
        {
            if (_comparision_value.weight==comparision.weight) {
                return 0;
            } else if (_comparision_value.weight>comparision.weight) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::hold :
        {
            if (_comparision_value.hold==comparision.hold) {
                return 0;
            } else if (_comparision_value.hold>comparision.hold) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::number :
        {
            if (_comparision_value.number==comparision.number) {
                return 0;
            } else if (_comparision_value.number>comparision.number) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::time :
        {
            if (_comparision_value.time==comparision.time) {
                return 0;
            } else if (_comparision_value.time>comparision.time) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::character :
        {
            if (_comparision_value.character==comparision.character) {
                return 0;
            } else if (_comparision_value.character>comparision.character) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::position :
            return _comparision_value.position.compare(comparision.position);
            break;
        case math::comparision::like :
            return _comparision_value.like.compare(comparision.like);
            break;

        default:
            break;
    }
    return 1;
}

int Math_Object::compare(Math_Object *other, math::comparision::Key key)
{
    return compare(other->getComparisionValue(), key);
}

int Math_Object::compare(cocos2d::Value value, math::comparision::Key key)
{
    switch (key) {
        case math::comparision::exactly :
            return _comparision_value.id.compare(value.asString());
            break;
        case math::comparision::name :
            return _comparision_value.name.compare(value.asString());
            break;
        case math::comparision::kind :
            return _comparision_value.kind.compare(value.asString());
            break;
        case math::comparision::color :
            return _comparision_value.color.compare(value.asString());
            break;
        case math::comparision::family :
            return _comparision_value.family.compare(value.asString());
            break;
        case math::comparision::size :
        {
            if (_comparision_value.size==value.asInt()) {
                return 0;
            } else if (_comparision_value.size<value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::height :
        {
            if (_comparision_value.height==value.asInt()) {
                return 0;
            } else if (_comparision_value.height>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::width :
        {
            if (_comparision_value.width==value.asInt()) {
                return 0;
            } else if (_comparision_value.width>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::length :
        {
            if (_comparision_value.length==value.asInt()) {
                return 0;
            } else if (_comparision_value.length>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::weight :
        {
            if (_comparision_value.weight==value.asInt()) {
                return 0;
            } else if (_comparision_value.weight>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::hold :
        {
            if (_comparision_value.hold==value.asInt()) {
                return 0;
            } else if (_comparision_value.hold>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::number :
        {
            if (_comparision_value.number==value.asInt()) {
                return 0;
            } else if (_comparision_value.number>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::time :
        {
            if (_comparision_value.time==value.asInt()) {
                return 0;
            } else if (_comparision_value.time>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::character :
        {
            if (_comparision_value.character==value.asInt()) {
                return 0;
            } else if (_comparision_value.character>value.asInt()) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::position :
        {
            return _comparision_value.position.compare(value.asString());
        }
            break;
        case math::comparision::like :
        {
            return _comparision_value.like.compare(value.asString());
        }
            break;
        default:
            break;
    }
    return 1;
}

int Math_Object::compare(math::resource::Image* img, math::comparision::Key key) {
    switch (key) {
        case math::comparision::exactly :
            return _comparision_value.id.compare(img->code);
            break;
        case math::comparision::name :
            return _comparision_value.name.compare(img->getText("name_1"));
            break;
        case math::comparision::kind :
            return _comparision_value.kind.compare(img->getText("kind"));
            break;
        case math::comparision::color :
            return _comparision_value.color.compare(img->getText("color"));
            break;
        case math::comparision::family :
            return _comparision_value.family.compare(img->getText("family"));
            break;
        case math::comparision::size :
        {
            int m_size = img->getComparisionValue("size");
            if (_comparision_value.size==m_size) {
                return 0;
            } else if (_comparision_value.size<m_size) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::height :
        {
            int m_value = img->getComparisionValue("height");
            if (_comparision_value.height==m_value) {
                return 0;
            } else if (_comparision_value.height>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::width :
        {
            int m_value = img->getComparisionValue("width");
            if (_comparision_value.width==m_value) {
                return 0;
            } else if (_comparision_value.width>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::length :
        {
            int m_value = img->getComparisionValue("length");
            if (_comparision_value.length==m_value) {
                return 0;
            } else if (_comparision_value.length>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::weight :
        {
            int m_value = img->getComparisionValue("weight");
            if (_comparision_value.weight==m_value) {
                return 0;
            } else if (_comparision_value.weight>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::hold :
        {
            int m_value = img->getComparisionValue("hold");
            if (_comparision_value.hold==m_value) {
                return 0;
            } else if (_comparision_value.hold>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::number :
        {
            int m_value = img->getComparisionValue("number");
            if (_comparision_value.number==m_value) {
                return 0;
            } else if (_comparision_value.number>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::time :
        {
            int m_value = img->getComparisionValue("time");
            if (_comparision_value.time==m_value) {
                return 0;
            } else if (_comparision_value.time>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::character :
        {
            int m_value = img->getComparisionValue("character");
            if (_comparision_value.character==m_value) {
                return 0;
            } else if (_comparision_value.character>m_value) {
                return 1;
            } else {
                return -1;
            }
        }
            break;
        case math::comparision::position :
        {
            return _comparision_value.position.compare(img->getText("position"));
        }
            break;
        case math::comparision::like :
        {
            return _comparision_value.like.compare(img->getText("like"));
        }
            break;
        default:
            break;
    }
    return 1;
}


bool Math_Object::isEqual(Math_Object *other, math::comparision::Key key)
{
    return compare(other, key)==0;
}

bool Math_Object::isEqual(math::comparision::Value comparision, math::comparision::Key key)
{
    return compare(comparision, key)==0;
}

bool Math_Object::isEqual(cocos2d::Value value, math::comparision::Key key)
{
    return compare(value, key)==0;
}

bool Math_Object::isEqual(math::resource::Image* img, math::comparision::Key key) {
    return compare(img, key)==0;
}


#pragma mark - detected
void Math_Object::setTouchDetectedType(math::common::DetectedType type)
{
    _detected_type=type;
}

bool Math_Object::isDetected(cocos2d::Vec2 point)
{
    cocos2d::Size imageSize;
    cocos2d::Size content_size = this->getContentSize();
    imageSize=_image->getContentSize();
    
    cocos2d::Vec2 fix_position=cocos2d::Vec2(this->getContentSize().width-imageSize.width, this->getContentSize().height-imageSize.height)/2;
    auto point_converted=cocos2d::Vec2(point.x-fix_position.x, point.y-fix_position.y);
    
    switch (_detected_type) {
        case math::common::DetectedType::box:
        {
            CCLOG("Tap In Bounding Box");
            //return object_touch_rect.containsPoint(point);
        }
            break;
        case math::common::DetectedType::circle:
        {
            CCLOG("Tap In Circle Or Elipse");
            point_converted=cocos2d::Vec2(point_converted.x, point_converted.y);
            //return Shape2DHitUtility::tapsOnElipse2D(circle_info.first, circle_info.second*elipse_info.first, circle_info.second*elipse_info.second, pointInObject);
        }
            break;
        case math::common::DetectedType::nontrans:
        {
            CCLOG("Tap On Non Transparent");
            //auto convert_touch=cocos2d::Vec2(_image_touch->getWidth()/imageSize.width, _image_touch->getHeight()/imageSize.height);
            //point_converted=cocos2d::Vec2(point_converted.x*convert_touch.x, point_converted.y*convert_touch.y);
            auto convert_touch=cocos2d::Vec2(_image_touch->getWidth()/imageSize.width, _image_touch->getHeight()/imageSize.height);
            point_converted=cocos2d::Vec2(point_converted.x*convert_touch.x, point_converted.y*convert_touch.y);
            return Shape2DHitUtility::tapsOnNonTransparent(point_converted, _image_touch);
            
            //return Shape2DHitUtility::touchOnImage(_img_src->path, _image->getContentSize(), point_converted);
        }
            break;
        default:
        {
            CCLOG("Tap In Polygon");
            //return Shape2DHitUtility::tapsOnPolygon2D(point_converted, vert);
        }
            break;
    }
    return true;
}

void Math_Object::setSelected(bool selected)
{
    if (_selected==selected) {
        return;
    }
//    if (action_selected != nullptr) {
//        this->stopAction(action_selected);
//        action_selected = nullptr;
//    }
    _selected=selected;
    if (_selected) {
        captureState();
        if (_coloring) {
            //setBackgroundColor("red");
        } else {
            if (select_eff) {
                select_eff->removeFromParentAndCleanup(true);
                select_eff = nullptr;
            }
            std::string gaf = "effect";
            auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
            select_eff = gaf_asset->createObjectAndRun(true);
            select_eff->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            select_eff->setPosition(getContentSize()/2);
            this->addChild(select_eff, -1);
        }
    } else {
        restoreCapturedState();
        if (_coloring) {
            setBackgroundColor("white");
        } else {
            if (select_eff) {
                select_eff->removeFromParentAndCleanup(true);
                select_eff = nullptr;
            }
        }
    }
}

bool Math_Object::isSelected() {
    return _selected;
}

void Math_Object::setSelectedAnimation(math::common::SelectedAnimation animation)
{
    _selected_animation=animation;
}

#pragma mark - animation
void Math_Object::onTouchObject(bool correct, float delay, int opacity) {
    scheduleOnce([=](float) {
        if (correct) {
            audio_helper->play("audio_correct.mp3");
            
            std::string gaf = "click1";
            auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
            auto anim = gaf_asset->createObjectAndRun(false);
            anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            anim->setScale((getContentSize().width>getContentSize().height?getContentSize().height:getContentSize().width)/200.0);
            anim->setPosition(getPosition());
            getParent()->addChild(anim, 20);
            scheduleOnce([=](float){
                anim->removeFromParentAndCleanup(true);
            }, 0.5, cocos2d::StringUtils::format("remove effect %d", _id));
            
            float current_scale = this->getScale();
            this->runAction(cocos2d::Sequence::create(cocos2d::EaseSineOut::create(cocos2d::ScaleTo::create(0.2, current_scale*1.2)), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.25, current_scale)), cocos2d::CallFunc::create([=](){
                _image->setOpacity(opacity);
                _lbl_value->setOpacity(opacity);
                _background->setOpacity(opacity);
            }), NULL));
            
        } else {
            audio_helper->play("audio_wrong.mp3");
            
            _image->setColor(cocos2d::Color3B(253, 192, 193));
            auto shake=cocos2d::Sequence::create(cocos2d::MoveBy::create(0.15, cocos2d::Vec2(8, 0)), cocos2d::MoveBy::create(0.15, cocos2d::Vec2(-8, 0)), NULL);
            this->runAction(cocos2d::Sequence::create(cocos2d::Repeat::create(shake, 2), cocos2d::CallFunc::create([=](){
                _image->setColor(cocos2d::Color3B::WHITE);
            }), NULL));
        }
    }, delay, cocos2d::StringUtils::format("onTouchObject %d", _id));
}

void Math_Object::onDropObject (float delay) {
    
    
    scheduleOnce([=](float) {
        audio_helper->play("audio_drop.mp3");
        
        std::string gaf = "drop_effect";
        auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
        auto anim = gaf_asset->createObjectAndRun(false);
        anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        anim->setScale(getContentSize().width/200.0);
        anim->setPosition(getPosition());
        getParent()->addChild(anim, 20);
        scheduleOnce([=](float){
            anim->removeFromParentAndCleanup(true);
        }, 0.5, cocos2d::StringUtils::format("remove drop effect %d", _id));
        
        float current_scale = this->getScale();
        this->runAction(cocos2d::Sequence::create(cocos2d::EaseSineOut::create(cocos2d::ScaleTo::create(0.2, current_scale*1.2)), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.25, current_scale)), cocos2d::CallFunc::create([=](){
        }), NULL));
        
    }, delay, cocos2d::StringUtils::format("onDropObject %d", _id));
}

void Math_Object::playAudio(std::string key) {
    audio_helper->play_sequence(_img_src->getAudios(key), nullptr, "");
}

void Math_Object::onShow(float dt, math::animation::ObjectShowType show, int delta) {
    this->stopAllActions();
    captureState();
    if (getParent()!=nullptr) {
        updateRoot(getParent());
    }
    
    if (show == math::animation::ScaleUp || show == math::animation::ScaleUpAndRotation || delta == 0) {
        this->setOpacity(0);
        delta = 100;
    } else {
        this->setOpacity(255);
    }

    switch (show) {
        case math::animation::No_Effect :
        {
            this->setOpacity(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::FadeIn::create(0.25)));
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
            this->setPosition(cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y - delta));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
        }
            break;
        case math::animation::SlideDown :
        {
            this->setPosition(cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y + delta));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
        }
            break;
        case math::animation::SlideLeft :
        {
            this->setPosition(cocos2d::Vec2(_captured_node.position.x + delta, _captured_node.position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
        }
            break;
        case math::animation::SlideRight :
        {
            this->setPosition(cocos2d::Vec2(_captured_node.position.x - delta, _captured_node.position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _captured_node.position)))));
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
}

void Math_Object::onHide(float dt, math::animation::ObjectShowType revert_show, int delta) {
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    this->stopAllActions();
    captureState();

    bool fade_out = false;
    if (delta == 0) {
        delta = 100;
        fade_out = true;
    }
    
    switch (revert_show) {
        case math::animation::No_Effect :
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeOut::create(0.25), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::ScaleUp :
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideUp :
        {
            auto position = cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y - delta);
            if (fade_out) {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            } else {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position)), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
        }
            break;
        case math::animation::SlideDown :
        {
            auto position = cocos2d::Vec2(_captured_node.position.x, _captured_node.position.y + delta);
            if (fade_out) {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            } else {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position)), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
        }
            break;
        case math::animation::SlideLeft :
        {
            auto position = cocos2d::Vec2(_captured_node.position.x + delta, _captured_node.position.y);
            if (fade_out) {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            } else {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position)), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
        }
            break;
        case math::animation::SlideRight :
        {
            auto position = cocos2d::Vec2(_captured_node.position.x - delta, _captured_node.position.y);
            if (fade_out) {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            } else {
                this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position)), cocos2d::CallFunc::create([=](){
                    this->removeFromParentAndCleanup(true);
                }), NULL));
            }
        }
            break;
        default:
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::Spawn::create(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::ScaleTo::create(0.5, 0), cocos2d::RotateTo::create(0.5, 180), NULL), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
    }
}

void Math_Object::onFlipUp(float dt)
{
    this->runAction(cocos2d::Sequence::create(cocos2d::ScaleTo::create(dt, 0, 1), cocos2d::CallFunc::create([=](){
        _image->setVisible(true);
        _lbl_value->setVisible(true);
    }), cocos2d::ScaleTo::create(dt, 1, 1), NULL));
}

void Math_Object::onFlipDown(float dt)
{
    this->runAction(cocos2d::Sequence::create(cocos2d::ScaleTo::create(dt, 0, 1), cocos2d::CallFunc::create([=](){
        _image->setVisible(false);
        _lbl_value->setVisible(false);
    }), cocos2d::ScaleTo::create(dt, 1, 1), NULL));
}


#pragma mark - testing
void Math_Object::onTouched()
{
    if (_locked) {
        return;
    }
    
    _image->setColor(cocos2d::Color3B::RED);
    _image->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::CallFunc::create([=](){
        _image->setColor(cocos2d::Color3B::WHITE);
    })));
    
    this->stopAllActions();
    if (_func_touched != nullptr) {
        _func_touched(this);
    }
}

void Math_Object::onTouchedAt(cocos2d::Vec2 point)
{
    if (_locked) {
        return;
    }
    cocos2d::Vec2 locationInNode = this->convertToNodeSpace(point);
    cocos2d::Size s = this->getBoundingBox().size;
    cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        if (isDetected(locationInNode)) {
            if (_touch==math::common::TouchPermission::touch) {
                _func_touched(this);
            } else {
                auto r_position = super_node->convertToNodeSpace(this->getWorldPosition());
                _seed_pos=super_node->convertToNodeSpace(this->getWorldPosition());
                _seed_z=this->getLocalZOrder();
                this->removeFromParentAndCleanup(false);
                super_node->addChild(this, 10000);
                this->setPosition(r_position);
                _func_touch_began(this);
            }
        }
    }
}

bool Math_Object::isTouchedAt(cocos2d::Vec2 point)
{
    cocos2d::Vec2 locationInNode = this->convertToNodeSpace(point);
    cocos2d::Size s = this->getBoundingBox().size;
    cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        if (isDetected(locationInNode)) {
            return true;
        }
    }
    return false;
}

#pragma mark - TOUCHED
void Math_Object::addEvents()
{
    if (number_add_events>0) {
        return;
    }
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(Math_Object::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(Math_Object::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(Math_Object::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(Math_Object::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    number_add_events++;
}

bool Math_Object::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event){
    if (_locked) {
        return false;
    }
    
    auto touchLocation = touch->getLocation();
    cocos2d::Vec2 locationInNode = this->convertToNodeSpace(touchLocation);
    cocos2d::Size s = this->getBoundingBox().size;
    cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        if (isDetected(locationInNode)) {
            //vung cham = bounding box
            if (_touch==math::common::TouchPermission::touch) {
                if (_func_touched!=nullptr) {
                    _func_touched(this);
                }
            } else if (_touch==math::common::TouchPermission::move) {
                auto r_position = super_node->convertToNodeSpace(this->getWorldPosition());
                captureState();
                _seed_pos=super_node->convertToNodeSpace(this->getWorldPosition());
                _seed_z=this->getLocalZOrder();
                _parent=this->getParent();
                this->removeFromParentAndCleanup(false);
                super_node->addChild(this, 1000);
                this->setPosition(r_position);
                if (_func_touch_began!=nullptr) {
                    _func_touch_began(this);
                }
                return true;
            }
        }
    }
    return false;
}

void Math_Object::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
    if (_allow_move) {
        this->setScale(1);
        auto position_convert = super_node->convertToNodeSpace(touch->getLocation());
        switch (_move_dir) {
            case 0:
            {
                this->setPosition(position_convert);
            }
                break;
            case 1:
            {
                //only move horizontal ~ align vertical
                this->setPosition(cocos2d::Vec2(position_convert.x, this->getPosition().y));
            }
                break;
            default:
            {
                //only move vertical ~ align horizontal
                this->setPosition(cocos2d::Vec2(this->getPosition().x, position_convert.y));
            }
                break;
        }
    }
}

void Math_Object::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_allow_move) {
        if (_func_touch_ended!=nullptr) {
            _func_touch_ended(this, super_node->convertToNodeSpace(touch->getLocation()));
        }
    } else {
        //this->backToStart();
    }
    _allow_move=true;
}

void Math_Object::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_allow_move) {
        if (_func_touch_ended!=nullptr) {
            _func_touch_ended(this, super_node->convertToNodeSpace(touch->getLocation()));
        }
    } else {
        //this->backToStart();
    }
    _allow_move=true;
}

#pragma mark - move control
void Math_Object::backToStart()
{
    if (_root!=nullptr) {
        auto world_point = super_node->convertToNodeSpace(this->getWorldPosition());
        this->removeFromParentAndCleanup(false);
        super_node->addChild(this, 10000);
        this->setPosition(world_point);
        
        float time_move = _root_pos.distance(world_point) / 1500;
        if (time_move>0.6)
            time_move=0.6;
        if (time_move<0.4) {
            time_move=0.4;
        }
        
        this->setLocked(true);
        this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::Spawn::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(time_move, _root_pos)), cocos2d::ScaleTo::create(time_move, 1)), cocos2d::CallFunc::create([=](){
            auto point = this->getWorldPosition();
            point = _root->convertToNodeSpace(point);
            this->removeFromParentAndCleanup(false);
            _root->addChild(this, _root_z);
            this->setPosition(point);
            this->setLocked(false);
        })));
        
        this->_id=_root_index;
        this->_group=_root_group;
    } else {
        return;
    }
}

void Math_Object::backToLast(int z_order, bool group)
{
    if (_parent!=nullptr) {
        
        auto world_point = super_node->convertToNodeSpace(this->getWorldPosition());
        
        this->removeFromParentAndCleanup(false);
        super_node->addChild(this, 10000);
        this->setPosition(world_point);

        float time_move = _seed_pos.distance(world_point) / 2000;
        if (time_move>0.5)
            time_move=0.5;
        if (time_move<0.2) {
            time_move=0.2;
        }
        this->setLocked(true);
        this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::Spawn::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(time_move, _seed_pos)), cocos2d::ScaleTo::create(time_move, 1)), cocos2d::CallFunc::create([=](){
            if (group) {
                Math_Group* m_group = (Math_Group*)(_group);
                m_group->addObject(this, _seed_pos);
            } else {
                auto point = this->getWorldPosition();
                point = _parent->convertToNodeSpace(point);
                this->removeFromParentAndCleanup(false);
                _parent->addChild(this, _seed_z);
                this->setPosition(point);
            }
            this->setLocked(false);
        })));
    } else {
        return;
    }
}

cocos2d::Vec2 Math_Object::getStartPosition() {
    return _root_pos;
}

cocos2d::Vec2 Math_Object::getSeedPosition() {
    return  _seed_pos;
}

void Math_Object::backLastGroup() {
    this->backToLast(-1, true);
}

void Math_Object::backStartGroup() {
    if (_group!=nullptr) {
        Math_Group* group = static_cast<Math_Group*>(_root_group);
        if (!group->hasObject(this)) {
            auto r_position = super_node->convertToNodeSpace(this->getWorldPosition());
            this->setPosition(r_position);
            this->removeFromParentAndCleanup(false);
            super_node->addChild(this);
            this->backToStart();
            scheduleOnce([=](float){
                group->pushBackObject(this);
            }, 0.6, "add to group");
        }
    }
}

void Math_Object::setSuperNode(cocos2d::Node *node) {
    super_node = node;
}

#pragma mark - Code moi them v2.1
void Math_Object::captureState() {
    _captured_node.position = this->getPosition();
    _captured_node.scale = this->getScale();
    _captured_node.rotation = this->getRotation();
    
}

void Math_Object::restoreCapturedState() {
    this->setPosition(_captured_node.position);
    this->setScale(_captured_node.scale);
    this->setRotation(_captured_node.rotation);
    
}

void Math_Object::fixHeightObject() {
    //Get object image real size.
    auto obj_size = this->getContentSize();
    float obj_width = obj_size.height * _image->getVirtualRendererSize().width / _image->getVirtualRendererSize().height;
    setObjectSize(cocos2d::Size(obj_width, obj_size.height));
}

void Math_Object::fixWidthObject() {
    //Get object image real size.
    auto obj_size = this->getContentSize();
    float obj_height = obj_size.width * _image->getVirtualRendererSize().height / _image->getVirtualRendererSize().width;
    setObjectSize(cocos2d::Size(obj_size.width, obj_height));
}


cocos2d::DrawNode* Math_Object::getDrawer() {
    if (_drawer == nullptr) {
        _drawer = cocos2d::DrawNode::create();
        this->addChild(_drawer, 5);
        _drawer->setPosition(cocos2d::Vec2::ZERO);
    }
    return _drawer;
}


