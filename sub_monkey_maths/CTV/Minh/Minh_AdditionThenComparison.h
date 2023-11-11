//
// Minh_AdditionThenComparison.h
//
//

#ifndef Minh_AdditionThenComparison_h
#define Minh_AdditionThenComparison_h

#include <stdio.h>
#include "Math_Quiz.h"

struct minh_opearator {
	std::vector<std::string> _ops;
	std::vector<int> _nums;
	int result;
};

class Minh_AdditionThenComparison : public Math_Quiz
{
public:
    static Minh_AdditionThenComparison* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    //void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
	std::vector<std::string> operation; // Vector chứa các 
	int number_min;
	int number_max;
	int number_operation_min;
	int number_operation_max;
	
	int number_operator;

	cocos2d::Size object_size;


	// Danh sach cac doi tuong 
	int obj_id = 0;
	std::unordered_map<int, minh_opearator> _operators;
	std::unordered_map<int, Math_Object *> _operators_object;
	math::comparision::Key key_compare = math::comparision::Key::name;

	// danh sach number
	std::vector<math::resource::Image *> numbers_img_list;

	//Danh sach cac doi tuong keo tha.
	std::vector<Math_Object*> _dragers;
	std::string display;

    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
	// debugging mode

private:
	void showObjectWithText(Math_Object * objec);
	minh_opearator genOperator();
	Math_Object * genOperatorObject(minh_opearator ope);
	void CloneDrager(Math_Object * object);
};

#endif /* Minh_SetTheCalculation */
