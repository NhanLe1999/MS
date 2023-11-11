//
// Minh_EstimateNearestNumber.h
//
//

#ifndef Minh_EstimateNearestNumber_h
#define Minh_EstimateNearestNumber_h

#define ESTIMATE_NEAR 0.2f
#define ESTIMATE_LOW 0.6f
#define ESTIMATE_HIGHT 0.8f

#include <stdio.h>
#include "Math_Quiz.h"


class Minh_EstimateNearestNumber : public Math_Quiz
{
public:
    static Minh_EstimateNearestNumber* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
	
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
	void onTouchGroup(Node * node);
private:
    bool init() override;
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    //Game config

	
	int est_num;

	int num_groups;
	std::string group_type;
	cocos2d::Size object_size;
	cocos2d::Size group_size;
	int delta;

	std::string answer;
	std::string icon_same;
	std::string icon_different;
	
	int obj_id = 0;
    int number_about;
	// Danh sach so luong cac doi tuong

	std::vector<int> number_est;
    std::vector<int> number_est_ques;

    int number_answer;

	math::comparision::Key key_compare = math::comparision::Key::name;

	// Danh sách icons sử dụng trong game
	std::vector<math::resource::Image *> list_icons;
	math::resource::Image * obj_img = nullptr;
    //callback Func
	void onTouchedAnswer(cocos2d::Node * node);
    bool clickable=true;
    
	// debugging mode
	int testing_step = 0;
	//Smart autotest
	//debugging mode
	void autoTestingRun(float dt);

	// Game 
	private:
	std::vector<Math_Object * > GenObjects(int num);


};

#endif /* Minh_EstimateNearestNumber */
