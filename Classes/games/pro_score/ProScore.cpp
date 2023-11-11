#include "ProScore.h"
#include <algorithm>
#include "APProfileManager.h"
#include <thread>
#include "MJHelper.h"
#include "HSAudioEngine.h"
#include "MJDefault.h"
#include "StoryAlert.h"
#include "JsonDump.h"
#include "MsHelperGame.h"
#include "AskPermissionPopup.h"
#include "AnimationManager.h"
#include "manager/CleverTapManager.h"
#include "MJDefinitions.h"
#include "cctype"
#include "cstring"
#include "HSLib.h"
#include "rapidfuzz/fuzz.hpp"


using rapidfuzz::fuzz::ratio;


#define AUDIO_APPEAR	"sounds/commonSound/in.mp3"
#define AUDIO_IN		"sounds/commonSound/appear1.mp3"
#define AUDIO_MOVE		"sounds/commonSound/Move_slide.mp3"
#define AUDIO_TAP		"sounds/pro_score/tap.mp3"
#define AUDIO_ANALYZE	"sounds/pro_score/phantichdiem.mp3"
#define AUDIO_RATE		"sounds/pro_score/rate.mp3"
#define RETRY_TIMES			                    3
#define SHOW_POPUP_TIMES	                    3
#define KEY_TIME_SKIP_RECORD					"key_time_skip_record"
#define TEXT_COLOR                              Color3B(85, 85, 85)

std::vector<std::pair<int, int>> getTextIndexSentence(std::string text, std::vector<math::resource::Sync_Text> syncDatas) {
	std::vector<std::pair<int, int>> result;
	int startIndex = 0, endIndex = 0;
	text = mj::helper::to_lower(text);

	for (auto str : syncDatas) {
		auto word = mj::helper::to_lower(str.w);
		if (!std::isalpha(word[word.length() - 1])) {
			word.pop_back();
		}
		startIndex = text.find(word, startIndex ? startIndex + 1 : 0);
		endIndex = startIndex + word.length();
		if (startIndex == std::string::npos) return { {-1,-1} };
		result.push_back({ startIndex,endIndex });
		startIndex = endIndex;
	}
	return result;
}

void highLightText(cocos2d::ui::Text* text, cocos2d::Node* currentNode, cocos2d::Color4B textOriginColor, cocos2d::Color3B highlightCorlor, int index, std::vector<math::resource::Sync_Text> syncDatas, std::vector<std::pair<int, int>> vectorTextIndex, std::function<void(int,std::string)> callback)
{
	if (index == syncDatas.size()) {
		//restore text state
		text->setTextColor(textOriginColor);
		for (int i = 0; i < text->getString().length(); i++) {
			if (text->getLetter(i) != nullptr) {
				text->getLetter(i)->setColor(TEXT_COLOR);
			}
		}
		if (callback) {
			callback(-1,"");
			return;
		}
	}
	for (auto p : vectorTextIndex) {
		if (p.first < 0 || p.second < 0) {
			if (callback) {
				callback(-1, "");
			}
			return;
		}
	}
	if (index == 0) {//prepare text
		text->setTextColor(Color4B::WHITE);
		for (int i = 0; i < text->getString().length(); i++) {
			if (text->getLetter(i) != nullptr) {
				text->getLetter(i)->setColor(Color3B(textOriginColor));
			}
		}
	}

	std::vector<Sprite*> textElement;

	for (int i = vectorTextIndex[index].first; i <= vectorTextIndex[index].second; i++) {
		if (text->getLetter(i)) {
			textElement.push_back(text->getLetter(i));
		}

	}

	for (auto it = textElement.begin(); it != textElement.end(); it++) {
		if (*it != nullptr) (*it)->setColor(highlightCorlor);
	}
	float duration = (syncDatas.at(index).e - syncDatas.at(index).s) / 1000.0f;
	currentNode->scheduleOnce([=](float dt) {
		for (auto it = textElement.begin(); it != textElement.end(); it++) {
			if (*it != nullptr) (*it)->setColor(Color3B(textOriginColor));
		}
		highLightText(text, currentNode, textOriginColor, highlightCorlor, index + 1, syncDatas, vectorTextIndex, callback);
	}, duration, "waitSync" + std::to_string(index));

}

ProScore* ProScore::createGame(std::vector<math::resource::Image*> images) {
	ProScore* p = new ProScore();
	if (p) {

		p->autorelease();
        
        p->setName("ProScoreInstance");
		p->setTag(PRO_SOCRE_TAG);
		p->didLoadFromCSB(images);

		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

void ProScore::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    setCallbackCickbutton();
	_time = getTime();
	this->setName("interactive_question_gameAI");
    if(images.empty()) {
        
        return;
    }
    
	auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");

    if(buffer_string.empty()) {
        return;
    }
    
	rapidjson::Document value_data;

	value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

	if (value_data.IsNull() || value_data.HasParseError() || !value_data.IsObject()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	value_data.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif

    _usingElsaAPI = CleverTapManager::GetInstance()->getProductConfigBoolValue("isElsaApiUse");

	generateData(value_data.GetObject(), images);

	_screenSize = cocos2d::Director::getInstance()->getVisibleSize();

	_origin = Director::getInstance()->getVisibleOrigin();

	auto whiteBackground = cocos2d::ui::Layout::create();
	whiteBackground->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	whiteBackground->setBackGroundColor(cocos2d::Color3B::WHITE);
	whiteBackground->setContentSize(_screenSize);
	whiteBackground->setTouchEnabled(true);
	whiteBackground->setName("ProScoreInstance");
    this->addChild(whiteBackground, -1);

	if (auto  background = ui::ImageView::create("bg.jpg")) {
		background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		background->setScale9Enabled(true);
		background->setOpacity(0.35 * 255);
		background->setCapInsets(Rect(1, 1, _screenSize.width - 2, _screenSize.height - 2));
		background->setPosition(_screenSize / 2);
		this->addChild(background, -1);
		background->setName("gameBg");
	}

	_connectionErrorPopupShowed = false;
	_popupShowCount = 0;
	_gameType = GameType::SENTENCE;

    addSpeaker();
    
	loadUI();
    
}

ProScore::~ProScore()
{
    _callbackCliclbutton = nullptr;
}

void ProScore::onExit()
{
    ui::Layout::onExit();
    _callbackCliclbutton = nullptr;
    
}

void ProScore::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images) {

    if(!value_data.HasMember("lesson")||value_data["lesson"].IsNull()) {
        return;
    }

	auto list_object = value_data["lesson"].GetArray();
	
	for (int i = 0; i < (int)list_object.Size(); i++) {

		CCLOG("i = %d | size = %d", i, (int)list_object.Size());
		
        model::Card card;

		auto info = math::resource::ResHelper::getImage(images, list_object[i]["item"].GetString());
        
        if(!info) {
           
            continue;
        }
        
		card.text = info->getText();

		model::Audio	au;
		au.path			= info->getAudio();
		au.sync_data	= info->getSyncText();

		card.audios.push_back(au);

		model::Image ig;
		ig.path = info->path;
		card.images.push_back(ig);

		this->_listCard.push_back(card);
	}
}


void ProScore::onEnter() {
    
    ui::Layout::onEnter();
    
    try
    {
        CleverTapManager::GetInstance()->pushEvent("start_interactive_question_activity", {

                {"activity_name",cocos2d::Value(_info.name)},
                {"story_name",cocos2d::Value(_info.storyName)},
                {"story_id",cocos2d::Value(XHSLib->story_id)},
        });
    }
    catch (const std::exception&)
    {

    }
}

void ProScore::loadUI() {

    if(_listCard.empty()) {
        return;
    }
    
	_firstTy = true;
    
	this->setContentSize(_screenSize);

	_imageObject = ImageView::create(_listCard[_currentTurn].images.front().path);
	Size imageMaxSize = Size(this->getContentSize().width*0.8, this->getContentSize().height * 0.55);
	float scaleRate;
	if (_imageObject->getBoundingBox().size.width / imageMaxSize.width > _imageObject->getBoundingBox().size.height / imageMaxSize.height) {//scale by width
		scaleRate = imageMaxSize.width / _imageObject->getBoundingBox().size.width;
	}
	else {
		scaleRate = imageMaxSize.height / _imageObject->getBoundingBox().size.height;
	}
	_imageObject->setScale(scaleRate);
	_imageObject->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - _imageObject->getBoundingBox().size.height / 2 - 50));
	this->addChild(_imageObject);

	_textObject = cocos2d::ui::Text::create(_listCard[_currentTurn].text, FONT_NAME, mj::helper::getMaxFontSize({ _listCard[_currentTurn].text },Size(_screenSize.width*0.7,150),INT_MAX, FONT_NAME));
	_textObject->setTextColor(Color4B(TEXT_COLOR));
	_textObject->setPosition(Vec2(_imageObject->getPosition() + Vec2(0, -_imageObject->getBoundingBox().size.height / 2 - _textObject->getBoundingBox().size.height / 2 - 50)));
    _textObject->setTouchEnabled(false);
    _textObject->setEnabled(true);
    //_textObject->addClickEventListener(CC_CALLBACK_1(ProScore::onSoundButtonClicked,this));
    _textObject->addClickEventListener(CC_CALLBACK_1(ProScore::onSoundButtonClicked,this));

    this->addChild(_textObject);

