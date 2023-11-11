#pragma once
#ifndef __MK_GAME_PRO_SCRE_H__
#define __MK_GAME_PRO_SCRE_H__
#include "MSRecord.h"
#include "network/HttpClient.h"
#include "ScoreBoardDetail.h"
#include "SpineWrapper.h"
#define FONT_NAME "fonts/font_game_0.ttf"
#include "MJBaseActivity.h"
#define PRO_SOCRE_TAG 3988892
namespace model {
	struct Image {
		int id;
		std::string path = "";
		std::string image_type;
		int imageCategoriesId;
	};

	struct Audio {
		int id;
		std::string path;
		float duration;
		std::vector<math::resource::Sync_Text> sync_data;
		cocos2d::ValueVector sync_data_phonic;
		int voice_id;
	};

	struct SyncTextPhonic {
		int hide;
		float time;

	};
	struct AudioPhonic {
		std::vector <std::string> audios_path;
		std::vector <float> audios_duration;
		cocos2d::ValueVector sync_data;
		//std::vector <SyncDataItem> sync_data;
	};

	struct Video {
		int id;
		std::string path;
		float duration;
		bool is_white_background;
		int videoType;
	};

	struct Video_FilterText {
		int id;
		std::string path;
		float duration;
		std::string text;
		std::string text_id;
		std::vector<Audio> audios;
	};
	struct FilterText { // text filter for sentence
		int id;
		std::string text;
		std::vector<Image> images;
		std::vector<Video_FilterText> videos;
		std::vector<Audio> audios;
	};

	class Card {
	public:
		Card() {
			id = -1;
		};
		~Card() {};
		int id;
		std::string text;
		cocos2d::Color3B color;
		cocos2d::Color3B color_hightlight;
		std::vector<Image> images;
		std::vector<Audio> audios;
		std::vector<Audio> audio_effects;
		std::vector<Audio> audio_hightlight;
		std::vector<Audio> audio_phonic_slow;
		std::vector<Audio> audio_phonic_normal;
		std::vector<Audio> audio_phonic_synctext;
		std::vector<Video> videos;
		std::vector<FilterText> filter_words; // dung cho sentence
		std::vector<AudioPhonic> phonic_audios; // dung cho phonic
		std::string name_display; // dung cho phonic
		cocos2d::ValueVector highlight;
		std::vector<int> _listGameLocked;
	};
	class CardHelper {
	public:
		static Card createCard(std::string word_id);
		static std::vector<Card> pool_cards;
		static model::Image getImageByType(Card card, int idImageCategories);
		static model::Image getImageMultisensoryByType(Card card, int idImageCategories);
		static model::Audio getAudioCard(Card card);
		static model::Video getVideoCard(Card card);
		static std::map<int, int> lastAudioIndex;

		//        static std::map<int, std::map<int, int>> lastImageIndexCard;
		//
		//        static std::map<int, int> lastVideoIndex;

		static model::Video getVideoMultisensoryCard(Card card);
		static Card         getCardMultisensory(int idMainCard, std::vector<Card> cardSentence);
		static Card         getCardSentence(int idMainCard, std::vector<Card> cardSentence);

	};
}

struct ResponseData{
    std::string status;
    std::string message;
    int score;
    std::string sentence;
	std::vector<std::pair<std::string, float>> wordScore;
    ResponseData(){};
};

class ProScore : public MSRecordDelegate, public ScoreBoardDetailDelegate ,  public ms::BaseActivity {

public:

	enum GameType {
		WORD,
		SENTENCE
	};

	static const int gameId;
    
	static const int gameSentenceId;

	CREATE_FUNC(ProScore);
	
	static ProScore* createGame(std::vector<math::resource::Image*> images);

	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);

	void didLoadFromCSB(std::vector<math::resource::Image*> images);

    void onRequestSucceedElsa(std::string responseData);
    void onRequestSucceedGoogle(std::string responseData);
    void onConnectionError();
	void acceptPermissionCallback();
	void denyPermissioCallback();
	void showSkipButton();
	virtual void replayAudio() override;
	virtual void onSoundButtonClicked(Ref* sender);
	void pushFireBaseEventGameAI();
	long long getTime();
	int _time = 0;
	void onFinishRecordGoogle(int finish_type, std::string url, std::string messenge);
    string getLastWordInString(string str, string delimiter);
	bool _wasStopRecord = false;
	std::string _messengerForClevertap;
	int _countTrackingEvents = 0;


