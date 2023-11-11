//
//  EscapeMazeQuiz.h
//  MJQuiz
//
//  Created by Tunado on 8/28/17.
//
//

#ifndef EscapeMazeQuiz_h
#define EscapeMazeQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "MazeCreator.h"
#include "PatternCreator.h"

USING_NS_CC;

class EscapeMazeQuiz : public Math_Quiz
{
public:
	static EscapeMazeQuiz* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;

	void autoTestingRun(float dt);
private:

	~EscapeMazeQuiz();

    struct MazeConfig {
        int column;
        int row;
        int start;
        int end;
        int size;
    };
    
    PatternCreator* pattern = nullptr;
    std::vector<MazeCreator*> the_mazes;
    std::vector<MazeConfig> mazes_list;
    
    std::vector<cocos2d::ui::ImageView*> img_views;
    std::vector<cocos2d::ui::ImageView*> img_circles;
    
    std::vector<Math_Object*> maze_paths;

    float object_size;
    std::string select_type;
    
    bool init() override;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> img_list_fuzzy;
    
    std::vector<std::vector<math::resource::Image*>> img_list_same;

    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;

    int testing_step = 0;
    int index_path = 0;
    int number_kind;
    int pattern_length;
    
    void endingMazes(float dt);
    int index_ending = 0;
    
    Math_Object*  last_object = nullptr;
    
    void addEvents();
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;
    
    Math_Object* touchOnObject(cocos2d::Vec2 point);

    bool moveable;

    Math_Object* getTarget();
    void activeTarget(Math_Object* object);
};


#endif /* DragObjectToMatrixProperties_Nam.h */