/*
    auto whiteBackground = cocos2d::ui::Scale9Sprite::create("mjstory/new_support_ui/contact_bg.png");
    //whiteBackground->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
   // whiteBackground->setBackGroundColor(cocos2d::Color3B::WHITE);
    whiteBackground->setPosition(_textObject->getContentSize()/2.f);
    whiteBackground->setContentSize(_textObject->getContentSize() + Size(100.f,30.f));
   // whiteBackground->setTouchEnabled(true);
    whiteBackground->setName("whitebg");
    _textObject->addChild(whiteBackground, -1);
 */
    
	_textObject->setScale(0);
	_imageObject->setScale(0);
	_imageObject->runAction(
		Sequence::create(
			Spawn::create(
				EaseBackOut::create(ScaleTo::create(0.5, scaleRate)),
				CallFunc::create([] {
					AUDIO_ENGINE->play(AUDIO_APPEAR);
				}),
				nullptr
			),
			CallFunc::create([=] {
				AUDIO_ENGINE->play(AUDIO_APPEAR);
				_textObject->runAction(
					Sequence::create(
						EaseBackOut::create(ScaleTo::create(0.5, 1)),
						CallFunc::create([=]{
                            _currentStepCount++;
							_messengerForClevertap = "play_audio_" + to_string(_currentTurn+1);
							highlightText([=](int,std::string){
                                _textObject->setTouchEnabled(true);
								loadRecordButton();
								
							});
						}),
						nullptr
					)
				);
			}),
			nullptr
		)
	);


}

void ProScore::addSpeaker()
{
    if(_soundButton) {
        return;
    }
    const float k_sacleFactor = 1.2f;
    
    _soundButton = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    auto screenSize = cocos2d::Director::getInstance()->getWinSize();
    _soundButton->setPosition(Vec2(69*k_sacleFactor, Director::getInstance()->getWinSize().height-60));
    _soundButton->setName("soundButton");
    _soundButton->setScale(k_sacleFactor);
    _soundButton->addClickEventListener(CC_CALLBACK_1(ProScore::onSoundButtonClicked,this));
    this->addChild(_soundButton, INT_MAX-1);
}

