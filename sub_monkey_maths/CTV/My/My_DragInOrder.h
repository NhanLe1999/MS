#ifndef My_DragInOrder_h
#define My_DragInOrder_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class My_DragInOrder : public Math_Quiz {
public:
    static My_DragInOrder* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart() override;
    void startQuestion() override;
    void onExplanation(bool correct) override;
    void onComplete() override;
    void autoTesting() override;
    
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onTouchedObject(cocos2d::Node* node);
    
private:
    bool init() override;
    
    int count = 0;
    int number_object;
    cocos2d::Size object_size;
    std::string value;
    std::string background;
    int min;
	int distance;
    int vt;
    
    std::string icon;
    
    //Biến lưu yêu cầu bài toán (lớn -> nhỏ hay ngược lại)
    std::string order;
    
    //Biến thể hiện kiểu đếm (trái ->phải hay ngược lại)
    std::string count_style;
    
    int slots_number;
    
    int testing_step = 0;
    
    math::resource::Image* slot_bg = nullptr;
	math::resource::Image* dot_bg = nullptr;
    Math_Object* dot = nullptr;
    math::resource::Image* dot_img = nullptr;
    
    //Vector hình ảnh hiển thị ở object
    std::vector<math::resource::Image*> object_icons;
    
    //vector lưu bcakground của slots
    std::vector<math::resource::Image*> slot_backgrounds;
    
    //Vector lưu âm thanh các objects
    std::vector<std::string> object_voices;
    
    //Vector lưu vị trí random của các object khi rơi xuống
    std::vector<std::pair<cocos2d::Vec2, float>> object_position;
    
    void readNumber(int index);
    
    std::vector<int> test;
    
    void autoTestingRun(float delaytime);
};

#endif /* My_DragInOrder_h */

