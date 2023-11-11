//
//  MSExploreView_Audiobooks.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/27/17.
//

#ifndef MSExploreView_Audiobooks_hpp
#define MSExploreView_Audiobooks_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"
#include "MSGradeBoxDelegate.h"
#include "MSGradeManager.h"
#include "PopularSearchTemplate.h"
#include "APPopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class MSComplexCollection;
class MJWorksheetNoResult;
class MSGradeBox;

class MSExploreView_Audiobooks : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::GradeBox, public ms::delegate::Collection {
public:
    enum class CollectionType {
        NONE,           //
        SERIES,         // danh sách audiobook theo chủ đề
        READED,         // danh sách audiobook đã đọc
        DOWNLOADED,     // danh sách audiobook đã tải
        FAVOURITE,      // danh sách audiobook yêu thíchA
    };
    
    enum class CurrentSearchType {
        ORGANIC,        // kết quả lấy từ một từ khoá do user nhập
        RECENT,         // kết quả lấy từ một từ khoá của Recent Searches
        POPULAR,        // kết quả lấy từ một từ khoá của Popular Searches
    };
    
    CREATE_FUNC(MSExploreView_Audiobooks);
    static MSExploreView_Audiobooks * createView(std::string name, MSExploreView_Audiobooks::CollectionType i_collectionType, int i_gradeId, const mj::model::AudiobookSeriesInfo& i_seriesInfo = {});
    void setTitle(const std::string& str_title);
    void hideSearchBar();
    void runAnimationAppear();
    void runAnimationDisappear();
    void setContentSize(const cocos2d::Size & sz) override;
    void enableEditMode(std::string str = "favorite.edit");
    void enablePlayAll(bool is_enabled);
    
private:
	~MSExploreView_Audiobooks();
    void onEnter() override;
    void onExit() override;
    void didLoadFromCSB(std::string name, MSExploreView_Audiobooks::CollectionType i_collectionType, int i_gradeId, const mj::model::AudiobookSeriesInfo& i_seriesInfo);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    void onEdit(cocos2d::Ref * sender);
    void onPlayAll(cocos2d::Ref * sender);
    void clearTextFeild(cocos2d::Ref* sender);
    void showPanelNoResult(const std::string& keyword);
    void hidePanelNoResult();
    void reloadData();
    void searchText();
    
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    void removeCellAtPosition(int idx);
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    void onReceivedEventGradeChanged(int grade_id);
	void pushCleverTapTrackingData();
	void initPopularSearch();
    void hideVirtualKeyboard();
    void pushCleverTapEvent_searchEvent();
private:
    // for variables
    void changeDisplayLanguage() override;
    std::vector<mj::model::AudiobookInfo>       _filtered_audiobooks;
    std::vector<cocos2d::ui::Widget *>          _ui_source;
    
    mj::model::AudiobookInfo                    m_clickedAudiobookInfo;
    ms::GradeManager::Scope                     m_scope;
    MSExploreView_Audiobooks::CurrentSearchType m_curSearchType;
    MSExploreView_Audiobooks::CollectionType    m_collectionType;
    mj::model::AudiobookSeriesInfo              m_seriesInfo;
    
    std::string     _title;
    std::string     _saved_edit_title;
    std::string     _readFrom = "";
    int             _current_grade_id;
    bool            _is_editing = false;
    
	std::string		m_itemName = "none";
	int				m_itemID = -1;
	bool			m_isItemSelected = false;
    bool            m_isSearchEnabled = true; // nếu thanh search bị disable thì có thể là màn hình Downloaded Stories
    
    ui::TextField *         textField = nullptr;
	PopularSearchTemplate * m_popularSearchTemplate = nullptr;
    ui::Button *            m_button_clear_search = nullptr;
    MSComplexCollection *   _collection = nullptr;
    MJWorksheetNoResult *   _panel_noResult = nullptr;
    MSGradeBox *            _gradebox = nullptr;
    
    CC_SYNTHESIZE(std::string, _series_name, SeriesName);
};

CREATE_READER(MSExploreView_Audiobooks);

#endif /* MSExploreView_Audiobooks_hpp */
