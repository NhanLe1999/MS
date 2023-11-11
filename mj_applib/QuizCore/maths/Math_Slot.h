//
//  Math_Slot.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/13/17.
//
//

#ifndef Math_Slot_hpp
#define Math_Slot_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"

#include "Math_Libs.h"
#include "Math_Object.h"

class Math_Slot : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(Math_Slot);

    static Math_Slot* createSlot();
    void setEndble(int id, math::resource::Image* bg);
    void setDisable();
    bool isEnable();
    
    int getId();
    bool isUsed();
    
    /**
     *  @brief Kiem tra slot
     *  
     *  Kiem tra xem diem co nam trong slot hay k?
     *  @param point vi tri tren man hinh (touch->location)
     *  @return true neu point nam trong slot.
     **/
    bool isAvailable(cocos2d::Vec2 point, int padding=0);
    void setComparisionValue(math::comparision::Value comparision);
    
    bool isCorrect(math::comparision::Key key);
    
    //Slot
    void setSlotSize(cocos2d::Size size);
    void fitSlotSize();
    
    //background
    void setBackground(math::resource::Image* image, float padding=0);
    void setBackgroundVisible(bool visible);
    void setBackgroundColor(cocos2d::Color3B color);
    cocos2d::ui::ImageView* getBackground();
    math::resource::Image* getBackgroundSource();
    
    //Text
    void setValueText(std::string text);
    std::string getValueText();
    void setValueTextVisible(bool visible);
    cocos2d::ui::Text* getValueLabel();
    
    //Linker
    void setLinker(Math_Object* object);
    void removeLinker();
    Math_Object* getLinker();
    
    //Animation
    void onShow(float dt, math::animation::ObjectShowType show = math::animation::ScaleUp);
    void onHide(float dt, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);
    
private:
    cocos2d::ui::ImageView* _background = nullptr;
    cocos2d::ui::Text* _lbl_value = nullptr;
    math::resource::Image* _bg_src = nullptr;

    bool _enable;
    int _id;
    bool _used;
    
    Math_Object* _linker = nullptr;
    math::comparision::Value _comparision;
    
    bool init() override;
    
    
    cocos2d::DrawNode* drawer;
    
};
CREATE_READER_CLASS(Math_SlotReader, Math_Slot);


#endif /* Math_Slot_hpp */