void ProScore::loadRecordButton() {

    auto analySpine = SpineWrapper::createWithPath("spine/proscore/phan tich 1");
	analySpine->setName("analySpine");
	analySpine->setVisible(false);
    this->addChild(analySpine);
	analySpine->setPosition(this->getContentSize() / 2 + Size(0,-125));
	analySpine->playSequence("phan tich", true);

	_recordButton = SpineWrapper::createWithPath("spine/proscore/voice");
	_recordButton->updateAnimationPosition(_recordButton->getContentSize() / 2);
	_recordButton->setAnchorPoint(Vec2(0.5, 0.5));
	_recordButton->setPosition(_textObject->getPosition()+Vec2(0,50));
	_recordButton->playSequence("guiding", true);
	_recordButton->setScale(0);

	//_recordButton->setBackGroundColor(Color3B::RED);
	//_recordButton->setBackGroundColorType(BackGroundColorType::SOLID);
	//_recordButton->setBackGroundColorOpacity(100);

	this->addChild(_recordButton);

	_stopRecordButton = SpineWrapper::createWithPath("spine/proscore/record");
	_stopRecordButton->updateAnimationPosition(_stopRecordButton->getContentSize() / 2);
	_stopRecordButton->setAnchorPoint(Vec2(0.5, 0.5));
	_stopRecordButton->setAnchorPoint(Vec2(0.5, 0.5));
	_stopRecordButton->setPosition(_recordButton->getPosition());
	_stopRecordButton->playSequence("action", true);
	_stopRecordButton->setVisible(0);

//	_stopRecordButton->setBackGroundColor(Color3B::BLUE);
//	_stopRecordButton->setBackGroundColorType(BackGroundColorType::SOLID);
//	_stopRecordButton->setBackGroundColorOpacity(100);

	_stopRecordButton->addClickEventListener(CC_CALLBACK_1(ProScore::onStopRecord, this));
	this->addChild(_stopRecordButton);

	showRecordButton();

}
void ProScore::showRecordButton() {
	AUDIO_ENGINE->play(AUDIO_IN);
	_imageObject->runAction(
		Sequence::create(
			ScaleTo::create(0.5,0),
			CallFunc::create([=] {
				_textObject->runAction(
					Sequence::create(
						Spawn::createWithTwoActions(
							EaseBackOut::create(MoveBy::create(0.5, Vec2(0, 300))),
							CallFunc::create([] {AUDIO_ENGINE->play(AUDIO_MOVE); })
						),
						CallFunc::create([=] {
							AUDIO_ENGINE->play(AUDIO_APPEAR);
							_recordButton->runAction(
								Sequence::create(
									EaseBackOut::create(ScaleTo::create(0.5, 1)),
									CallFunc::create([=] {
										_recordButton->setTouchEnabled(true);
										_recordButton->addClickEventListener(CC_CALLBACK_1(ProScore::onRecordButtonClicked, this));
										//this->showTapGuideGame(_recordButton->getPosition(), INT_MAX, 3, 3);
									}),
									nullptr
								)
							);
						}),
						nullptr
					)
				);
			}),
			nullptr
		)
	);
	
}
void ProScore::onRecordButtonClicked(Ref*) {

	_currentStepCount++;
	_messengerForClevertap = "start_recording_" + to_string(_currentTurn + 1);

	if (!requestRecordPermistion()) {
		return;
	}

	if (_usingElsaAPI) {
		onRecordButtonClickedElsa(this);
		return;
	}

	_wasStopRecord = false;
    cocos2d::experimental::AudioEngine::stop(_idAudioQuestion);
    AUDIO_ENGINE->play(AUDIO_TAP);

	_recordButton->setVisible(false);
	_recordButton->setTouchEnabled(false);
	_stopRecordButton->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _stopRecordButton->setTouchEnabled(true);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _stopRecordButton->setTouchEnabled(true);
#endif

    for(auto l : _listTextTouchArea){
        l->removeFromParent();
    }
    _listTextTouchArea.clear();
	_listTextResponseData.clear();
	if (utils::findChild(this, "_scoreLayout")) {
		utils::findChild(this, "_scoreLayout")->removeFromParent();
	}
	for (auto line : _vectorLine) if (line) line->removeFromParent();
	_vectorLine.clear();
	if (utils::findChild(this, "_replayButton")) {
		utils::findChild(this, "_replayButton")->removeFromParent();
	}
    
    _textObject->setString(_listCard[_currentTurn].text);
    
	_textObject->setTextColor(Color4B::WHITE);
	for (int i = 0; i < _textObject->getString().length(); i++) {
		if(_textObject->getLetter(i))_textObject->getLetter(i)->setColor(TEXT_COLOR);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->runAction(Sequence::create(DelayTime::create(_maxDelayTimeRecord * 3), CallFunc::create([=]{
		if(!_wasStopRecord){
			_messengerForClevertap = "stop_recording_" + to_string(_currentTurn+1);
			onFinishRecordGoogle(4, "", "");
			CCLOG("Auto finished record");
		}
	}), nullptr));
	cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.SpeechRecognizerFragment", "StartListening");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	mj::helper::startSpeechToText([=](std::string textData){
		if(_callbackCliclbutton)
        {
            _callbackCliclbutton(textData);
        }
    });

	this->runAction(Sequence::create(DelayTime::create(_maxDelayTimeRecord * 2), CallFunc::create([=]{
		if(!_wasStopRecord){
			onStopRecord(this);
			_messengerForClevertap = "stop_recording_" + to_string(_currentTurn+1);
		}
	}), nullptr));

#endif
}

void ProScore::setCallbackCickbutton()
{
    _callbackCliclbutton = [=] (std::string textData) {
        std::string tempData = textData;
        std::string lastword_ofBabtTalk = getLastWordInString(tempData, " ");
        std::string tempData_ofStandard = _textObject->getString();
        std::string lastword_ofStandard = getLastWordInString(tempData_ofStandard, " ");
        if(lastword_ofBabtTalk == lastword_ofStandard){
            this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=]{
                if(!_wasStopRecord){
                    _messengerForClevertap = "stop_recording_" + to_string(_currentTurn+1);
                    onStopRecord(this);
                }
            }),nullptr));
        }
        CCLOG("data:%s",textData.c_str());
    };
    
}

void ProScore::onStopRecord(Ref*) {

    CCLOG("onStopRecord proscore");
    _currentStepCount++;
    _messengerForClevertap = "stop_recording_" + to_string(_currentTurn+1);

	if(_usingElsaAPI){
		_record->stopRecord();
		return;
	}

	_wasStopRecord = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.SpeechRecognizerFragment", "StopListening");
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//delay 0.5s
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=]{
    	mj::helper::stopSpeechToText([=](std::string textData){
        	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            	_messengerForClevertap = "stop_recording_" + to_string(_currentTurn+1);
        		onFinishRecordGoogle(4,"",textData);
        	});
        	CCLOG("data:%s",textData.c_str());
    	});
    }), nullptr));
#endif
}

void ProScore::onFinishRecordGoogle(int finish_type, std::string url, std::string messenge){
	CCLOG("onFinishRecordGoogle");
	_wasStopRecord = true;
	std::string absolutePath = url;
	std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	path = absolutePath;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	path = url;
#endif
	CCLOG("+++++++++++++++++++++++++++%s", url.c_str());
	CCLOG("+++++++++++++++++++++++++++%s", absolutePath.c_str());
	_recordPath = path;
	switch (finish_type)
	{
		case MSRecord::RECORD_TYPE::RECORD_DONE:
		{
			_stopRecordButton->setVisible(false);
			_stopRecordButton->setTouchEnabled(false);
			_recordButton->setVisible(true);
			_recordButton->playSequence("idle");
			AUDIO_ENGINE->play("sounds/pro_score/stop-ghiam.mp3");
			sendRequest(messenge);
			break;
		}
		case MSRecord::RECORD_TYPE::PLAY_DONE:
		{
			if (!_isCallApi) {
				if (utils::findChild(this, "_replayButton")) {
					_replayButton->setTouchEnabled(true);
				}
				break;
			}
			sendRequest(messenge);
			break;
		}
		default:
			break;
	}
}

