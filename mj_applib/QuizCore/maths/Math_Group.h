//
//  Math_Group.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/17/17.
//
//

#ifndef Math_Group_hpp
#define Math_Group_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "Math_Object.h"

class Math_Group : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(Math_Group);
    
    static Math_Group * createGroup();
    void setEnable(int g_id, std::vector<Math_Object*> list, math::group::GroupType type, int cap);
    bool isEnable();
    void setDisable(float delay = 0, math::animation::ObjectShowType revert_show = math::animation::SlideRight, math::animation::ObjectShowType revert_itemshow = math::animation::ScaleUp);
    
    int getId();
    void enableToch(math::callback::onTouchObjectBegan func);
    void onHide(float delay = 0, math::animation::ObjectShowType revert_show = math::animation::SlideRight, math::animation::ObjectShowType revert_itemshow = math::animation::ScaleUp);
    void onShow(float delay = 0, math::animation::ObjectShowType show = math::animation::SlideRight, math::animation::ObjectShowType itemshow = math::animation::ScaleUp);
    
    void setObjectSize(cocos2d::Size size);
    void setGroupSize(cocos2d::Size size);
    void setAlignObjects(math::group::GroupAlign align);
    void setArrangeObjects(math::group::GroupArange arrange);
    
    //Make object to frame
    void setGroupFrames(int column, int row, math::resource::Image* img, math::resource::Image* bg_img = NULL);
    
    //Make object to queue
    void setGroupQueue (cocos2d::Size size);
    
    cocos2d::ui::ImageView* getBackground();
    void setBackground(math::resource::Image* img, float padding=0.3);
    void setForeground(math::resource::Image* img, float padding=0.3);
    void setBackgroundVisiable(bool visiable);
    void setForegroundVisiable(bool visiable);
    
    /**
     *  @brief Cập nhật vị trí.
     *  Sắp xêp các object trong group.
     *  @param no_free bỏ qua cap nhat vi trí.
     *  @param animated move khi thay đổi vị trí.
     */
    void updatePosition(bool no_free, bool animated);
    
    
    //name
    void setGroupName(std::string name, int position = 0);
    cocos2d::ui::Text* getNameText();
    void showNameText(bool animated);
    void hideNameText(bool animated);
    
    //value
    void setValue(std::string value);
    cocos2d::ui::Text* getValueText();
    void showValueText(bool animated);
    void hideValueText(bool animated);
    
    //Animation
    void onTouchedAnimation (bool correct);
    
    //Group+Object
    /**
     *  @brief Kiem tra obj co thuoc group hay k?
     *  @return yes neu thuoc group.
     **/
    bool hasObject(Math_Object* object);
    
    /**
     *  @brief Lay so obj trong group
     *  @return So object.
     **/
    int getNumberObjects();
    
    /**
     *  @brief Lay index cua obj trong group
     *  @return index cua obj hoac -1 neu obj khong nam trong group.
     **/
    int getIndexOfObject(Math_Object* object);
    
    /**
     *  @brief Lay object trong group tai index=i
     *  @return object neu tim thay, hoac nullptr neu k tim thay.
     **/
    Math_Object* getObjectAtIndex(int i);
    
    /**
     *  @brief Xoa object khoi group.
     *  @return true neu tim thay object va xoa thanh cong.
     **/
    bool removeObject(Math_Object* object, bool seed = false);
    
    /**
     *  @brief Xóa tất cả object khỏi group
     **/
    void removeAllObjects();
    
    /**
     *  @brief Add Obj vao cuoi group.
     *  Neu so luong object da full, k add.
     *  @return true khi add thanh cong
     **/
    bool pushBackObject(Math_Object* object);
    
    /** 
     *  @brief Lay obj cuoi cua group.
     *  Dong thoi xoa no khoi group
     *  @return A Match_Object*
     **/
    Math_Object* popBackObject();
    
    /**
     *  @brief Add Obj vao dau group.
     *  Neu so luong object da full, k add.
     *  @return true khi add thanh cong
     **/
    bool pushFrontObject(Math_Object* object);
    
    /**
     *  @brief Lay obj dau tien group.
     *  Dong thoi xoa no khoi group
     *  @return A Match_Object*
     **/
    Math_Object* popFrontObject();
    
    /**
     *  @brief Add object vao vi tri keo tha.
     *  Neu so luong object da full, k add.
     *  @return true khi add thanh cong
     **/
    bool addObject(Math_Object* object, cocos2d::Vec2 point);
    
    //Check compare
    void setComparisionValue(math::comparision::Value compare_value);
    void setComparisionKey(math::comparision::Key compare_key);
    
    /**
     *  @brief Kiem tra Logic Object.
     *  So sanh object voi comparision value cua Group.
     *  @return true neu bang nhau.
     **/
    bool isMatchWithGroup(Math_Object* object);
    bool isMatchGroup ();
    
    /**
     *  @brief Kiem tra Logic Object.
     *  So sanh voi obj dau tien cua group.
     *  Neu group rong~ tra ve true.
     *  @return true neu bang nhau
     **/
    bool isMatchWithOther(Math_Object* object);
    bool isMatchGroupWithOther ();
    
    //control
    /**
     *  @brief Kiem tra vi tri cham cho nam tren group.
     *  @param touch location, vi tri cham tren man hinh.
     *  @return true neu point nam trong group.
     **/
    bool isTouchOnGroup(cocos2d::Vec2 point);
    
    /**
     *  @brief Convert diem cham sang diem tren group.
     *  @param touch location, vi tri cham tren man hinh.
     *  @return point trong group.
     **/
    cocos2d::Vec2 convertToGroupPosition(cocos2d::Vec2 point);
    
    /**
     *  @brief Animation add to group
     *
     **/
    void onAddObjectSuccess(Math_Object* object);
    
    
    //testing
    /**
     *  @brief Dùng cho testing.
     *  touch Group in code.
     **/
    void onTouchOnGroup();
    
    
    //Add code ver 2.1
    //Config vung keo thả
    /**
     *  @brief set keo tha theo check touch image
     *  Nếu điểm chạm nằm trong image check touch thì mới return true.
     */
    void setCheckTouchImage(math::resource::Image* img);

    /**
     *  @brief set show/hide check touch image
     *  Hiển thị hoặc ẩn ảnh dùng để check touch
     */
    void setVisibleCheckTouchImage(bool visible);
    

    //Add code ver 2.1.1
    //Add các methods về sắp xếp object trong group.
    //Tối ưu group, thêm điều kiện...
    
    /**
     *  @brief truyền vào vị trí do người dùng config.
     *  Khi đó, group sẽ đặt vị trí các object theo thứ tự trong list.
     */
    void setCustomPosition(std::vector<cocos2d::Vec2> positions);
    void removeCustomPosition();
    
    cocos2d::Vec2 getPositionAtFrame(int col, int row);
    cocos2d::Vec2 getPositionAtIndex(int index);

    /**
     *  @brief Add object về lại group.
     *  Dùng khi game move object vào group. trong trường hợp group nguồn và group đích trùng nhau.
     *  @param object đối tượng kéo thả.
     */
    void addBackObject(Math_Object* object);
    
    //Queue group
    /**
     *  @brief Queue group.
     *  Là dạng đặc biệt của group. Object được xếp chỉ trên 1 hàng và liên tiếp nhau.
     *  Những object mà slot cạnh bên trống mới có thể move được.
     *  Hạn chế move ra ngoài group.
     */
    void setQueueGroup (int cap, math::resource::Image* bg_img = NULL);
    void setQueueGroupWithLists (std::vector<Math_Object *> list, std::vector<Math_Object *> list2);

    /**
     *  @brief Queue group.
     *  Là dạng đặc biệt của group. Object được xếp chỉ trên 1 hàng và liên tiếp nhau.
     *  Những object mà slot cạnh bên trống mới có thể move được.
     *  Hạn chế move ra ngoài group.
     */
    enum ItemAlign {
        ItemAlignLeft,
        ItemAlignRight,
        ItemAlignBottom,
        ItemAlignTop
    };
    void setItemsAlign(ItemAlign align);
    
    //Setcolor
    void setGroupBackgroungColor(cocos2d::Color3B color);
    void setGroupForegroungColor(cocos2d::Color3B color);

    void setGroupBackgroungOpacity(GLubyte opac);
    void setGroupForegroungOpacity(GLubyte opac);

    void setSuperNode(cocos2d::Node* node);
    
    void clippedBackgroundEdge(int edge);
    
