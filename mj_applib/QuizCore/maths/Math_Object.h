//
//  Math_Object.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/10/17.
//
//

#ifndef Math_Object_hpp
#define Math_Object_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"

#include "Math_Libs.h"
#include "Math_Func.h"

class Math_Object : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(Math_Object);
    
    //init
    static Math_Object* createObject();
    void setEnable(int id, math::common::TouchPermission touch, math::resource::Image* image, math::resource::Image* bg, bool color=false, bool copy=false);
    void setDisable();
    bool isEnable();
    
    //state
    
    /**
     Setup clone from other

     @param object Object goc
     */
    CC_DEPRECATED_ATTRIBUTE void cloneFrom(Math_Object* object);
    
    /**
     Clone Object

     @return Đối tượng cloned
     */
    Math_Object* clone();
    
    /**
     Enabe drag object. Lưu trạng thái bắt đầu

     @param root node cha
     */
    void enableDrag(Node* root);
    
    /**
     Update trạng thái bắt đầu

     @param root node cha
     */
    void updateRoot(Node* root);
    
    /**
     Đánh dâu object khi được add vào group

     @param id id trong group
     @param group Group
     @param kind Kind trong group
     */
    void addToGroup(int id, Node* group, int kind);
    
    /**
     Set các hàm callback khi move

     @param began_func Call khi băt dầu move
     @param ended_func Call khi kết thúc move
     */
    void setMoveCallback(math::callback::onTouchObjectBegan began_func, math::callback::onTouchObjectEnded ended_func);
    
    /**
     Set hamc callback khi Chạm vào object

     @param touch_func Call khi touch vào object
     */
    void setTouchCallback(math::callback::onTouchObjectBegan touch_func);
    
#pragma mark - setter & getter
    int getID();
    math::common::TouchPermission getTouch();
    
    void setComparisionValue(int size=0);
    void setComparisionValue(std::string key, int comparision_value);
    void setComparisionValue(std::string key, std::string property);

    math::comparision::Value getComparisionValue();
    void setTouch(math::common::TouchPermission touch);
    
    void setSelected(bool selected);
    bool isSelected();
    void setSelectedAnimation(math::common::SelectedAnimation animation);
    
    void setCopied(bool copied);
    bool isCopied();
    
    void setColoring(bool coloring);
    bool isColoring();
    
    int getGroupID();
    int getKind();
    
    void setLocked(bool lock);
    bool isLocked();
    
    void setAllowMove(bool allow);
    bool isAllowMove();
    
    void setMoveDirection(int md);
    int getMoveDirection();
    
