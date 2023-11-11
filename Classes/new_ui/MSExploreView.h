//
//  MSExploreView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/10/17.
//

#ifndef MSExploreView_hpp
#define MSExploreView_hpp

#include "cocostudiomacro2.h"
#include "StoryDataModel.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"
#include "MSGradeBoxDelegate.h"
#include "MSGradeManager.h"
#include "PopularSearchTemplate.h"
#include "APPopupBase.h"

class MSComplexCollection;
class MJWorksheetNoResult;
class MSGradeBox;

class MSExploreView : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::GradeBox, public ms::delegate::Collection {
public:
    enum class CollectionType {
        NONE,           //
        SERIES,         // danh sách story theo chủ đề
        READED,         // danh sách story đã đọc
        DOWNLOADED,     // danh sách story đã tải
        FAVOURITE,      // danh sách story yêu thích
    };
    
    CREATE_FUNC(MSExploreView);
    static MSExploreView * createView(std::string name, MSExploreView::CollectionType i_collectionType, int i_gradeID, int i_categoryID = -1);
    void runAnimationAppear();
    void runAnimationDisappear();
    
    void setContentSize(const cocos2d::Size & size) override;
    void enableEditMode(std::string str = "favorite.edit");
    void onExit() override;
    void setTitle(const std::string& str_title);
    void hideSearchBar();

private:
    enum class CurrentSearchType {
        ORGANIC,        // kết quả lấy từ một từ khoá do user nhập
        RECENT,         // kết quả lấy từ một từ khoá của Recent Searches
        POPULAR,        // kết quả lấy từ một từ khoá của Popular Searches
    };

	~MSExploreView();

    void onEnter() override;
    void didLoadFromCSB(std::string name, MSExploreView::CollectionType i_collectionType, int i_gradeID, int i_categoryID);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    void onEdit(cocos2d::Ref * sender);
    void onClearSearch(cocos2d::Ref* sender);
    void showPanelNoResult(const std::string& keyword, std::string gradeName = "");
    void hidePanelNoResult();
    void reloadData();
    void searchText();
    void changeDisplayLanguage() override;
    
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget * cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    void removeCellAtPosition(int index);
    
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
    void onReceivedEventGradeChanged(int grade_id);

	void pushCleverTapTrackingData();
    
    void onSelectSearchField();

	void initPopularSearch();
    
    void hideVirtualKeyboard();
    
    void pushCleverTapEvent_searchEvent();

private:
    // for variables
    // ai thêm biến/đối tượng thì khai báo ở đây nhé, đừng viết loạn lên nữa =.=
    
    bool                                _is_editing = false;
    std::string                         _saved_title_edit;
    std::string                         _title;
    
    std::vector<mj::model::StoryInfo>   _filtered_stories;
    std::vector<cocos2d::ui::Widget *>  _ui_source;
    
    MSComplexCollection *               _collection = nullptr;
    MJWorksheetNoResult *               _panel_noResult = nullptr;
    MSGradeBox *                        _gradebox = nullptr;
    
    ui::TextField *                     m_searchText = nullptr; // Text nhập vào ô tìm kiếm.
    PopularSearchTemplate *             m_popularSearchTemplate = nullptr;
    ui::Button *                        m_button_clear_search = nullptr;
    
    mj::model::StoryInfo                m_clickedStoryInfo;
    ms::GradeManager::Scope             m_scope;
    MSExploreView::CurrentSearchType    m_curSearchType;
    MSExploreView::CollectionType       m_collectionType;
	std::string		                    m_itemName			= "none";
    int                                 m_categoryID        = -1; // nếu category id > 0 thì lấy danh sách truyện từ chủ đề
	int				                    m_itemID			= -1;
	bool			                    m_isItemSelected	= false;
    bool                                m_isSearchEnabled   = true; // nếu thanh search bị disable thì có thể là màn hình Downloaded Stories
    std::function<void(Ref * sender, ui::TextField::EventType event)> _textFieldListener = nullptr;
};

CREATE_READER(MSExploreView);
#endif /* MSExploreView_hpp */
