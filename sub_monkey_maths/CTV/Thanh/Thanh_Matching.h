//
//  Thanh_Matching.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_Matching_h
#define Thanh_Matching_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_Matching : public Math_Quiz
{
public:
    enum GameMode{
        total,
        same
    };
    enum ObjectOption{
        image,
        text,
        number
    };

    static Thanh_Matching* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;
private:
    bool init() override;

    // Game config
    int number_pair;
    int max_number;
	math::comparision::Key _key_comparision;
	bool touch_disable;


    // Object config
    bool object_reuse;
    cocos2d::Size object_size;
    math::resource::Image* object_background = nullptr;
    ObjectOption object_option_1, object_option_2;

    //Slot config
    cocos2d::Size slot_size;
    math::resource::Image *slot_background = nullptr;

    // Question config
    int equal;
    GameMode _mode;

    

    // danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_1, icons_2;

    //Danh sach slot doi tuong
    std::vector<Math_Slot*> slots;

    // Danh sách đối tượng
    std::vector<Math_Object*> objects;
    std::vector<Math_Object*> list_a, list_b;

    //callback Func
    void onAnswerDone(bool correct);

    //debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);


    // button
    Math_Answer* btn_done = nullptr;

    // event 
    bool onlyTouch; // kiểm soát touch hay move, nếu move vẫn trong object thì chỉ coi là touch

    std::queue<int> queue_obj; // kiểm soát số lượng object đã touch

    bool isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node* object); // trả về true nếu điểm nằm trong object và ngược lại

    std::vector<cocos2d::DrawNode*> drawer;
	std::vector<std::pair<cocos2d::Node*, cocos2d::ui::ImageView*> > my_pair; 
    int index;  // vị trí của object selected
    int pass;   // vị trí của object được chọn khi move đến.
    int obj_id, gr_id;
    std::vector<int> status; // kiểm soát object còn nối được không

    void setSelected(bool selected, Math_Object* object);

    // ham trong game
    /**
     * @brief kiểm tra 2 object có bắt căcpj được với nhau
     * tham sô là vị trí của object trong list node
     **/
    bool isCorrect(int i, int j); 
    /**
     * @brief nối 2 object bằng dây nối drawnode
     * @param i object 1
     * @param j object 2
     **/
    void connect(int i, int j);
    /**
     * @brief tạo ra list giá trị cho game
     * @param a list giá trị của cột a
     * @param b list giá trị của cột b
     **/ 
	std::vector<int> values_a, values_b;
};

#endif /* Thanh_Matching_h */