private:
    void onEnter() override;
    void onExit() override;
    ~ProScore();
	void loadUI();
	void loadRecordButton();
	void showRecordButton();
	void onRecordButtonClicked(Ref*);
	void onRecordButtonClickedElsa(Ref*);
	void onStopRecord(Ref*);
	void onFinishRecord(int finish_type, std::string url) override;
	void addAnalyzeLayout();
    void removeAnalyzeLayout();
	void onAnalyzeDone(int score, std::vector<std::pair<std::string, float>> wordScore);
	void nextGameButtonClicked();

	void doAutoTest();
	void fakeData(ResponseData &data);

	bool _enableLower = true;
	std::vector<std::pair<std::string, int>> _parmas;

	void highlightText(std::function<void(int auId, std::string auPath)> onHiglightDone = nullptr);

	std::string scoreToMessage(int score);
	Color3B scoreToColor(int score);
	std::string scoreToEmojiAction(int score);
	std::string messageToAudioPath(std::string message);
	std::string scoreToAudioRate(int score);

	void clearStage(std::function<void()> callback=nullptr);
    void sendRequest(std::string url);
	void sendRequestElsa(std::string url);
	std::vector<std::pair<std::string, std::string>> buildRequestBody();
    ResponseData extractData(std::string data);
	void presentScore(float totalScore, std::vector<std::pair<std::string, float>> wordScore);
	void highlightWordScore(std::vector<std::pair<std::string, float>> wordScore);
    void highlightWordScoreGoogle(std::vector<std::pair<std::string, float>> wordScore);
    void createScoreBoard(int score,std::string customMessage = "");
	std::string simplifyText(std::string msg);
	void showScoreBoardDetails(Ref* sender, ResponseData textData);
    
	void onFinishGame();
    void showPopupAskPermission(int permissionStatus);
	void addTextTouchArea();
	void loadTextScoreData();
    void addSpeaker();
	void onCloseGame() override;
	bool requestRecordPermistion();
	void pushStopCevertapEvent();
    void setCallbackCickbutton();

	std::vector<model::Card> _listCard;
	ImageView* _imageObject;
	SpineWrapper* _recordButton;
	SpineWrapper* _stopRecordButton;
	ImageView* _nextGameButton;
	Button* _replayButton;//ear button
    Button* _soundButton = nullptr;//ear button
    
    std::function<void(std::string)> _callbackCliclbutton = nullptr;

	Layout* _scoreLayout;
	MSRecord* _record;
	Text* _textObject;
	std::vector<Node*> _vectorLine;//for remove
	int _currentTurn = 0;
    float _currentStepCount;
	int _score;
	network::HttpRequest* _request;
	bool _isCallApi;
	bool _firstTy;
    int _idAudioQuestion = -1;
    float _maxDelayTimeRecord = 10;

	std::vector<std::string> MSG_VERRY_BAD_FIRST_TIME = { 
		"Oops! Try again!","Try that again!",
		"Let's try that again!" 
	};
	std::vector<std::string> MSG_VERRY_BAD_SECOND_TIME = { 
		"Good try! Let's do it better next  time!",
		"Don't worry! Try next time!",
		"Nice try! Try to do better next time!",
		"Nice try! Try to practice more!",
		"Good try! Let's do it better next  time!",
		"Good try! Try to do better next time!"
	};
	std::vector<std::string> MSG_MEDIUM = {
		"Well done! Let's do it better next time!",
		"Well done! Try to practice more!",
		"Well done! Try to do better next time!"
	};
	std::vector<std::string> MSG_PRETTY_GOOD = {
		"Bravo! Almost correct!",
		"Good job! Almost correct!",
		"Well done! Almost correct!",
		"Great job! Almost correct!"
	};
	std::vector<std::string> MSG_GOOD = {
		"Fantastic!",
		"Wonderful!",
		"Amazing!",
		"Perfect!",
		"Awesome!",
		"Excellent!"
	};
	std::string _recordPath = "";
	bool _connectionErrorPopupShowed;
	int _retryCount = 0;
	ResponseData _responseData;

    bool _isHighlightDataValid;
	std::vector<Layout*> _listTextTouchArea;
	std::vector<ResponseData> _listTextResponseData;

	int _popupShowCount;

	bool _usingElsaAPI = false;

	GameType _gameType;

	cocos2d::Size _screenSize;

	cocos2d::Vec2 _origin;
};
#endif
