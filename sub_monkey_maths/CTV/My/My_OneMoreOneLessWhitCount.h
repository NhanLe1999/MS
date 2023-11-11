#ifndef __My_OneMoreOneLessWhitCount__
#define __My_OneMoreOneLessWhitCount__

#include "Math_Quiz.h"

class My_OneMoreOneLessWhitCount : public Math_Quiz {
public:
	static My_OneMoreOneLessWhitCount* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;

	void onTouchObject(cocos2d::Node* node);
	void onTouchGroup(cocos2d::Node* node);
private:
	bool init() override;
    
    enum GameType {
        More,
        Less,
        Equal
    };
    
    GameType game_type;

	int number_counting = 0;
	bool display_number;
    int number_object;
    int number_question;
	std::string game_Type;
	//Số lượng object trong mỗi group;
	std::vector<int> number_objects;

    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    bool counting = true;
    Math_Group* temp_group = nullptr;

	math::resource::Image* group_bg = nullptr;

	float objects_width;
};

#endif //_TOCUH_OBJECT__
