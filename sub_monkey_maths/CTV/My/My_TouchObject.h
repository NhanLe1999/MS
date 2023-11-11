#ifndef __My_TouchObject__
#define __My_TouchObject__

#include "Math_Quiz.h"

class My_TouchObject : public Math_Quiz {
public:
	static My_TouchObject* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchObject(cocos2d::Node* node);
	void onTouchGroup(cocos2d::Node* node);
private:
	bool init() override;

	int number_counting = 0;
	bool display_number;

	//Kiểu Game (so sánh nhiều hay ít hơn)
	std::string game_Type;
	int number_group;
	//Số lượng object trong mỗi group;
	std::vector<int> number_objects;

	int result;
	int number_object;
	int distance;
	cocos2d::Size object_size;

	math::resource::Image* icon = nullptr;
	bool counting = true;

	math::resource::Image* group_bg = nullptr;

	float objects_width;
	math::resource::Image* obj_bg = nullptr;
	Math_Object* object_bg = nullptr;
	std::vector<Math_Object*> objects;

};

#endif ! //_TOCUH_OBJECT__
