#ifndef MJ_WORKSHEET
#define MJ_WORKSHEET

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJWorksheetItem.h"
#include "MSPopularSearchItem.h"
#include "MSCollectionDataSource.h"
#include "MSGradeBoxDelegate.h"
#include "MSGradeManager.h"
#include "MSGradeBox.h"
#include "MSMaterialEnterEmail.h"
#include "StoryDataModel.h"
#include "PopularSearchTemplate.h"
#include "MaterialPopup.h"

class MSComplexCollection;
class MJWorksheetNoResult;

using namespace cocos2d;
using namespace cocos2d::ui;

class MJWorksheet : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource, public ms::delegate::GradeBox, public MJWorksheetItemDelegate, public MSMaterialEnterEmailDelegate,public MaterialPopupDelegate {
public:
    enum class Page {
        NONE,
        DEFAULT,
        SEARCH_DEFAULT,
        SEARCH_KEYWORD,
    };
    
    enum class CurrentSearchType {
        ORGANIC,        // kết quả lấy từ một từ khoá do user nhập
        RECENT,         // kết quả lấy từ một từ khoá của Recent Searches
        POPULAR,        // kết quả lấy từ một từ khoá của Popular Searches
    };
    
    CREATE_FUNC(MJWorksheet);
	static MJWorksheet* createView(mj::model::MaterialInfo::Type itemType);

	void setContentSize(const cocos2d::Size& size) override;
    
    void tickWorksheetItems(const std::string& selected_ids, int grade_id);
    
private:
        
    // MSMaterialEnterEmailDelegate
    mj::model::MaterialInfo::Type fromMaterialScreen() override {return _itemType;};
    std::string getStringMaterialIds() override;
    void onMaterialEmailSent() override;
    void pushEventNotation() override;
    
    // MJWorksheetItemDelegate
    int getSelectedWorksheetNumber() override {return (int)_selected_items.size();};
    void onWorksheetItemClicked(cocos2d::Ref * ref) override;
    std::string getGradeNameByGradeId(int gradeId) override;
    
    // CollectionDataSource
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    
    // GradeBox
    bool isGradeBoxFromMaterialScreen() override {return true;} // phải có hàm này và trả về true nhé, để gradebox còn biết là màn hình worksheet để gửi event CleverTap cho đúng
    void onSelectGradeBox() override {};
    void onDeselectGradeBox(int grade_id) override {};
    
    void onBackWarningClose1() override;

    // Zzz
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& name) override;
    
    void initPopularSearch();
    
    bool isWorksheetItemSent(int mat_id);
    
	void didLoadFromCSB(mj::model::MaterialInfo::Type itemType);
    
	void hideVirtualKeyboard();
    
    void showPanelNoResult(const std::string& keyword);
    
    void hidePanelNoResult();
    
    void onReceivedEventGradeChanged(int grade_id);
    
    void resetSelectedWorksheetItems();
    
    void resetSelectedWorksheetNumber(float durationForChangeButton = -1.f);
    
    void reloadData();

    void changeDisplayLanguage() override;
    
    // CleverTap events
    void pushCleverTapEvent_screenChooseType(const std::string& type, const std::string& action, int selected_number);
    void pushCleverTapEvent_search(const std::string& type, const std::string& search_type, const std::string& value);
    void pushCleverTapEvent_searchEvent();
    void pushCleverTapEvent_buttonSendEmail(bool is_selected);
    void pushCleverTapEvent_notation();

    // Pages
    void changeToPageDefault();
    void changeToPageSearch();
    void changeToPageSearchWithKeyword();
    
    // Buttons
    void onAttachSearchField(cocos2d::Ref* sender);
	void onBack(cocos2d::Ref* sender);
	void onClearText(cocos2d::Ref* sender);
    void onSendEmail(cocos2d::Ref * sender);
    void onClickInfo(cocos2d::Ref * sender);
	
    // Get/Set Data
    void resetDataRoot();
    void resetDataMaterials(const std::string& text_search = "");
    std::vector<int> getWorksheetItemSent();
    void pushNewWorksheetItemSent();
    
    void onExit() override;

	cocos2d::ui::TextField *        _searchTextField = nullptr;
	MSComplexCollection *           _searchResult = nullptr;
    PopularSearchTemplate *         _popularSearchTemplate = nullptr;
    MJWorksheetNoResult *           _panel_noResult = nullptr;          // panel thông báo không có kết quả search
    ui::ImageView *                 _round_number = nullptr;            // text: số học liệu người dùng chọn
    ui::ImageView *                 _panel_info = nullptr;              // panel thông tin về các icon readed và downloaded
    ui::Button *                    _button_sendemail = nullptr;        // button SendEmail
    ui::Button *                    _button_clear = nullptr;            // button clear textfield
	MSGradeBox *                    _gradeBox = nullptr;
    
    mj::model::MaterialInfo::Type               _itemType;              //
    MJWorksheet::Page                           _page;                  // trang mặc định hoặc trang tìm kiếm
    MJWorksheet::CurrentSearchType              _curSearchType;         //
    ms::GradeManager::Scope                     _scope;                 //
    int                                         _gradeID = -1;          //
    int                                         _notationCounter = 0;   //Số lần bấm vào chú thích
    std::string                                 _str_materialIds;       // chuỗi id các worksheet item được chọn gửi, set khi bấm nút Send Email
    std::vector<int>                            _data_sent;             // danh sách id các học liệu đã được gửi về email
    std::vector<mj::model::MaterialInfo>        _data_root;             // danh sách học liệu gốc ứng với: type và grade
    std::vector<mj::model::MaterialInfo>        _data;                  // danh sách học liệu được hiển thị
	std::vector<MJWorksheetItem *>              _ui_source;             //
    std::vector<MJWorksheetItem *>              _selected_items;        // danh sách học liệu được chọn
    std::vector<mj::model::GradeInfo>           _gradeInfos;            // danh sách grade      - phần quản lý grade(level) có lỗi nên phải lưu 2 vector infos và names
    std::vector<std::string>                    _gradeNames;            // danh sách tên grade  - phần quản lý grade(level) có lỗi nên phải lưu 2 vector infos và names
    
    bool                                        _isReloadDataCalledFromOnEnterFucntion; //
    int                                         _clickedCounter_button_sendEmail = 0;   // số lần bấm nút gửi học liệu (Send Email)
};

CREATE_READER(MJWorksheet);
#endif