void ProScore::onFinishRecord(int finish_type, std::string url) {
    std::string absolutePath = url;
	std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	path = absolutePath;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	path = url;
#endif
	CCLOG("+++++++++++++++++++++++++++%s", url.c_str());
	CCLOG("+++++++++++++++++++++++++++%s", absolutePath.c_str());
	_recordPath = path;
	switch (finish_type)
	{
	case MSRecord::RECORD_TYPE::RECORD_DONE:
	{
		_stopRecordButton->setVisible(false);
		_stopRecordButton->setTouchEnabled(false);
		_recordButton->setVisible(true);
		_recordButton->playSequence("idle");
		AUDIO_ENGINE->play("sounds/pro_score/stop-ghiam.mp3");
		sendRequestElsa(path);
		break;
	}
	case MSRecord::RECORD_TYPE::PLAY_DONE:
	{
		if (!_isCallApi) {
            if (utils::findChild(this, "_replayButton")) {
                _replayButton->setTouchEnabled(true);
            }
			break;
		}
		sendRequestElsa(path);
		break;
	}
	default:
		break;
	}
}
void ProScore::addAnalyzeLayout() {
    auto blackLayout = Layout::create();
    blackLayout->setName("analyzeLayout");
    blackLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    blackLayout->setBackGroundColor(Color3B::BLACK);
    blackLayout->setBackGroundColorOpacity(255/2);
    blackLayout->setContentSize(this->getContentSize());
    blackLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(blackLayout,INT_MAX);
    blackLayout->setPosition(this->getContentSize() / 2);
    blackLayout->setTouchEnabled(true);
    blackLayout->setSwallowTouches(true);

	auto analyzeSpine = SpineWrapper::createWithPath("spine/proscore/phan tich 1");
	analyzeSpine->setName("analyzeSpine");
    blackLayout->addChild(analyzeSpine);
	analyzeSpine->setPosition(this->getContentSize() / 2 + Size(0,-125));
	analyzeSpine->playSequence("phan tich", true);
	AUDIO_ENGINE->playSound(AUDIO_ANALYZE,true);
}
void ProScore::removeAnalyzeLayout(){
    if(auto blackLayout = utils::findChild<Layout*>(this, "analyzeLayout")){
        blackLayout->removeFromParent();
        AUDIO_ENGINE->stopSound(AUDIO_ANALYZE);
    }
    
}
void ProScore::onAnalyzeDone(int score, std::vector<std::pair<std::string, float>> wordScore) {
	AUDIO_ENGINE->play(scoreToAudioRate(score));
	createScoreBoard(score);

	//add replay button(ear button)
	_replayButton = Button::create("games/proscore/ear.png", "games/proscore/ear.png");
	_replayButton->addClickEventListener([=](Ref*) {
		_isCallApi = false;
		if(_usingElsaAPI){
            _record->startPlay(false);
		}
		_replayButton->setTouchEnabled(false);
	});
	_replayButton->setAnchorPoint(Vec2(0.5, 0.5));
	_replayButton->setTouchEnabled(true);
    _replayButton->setVisible(_usingElsaAPI);
	_replayButton->setSwallowTouches(true);
	this->addChild(_replayButton);
	_replayButton->setName("_replayButton");
	
	//highlight word score
	if(_usingElsaAPI){
		highlightWordScore(wordScore);
	}
	else{
		highlightWordScoreGoogle(wordScore);
	}

	_replayButton->setPosition(_textObject->getPosition() + Vec2(_textObject->getBoundingBox().size.width / 2 + 50, 0));

	if (score < 35 && _firstTy) {
		_recordButton->setTouchEnabled(true);
		_recordButton->playSequence("guiding", true);
	}
	else {

        _countTrackingEvents++;

		//show nextGameButton 
		_nextGameButton = ImageView::create("games/proscore/nextButton.png");
		_nextGameButton->setTouchEnabled(true);
		_nextGameButton->addClickEventListener([=](Ref*) {
            
            _currentTurn++;
            
            if(_currentTurn >= _listCard.size()) {

                _textObject->setTouchEnabled(false);
            }
            
            _nextGameButton->setTouchEnabled(false);
            
            if( _currentTurn  >= _listCard.size()) {
                
                if(_soundButton) {
                    _soundButton->setTouchEnabled(false);
                }
                
                if(_replayButton) {
                    _replayButton->setTouchEnabled(false);
                }
            }
            
			nextGameButtonClicked();
		
        });
		_nextGameButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_nextGameButton->setPosition(_recordButton->getPosition());
		this->addChild(_nextGameButton);
		//this->showTapGuideGame(_nextGameButton->getPosition(),INT_MAX,5,5);
		_recordButton->setVisible(false);
	
	}

	_firstTy = false;
    
    _textObject->removeChildByName("whitebg");
    
    if(_usingElsaAPI) {
        
        addTextTouchArea();

    }
}
std::string ProScore::scoreToMessage(int score) {
	std::vector<std::string> vectorMsg;
	if (score < 35) {
		vectorMsg = _firstTy ? MSG_VERRY_BAD_FIRST_TIME : MSG_VERRY_BAD_SECOND_TIME;
	}
	else if (score >= 35 && score < 40) {
		vectorMsg = MSG_MEDIUM;
	}
	else if (score >= 40 && score < 80) {
		vectorMsg = MSG_PRETTY_GOOD;
	}
	else if (score >= 80 && score <= 100) {
		vectorMsg = MSG_GOOD;
	}

	return  vectorMsg.at(cocos2d::random(0, (int)vectorMsg.size() - 1));
}
Color3B ProScore::scoreToColor(int score) {
	if (score < 20) return Color3B::RED;
	if (score >= 20 && score < 60) return Color3B::ORANGE;
	if (score >=60) return Color3B(14, 168, 69);//green
	return Color3B(14, 168, 69);//green
}
std::string ProScore::scoreToEmojiAction(int score) {
	if (score < 20) return "emoji_01";
	if (score >= 20 && score < 40) return "emoji_02";
	if (score >= 40 && score < 60) return "emoji_03";
	if (score >= 60 && score < 80) return "emoji_04";
	if (score >= 80) return "emoji_05";
	return "emoji_05";
}

std::string ProScore::messageToAudioPath(std::string message) {
	return "sounds/pro_score/"+message + std::to_string(random<int>(1, 2)) + ".mp3";
}


std::string ProScore::scoreToAudioRate(int score) {
	if (score < 20) return "sounds/pro_score/end-sai.mp3";
	if (score >= 20 && score < 60) return "sounds/pro_score/end-dung it.mp3";
	if (score >= 60) return "sounds/pro_score/end-dung.mp3";
    return "sounds/pro_score/end-dung.mp3";
}
void ProScore::clearStage(std::function<void()> callback) {

	std::vector<Node*> children = { _nextGameButton,_imageObject,_textObject,_recordButton,_replayButton,_scoreLayout };
	//children.insert(children.end(), _vectorLine.begin(), _vectorLine.end());
	for (int i = 0; i < children.size();i++) {
		if (children[i]) {
			children[i]->runAction(
				Sequence::create(
					DelayTime::create(i * 0.15),
					Spawn::create(
						EaseBackIn::create(ScaleTo::create(0.5, 0)),
						CallFunc::create([=] {
							if(children[i]->isVisible())AUDIO_ENGINE->play(AUDIO_MOVE);
						}),
						nullptr
					),
//					RemoveSelf::create(),
                    CallFunc::create([=]{
                        children[i]->removeFromParent();
                    }),
					nullptr
				)
			);
		}
	}
	this->scheduleOnce([=](float) {
		for (int i = 0; i < _vectorLine.size(); i++) {
			auto line = _vectorLine[i];
			line->runAction(
				Sequence::create(
					Spawn::create(
						EaseBackIn::create(MoveBy::create(0.5, Vec2(0, (i % 2 ? 1 : -1) * _screenSize.height))),
						CallFunc::create([=] {if(i==0)AUDIO_ENGINE->play(AUDIO_MOVE); }),
						nullptr
					),
					RemoveSelf::create(),
					nullptr
				)
			);
		}
	},0.5 + children.size() * 0.15, "waitDeleteChildren");

	this->scheduleOnce([=](float) {
		_vectorLine.clear();
		_listTextResponseData.clear();
		for (auto a : _listTextTouchArea) a->removeFromParent();
		_listTextTouchArea.clear();
		if (callback) callback();
	},0.5+children.size()*0.15 + 1.5,"waitDeleteWidget");
}
std::string ProScore::simplifyText(std::string msg) {
	auto charIndex = msg.find('!');
	std::string str1 = msg.substr(0, charIndex+1);
 	return str1;
}

