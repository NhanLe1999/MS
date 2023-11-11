//
// Minh_EstimationQuantity.h
//
//

#ifndef Minh_EstimationQuantity_h
#define Minh_EstimationQuantity_h

#define ESTIMATE_NEAR 0.2f
#define ESTIMATE_LOW 0.6f
#define ESTIMATE_HIGHT 0.8f

#include <stdio.h>
#include "Math_Quiz.h"


class Minh_EstimationQuantity : public Math_Quiz
{
public:
    static Minh_EstimationQuantity* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //Game config
	int est_num;
    int number_about;
	int est_shown;

	std::vector<Math_Object *> est_objects;
	std::vector<Math_Object *> show_objects;
	int condition;

	std::string answer;
	std::string icon_same;
	std::string icon_filter;
	
	// Danh sach cac doi tuong
	cocos2d::Size object_size = cocos2d::Size(80, 80);
	cocos2d::Size group_size = cocos2d::Size(450, 400);

	// Groups
	math::comparision::Key key_compare = math::comparision::Key::name;
	// Danh sach icons
	std::vector<math::resource::Image *> list_icons;

	// Danh sách icons sử dụng trong game
	std::vector<math::resource::Image *> kind_list;

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

#endif /* Minh_EstimationQuantity */
