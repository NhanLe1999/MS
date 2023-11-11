#pragma once
#include <string>
#include <vector>
#include "cocos2d.h"
#include "Math_Libs.h"

class QuestionInfo
{
public:
	std::vector <std::string> getParts();
	void setQuestionParts(const std::vector<std::string> &_questionParts);
	std::vector <std::string> getQuestionParts();
	std::string choiceAt(const int &index);
	int getNumChoice();	
	void setNumChoice(const int &_numChoice);
	void setChoices(const std::vector<std::string> &_choices);	
	void setSentence(std::string textSentence, std::string pahtAudioSentence, std::vector<math::resource::Sync_Text> listHilight);

	std::string getAnswerSoundAt(int index);	
	std::vector <std::string> getSentenceSound();
	void setSentenceSound(std::vector <std::string> _str);

	std::string getTextSentencecc();

	std::string getPathAudioSentence();

	std::vector<math::resource::Sync_Text> getValueListHilightTextSentence();

	int getNumBlank();

	std::vector<std::string> answersss;

	std::vector<std::string> answerSound;
    std::string question_vi = "";
    std::unordered_map<std::string, std::string> textsAndSounds;
    
protected:
	std::vector <std::string> questionParts;
	int numChoice;
	std::vector <std::string> choices;	
	
	std::vector <std::string> sentenceSound;
	std::string _textSentence = "";
	std::string _pahtAudioSentence = "";
	std::vector<math::resource::Sync_Text> _listHilight = {};

	
};