void ProScore::createScoreBoard(int score, std::string customMessage) {
	_scoreLayout = Layout::create();
	auto emojiSpine = SpineWrapper::createWithPath("spine/proscore/emoji");
	auto msgInfo = scoreToMessage(score);
	std::string message = std::to_string(score) + "%: " + simplifyText(msgInfo);
	if (!customMessage.empty()) {
		message = customMessage;
	}
	Text* msg = Text::create(message,FONT_NAME, 50);
	msg->setTextColor(Color4B(scoreToColor(score)));

	msg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	msg->setTextVerticalAlignment(TextVAlignment::CENTER);
	msg->setTextHorizontalAlignment(TextHAlignment::LEFT);

	emojiSpine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	emojiSpine->updateAnimationPosition(emojiSpine->getContentSize() / 2);

	auto h = emojiSpine->getContentSize().height > msg->getContentSize().height ? emojiSpine->getContentSize().height : msg->getContentSize().height;
	_scoreLayout->setContentSize(Size(msg->getContentSize().width + emojiSpine->getContentSize().width + 20, h));
	_scoreLayout->addChild(emojiSpine);
	_scoreLayout->addChild(msg);
	emojiSpine->setPosition(emojiSpine->getContentSize() / 2);

	msg->setPosition(emojiSpine->getPosition() + Vec2(emojiSpine->getContentSize().width / 2, 0) + Vec2(20, 0));
	_scoreLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_scoreLayout->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - _scoreLayout->getBoundingBox().size.height / 2 - 50));
	this->addChild(_scoreLayout);
	_scoreLayout->setName("_scoreLayout");
	emojiSpine->playSequence(scoreToEmojiAction(score), true);

	_scoreLayout->setScale(0);
	_scoreLayout->runAction(
		Sequence::create(
			EaseBackOut::create(ScaleTo::create(0.5, 1)),
			CallFunc::create([=] {
				if(customMessage.empty())AUDIO_ENGINE->play(messageToAudioPath(msgInfo));
			}),
			nullptr
		)
	);
	
}

void ProScore::sendRequest(std::string url) {
    removeAnalyzeLayout();
	addAnalyzeLayout();

    this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]{
        onRequestSucceedGoogle(url);
    }), nullptr));

}
ResponseData ProScore::extractData(std::string dataValue){
    CCLOG("proscore api response datas : %s",dataValue.c_str());
    ResponseData resData;
    rapidjson::Document value_data;
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(dataValue.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", dataValue.c_str());\
        return ResponseData();
    }
    std::string  status = JSON_SAFE_EXTRACT(value_data, "status", String, "");
    resData.status = status;
    std::string  message = JSON_SAFE_EXTRACT(value_data, "message", String, "");
    resData.message = message;
    if(JSON_SAFE_CHECK(value_data, "data", Object)){
        auto data = value_data["data"].GetObject();
        
        auto score = JSON_SAFE_EXTRACT(data, "score", Float, 0);
        if(score == 0){
            score = JSON_SAFE_EXTRACT(data, "score", Int, 0);
        }
        resData.score = score;
		if (JSON_SAFE_CHECK(data, "phonemes", Array)) {
			for (int i = 0; i < data["phonemes"].GetArray().Size(); i++) {
				std::string word = JSON_SAFE_EXTRACT(data["phonemes"][i], "text", String, "");
                if(word.empty()) continue;
				float wordscore = JSON_SAFE_EXTRACT(data["phonemes"][i], "score", Float, 0);
                if(wordscore == 0){
                    wordscore = JSON_SAFE_EXTRACT(data["phonemes"][i], "score", Int, 0);
                }
				std::transform(word.begin(), word.end(), word.begin(), ::tolower);
				resData.wordScore.push_back({ word,wordscore });
			}
		}
    }
	//fakeData(resData);
	_responseData = resData;
	loadTextScoreData();
    return resData;
}
void ProScore::loadTextScoreData() {
	ResponseData responseData;
	for (int i = 0; i < _responseData.wordScore.size();i++) {
        if (_responseData.wordScore[i].first == " " ) {
            _listTextResponseData.push_back(responseData);
            responseData = ResponseData();
            continue;
        }
		responseData.sentence += _responseData.wordScore[i].first;
		responseData.wordScore.push_back({ _responseData.wordScore[i].first ,_responseData.wordScore[i].second });
        
        if (i==_responseData.wordScore.size()-1) {
            _listTextResponseData.push_back(responseData);
        }
	}
    
    if(_listTextResponseData.size()==1){
        _listTextResponseData.front().score =_responseData.score;
        return;
    }
    
    //calculate average word score
    for(auto &response : _listTextResponseData){
        int sum =0;
        for(auto wordScore : response.wordScore){
            sum+=wordScore.second;
        }
        response.score = sum/response.wordScore.size();
    }
}
void ProScore::onRequestSucceedElsa(std::string responseData){
	ResponseData data = extractData(responseData);
	presentScore(data.score, data.wordScore);
}

void ProScore::onRequestSucceedGoogle(std::string responseData) {

	string temp = responseData;

	string textobject = _textObject->getString();

	mj::helper::removeCharsFromString(temp, ",;\"@#!$%^&*()_-+=~|\\?/><.");// remove special character "
	mj::helper::removeCharsFromString(textobject, ",;\"@#!$%^&*()_-+=~|\\?/><.");// remove special character "

    temp= mj::helper::to_lower(temp);
    
    textobject= mj::helper::to_lower(textobject);

	// find diff char
	rapidfuzz::Editops editops = rapidfuzz::indel_editops(temp, textobject);

	// khôi phục tại chuỗi sau khi loại bỏ kí tự lạ
	textobject = _textObject->getString();

	ResponseData data_temp;

	for(int i = 0; i < textobject.size(); i++){
		std::string curletter = textobject.substr(i, 1);
		data_temp.wordScore.push_back({curletter, 90});
	}

	std::string listErText = "";
	for (int i = 0; i < editops.size(); i++) {
		if (editops[i].type == rapidfuzz::EditType::Insert || editops[i].type == rapidfuzz::EditType::Replace || editops[i].type == rapidfuzz::EditType::Delete) {
			int erPos = editops[i].dest_pos;
			if(erPos < textobject.size()){
				std::string letter = textobject.substr(erPos, 1);
				data_temp.wordScore[erPos] = {letter, 10};
				CCLOG("vi tri %i: %s", erPos, letter.c_str());
				listErText = listErText + "pos_" + to_string(erPos) + ": " + letter + " ";
			}
		}
	}

	// tinh diem
    float erLenght = 0;
	for(int i = 0; i < data_temp.wordScore.size(); i++){
	    if(data_temp.wordScore[i].second < 11){
            erLenght++;
        }
	}
	if(textobject.substr(textobject.size()-1, 1) == "."){
	    erLenght++;
	}
    float lenght_text_object = textobject.size();
    float score = (lenght_text_object - erLenght)/lenght_text_object;
    if(score < 0) score = 0;
    score = score*100;

	presentScore((int)score, data_temp.wordScore);
}