private:
    bool init() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onGroupClicked(cocos2d::Ref*);
    
    cocos2d::ui::ImageView* _background = nullptr;
    cocos2d::ui::ImageView* _foreground = nullptr;
    cocos2d::ui::ImageView* _check_touch = nullptr;
    
    cocos2d::ui::Text* _txt_name = nullptr;
    cocos2d::ui::Text* _txt_value = nullptr;
    cocos2d::ui::Layout* _panel = nullptr;
    cocos2d::ui::Button* _button = nullptr;
    
    math::resource::Image* _bg_src = nullptr;
    math::resource::Image* _fg_src = nullptr;
    math::resource::Image* _touch_src = nullptr;
    
    std::vector<Math_Object*> _objects;
    math::comparision::Value _compare_value;
    math::comparision::Key _compare_key;
    
    math::group::GroupType _group_type;
    math::group::GroupAlign _group_align = math::group::random;
    math::group::TouchDetect _touch_type;
    math::group::GroupArange _group_arange = math::group::mix;
    
    math::callback::onTouchObjectBegan _callback_func;
    
    cocos2d::Node* super_node = nullptr;
    
    bool _enable;
    bool _custom_position;
    int _id;
    int _group_cap;
    std::vector<cocos2d::Vec2> _locations;
    std::vector<int> _indexes;
    cocos2d::Size _object_size=cocos2d::Size(150, 150);
    int _frame_col;
    int _frame_row;
    
    std::vector<cocos2d::ui::ImageView*> _frames;
    
    //Sap xep trong group
    void swapObjects(Math_Object* object1, Math_Object* object2);
    void onFullWarning();
    
    
};
CREATE_READER_CLASS(Math_GroupReader, Math_Group);

#endif /* Math_Group_hpp */
