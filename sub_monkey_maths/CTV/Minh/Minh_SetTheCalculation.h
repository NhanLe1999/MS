//
//  Minh_SetTheCaculation.h
//
//

#ifndef Minh_SetTheCaculation_h
#define Minh_SetTheCaculation_h

#include <stdio.h>
#include "Math_Quiz.h"

class Minh_SetTheCaculation : public Math_Quiz
{
public:
    static Minh_SetTheCaculation* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    //void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
	int math_result;
	bool is_carry;
	bool sequence;
	int num_operand;
	std::string op_type;
	int range_minimum;
	int range_maximum;

	std::vector<int> operand_values;
	std::vector<Math_Object * > _operators;
	std::vector<std::vector<Math_Object *> > objects_list;


	cocos2d::ui::ImageView * equal_sign = nullptr;

	cocos2d::Size object_size = cocos2d::Size(120,120);
	cocos2d::Size drager_size = cocos2d::Size(90, 90);

//	int number_drager;
	math::comparision::Key key_compare = math::comparision::number;
	math::resource::Image* img_bg = nullptr;

	//danh sach image operator
	std::vector<math::resource::Image*> operators_list;
	std::vector<int> digits;
	//danh sach so luong cho moi loai obj
	std::vector<int> number_list;

	Math_Slot * sequence_slot;
	int number_slot;

	//Danh sach cac doi tuong keo tha.
	std::vector<Math_Object*> _dragers;
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
	// debugging mode

private: 
	void GenOperand();
	void GenOperandDigits();
	void CloneDrager(Math_Object * object);
	void RemoveDrager(Math_Object * object);
	void ShowValueTextOnly(Math_Object * object, std::string position = "middle");
};

#endif /* Minh_SetTheCaculation */