void ProScore::onConnectionError(){
	removeAnalyzeLayout();
	if (!_connectionErrorPopupShowed) {
		_connectionErrorPopupShowed = true;

		auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
		auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
		alert->setNetworkDownContent(false, true);
		alert->setOkButtonCallback([=]() {
		
			createScoreBoard(0, "Network Error, Try Again!");
			//			highlightWordScore({ {_listCard[_currentTurn].text,0} });
			for (int i = 0; i < _textObject->getString().length(); i++) {
				if (_textObject->getLetter(i)) {
					_textObject->getLetter(i)->setColor(Color3B::RED);
				}
			}
			_recordButton->setTouchEnabled(true);
			_recordButton->playSequence("guiding", true);

		});

		alert->setCacelButtonCallback([=]() {

			createScoreBoard(0, "Network Error, Try Again!");
			//			highlightWordScore({ {_listCard[_currentTurn].text,0} });
			for (int i = 0; i < _textObject->getString().length(); i++) {
				if (_textObject->getLetter(i)) {
					_textObject->getLetter(i)->setColor(Color3B::RED);
				}
			}
			_recordButton->setTouchEnabled(true);
			_recordButton->playSequence("guiding", true);
		});

		alert->show();

	}
	else {
		createScoreBoard(0, "Network Error!");
//		highlightWordScore({ {_listCard[_currentTurn].text,0} });
        for(int i = 0 ; i < _textObject->getString().length();i++){
            if(_textObject->getLetter(i)){
                _textObject->getLetter(i)->setColor(Color3B::RED);
            }
        }
		showSkipButton();
	}
}

void ProScore::presentScore(float totalScore, std::vector<std::pair<std::string, float>> wordScore) {
	AUDIO_ENGINE->stopSound(AUDIO_ANALYZE);
	AUDIO_ENGINE->playSound(AUDIO_RATE);

    if (auto analyzeSpine = utils::findChild<SpineWrapper*>(this, "analyzeSpine")) {
        analyzeSpine->removeFromParent();
        SpineWrapper* presentScoreSpine = SpineWrapper::createWithPath("spine/proscore/phan tich 2");
        presentScoreSpine->setName("presentScoreSpine");
        presentScoreSpine->setPosition(this->getContentSize() / 2);
        auto blackLayout = utils::findChild(this, "analyzeLayout");
        blackLayout->addChild(presentScoreSpine);
        presentScoreSpine->playSequence(scoreToEmojiAction(totalScore), false, [=](SkeletonAnimation*, std::string) {
            utils::findChild(this, "analyzeLayout")->removeFromParent();
            onAnalyzeDone(totalScore, wordScore);
        });
    }
}

void ProScore::highlightWordScoreGoogle(std::vector<std::pair<std::string, float>> wordScore){
	std::string totalWords = "";
	for (int i = 0; i < wordScore.size();i++) {
		totalWords += wordScore[i].first;
	}
	auto textString = _textObject->getString();

	_isHighlightDataValid = (mj::helper::softCompareString(totalWords, textString));

	if(!_isHighlightDataValid) return;

	_textObject->setTextColor(Color4B::WHITE);

	float trimmer = 0;//trim length of bottom line 5 px
	auto currentIndex = -1;
	int size = wordScore.size();
	for (int i = 0; i < wordScore.size(); i++) {
        currentIndex++;
        if(currentIndex >= _textObject->getString().size())
        {
            continue;
        }
        
		float lossesWidth = 0;
		auto drawWidth = 0;
		auto wordColor = scoreToColor(wordScore[i].second);

        auto node = _textObject->getLetter(currentIndex);
        if(!node) {
            continue;
        }
        node->setColor(wordColor);
        drawWidth += node->getBoundingBox().size.width-trimmer;
        lossesWidth += trimmer;

		auto bottomLine = DrawNode::create(10);
		auto startDrawIndex = currentIndex - wordScore[i].first.length() + 1;
        if(startDrawIndex >= _textObject->getString().size())
        {
            continue;
        }
        
        auto drawNode = _textObject->getLetter(startDrawIndex);
        
        if(!drawNode) {
            continue;
        }
        
		auto startDrawPos = _textObject->convertToWorldSpace(drawNode->getPosition() + Vec2(-drawNode->getBoundingBox().size.width / 2+ lossesWidth/2,0));
		startDrawPos.y = _textObject->getPosition().y - _textObject->getBoundingBox().size.height / 2;
		

        bottomLine->drawLine(startDrawPos, startDrawPos + Vec2(drawWidth, 0), Color4F(wordColor));
		this->addChild(bottomLine);
		
        _vectorLine.push_back(bottomLine);
        
		if (wordScore[i].first.find('#') != std::string::npos) {
			_textObject->getLetter(currentIndex)->setVisible(false);
			bottomLine->setVisible(false);
		}
	}
}

