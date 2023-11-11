
#ifndef MP_TAB_LESSONS_H__
#define MP_TAB_LESSONS_H__

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MPTabLessonsDelegate {
public :
	virtual void onClickedLessonDelegate(mj::model::LessonInfo lesson) {};
};

class MPTabLessons : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol   {
public:
    CREATE_FUNC(MPTabLessons);
    static MPTabLessons * createTab();
	std::vector<mj::model::LessonInfo> getAllLessonsByGrade(int grade_id);
	std::vector<mj::model::LessonInfo> getAllLessons();
	void setDelegate(MPTabLessonsDelegate * delegate);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
	void onClickedGrade(cocos2d::Ref * ref);
protected:

	int numberOfCells();
	int numberOfColumns();
	cocos2d::Size collectionSize();
	cocos2d::Size modelSize();
	double percentScrollWhenReload();
	cocos2d::ui::Widget * cellForPosition(int row, int col);
	MPTabLessonsDelegate * _delegate;
private:
	void fetchLessonsFromAPI(std::function<void(bool)> callback = nullptr);
	void fetchCategories();
    void didLoadFromCSB();
    void onEnter() override;
    
	std::vector<mj::model::LessonInfo> _all_lessons;
    std::vector<mj::model::LessonInfo> _lessons;
	//std::vector<mj::model::CategoryInfo> _category;
	int m_currentCategory = 0;
	//cocos2d::ui::ScrollView * _collection;

    void onLearnLesson(mj::model::LessonInfo lesson);
    
	void filterLessons(int category_id);
    void reloadData();
    void onBackToLesson();
    
    int _saved_current_row = 0;
};

CREATE_READER(MPTabLessons);

#endif /* MP_TAB_LESSONS_H__ */
