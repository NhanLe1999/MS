#pragma once
#include "json/document.h"
#include "json/rapidjson.h"
#include "QuestionInfo.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


class DataParser
{
public:
	DataParser();
	DataParser(std::string json_path);

	void init();
	int getNumQuest();
	void parseDataIntoQuestionInfoInLevel(QuestionInfo *des, const int &level);

	void setIntros(std::vector<std::string> _intros);
	std::vector<std::string> getIntros();

protected:
	rapidjson::Document doc;
	std::string myData;

private:
	std::string jsonFileName = "config.json";
	const std::string blankSymbol = "#";
	const char* numQuestion = "numQuestion";
	const char* data = "data";
	const char* questions = "sentence";
	const char* numChoice = "numChoice";
	const char* choices = "choices";
	const char* answers = "answers";

	const char* answerSound = "answerSound";
	const char* sentenceSound = "sentenceSound";

	std::vector<std::string> intros;
};