void ProScore::highlightWordScore(std::vector<std::pair<std::string, float>> wordScore) {
	//check data
	std::string totalWords = "";
	for (int i = 0; i < wordScore.size();i++) {
		//change space to #
		std::replace(wordScore[i].first.begin(), wordScore[i].first.end(), ' ', '#');

		totalWords += wordScore[i].first;
	}
	//change space to #
	auto textString = _textObject->getString();

    std::replace(textString.begin(), textString.end(), ' ', '#');

	_isHighlightDataValid = (mj::helper::softCompareString(totalWords, textString));

    textString = _textObject->getString();

    mj::helper::removeCharsFromString(textString, ",;\"@#!$%^&*()_-+=~|\\?/><.");// remove special character "

    std::replace(textString.begin(), textString.end(), ' ', '#');

    if(!_isHighlightDataValid) return;

    _textObject->setString(textString);

	_textObject->setTextColor(Color4B::WHITE);

	float trimmer = 0;//trim length of bottom line 5 px
	auto currentIndex = -1;
	for (int i = 0; i < wordScore.size(); i++) {
		float lossesWidth = 0;
		auto drawWidth = 0;
		auto wordColor = scoreToColor(wordScore[i].second);
		for (int j = 0; j < wordScore[i].first.length(); j++) {
			currentIndex++;

            auto node = _textObject->getLetter(currentIndex);

            if(!node) {
                return;
            }

            node->setColor(wordColor);
			drawWidth += node->getBoundingBox().size.width-trimmer;
			lossesWidth += trimmer;
		}
		auto bottomLine = DrawNode::create(10);
		auto startDrawIndex = currentIndex - wordScore[i].first.length() + 1;
		auto startDrawPos = _textObject->convertToWorldSpace(_textObject->getLetter(startDrawIndex)->getPosition() + Vec2(-_textObject->getLetter(startDrawIndex)->getBoundingBox().size.width / 2+ lossesWidth/2,0));
//		startDrawPos.y = _textObject->getPosition().y - _textObject->getBoundingBox().size.height / 2 + 20;//update y
        startDrawPos.y = _textObject->getPosition().y - _textObject->getBoundingBox().size.height / 2;
		bottomLine->drawLine(startDrawPos, startDrawPos + Vec2(drawWidth, 0), Color4F(wordColor));
		this->addChild(bottomLine);
		_vectorLine.push_back(bottomLine);
		//disable '#' and it's bottom
		if (wordScore[i].first.find('#') != std::string::npos) {
			//found
			_textObject->getLetter(currentIndex)->setVisible(false);
			bottomLine->setVisible(false);
		}
	}

	_textObject->setTouchEnabled(true);
	//_textObject->addClickEventListener(CC_CALLBACK_1(ProScore::showScoreBoardDetails,this));
}
void ProScore::showScoreBoardDetails(Ref* sender, ResponseData textData){
//    if(textData.wordScore.size()!=textData.sentence.length()){
//        return;
//    }
    if(!_isHighlightDataValid) return;
	//this->hideTapGuideGame();#fixme
	auto blackLayout = Layout::create();
	blackLayout->setBackGroundColorType(BackGroundColorType::SOLID);
	blackLayout->setBackGroundColor(Color3B::BLACK);
	blackLayout->setBackGroundColorOpacity(255 / 2);
	blackLayout->setContentSize(this->getContentSize());
	blackLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(blackLayout);
	blackLayout->setPosition(this->getContentSize() / 2);
	blackLayout->setTouchEnabled(true);
	blackLayout->setSwallowTouches(true);

	auto scoreBoardDetails = ScoreBoardDetail::createScoreBoardDetail(textData.score, textData.sentence, textData.wordScore);
	scoreBoardDetails->setDelegate(this);
	this->addChild(scoreBoardDetails);
	scoreBoardDetails->setPosition(_screenSize / 2);
    
	ANIMATION_MANAGER->transitionSlideIn(AnimationManager::Direction::LEFT, 0.5, scoreBoardDetails);

	blackLayout->addClickEventListener([=](Ref*) {
		ANIMATION_MANAGER->transitionSlideOut(AnimationManager::Direction::LEFT, 0.25, scoreBoardDetails, [=] {
			scoreBoardDetails->removeFromParent();
			blackLayout->removeFromParent();
		});
	});
}

void ProScore::acceptPermissionCallback(){
    CCLOG("acceptPermissionCallback-------------------");

}

void ProScore::denyPermissioCallback(){
    CCLOG("denyPermissioCallback-----------");
	MJDEFAULT->setIntegerForKey(KEY_TIME_SKIP_RECORD, mj::helper::getTime());
    this->onFinishGame();
	//EVENT_MANAGER->logEvent("microphone_popup", {
	//	   std::make_pair("allow",cocos2d::Value(false)),
	//	   std::make_pair("source",cocos2d::Value("lesson"))
	//});
}

void ProScore::showSkipButton() {
	auto skipGameButton = ImageView::create("games/proscore/nextButton.png");
	skipGameButton->setTouchEnabled(true);
	skipGameButton->addClickEventListener([=](Ref*) {
        skipGameButton->setTouchEnabled(false);
		MJDEFAULT->setIntegerForKey(KEY_TIME_SKIP_RECORD, mj::helper::getTime());
		this->onFinishGame();
		});
	skipGameButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	skipGameButton->setPosition(_recordButton->getPosition());
	this->addChild(skipGameButton);
	_recordButton->setVisible(false);
}

void ProScore::replayAudio() {
	_isCallApi = false;
	_record->startPlay(false);
}

void ProScore::nextGameButtonClicked() {
	//this->hideTapGuideGame();#fixme
    
    _currentStepCount++;
    _messengerForClevertap = "click_next_" + to_string(_currentTurn+1);
	//_currentTurn++;
    clearStage([=]{
        if (_currentTurn == _listCard.size()) {
            this->onFinishGame();
        }
        else {
            this->loadUI();
        }
    });
	
}
void ProScore::fakeData(ResponseData& data) {
	data.score = random(0, 100);
	for (int i = 0; i < data.wordScore.size(); i++) {
		data.wordScore[i].second = random(0, 100);
	}
}
void ProScore::addTextTouchArea() {
	auto listWord = mj::helper::split(_listCard[_currentTurn].text, ' ');
	for (auto i = 0; i < listWord.size(); i++) {
		auto layout = Layout::create();
		auto text = Text::create(listWord[i], _textObject->getFontName(), _textObject->getFontSize());
		layout->setContentSize(text->getBoundingBox().size);
		layout->setAnchorPoint(Vec2::ZERO);
		_listTextTouchArea.push_back(layout);
		_textObject->getParent()->addChild(layout);
        layout->setTouchEnabled(true);
        layout->setSwallowTouches(true);
		layout->addClickEventListener([=](Ref* sender) {
            
            _messengerForClevertap = "click_result_" + to_string(_currentTurn+1);

            if(!_usingElsaAPI) {
                return;
            }
			showScoreBoardDetails(sender, _listTextResponseData[i]);
		});
		//layout->setBackGroundColorType(BackGroundColorType::SOLID);
		//layout->setBackGroundColorOpacity(100);
		//layout->setBackGroundColor(Color3B::RED);
	}
	auto originTextObjectAnchorPoint = _textObject->getAnchorPoint();

	mj::helper::setAnchorPointWithoutChangePosition(_textObject, Vec2::ANCHOR_BOTTOM_LEFT);
	
	auto beginPosition = _textObject->getPosition();
	mj::helper::setAnchorPointWithoutChangePosition(_textObject, originTextObjectAnchorPoint);
    
    if(_listTextTouchArea.empty()) {
        return;
    }
    
	_listTextTouchArea.front()->setPosition(beginPosition);
	if (_listTextTouchArea.size() == 1) return;
	auto sharp = Text::create("#", _textObject->getFontName(), _textObject->getFontSize());
	for (int i = 1; i < _listTextTouchArea.size(); i++) {
		_listTextTouchArea[i]->setPosition(_listTextTouchArea[i - 1]->getPosition() + Vec2(_listTextTouchArea[i - 1]->getBoundingBox().size.width, 0) + Vec2(sharp->getBoundingBox().size.width, 0));

	}
}
void ProScore::onSoundButtonClicked(Ref* sender) {
	
    auto currentSate = cocos2d::experimental::AudioEngine::getState(_idAudioQuestion);

    if (currentSate != cocos2d::experimental::AudioEngine::AudioState::PLAYING) {
        std::string cc = mj::helper::get_random_item(_listCard[_currentTurn].audios).path;
        _idAudioQuestion = AUDIO_ENGINE->play(cc, false, 1, [=]() {
            
        });
    }
}

