#include "DataParser.h"
#include "cocos2d.h"
#include "MJHelper.h"
#include "Math_Quiz.h"
#include "StoryConfigManager.h"

USING_NS_CC;
using namespace std;

DataParser::DataParser()
{
}

DataParser::DataParser(std::string json_path)
{
	jsonFileName = json_path;
}

void DataParser::init() {
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(jsonFileName);

	doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
	if (doc.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	doc.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif

	this->intros.clear();
    
    if (CONFIG_MANAGER->isGameVMonkey())
    {
        if(this->doc.HasMember("audio_intro_vi"))
        {
            std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
            auto audio_intro = this->doc["audio_intro_vi"].GetArray();
            
            for (int i = 0; i < (int)audio_intro.Size(); i++)
            {
                auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
                auto path =  img_audio->getAudios("name_1").front();
                
                if(!path.empty())
                {
                    this->intros.push_back(path);
                }
            }
        }
    }
    else
    {
        this->intros.push_back("sounds/waterpipe/1st.mp3");
        this->intros.push_back("sounds/waterpipe/2nd.mp3");
    }

}

int DataParser::getNumQuest() {
	CCLOG("%d", this->doc[this->data].Size());
	return this->doc[this->data].Size();
}


void DataParser::setIntros(std::vector<std::string> _intros) {
	this->intros = _intros;
}

std::vector<std::string> DataParser::getIntros() {
	return this->intros;
}

void DataParser::parseDataIntoQuestionInfoInLevel(QuestionInfo *des, const int &level) {

	rapidjson::Value& currentLevel = this->doc[this->data][level];	

	vector <string> question; question.clear();
	vector <string> choice; choice.clear();
	// read answers
	std::stringstream ss;
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	
	std::vector <std::string> sentenceSounds;
	for (rapidjson::SizeType i = 0; i < currentLevel["word"].Size(); i++) {
		rapidjson::Value& item_info = currentLevel["word"][i];
		std::string img_id = item_info["item"].GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		auto text = image->getText("name_1");
		auto audio = image->getAudios("name_1").front();
		bool is_text = false;
		if (item_info.HasMember("is_text") ){
			is_text = item_info["is_text"].GetBool();
		};
		
		if (is_text) {
			des->answersss.push_back(text);
			des->answerSound.push_back(audio);
			choice.push_back(text);
			auto ques = ss.str();
			ss.str("");
			ss.clear();
			if (!ques.empty()) question.push_back(ques);
			question.push_back("#");
			sentenceSounds.push_back("#");
		}
		else {
			ss << text << " ";
			sentenceSounds.push_back(audio);
			//question.push_back(text);
		}
		
	}
	// end
	auto ques = ss.str();
	if (!ques.empty()) question.push_back(ques);
	// read wrong answer
	for (rapidjson::SizeType i = 0; i < currentLevel["wrong_answer"].Size(); i++) {
		std::string img_id = currentLevel["wrong_answer"][i].GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		auto text = image->getText("name_1");
		auto audio = image->getAudios("name_1").front();
		choice.push_back(text);
        
        des->textsAndSounds.insert({text,audio});
	}
	
	//std::vector <std::string> sentenceSounds;
	//for (int i = 0; i < (int)currentLevel[this->sentenceSound].Size(); i++) {
	//	string str = currentLevel[this->sentenceSound][i].GetString();
	//	sentenceSounds.push_back(str);
	//}

	if (auto isSentence = JSON_SAFE_CHECK(currentLevel, "sentence", String))
	{
		auto sentence = currentLevel["sentence"].GetString();

		auto imageSentence = math::resource::ResHelper::getImage(images, sentence);

		std::string textSentence = imageSentence->getText();

		std::string audioPath = imageSentence->getAudios().front();

		auto listHilight = imageSentence->getSyncText();
		des->setSentence(textSentence, audioPath, listHilight);
	}

	//auto sentence = currentLevel["sentence"].GetString();
	// slipt question
	mj::helper::shuffle(choice);
	des->setQuestionParts(question);
	des->setChoices(choice);
	des->setSentenceSound(sentenceSounds);
	des->setNumChoice(choice.size());
    
}




