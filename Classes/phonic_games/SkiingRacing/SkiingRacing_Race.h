
#ifndef SkiingRacing_Race_HPP
#define SkiingRacing_Race_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
struct SkiingRacingQuestion {
    std::vector<std::pair<std::string,std::string>> answers;
	std::string correctAnswer;
	std::string correctAudio;
};
class SkiingRacing_RaceDelegate {
public:
	virtual void onClickedDelegate(cocos2d::Ref *sender, ui::Text* text, int id) {};
};

class SkiingRacing_Race : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(SkiingRacing_Race);

	static SkiingRacing_Race * createRace();
	static SkiingRacing_Race * createRace(SkiingRacingQuestion question,int numberOfQuestion);
	void playEffect(bool correct);
	void setDelegate(SkiingRacing_RaceDelegate* delegate);
private:
	void didLoadFromCSB();
	void didLoadFromCSB(SkiingRacingQuestion question, int numberOfQuestion);
	SkiingRacingQuestion _question;
	
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	//    bool _water_splash = false;
	
	SkiingRacing_RaceDelegate* _delegate;
	
	
};

CREATE_READER(SkiingRacing_Race);

#endif