#pragma mark -
    /**
     Set Object size. nếu compare size càng lớn, image càng bé.

     @param size Kích thước
     @param align_bot căn chỉnh dưới?
     */
    void setObjectSize(cocos2d::Size size, bool align_bot = false);
    void fitObjectSize();
    
    /**
     Set object without background

     @param size kich thuước
     */
    void setObjectFullSize(cocos2d::Size size);
    
    //image
    void setImage(math::resource::Image* image);
    void setImageVisible(bool visible);
    void setImageColor(std::string color_name);
    void setImageColor(cocos2d::Color3B color);
    cocos2d::ui::ImageView* getImage();
    math::resource::Image* getImageSource();
    
    //background
    void setBackground(math::resource::Image* image, float padding=0);
    void setBackgroundVisible(bool visible);
    void setBackgroundColor(std::string color_name);
    void setBackgroundColor(cocos2d::Color3B color);
    cocos2d::ui::ImageView* getBackground();
    math::resource::Image* getBackgroundSource();
    
    //value
    void setValueText(std::string text);
    void setValueTextByKey(std::string key);
    std::string getValueText();
    void fitValueTextSize();
    cocos2d::ui::Text* getLabelValue();
    void showValueTextWithAnimation(bool animated);
    void hideValueTextWithAnimation(bool animated);
    
    void setValueTextPosition(std::string position);
    
    //compare
    bool isSame(Math_Object* other);
    bool isEqual(Math_Object* other, math::comparision::Key key);
    bool isEqual(math::comparision::Value comparision, math::comparision::Key key);
    bool isEqual(cocos2d::Value value, math::comparision::Key key);
    bool isEqual(math::resource::Image* img, math::comparision::Key key);

    int compare(Math_Object* other, math::comparision::Key key);
    int compare(math::comparision::Value comparision, math::comparision::Key key);
    int compare(cocos2d::Value value, math::comparision::Key key);
    int compare(math::resource::Image* img, math::comparision::Key key);

    //detected
    void setTouchDetectedType(math::common::DetectedType type);
    
    //control move
    /**
     *  @brief move object về vị trí ban đầu được set.
     */
    void backToStart();
    
    /**
     *  @brief move object về vị trí hiện tại.
     *  Sử dụng khi kéo thả object. Vị trí hiện tại là vị trí trước khi kéo thả.
     */
    void backToLast(int z_order, bool group = false);
    cocos2d::Vec2 getStartPosition();
    cocos2d::Vec2 getSeedPosition();
    
    void backLastGroup();
    void backStartGroup();
    
    void onShow(float dt, math::animation::ObjectShowType show = math::animation::ScaleUp, int delta = 0);
    void onHide(float dt, math::animation::ObjectShowType revert_show = math::animation::ScaleUp, int delta = 0);
    
    void onFlipDown(float dt);
    void onFlipUp(float dt);
    
    //testing
    void onTouched();
    void onTouchedAt(cocos2d::Vec2 point);
    bool isTouchedAt(cocos2d::Vec2 point);
    
    /**
     *  @brief Lưu trạng thái hiện tại của object
     */
    void captureState();
    
    /**
     *  @brief khôi phục object về trạng thái đã lưu
     */
    void restoreCapturedState();
    
    /**
     *  @brief Fix object size theo chiều cao.
     *  Một số ảnh chủ đề 'height' có xu hướng height >> width.
     *  Hàm này để resize object theo height.
     */
    void fixHeightObject ();

    /**
     *  @brief Fix object size theo chiều rộng.
     *  Một số ảnh chủ đề 'length' có xu hướng width >> height.
     *  Hàm này để resize object theo width.
     */
    void fixWidthObject ();
    
    bool isDetected(cocos2d::Vec2 point);
    
    void setSuperNode(cocos2d::Node* node);
    
    cocos2d::DrawNode* getDrawer();
    
    //new animation
    void onTouchObject(bool correct, float delay = 0, int opacity = 255);
    void onDropObject(float delay = 0);

    /**
     Play audio

     @param key prop key
     */
    void playAudio(std::string key = "name_1");

private:
    //Properties
    int _id;
    math::common::TouchPermission _touch;
    math::comparision::Value _comparision_value;

    bool _enable;
    bool _selected;
    bool _copied;
    bool _coloring;
    
    Node* _group=nullptr;
    int _kind;
    
    //drag
    Node* _root_group=nullptr;
    int _root_index;
    Node* _root = nullptr;
    Node* _parent = nullptr;
    cocos2d::Vec2 _root_pos;
    cocos2d::Vec2 _seed_pos;
    int _root_z;
    int _seed_z;
    
    Node* super_node = nullptr;
    
    //ui
    cocos2d::ui::ImageView* _background = nullptr;
    cocos2d::ui::ImageView* _image= nullptr;
    cocos2d::ui::Text* _lbl_value= nullptr;
    math::resource::Image* _img_src = nullptr;
    math::resource::Image* _bg_src = nullptr;
    
    std::string _position_txt = "middle";
    math::animation::CapturedNode _captured_node;
    
    //Touch events
    bool _locked;
    bool _allow_move;
    int _move_dir;
    
    cocos2d::Action* action_selected=nullptr;
    gaf::GAFObject* select_eff = nullptr;
    
    //touch detected
    cocos2d::Image* _image_touch=nullptr;
    math::common::DetectedType _detected_type=math::common::box;
    math::common::SelectedAnimation _selected_animation = math::common::scale;
    
    //callback
    math::callback::onTouchObjectBegan _func_touched = nullptr;
    math::callback::onTouchObjectBegan _func_touch_began = nullptr;
    math::callback::onTouchObjectEnded _func_touch_ended = nullptr;
    
    bool init() override;
    void addEvents();
    int number_add_events=0;
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;

    cocos2d::DrawNode * _drawer = nullptr;
    
};
CREATE_READER_CLASS(Math_ObjectReader, Math_Object);

#endif /* Math_Object_hpp */
