#ifndef __My_VennDiagram_H__
#define __My_VennDiagram_H__

#include "Math_Quiz.h"

class My_VennDiagram : public Math_Quiz {
public:
	static My_VennDiagram* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point);
	void onAnswerDone(cocos2d::Node* node);

private:
	bool init() override;

	int number_group;
	int count = 0;
	int number_object;

	bool clickable = false;

	int number_object_1;
	int number_object_2;
	int number_same_1;
	int number_same_2;

	std::vector<math::resource::Image*> icon_1;
	std::vector<math::resource::Image*> icon_2;
	std::vector<math::resource::Image*> icon_same_1;
	std::vector<math::resource::Image*> icon_same_2;

	std::string prop_1;
	std::string prop_2;

	cocos2d::Size object_size;

	math::resource::Image* group_bg = nullptr;
	math::resource::Image* venn_left = nullptr;
	math::resource::Image* venn_right = nullptr;
	math::resource::Image* venn_mid = nullptr;
	math::resource::Image* venn = nullptr;

	std::vector<math::resource::Image*> venn_imgs;

	Math_Object* venn_diagram = nullptr;

	Math_Answer* answer = nullptr;

};

#endif // !__My_VennDiagram_H__
