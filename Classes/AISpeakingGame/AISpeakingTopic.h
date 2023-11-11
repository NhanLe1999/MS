///*
/////////////////////////////////////////////////////////////////////////
////  AISpeakingTopic_hpp
////  Implementation of the Class AISpeakingTopic_hpp
////  Created on:      21-04-2022
////  Original author: TruongBX
/////////////////////////////////////////////////////////////
//
//
//#ifndef AISpeakingTopic_hpp
//#define AISpeakingTopic_hpp
//#include <stdio.h>
//#include <string>
//#include <vector>
//#include <stdio.h>
//#include <string>
//#include <vector>
//#include "AISpeakingTopic.h"
//#include "cocostudiomacro2.h"
//#include "MSCollectionDelegate.h"
//#include "ui/CocosGui.h"
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "MJDefinitions.h"
//#include "RemoteImageView.h"
//#include "MJAPI_FirstInstall_Thumb_Zip.h"
//#include "MSCollectionAiSpeakingLessonItem.h"
//#include "MSLessonAISpeakingManager.h"
//#include "APProfileManager.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#include "MSPreActivity.h"
//#include "StoryAlert.h"
//#include "MSCollectionDataSource.h"
//#include "MSCollectionDelegate.h"
//
//USING_NS_CC;
//#define DELAY_CREAT_LESSON 0.05
//class AISpeakingTopic : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol,
//	public ms::delegate::CollectionDataSource, public ms::delegate::Collection,
//	public ms::delegate::CollectionAISpeakingLessonItem, public ms::delegate::PreActivity, public StoryAlertDelegate
//{
//
//public:
//	CREATE_FUNC(AISpeakingTopic);
//	AISpeakingTopic();
//	virtual ~AISpeakingTopic();
//	static AISpeakingTopic* createTab(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo, std::vector<mj::model::AISpeakingLessonInfo> aiSpeakingLessons, std::vector < int >lessonIdsLearnedFromProfile, bool isLockTopic = false);
//
//	int getLearnedProccessing();
//	std::vector<int> getSpeakingIDList();
//	std::string getThumbPath();
//	std::string getTopicColorCode();
//	int getTopicId();
//	std::string getTopicName();
//	int getTopicOrder();
//	void setLearnedProccessing(int learnedProccessing);
//	void setSpeakingIDList(std::vector<int> speakingIDList);
//	void setThumbPath(std::string thumbPath);
//	void setTopicColorCode(std::string topicColorCode);
//	void setTopicId(int topicId);
//	void setTopicName(std::string topicName);
//	void setTopicOrder(int topicOrder);
//	void setNumberLessonLearned(int  numberLessonLearned);
//	int getNumberLessonLearned();
//	void clearUnusedContents();
//	void SetInfoViewTopic(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo);
//	void setAvataTopicInfo(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo);
//	void setUpListView();
//	void selectedTopicItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
//	void onDowloadAvata(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo);
//	ListView* getListViewLesson() {
//		return _speakingItemsList;
//	}
//	mj::model::AISpeakingCategoryInfo getCategoryInfo() {
//		return _aISpeakingCategoryInfo;
//	}
//	int getLessonSize() {
//		return _aiSpeakingLessons.size();
//	}
//	void showPermissionPopup();
//	void onUpdateTopicProccess() override;
//    void setLockTopic(bool isLockTopic){
//        _lockTopic = isLockTopic;
//    };
//    bool getLockTopic(){
//        return _lockTopic;
//    };
//private:
//	void didLoadFromCSB(mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo, std::vector<mj::model::AISpeakingLessonInfo> aiSpeakingLessons, std::vector < int >lessonIdsLearnedFromProfile, bool isLockTopic = false);
//	void onSelectCollectionItem(cocos2d::ui::Widget* item) override;
//	void reloadData();
//	void changeTextDisplayLang();
//	void setTopicState();
//	void setUpdateTopicProcess(std::string cirlceProccess, int percent);
//	cocos2d::Size collectionSize() override;
//	void offLoadCellAt(int index) override;
//	void onLearnLesson(mj::model::AISpeakingLessonInfo lesson) override;
//	CC_SYNTHESIZE(ms::delegate::CollectionAISpeakingLessonItem*, _delegate, Delegate);
//    CC_SYNTHESIZE(bool, _isTopicLearned, IsTopicLearned);
//	bool IsTopicThumbExits(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo);
//	bool checkLessonLearned(int id, std::vector<int> listIdOfLessonsLearned);
//	vector<int>getLessonLearnedfromProfile();
//	void setLessonLearedfromProfile(vector<int> lessonLeared);
//    void mouseScroll(std::string listviewName = "", ListView * listView = nullptr, float percent = 10, float timeInSec = 1.5, bool attenuated = true);
//    void onExit() override;
//
//	std::vector<int> aiSpeakingItemsIDList;
//	mj::model::AISpeakingCategoryInfo _aISpeakingCategoryInfo;
//	std::string aiSpeakingTopicName;
//	int aiSpeakingTopicOrder;
//	int alSpeakingTopicId;
//	*/
///**
//	 * learned processing of topic. (Unit in percent)
//	 *//*
//
//	int _learnProccessing = 0;
//	*/
///**
//	 * icon color of topic in hex code
//	 *//*
//
//	std::string topicColorCode, _scheduleName = "waitingDownloadIcon";
//	std::string cirlceProccessImageName;
//	std::string topicThumbPath;
//	cocos2d::ui::ListView* _speakingItemsList = nullptr;
//	cocos2d::ui::Text* _title = nullptr;
//	cocos2d::ui::Layout* _panel_avata = nullptr, *_cirlceProccessLayout = nullptr, *_whiteLayout = nullptr, *_greenLayout = nullptr, * _avataIconLayout = nullptr;
//	RemoteImageView* _panel_avata_img = nullptr, * _grayImg = nullptr, * _whiteImg = nullptr, * _selectedCircleImage = nullptr, * _avataIconImg = nullptr;
//	EventListenerMouse* mouseListener = nullptr;
//	EventListenerTouchOneByOne* touchListener = nullptr;
//	std::vector<mj::model::AISpeakingLessonInfo> _aiSpeakingLessons;
//	int _index = 0;
//	std::vector < int >_lessonIdsLearnedFromProfile;
//	std::vector<MSCollectionAiSpeakingLessonItem*>_MSCollectionAiSpeakingLessonItems;
//	ProgressTimer* _progcess = nullptr;
//	int _numberLessonLearned = 0;
//    bool _isExitTopic = false, _lockTopic = false, _isDownloadTopicIconSuceesss = false;
//    int _countForUserExpectedInOneTopic = 0; // using to limit number of free lesson in each topic
//
//};
//CREATE_READER(AISpeakingTopic);
//
//#endif // defined(AISpeakingTopic_hpp)
//*/
