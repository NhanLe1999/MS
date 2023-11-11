#include "QuestionInfo.h"

using namespace std;

vector <string> QuestionInfo::getParts() {
	vector <string> ans;
	for (auto part : this->questionParts) {
		if (part != "#") {
			ans.push_back(part);
		}
	}
	return ans;
}

string QuestionInfo::choiceAt(const int &index) {
	return this->choices[index];
}

int QuestionInfo::getNumChoice() {
	return this->numChoice;
}

void QuestionInfo::setQuestionParts(const std::vector<std::string> &_questionParts) {
	this->questionParts.clear();
	for (auto questionPart : _questionParts) {
		this->questionParts.push_back(questionPart);
	}
}

std::vector<std::string> QuestionInfo::getQuestionParts()
{
	return questionParts;
}

void QuestionInfo::setNumChoice(const int &_numChoice) {
	this->numChoice = _numChoice;
}

void QuestionInfo::setChoices(const std::vector<std::string> &_choices) {
	this->choices.clear();
	for (auto choice : _choices) {
		this->choices.push_back(choice);
	}
}

void QuestionInfo::setSentence(std::string textSentence , std::string pahtAudioSentence, std::vector<math::resource::Sync_Text> listHilight)
{
	this->_listHilight = listHilight;
	this->_pahtAudioSentence = pahtAudioSentence;
	this->_textSentence = textSentence;
}

std::string QuestionInfo::getTextSentencecc()
{
	return this->_textSentence;
}

std::string QuestionInfo::getPathAudioSentence()
{
	return this->_pahtAudioSentence;
}

std::vector<math::resource::Sync_Text> QuestionInfo::getValueListHilightTextSentence()
{
	return this->_listHilight;
}

std::string QuestionInfo::getAnswerSoundAt(int index) {
	return this->answerSound[index];
}

std::vector <std::string> QuestionInfo::getSentenceSound() {
	return this->sentenceSound;
}

void QuestionInfo::setSentenceSound(std::vector<std::string> _str) {
	this->sentenceSound = _str;
}

int QuestionInfo::getNumBlank() {
	int ans = 0;
	for (auto part : this->questionParts) {
		if (part == "#") {
			++ans;
		}
	}
	return ans;
}