void ProScore::highlightText(std::function<void(int auId, std::string auPath)> onHiglightDone) {

	switch (_gameType)
	{
	case ProScore::WORD:
	{
		_textObject->setTextColor(Color4B::RED);
        
        _idAudioQuestion =  AUDIO_ENGINE->play(_listCard[_currentTurn].audios.at(cocos2d::random(0, (int)_listCard[_currentTurn].audios.size() - 1)).path, false, 1, [=] {
			_textObject->setTextColor(Color4B(TEXT_COLOR));
			if (onHiglightDone) {
				onHiglightDone(-1,"");
			}
		});
		_maxDelayTimeRecord = _listCard[_currentTurn].audios.front().sync_data.back().e/1000.0f;
		CCLOG("thoi gian delay: %f", _maxDelayTimeRecord);
		break;
	}
	case ProScore::SENTENCE:
	{
		auto syncData = _listCard[_currentTurn].audios.front().sync_data;

        _idAudioQuestion = AUDIO_ENGINE->play(_listCard[_currentTurn].audios.front().path);

		_maxDelayTimeRecord = _listCard[_currentTurn].audios.front().sync_data.back().e/1000.0f;
		CCLOG("thoi gian delay: %f", _maxDelayTimeRecord);

		highLightText(_textObject, this, _textObject->getTextColor(),Color3B::RED, 0, syncData, getTextIndexSentence(_textObject->getString(),syncData), onHiglightDone);

		break;
	}
	default:
		break;
	}
}

void ProScore::onFinishGame() {

   
    
    pushStopCevertapEvent();
    pushFireBaseEventGameAI();

    if(_soundButton) {
        
        _soundButton->setVisible(false);
    }
    
    commonEndGame(ms::BaseActivity::EGT_COMMON);
}

void ProScore::pushFireBaseEventGameAI()
{
	float completedLesson = ((_currentTurn+1) / _listCard.size()) * 100;
	int time = getTime();

	MS_LOGGER.logEvent("interactive_question_gameAI", {
		{"completed",cocos2d::Value(completedLesson)},
		{"time_spent",cocos2d::Value(time - _time)},
	});
}

void ProScore::onCloseGame() {
	pushFireBaseEventGameAI();
    pushStopCevertapEvent();
}

long long ProScore::getTime() {
	auto timeSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return timeSinceEpoch;
}

string ProScore::getLastWordInString(string str, string delimiter)
{
	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		CCLOG("Word in String: %s", token.c_str());
		str.erase(0, pos + delimiter.length());
	}
	return str;
}

void ProScore::onRecordButtonClickedElsa(Ref *) {
	//this->hideTapGuideGame();#fixme

	cocos2d::experimental::AudioEngine::stop(_idAudioQuestion);

	AUDIO_ENGINE->play(AUDIO_TAP);

	_record = MSRecord::createRecord(_gameType == GameType::WORD ? 3 : 7, "audiomj.wav");

	if(!_record){
		//onPermissionDenied();
		return;
	}
	_recordButton->setVisible(false);
	_recordButton->setTouchEnabled(false);
	_stopRecordButton->setVisible(true);
	_stopRecordButton->setTouchEnabled(true);

	//this->hideTapGuideGame();//#fixme

	this->addChild(_record);
	_record->setDelegate(this);
	_record->setName("_record");

	//reset game states (for case try again)
	for(auto l : _listTextTouchArea){
		l->removeFromParent();
	}
	_listTextTouchArea.clear();
	_listTextResponseData.clear();
	if (utils::findChild(this, "_scoreLayout")) {
		utils::findChild(this, "_scoreLayout")->removeFromParent();
	}
	for (auto line : _vectorLine) if (line) line->removeFromParent();
	_vectorLine.clear();
	if (utils::findChild(this, "_replayButton")) {
		utils::findChild(this, "_replayButton")->removeFromParent();
	}

	_textObject->setString(_listCard[_currentTurn].text);

	_textObject->setTextColor(Color4B::WHITE);
	for (int i = 0; i < _textObject->getString().length(); i++) {
		if(_textObject->getLetter(i))_textObject->getLetter(i)->setColor(TEXT_COLOR);
	}

}

void ProScore::sendRequestElsa(std::string url){
	removeAnalyzeLayout();
	addAnalyzeLayout();

	std::vector<std::pair<std::string,std::string>> params;

	std::string text = _listCard[_currentTurn].text;

	mj::helper::removeCharsFromString(text, ",;\"@#!$%^&*()_-+=~|\\?/><.");// remove special character "

	params =
			{
					{"profile_id",std::to_string(PROFILE_MANAGER.getCurrentProfileId())},
					{"lesson_id","1"},
					{"sentence",text},
					{"audio_file",url},
			};

	cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_NETWORK,
												   [=](void * param) {

												   }, nullptr, [=]() {
				NativeBridge::callProscoreAPI(params, CC_CALLBACK_1(ProScore::onRequestSucceedElsa, this), CC_CALLBACK_0(ProScore::onConnectionError, this));
			});
}

bool ProScore::requestRecordPermistion() {

	auto permissionStatus =  MSRecord::getPermissionStatus();
	if(permissionStatus != MSRecord::GRANTED){
		_popupShowCount++;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "setDisableStartRecordingGameProscore");
#endif

		AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::RECORD,CC_CALLBACK_0(ProScore::acceptPermissionCallback, this),CC_CALLBACK_0(ProScore::denyPermissioCallback, this));

        if (_popupShowCount >= SHOW_POPUP_TIMES) {
			AskPermissionPopup::loadCancelButton([=] {
				//MJDEFAULT->setIntegerForKey(KEY_TIME_SKIP_RECORD, monkey_helper::utils::getTime());
				this->onFinishGame();
			});
		}
		return false;
	}

	return true;
}

void ProScore::pushStopCevertapEvent(){
    
    float completedLesson = ((float)_countTrackingEvents/(float) _listCard.size()) * 100.0f;
    
	int time = getTime();
    
	try
	{
		CleverTapManager::GetInstance()->pushEvent("stop_gameAI", {

				{"story_name",cocos2d::Value{_info.storyName}},
				{"story_id",cocos2d::Value(XHSLib->story_id)},
				{"stop_at",cocos2d::Value(_messengerForClevertap)},
				{"completed",cocos2d::Value(completedLesson)},
				{"time_spent",cocos2d::Value(time - _time)},
		});
	}
	catch (const std::exception&)
	{

	}
}
